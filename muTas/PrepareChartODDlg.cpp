// PrepareChartODDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PrepareChartODDlg.h"
#include "afxdialogex.h"

//#include "StringChecker.h"

#include "Target.h"
//^#include "DBaseConnector.h"
#include "DBaseMatrixView.h"
//#include "StringChecker.h"
#include "ViewChartDlg.h"
#include "KmzSystem.h"

// KPrepareChartODDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPrepareChartODDlg, KResizeDialogEx)

KPrepareChartODDlg::KPrepareChartODDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPrepareChartODDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KPrepareChartODDlg::~KPrepareChartODDlg()
{
}

void KPrepareChartODDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_cboTable);
	DDX_Control(pDX, IDC_COMBO_FIELD, m_cboField);
	DDX_Control(pDX, IDC_COMBO_ORIGIN_UNIT, m_cboOriginUnit);
	DDX_Control(pDX, IDC_COMBO_DES_UNIT, m_cboDestUnit);
	DDX_Control(pDX, IDC_REPORT_ORIGIN, m_wndReportCtrlOrigin);
	DDX_Control(pDX, IDC_REPORT_DEST,   m_wndReportCtrlDest);
	DDX_Control(pDX, IDC_CHARTCONTROL2, m_wndChartControl);
	
}


BEGIN_MESSAGE_MAP(KPrepareChartODDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL,                 &KPrepareChartODDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_ORIGIN,         &KPrepareChartODDlg::OnBnClickedCheckOrigin)
	ON_BN_CLICKED(IDC_CHECK_DESTINATION,    &KPrepareChartODDlg::OnBnClickedCheckDestination)
	ON_BN_CLICKED(IDC_BUTTON_CHART,         &KPrepareChartODDlg::OnBnClickedButtonChart)
	ON_CBN_SELCHANGE(IDC_COMBO_ORIGIN_UNIT, &KPrepareChartODDlg::OnCbnSelchangeComboOriginUnit)
	ON_CBN_SELCHANGE(IDC_COMBO_DES_UNIT,    &KPrepareChartODDlg::OnCbnSelchangeComboDesUnit)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE,       &KPrepareChartODDlg::OnCbnSelchangeComboTable)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD,       &KPrepareChartODDlg::OnCbnSelchangeComboField)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO3, &KPrepareChartODDlg::OnBnClickedRadioChart)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_ORIGIN, &KPrepareChartODDlg::OnReportACheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_DEST,   &KPrepareChartODDlg::OnReportBCheckItem)
	ON_BN_CLICKED(IDC_CHECK_SHOWLABELS,                 &KPrepareChartODDlg::OnCheckShowLabels)
	ON_BN_CLICKED(IDC_CHECK_SHOWAXISX,                  &KPrepareChartODDlg::OnCheckShowAxisX)

END_MESSAGE_MAP()


// KPrepareChartODDlg 메시지 처리기입니다.
BOOL KPrepareChartODDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitialControl();
	LoadComboField();
	LoadOriginReportData();
	LoadDestReportData();
	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPrepareChartODDlg::ResizeComponent()
{
	SetResize(IDC_REPORT_ORIGIN,			  SZ_TOP_LEFT,    SZ_MIDDLE_LEFT);
	SetResize(IDC_SEPRATOR4,				  SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);

	SetResize(IDC_DESTINATION_STATIC,		  SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_COMBO_DES_UNIT,		      SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_CHECK_DESTINATION,		  SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);

	SetResize(IDC_REPORT_DEST,				  SZ_MIDDLE_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR1,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CHART,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,						  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR5,				  SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHARTCONTROL2,			  SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_CHECK_SHOWLABELS,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SHOWAXISX,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_RADIO1,					  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO2,			          SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO3,			          SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_EXPLAIN,			  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KPrepareChartODDlg::InitialControl()
{
	m_cboField.ResetContent();

	KIOTables* pIOTables    = m_pTarget->Tables();
	size_t     nxTableCount = pIOTables->GetCount();

	int nODTableCount = 0;
	for (size_t i=0; i<nxTableCount; i++)
	{
		KIOTable* pIOTable = pIOTables->GetTableAt(i);

		if (pIOTable->TableType() == KEMIOTableTypeODData)
		{
			int nIndex = m_cboTable.AddString(pIOTable->Caption());
			m_cboTable.SetItemData(nIndex, (DWORD_PTR)pIOTable);
			nODTableCount++;
		}
	}

	if (nODTableCount > 0)
	{
		m_cboTable.SetCurSel(0);	
		LoadComboField();
	}

	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_ZONE);
	const 
    KIOColumns* pIOColumns = pIOTable->Columns();
	int nColumnCount = pIOColumns->ColumnCount();

	int nCount = 0;
	if (KmzSystem::IsPassengerSystem() == true)
	{
		for (int i=0; i<nColumnCount; i++)
		{
			KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
			if (pIOColumn->DataType() ==  KEMIODataTypeInteger)
			{
				int nIndex = m_cboOriginUnit.AddString(pIOColumn->Caption());
				m_cboOriginUnit.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

				nIndex = m_cboDestUnit.AddString(pIOColumn->Caption());
				m_cboDestUnit.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

				nCount++;
			}
		}
	}
	else
	{
		for (int i=0; i<nColumnCount; i++)
		{
			KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
			if (pIOColumn->DataType() ==  KEMIODataTypeInteger)
			{
				CString strColumnName = pIOColumn->Name();// 화물에서 이 두개의 필드만 보여 달라고 한다.(KOTI 요청)
				if (strColumnName.CompareNoCase(COLUMN_ZONE_MAIN_CATEGORY) != 0 &&
					strColumnName.CompareNoCase(COLUMN_ZONE_ID) != 0)
				{
					continue;
				}

				int nIndex = m_cboOriginUnit.AddString(pIOColumn->Caption());
				m_cboOriginUnit.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

				nIndex = m_cboDestUnit.AddString(pIOColumn->Caption());
				m_cboDestUnit.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

				nCount++;
			}
		}
	}

	if (nCount > 0)
	{
		m_cboOriginUnit.SetCurSel(0);
		m_cboDestUnit.SetCurSel(0);
	}

	KReportCtrlSetting::Default(m_wndReportCtrlOrigin);
	KReportCtrlSetting::Default(m_wndReportCtrlDest);

	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrlOrigin.AddColumn(new CXTPReportColumn(0, _T("Select"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrlOrigin.AddColumn(new CXTPReportColumn(1, _T("ID"),   30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrlOrigin.AddColumn(new CXTPReportColumn(2, _T("Name"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	m_wndReportCtrlOrigin.Populate();
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }

	pColumn = m_wndReportCtrlDest.AddColumn(new CXTPReportColumn(0, _T("Select"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrlDest.AddColumn(new CXTPReportColumn(1, _T("ID"),   30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrlDest.AddColumn(new CXTPReportColumn(2, _T("Name"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }

	m_wndReportCtrlDest.Populate();

	KChartCtrlSetting::DefaultCtrl(m_wndChartControl);
	
	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
}


void KPrepareChartODDlg::LoadComboField()
{
	m_cboField.ResetContent();

	int nIndex = m_cboTable.GetCurSel();
	if (nIndex < 0) return;

	KIOTable*   pIOTable   = (KIOTable*)m_cboTable.GetItemData(nIndex);
	const 
    KIOColumns* pIOColumns = pIOTable->Columns();

	int nColumnCount = pIOColumns->ColumnCount();
	int nFieldCount  = 0;
	for (int i=0; i<nColumnCount; i++)
	{
		KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
		if (pIOColumn->DataType() ==  KEMIODataTypeDouble)
		{
			nIndex = m_cboField.AddString(pIOColumn->Caption());
			m_cboField.SetItemData(nIndex, (DWORD_PTR)pIOColumn);
			nFieldCount++;
		}
	}

	if (nFieldCount > 0)
	{
		m_cboField.SetCurSel(0);
	}
}


void KPrepareChartODDlg::OnCbnSelchangeComboTable()
{
	m_mapDesireInfo.clear();
	LoadComboField();

	UpdateChartData();
}


void KPrepareChartODDlg::OnCbnSelchangeComboField()
{
	m_mapDesireInfo.clear();

	UpdateChartData();
}


void KPrepareChartODDlg::LoadOriginReportData()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_ORIGIN);   
	m_vecDesireOrigin.clear();
	m_wndReportCtrlOrigin.ResetContent();

	int nIndex = m_cboOriginUnit.GetCurSel();
	if (nIndex < 0) return;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ZONE);
	KIOColumn* pIOColumn = (KIOColumn*)m_cboOriginUnit.GetItemData(nIndex);

	CString strSQL;
	// Integer - Primary(zone_id), 그냥숫자값, 코드
	if (pIOColumn->PrimaryKey())
	{
		strSQL.Append      (_T(" Select "));
		strSQL.AppendFormat(_T("  Ta.%s, max(Ta.name), avg(x), avg(y) "), pIOColumn->Name());
		strSQL.Append      (_T(" From Zone Ta, Node Tb "));
		strSQL.Append      (_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}
	else
	{
		strSQL.Append      (_T(" Select "));
		strSQL.AppendFormat(_T("  Ta.%s, Ta.%s, avg(x), avg(y) "), pIOColumn->Name(), pIOColumn->Name());
		strSQL.Append      (_T(" From Zone Ta, Node Tb "));
		strSQL.Append      (_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{        
		bool bCodeType = pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode ? true:false;
		KCodeGroup*      pCodeData    = NULL;
		KCodeManager*    pCodeManager = pIOTable->GetOwner()->GetCodeManager();

		std::map<int,CString> mapCode;
		if ( bCodeType)
		{			
			pCodeData   = pCodeManager->FindCodeGroup(pIOColumn->CodeGroup());
			pCodeData->GetCodes(mapCode);
		}

		KResultSetPtr    spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			CString strID   = spResultSet->GetValueString(0);
			CString strName = spResultSet->GetValueString(1);
			double  dMapX   = spResultSet->GetValueDouble(2);
			double  dMapY   = spResultSet->GetValueDouble(3);

			TDesireOrigin oTDesireOrigin;		

			oTDesireOrigin.TID   = _ttoi64(strID);
			oTDesireOrigin.TMapX = dMapX;
			oTDesireOrigin.TMapY = dMapY;

			// 명칭
			if (bCodeType)
			{
				std::map<int,CString>::iterator iter = mapCode.find(_ttoi(strID));
				std::map<int,CString>::iterator end  = mapCode.end();

				if (iter != end)
				{
					strName = iter->second;
				}
			}

			oTDesireOrigin.TName = strName;			
			m_vecDesireOrigin.push_back(oTDesireOrigin);
		}

		CXTPReportRecord    * pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		size_t nxCount = m_vecDesireOrigin.size();
		for (size_t i=0; i<nxCount; i++)
		{
			TDesireOrigin& oTDesireOrigin = m_vecDesireOrigin[i];

			pRecord = m_wndReportCtrlOrigin.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)&m_vecDesireOrigin[i]);		

			CString strID;
			strID.Format(_T("%I64d"), oTDesireOrigin.TID);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strID));
			pItem->SetAlignment(xtpColumnTextCenter);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTDesireOrigin.TName));
			pItem->SetAlignment(xtpColumnTextCenter);			
		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(_T("존 정보 조회 중 오류가 발생하였습니다."));
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("존 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 

	m_wndReportCtrlOrigin.Populate();
}


void KPrepareChartODDlg::LoadDestReportData()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_DESTINATION);   
	m_vecDesireDest.clear();
	m_wndReportCtrlDest.ResetContent();

	int nIndex = m_cboDestUnit.GetCurSel();
	if (nIndex < 0) return;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ZONE);
	KIOColumn* pIOColumn = (KIOColumn*)m_cboDestUnit.GetItemData(nIndex);

	CString strSQL;
	// Integer - Primary(zone_id), 그냥숫자값, 코드
	if (pIOColumn->PrimaryKey())
	{
		strSQL.Append(_T(" Select "));
		strSQL.AppendFormat(_T(" Ta.%s, max(Ta.name), avg(x), avg(y) "), pIOColumn->Name());
		strSQL.Append(_T(" From Zone Ta, Node Tb "));
		strSQL.Append(_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}
	else
	{
		strSQL.Append(_T(" Select "));
		strSQL.AppendFormat(_T(" Ta.%s, Ta.%s, avg(x), avg(y) "), pIOColumn->Name(), pIOColumn->Name());
		strSQL.Append(_T(" From Zone Ta, Node Tb "));
		strSQL.Append(_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{        
		bool bCodeType = pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode ? true:false;
		KCodeGroup*      pCodeData    = NULL;
		KCodeManager*    pCodeManager = pIOTable->GetOwner()->GetCodeManager();

		std::map<int,CString> mapCode;
		if ( bCodeType)
		{			
			pCodeData   = pCodeManager->FindCodeGroup(pIOColumn->CodeGroup());
			pCodeData->GetCodes(mapCode);
		}

		KResultSetPtr    spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			CString strID   = spResultSet->GetValueString(0);
			CString strName = spResultSet->GetValueString(1);
			double  dMapX   = spResultSet->GetValueDouble(2);
			double  dMapY   = spResultSet->GetValueDouble(3);

			TDesireDest   oTDesireDest;

			oTDesireDest.TID   = _ttoi64(strID);
			oTDesireDest.TMapX = dMapX;
			oTDesireDest.TMapY = dMapY;

			// 명칭
			if (bCodeType)
			{
				std::map<int,CString>::iterator iter = mapCode.find(_ttoi(strID));
				std::map<int,CString>::iterator end  = mapCode.end();

				if (iter != end)
				{
					strName = iter->second;
				}
			}

			oTDesireDest.TName = strName;	
			m_vecDesireDest.push_back(oTDesireDest);
		}

		CXTPReportRecord    * pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		size_t nxCount = m_vecDesireDest.size();
		for (size_t i=0; i<nxCount; i++)
		{
			TDesireDest& oTDesireDest = m_vecDesireDest[i];

			pRecord = m_wndReportCtrlDest.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)&m_vecDesireDest[i]);

			CString strID;
			strID.Format(_T("%I64d"), oTDesireDest.TID);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strID));
			pItem->SetAlignment(xtpColumnTextCenter);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTDesireDest.TName));
			pItem->SetAlignment(xtpColumnTextCenter);
		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(_T("존 정보 조회 중 오류가 발생하였습니다."));
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("존 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 

	m_wndReportCtrlDest.Populate();
}


void KPrepareChartODDlg::OnCbnSelchangeComboOriginUnit()
{
	LoadOriginReportData();
	UpdateChartData();
}


void KPrepareChartODDlg::OnCbnSelchangeComboDesUnit()
{
	LoadDestReportData();
	UpdateChartData();
}


void KPrepareChartODDlg::OnBnClickedButtonChart()
{
	UpdateData(TRUE);

	try
	{
		if ( !InvalidateInputInfo())
		{
			throw 1;
		}

		if ( !CheckOriginDestinationSelect())
		{
			throw 1;
		}

		UpdateChartData();
	}
	catch(...)
	{
		return;
	}	
}


void KPrepareChartODDlg::UpdateChartData()
{
	TDesireODInfo oTDesireODInfo;

	try
	{
		int       nCur              = m_cboTable.GetCurSel();
		KIOTable* pIOTable          = (KIOTable*)m_cboTable.GetItemData(nCur);
		oTDesireODInfo.strTableName = pIOTable->Caption();

		nCur                        = m_cboField.GetCurSel();
		KIOColumn* pIOColumn        = (KIOColumn*)m_cboField.GetItemData(nCur);
		oTDesireODInfo.strFieldName = pIOColumn->Caption();
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	}

	m_vecDesireMapData.clear();//jyk- 딱히 차트 Control을 Reset시키는방법을 찾지 못하여, 빈 데이터로 차트를 그리는 방법으로 대체함
	CreateChart(m_vecDesireMapData, oTDesireODInfo);

	if ( !InvalidateInputInfo(false))
	{
		return;
	}

	if ( !CheckOriginDestinationSelect(false))
	{
		return;
	}


	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}
	QBicSimpleProgressThread::ExecuteThread(MakeChartDataThreadCaller, this, false, nLang);

	try
	{
		CreateChart(m_vecDesireMapData, oTDesireODInfo);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	} 
}


unsigned __stdcall KPrepareChartODDlg::MakeChartDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("그래프 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Draw Chart..."));
	}

	KPrepareChartODDlg* pDlg = (KPrepareChartODDlg*)pParameter->GetParameter();
	pDlg->GetDesireData();

	return 0;
}


void KPrepareChartODDlg::GetDesireData()
{
	try
	{
		int nIndex = 0;	

		m_vecDesireMapData.clear();

		nIndex = m_cboTable.GetCurSel();
		KIOTable* pODTable           = (KIOTable*)m_cboTable.GetItemData(nIndex);

		nIndex = m_cboField.GetCurSel();
		KIOColumn* pODColumn         = (KIOColumn*)m_cboField.GetItemData(nIndex);

		int nOriginUnitIndex = m_cboOriginUnit.GetCurSel();
		KIOColumn* pOriginUnitColumn = (KIOColumn*)m_cboOriginUnit.GetItemData(nOriginUnitIndex);

		int nDestUnitIndex = m_cboDestUnit.GetCurSel();
		KIOColumn* pDestUnitColumn   = (KIOColumn*)m_cboDestUnit.GetItemData(nDestUnitIndex);

		// 테이블, 필드 선택이 변경되면 클리어 한다.
		KODKey oExistKey(nOriginUnitIndex, nDestUnitIndex);
		std::map<KODKey, std::map<KODKey, double>>::iterator iterInfo = m_mapDesireInfo.find(oExistKey);
		std::map<KODKey, std::map<KODKey, double>>::iterator endInfo  = m_mapDesireInfo.end();

		std::map<KODKey, double> mapDesire;
		if (iterInfo == endInfo)
		{
			if (!KDBaseMatrixView::ODMatrixRecord(pODTable, pOriginUnitColumn->Name(), pDestUnitColumn->Name(), pODColumn->Name(), mapDesire))
			{
				AfxMessageBox(_T("정보 조회중 오류가 발생하였습니다."));
				throw 1;
			}	

			m_mapDesireInfo.insert(std::make_pair(oExistKey, mapDesire));
		}
		else
		{
			mapDesire.insert(iterInfo->second.begin(), iterInfo->second.end());
		}

		CXTPReportRecords* pOriginRecords = m_wndReportCtrlOrigin.GetRecords();
		int nCount = pOriginRecords->GetCount();

		for (int i=0; i<nCount; i++)
		{
			TDesireMapData        oDesireMapData;

			CXTPReportRecord*     pRecord = pOriginRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}

			TDesireOrigin* pTDesireOrigin = (TDesireOrigin*)pItem->GetItemData();

			oDesireMapData.TOrigin = *pTDesireOrigin;
			std::vector<TDesireDest>& vecDestination = oDesireMapData.TVecDestination;

			CXTPReportRecords* pDestRecords = m_wndReportCtrlDest.GetRecords();
			int nDestCount   = pDestRecords->GetCount();
			for (int iDest=0; iDest<nDestCount; iDest++)
			{
				CXTPReportRecord*     pDestRecord = pDestRecords->GetAt(iDest);
				CXTPReportRecordItem* pDestItem   = pDestRecord->GetItem(0);

				if (pDestItem->IsChecked() == FALSE)
				{
					continue;
				}

				TDesireDest* pTDesireDest = (TDesireDest*)pDestItem->GetItemData();

				std::map<KODKey, double>::iterator iter = mapDesire.find(KODKey(pTDesireOrigin->TID, pTDesireDest->TID));
				std::map<KODKey, double>::iterator end  = mapDesire.end();

				if (iter != end)
				{
					double dValue = iter->second;
					pTDesireDest->TValue = dValue;

					vecDestination.push_back(*pTDesireDest);
				}
			}

			m_vecDesireMapData.push_back(oDesireMapData);
		}			
	}
	catch (...)
	{
		m_vecDesireMapData.clear();
		TxLogDebugException();	
	}
}


bool KPrepareChartODDlg::InvalidateInputInfo(bool a_bMessage)
{
	if (m_cboTable.GetCurSel() < 0)
	{
		if (a_bMessage)
		{
			AfxMessageBox(_T("테이블 정보가 존재하지 않습니다."));
			m_cboTable.SetFocus();
		}
		return false;
	}
	//#

	if (m_cboField.GetCurSel() < 0)
	{
		if (a_bMessage)
		{
			AfxMessageBox(_T("컬럼 정보가 존재하지 않습니다."));
			m_cboField.SetFocus();
		}
		return false;
	}
	//#

	if (m_cboOriginUnit.GetCurSel() < 0)
	{
		if (a_bMessage)
		{
			AfxMessageBox(_T("기점 존 정보가 존재하지 않습니다."));
			m_cboOriginUnit.SetFocus();
		}
		return false;
	}
	//#

	if (m_cboDestUnit.GetCurSel() < 0)
	{
		if (a_bMessage)
		{
			AfxMessageBox(_T("종점 존 정보가 존재하지 않습니다."));
			m_cboDestUnit.SetFocus();
		}
		return false;
	}
	//#

	return true;
}


void KPrepareChartODDlg::CreateChart(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo)
{
	m_mapPChartContent.clear();
	m_mapPChartContent.insert(std::make_pair(IDC_RADIO1,   CreateRadarChart(a_vecDesireMapData, a_oTDesireODInfo)));
	m_mapPChartContent.insert(std::make_pair(IDC_RADIO2,   CreateBarChart(a_vecDesireMapData, a_oTDesireODInfo)));
	m_mapPChartContent.insert(std::make_pair(IDC_RADIO3,   CreateAreaChart(a_vecDesireMapData, a_oTDesireODInfo)));

	int nID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3);
	OnBnClickedRadioChart(nID);
}


CXTPChartContent* KPrepareChartODDlg::CreateRadarChart(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo)
{
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent(pContent, CreateTitle(a_oTDesireODInfo));

	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;
		CXTPChartRadarDiagram*     pDiagram          = nullptr;

		for (size_t i= 0; i< a_vecDesireMapData.size(); i++)
		{
			TDesireOrigin  &oTOrigin            = a_vecDesireMapData[i].TOrigin;
			auto           &vecTDestination     = a_vecDesireMapData[i].TVecDestination;

			pSeries = pSeriesCollection->Add(new CXTPChartSeries());
			KChartCtrlSetting::DefaultSeriesRadar(pSeries, oTOrigin.TName);

			pDiagram = (CXTPChartRadarDiagram*)pSeries->GetDiagram();
			KChartCtrlSetting::DefaultDiagramRadar(pDiagram);

			for (size_t k= 0; k< vecTDestination.size(); k++)
			{
				TDesireDest  &oTDesireDest      = vecTDestination[k];

				CString strXAxisZoneID;
				strXAxisZoneID.Format(_T("%I64d"), oTDesireDest.TID);

				CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTDesireDest.TValue);  
				pSeries->GetPoints()->Add(pChartPoint);

				CXTPChartAxisCustomLabel* pXAxisLabel = new CXTPChartAxisCustomLabel();
				pXAxisLabel->SetAxisValue(strXAxisZoneID);

				CString strXAxisZoneIDLegend;
				strXAxisZoneIDLegend.Format(_T("%I64d:%s"), oTDesireDest.TID, oTDesireDest.TName);				
				pXAxisLabel->SetText(strXAxisZoneIDLegend);

				pDiagram->GetAxisX()->GetCustomLabels()->Add(pXAxisLabel);
			}
		}
#pragma region Custom Code
		// Nombar Test Code Start
		// 	pSeries->SetArgumentScaleType(xtpChartScaleQualitative/*xtpChartScaleNumerical*/);
		// 	pDiagram->GetAxisX()->SetGridSpacingAuto(FALSE);
		// 	pDiagram->GetAxisX()->SetGridSpacing(1);
		// 
		// 	CXTPChartSeriesPoint* pChartPoint = new CXTPChartSeriesPoint(1, 100); 
		// 	pSeries->GetPoints()->Add(pChartPoint);
		// 
		// 	for (int i=50; i<53; i++)
		// 	{  
		// 		CString strZone;
		// 		strZone.Format(_T("%d"), i);
		// 
		// 		CXTPChartSeriesPoint* pChartPoint = new CXTPChartSeriesPoint(i, i);  
		// 
		// 		CString strLegend;
		// 		strLegend.Format(_T("%d 존지존"), i);
		// 		//pChartPoint->SetLegentText(strLegend);
		// 
		// 		CXTPChartAxisCustomLabel* pLabel = new CXTPChartAxisCustomLabel();
		// 		pLabel->SetAxisValue(strZone);
		// 		pLabel->SetText(strLegend);
		// 		pDiagram->GetAxisX()->GetCustomLabels()->Add(pLabel);
		// 		pSeries->GetPoints()->Add(pChartPoint);
		// 		//delete pChartPoint;
		// 	}

		//pDiagram->GetAxisX()->GetLabel()->SetFormat(_T("%.0f nombara"));
#pragma endregion Custom Code
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CXTPChartContent* KPrepareChartODDlg::CreateBarChart(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo)
{
	/*CXTPChartContent*          pContent         = m_wndChartControl.GetContent();*/
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent( pContent, CreateTitle(a_oTDesireODInfo) );
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;
		CXTPChartDiagram2D*        pDiagram2D        = nullptr;

		for (size_t i= 0; i< a_vecDesireMapData.size(); i++)
		{
			TDesireOrigin  &oTOrigin                 = a_vecDesireMapData[i].TOrigin;
			auto           &vecTDestination          = a_vecDesireMapData[i].TVecDestination;
			
			pSeries = pSeriesCollection->Add(new CXTPChartSeries());
			KChartCtrlSetting::DefaultSeriesBar(pSeries, oTOrigin.TName);

			CString strXAxisZoneID(_T(""));
			for (size_t k= 0; k< vecTDestination.size(); k++)
			{
				TDesireDest  &oTDesireDest      = vecTDestination[k];
				CString      &strName           = oTDesireDest.TName;

				if(strName.Compare(_T("")) == 0)
					strXAxisZoneID.Format(_T("%I64d"), oTDesireDest.TID);
				else
					strXAxisZoneID.Format(_T("%s : %I64d"), oTDesireDest.TName, oTDesireDest.TID);  // 라벨 각도가 있을 경우 아이디를 뒷쪽으로

				CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTDesireDest.TValue);  
				pSeries->GetPoints()->Add(pChartPoint);
			}
		}

		if (nullptr != pSeries)
		{
			pDiagram2D = (CXTPChartDiagram2D*)pSeries->GetDiagram();
			if (KmzSystem::GetLanguage()==KEMKorea) {
				KChartCtrlSetting::DefaultDiagram2D(pDiagram2D, _T("종점"), a_oTDesireODInfo.strFieldName);
			}
			else {
				KChartCtrlSetting::DefaultDiagram2D(pDiagram2D, _T("Destination"), a_oTDesireODInfo.strFieldName);
			}
		}
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CXTPChartContent* KPrepareChartODDlg::CreateAreaChart(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo)
{
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent(pContent, CreateTitle(a_oTDesireODInfo));
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;
		CXTPChartDiagram2D*        pDiagram2D        = nullptr;

		for (size_t i= 0; i< a_vecDesireMapData.size(); i++)
		{
			TDesireOrigin  &oTOrigin            = a_vecDesireMapData[i].TOrigin;
			auto           &vecTDestination     = a_vecDesireMapData[i].TVecDestination;

			pSeries = pSeriesCollection->Add(new CXTPChartSeries());
			KChartCtrlSetting::DefaultSeriesArea(pSeries, oTOrigin.TName);

			CString strXAxisZoneID(_T(""));
			for (size_t k= 0; k< vecTDestination.size(); k++)
			{
				TDesireDest  &oTDesireDest      = vecTDestination[k];
				CString      &strName           = oTDesireDest.TName; 

				if(strName.Compare(_T("")) == 0)
					strXAxisZoneID.Format(_T("%I64d"), oTDesireDest.TID);
				else
					strXAxisZoneID.Format(_T("%s : %I64d"), oTDesireDest.TName, oTDesireDest.TID);  // 라벨 각도가 있을 경우 아이디를 뒷쪽으로

				CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTDesireDest.TValue);  
				pSeries->GetPoints()->Add(pChartPoint);
			}
		}

		if (nullptr != pSeries)
		{
			pDiagram2D = (CXTPChartDiagram2D*)pSeries->GetDiagram();
			if (KmzSystem::GetLanguage()==KEMKorea) {
				KChartCtrlSetting::DefaultDiagram2D(pDiagram2D, _T("종점"), a_oTDesireODInfo.strFieldName);
			}
			else {
				KChartCtrlSetting::DefaultDiagram2D(pDiagram2D, _T("Destination"), a_oTDesireODInfo.strFieldName);
			}
		}
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CString KPrepareChartODDlg::CreateTitle(TDesireODInfo &a_oTDesireODInfo)
{
	CString strTitle(_T(""));
	strTitle.Format(_T("%s : %s"), a_oTDesireODInfo.strTableName, a_oTDesireODInfo.strFieldName);
	return strTitle;
}


void KPrepareChartODDlg::OnBnClickedRadioChart(UINT nID)
{
	try
	{
		AutoType iter = m_mapPChartContent.find(nID);
		if(iter != m_mapPChartContent.end())
		{
			CXTPChartContent* pContent = iter->second;

			if (nullptr == pContent)
			{
				return;
			}

			m_wndChartControl.SetContent(pContent);
			UpdateLabels();
			UpdateAxisX();
		}
	}
	catch (...)
	{

	}
}


void KPrepareChartODDlg::UpdateLabels()
{
	BOOL bChecked = IsDlgButtonChecked(IDC_CHECK_SHOWLABELS);

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle =  (CXTPChartPointSeriesStyle*)(pSeriesCollection->GetAt(i)->GetStyle());

		pStyle->GetLabel()->SetVisible(bChecked);
	}
}


void KPrepareChartODDlg::UpdateAxisX()
{
    int nID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3);
	BOOL bChecked = IsDlgButtonChecked(IDC_CHECK_SHOWAXISX);

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle   = (CXTPChartPointSeriesStyle*)(pSeriesCollection->GetAt(i)->GetStyle());
        
        if (nID == IDC_RADIO1) {
            CXTPChartRadarDiagram* pDiagram = (CXTPChartRadarDiagram*)(pSeriesCollection->GetAt(i)->GetDiagram());
            CXTPChartAxis* pAxis = pDiagram->GetAxisX();
            if (pAxis) {
                pAxis->SetVisible(bChecked);
            }
        }
        else {
            CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)(pSeriesCollection->GetAt(i)->GetDiagram());
            CXTPChartAxis* pAxis = pDiagram->GetAxisX();
            if (pAxis) {
                pAxis->SetVisible(bChecked);
            }
        }		
	}
}


void KPrepareChartODDlg::OnCheckShowLabels() 
{
	UpdateLabels();
}


void KPrepareChartODDlg::OnCheckShowAxisX() 
{
	UpdateAxisX();
}


void KPrepareChartODDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KPrepareChartODDlg::OnBnClickedCheckOrigin()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_ORIGIN);    

	CXTPReportRecords* pRecords = m_wndReportCtrlOrigin.GetRecords();
	int nCount = pRecords->GetCount();

	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(bCheck);
	}
	m_wndReportCtrlOrigin.RedrawControl();
}


void KPrepareChartODDlg::OnBnClickedCheckDestination()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_DESTINATION);    

	CXTPReportRecords* pRecords = m_wndReportCtrlDest.GetRecords();
	int nCount = pRecords->GetCount();

	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(bCheck);
	}
	m_wndReportCtrlDest.RedrawControl();
}


bool KPrepareChartODDlg::CheckOriginDestinationSelect(bool a_bMessage)
{
	int nCount = 0;
	int nSelectedCount = 0;
	CXTPReportRecords* pRecords = NULL;

	pRecords = m_wndReportCtrlOrigin.GetRecords();
	nCount   = pRecords->GetCount();
	nSelectedCount = 0;
	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked() == TRUE)
		{
			nSelectedCount ++;
		}
	}

	if (nSelectedCount < 1)
	{
		if (a_bMessage)
		{
			AfxMessageBox(_T("선택된 기점 정보가 존재하지 않습니다."));
		}
		
		return false;
	}

	pRecords = m_wndReportCtrlDest.GetRecords();
	nCount   = pRecords->GetCount();
	nSelectedCount = 0;
	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked() == TRUE)
		{
			nSelectedCount ++;
		}
	}

	if (nSelectedCount < 1)
	{
		if (a_bMessage)
		{
			AfxMessageBox(_T("선택된 종점 정보가 존재하지 않습니다."));
		}
		
		return false;
	}

	return true;
}


void KPrepareChartODDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK_ORIGIN, BST_UNCHECKED);
	}
}


void KPrepareChartODDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK_DESTINATION, BST_UNCHECKED);
	}
}