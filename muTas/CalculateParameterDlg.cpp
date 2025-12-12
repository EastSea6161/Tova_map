// CalculatParameterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CalculateParameterDlg.h"
#include "afxdialogex.h"
#include "KExRecordItem.h"
#include "Target.h"
#include "ParameterDefineCostDlg.h"
#include "ParameterDefineTime.h"
#include "Project.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "WriteCodeAgrument.h"
#include "Launcher.h"
#include "KxBulkDbase.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "BulkDBaseTurn.h"
#include "ChildFrm.h"
#include "MainFrameWnd.h"
#include "IOView.h"
#include "IOTableController.h"
#include "DBaseCheckData.h"
#include "ImChampDir.h"

enum
{
	_0_COLUMN_SELECT = 0,
	_1_COLUMN_MODE,
	_2_COLUMN_INVEH_TIME,
	_3_COLUMN_WAIT_TIME,
	_4_COLUMN_TRANSFER_TIME,
	_5_COLUMN_ACCESS_TIME,
	_6_COLUMN_EGRESS_TIME,
	_7_COLUMN_COST,
	_8_COLUMN_LENGTH,
	_9_COLUMN_NOTRANSFER,
	_10_COLUMN_NOSTOP
};

// KCalculatParameterDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCalculatParameterDlg, KResizeDialogEx)

KCalculatParameterDlg::KCalculatParameterDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KCalculatParameterDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_bUseTransit(true),
    m_bApplyTurn(false)
{

}

KCalculatParameterDlg::~KCalculatParameterDlg()
{
}

void KCalculatParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wmReportControl);
}


BEGIN_MESSAGE_MAP(KCalculatParameterDlg, KResizeDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItem)
	ON_BN_CLICKED(IDC_BUTTON_COST, &KCalculatParameterDlg::OnBnClickedButtonCost)
	ON_BN_CLICKED(IDC_BUTTON_TIME, &KCalculatParameterDlg::OnBnClickedButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &KCalculatParameterDlg::OnBnClickedButtonRun)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON_VIEW, &KCalculatParameterDlg::OnBnClickedButtonView)
END_MESSAGE_MAP()


// KCalculatParameterDlg 메시지 처리기입니다.


BOOL KCalculatParameterDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		ResizeComponent();

		if (false == KDBaseCheckData::GetCheckAccessStopCount(m_pTarget))
		{
			CString strMsg;
			strMsg.Format(_T("버스/도시철도/버스+도시철도 관련 기종점간 통행속성 데이터 생성을 위해 \r\n경로 만들기 > 대중교통 > 이용가능 역/정류장 생성을 먼저 실행해 주세요."));
			AfxMessageBox(strMsg);
			m_bUseTransit= false;
		}

		if (false == KDBaseCheckData::GetCheckTransitCostdata(m_pTarget))
		{
			CString strMsg;
			strMsg.Format(_T("버스/도시철도/버스+도시철도 관련 기종점간 통행속성 데이터 생성을 위해 \r\n데이터 > 도시부 대중교통 수단 설정을 먼저 실행해 주세요."));
			AfxMessageBox(strMsg);
			m_bUseTransit= false;
		}

		KReportCtrlSetting::Default(m_wmReportControl, TRUE);
		InitReportHeader();
		InitBaseData();
		UpdateReportControl();

        ((CButton*)GetDlgItem(IDC_BUTTON_COST))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_TIME))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_VIEW))->EnableWindow(FALSE);

		InitAutoCost();
		InitTransitCost();
		InitTaxiCost();
		InitTruckCost();

		m_argFilePath = ImChampDir::GetAppDirectory() + _T("\\x64\\ODParameter.arg");
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return FALSE;
	}
	catch(...)
	{
		TxLogDebugException();
		return FALSE;
	}
	return TRUE;  
}

void KCalculatParameterDlg::ResizeComponent()
{

	SetResize(IDC_REPORT,	 	  SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_COST,	  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_TIME,	  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_RUN,	  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
    SetResize(IDC_CHECK1,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_VIEW,	  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,			  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1,	  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);	
}

void KCalculatParameterDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T("Select"), 50, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_1_COLUMN_MODE, _T("Mode"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("수단명"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_2_COLUMN_INVEH_TIME, _T("InVeh Time"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("차내통행시간"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_3_COLUMN_WAIT_TIME, _T("Wait Time"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("대기시간"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_4_COLUMN_TRANSFER_TIME, _T("Transfer Time"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("환승시간"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_5_COLUMN_ACCESS_TIME, _T("Access Time"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("출발 접근시간"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_6_COLUMN_EGRESS_TIME, _T("Egress Time"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("도착 접근시간"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_7_COLUMN_COST, _T("Cost"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행비용"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_8_COLUMN_LENGTH, _T("Length"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행거리"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_9_COLUMN_NOTRANSFER, _T("No.of Transfer"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("환승횟수"));
        }

		pColumn = m_wmReportControl.AddColumn(new CXTPReportColumn(_10_COLUMN_NOSTOP, _T("No.of Stop"), 45));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("역/정류장 개수"));
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



void KCalculatParameterDlg::InitBaseData()
{
	try
	{
		m_vecOutputInfo.clear();

		//Auto
		TParaOutputInfo oAutoInfo;
		oAutoInfo.bUseData		= false;
		oAutoInfo.nMode			= P_MODE_AUTO;
		//oAutoInfo.strMode		= _T("Auto");
		//InitOutputInfo(oAutoInfo, _T("Auto"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oAutoInfo.strMode = _T("Auto");
			InitOutputInfo(oAutoInfo, _T("Auto"));
		}
		else {
			oAutoInfo.strMode = _T("PassengerCar");
			InitOutputInfo(oAutoInfo, _T("PassengerCar"));
		}
		GetColumnName(oAutoInfo);
		m_vecOutputInfo.push_back(oAutoInfo);

		//Taxi
		TParaOutputInfo oTaxiInfo;
		oTaxiInfo.bUseData		= false;
		oTaxiInfo.nMode			= P_MODE_TAXI;
		oTaxiInfo.strMode		= _T("Taxi");
		InitOutputInfo(oTaxiInfo, _T("Taxi"));
		GetColumnName(oTaxiInfo);
		m_vecOutputInfo.push_back(oTaxiInfo);

		//Truck
		TParaOutputInfo oTruckInfo;
		oTruckInfo.bUseData		= false;
		oTruckInfo.nMode		= P_MODE_TRUCK;
		oTruckInfo.strMode		= _T("Truck");
		InitOutputInfo(oTruckInfo, _T("Truck"));
		GetColumnName(oTruckInfo);
		m_vecOutputInfo.push_back(oTruckInfo);

		//Bus
		TParaOutputInfo oBusInfo;
		oBusInfo.bUseData		= false;
		oBusInfo.nMode			= P_MODE_BUS;
		oBusInfo.strMode		= _T("Bus");
		InitOutputInfo(oBusInfo, _T("Bus"));
		GetColumnName(oBusInfo);
		m_vecOutputInfo.push_back(oBusInfo);

		//Rail
		TParaOutputInfo oRailInfo;
		oRailInfo.bUseData		= false;
		oRailInfo.nMode			= P_MODE_RAIL;
		oRailInfo.strMode		= _T("Rail");
		InitOutputInfo(oRailInfo, _T("Rail"));
		GetColumnName(oRailInfo);
		m_vecOutputInfo.push_back(oRailInfo);

		//Bus+Rail
		TParaOutputInfo oBusRailInfo;
		oBusRailInfo.bUseData		= false;
		oBusRailInfo.nMode			= P_MODE_BUS_RAIL;
		oBusRailInfo.strMode		= _T("Bus+Rail");
		InitOutputInfo(oBusRailInfo, _T("BR"));
		GetColumnName(oBusRailInfo);
		m_vecOutputInfo.push_back(oBusRailInfo);

		//Park&Ride
		TParaOutputInfo oParkRideInfo;
		oParkRideInfo.bUseData		= false;
		oParkRideInfo.nMode			= P_MODE_PARK_RIDE;
		oParkRideInfo.strMode		= _T("Park_Ride");
		InitOutputInfo(oParkRideInfo, _T("PR"));
		GetColumnName(oParkRideInfo);
		m_vecOutputInfo.push_back(oParkRideInfo);

		//Kiss&Ride
		TParaOutputInfo oKissRideInfo;
		oKissRideInfo.bUseData		= false;
		oKissRideInfo.nMode			= P_MODE_KISS_RIDE;
		oKissRideInfo.strMode		= _T("Kiss_Ride");
		InitOutputInfo(oKissRideInfo, _T("KR"));
		GetColumnName(oKissRideInfo);
		m_vecOutputInfo.push_back(oKissRideInfo);

		//Taxi&Transit
		TParaOutputInfo oTaxiTransitInfo;
		oTaxiTransitInfo.bUseData		= false;
		oTaxiTransitInfo.nMode			= P_MODE_TAXI_TRANSIT;
		oTaxiTransitInfo.strMode		= _T("Taxi_Transit");
		InitOutputInfo(oTaxiTransitInfo, _T("TT"));
		GetColumnName(oTaxiTransitInfo);
		m_vecOutputInfo.push_back(oTaxiTransitInfo);

		//Define Time
		m_nTimeMethod = 1;
		
		m_oAssignInfo.dDWTimeBusstop		= 0.25;
		m_oAssignInfo.dDwTimeRailStation	= 0.5;
		m_oAssignInfo.nInvehicle			= 1;
		m_oAssignInfo.nWating				= 1;
		m_oAssignInfo.nTransfer				= 1;
		m_oAssignInfo.nAETime				= 1;

		m_pIOColumnUseLink = nullptr;
		

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

void KCalculatParameterDlg::InitOutputInfo( TParaOutputInfo& a_oParaOutputInfo, CString strHeader )
{
	try
	{
        CString strCol(_T(""));

		a_oParaOutputInfo.nInVehTime		= 0;
        a_oParaOutputInfo.strInVehTime.Format(_T("%s_InVehTime"),strHeader);

		a_oParaOutputInfo.nWaitTime			= 0;
        a_oParaOutputInfo.strWaitTime.Format(_T("%s_WaitTime"), strHeader);

		a_oParaOutputInfo.nTransferTime		= 0;
        a_oParaOutputInfo.strTransferTime.Format(_T("%s_TransTime"), strHeader);

		a_oParaOutputInfo.nAccessTime		= 0;
        a_oParaOutputInfo.strAccessTime.Format(_T("%s_AccTime"), strHeader);

		a_oParaOutputInfo.nEgressTime		= 0;
        a_oParaOutputInfo.strEgressTime.Format(_T("%s_EgrTime"), strHeader);

		a_oParaOutputInfo.nCost				= 0;
        a_oParaOutputInfo.strCost.Format(_T("%s_Cost"), strHeader);

		a_oParaOutputInfo.nLength			= 0;
        a_oParaOutputInfo.strLength.Format(_T("%s_Length"), strHeader);

		a_oParaOutputInfo.nNoTransfer		= 0;
        a_oParaOutputInfo.strNoTransfer.Format(_T("%s_NoTrans"), strHeader);

		a_oParaOutputInfo.nNoStop		    = 0;
        a_oParaOutputInfo.strNoStop.Format(_T("%s_NoStop"), strHeader);
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

void KCalculatParameterDlg::UpdateReportControl()
{
	try
	{
		int nCount = m_vecOutputInfo.size();

		for (int i = 0; i < nCount ; i++)
		{

			CXTPReportRecord*		pRecord = m_wmReportControl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			TParaOutputInfo oInfo = m_vecOutputInfo[i];

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			if (oInfo.nMode == P_MODE_KISS_RIDE || oInfo.nMode == P_MODE_PARK_RIDE || oInfo.nMode == P_MODE_TAXI_TRANSIT)
			{
				pItem->SetEditable(FALSE);
			}

			if (m_bUseTransit == false)
			{
				if(oInfo.nMode == P_MODE_BUS || oInfo.nMode == P_MODE_RAIL || oInfo.nMode == P_MODE_BUS_RAIL)
				{
					pItem->SetEditable(FALSE);
				}
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strMode));
			pItem->SetItemData(oInfo.nMode);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strInVehTime));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			if (P_MODE_AUTO == oInfo.nMode || P_MODE_TAXI == oInfo.nMode || P_MODE_TRUCK == oInfo.nMode)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strCost));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strLength));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strWaitTime));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strTransferTime));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strAccessTime));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strEgressTime));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strCost));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strLength));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strNoTransfer));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}

			if (oInfo.nMode == P_MODE_BUS || oInfo.nMode == P_MODE_BUS_RAIL )
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.strNoStop));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
		}

		m_wmReportControl.Populate();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}	

    CheckButtonState();
}

void KCalculatParameterDlg::OnReportCheckItem( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == _0_COLUMN_SELECT)
		{
			CheckButtonState();
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

void KCalculatParameterDlg::CheckButtonState()
{
	try
	{
		bool bCheckPassenger = false;
		bool bCheckTransit   = false;
		bool bCheckInter	 = false;

		CXTPReportRecords* pRecords = m_wmReportControl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_SELECT);
			if (TRUE == pItem->GetCheckedState())
			{
				CXTPReportRecordItem* pModeItem		= nullptr;
				pModeItem = pRecord->GetItem(_1_COLUMN_MODE);
				int nMode = pModeItem->GetItemData();

				if (P_MODE_AUTO == nMode || P_MODE_TAXI == nMode || P_MODE_TRUCK == nMode)
				{
					bCheckPassenger = true;
				}

				if (P_MODE_BUS == nMode || P_MODE_RAIL == nMode || P_MODE_BUS_RAIL == nMode)
				{
					bCheckTransit = true;
				}

				if (P_MODE_PARK_RIDE == nMode || P_MODE_KISS_RIDE == nMode || P_MODE_TAXI_TRANSIT == nMode)
				{
					bCheckInter = true;
				}
			}
		}

		if (true == bCheckInter && false == bCheckPassenger &&  false== bCheckTransit)
		{
			((CButton*)GetDlgItem(IDC_BUTTON_COST))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_TIME))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(FALSE);
		}
		else if ( true == bCheckPassenger || true == bCheckTransit)
		{
			((CButton*)GetDlgItem(IDC_BUTTON_COST))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON_TIME))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(FALSE);
		}
		else if (false == bCheckInter && false == bCheckPassenger &&  false== bCheckTransit)
		{
			((CButton*)GetDlgItem(IDC_BUTTON_TIME))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_COST))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(FALSE);
		}

        if (KBulkDBaseTurn::IsTurn(m_pTarget)) {
            bool bCheckTurn(false);

            if (bCheckPassenger || bCheckInter) {
                bCheckTurn = true;
            }
            
            if (bCheckTurn) {
                GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
                CheckDlgButton(IDC_CHECK1, BST_CHECKED);
            }
            else {
                GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
                CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
            }
        }
        else {
            GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
            CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
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


void KCalculatParameterDlg::OnBnClickedButtonCost()
{
	SaveOutputInfo();
	KParameterDefineCostDlg oDlg(m_vecOutputInfo, m_pTarget);
	oDlg.SetCostInfo(m_vecAutoCost, m_vecTaxiCost, m_vecTruckCost, m_vecTransitCost);
	if (oDlg.DoModal() == IDOK)
	{
		oDlg.GetCostInfo(m_vecAutoCost, m_vecTaxiCost, m_vecTruckCost, m_vecTransitCost);
		((CButton*)GetDlgItem(IDC_BUTTON_TIME))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(FALSE);
	}
}


void KCalculatParameterDlg::OnBnClickedButtonTime()
{
	KParameterDefineTime oDlg(m_pTarget, m_nTimeMethod, m_pIOColumnUseLink, m_oAssignInfo);
	if (oDlg.DoModal() == IDOK)
	{
		oDlg.GetDefineTimeData(m_nTimeMethod, m_pIOColumnUseLink, m_oAssignInfo);
		((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(TRUE);
	}
}

void KCalculatParameterDlg::InitAutoCost()
{
	m_vecAutoCost.clear();

	TAutoFuel oGasoline;
	oGasoline.nFuelType			= P_FUEL_GASOLINE;
	oGasoline.strFuelName		= P_GASOLINE_NAME;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oGasoline.strFuelName		= P_GASOLINE_NAME_K;
	}
	oGasoline.dPrice			= 2000.0;
	oGasoline.dRation			= 0.5;
	oGasoline.dFuelEfficiency	= 8.00;
	m_vecAutoCost.push_back(oGasoline);

	TAutoFuel oDiesel;
	oDiesel.nFuelType			= P_FUEL_DIESEL;
	oDiesel.strFuelName			= P_DIESEL_NAME;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oDiesel.strFuelName		= P_DIESEL_NAME_K;
	}
	oDiesel.dPrice				= 1700.0;
	oDiesel.dRation				= 0.35;
	oDiesel.dFuelEfficiency		= 12.00;
	m_vecAutoCost.push_back(oDiesel);

	TAutoFuel oLPG;
	oLPG.nFuelType				= P_FUEL_LPG;
	oLPG.strFuelName			= P_LPG_NAME;
	oLPG.dPrice					= 1200.0;
	oLPG.dRation				= 0.15;
	oLPG.dFuelEfficiency		= 6.00;
	m_vecAutoCost.push_back(oLPG);
}

void KCalculatParameterDlg::InitTaxiCost()
{
	m_vecTaxiCost.clear();

	TTaxiFuel oLPG;
	oLPG.nFuelType			= P_FUEL_LPG;
	oLPG.strFuelName		= P_LPG_NAME;
	oLPG.dBaseFare			= 2300.0;
	oLPG.dBaseDistance		= 2.00;
	oLPG.dDistBaseFare		= 100;
	oLPG.dUnitDist			= 0.15;

	m_vecTaxiCost.push_back(oLPG);
}

void KCalculatParameterDlg::InitTruckCost()
{
	m_vecTruckCost.clear();

	TTruckFuel oDisel;
	oDisel.nFuelType		= P_FUEL_DIESEL;
	oDisel.strFuelName		= P_DIESEL_NAME;
	oDisel.dPrice			= 1700.0;
	oDisel.dFuelEfficiency	= 3.0;
	m_vecTruckCost.push_back(oDisel);
}

void KCalculatParameterDlg::InitTransitCost()
{
	KDBaseDefineTransitCost::GetTransitModeCostValue2DB(m_pTarget, m_vecTransitCost);

}

void KCalculatParameterDlg::SaveOutputInfo()
{
	try
	{
		CXTPReportRecords* pRecords = m_wmReportControl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_SELECT);
			if (TRUE == pItem->GetCheckedState())
			{
				CXTPReportRecordItem* pModeItem		= nullptr;
				pModeItem = pRecord->GetItem(_1_COLUMN_MODE);
				int nMode = pModeItem->GetItemData();

				int nCount = m_vecOutputInfo.size();
				for (int k = 0; k < nCount; k++)
				{
					TParaOutputInfo& oInfo = m_vecOutputInfo[k];
					if (oInfo.nMode == nMode)
					{
						oInfo.bUseData = true;
					}

				}
			}
			else
			{
				CXTPReportRecordItem* pModeItem		= nullptr;
				pModeItem = pRecord->GetItem(_1_COLUMN_MODE);
				int nMode = pModeItem->GetItemData();

				int nCount = m_vecOutputInfo.size();
				for (int k = 0; k < nCount; k++)
				{
					TParaOutputInfo& oInfo = m_vecOutputInfo[k];
					if (oInfo.nMode == nMode)
					{
						oInfo.bUseData = false;
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

void KCalculatParameterDlg::GetColumnName(TParaOutputInfo& a_oParaOutputInfo)
{
	if (a_oParaOutputInfo.nMode == P_MODE_AUTO)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_AUTO_INVEHTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_AUTO_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_AUTO_LENGTH;
	}
	else if(a_oParaOutputInfo.nMode == P_MODE_TAXI)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_TAXI_INVEHTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_TAXI_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_TAXI_LENGTH;
	}
	else if (a_oParaOutputInfo.nMode == P_MODE_TRUCK)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_TRUCK_INVEHTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_TRUCK_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_TRUCK_LENGTH;
	}
	else if (a_oParaOutputInfo.nMode == P_MODE_BUS)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_BUS_INVEHTIME;
		a_oParaOutputInfo.strWaitColumnName		= COLUMN_PARA_BUS_WAITTIME;
		a_oParaOutputInfo.strTransferColumnName	= COLUMN_PARA_BUS_TRANSTIME;
		a_oParaOutputInfo.strAccessColumnName	= COLUMN_PARA_BUS_ACCTIME;
		a_oParaOutputInfo.strEgrColumnName		= COLUMN_PARA_BUS_ERGTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_BUS_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_BUS_LENGTH;
		a_oParaOutputInfo.strNoTransferColumnName	= COLUMN_PARA_BUS_NOTRANSFER;
		a_oParaOutputInfo.strNoStopColumnName	= COLUMN_PARA_BUS_NOOFSTOP;
	}
	else if (a_oParaOutputInfo.nMode == P_MODE_RAIL)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_RAIL_INVEHTIME;
		a_oParaOutputInfo.strWaitColumnName		= COLUMN_PARA_RAIL_WAITTIME;
		a_oParaOutputInfo.strTransferColumnName	= COLUMN_PARA_RAIL_TRANSTIME;
		a_oParaOutputInfo.strAccessColumnName	= COLUMN_PARA_RAIL_ACCTIME;
		a_oParaOutputInfo.strEgrColumnName		= COLUMN_PARA_RAIL_ERGTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_RAIL_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_RAIL_LENGTH;
		a_oParaOutputInfo.strNoTransferColumnName	= COLUMN_PARA_RAIL_NOTRANSFER;
	}
	else if (a_oParaOutputInfo.nMode == P_MODE_BUS_RAIL)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_BUS_RAIL_INVEHTIME;
		a_oParaOutputInfo.strWaitColumnName		= COLUMN_PARA_BUS_RAIL_WAITTIME;
		a_oParaOutputInfo.strTransferColumnName	= COLUMN_PARA_BUS_RAIL_TRANSTIME;
		a_oParaOutputInfo.strAccessColumnName	= COLUMN_PARA_BUS_RAIL_ACCTIME;
		a_oParaOutputInfo.strEgrColumnName		= COLUMN_PARA_BUS_RAIL_ERGTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_BUS_RAIL_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_BUS_RAIL_LENGTH;
		a_oParaOutputInfo.strNoTransferColumnName	= COLUMN_PARA_BUS_RAIL_NOTRANSFER;
		a_oParaOutputInfo.strNoStopColumnName	= COLUMN_PARA_BUS_RAIL_NOOFSTOP;
	}
	else if (a_oParaOutputInfo.nMode == P_MODE_PARK_RIDE)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_PR_INVEHTIME;
		a_oParaOutputInfo.strWaitColumnName		= COLUMN_PARA_PR_WAITTIME;
		a_oParaOutputInfo.strTransferColumnName	= COLUMN_PARA_PR_TRANSTIME;
		a_oParaOutputInfo.strAccessColumnName	= COLUMN_PARA_PR_ACCTIME;
		a_oParaOutputInfo.strEgrColumnName		= COLUMN_PARA_PR_ERGTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_PR_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_PR_LENGTH;
		a_oParaOutputInfo.strNoTransferColumnName	= COLUMN_PARA_PR_NOTRANSFER;
	}
	else if (a_oParaOutputInfo.nMode == P_MODE_KISS_RIDE)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_KR_INVEHTIME;
		a_oParaOutputInfo.strWaitColumnName		= COLUMN_PARA_KR_WAITTIME;
		a_oParaOutputInfo.strTransferColumnName	= COLUMN_PARA_KR_TRANSTIME;
		a_oParaOutputInfo.strAccessColumnName	= COLUMN_PARA_KR_ACCTIME;
		a_oParaOutputInfo.strEgrColumnName		= COLUMN_PARA_KR_ERGTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_KR_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_KR_LENGTH;
		a_oParaOutputInfo.strNoTransferColumnName	= COLUMN_PARA_KR_NOTRANSFER;
	}
	else if (a_oParaOutputInfo.nMode == P_MODE_TAXI_TRANSIT)
	{
		a_oParaOutputInfo.strInVehColumnName	= COLUMN_PARA_TT_INVEHTIME;
		a_oParaOutputInfo.strWaitColumnName		= COLUMN_PARA_TT_WAITTIME;
		a_oParaOutputInfo.strTransferColumnName	= COLUMN_PARA_TT_TRANSTIME;
		a_oParaOutputInfo.strAccessColumnName	= COLUMN_PARA_TT_ACCTIME;
		a_oParaOutputInfo.strEgrColumnName		= COLUMN_PARA_TT_ERGTIME;
		a_oParaOutputInfo.strCostColumnName		= COLUMN_PARA_TT_COST;
		a_oParaOutputInfo.strLengthColumnName	= COLUMN_PARA_TT_LENGTH;
		a_oParaOutputInfo.strNoTransferColumnName	= COLUMN_PARA_TT_NOTRANSFER;
	}
}


void KCalculatParameterDlg::OnBnClickedButtonRun()
{
    m_bApplyTurn = false; {
        if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED) {
            m_bApplyTurn = true;
        }
        else {
            m_bApplyTurn = false;
        }
    }

	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(AccessThreadCaller, this, true, nLang);

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("기종점간 통행 속성 값 산출 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("O/D Parameter Module Complete."));
			}
			AfxMessageBox(_T("성공적으로 완료되었습니다."));
		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
			//AddStatusMessage(_T("> User ..."));
			ThrowException(_T("> User Stop.."));
		}
		else
		{
			ThrowException(_T("수행 중 오류가 발생하였습니다."));
		}

		((CButton*)GetDlgItem(IDC_BUTTON_VIEW))->EnableWindow(TRUE);
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

unsigned __stdcall KCalculatParameterDlg::AccessThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("기종점간 통행 속성 값 산출 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run O/D Parameter Module..."));
	}

	KCalculatParameterDlg*        pDlg = (KCalculatParameterDlg*)pParameter->GetParameter();
	pDlg->RunModel();

	return 0;
}


void KCalculatParameterDlg::ReceiveStatusMessageFromDll( void* pParam1, int nStatus, char* strMsg )
{
	AddStatusMessage(CString(strMsg));
}



void KCalculatParameterDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KCalculatParameterDlg::CreateArgument(TParaSelectedMode oSelectedMode)
{
	ImChampDir::SetCurrentDirectoryToApp();

	ClearFiles();

	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(m_argFilePath, nOpenMode);

	m_strOutputFolder = ConvertSavePath(_T(""));

	CreateDirectory( m_strOutputFolder, NULL );
	try
	{
		ArgumentWriteProjectName(of);
		ArgumentWriteUseMode(of);

		if (oSelectedMode.bAuto == true)
		{
			ArgumentWriteAutoCost(of);
		}
		
		if (oSelectedMode.bTaxi == true)
		{
			ArgumentWriteTaxiCost(of);
		}
		
		if (oSelectedMode.bTruck == true)
		{
			ArgumentWriteTruckCost(of);
		}
		
		if (oSelectedMode.bTransit == true)
		{
			ArgumentWriteTransitCost(of);
		}
		
		ArgumentWriteTimeMethod(of);

		if (oSelectedMode.bTransit == true)
		{
			ArgumentWriteAssignRule(of);
			ArgumentWriteNoClass(of);
		}

		ArgumentWriteTurnRestriction(of);
		
        //if (true == oSelectedMode.bAuto ||
        //    true == oSelectedMode.bTaxi ||
        //    true == oSelectedMode.bTruck) {

        //    ArgumentWriteTurnRestriction(of);
        //}

		ArgumentWirteFileFolder(of);
		of.Close();

		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget);

		if ( !KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("node.dat")) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		}
		if ( !KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("line_info.dat")), ConvertSavePath(_T("line.dat")) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LineFile) 중 오류가 발생하였습니다. !!!"));
		}

		if( !KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(),  ConvertSavePath(_T("turn.dat"))))
		{
			ThrowException(_T("Dat 생성(ExportDB2TurnFile) 중 오류가 발생하였습니다. !!!"));
		}

		KIOColumns oLinkColumns;
		oLinkColumns.AddNewColumn(m_pIOColumnUseLink);
		if ( !KBulkDBase2File::ExportDB2LinkFileGeneratorPath(m_pTarget->GetDBaseConnection(), m_pIOColumnUseLink, ConvertSavePath(T_DAT_LINK) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LinkFileC) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2AccessStopNodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2AccessNodeStopFile) 중 오류가 발생하였습니다. !!!"));
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

CString KCalculatParameterDlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, a_strFileName);

	return strSavePath;
}

void KCalculatParameterDlg::ClearFiles()
{
	int nResult = -1;
	try
	{
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINEINFO));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TURN));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
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

void KCalculatParameterDlg::ArgumentWriteProjectName( CStdioFileEx& of )
{
	CString strOut;

	KProject* pProject = (KProject*)( (CKmzApp*)AfxGetApp() )->GetProject();

	try
	{
		strOut.Format(_T("%s_%s_%s\r\n"), pProject->GetName(), m_pTarget->CaptionScenario(), m_pTarget->CaptionTargetYear());
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KCalculatParameterDlg::ArgumentWriteUseMode( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		int nCount = m_vecOutputInfo.size();
		for (int i = 0; i < nCount ; i++)
		{
			TParaOutputInfo oInfo = m_vecOutputInfo[i];
			int nUse = 0;
			if (true == oInfo.bUseData)
			{
				nUse = 1;
			}
			strOut.Format(_T("%s\t%d\r\n"), oInfo.strMode, nUse);
			of.WriteString(strOut);
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

void KCalculatParameterDlg::ArgumentWriteAutoCost( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		strOut.Format(_T("AutoCost\r\n"));
		of.WriteString(strOut);

		int nCount = m_vecAutoCost.size();
		for (int i = 0; i < nCount ; i++)
		{
			TAutoFuel oInfo = m_vecAutoCost[i];
			strOut.Format(_T("%s\t%.2f\t%.2f\t%.2f\r\n"), oInfo.strFuelName, oInfo.dPrice, oInfo.dRation, oInfo.dFuelEfficiency);
			of.WriteString(strOut);
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

void KCalculatParameterDlg::ArgumentWriteTaxiCost( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		int nCount = m_vecTaxiCost.size();
		for (int i = 0; i < nCount ; i++)
		{
			TTaxiFuel oInfo = m_vecTaxiCost[i];
			strOut.Format(_T("Taxi\t%.2f\t%.2f\t%.2f\t%.2f\r\n"), oInfo.dBaseFare, oInfo.dBaseDistance, oInfo.dDistBaseFare, oInfo.dUnitDist);
			of.WriteString(strOut);
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

void KCalculatParameterDlg::ArgumentWriteTruckCost( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		int nCount = m_vecTruckCost.size();
		for (int i = 0; i < nCount ; i++)
		{
			TTruckFuel oInfo = m_vecTruckCost[i];
			strOut.Format(_T("Truck\t%.2f\t%.2f\r\n"), oInfo.dPrice, oInfo.dFuelEfficiency);
			of.WriteString(strOut);
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

void KCalculatParameterDlg::ArgumentWriteTransitCost( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		int nBusCount(0);
		int nRailCount(0);
		int nCount = m_vecTransitCost.size();
		for (int i = 0; i < nCount; i++)
		{
			TTransitCost oInfo = m_vecTransitCost[i];
			if (oInfo.nMode == TRANSIT_MODE_BUS)
			{
				nBusCount++;
			}
			else if (oInfo.nMode == TRANSIT_MODE_RAIL)
			{
				nRailCount++;
			}
		}
		
		strOut.Format(_T("detailedLineType\t%d\t%d\r\n"), nBusCount, nRailCount);
		of.WriteString(strOut);

		for (int i = 0; i < nCount ; i++)
		{
			TTransitCost oInfo = m_vecTransitCost[i];
			
			CString strMode(_T(""));
			if (oInfo.nMode == TRANSIT_MODE_BUS)
			{
				strMode = _T("buscost");
			}
			else if (oInfo.nMode == TRANSIT_MODE_RAIL)
			{
				strMode = _T("railcost");
			}
			strOut.Format(_T("%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%d\r\n"), strMode,oInfo.nTType, oInfo.dTbasicRate, oInfo.dTbasicDistance,
				oInfo.dTAddRate, oInfo.dTAddDistance, oInfo.nTMergeRate);
			of.WriteString(strOut);
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

void KCalculatParameterDlg::ArgumentWriteTimeMethod( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		strOut.Format(_T("TimeMethod\t%d\r\n"), m_nTimeMethod);
		of.WriteString(strOut);
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


void KCalculatParameterDlg::ArgumentWriteAssignRule( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		strOut.Format(_T("dwelltime_bus\t%.2f\r\n"), m_oAssignInfo.dDWTimeBusstop);
		of.WriteString(strOut);
		strOut.Format(_T("dwelltime_rail\t%.2f\r\n"), m_oAssignInfo.dDwTimeRailStation);
		of.WriteString(strOut);
		strOut.Format(_T("time\t%d\t%d\t%d\t%d\r\n"), m_oAssignInfo.nInvehicle, m_oAssignInfo.nWating, m_oAssignInfo.nTransfer, m_oAssignInfo.nAETime);
		of.WriteString(strOut);
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


void KCalculatParameterDlg::ArgumentWriteNoClass( CStdioFileEx& of )
{	
	try
	{
		CString strOut;
		std::map<CString, double> mapNoofClass;
		KDBaseAssignment::GetNoClass(m_pTarget, mapNoofClass);

		int nCount = mapNoofClass.size();
		strOut.Format(_T("noClass\t%d\r\n"), nCount);
		of.WriteString(strOut);

		std::map<CString, double>::iterator iter = mapNoofClass.begin();
		std::map<CString, double>::iterator end  = mapNoofClass.end();

		while(iter != end)
		{
			strOut.Format(_T("%s\t%f\r\n"), iter->first, iter->second);
			of.WriteString(strOut);
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


void KCalculatParameterDlg::ArgumentWriteTurnRestriction(CStdioFileEx& of)
{
    CString strOut;
    try
    {
        int nTurn(0); {
            if (m_bApplyTurn) {
                nTurn = 1;
            }
            else {
                nTurn = 0;
            }
        }
        strOut.Format(_T("TurnRestriction\t%d\r\n"), nTurn);
        of.WriteString(strOut);
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
    }
    catch (...)
    {
        TxLogDebugException();
    }
}


void KCalculatParameterDlg::ArgumentWirteFileFolder( CStdioFileEx& of )
{
	CString strFolder;
	try
	{
		strFolder.Format(_T("%s"), m_strOutputFolder);
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);
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


void KCalculatParameterDlg::RunModel()
{
	try
	{
		TParaSelectedMode oSelectMode = {false, false, false, false ,false ,false ,false ,false ,false, false};
		SelectedMode(oSelectMode);

		CreateArgument(oSelectMode);

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			ThrowException(_T(">User Stop.."));
		}
		m_nResultCode = DLL_UNEXPECTED_ERROR;
		CString strWindowName;
		GetWindowText(strWindowName);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("기종점간 통행 속성 값 산출 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run O/D Parameter Module..."));
		}
		//KLauncher::Run(strWindowName, 15);
		KLauncher::Run64(strWindowName, 15);

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("기종점간 통행 속성 값 산출 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("O/D Parameter Module Complete."));
			}
			TxLogDebug(_T("Model Run Ok.."));
		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
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
			AddStatusMessage(_T("> Parameter OD  Dll Not Found ..."));
			TxLogDebug(_T("DLL NOT Found.."));
			ThrowException(_T("DLL Not Found.."));
		}
		else
		{
			if (QBicListSimpleProgressThread::IsUserStopped())
			{
				m_nResultCode = DLL_USER_STOPPING;
				if (KmzSystem::GetLanguage() == KEMKorea) {
					AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
				}
				else {
					AddStatusMessage(_T("Running Stopped."));
				}
				TxLogDebug(_T("User Stop.."));
				ThrowException(_T("User Stop.."));
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
				ThrowException(_T("Parameter OD End(DLL Error)"));
			}
			return;
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
		}

		KIOColumns rParameterODColumnCollection;

		GetOutputColumnCollection(rParameterODColumnCollection);

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		UpadateParaODFile2DB(spDBaseConnection, rParameterODColumnCollection, oSelectMode);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		m_nResultCode = DLL_UNEXPECTED_ERROR;
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		m_nResultCode = DLL_UNEXPECTED_ERROR;
		return;
	}
}

BOOL KCalculatParameterDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
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

	return KResizeDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void KCalculatParameterDlg::GetOutputColumnCollection( KIOColumns& rParameterColumnCollection )
{
	try
	{
		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);

		int nCount = m_vecOutputInfo.size();
		for (int i = 0; i < nCount; i++ )
		{
			TParaOutputInfo oInfo = m_vecOutputInfo[i];
			if (true == oInfo.bUseData )
			{
				//invehTime
				KIOColumn* pColumn = nullptr;
				GetResultColumn(pColumn, pTable, oInfo.strInVehColumnName);
				rParameterColumnCollection.AddNewColumn(pColumn);

				if (oInfo.nMode != P_MODE_AUTO && oInfo.nMode != P_MODE_TAXI && oInfo.nMode != P_MODE_TRUCK )
				{
					pColumn = nullptr;
					GetResultColumn(pColumn, pTable, oInfo.strWaitColumnName);
					rParameterColumnCollection.AddNewColumn(pColumn);

					pColumn = nullptr;
					GetResultColumn(pColumn, pTable, oInfo.strTransferColumnName);
					rParameterColumnCollection.AddNewColumn(pColumn);

					pColumn = nullptr;
					GetResultColumn(pColumn, pTable, oInfo.strAccessColumnName);
					rParameterColumnCollection.AddNewColumn(pColumn);

					pColumn = nullptr;
					GetResultColumn(pColumn, pTable, oInfo.strEgrColumnName);
					rParameterColumnCollection.AddNewColumn(pColumn);
				}

				pColumn = nullptr;
				GetResultColumn(pColumn, pTable, oInfo.strCostColumnName);
				rParameterColumnCollection.AddNewColumn(pColumn);

				pColumn = nullptr;
				GetResultColumn(pColumn, pTable, oInfo.strLengthColumnName);
				rParameterColumnCollection.AddNewColumn(pColumn);

				if (oInfo.nMode != P_MODE_AUTO && oInfo.nMode != P_MODE_TAXI && oInfo.nMode != P_MODE_TRUCK )
				{
					pColumn = nullptr;
					GetResultColumn(pColumn, pTable, oInfo.strNoTransferColumnName);
					rParameterColumnCollection.AddNewColumn(pColumn);
				}
				
				if (oInfo.nMode == P_MODE_BUS || oInfo.nMode == P_MODE_BUS_RAIL)
				{
					pColumn = nullptr;
					GetResultColumn(pColumn, pTable, oInfo.strNoStopColumnName);
					rParameterColumnCollection.AddNewColumn(pColumn);
				}

			}
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
		ThrowException(_T("Error"));
	}	
}

void KCalculatParameterDlg::GetResultColumn( KIOColumn*& a_pColumn, KIOTable* a_pTable ,CString a_strColumnName )
{
	try
	{
		const KIOColumns* pColumCollection = a_pTable->Columns();

		int nColumnCount = pColumCollection->ColumnCount();
		for (int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pColumn = pColumCollection->GetColumn(i);
			CString strColumnName = pColumn->Name();
			if (strColumnName.CompareNoCase(a_strColumnName) == 0)
			{
				a_pColumn = pColumn;
				return;
			}
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
		ThrowException(_T("Error"));
	}	
}

void KCalculatParameterDlg::UpadateParaODFile2DB( KDBaseConPtr spDBaseConnection, KIOColumns& rParaODColumnCollection,TParaSelectedMode oSelectedMode )
{
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*		 pParaODTable	   = pIOTables->FindTable(TABLE_PARAMETER_OD);
	int nColumnCount = rParaODColumnCollection.ColumnCount();

	if (nColumnCount == 0)
	{
		ThrowException(_T("적용 컬럼 정보가 없습니다."));
	}
	try
	{
		CString strOutLinkFile = ConvertSavePath(_T("ODParameter.dat"));
		std::ifstream ifs(strOutLinkFile, std::ios::binary );    
		if( !ifs )
			ThrowException(_T("File Not Found"));
		KSDoubleRecord* pRecord = new KSDoubleRecord(nColumnCount);
		__int64 nxOZoneID, nxDZoneID;
		double dValue;
		int nValue;

		KxBulkDbase oBulkDBase(spDBaseConnection, pParaODTable, &rParaODColumnCollection);
		TxLogDebug(_T("import loop start!"));
		while (!ifs.eof()) 
		{
			int nIndex = 0;

			ifs.read( reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer) );
			ifs.read( reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer) );

			if (oSelectedMode.bAuto == true)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //AutoInveh
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //AutoCost
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //AutoLength
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			}

			
			if (oSelectedMode.bTaxi == true)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			}

			if (oSelectedMode.bTruck == true)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cos
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
				pRecord->SetAt(nIndex,dValue);
				nIndex++;
			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			}
			
			if (oSelectedMode.bBus == true)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
				pRecord->SetAt(nIndex,(double)nValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no stop
				pRecord->SetAt(nIndex,(double)nValue);
				nIndex++;
			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no stop
			}


			if (oSelectedMode.bRail == true)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
				pRecord->SetAt(nIndex,(double)nValue);
				nIndex++;

			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
			}

			if (oSelectedMode.bBus_Rail == true)
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh4
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
				pRecord->SetAt(nIndex,dValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
				pRecord->SetAt(nIndex,(double)nValue);
				nIndex++;

				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no stop
				pRecord->SetAt(nIndex,(double)nValue);
				nIndex++;
			}
			else
			{
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
				ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
				ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no stop
			}

			//임시로 막아둠
			//if (oSelectedMode.bPark_Ride == true)
			//{
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
			//	pRecord->SetAt(nIndex,(double)nValue);
			//	nIndex++;

			//}
			//else
			//{
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			//	ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
			//}

			//if (oSelectedMode.bKiss_Ride == true)
			//{
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
			//	pRecord->SetAt(nIndex,(double)nValue);
			//	nIndex++;

			//}
			//else
			//{
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			//	ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
			//}


			//if (oSelectedMode.bTaxi_Transit == true)
			//{
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			//	pRecord->SetAt(nIndex,dValue);
			//	nIndex++;

			//	ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
			//	pRecord->SetAt(nIndex,(double)nValue);
			//	nIndex++;
			//}
			//else
			//{
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //inveh
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //wait
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //transfer
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //access
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //egr
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //cost
			//	ifs.read( reinterpret_cast<char*>(&dValue), sizeof(double)); //length
			//	ifs.read( reinterpret_cast<char*>(&nValue), sizeof(int)); //no transfer
			//}


			if(ifs.eof())
				break;
			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}
		TxLogDebug(_T("import loop end!"));
		oBulkDBase.ExecuteFullOuterJoin();

		delete pRecord;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ThrowException(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		
	}
}

void KCalculatParameterDlg::SelectedMode( TParaSelectedMode& oSelectedMode )
{
	int nCount = m_vecOutputInfo.size();
	for (int i =0; i <nCount; i++)
	{
		TParaOutputInfo oInfo = m_vecOutputInfo[i];
		
		if (oInfo.nMode == P_MODE_AUTO && oInfo.bUseData == true)
		{
			oSelectedMode.bAuto = true;
		}
		else if (oInfo.nMode == P_MODE_TAXI && oInfo.bUseData ==true)
		{
			oSelectedMode.bTaxi = true;
		}
		else if (oInfo.nMode == P_MODE_TRUCK && oInfo.bUseData == true)
		{
			oSelectedMode.bTruck = true;
		}
		else if (oInfo.nMode == P_MODE_BUS && oInfo.bUseData == true)
		{
			oSelectedMode.bBus = true;
			oSelectedMode.bTransit = true;
		}
		else if (oInfo.nMode == P_MODE_RAIL && oInfo.bUseData == true)
		{
			oSelectedMode.bRail = true;
			oSelectedMode.bTransit = true;
		}
		else if (oInfo.nMode == P_MODE_BUS_RAIL && oInfo.bUseData == true)
		{
			oSelectedMode.bBus_Rail = true;
			oSelectedMode.bTransit = true;
		}
		else if (oInfo.nMode == P_MODE_PARK_RIDE && oInfo.bUseData == true)
		{
			oSelectedMode.bPark_Ride = true;
		}
		else if (oInfo.nMode == P_MODE_KISS_RIDE && oInfo.bUseData ==true)
		{
			oSelectedMode.bKiss_Ride = true;
		}
		else if (oInfo.nMode == P_MODE_TAXI_TRANSIT && oInfo.bUseData == true)
		{
			oSelectedMode.bTaxi_Transit = true;
		}
	}
}


void KCalculatParameterDlg::OnBnClickedButtonView()
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pParameterTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
	ASSERT(NULL != pParameterTable);

	CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	pChildFrame->ShowIOView( true );
	KIOView* pIOView = pChildFrame->GetIOView();

	if(NULL != pIOView)
	{
		KIOTableView* pTableView = pIOView->FindTableView(pParameterTable->Name());
		if(NULL == pTableView)
		{
			pTableView = pIOView->CreateNewTableView(pParameterTable);
			KIOTableController* pController = new KIOTableController(m_pTarget);
			pController->Attach(pParameterTable, pTableView);
		}
		pIOView->SetActiveTableView(pParameterTable->Name());
	}

	pParameterTable->Notify();

	TxLogDebugEndMsg();
}
