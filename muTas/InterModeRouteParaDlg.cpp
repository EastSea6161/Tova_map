// InterModeRouteParaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterModeRouteParaDlg.h"
#include "afxdialogex.h"
#include "Target.h"

//#include "CodeManager.h"
//#include "CodeGroup.h"

// KInterModeRouteParaDlg 대화 상자입니다.

const int MODE_COST = 1;
const int MODE_TIME = 2;

IMPLEMENT_DYNAMIC(KInterModeRouteParaDlg, KResizeDialogEx)

KInterModeRouteParaDlg::KInterModeRouteParaDlg(KTarget* a_pTarget, TMRParamter a_oMRPara, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterModeRouteParaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_oMRParamter(a_oMRPara)
{

}

KInterModeRouteParaDlg::~KInterModeRouteParaDlg()
{
}

void KInterModeRouteParaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_URBAN, m_wndReportCtrlUrban);
	DDX_Control(pDX, IDC_REPORT_REGION, m_wndReportCtrlRegional);
	DDX_Control(pDX, IDC_REPORT_Dummy, m_wndReportCtrlDummy);
}


BEGIN_MESSAGE_MAP(KInterModeRouteParaDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KInterModeRouteParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterModeRouteParaDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KInterModeRouteParaDlg 메시지 처리기입니다.


BOOL KInterModeRouteParaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrlUrban, TRUE);
	KReportCtrlSetting::Default(m_wndReportCtrlRegional, TRUE);
	KReportCtrlSetting::Default(m_wndReportCtrlDummy, TRUE);
	
	InitUrbanReportHeader();
	InitRegionReportHeader();
	InitDummyReportHeader();

	UpdateUrbanReportData();
	UpdateRegionReportData();
	UpdateDummyReportData();

	CString strValue(_T(""));
	strValue.Format(_T("%.5f"), m_oMRParamter.dMEUParameter);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_UMEU);
	SetDlgItemText(IDC_EDIT_UMEU, strValue);

	strValue.Format(_T("%.5f"), m_oMRParamter.dAEParameter);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RMEU);
	SetDlgItemText(IDC_EDIT_RMEU, strValue);

	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterModeRouteParaDlg::ResizeComponent()
{
	try
	{
// 		SetResize(IDC_REPORT_URBAN,		      CXTPResizePoint(0, 0),   CXTPResizePoint(1, 1));
// 
// 		SetResize(IDC_SEPRATOR5,			  CXTPResizePoint(1, 0),   CXTPResizePoint(1, 1));
// 
// 		SetResize(IDC_STATIC_REGION,		  CXTPResizePoint(1, 0),   CXTPResizePoint(1, 0));
// 		SetResize(IDC_STATIC_Urban3,		  CXTPResizePoint(1, 0),   CXTPResizePoint(1, 0));
// 		SetResize(IDC_EDIT_RMEU,		      CXTPResizePoint(1, 0),   CXTPResizePoint(1, 0));
// 		SetResize(IDC_REPORT_REGION,		  CXTPResizePoint(1, 0),   CXTPResizePoint(1, 0.5));
// 		SetResize(IDC_SEPRATOR2,			  CXTPResizePoint(1, 0.5), CXTPResizePoint(1, 0.5));
// 
// 		SetResize(IDC_STATIC,		          CXTPResizePoint(1, 0.5), CXTPResizePoint(1, 0.5));
// 		SetResize(IDC_REPORT_Dummy,			  CXTPResizePoint(1, 0.5), CXTPResizePoint(1, 1));
// 
// 		SetResize(IDC_SEPRATOR1,			  CXTPResizePoint(0, 1),   CXTPResizePoint(1, 1));
// 
// 		SetResize(IDOK,					      CXTPResizePoint(1, 1),   CXTPResizePoint(1, 1));
// 		SetResize(IDCANCEL,					  CXTPResizePoint(1, 1),   CXTPResizePoint(1, 1));

		SetResize(IDC_REPORT_URBAN,		      SZ_TOP_LEFT,       SZ_BOTTOM_RIGHT);

		SetResize(IDC_SEPRATOR5,			  SZ_TOP_RIGHT,      SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC_REGION,		  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
		SetResize(IDC_STATIC_Urban3,		  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
		SetResize(IDC_EDIT_RMEU,		      SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
		SetResize(IDC_REPORT_REGION,		  SZ_TOP_RIGHT,     SZ_MIDDLE_RIGHT);
		SetResize(IDC_SEPRATOR2,			  SZ_MIDDLE_RIGHT,  SZ_MIDDLE_RIGHT);

		SetResize(IDC_STATIC,		          SZ_MIDDLE_RIGHT,  SZ_MIDDLE_RIGHT);
		SetResize(IDC_REPORT_Dummy,			  SZ_MIDDLE_RIGHT,  SZ_BOTTOM_RIGHT);

		SetResize(IDC_SEPRATOR1,			  SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);

		SetResize(IDOK,					      SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModeRouteParaDlg::InitUrbanReportHeader()
{
	try
	{
		std::map<KIOColumn*, TUrbanModeParameter> mapUrbanPara;
		mapUrbanPara = m_oMRParamter.mapUrbanParameter;
		
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(0, _T("Mode"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);

		int i = 1;
		AutoType iter = mapUrbanPara.begin();
		AutoType end  = mapUrbanPara.end();
		while(iter != end)
		{
			KIOColumn* pParaColumn = iter->first;

			pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(i, pParaColumn->Caption(), 50));
			pColumn->SetHeaderAlignment(DT_CENTER);
			i++;
			++iter;
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

void KInterModeRouteParaDlg::InitRegionReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlRegional.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrlRegional.AddColumn(new CXTPReportColumn(1, _T("총목적"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
}


void KInterModeRouteParaDlg::InitDummyReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlDummy.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrlDummy.AddColumn(new CXTPReportColumn(1, _T("총목적"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
}


void KInterModeRouteParaDlg::UpdateUrbanReportData()
{
	try
	{
		std::map<KIOColumn*, TUrbanModeParameter> mapUrbanPara;
		mapUrbanPara = m_oMRParamter.mapUrbanParameter;

		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;


		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Auto_Time")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Auto_Cost")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Auto_ParkingFee")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Taxi_Time")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Taxi_Cost")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Transit_InvehTime")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Transit_WatingTime")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Transit_TransferTime")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Transit_AccessWalk")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Transit_EgressWalk")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Transit_TransferNo")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Transit_Cost")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Dummy_Auto")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Dummy_Taxi")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Dummy_Bus")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Dummy_Subway")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Dummy_Bus+Subway")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Dummy_Park&Ride")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Dummy_Kiss&Ride")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Dummy_Taxi&Transit")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);


		AutoType iter = mapUrbanPara.begin();
		AutoType end  = mapUrbanPara.end();
		while(iter != end)
		{
			pRecord  = nullptr;
			TUrbanModeParameter oParameter = iter->second;

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(0);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dAutoTime, _T("%.4f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(1);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dAutoCost, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(2);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dAutoParkingFee, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(3);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dTaxiTime, _T("%.4f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(4);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dTaxiCost, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(5);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dTrInvehiTime, _T("%.8f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(6);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dTrWaitingTime, _T("%.8f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(7);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dTrTransferTime, _T("%.8f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(8);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dTrAccessWalk, _T("%.8f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(9);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dTrEgressWalk, _T("%.8f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(10);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.nTrTransferNo, _T("%.0f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(11);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dTrCost, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(12);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dDumAuto, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(13);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dDumTaxi, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(14);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dDumBus, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(15);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dDumSubway, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(16);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dDumBusSubway, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(17);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dDumParkRide, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(18);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dDumKissRide, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);

			pRecord = m_wndReportCtrlUrban.GetRecords()->GetAt(19);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oParameter.dDumTaxiTransit, _T("%.6f")));
			pItem->SetAlignment(DT_RIGHT);


			++iter;
		}

		m_wndReportCtrlUrban.Populate();
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

void KInterModeRouteParaDlg::UpdateRegionReportData()
{
	try
	{
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		std::vector<TRegionMEUTimeCost> vecModeCost;
		vecModeCost = m_oMRParamter.vecModeCost;

		size_t nCount = vecModeCost.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TRegionMEUTimeCost oTimeCostInfo= vecModeCost[i];
			
			CString strTimeCaption (_T(""));
			CString strCostCaption (_T(""));

			strTimeCaption.Format(_T("%s_Time"), oTimeCostInfo.strMode);
			strCostCaption.Format(_T("%s_Cost"), oTimeCostInfo.strMode);

			pRecord = m_wndReportCtrlRegional.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strCostCaption));
			pItem->SetItemData(oTimeCostInfo.nCode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTimeCostInfo.dCost, _T("%.6f")));
			pItem->SetItemData(MODE_COST);
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnTextRight);

			pRecord = m_wndReportCtrlRegional.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strTimeCaption));
			pItem->SetItemData(oTimeCostInfo.nCode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTimeCostInfo.dTime, _T("%.5f")));
			pItem->SetItemData(MODE_TIME);
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnTextRight);
		}

		m_wndReportCtrlRegional.Populate();

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

void KInterModeRouteParaDlg::UpdateDummyReportData()
{
	try
	{
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		std::vector<TRegionDummy> vecDummy;
		vecDummy = m_oMRParamter.vecDummy;

		size_t nCount = vecDummy.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TRegionDummy oDummy = vecDummy[i];

			CString strCaption(_T(""));
			strCaption.Format(_T("Dummy_%s"), oDummy.strMode);

			pRecord = m_wndReportCtrlDummy.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strCaption));
			pItem->SetItemData(oDummy.nCode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oDummy.dCost, _T("%.3f")));
			pItem->SetItemData(oDummy.nCode);
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnTextRight);
		}

		m_wndReportCtrlDummy.Populate();
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


void KInterModeRouteParaDlg::OnBnClickedOk()
{
	try
	{
		CString strMEU;
		CString strAEMEU;
		GetDlgItemText(IDC_EDIT_UMEU, strMEU);
		GetDlgItemText(IDC_EDIT_RMEU, strAEMEU);

		m_oMRParamter.dMEUParameter = _ttof(strMEU);
		m_oMRParamter.dAEParameter	= _ttof(strAEMEU);

		SaveResultUrbanData();
		SaveResultRegionalData();
		SaveReultDummyData();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}


	KResizeDialogEx::OnOK();
}


void KInterModeRouteParaDlg::OnBnClickedCancel()
{
	try
	{
		
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	KResizeDialogEx::OnCancel();
}

void KInterModeRouteParaDlg::SaveResultUrbanData()
{
	try
	{
		std::map<KIOColumn*, TUrbanModeParameter>& mapUrbanPara = m_oMRParamter.mapUrbanParameter;

		CXTPReportRecord*			pRecord = nullptr;
		CXTPReportRecordItem*		pItem   = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;


		AutoType iter = mapUrbanPara.begin();
		AutoType end  = mapUrbanPara.end();
		while(iter != end)
		{
			KIOColumn* pPurposeColumn = iter->first;
			TUrbanModeParameter& oParameter = iter->second;

			int nIndex = 0;
			CXTPReportColumns* pColumns = m_wndReportCtrlUrban.GetColumns();
			int nCount = pColumns->GetCount();
			for (int i = 0; i < nCount; i++)
			{
				CXTPReportColumn*  pColumn  =  pColumns->GetAt(i);
				CString stra = pColumn->GetCaption();
				CString strb = pPurposeColumn->Caption();
				if (pColumn->GetCaption() == pPurposeColumn->Caption())
				{
					nIndex = i;
				}
			}
				
			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(0);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dAutoTime		= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(1);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dAutoCost		= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(2);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dAutoParkingFee	= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(3);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dTaxiTime		= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(4);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dTaxiCost		= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(5);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dTrInvehiTime	= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(6);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dTrWaitingTime	= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(7);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dTrTransferTime	= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(8);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dTrAccessWalk	= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(9);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dTrEgressWalk	= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(10);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.nTrTransferNo	= (int)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(11);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dTrCost			= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(12);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dDumAuto			= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(13);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dDumTaxi			= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(14);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dDumBus			= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(15);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dDumSubway		= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(16);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dDumBusSubway	= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(17);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dDumParkRide		= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(18);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dDumKissRide		= (double)pItemNum->GetValue();

			pRecord						= m_wndReportCtrlUrban.GetRecords()->GetAt(19);
			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(nIndex);
			oParameter.dDumTaxiTransit	= (double)pItemNum->GetValue();

			++iter;
		}

		m_oMRParamter.mapUrbanParameter = mapUrbanPara;

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

void KInterModeRouteParaDlg::SaveResultRegionalData()
{
	try
	{
		std::vector<TRegionMEUTimeCost>& vecModeCost = m_oMRParamter.vecModeCost;

		CXTPReportRecords*          pRecords = m_wndReportCtrlRegional.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;

		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{

			pRecord = pRecords->GetAt(i);
			
			CString strModeName(_T(""));
			pItem		= (CXTPReportRecordItem*)pRecord->GetItem(0);
			int nCode   = pItem->GetItemData();

			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			int nType		= pItemNum->GetItemData();
			double dValue	= (double)pItemNum->GetValue();

			size_t nCount = vecModeCost.size();
			for (size_t i = 0; i < nCount;i++)
			{
				TRegionMEUTimeCost& oCostInfo = vecModeCost[i];
				if (oCostInfo.nCode == nCode)
				{
					if (nType == MODE_TIME)
					{
						oCostInfo.dTime = dValue;
					}
					else
					{
						oCostInfo.dCost = dValue;
					}
				}
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
}

void KInterModeRouteParaDlg::SaveReultDummyData()
{
	try
	{
		std::vector<TRegionDummy> vecDummy;

		CXTPReportRecords*          pRecords = m_wndReportCtrlDummy.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;

		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			TRegionDummy oDummy;
			pRecord = pRecords->GetAt(i);
			pItem		= (CXTPReportRecordItem*)pRecord->GetItem(0);
			int nCode   = pItem->GetItemData();

			oDummy.nCode = nCode;
			if (nCode < 1)
			{
				oDummy.strMode = _T("Auto");
			}
			else
			{
				oDummy.strMode = pCodeGroup->SingleCodeValue(nCode);
			}

			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			int nType		= pItemNum->GetItemData();
			oDummy.dCost	= (double)pItemNum->GetValue();
			vecDummy.push_back(oDummy);
		}
		m_oMRParamter.vecDummy = vecDummy;
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

void KInterModeRouteParaDlg::GetModeRouteParameter( TMRParamter& a_oMRParameter )
{
	a_oMRParameter = m_oMRParamter;
}