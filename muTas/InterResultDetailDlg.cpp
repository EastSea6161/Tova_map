// InterResultDetailDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterResultDetailDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "MapView.h"
#include "ImChampFrameWindow.h"
#include "MultiModalResultInfo.h"

enum
{
	_0_A_COLUMN_INVEH_TIME    = 0,
	_1_A_COLUMN_WAIT_TIME     = 1,
	_2_A_COLUMN_TRANS_TIME    = 2,
	_3_A_COLUMN_ACCWALK_TIME  = 3,
	_4_A_COLUMN_EGRWALK_TIME  = 4,
	_5_A_COLUMN_COST          = 5,
	_6_A_COLUMN_LENGTH        = 6,
	_7_A_COLUMN_TRIP          = 7,
};


enum
{
	_0_B_COLUMN_TRANSITID     = 0,
	_1_B_COLUMN_ROUTE         = 1,
	_2_B_COLUMN_PATHTYPE      = 2,
	_3_B_COLUMN_STARTSTATION  = 3,
	_4_B_COLUMN_ENDSTATION    = 4,
	_5_B_COLUMN_TRIP          = 5
};
enum 
{
	COLUMN_TOTAL_COUNT      = 6
};

// KInterResultDetailDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterResultDetailDlg, KResizeDialogEx)

KInterResultDetailDlg::KInterResultDetailDlg(KTarget* a_pTarget, bool a_bShowTrip, std::map<Integer, CString> &a_mapNodeName, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterResultDetailDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_bShowTrip(a_bShowTrip)
	, m_mapNodeName(a_mapNodeName)
	, m_pParentWnd(pParent)

{

}

KInterResultDetailDlg::~KInterResultDetailDlg()
{
	if (nullptr != m_spTransitPathViewDlg)
		m_spTransitPathViewDlg.reset();
}

void KInterResultDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT_A,               m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT,                 m_wndReportBCtrl);
}


BEGIN_MESSAGE_MAP(KInterResultDetailDlg, KResizeDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT,                      &KInterResultDetailDlg::OnReportBItemDblClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT,                       &KInterResultDetailDlg::OnReportBItemClick)
	ON_NOTIFY (XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, &KInterResultDetailDlg::OnItemButtonClick)
	ON_BN_CLICKED(IDCANCEL,                               &KInterResultDetailDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


void KInterResultDetailDlg::InitializeEditData(KEMInterModalPathMode a_emInterModalPathMode, CString a_strPathIdex, KInterPathEditInfo* a_pTrasitEditInfo, CRect rectParent )
{
	m_emInterModalPathMode = a_emInterModalPathMode;

	Integer              &nxOrginID           = a_pTrasitEditInfo->nxOrginID;
	Integer              &nxDestinationID     = a_pTrasitEditInfo->nxDestinationID;
	KEMInterMode         &emInterMode         = a_pTrasitEditInfo->emInterMode;
	CString              strAssignType(_T(""));
	
	if (KmzSystem::GetLanguage() == KEMKorea) {
		if(KEMInterModeBus == emInterMode)
			strAssignType = _T("버스");
		else if(KEMInterModeRail == emInterMode)
			strAssignType = _T("도시철도");
		else if(KEMInterModeBusRail == emInterMode)
			strAssignType = _T("버스+도시철도");
		else if(KEMInterModeParkRide == emInterMode)
			strAssignType = _T("Park & Ride");
		else if(KEMInterModeKissRide == emInterMode)
			strAssignType = _T("Kiss & Ride");
		else if(KEMInterModeTaxiTransit == emInterMode)
			strAssignType = _T("택시 & 대중교통");
		else if(KEMInterModeAutoTaxi == emInterMode)
			strAssignType = _T("Auto");

	}
	else {
		if(KEMInterModeBus == emInterMode)
			strAssignType = _T("Bus");
		else if(KEMInterModeRail == emInterMode)
			strAssignType = _T("Rail");
		else if(KEMInterModeBusRail == emInterMode)
			strAssignType = _T("Bus+Rail");
		else if(KEMInterModeParkRide == emInterMode)
			strAssignType = _T("Park & Ride");
		else if(KEMInterModeKissRide == emInterMode)
			strAssignType = _T("Kiss & Ride");
		else if(KEMInterModeTaxiTransit == emInterMode)
			strAssignType = _T("Taxi & Transit");
		else if(KEMInterModeAutoTaxi == emInterMode)
			strAssignType = _T("PassengerCar");
	}

	CString strSubTitle(_T(""));
    
    if (KmzSystem::GetLanguage() == KEMKorea) {
        if (KEMInterModalPathModeUrban == m_emInterModalPathMode) {
            strSubTitle.Format(_T("기점 : %I64d    종점 : %I64d    [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
        }
        else { //KEMInterModalPathModeRegional	
            if (KEMInterAccEgrTypeAccess == a_pTrasitEditInfo->emInterAccEgrType ) {
                strSubTitle.Format(_T("기점 : %I64d    터미널 : %I64d    [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
            }
            else {
                strSubTitle.Format(_T("터미널 : %I64d    종점 : %I64d    [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
            }
        }
    }
    else {
        if (KEMInterModalPathModeUrban == m_emInterModalPathMode) {
            strSubTitle.Format(_T("Origin:%I64d Destination:%I64d [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
        }
        else { //KEMInterModalPathModeRegional	
            if (KEMInterAccEgrTypeAccess == a_pTrasitEditInfo->emInterAccEgrType ) {
                strSubTitle.Format(_T("Origin:%I64d Terminal:%I64d [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
            }
            else {
                strSubTitle.Format(_T("Terminal:%I64d Destination:%I64d [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
            }
        }
    }	
	
	SetDlgItemText(IDC_STATIC_SUB, strSubTitle);

	std::vector<TInResultDetailActiveInfo> vecActiveDetailInfo;
	GetActiveDetailInfoData(a_pTrasitEditInfo, vecActiveDetailInfo);

	InitReportAHeader    (vecActiveDetailInfo);
	UpdateEditReportAData(vecActiveDetailInfo);

	UpdateEditReportBData(a_pTrasitEditInfo);
}


void KInterResultDetailDlg::GetActiveDetailInfoData(KInterPathEditInfo* a_pTrasitEditInfo, std::vector<TInResultDetailActiveInfo> &a_vecActiveDetailInfo)
{
	a_vecActiveDetailInfo.clear();

	CString CONTENT_TIME         = _T("Time");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CONTENT_TIME       = _T("통행시간");
	}

	CString CONTENT_LENGTH       = _T("Length");
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CONTENT_LENGTH       = _T("통행거리");
    }

	CString CONTENT_COST         = _T("Cost"); 
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CONTENT_COST         = _T("통행비용");
    }

	CString CONTENT_TRIP         = _T("Trip");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CONTENT_TRIP         = _T("통행량(인)");
	}

	// 2019.03.12 명지대 요청 : 명칭 변경(통행시간 -> 통행시간(승용차/택시))
	CString CONTENT_TRAVELTIME   = _T("TravelTime(PassengerCar/Taxi)");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CONTENT_TRAVELTIME       = _T("통행시간(승용차/택시)");
	}
	// 2019.03.12 명지대 요청 : 명칭 변경(통행시간 -> 통행시간(승용차/택시)) 끝

	CString CONTENT_ACCWALKTIME  = _T("AccWalkTime");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CONTENT_ACCWALKTIME    = _T("출발 접근시간");
	}
	CString CONTENT_EGRWALKTIME  = _T("EgrWalkTime");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		CONTENT_EGRWALKTIME    = _T("도착 접근시간");
	}

	CString CONTENT_INVEHTIME    = _T("InVehTime");
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CONTENT_INVEHTIME    = _T("차내통행시간");
    }

	CString CONTENT_WAITTIME     = _T("WaitTime"); 
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CONTENT_WAITTIME     = _T("대기시간"); 
    }

	CString CONTENT_TRANSFERTIME = _T("TransferTime");
    if (KmzSystem::GetLanguage() == KEMKorea) {
        CONTENT_TRANSFERTIME = _T("환승시간");
    }

	try
	{
		/*
		맨 마지막 순서는 항상 Trip 으로
		*/
		KEMInterMode          &emInterMode          = a_pTrasitEditInfo->emInterMode;
		KEMInterRegDetailMode &emInterRegDetailMode = a_pTrasitEditInfo->emInterRegDetailMode;

		if (KEMInterModalPathModeRegional == m_emInterModalPathMode)
		{
			if(KEMInterRegDetailModeAuto == emInterRegDetailMode)
			{
				TRegAutoSubData     &oRegAutoSubData = a_pTrasitEditInfo->oTRegAutoSubData;
				
				RegisterDetailInfo(CONTENT_TIME,		oRegAutoSubData.dAutoTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_LENGTH,		0.0,							a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_COST,		oRegAutoSubData.dAutoCost,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRIP,		oRegAutoSubData.dTrip,			a_vecActiveDetailInfo);
			}
			else if(KEMInterRegDetailModeBus == emInterRegDetailMode || KEMInterRegDetailModeBusRail == emInterRegDetailMode)
			{
				TInEditBusSubData  &oTBusSubData = a_pTrasitEditInfo->oTBusSubData;

				RegisterDetailInfo(CONTENT_INVEHTIME,	oTBusSubData.dInVehTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_WAITTIME,	oTBusSubData.dWaitTime,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRANSFERTIME,oTBusSubData.dTransferTime,		a_vecActiveDetailInfo);
				if (KEMInterAccEgrTypeAccess == a_pTrasitEditInfo->emInterAccEgrType)
					RegisterDetailInfo(CONTENT_ACCWALKTIME,	oTBusSubData.dAccessWalkTime,		a_vecActiveDetailInfo); // ReadData - oTBusSubData.dAccessWalkTime에 walkTime 이 Input됨
				else
					RegisterDetailInfo(CONTENT_EGRWALKTIME,	oTBusSubData.dAccessWalkTime,		a_vecActiveDetailInfo); // ReadData - oTBusSubData.dAccessWalkTime에 walkTime 이 Input됨
				RegisterDetailInfo(CONTENT_LENGTH,		oTBusSubData.dLength,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_COST,		oTBusSubData.dCost,				a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRIP,		oTBusSubData.dTrip,				a_vecActiveDetailInfo);
			}
			else if(KEMInterRegDetailModeRail == emInterRegDetailMode)
			{
				TInEditRailSubData &oTRailSubData = a_pTrasitEditInfo->oTRailSubData;

				RegisterDetailInfo(CONTENT_INVEHTIME,	oTRailSubData.dInVehTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_WAITTIME,	oTRailSubData.dWaitTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRANSFERTIME,oTRailSubData.dTransferTime,	a_vecActiveDetailInfo);
				if (KEMInterAccEgrTypeAccess == a_pTrasitEditInfo->emInterAccEgrType)
					RegisterDetailInfo(CONTENT_ACCWALKTIME,	oTRailSubData.dAccessWalkTime,		a_vecActiveDetailInfo); // ReadData - oTBusSubData.dAccessWalkTime에 walkTime 이 Input됨
				else
					RegisterDetailInfo(CONTENT_EGRWALKTIME,	oTRailSubData.dAccessWalkTime,		a_vecActiveDetailInfo); // ReadData - oTBusSubData.dAccessWalkTime에 walkTime 이 Input됨
				RegisterDetailInfo(CONTENT_LENGTH,		oTRailSubData.dLength,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_COST,		oTRailSubData.dCost,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRIP,		oTRailSubData.dTrip,			a_vecActiveDetailInfo);
			}
		}
		else
		{
			if(KEMInterModeBus == emInterMode || KEMInterModeBusRail == emInterMode)
			{
				TInEditBusSubData  &oTBusSubData = a_pTrasitEditInfo->oTBusSubData;

				RegisterDetailInfo(CONTENT_INVEHTIME,	oTBusSubData.dInVehTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_WAITTIME,	oTBusSubData.dWaitTime,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRANSFERTIME,oTBusSubData.dTransferTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_ACCWALKTIME,	oTBusSubData.dAccessWalkTime,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_EGRWALKTIME,	oTBusSubData.dEgressWalkTime,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_LENGTH,		oTBusSubData.dLength,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_COST,		oTBusSubData.dCost,				a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRIP,		oTBusSubData.dTrip,				a_vecActiveDetailInfo);
			}
			else if(KEMInterModeRail == emInterMode)
			{
				TInEditRailSubData &oTRailSubData = a_pTrasitEditInfo->oTRailSubData;

				RegisterDetailInfo(CONTENT_INVEHTIME,	oTRailSubData.dInVehTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_WAITTIME,	oTRailSubData.dWaitTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRANSFERTIME,oTRailSubData.dTransferTime,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_ACCWALKTIME,	oTRailSubData.dAccessWalkTime,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_EGRWALKTIME,	oTRailSubData.dEgressWalkTime,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_LENGTH,		oTRailSubData.dLength,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_COST,		oTRailSubData.dCost,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRIP,		oTRailSubData.dTrip,			a_vecActiveDetailInfo);
			}
			else if(KEMInterModeAutoTaxi == emInterMode)
			{
				TInEditAutoSubData &oTAutoSubData = a_pTrasitEditInfo->oTAutoSubData;

				double dCost = oTAutoSubData.dAutoCost;
				if ( dCost < oTAutoSubData.dTaxiCost )
				{
					dCost = oTAutoSubData.dTaxiCost;
				}

				RegisterDetailInfo(CONTENT_TIME,	    oTAutoSubData.dTravelTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_LENGTH,		oTAutoSubData.dTravelLength,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_COST,		dCost,							a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRIP,		oTAutoSubData.dTrip,			a_vecActiveDetailInfo);
			}
			else if(KEMInterModeParkRide == emInterMode || KEMInterModeKissRide == emInterMode || KEMInterModeTaxiTransit == emInterMode)
			{
				TInEditMultiSubData &oTMultiSubData = a_pTrasitEditInfo->oTMultiSubData;
				
				double dCost = oTMultiSubData.dVehCost + oTMultiSubData.dTransitCost;

				// 2019.03.12 명지대 요청, 차내통행시간과 통행시간 순서 변경
				RegisterDetailInfo(CONTENT_TRAVELTIME,	oTMultiSubData.dVehTime,		a_vecActiveDetailInfo);// 승용차/택시
				RegisterDetailInfo(CONTENT_INVEHTIME,	oTMultiSubData.dTravelTime,		a_vecActiveDetailInfo);	
				// 2019.03.12 명지대 요청, 차내통행시간과 통행시간 순서 변경 끝
				RegisterDetailInfo(CONTENT_WAITTIME,	oTMultiSubData.dWaitTime,		a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRANSFERTIME,oTMultiSubData.dTransferTime,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_ACCWALKTIME,	oTMultiSubData.dAccessWalkTime,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_EGRWALKTIME,	oTMultiSubData.dEgressWalkTime,	a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_LENGTH,		oTMultiSubData.dLength,			a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_COST,		dCost,							a_vecActiveDetailInfo);
				RegisterDetailInfo(CONTENT_TRIP,		oTMultiSubData.dTrip,			a_vecActiveDetailInfo);
			}
		}
	}
	catch (...)
	{
		a_vecActiveDetailInfo.clear();
		return;
	}
}


void KInterResultDetailDlg::RegisterDetailInfo(CString a_strContentName, double a_dValue, std::vector<TInResultDetailActiveInfo> &a_vecActiveDetailInfo)
{
	try
	{
		TInResultDetailActiveInfo oTInResultDetailActiveInfo;

		oTInResultDetailActiveInfo.strContentName = a_strContentName;
		oTInResultDetailActiveInfo.dValue         = a_dValue;

		a_vecActiveDetailInfo.push_back(oTInResultDetailActiveInfo);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


// KInterResultDetailDlg 메시지 처리기입니다.

BOOL KInterResultDetailDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if (nullptr != m_pParentWnd) // Domodal로 오픈할 경우 m_pParentWnd == null
	{
		CRect rectParent;
		m_pParentWnd->GetWindowRect(&rectParent);

		CRect rect;
		GetWindowRect(rect);
		MoveWindow(rectParent.left + rectParent.Width(), rectParent.top + 320, rect.Width(), rect.Height());
	}

	KReportCtrlSetting::DefaultC(m_wndReportACtrl);
	KReportCtrlSetting::DefaultC(m_wndReportBCtrl);

	InitReportBHeader();
	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterResultDetailDlg::InitReportAHeader(std::vector<TInResultDetailActiveInfo> &a_vecActiveDetailInfo)
{
	m_wndReportACtrl.ResetContent();
	m_wndReportACtrl.GetColumns()->Clear();
	m_wndReportACtrl.Populate();

	CXTPReportColumn*                pColumn      = nullptr;

	size_t nxCount = a_vecActiveDetailInfo.size();
	for (size_t i= 0; i< nxCount; i++)
	{
		pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(i,         a_vecActiveDetailInfo[i].strContentName,       10));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);

		if (nxCount-1 == i) // 맨 마지막 trip
		{
			if (m_bShowTrip)
				pColumn->SetVisible(TRUE);
			else
				pColumn->SetVisible(FALSE);
		}
	}

	m_wndReportACtrl.Populate();
}


void KInterResultDetailDlg::InitReportBHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_B_COLUMN_TRANSITID,          _T("Line ID[Name]"),       50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("노선ID[노선명]"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_B_COLUMN_ROUTE,              _T("Route"),            35, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("구간"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_B_COLUMN_PATHTYPE,           _T("Type"),             35, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("유형"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_3_B_COLUMN_STARTSTATION,       _T("Start Station"),    20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("구간시점"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_4_B_COLUMN_ENDSTATION,        _T("End Station"),       20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("구간종점"));
    }

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_5_B_COLUMN_TRIP,              _T("Trip"),              10));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (m_bShowTrip)
		pColumn->SetVisible(TRUE);
	else
		pColumn->SetVisible(FALSE);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("통행량(인)"));
	}

	m_wndReportBCtrl.Populate();
}


void KInterResultDetailDlg::ResizeComponent()
{
	SetResize(IDC_REPORT_A,       			      SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR9,       			      SZ_TOP_LEFT,     SZ_TOP_RIGHT);

	SetResize(IDC_REPORT,       			      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR8,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_SUB,  			          SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
}


void KInterResultDetailDlg::UpdateEditReportAData(std::vector<TInResultDetailActiveInfo> &a_vecActiveDetailInfo)
{
	m_wndReportACtrl.ResetContent();

	try
	{
		CXTPReportRecord*         pRecord     = nullptr;
		CXTPReportRecordItem*     pItem       = nullptr;

		pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());

		size_t nxCount = a_vecActiveDetailInfo.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(a_vecActiveDetailInfo[i].dValue,   _T("%.2f")));
			pItem->SetAlignment(DT_CENTER);
		}

		m_wndReportACtrl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	} 
}


void KInterResultDetailDlg::UpdateEditReportBData(KInterPathEditInfo* a_pTrasitEditInfo)
{
	m_wndReportBCtrl.ResetContent();

	try
	{
		CXTPReportRecord*         pRecord     = nullptr;
		CXTPReportRecordItem*     pItem       = nullptr;
		CXTPReportRecordItemText* pItemText   = nullptr;

		Integer nxStartStationID(0);
		Integer nxEndStationID(0);
		CString strTransitName(_T(""));

		std::vector<TInIndexResultTransitInfo> &vecResulTransitInfo = a_pTrasitEditInfo->vecResulTransitInfo;

		int     nIdx(0);
		CString strIdx(_T(""));
		for (size_t i= 0; i< vecResulTransitInfo.size(); i++)
		{
			std::vector<TInResultTransitInfo>      &vecLowResultTransitInfo = vecResulTransitInfo[i].vecLowResultTransitInfo;
			KODKey                                 &oIndexKey               = vecResulTransitInfo[i].oIndexKey;

			if(vecLowResultTransitInfo.size() < 1)
				continue;

			pRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());

			Integer &nxFIndex = oIndexKey.OriginID;
			Integer &nxTIndex = oIndexKey.DestinationID;
            
            if (KmzSystem::GetLanguage() == KEMKorea) {
                if (0 == nxTIndex) {
                    strIdx.Format(_T("구간 %I64d"), nxFIndex);
                }
                else {
                    strIdx.Format(_T("구간 %I64d-%I64d"), nxFIndex, nxTIndex);
                }
            }
            else {
                if (0 == nxTIndex) {
                    strIdx.Format(_T("Section %I64d"), nxFIndex);
                }
                else {
                    strIdx.Format(_T("Section %I64d-%I64d"), nxFIndex, nxTIndex);
                }
            }			

			pItem  = pRecord->AddItem(new CXTPReportRecordItemText(strIdx));
			for (int n= 0; n< COLUMN_TOTAL_COUNT-1; n++)
			{
				pRecord->AddItem(new CXTPReportRecordItemText());
			}
			pItem->SetAlignment(DT_LEFT);

			CXTPReportRecords* pChildRecords = pRecord->GetChilds();

			for (size_t k= 0; k< vecLowResultTransitInfo.size(); k++)
			{
				TInResultTransitInfo &oTMResultBusInfo = vecLowResultTransitInfo[k];

				CXTPReportRecord* pChildRecord    = pChildRecords->Add(new CXTPReportRecord());

				KInterModalPathInfo::GetStatoinIDFromSEQ(m_pTarget, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartStationID);
				KInterModalPathInfo::GetStatoinIDFromSEQ(m_pTarget, oTMResultBusInfo.nEndStation,   oTMResultBusInfo.nxTransitID, nxEndStationID);
				GetTransitName(m_pTarget, oTMResultBusInfo.nxTransitID, strTransitName); 

				CString strTransit(_T(""));
				strTransit.Format(_T("%I64d [%s]"), oTMResultBusInfo.nxTransitID, strTransitName);
				CString strTransitID(_T(""));
				strTransitID.Format(_T("%I64d"), oTMResultBusInfo.nxTransitID);
				pItemText = (CXTPReportRecordItemText*)pChildRecord->AddItem(new CXTPReportRecordItemText());
				pItemText->SetValue(strTransitID);
				pItemText->SetCaption(strTransit);
				pItemText->SetAlignment(DT_LEFT);

				pItem = pChildRecord->AddItem(new CXTPReportRecordItem());
				CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				pButton->SetAlignment(xtpItemControlUnknown);
				
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    pButton->SetCaption(_T("표출"));
                    pButton->SetSize(CSize(35, 0));
                }
                else {
                    pButton->SetCaption(_T("R"));
                    pButton->SetSize(CSize(35, 0));
                }
                
				pButton->SetThemed(TRUE);

				CString strType(_T(""));
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    if(KEMPathTypeBus == oTMResultBusInfo.emPathType)
                        strType=_T("버스");
                    else
                        strType=_T("도시철도");
                }
                else {
                    if(KEMPathTypeBus == oTMResultBusInfo.emPathType)
                        strType=_T("Bus");
                    else
                        strType=_T("Rail");
                }
				
				pItem = pChildRecord->AddItem(new CXTPReportRecordItemText(strType));
				pItem->SetAlignment(DT_CENTER);

				CString strStartStationName(_T("")); {
					auto itFind = m_mapNodeName.find(nxStartStationID);
					if (itFind != m_mapNodeName.end()) {
						strStartStationName = itFind->second;
					}
					if (strStartStationName.IsEmpty() == true) {
						strStartStationName = _T("-");
					}
				}

				CString strEndStationName(_T("")); {
					auto itFind = m_mapNodeName.find(nxEndStationID);
					if (itFind != m_mapNodeName.end()) {
						strEndStationName = itFind->second;
					}
					if (strEndStationName.IsEmpty() == true) {
						strEndStationName = _T("-");
					}
				}

				int &nStartStationSeq = oTMResultBusInfo.nStartStation;
				CString strStartStation(_T(""));
				if (nStartStationSeq <= 1)
				{
					strStartStation.Format(_T("%I64d(%d) [%s]"), nxStartStationID, nStartStationSeq, strStartStationName);
				}
				else
				{
					strStartStation.Format(_T("%I64d(%d) [%s]"), nxStartStationID, nStartStationSeq-1, strStartStationName);
				}

				pItem = pChildRecord->AddItem(new CXTPReportRecordItemText(strStartStation));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetItemData((DWORD_PTR)nxStartStationID);

				int &nEndStationSeq = oTMResultBusInfo.nEndStation;
				CString strEndStation(_T(""));
				strEndStation.Format(_T("%I64d(%d) [%s]"), nxEndStationID, nEndStationSeq-1, strEndStationName);
				pItem = pChildRecord->AddItem(new CXTPReportRecordItemText(strEndStation));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetItemData((DWORD_PTR)nxEndStationID);

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), oTMResultBusInfo.dTrip);
				pItem = pChildRecord->AddItem(new CXTPReportRecordItemText(strTrip));
				pItem->SetAlignment(DT_CENTER);
			}
		}

		m_wndReportBCtrl.Populate();
		m_wndReportBCtrl.ExpandAll(TRUE);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	} 
}


void KInterResultDetailDlg::GetTransitName(KTarget* a_pTarget, Integer &a_nxTransitID, CString &a_strTransitName)
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" SELECT name FROM %s WHERE transit_ID = %I64d "), TABLE_TRANSIT, a_nxTransitID);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while (spResult->Next())
		{
			a_strTransitName = spResult->GetValueString(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}
}


void KInterResultDetailDlg::OnReportBItemDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		//         XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		//         if (nullptr == pClickedItem)
		//             return;
		// 
		//         CXTPReportRow* pRow = pClickedItem->pRow;
		//         if (nullptr == pRow)
		//             return;
		// 
		//         pClickedItem->pRow->SetSelected(TRUE);	
		//         int nIndex = pClickedItem->pColumn->GetItemIndex();
		// 
		//         CXTPReportRecord*         pRecord         = pRow->GetRecord();
		//         CXTPReportRecordItem*     pRecordItem     = pRecord->GetItem(/*nIndex*/0);
		//         CXTPReportRecordItemText* pRecordItemText = dynamic_cast<CXTPReportRecordItemText*>(pRecordItem);
		//         if (nullptr == pRecordItemText)
		//             throw 1;
		// 
		//         KMapView* pMapView = ImChampFrameWindow::GetShowMapView(m_pTarget);
		//         if (nullptr == pMapView)
		//             throw 1;
		// 
		//         Integer nxTransitID = _ttoi64(pRecordItemText->GetValue());
		// 
		//         if( nullptr == m_spTransitPathViewDlg)
		//         {
		//             m_spTransitPathViewDlg = KTransitPathViewDlgPtr(new KTransitPathViewDlg(m_pTarget, pMapView));
		//             m_spTransitPathViewDlg->Create(KTransitPathViewDlg::IDD);
		//         }
		// 
		//         m_spTransitPathViewDlg->SetViewTransit(nxTransitID);
		//         m_spTransitPathViewDlg->ShowWindow(SW_SHOW);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KInterResultDetailDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

		if (!pItemNotify->pRow || !pItemNotify->pColumn)
			return;

		int nIndex = pItemNotify->pColumn->GetItemIndex();

		if( _3_B_COLUMN_STARTSTATION == nIndex || _4_B_COLUMN_ENDSTATION == nIndex) 
		{
			CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();

			if ( pRecord->HasChildren() == TRUE )
			{
				return;
			}

			CXTPReportRecordItem* pItem   = pRecord->GetItem(nIndex);
			Integer nxNodeID = (Integer)pItem->GetItemData();

			KMapView* pMapView = ImChampFrameWindow::GetShowMapView(m_pTarget);
			if (nullptr == pMapView)
				throw 1;

			pMapView->FlashToFeature(KLayerID::Node(), nxNodeID);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KInterResultDetailDlg::OnItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
		if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
			return;

		CXTPReportRecordItem* pItem = pItemNotify->pItem;

		if(_1_B_COLUMN_ROUTE == pItem->GetIndex())
		{
			pItemNotify->pRow->SetSelected(TRUE);	

			CXTPReportRecord*     pRecord        = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItemB         = pRecord->GetItem(_0_B_COLUMN_TRANSITID);

			CXTPReportRecordItemText* pRecordItemText = dynamic_cast<CXTPReportRecordItemText*>(pItemB);
			if (nullptr == pRecordItemText)
				throw 1;

			KMapView* pMapView = ImChampFrameWindow::GetShowMapView(m_pTarget);
			if (nullptr == pMapView)
				throw 1;

			Integer nxTransitID = _ttoi64(pRecordItemText->GetValue());

			m_spTransitPathViewDlg = KTransitPathViewDlgPtr(new KTransitPathViewDlg(m_pTarget, pMapView));
			m_spTransitPathViewDlg->Create(KTransitPathViewDlg::IDD);
			m_spTransitPathViewDlg->SetViewTransit(nxTransitID);
			m_spTransitPathViewDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KInterResultDetailDlg::OnBnClickedCancel()
{
	if (nullptr != m_spTransitPathViewDlg)
		m_spTransitPathViewDlg.reset();
	KResizeDialogEx::OnCancel();
}


void KInterResultDetailDlg::ActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spTransitPathViewDlg)
	{
		if(!m_spTransitPathViewDlg->IsUserClosed())
		{
			if(!m_spTransitPathViewDlg->IsWindowVisible())
				m_spTransitPathViewDlg->ShowWindow(SW_SHOW);
		}
	}
}


void KInterResultDetailDlg::DeActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spTransitPathViewDlg)
	{
		if(!m_spTransitPathViewDlg->IsUserClosed())
		{
			if(m_spTransitPathViewDlg->IsWindowVisible())
				m_spTransitPathViewDlg->ShowWindow(SW_HIDE);
		}
	}
}
