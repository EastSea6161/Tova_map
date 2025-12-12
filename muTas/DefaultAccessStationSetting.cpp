// DefaultAccessStationSetting.cpp : 구현 파일입니다.

#include "stdafx.h"
#include "DefaultAccessStationSetting.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

namespace DefaultAccessStationSetting
{
	const int	  INITTIME		  = 5;
};
using namespace DefaultAccessStationSetting;

enum
{
	_0_COLUMN_ID,
	_1_COLUMN_CLASS_NAME = 0,
	_2_COLUMN_TIME,
};

// KDefaultAccessStationSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultAccessStationSetting, KResizeDialogEx)

	KDefaultAccessStationSetting::KDefaultAccessStationSetting(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultAccessStationSetting::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nCountRecord(0)
{
	KDBaseDefaultParameter::LoadDefaultAccessSetting(m_mapDefaultAccessSetting);
	KDBaseDefaultParameter::LoadDefaultAccessClassInfo(m_mapDefaultAccessClassInfo);
	KDBaseDefaultParameter::SetAccessibility(m_mapAccessibility);
}

KDefaultAccessStationSetting::~KDefaultAccessStationSetting()
{
}

void KDefaultAccessStationSetting::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO, m_cboClass);
}

BEGIN_MESSAGE_MAP(KDefaultAccessStationSetting, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO,		  &KDefaultAccessStationSetting::OnCbnSelchangeComboClass)
	ON_BN_CLICKED	(IDOK,			  &KDefaultAccessStationSetting::OnBnClickedOk)
	ON_BN_CLICKED	(IDCANCEL,	      &KDefaultAccessStationSetting::OnBnClickedCancel)
	ON_BN_CLICKED   (IDDefaultSystem, &KDefaultAccessStationSetting::OnBnClickedDefaultsystem)
END_MESSAGE_MAP()

// KDefaultAccessStationSetting 메시지 처리기입니다.

BOOL KDefaultAccessStationSetting::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);

	try
	{
		InitReportHeader();
		ResizeComponent();
		CountClassInfo();

		InitAccessSetting();
		InitAccessClassInfo();
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return TRUE;
}


void KDefaultAccessStationSetting::CountClassInfo()
{
	try
	{
		AutoType iterUserRecoardSize = m_mapDefaultAccessClassInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endRecoardSize  = m_mapDefaultAccessClassInfo.end();
		if(iterUserRecoardSize != endRecoardSize)
		{
			std::vector<TAccessClassInfo>& vecAccessClassInfo = iterUserRecoardSize->second;
			size_t nUserCodeCount = vecAccessClassInfo.size();
			m_nCountRecord = (int)nUserCodeCount;
		}
		else
		{
			AutoType iterSystemRecoardSize = m_mapDefaultAccessClassInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			if(iterSystemRecoardSize != endRecoardSize)
			{
				std::vector<TAccessClassInfo>& vecAccessClassInfo = iterSystemRecoardSize->second;
				size_t nSystemCodeCount = vecAccessClassInfo.size();
				m_nCountRecord = (int)nSystemCodeCount;
			}
			else
			{
				m_nCountRecord = 3;
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
return;
}


void KDefaultAccessStationSetting::InitAccessClassInfo()
{
	try
	{
		AutoType iterClassInfo = m_mapDefaultAccessClassInfo.begin();
		AutoType endClassInfo  = m_mapDefaultAccessClassInfo.end();

		if(m_mapDefaultAccessClassInfo.size()< 1)
		{
			UpdateReportRecord();
			SaveAccessClassinfo(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
		else
		{
			AutoType iterUser = m_mapDefaultAccessClassInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			AutoType endUser  = m_mapDefaultAccessClassInfo.end();
			if(iterUser != endUser)
			{
				InitReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefaultAccessStationSetting::InitAccessSetting()
{
	try
	{
		if (m_mapDefaultAccessSetting.size()< 1)
		{
			SetDlgItemInt(IDC_EDIT_DISTANCE, 1);
			CheckDlgButton(IDC_CHECK, TRUE);
			InitComboClass(3);
			SaveAccessSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
		else
		{
			AutoType iterUser = m_mapDefaultAccessSetting.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

			int      nCode(0);
			if(iterUser != m_mapDefaultAccessSetting.end())
				nCode = KDefaultParameterMasterCode::USER_MASETER_CODE;
			else
				nCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;

			InitSetting(nCode);
			InitComboClass((int) m_nCountRecord);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefaultAccessStationSetting::UpdateReportRecord()
{
	try
	{
		UpdateData(TRUE);
		m_wndReportCtrl.ResetContent(TRUE);

		CXTPReportRecord*     pRecord    = nullptr;
		CXTPReportRecordItem* pItem      = nullptr;
		int					  nClassTime = KAccessStationDefine::ACCESS_INIT_TIME;

		int nSelCur = m_cboClass.GetCurSel();

		int nGrade  = (int)(m_cboClass.GetItemData(nSelCur));

		for (int i= 0; i< nGrade; i++)
		{
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			CString strClassName(_T(""));

			AutoType find		= m_mapAccessibility.find(i+1);
			AutoType findend	= m_mapAccessibility.end();

			if (find != findend)
			{
				strClassName = find->second;
			}
			else
			{
				AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
				return;
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strClassName));
			pItem->SetItemData((DWORD_PTR) (i+1));
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nClassTime, _T("%.0f")));
			pItem->SetAlignment(xtpColumnTextCenter);

			nClassTime += KAccessStationDefine::ACCESS_INCREASE_TIME;
		}
		m_wndReportCtrl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}	
}

void KDefaultAccessStationSetting::InitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_CLASS_NAME, _T("Class"),    50));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("등급"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_TIME,       _T("Access Time(min)"), 50));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("도보접근시간(분)"));
	}
}

void KDefaultAccessStationSetting::InitSetting(int a_nMasterCode)
{
	try
	{
		AutoType iter = m_mapDefaultAccessSetting.find(a_nMasterCode);
		std::vector<KAccessStationSetting>& vecAccessSetting = iter->second;

		KAccessStationSetting oAccessSetting = vecAccessSetting[0];
		double dDistance                     = oAccessSetting.dDistance;
		int    RegionCode                    = oAccessSetting.nRegionCode;

		CString strValue;
		strValue.Format(_T("%.2f"), dDistance);

		SetDlgItemText(IDC_EDIT_DISTANCE, strValue);

		if ( 1 == RegionCode)
		{
			CheckDlgButton(IDC_CHECK, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK, FALSE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}	
}

void KDefaultAccessStationSetting::InitReportRecord(int a_nMasterCode)
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

		AutoType iter = m_mapDefaultAccessClassInfo.find(a_nMasterCode);
		AutoType end  = m_mapDefaultAccessClassInfo.end();
		if (iter != end)
		{
			std::vector<TAccessClassInfo>& vecAccessClassInfo = iter->second;
			size_t						   nUserCount		  = vecAccessClassInfo.size();

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
		}

		m_wndReportCtrl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}	
}

void KDefaultAccessStationSetting::InitComboClass(int a_nInitGrade)
{
	try
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
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDefaultAccessStationSetting::ResizeComponent()
{
	SetResize(IDC_SEPRATOR2,   SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR4,   SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,	   SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_SEPRATOR3,   SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);
	SetResize(IDDefaultSystem, SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT); 
	SetResize(IDOK,			   SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,		   SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}

bool KDefaultAccessStationSetting::InvalidateInputInfo()
{

	CString strDistance(_T(""));
	GetDlgItemText(IDC_EDIT_DISTANCE, strDistance);

	if( !QBicStringChecker::IsNumeric(strDistance))
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

	for (int i= 0; i< nRecordCount; i++)
	{
		CXTPReportRecord*           pRecord      = pRecords->GetAt(i);
		CXTPReportRecordItem*       pItem        = pRecord->GetItem(_1_COLUMN_CLASS_NAME);
		CXTPReportRecordItemNumber* pItemNum     = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_TIME);

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

bool KDefaultAccessStationSetting::SaveAccessSetting(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	CString strValue;

	try
	{    
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s where Master_Code  = 1"), KDefaultParaTable::TABLE_DEFAULT_ACCESS_CLASSINFO_SETTING);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_ACCESS_CLASSINFO_SETTING);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		double dDistance   = 0;
		int    nRegionCode = 0;

		GetDlgItemText(IDC_EDIT_DISTANCE, strValue);
		dDistance = _ttof(strValue);

		CButton *pButton  = (CButton*)GetDlgItem(IDC_CHECK);
		nRegionCode  = pButton->GetCheck();

		int nSelCur = m_cboClass.GetCurSel();
		int nClassNumber  = (int)(m_cboClass.GetItemData(nSelCur));

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T("Insert Into %s(Master_Code, Distance, Region_Code, Class_Number)"), KDefaultParaTable::TABLE_DEFAULT_ACCESS_CLASSINFO_SETTING);
		strSQL.AppendFormat(_T(" VALUES ( %d, %f, %d, %d )"), a_nMasterCode, dDistance, nRegionCode, nClassNumber);

		spDBaseConnection->ExecuteUpdate(strSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return false;
	}

	return true;
}

bool KDefaultAccessStationSetting::SaveAccessClassinfo(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{    
		spDBaseConnection->BeginTransaction();

		CString          strPreparedQuery (_T(""));

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("delete from %s where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_ACCESS_CLASSINFO);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("delete from %s"), KDefaultParaTable::TABLE_DEFAULT_ACCESS_CLASSINFO);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		int	nClassTime = 0;

		strPreparedQuery.Format(
			_T(" INSERT INTO %s ")
			_T(" (Master_Code, ID, Class, Time ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_ACCESS_CLASSINFO);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strPreparedQuery);

		CXTPReportRecords*          pRecords  = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		int							nRecordCount = pRecords->GetCount();
		int							nclassTime   = 0;
		CString                     strClassName (_T(""));
		CString                     strClassTime (_T(""));

		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord           = pRecords->GetAt(i);

			pItemText         = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_CLASS_NAME);
			strClassName      = pItemText ->GetCaption(NULL);

			pItemNum          = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_TIME);
			strClassTime       = pItemNum ->GetCaption(NULL);
			nClassTime        = _ttoi(strClassTime);

			spPrepareStatement->BindInt   (1, a_nMasterCode);
			spPrepareStatement->BindInt   (2, i+1);
			spPrepareStatement->BindText  (3, strClassName);
			spPrepareStatement->BindInt   (4, nClassTime);
			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return false;
	}

	return true;
}

void KDefaultAccessStationSetting::OnCbnSelchangeComboClass()
{
	UpdateReportRecord();
}

void KDefaultAccessStationSetting::OnBnClickedOk()
{
	UpdateData(TRUE);
	try
	{
		if (!InvalidateInputInfo())
		{
			return;
		}
		
		if (!SaveAccessSetting(KDefaultParameterMasterCode::USER_MASETER_CODE))
		{
			return;
		}
		if (!SaveAccessClassinfo(KDefaultParameterMasterCode::USER_MASETER_CODE))
		{
			return;
		}
	}
	catch (int)
	{
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}

	KResizeDialogEx::OnOK();
}

void KDefaultAccessStationSetting::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KDefaultAccessStationSetting::OnBnClickedDefaultsystem()
{
	try
	{
		AutoType iter = m_mapDefaultAccessSetting.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType end  = m_mapDefaultAccessSetting.end();
		if(iter != end)
		{
			AutoType iter = m_mapDefaultAccessClassInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			std::vector<TAccessClassInfo>& vecAccessClassInfo = iter->second;
			int nUserCodeCount = vecAccessClassInfo.size();

			InitSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
			InitComboClass((int) nUserCodeCount);
		}
		else
		{
			KDBaseDefaultParameter::LoadDefaultAccessSetting(m_mapDefaultAccessSetting);
			AutoType iter = m_mapDefaultAccessClassInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			std::vector<TAccessClassInfo>& vecAccessClassInfo = iter->second;
			int nSystemCodeCount = vecAccessClassInfo.size();

			InitSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			InitComboClass(nSystemCodeCount);
		}

		AutoType iterClassInfo = m_mapDefaultAccessClassInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endClassInfo  = m_mapDefaultAccessClassInfo.end();
		if (iterClassInfo != endClassInfo)
		{
			InitReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else 
		{
			KDBaseDefaultParameter::LoadDefaultAccessClassInfo(m_mapDefaultAccessClassInfo);
			InitReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

