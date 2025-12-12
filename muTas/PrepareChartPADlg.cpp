// PrepareChartPADlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PrepareChartPADlg.h"
#include "afxdialogex.h"

//#include "StringChecker.h"
#include "KmzSystem.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "DBaseMatrixView.h"
#include "PrepareChartPAColumnDlg.h"

const CString CREATE_CHART  = _T("그래프 생성중입니다...");

// KPrepareChartPADlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPrepareChartPADlg, KResizeDialogEx)

KPrepareChartPADlg::KPrepareChartPADlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPrepareChartPADlg::IDD, pParent)
	,  m_pTarget(a_pTarget)
{
	m_pSelectedColumnP = new KIOColumns;
	m_pSelectedColumnC = new KIOColumns;
}

KPrepareChartPADlg::~KPrepareChartPADlg()
{
	delete m_pSelectedColumnP;
	delete m_pSelectedColumnC;
}

void KPrepareChartPADlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_UNIT,    m_cboGroupUnit);
	DDX_Control(pDX, IDC_REPORT_A,              m_wndReportCtrl);
	DDX_Control(pDX, IDC_CHARTCONTROL,          m_wndChartControl);
}


BEGIN_MESSAGE_MAP(KPrepareChartPADlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_CHECK_A_ALL,                 &KPrepareChartPADlg::OnBnClickedCheckAll)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,              &KPrepareChartPADlg::OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_A, &KPrepareChartPADlg::OnReportCheckItem)

	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_UNIT,    &KPrepareChartPADlg::OnCbnSelchangeComboGroupUnit)
	ON_BN_CLICKED(IDC_BUTTON1, &KPrepareChartPADlg::OnBnClickedButtonSelColumP)
	ON_BN_CLICKED(IDC_BUTTON2, &KPrepareChartPADlg::OnBnClickedButtonSelColumC)
	ON_BN_CLICKED(IDC_BUTTON_CHART, &KPrepareChartPADlg::OnBnClickedButtonChart)
	ON_BN_CLICKED(IDCANCEL, &KPrepareChartPADlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_SHOWLABELS, &KPrepareChartPADlg::OnCheckShowLabels)
	ON_BN_CLICKED(IDC_CHECK_SHOWAXISX, &KPrepareChartPADlg::OnCheckShowAxisX)
	
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO2, &KPrepareChartPADlg::OnBnClickedRadioChart)
END_MESSAGE_MAP()


// KPrepareChartPADlg 메시지 처리기입니다.
BOOL KPrepareChartPADlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitialControl();
	LoadReportData();

	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPrepareChartPADlg::InitialControl()
{
	//CString strAttraction = (KmzSystem::IsPassengerSystem() == true) ? ATTRACTION_STRING : CONSUMPTION_STRING;
	CString strAttraction=_T("");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strAttraction = (KmzSystem::IsPassengerSystem() == true) ? _T("유입량") : CONSUMPTION_STRING;
	}
	else {
		strAttraction = (KmzSystem::IsPassengerSystem() == true) ? ATTRACTION_STRING : CONSUMPTION_STRING;
	}
	SetDlgItemText(IDC_STATIC_CONSUMPT, strAttraction + _T(" :"));

	CheckDlgButton(IDC_CHECK_A_ALL, BST_CHECKED);

	KIOTables*  pIOTables  = m_pTarget->Tables();
	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_ZONE);
	const 
    KIOColumns* pIOColumns = pIOTable->Columns();
	
    int nColumnCount = pIOColumns->ColumnCount();

	m_cboGroupUnit.ResetContent();
	int nCount = 0;

	if (KmzSystem::IsPassengerSystem() == true)
	{
		for (int i=0; i<nColumnCount; i++)
		{
			KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
			if (pIOColumn->DataType() ==  KEMIODataTypeInteger)
			{
				int nIndex = m_cboGroupUnit.AddString(pIOColumn->Caption());
				m_cboGroupUnit.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

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

				int nIndex = m_cboGroupUnit.AddString(pIOColumn->Caption());
				m_cboGroupUnit.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

				nCount++;
			}
		}
	}

	if (nCount > 0)
	{
		m_cboGroupUnit.SetCurSel(0);
	}

	KReportCtrlSetting::Default(m_wndReportCtrl);

	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Select"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("ID"),   30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Name"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }

	m_wndReportCtrl.Populate();

	KChartCtrlSetting::DefaultCtrl(m_wndChartControl);

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
}


void KPrepareChartPADlg::LoadReportData()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_A_ALL);   
	m_vecZone.clear();
	m_wndReportCtrl.ResetContent();

	int nIndex = m_cboGroupUnit.GetCurSel();
	if (nIndex < 0) return;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ZONE);
	KIOColumn* pIOColumn = (KIOColumn*)m_cboGroupUnit.GetItemData(nIndex);

	CString strSQL;
	// Integer - Primary(zone_id), 그냥숫자값, 코드
	if (pIOColumn->PrimaryKey())
	{
		strSQL.Append      (_T(" Select "));
		strSQL.AppendFormat(_T("  Ta.%s, max(Ta.name) "), pIOColumn->Name());
		strSQL.Append      (_T(" From Zone Ta, Node Tb "));
		strSQL.Append      (_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}
	else
	{
		strSQL.Append      (_T(" Select "));
		strSQL.AppendFormat(_T("  Ta.%s, Ta.%s "), pIOColumn->Name(), pIOColumn->Name());
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

			TChartZoneDB oTChartZoneDB;
			oTChartZoneDB.nxID   = _ttoi64(strID);

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

			oTChartZoneDB.strName = strName;			
			m_vecZone.push_back(oTChartZoneDB);
		}

		CXTPReportRecord    * pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		size_t nxCount = m_vecZone.size();
		for (size_t i=0; i<nxCount; i++)
		{
			TChartZoneDB& oTChartZoneDB = m_vecZone[i];

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)&m_vecZone[i]);		

			CString strID(_T(""));
			strID.Format(_T("%I64d"), oTChartZoneDB.nxID);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strID));
			pItem->SetAlignment(xtpColumnTextCenter);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTChartZoneDB.strName));
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

	m_wndReportCtrl.Populate();
}


void KPrepareChartPADlg::ResizeComponent()
{
	SetResize(IDC_REPORT_A,				      SZ_TOP_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR1,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CHART,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,						  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR5,				  SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHARTCONTROL,			      SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_CHECK_SHOWLABELS,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SHOWAXISX,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO1,					  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO2,			          SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_EXPLAIN,			  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KPrepareChartPADlg::OnCbnSelchangeComboGroupUnit()
{
	LoadReportData();
	UpdateChartData();
}


void KPrepareChartPADlg::OnBnClickedCheckAll()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_A_ALL);    

	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	int nCount = pRecords->GetCount();

	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(bCheck);
	}
	m_wndReportCtrl.RedrawControl();
}


void KPrepareChartPADlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	//int nIndex = pItemNotify->pColumn->GetItemIndex();
}


void KPrepareChartPADlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK_A_ALL, BST_UNCHECKED);
	}
}


void KPrepareChartPADlg::OnBnClickedButtonSelColumP()
{
	KPrepareChartPAColumnDlg oDlg(m_pTarget, KEMPATypeProduction, m_pSelectedColumnP);
	
	if( IDOK == oDlg.DoModal() )
	{
		CString strDisplayColumns(_T(""));

		for (int i= 0; i< m_pSelectedColumnP->ColumnCount(); i++)
		{
			if( 0 != i )
				strDisplayColumns.Append( _T(", ") );

			KIOColumn* pColumn = m_pSelectedColumnP->GetColumn(i);
			strDisplayColumns.Append( pColumn->Caption() );
		}

		SetDlgItemText(IDC_EDIT_COLUMN_P, strDisplayColumns);

		m_mapProductInfo.clear();

		UpdateChartData();
	}
}


void KPrepareChartPADlg::OnBnClickedButtonSelColumC()
{
	KPrepareChartPAColumnDlg oDlg(m_pTarget, KEMPATypeAttraction, m_pSelectedColumnC);
	
	if( IDOK == oDlg.DoModal() )
	{
		CString strDisplayColumns(_T(""));

		for (int i= 0; i< m_pSelectedColumnC->ColumnCount(); i++)
		{
			if( 0 != i )
				strDisplayColumns.Append( _T(", ") );

			KIOColumn* pColumn = m_pSelectedColumnC->GetColumn(i);
			strDisplayColumns.Append( pColumn->Caption() );
		}

		SetDlgItemText(IDC_EDIT_COLUMN_C, strDisplayColumns);

		m_mapConsumptInfo.clear();

		UpdateChartData();
	}
}


void KPrepareChartPADlg::OnCheckShowLabels() 
{
	UpdateLabels();
}


void KPrepareChartPADlg::OnCheckShowAxisX() 
{
	UpdateAxisX();
}


void KPrepareChartPADlg::OnBnClickedRadioChart(UINT nID)
{
	ShowChart(nID);
}


void KPrepareChartPADlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KPrepareChartPADlg::OnBnClickedButtonChart()
{
	UpdateData(TRUE);
	try
	{
		if ( !InvalidateInputInfo())
			throw 1;

		if ( !CheckPASelect())
			throw 1;

		UpdateChartData();
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


void KPrepareChartPADlg::UpdateChartData()
{
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(MakeChartDataThreadCaller, this, false, nLang);

	try
	{
		CreateChart(m_vecChartZoneDisplayProduct, m_vecChartZoneDisplayConsumpt);
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


unsigned __stdcall KPrepareChartPADlg::MakeChartDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("그래프 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Draw Chart..."));
	}

	KPrepareChartPADlg* pDlg = (KPrepareChartPADlg*)pParameter->GetParameter();
	pDlg->MakeChartData();

	return 0;
}


void KPrepareChartPADlg::MakeChartData()
{
	try
	{
		int        nUnitIndex   = m_cboGroupUnit.GetCurSel();

		std::map<Integer, std::vector<double>> mapProduct;
		std::map<Integer, std::vector<double>> mapConsumpt;

		GetTotalProductionData (nUnitIndex,  mapProduct);
		GetTotalConsumptionData(nUnitIndex,  mapConsumpt);

		m_vecChartZoneDisplayProduct.clear();
		for (int i= 0; i< m_pSelectedColumnP->ColumnCount(); i++)
		{
			KIOColumn* pIOColumn = m_pSelectedColumnP->GetColumn(i);

			TChartZoneDisplay  oTChartZoneDisplay;
			oTChartZoneDisplay.strModelName = pIOColumn->Caption();

			m_vecChartZoneDisplayProduct.push_back(oTChartZoneDisplay);
		}

		m_vecChartZoneDisplayConsumpt.clear();
		for (int i= 0; i< m_pSelectedColumnC->ColumnCount(); i++)
		{
			KIOColumn* pIOColumn = m_pSelectedColumnC->GetColumn(i);

			TChartZoneDisplay  oTChartZoneDisplay;
			oTChartZoneDisplay.strModelName = pIOColumn->Caption();

			m_vecChartZoneDisplayConsumpt.push_back(oTChartZoneDisplay);
		}

		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		int nCount       = pRecords->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}

			TChartZoneDB* pTChartZoneDB = (TChartZoneDB*)pItem->GetItemData();

			if(m_vecChartZoneDisplayProduct.size() > 0)
			{
				AutoType iter  = mapProduct.find(pTChartZoneDB->nxID);
				AutoType itEnd = mapProduct.end();
				if(iter != itEnd)
				{
					std::vector<double> &vecData = iter->second;
					for (size_t k= 0; k< m_vecChartZoneDisplayProduct.size(); k++)
					{
						TChartZoneDB oTChartZoneDB = *pTChartZoneDB;
						oTChartZoneDB.dValue       = vecData[k];

						TChartZoneDisplay          &oTChartZoneDisplay = m_vecChartZoneDisplayProduct[k];
						std::vector<TChartZoneDB>  &vecTChartZoneDB    = oTChartZoneDisplay.vecTChartZoneDB;

						vecTChartZoneDB.push_back(oTChartZoneDB);
					}
				}
			}

			if(m_vecChartZoneDisplayConsumpt.size() > 0)
			{
				AutoType iter  = mapConsumpt.find(pTChartZoneDB->nxID);
				AutoType itEnd = mapConsumpt.end();
				if(iter != itEnd)
				{
					std::vector<double> &vecData = iter->second;
					for (size_t k= 0; k< m_vecChartZoneDisplayConsumpt.size(); k++)
					{
						TChartZoneDB oTChartZoneDB = *pTChartZoneDB;
						oTChartZoneDB.dValue       = vecData[k];

						TChartZoneDisplay          &oTChartZoneDisplay = m_vecChartZoneDisplayConsumpt[k];
						std::vector<TChartZoneDB>  &vecTChartZoneDB    = oTChartZoneDisplay.vecTChartZoneDB;

						vecTChartZoneDB.push_back(oTChartZoneDB);
					}
				}
			}
		}
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


// void KPrepareChartPADlg::UpdateChartData()
// {
// 	try
// 	{
// 		CWaitCursor wc;
// 
// 		int        nUnitIndex   = m_cboGroupUnit.GetCurSel();
// 
// 		std::map<Integer, std::vector<double>> mapProduct;
// 		std::map<Integer, std::vector<double>> mapConsumpt;
// 
// 		GetTotalProductionData (nUnitIndex,  mapProduct);
// 		GetTotalConsumptionData(nUnitIndex,  mapConsumpt);
// 
// 		std::vector<TChartZoneDisplay> vecChartZoneDisplayProduct;
// 		for (int i= 0; i< m_pSelectedColumnP->ColumnCount(); i++)
// 		{
// 			KIOColumn* pIOColumn = m_pSelectedColumnP->GetColumn(i);
// 
// 			TChartZoneDisplay  oTChartZoneDisplay;
// 			oTChartZoneDisplay.strModelName = pIOColumn->Caption();
// 
// 			vecChartZoneDisplayProduct.push_back(oTChartZoneDisplay);
// 		}
// 
// 		std::vector<TChartZoneDisplay> vecChartZoneDisplayConsumpt;
// 		for (int i= 0; i< m_pSelectedColumnC->ColumnCount(); i++)
// 		{
// 			KIOColumn* pIOColumn = m_pSelectedColumnC->GetColumn(i);
// 
// 			TChartZoneDisplay  oTChartZoneDisplay;
// 			oTChartZoneDisplay.strModelName = pIOColumn->Caption();
// 
// 			vecChartZoneDisplayConsumpt.push_back(oTChartZoneDisplay);
// 		}
// 
// 		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
// 		int nCount       = pRecords->GetCount();
// 
// 		for (int i= 0; i< nCount; i++)
// 		{
// 			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
// 			CXTPReportRecordItem* pItem   = pRecord->GetItem(0);
// 
// 			if (pItem->IsChecked() == FALSE)
// 			{
// 				continue;
// 			}
// 
// 			TChartZoneDB* pTChartZoneDB = (TChartZoneDB*)pItem->GetItemData();
// 
// 			if(vecChartZoneDisplayProduct.size() > 0)
// 			{
// 				AutoType iter  = mapProduct.find(pTChartZoneDB->nxID);
// 				AutoType itEnd = mapProduct.end();
// 				if(iter != itEnd)
// 				{
// 					std::vector<double> &vecData = iter->second;
// 					for (size_t k= 0; k< vecChartZoneDisplayProduct.size(); k++)
// 					{
// 						TChartZoneDB oTChartZoneDB = *pTChartZoneDB;
// 						oTChartZoneDB.dValue       = vecData[k];
// 
// 						TChartZoneDisplay          &oTChartZoneDisplay = vecChartZoneDisplayProduct[k];
// 						std::vector<TChartZoneDB>  &vecTChartZoneDB    = oTChartZoneDisplay.vecTChartZoneDB;
// 
// 						vecTChartZoneDB.push_back(oTChartZoneDB);
// 					}
// 				}
// 			}
// 
// 			if(vecChartZoneDisplayConsumpt.size() > 0)
// 			{
// 				AutoType iter  = mapConsumpt.find(pTChartZoneDB->nxID);
// 				AutoType itEnd = mapConsumpt.end();
// 				if(iter != itEnd)
// 				{
// 					std::vector<double> &vecData = iter->second;
// 					for (size_t k= 0; k< vecChartZoneDisplayConsumpt.size(); k++)
// 					{
// 						TChartZoneDB oTChartZoneDB = *pTChartZoneDB;
// 						oTChartZoneDB.dValue       = vecData[k];
// 
// 						TChartZoneDisplay          &oTChartZoneDisplay = vecChartZoneDisplayConsumpt[k];
// 						std::vector<TChartZoneDB>  &vecTChartZoneDB    = oTChartZoneDisplay.vecTChartZoneDB;
// 
// 						vecTChartZoneDB.push_back(oTChartZoneDB);
// 					}
// 				}
// 			}
// 		}
// 
// 		CreateChart(vecChartZoneDisplayProduct, vecChartZoneDisplayConsumpt);
// 	}
// 	catch (KExceptionPtr ex)
// 	{
// 		TxLogDebugException();
// 		AfxMessageBox(ex->GetErrorMessage());
// 		return;
// 	}
// 	catch (...)
// 	{
// 		TxLogDebugException();
// 		return;
// 	} 
// }


void KPrepareChartPADlg::CreateChart(std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayProduct, std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayConsumpt)
{
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicDonkeyProgress oDonkeyProgress(this, CREATE_CHART, TRUE, FALSE, FALSE, nLang);    
	m_mapPChartContent.clear();
	m_mapPChartContent.insert(std::make_pair(IDC_RADIO1,   CreateBarChart (a_vecChartZoneDisplayProduct, a_vecChartZoneDisplayConsumpt)));
	m_mapPChartContent.insert(std::make_pair(IDC_RADIO2,   CreateAreaChart(a_vecChartZoneDisplayProduct, a_vecChartZoneDisplayConsumpt)));

	int nID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	ShowChart(nID);
}


void KPrepareChartPADlg::ShowChart(int nID)
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
		TxLogDebugException();
	}
}


void KPrepareChartPADlg::UpdateLabels()
{
	BOOL bChecked = IsDlgButtonChecked(IDC_CHECK_SHOWLABELS);

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle =  (CXTPChartPointSeriesStyle*)(pSeriesCollection->GetAt(i)->GetStyle());

		pStyle->GetLabel()->SetVisible(bChecked);
	}
}


void KPrepareChartPADlg::UpdateAxisX()
{
	BOOL bChecked = IsDlgButtonChecked(IDC_CHECK_SHOWAXISX);

	CXTPChartSeriesCollection* pSeriesCollection = m_wndChartControl.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle   = (CXTPChartPointSeriesStyle*)(pSeriesCollection->GetAt(i)->GetStyle());
		CXTPChartDiagram2D*        pDiagram = (CXTPChartDiagram2D*)(pSeriesCollection->GetAt(i)->GetDiagram());
		pDiagram->GetAxisX()->SetVisible(bChecked);
	}
}


bool KPrepareChartPADlg::InvalidateInputInfo()
{
	if (m_cboGroupUnit.GetCurSel() < 0)
	{
		AfxMessageBox(_T("그룹 생성 정보가 존재하지 않습니다."));
		m_cboGroupUnit.SetFocus();
		return false;
	}
	//#

	if(m_pSelectedColumnP->ColumnCount() < 1 && m_pSelectedColumnC->ColumnCount() < 1)
	{
		CString strMsg(_T(""));
		CString strAttraction = (KmzSystem::IsPassengerSystem() == true) ? ATTRACTION_STRING : CONSUMPTION_STRING;
		if (KmzSystem::GetLanguage()==KEMKorea) {
			strMsg = _T("유출량/유입량 Column 정보가 없습니다");
		}
		else {
			strMsg.Format(_T("Production/%s Column 정보가 없습니다"), strAttraction);
		}
		AfxMessageBox(strMsg);

		OnBnClickedButtonSelColumP();
		return false;
	}

	return true;
}


bool KPrepareChartPADlg::CheckPASelect()
{
	try
	{
		int nCount(0);
		int nSelectedCount(0);
		CXTPReportRecords* pRecords = nullptr;

		pRecords = m_wndReportCtrl.GetRecords();
		nCount   = pRecords->GetCount();
		nSelectedCount = 0;
		for (int i= 0; i<nCount; i++)
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
			int        nCur      = m_cboGroupUnit.GetCurSel();
			KIOColumn* pIOColumn = (KIOColumn*)m_cboGroupUnit.GetItemData(nCur);
			CString    strMsg(_T(""));
			strMsg.Format(_T("선택된 %s 정보가 없습니다."), pIOColumn->Caption());

			ThrowException(strMsg);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(ex->GetErrorMessage());
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	} 
	
	return true;
}


void KPrepareChartPADlg::GetTotalProductionData(int a_nUnitIndex, std::map<Integer, std::vector<double>> &a_mapProduct)
{
	try
	{
		std::map<int, std::map<Integer, std::vector<double>>>::iterator iterInfo = m_mapProductInfo.find(a_nUnitIndex);
		std::map<int, std::map<Integer, std::vector<double>>>::iterator endInfo  = m_mapProductInfo.end();

		if (iterInfo == endInfo)
		{
			KIOTables* pIOTables = m_pTarget->Tables();
			KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ZONE);
			KIOColumn* pIOColumn = (KIOColumn*)m_cboGroupUnit.GetItemData(a_nUnitIndex);

			CString strSQL(_T(""));
			strSQL.Append      (_T(" Select "));
			strSQL.AppendFormat(_T("  Ta.%s "), pIOColumn->Name());
			for (int i= 0; i< m_pSelectedColumnP->ColumnCount(); i++)
			{
				KIOColumn* pIOSelectedColumn = m_pSelectedColumnP->GetColumn(i);
				strSQL.AppendFormat(_T(" ,SUM(Ta.%s) "), pIOSelectedColumn->Name());
			}
			strSQL.Append      (_T(" From Zone Ta, Node Tb "));
			strSQL.Append      (_T(" Where Ta.zone_id = Tb.node_id "));
			strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());

			KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
      
			KResultSetPtr    spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
			a_mapProduct.clear();
			while ( spResultSet->Next() )
			{
				Integer nxID      = spResultSet->GetValueInt64  (0);
				std::vector<double> vecDValue;
				for (int i= 0; i< m_pSelectedColumnP->ColumnCount(); i++)
				{
					double dValue = spResultSet->GetValueDouble(1+i);
					vecDValue.push_back(dValue);
				}
				a_mapProduct.insert(std::make_pair(nxID, vecDValue));
			}

			m_mapProductInfo.insert(std::make_pair(a_nUnitIndex, a_mapProduct));
		}
		else
		{
			a_mapProduct.insert(iterInfo->second.begin(), iterInfo->second.end());
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		ThrowException(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("존 정보 조회 중 오류가 발생하였습니다."));
	} 
}


void KPrepareChartPADlg::GetTotalConsumptionData(int a_nUnitIndex, std::map<Integer, std::vector<double>> &a_mapConsumpt)
{
	try
	{
		std::map<int, std::map<Integer, std::vector<double>>>::iterator iterInfo = m_mapConsumptInfo.find(a_nUnitIndex);
		std::map<int, std::map<Integer, std::vector<double>>>::iterator endInfo  = m_mapConsumptInfo.end();

		if (iterInfo == endInfo)
		{
			KIOTables* pIOTables = m_pTarget->Tables();
			KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ZONE);
			KIOColumn* pIOColumn  = (KIOColumn*)m_cboGroupUnit.GetItemData(a_nUnitIndex);

			CString strSQL(_T(""));
			strSQL.Append      (_T(" Select "));
			strSQL.AppendFormat(_T("  Ta.%s "), pIOColumn->Name());
			for (int i= 0; i< m_pSelectedColumnC->ColumnCount(); i++)
			{
				KIOColumn* pIOSelectedColumn = m_pSelectedColumnC->GetColumn(i);
				strSQL.AppendFormat(_T(" ,SUM(Ta.%s) "), pIOSelectedColumn->Name());
			}
			strSQL.Append      (_T(" From Zone Ta, Node Tb "));
			strSQL.Append      (_T(" Where Ta.zone_id = Tb.node_id "));
			strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());

			KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
			KResultSetPtr spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
			a_mapConsumpt.clear();
			while ( spResultSet->Next() )
			{
				Integer nxID      = spResultSet->GetValueInt64  (0);
				std::vector<double> vecDValue;
				for (int i= 0; i< m_pSelectedColumnC->ColumnCount(); i++)
				{
					double dValue = spResultSet->GetValueDouble(1+i);
					vecDValue.push_back(dValue);
				}
				a_mapConsumpt.insert(std::make_pair(nxID, vecDValue));
			}

			m_mapConsumptInfo.insert(std::make_pair(a_nUnitIndex, a_mapConsumpt));
		}
		else
		{
			a_mapConsumpt.insert(iterInfo->second.begin(), iterInfo->second.end());
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		ThrowException(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("존 정보 조회 중 오류가 발생하였습니다."));
	} 
}


CXTPChartContent* KPrepareChartPADlg::CreateAreaChart(std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayProduct, std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayConsumpt)
{
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent(pContent, _T(""));
	pContent->SetPanelDirection(xtpChartPanelVertical);

	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;
		
		if(a_vecChartZoneDisplayProduct.size() > 0)
		{
			CXTPChartDiagram2D*        pDiagramA         = (CXTPChartDiagram2D*)pContent->GetPanels()->Add(new CXTPChartDiagram2D());
			KChartCtrlSetting::DefaultDiagram2D(pDiagramA, _T(""), _T(""));
			pDiagramA->SetRotated(FALSE);

			for (size_t i= 0; i< a_vecChartZoneDisplayProduct.size(); i++)
			{
				CString  &strModelName            = a_vecChartZoneDisplayProduct[i].strModelName;
				AutoType &vecTChartZoneDB         = a_vecChartZoneDisplayProduct[i].vecTChartZoneDB;

				pSeries = pSeriesCollection->Add(new CXTPChartSeries());
				KChartCtrlSetting::DefaultSeriesArea(pSeries, strModelName);

				CString strXAxisZoneID(_T(""));
				for (size_t k= 0; k< vecTChartZoneDB.size(); k++)
				{
					TChartZoneDB &oTChartZoneDB   = vecTChartZoneDB[k];
					CString      &strName         = oTChartZoneDB.strName; 

					if(strName.Compare(_T("")) == 0)
						strXAxisZoneID.Format(_T("%I64d"), oTChartZoneDB.nxID);
					else
						strXAxisZoneID.Format(_T("%s : %I64d"), oTChartZoneDB.strName, oTChartZoneDB.nxID); 

					CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTChartZoneDB.dValue);  
					pSeries->GetPoints()->Add(pChartPoint);
				}

				pSeries->SetDiagram(pDiagramA);
			}
		}

		if(a_vecChartZoneDisplayConsumpt.size() > 0)
		{
			CXTPChartDiagram2D*        pDiagramB         = (CXTPChartDiagram2D*)pContent->GetPanels()->Add(new CXTPChartDiagram2D());
			KChartCtrlSetting::DefaultDiagram2D(pDiagramB, _T(""), _T(""));
			pDiagramB->SetRotated(FALSE);

			for (size_t i= 0; i< a_vecChartZoneDisplayConsumpt.size(); i++)
			{
				CString  &strModelName            = a_vecChartZoneDisplayConsumpt[i].strModelName;
				AutoType &vecTChartZoneDB         = a_vecChartZoneDisplayConsumpt[i].vecTChartZoneDB;

				pSeries = pSeriesCollection->Add(new CXTPChartSeries());
				KChartCtrlSetting::DefaultSeriesArea(pSeries, strModelName);

				CString strXAxisZoneID(_T(""));
				for (size_t k= 0; k< vecTChartZoneDB.size(); k++)
				{
					TChartZoneDB &oTChartZoneDB   = vecTChartZoneDB[k];
					CString      &strName         = oTChartZoneDB.strName; 

					if(strName.Compare(_T("")) == 0)
						strXAxisZoneID.Format(_T("%I64d"), oTChartZoneDB.nxID);
					else
						strXAxisZoneID.Format(_T("%s : %I64d"), oTChartZoneDB.strName, oTChartZoneDB.nxID); 

					CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTChartZoneDB.dValue);  
					pSeries->GetPoints()->Add(pChartPoint);
				}

				pSeries->SetDiagram(pDiagramB);
			}
		}
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CXTPChartContent* KPrepareChartPADlg::CreateBarChart(std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayProduct, std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayConsumpt)
{
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent(pContent, _T(""));
	pContent->SetPanelDirection(xtpChartPanelVertical);

	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;

		if(a_vecChartZoneDisplayProduct.size() > 0)
		{
			CXTPChartDiagram2D*        pDiagramA         = (CXTPChartDiagram2D*)pContent->GetPanels()->Add(new CXTPChartDiagram2D());
			KChartCtrlSetting::DefaultDiagram2D(pDiagramA, _T(""), _T(""));
			pDiagramA->SetRotated(FALSE);

			for (size_t i= 0; i< a_vecChartZoneDisplayProduct.size(); i++)
			{
				CString  &strModelName            = a_vecChartZoneDisplayProduct[i].strModelName;
				AutoType &vecTChartZoneDB         = a_vecChartZoneDisplayProduct[i].vecTChartZoneDB;

				pSeries = pSeriesCollection->Add(new CXTPChartSeries());
				KChartCtrlSetting::DefaultSeriesBar(pSeries, strModelName);

				CString strXAxisZoneID(_T(""));
				for (size_t k= 0; k< vecTChartZoneDB.size(); k++)
				{
					TChartZoneDB &oTChartZoneDB   = vecTChartZoneDB[k];
					CString      &strName         = oTChartZoneDB.strName; 

					if(strName.Compare(_T("")) == 0)
						strXAxisZoneID.Format(_T("%I64d"), oTChartZoneDB.nxID);
					else
						strXAxisZoneID.Format(_T("%s : %I64d"), oTChartZoneDB.strName, oTChartZoneDB.nxID); 

					CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTChartZoneDB.dValue);  
					pSeries->GetPoints()->Add(pChartPoint);
				}

				pSeries->SetDiagram(pDiagramA);
			}
		}

		if(a_vecChartZoneDisplayConsumpt.size() > 0)
		{
			CXTPChartDiagram2D*        pDiagramB         = (CXTPChartDiagram2D*)pContent->GetPanels()->Add(new CXTPChartDiagram2D());
			KChartCtrlSetting::DefaultDiagram2D(pDiagramB, _T(""), _T(""));
			pDiagramB->SetRotated(FALSE);

			for (size_t i= 0; i< a_vecChartZoneDisplayConsumpt.size(); i++)
			{
				CString  &strModelName            = a_vecChartZoneDisplayConsumpt[i].strModelName;
				AutoType &vecTChartZoneDB         = a_vecChartZoneDisplayConsumpt[i].vecTChartZoneDB;

				pSeries = pSeriesCollection->Add(new CXTPChartSeries());
				KChartCtrlSetting::DefaultSeriesBar(pSeries, strModelName);

				CString strXAxisZoneID(_T(""));
				for (size_t k= 0; k< vecTChartZoneDB.size(); k++)
				{
					TChartZoneDB &oTChartZoneDB   = vecTChartZoneDB[k];
					CString      &strName         = oTChartZoneDB.strName; 

					if(strName.Compare(_T("")) == 0)
						strXAxisZoneID.Format(_T("%I64d"), oTChartZoneDB.nxID);
					else
						strXAxisZoneID.Format(_T("%s : %I64d"), oTChartZoneDB.strName, oTChartZoneDB.nxID); 

					CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTChartZoneDB.dValue);  
					pSeries->GetPoints()->Add(pChartPoint);
				}

				pSeries->SetDiagram(pDiagramB);
			}
		}
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}