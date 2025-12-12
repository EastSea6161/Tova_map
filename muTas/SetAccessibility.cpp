// SetAccessibility.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SetAccessibility.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Target.h"
#include "ImChampDir.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "WriteCodeAgrument.h"
#include "Launcher.h"
#include "DefineNamesDefaultPara.h"

enum
{
	_0_COLUMN_CLASS_NAME = 0,
	_1_COLUMN_TIME
};

// KSetAccessibility 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSetAccessibility, KDialogEx)

KSetAccessibility::KSetAccessibility(KProject* a_pProject, KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KSetAccessibility::IDD, pParent)
	,m_pProject(a_pProject)
	,m_pTarget(a_pTarget)
	,m_bRunResult(false)
	,m_strOutputFolerPath(_T(""))
	,m_strRunErrMsg(_T(""))
	,m_strBoundaryDistance(_T("1"))
{

}

KSetAccessibility::~KSetAccessibility()
{
}

void KSetAccessibility::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO2, m_cboClass);
}


BEGIN_MESSAGE_MAP(KSetAccessibility, KDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO2, &KSetAccessibility::OnCbnSelchangeComboClass)
	ON_BN_CLICKED(IDOK,          &KSetAccessibility::OnBnClickedButtonRun)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT, &KSetAccessibility::OnReportItemValueChanged)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTONDEFAULT, &KSetAccessibility::OnBnClickedDefault)
END_MESSAGE_MAP()


// KSetAccessibility 메시지 처리기입니다.

BOOL KSetAccessibility::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		CString strValue = m_strBoundaryDistance;
		double  dValue   = _ttof(strValue);
		
		strValue.Format(_T("%.2f"), dValue);

		SetDlgItemText(IDC_EDIT_DISTANCE, strValue);

		std::vector<KNodeTypeInfo>    vecNodeTypeInfo;
		LoadNodeType(vecNodeTypeInfo);


		//Using Region Code는 다음의 프로세스를 거침
		//1. node의 존ID 컬럼에 정보가 하나도 없을시는 비활성화
		//2. 1의 경우가 아닐경우에는 Default에서 설정한 값을 적용함
		if (UsingCheckZoneID())
		{
			GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		}

        //★ 클래스 구분 코드, 거리, 지역코드, 클래스 갯수
		KDBaseDefaultParameter::LoadDefaultAccessSetting(m_mapDefaultAccessSetting);

        //★ 클래스 정보 리스트
		KDBaseDefaultParameter::LoadDefaultAccessClassInfo(m_mapDefaultAccessClassInfo);
		
        //★ A, B, C, D, E, F, G, H, I, J 단계(10단계)
        KDBaseDefaultParameter::SetAccessibility(m_mapAccessibility);

		KReportCtrlSetting::DefaultC(m_wndReportCtrl, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);

		InitReportHeader();

		AutoType find = m_mapDefaultAccessClassInfo.find(1);
		AutoType end = m_mapDefaultAccessClassInfo.end();

		if(find != end)
		{
			AutoType iter = m_mapDefaultAccessClassInfo.find(1);
			std::vector<TAccessClassInfo> vecAccessClassInfo = iter->second;
			int nUserCodeCount = vecAccessClassInfo.size();

			InitComboClass((int)nUserCodeCount);
			InitDefaultSetting();
			InitDefaultReportRecord();
		}
		else 
		{
			AutoType iter = m_mapDefaultAccessClassInfo.find(0);
			std::vector<TAccessClassInfo> vecAccessClassInfo = iter->second;
			int nUserCodeCount = vecAccessClassInfo.size();

			InitComboClass((int)nUserCodeCount);
			InitSetting();
			InitReportRecord();
		}

		m_strOutputFolerPath = ConvertSavePath(_T(""));	
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		KDialogEx::OnCancel();
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		KDialogEx::OnCancel();
	}
	return TRUE;  
}

void KSetAccessibility::LoadNodeType(std::vector<KNodeTypeInfo> &a_vecNodeTypeInfo)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(	_T(" SELECT TBL2.node_type AS node_type, TBL1.KCode_Display AS KCode_Display ")
			_T(" FROM ")
			_T(" (SELECT node_type FROM node GROUP BY node_type)TBL2 ")
			_T(" LEFT OUTER JOIN ")
			_T(" (SELECT KCode_Key, KCode_Display FROM Kcode_Define WHERE KCode_Group_Key = %d)TBL1 ")
			_T(" ON TBL1.KCode_Key = TBL2.node_type "), CODE_GROUP_KEY_NODE         
			);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		while( pResult->Next() )
		{
			KNodeTypeInfo oKNodeTypeInfo;

			oKNodeTypeInfo.nNodeType          = pResult->GetValueInt   (0);
			oKNodeTypeInfo.strCodeDisplay     = pResult->GetValueString(1);

			a_vecNodeTypeInfo.push_back(oKNodeTypeInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

void KSetAccessibility::LoadZoneName(std::map<Integer, CString> &a_mapZoneName)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	a_mapZoneName.clear();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" SELECT zone_id, full_name FROM %s "), TABLE_ZONE);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			Integer nxZoneID      = pResult->GetValueInt64  (0);
			CString strFullName   = pResult->GetValueString (1);

			a_mapZoneName.insert(std::make_pair(nxZoneID, strFullName));
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KSetAccessibility::LoadTransitName(std::map<Integer, CString> &a_mapTransitName)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	a_mapTransitName.clear();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" SELECT transit_id, name FROM %s "), TABLE_TRANSIT);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			Integer nxTransitID      = pResult->GetValueInt64  (0);
			CString strName          = pResult->GetValueString (1);

			a_mapTransitName.insert(std::make_pair(nxTransitID, strName));
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}

void KSetAccessibility::InitComboClass(int a_nInitGrade)
{
	m_cboClass.ResetContent();

	CString strClass(_T(""));
	int     nInitCur = 0;  

	for (int i= 0; i< KAccessStationDefine::ACCESS_ALL_CLASS_COUNT; i++)
	{
		int nClass = (i+1);
		strClass.Format(_T("%d"), nClass);

		int nCur   = m_cboClass.AddString(strClass);
		
		m_cboClass.SetItemData( nCur, (DWORD_PTR)nClass );

		if( nClass == a_nInitGrade )
		{
			nInitCur = nCur;
		}
	}
	m_cboClass.SetCurSel(nInitCur);
}

void KSetAccessibility::InitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CLASS_NAME, _T("Class"),         50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("등급"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_TIME,       _T("Time(min)"),     50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도보접근시간(분)"));
    }
	
	m_wndReportCtrl.Populate();	  
}

void KSetAccessibility::InitReportRecord()
{
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;
		int					    nClassTime		 = KAccessStationDefine::ACCESS_INIT_TIME;
		int                     dTime		     = 0;
		int					    nUserDefineFirst = 0;

		AutoType iter = m_mapDefaultAccessClassInfo.find(0);
		std::vector<TAccessClassInfo>& vecAccessClassInfo = iter->second;
		size_t							   nUserCount	  = vecAccessClassInfo.size();

		for (size_t i = 0; i < nUserCount; i++)
		{
			TAccessClassInfo oinfo = vecAccessClassInfo[i];
			int Time			   = oinfo.nTimeMin;
			CString strName		   = oinfo.strClassName;


			int nUserCountCombo = 0;
			nUserCountCombo++;

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strName));
			pItem ->SetItemData((DWORD_PTR) (nUserCountCombo+1));
			pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(Time, _T("%.0f")));
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(TRUE);

		}
		m_wndReportCtrl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}	
}

void KSetAccessibility::UpdateReportRecord( void )
{
	UpdateData(TRUE);
	m_wndReportCtrl.ResetContent(TRUE);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	double                dTime   = KAccessStationDefine::ACCESS_INIT_TIME;

	int nSelCur = m_cboClass.GetCurSel();
	int nGrade  = (int)(m_cboClass.GetItemData(nSelCur));

	std::map<int, CString>::iterator iter  = m_mapAccessibility.begin();
	std::map<int, CString>::iterator itEnd = m_mapAccessibility.end();

	for (int i= 0; i< nGrade; i++)
	{
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		CString strClassName(_T(""));
		iter    = m_mapAccessibility.find(i+1);
		if(iter != itEnd)
		{	
			strClassName = iter->second;
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strClassName));
		pItem->SetItemData((DWORD_PTR) (i+1));
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetAlignment(xtpColumnTextCenter);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dTime, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextCenter);

		dTime += KAccessStationDefine::ACCESS_INCREASE_TIME;
	}

	m_wndReportCtrl.Populate();
}

void KSetAccessibility::InitSetting()
{
	try
	{
		AutoType iter = m_mapDefaultAccessSetting.find(0);
		std::vector<KAccessStationSetting>& vecAccessSetting = iter->second;

		KAccessStationSetting oAccessSetting = vecAccessSetting[0];
		double dDistance                     = oAccessSetting.dDistance;
		int    RegionCode			         = oAccessSetting.nRegionCode;

		CString strValue;
		strValue.Format(_T("%.2f"), dDistance);

		SetDlgItemText(IDC_EDIT_DISTANCE, strValue);

		if ( GetDlgItem(IDC_CHECK1)->IsWindowEnabled() && 1 == RegionCode)
		{
			CheckDlgButton(IDC_CHECK1, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK1, FALSE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KSetAccessibility::InitDefaultSetting()
{
	try
	{
		AutoType iter = m_mapDefaultAccessSetting.find(1);
		std::vector<KAccessStationSetting>& vecAccessSetting = iter->second;

		KAccessStationSetting oAccessSetting = vecAccessSetting[0];
		double dDistance                     = oAccessSetting.dDistance;
		int    RegionCode                    = oAccessSetting.nRegionCode;

		CString strValue;
		strValue.Format(_T("%.2f"), dDistance);

		SetDlgItemText(IDC_EDIT_DISTANCE, strValue);

		if ( GetDlgItem(IDC_CHECK1)->IsWindowEnabled() && 1 == RegionCode)
		{
			CheckDlgButton(IDC_CHECK1, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK1, FALSE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KSetAccessibility::InitDefaultReportRecord()
{
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;
		int					    nClassTime		 = KAccessStationDefine::ACCESS_INIT_TIME;
		int                     dTime		     = 0;
		int					    nUserDefineFirst = 0;

		AutoType iter = m_mapDefaultAccessClassInfo.find(1);
		std::vector<TAccessClassInfo>& vecAccessClassInfo = iter->second;
		size_t							   nUserCount		  = vecAccessClassInfo.size();

		for (size_t i = 0; i < nUserCount; i++)
		{
			TAccessClassInfo oinfo = vecAccessClassInfo[i];
			int Time			   = oinfo.nTimeMin;
			CString strName		   = oinfo.strClassName;


			int nUserCountCombo = 0;
			nUserCountCombo++;

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strName));
			pItem ->SetItemData((DWORD_PTR) (nUserCountCombo+1));
			pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(Time, _T("%.0f")));
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(TRUE);

		}
		m_wndReportCtrl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
	}
}

void KSetAccessibility::ResortReportRecord(std::map<int, int> &a_mapTime)
{
	m_wndReportCtrl.ResetContent(TRUE);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	std::map<int, int>::iterator iter  = a_mapTime.begin();
	std::map<int, int>::iterator itEnd = a_mapTime.end();

	TCHAR cFirst    = KAccessStationDefine::ACCESS_CLASS_INIT_CHAR;
	int   nClass    = 1;

	while (iter != itEnd)
	{
		int nTimeMin = iter->first;
		int nSameCnt = iter->second;

		for (int k= 0; k< nSameCnt; k++)
		{
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			CString strClassName = (CString)cFirst++;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strClassName));
			pItem->SetItemData((DWORD_PTR) nClass);
			pItem->SetAlignment(xtpColumnTextCenter);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(double(nTimeMin), _T("%.0f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			
			nClass++;
		}

		++iter;
	}

	m_wndReportCtrl.Populate();
}


void KSetAccessibility::OnCbnSelchangeComboClass()
{
	UpdateReportRecord();
}


void KSetAccessibility::OnReportItemValueChanged( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nColumnIndex        = pItemNotify->pColumn->GetItemIndex();
	int nRowIndexChanged    = pItemNotify->pRow->GetIndex();

	if ( _1_COLUMN_TIME == nColumnIndex )
	{
		CXTPReportRecord*           pRecordChanged   = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemNumber* pItemNumChanged  = (CXTPReportRecordItemNumber*)pRecordChanged->GetItem(_1_COLUMN_TIME);
		int nValueChaned     = (int)pItemNumChanged->GetValue();

		int nTotalRowCount = m_wndReportCtrl.GetRows()->GetCount();
		for(int i= 0; i< nTotalRowCount; ++i)
		{
			CXTPReportRow* pRow       = m_wndReportCtrl.GetRows()->GetAt(i);
			int            nRowIdx    = pRow->GetIndex();

			CXTPReportRecord*           pRecord    = pRow->GetRecord();
			CXTPReportRecordItem*       pItem      = pRecord->GetItem(_0_COLUMN_CLASS_NAME);
			CXTPReportRecordItemNumber* pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_TIME);
			int nValue           = (int)pItemNum->GetValue();

			if (nRowIdx < nRowIndexChanged)
			{
				if(nValue > nValueChaned)
				{
					pItemNum->SetValue((double)nValueChaned);
				}
			}
			if (nRowIdx > nRowIndexChanged)
			{
				if(nValue < nValueChaned)
				{
					pItemNum->SetValue((double)nValueChaned);
				}
			}
		}

		m_wndReportCtrl.RedrawControl();
	}

#pragma region OLD
// 	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
// 
// 	if (!pItemNotify->pRow || !pItemNotify->pColumn)
// 		return;
// 
// 	int nColumnIndex        = pItemNotify->pColumn->GetItemIndex();
// 	int nRowIndexChanged    = pItemNotify->pRow->GetIndex();
// 
// 	if ( _1_COLUMN_TIME == nColumnIndex )
// 	{
// 		CXTPReportRecord*           pRecordChanged   = pItemNotify->pItem->GetRecord();
// 		CXTPReportRecordItemNumber* pItemNumChanged  = (CXTPReportRecordItemNumber*)pRecordChanged->GetItem(_1_COLUMN_TIME);
// 		int nValueChaned     = (int)pItemNumChanged->GetValue();
// 
// 		CString strMsg(_T(""));
// 		std::map<int, int> mapTime;    // <time, sameTime 갯수>
// 		std::map<int, int>::iterator iter;
// 		
// 		int nTotalRowCount = m_wndReportCtrl.GetRows()->GetCount();
// 		for(int i= 0; i< nTotalRowCount; ++i)
// 		{
// 			CXTPReportRow* pRow       = m_wndReportCtrl.GetRows()->GetAt(i);
// 			int            nRowIdx    = pRow->GetIndex();
// 
// 			CXTPReportRecord*           pRecord    = pRow->GetRecord();
// 			CXTPReportRecordItem*       pItem      = pRecord->GetItem(_0_COLUMN_CLASS_NAME);
// 			CXTPReportRecordItemNumber* pItemNum   = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_TIME);
// 			int nValue           = (int)pItemNum->GetValue();
// 
// 			iter = mapTime.find(nValue);
// 
// 			if(mapTime.end() == iter)
// 			{
// 				mapTime.insert(std::make_pair(nValue, 1));
// 			}
// 			else
// 			{
// 				int &nSameCnt = iter->second;
// 				++nSameCnt;
// 			}
// 
// 			if(nRowIndexChanged != nRowIdx)
// 			{
// 				if(nValueChaned == nValue)
// 				{
// 					strMsg.Format(_T("Class(%s) 와 Time 중복"), pItem->GetCaption(NULL));
// 				}
// 			}
// 		}
// 		
// 		if(strMsg.Compare(_T("")) != 0)
// 		{
// 			AfxMessageBox(strMsg);
// 		}
// 
// 		ResortReportRecord(mapTime);
// 	}
#pragma endregion OLD
}


void KSetAccessibility::OnBnClickedButtonRun()
{
	UpdateData(TRUE);

	try
	{
		// 유효성 체크
		if (!InvalidateInputInfo())
			return;

		//Boundary Dist는 0.01보다 커야함
		CString strDist;
		GetDlgItemText(IDC_EDIT_DISTANCE, strDist);
		double  dValue = _ttof(strDist);

		if (dValue < 0.1)
		{
			AfxMessageBox(_T("입력한 접근 반경이 너무 작습니다."));
			GetDlgItem(IDC_EDIT_DISTANCE)->SetFocus();
			return;
		}

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(AccessThreadCaller, this, true, nLang);

		if ( !m_bRunResult)
		{
			AfxMessageBox(m_strRunErrMsg);
			return;
		}
		else 
		{
			AfxMessageBox(_T("이용가능 역/정류장 생성을 완료하였습니다.\n이용가능 역/정류장 편집 기능으로 생성된 결과에 대한 검색 및 편집이 가능합니다."));
		}

		KIOTable* pTalble = m_pTarget->Tables()->FindTable(TABLE_ACCESS_STOP_NODE);
		pTalble->Notify();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());        
        AfxMessageBox(_T("이용가능 역/정류장 생성 중 오류가 발생하였습니다."));
		return;
	}
	catch(...)
	{
		TxLogDebugException();
        AfxMessageBox(_T("이용가능 역/정류장 생성 중 오류가 발생하였습니다."));
		return;
	}

	KDialogEx::OnOK();	
}


bool KSetAccessibility::InvalidateInputInfo()
{
	
	CString strDistance(_T(""));
	GetDlgItemText(IDC_EDIT_DISTANCE, strDistance);

	if (!QBicStringChecker::IsNumeric(strDistance))
	{
		AfxMessageBox(_T("접근 반경에 숫자 값을 입력해 주세요."));
		GetDlgItem(IDC_EDIT_DISTANCE)->SetFocus();
		return false;
	}

	double dDistance = _ttof(strDistance);

	if (dDistance < 0.01)
	{
		AfxMessageBox(_T("0.01보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_DISTANCE)->SetFocus();
		return false;
	}

	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	std::map<int, std::vector<CString>> mapTime;    // <time, sameTime 갯수>
	std::map<int, std::vector<CString>>::iterator iter = mapTime.begin();

	for (int i = 0; i < nRecordCount; i++)
	{
		CXTPReportRecord*           pRecord      = pRecords->GetAt(i);
		CXTPReportRecordItem*       pItem        = pRecord->GetItem(_0_COLUMN_CLASS_NAME);
		CXTPReportRecordItemNumber* pItemNum     = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_TIME);

		int      nClassID     = (int)pItem->GetItemData();
		CString  strClassName = pItem->GetCaption(NULL);
		int      nTimeMin     = (int)pItemNum->GetValue();

		if(nTimeMin < 1)
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("%s 등급의 도보접근시간을 1분이상으로 입력해 주세요."), strClassName);
			AfxMessageBox(strMsg);
			return false;
		}

		iter = mapTime.find(nTimeMin);
		if(mapTime.end() == iter)
		{
			std::vector<CString> vecClassName;
			vecClassName.push_back(strClassName);

			mapTime.insert(std::make_pair(nTimeMin, vecClassName));
		}
		else
		{
			std::vector<CString> &vecClassName = iter->second;
			vecClassName.push_back(strClassName);
		}
	}

	iter = mapTime.begin();
	std::map<int, std::vector<CString>>::iterator itEnd = mapTime.end();
	while(iter != itEnd)
	{
		int nTimeMin = iter->first;
		std::vector<CString> vecClassName = iter->second;
		
		if(vecClassName.size() > 1) //중복 의미
		{
			CString strNames(_T(""));
			for(size_t i= 0; i< vecClassName.size(); i++)
			{
				if( 0 != i)
				{
					strNames.Append(_T(", "));
				}
				strNames.Append(vecClassName[i]);
			}

			CString strMsg(_T(""));
			strMsg.Format(_T("도보접근시간(분)이 중복된 숫자로 입력되었습니다 : %s "), strNames);
			AfxMessageBox(strMsg);
			return false;
		}
		++iter;
	}

	return true;
}


unsigned __stdcall KSetAccessibility::AccessThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("이용가능 역/정류장 생성 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Access Node Generator..."));
	}

	KSetAccessibility*        pDlg = (KSetAccessibility*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}


void KSetAccessibility::Run()
{
	TxLogDebugStartMsg();
	try
	{
		CreateArgument();

		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget);

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				ThrowException(_T("User Stopped..."));
			}
		}
		
		m_nResultCode = DLL_UNEXPECTED_ERROR;
 		CString strWindowName;
 		GetWindowText(strWindowName);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("이용가능 역/정류상 생성 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run Access Node Generator Module..."));
		}

		KLauncher::Run(strWindowName, 14);

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("이용가능 역/정류장 생성 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("Access Node Generator Module Complete."));
			}
			TxLogDebug(_T("Model Run Ok.."));
		}
		else if (DLL_USER_STOPPING == m_nResultCode || QBicSimpleProgressThread::IsUserStopped())
		{
            m_nResultCode = DLL_USER_STOPPING;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			TxLogDebug(_T("Model Stop.."));
			ThrowException(_T("Model Stop.."));
		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			AddStatusMessage(_T("> Access Node Generator  Dll Not Found ..."));
			TxLogDebug(_T("DLL NOT Found.."));
			ThrowException(_T("DLL Not Found.."));
		}
		else
		{			
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Running Failed."));
			}
			TxLogDebug(_T("Model Run Error.."));
			ThrowException(_T("Access Node Generator End(Error)"));
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
		}

		std::map<Integer, CString> mapZoneName;
		LoadZoneName(mapZoneName);

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			m_nResultCode = DLL_USER_STOPPING;
			ThrowException(_T(">User Stop.."));
		}

		if ( !ReadOutPutFile(mapZoneName) )
		{
			m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Saving Output Data Failed."));
			}
			TxLogDebug(_T("Output 정보 입력 실패..."));
			ThrowException(_T("Output 정보 입력 실패"));
		}

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			m_nResultCode = DLL_USER_STOPPING;
			ThrowException(_T(">User Stop.."));
		}

		InsertAccessLineStation();
		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_ACCESS_LINE_STATION);
        if (pTable)
		    pTable->Notify();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		m_strRunErrMsg = ex->GetErrorMessage();
		//AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		m_strRunErrMsg = _T("Failed Set Accessibility");
		return;
	}

	m_bRunResult = true;	

	TxLogDebugEndMsg();
}


void KSetAccessibility::CreateArgument()
{
	ClearFiles();

	ImChampDir::SetCurrentDirectoryToApp();

	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(T_ARG_ACCESSIBILITY, nOpenMode);

	CreateDirectory( ConvertSavePath(_T("")), NULL );

	try
	{
		ArgumentWriteProjectName(of);
		ArgumentWriteProcessMethod(of);
		ArgumentWriteClass(of);
		ArgumentWriteFolder(of);
		
		of.Close();

		//TxLogDebug("START NODE CREAETE FILE ------ ");
		if ( !KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("node.dat")) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		}
		if ( !KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("line_info.dat")), ConvertSavePath(_T("line.dat")) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LineFile) 중 오류가 발생하였습니다. !!!"));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ThrowException(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		of.Close();
	}
}


void KSetAccessibility::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove((CStringA)T_ARG_ACCESSIBILITY);
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINEINFO));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINE));

		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE));
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KSetAccessibility::ArgumentWriteProjectName(CStdioFileEx& of)
{
	CString strProjectName = m_pProject->GetName();

	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("%s\r\n"), strProjectName);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KSetAccessibility::ArgumentWriteProcessMethod(CStdioFileEx& of)
{
	int     nCheck		   = 0;
	double  dDistance      = 0;

	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK1);
	if (TRUE == bCheck)
	{
		nCheck = 1;
	}


	CString strDistance(_T(""));
	GetDlgItemText(IDC_EDIT_DISTANCE, strDistance);
	dDistance = _ttof(strDistance);
	
	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("ProcessMethod\t%.3f\t%d\r\n"), dDistance, nCheck);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KSetAccessibility::ArgumentWriteClass(CStdioFileEx& of)
{
	try
	{
		CString strOut(_T(""));

		CXTPReportRecords*          pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;

		int        nRecordCount   = pRecords->GetCount();
		int        nClassID       = 0;
		CString    strClassName(_T(""));
		int        nTimeMin       = 0;

		strOut.Format(_T("noClass\t%d\r\n"), nRecordCount);
		of.WriteString(strOut);
		
		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord      = pRecords->GetAt(i);

			pItem        = pRecord->GetItem(_0_COLUMN_CLASS_NAME);
			nClassID     = (int)pItem->GetItemData();
			strClassName = pItem->GetCaption(NULL);

			pItemNum     = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_TIME);
			nTimeMin     = (int)pItemNum->GetValue();

			strOut.Format(_T("%s\t%d\r\n"), strClassName, nTimeMin);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KSetAccessibility::ArgumentWriteFolder(CStdioFileEx& of)
{
	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("%s\r\n"), m_strOutputFolerPath);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}


bool KSetAccessibility::ReadOutPutFile(std::map<Integer, CString> a_mapZoneName)
{
	TxLogDebugStartMsg();

	std::ifstream ifs( ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE), std::ios::binary );    

	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount = 0;
			m_vecResultInfo.clear();
			while(! ifs.eof())
			{
				//KAccBusSubwayStopAtZoneInfo oTAccZoneInfo = {0,0,0,0,_T("")};
				TAccessStopNodeInfo oTAccessResultInfo = {0,0,0,_T(""), _T(""), _T(""), 0};

                ifs.read( reinterpret_cast<char*>( &(oTAccessResultInfo.nxZoneID)),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oTAccessResultInfo.nxNodeID)),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oTAccessResultInfo.nMode)),           sizeof(int) );
				
				char cClassZone = 'e';
				ifs.read( reinterpret_cast<char*>( &(cClassZone)),     sizeof(char) );
				CString strClassZone(_T(""));
				strClassZone.Format(_T("%c"), cClassZone);
				oTAccessResultInfo.strClassInZone = strClassZone;

				char cClassNetwork = 'e';
				ifs.read( reinterpret_cast<char*>( &(cClassNetwork)),     sizeof(char) );
				CString strClassNet(_T(""));
				strClassNet.Format(_T("%c"), cClassNetwork);
				oTAccessResultInfo.strClassInNetwork = strClassNet;

				ifs.read( reinterpret_cast<char*>( &(oTAccessResultInfo.nNoLine)),           sizeof(int) );

				AutoType iterfind = a_mapZoneName.find(oTAccessResultInfo.nxZoneID);
				AutoType iterend = a_mapZoneName.end();
				if (iterfind != iterend)
				{
					oTAccessResultInfo.strZoneName = iterfind->second;
				}

				if(ifs.eof())break;

				m_vecResultInfo.push_back(oTAccessResultInfo);

				nCount++;
			}

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}
		else
		{
			throw 1;		
		}

		ifs.close();
	}
	catch (...)
	{
		ifs.close();
		TxLogDebugException();
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}


void KSetAccessibility::InsertAccessLineStation()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString             strPreparedQuery (_T(""));

	try
	{
		spDBaseConnection->BeginTransaction();
		
		#pragma region delete - access_line_station

		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_ACCESS_STOP_NODE);
		spDBaseConnection->ExecuteUpdate(strDelQuery);

		#pragma endregion delete - access_line_station

		#pragma region insert - access_line_station

		strPreparedQuery.Format(
			_T(" INSERT INTO %s ")
			_T(" ( zone_id, node_id, mode, zone_name, class_in_zone, class_in_network, no_of_line ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,?,?,?,? ) "), TABLE_ACCESS_STOP_NODE);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strPreparedQuery);

		int nloopCount = 0;

		int nCount = m_vecResultInfo.size();
		for(int i = 0; i < nCount; i ++)
		{

			TAccessStopNodeInfo oTAccZoneLineInfo = m_vecResultInfo[i];

			if (oTAccZoneLineInfo.strClassInNetwork.Compare(_T("Z")) == 0 || 
				oTAccZoneLineInfo.strClassInNetwork.Compare(_T("Z")) == 0 ||
				oTAccZoneLineInfo.nNoLine == 0)
				continue;

			Integer &nxZoneID			= oTAccZoneLineInfo.nxZoneID;
			Integer &nxNode				= oTAccZoneLineInfo.nxNodeID;
			int     &nMode				= oTAccZoneLineInfo.nMode;
			CString &strZoneName		= oTAccZoneLineInfo.strZoneName;
			CString &strZoneClass		= oTAccZoneLineInfo.strClassInZone;
			CString &strNetworkClass	= oTAccZoneLineInfo.strClassInNetwork;
			int		&nLineNo			= oTAccZoneLineInfo.nNoLine;


			spPrepareStatement->BindInt64  (1, nxZoneID);
			spPrepareStatement->BindInt64  (2, nxNode);
			spPrepareStatement->BindInt    (3, nMode);
			spPrepareStatement->BindText   (4, strZoneName);
			spPrepareStatement->BindText   (5, strZoneClass);
			spPrepareStatement->BindText   (6, strNetworkClass);
			spPrepareStatement->BindInt	   (7, nLineNo);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		#pragma endregion insert - access_line_station

		#pragma region delete - access_class
		
		strDelQuery.Format(_T("delete from %s"), TABLE_ACCESS_CLASS);
		spDBaseConnection->ExecuteUpdate(strDelQuery);
		
		#pragma endregion delete - access_class

		#pragma region insert - access_class		
		strPreparedQuery.Format(
			_T(" INSERT INTO %s ")
			_T(" ( class_id, class_name, time_min ) ")
			_T(" VALUES ")
			_T(" ( ?,?,? ) "), TABLE_ACCESS_CLASS);

		KPreparedStatementPtr spPrepareStatement2 = spDBaseConnection->PrepareStatement(strPreparedQuery);

		CXTPReportRecords*          pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;

		int        nRecordCount   = pRecords->GetCount();
		int        nClassID       = 0;
		CString    strClassName(_T(""));
		int        nTimeMin       = 0;

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord      = pRecords->GetAt(i);

			pItem        = pRecord->GetItem(_0_COLUMN_CLASS_NAME);
			strClassName = pItem->GetCaption(NULL);

			pItemNum     = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_TIME);
			nTimeMin     = (int)pItemNum->GetValue();

			spPrepareStatement2->BindInt    (1, (i+1));
			spPrepareStatement2->BindText   (2, strClassName);
			spPrepareStatement2->BindInt    (3, nTimeMin);

			spPrepareStatement2->ExecuteUpdate();
			spPrepareStatement2->Reset();

		}

		#pragma endregion insert - access_class

		#pragma  region delete - access_info
		
		strDelQuery.Format(_T("delete from %s"), TABLE_ACCESS_STOP_NODE_INFO);
		spDBaseConnection->ExecuteUpdate(strDelQuery);

		#pragma  endregion delete - access_info


		#pragma  region insert - access_info

		int     nCheck		   = 0;
		double  dDistance      = 0;

		BOOL bCheck = IsDlgButtonChecked(IDC_CHECK1);
		if (TRUE == bCheck)
		{
			nCheck = 1;
		}

		CString strDistance(_T(""));
		GetDlgItemText(IDC_EDIT_DISTANCE, strDistance);
		dDistance = _ttof(strDistance);

		strPreparedQuery.Format(
			_T(" INSERT INTO %s ")
			_T(" ( boundary, using_code ) ")
			_T(" VALUES ")
			_T(" ( %f, %d ) "), TABLE_ACCESS_STOP_NODE_INFO, dDistance, nCheck);
		
		spDBaseConnection->ExecuteUpdate(strPreparedQuery);

		#pragma  region insert - access_info

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
}


void KSetAccessibility::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KSetAccessibility::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


CString KSetAccessibility::ConvertSavePath(CString a_strFileName)
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, a_strFileName);

	return strSavePath;
}

bool KSetAccessibility::UsingCheckZoneID()
{
	bool bUsingZoneID = false;
	int  nCount		  = 0;

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format(_T("SELECT COUNT(*) FROM node WHERE zone_id != 0"));

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			nCount = spResultSet->GetValueInt(0);
		}

		if (nCount != 0)
			bUsingZoneID = true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return bUsingZoneID;
	}
	catch(...)
	{
		TxLogDebugException();
		return bUsingZoneID;
	}

	return bUsingZoneID;
}


BOOL KSetAccessibility::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	try
	{
		int     nMsgCode = pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return KDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void KSetAccessibility::OnBnClickedDefault()
{
	try
	{
		KDBaseDefaultParameter::LoadDefaultAccessSetting(m_mapDefaultAccessSetting);
		KDBaseDefaultParameter::LoadDefaultAccessClassInfo(m_mapDefaultAccessClassInfo);

		AutoType find = m_mapDefaultAccessClassInfo.find(1);
		AutoType end = m_mapDefaultAccessClassInfo.end();

		if( find == end )
		{
			AfxMessageBox(_T("설정값 정보가 없습니다."));
		}
		else
		{
			AutoType iter = m_mapDefaultAccessClassInfo.find(1);

			std::vector<TAccessClassInfo> vecAccessClassInfo = iter->second;
			int nUserCodeCount = vecAccessClassInfo.size();

			InitComboClass((int)nUserCodeCount);
			InitDefaultSetting();
			InitDefaultReportRecord();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("Default 정보가 없습니다. "));
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("Default 정보가 없습니다. "));
		return;
	}
}
