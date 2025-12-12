// MultiModalResultDetailDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultiModalResultDetailDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
#include "ImChampFrameWindow.h"

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

// KMultiModalResultDetailDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMultiModalResultDetailDlg, KResizeDialogEx)

KMultiModalResultDetailDlg::KMultiModalResultDetailDlg(KTarget* a_pTarget, bool a_bShowTrip, std::map<Integer, CString> &a_mapNodeName, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMultiModalResultDetailDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_bShowTrip(a_bShowTrip)
	, m_mapNodeName(a_mapNodeName)
	, m_pParentWnd(pParent)
{
	
}

KMultiModalResultDetailDlg::~KMultiModalResultDetailDlg()
{
}

void KMultiModalResultDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_REPORT_A,               m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT,                 m_wndReportBCtrl);
}


BEGIN_MESSAGE_MAP(KMultiModalResultDetailDlg, KResizeDialogEx)
    ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KMultiModalResultDetailDlg::OnReportBItemDblClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT,  &KMultiModalResultDetailDlg::OnReportBItemClick)
	ON_NOTIFY (XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, &KMultiModalResultDetailDlg::OnItemButtonClick)
	ON_BN_CLICKED(IDCANCEL, &KMultiModalResultDetailDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


void KMultiModalResultDetailDlg::InitializeData( CString a_strPathIdex, KMBusAssignInfo* a_pTrasitAssignInfo, CRect rectParent )
{
	Integer              &nxOrginID           = a_pTrasitAssignInfo->nxOrginID;
	Integer              &nxDestinationID     = a_pTrasitAssignInfo->nxDestinationID;
	KEMTransitAssignMode &emTransitAssignMode = a_pTrasitAssignInfo->emTransitAssignMode;
	CString strAssignType(_T(""));
    CString strSubTitle(_T(""));

    if (KmzSystem::GetLanguage() == KEMKorea) {
        if(KEMPathTypeBus == emTransitAssignMode)
            strAssignType = _T("버스");
        else if(KEMPathTypeRail == emTransitAssignMode)
            strAssignType = _T("도시철도");
        else
            strAssignType = _T("버스+도시철도");

        strSubTitle.Format(_T("기점 : %I64d    종점 : %I64d    [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
    }
    else {
        if(KEMPathTypeBus == emTransitAssignMode)
            strAssignType = _T("Bus");
        else if(KEMPathTypeRail == emTransitAssignMode)
            strAssignType = _T("Rail");
        else
            strAssignType = _T("Bus+Rail");

        strSubTitle.Format(_T("Origin:%I64d  Destination:%I64d [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
    }	

	SetDlgItemText(IDC_STATIC_SUB, strSubTitle);

	UpdateReportAData(a_pTrasitAssignInfo);
	UpdateReportBData(a_pTrasitAssignInfo);

// 	CRect rect;
// 	GetWindowRect(rect);
// 
// 	MoveWindow(rectParent.left + rectParent.Width(), rectParent.top + 200, rect.Width(), rect.Height());

}


void KMultiModalResultDetailDlg::InitializeEditData( CString a_strPathIdex, KMTransitEditInfo* a_pTrasitEditInfo, CRect rectParent )
{
	Integer              &nxOrginID           = a_pTrasitEditInfo->nxOrginID;
	Integer              &nxDestinationID     = a_pTrasitEditInfo->nxDestinationID;
	KEMTransitAssignMode &emTransitAssignMode = a_pTrasitEditInfo->emTransitAssignMode;
	CString               strAssignType(_T(""));
	CString               strSubTitle(_T(""));


	if (KmzSystem::GetLanguage() == KEMKorea) {
		if(KEMPathTypeBus == emTransitAssignMode)
			strAssignType = _T("버스");
		else if(KEMPathTypeRail == emTransitAssignMode)
			strAssignType = _T("도시철도");
		else
			strAssignType = _T("버스+도시철도");

		strSubTitle.Format(_T("기점 : %I64d    종점 : %I64d    [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
	}
	else {
		if(KEMPathTypeBus == emTransitAssignMode)
			strAssignType = _T("Bus");
		else if(KEMPathTypeRail == emTransitAssignMode)
			strAssignType = _T("Rail");
		else
			strAssignType = _T("Bus+Rail");

		strSubTitle.Format(_T("Origin:%I64d Destination:%I64d [%s %s]"), nxOrginID, nxDestinationID, strAssignType, a_strPathIdex);
	}	

	SetDlgItemText(IDC_STATIC_SUB, strSubTitle);

	UpdateEditReportAData(a_pTrasitEditInfo);
	UpdateEditReportBData(a_pTrasitEditInfo);
}


void KMultiModalResultDetailDlg::ActivatedFrameWndNotifyProcess()
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


void KMultiModalResultDetailDlg::DeActivatedFrameWndNotifyProcess()
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


// KMultiModalResultDetailDlg 메시지 처리기입니다.

BOOL KMultiModalResultDetailDlg::OnInitDialog()
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

	InitReportAHeader();
	InitReportBHeader();
	ResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KMultiModalResultDetailDlg::InitReportAHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_0_A_COLUMN_INVEH_TIME,         _T("InVehTime"),       12));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("차내통행시간"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_1_A_COLUMN_WAIT_TIME,          _T("WaitTime"),        12));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("대기시간"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_2_A_COLUMN_TRANS_TIME,         _T("TransferTime"),    12));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("환승시간"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_3_A_COLUMN_ACCWALK_TIME,       _T("AccWalkTime"),     12));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("출발접근시간"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_4_A_COLUMN_EGRWALK_TIME,       _T("EgrWalkTime"),     12));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도착접근시간"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_5_A_COLUMN_COST,               _T("Cost"),            14));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행비용"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_6_A_COLUMN_LENGTH,             _T("Length"),          13));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행거리"));
    }

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_7_A_COLUMN_TRIP,               _T("Trip"),            13));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (m_bShowTrip)
		pColumn->SetVisible(TRUE);
	else
		pColumn->SetVisible(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("통행량(인)"));
	}

	m_wndReportACtrl.Populate();
}


void KMultiModalResultDetailDlg::InitReportBHeader()
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

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_B_COLUMN_PATHTYPE,           _T("Type"),             45, FALSE));
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
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("통행량(인)"));
	}

	m_wndReportBCtrl.Populate();
}


void KMultiModalResultDetailDlg::ResizeComponent()
{
	SetResize(IDC_REPORT_A,       			      SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR9,       			      SZ_TOP_LEFT,     SZ_TOP_RIGHT);

	SetResize(IDC_REPORT,       			      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR8,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_SUB,  			          SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
}


void KMultiModalResultDetailDlg::UpdateReportAData(KMBusAssignInfo* a_pTrasitAssignInfo)
{
	m_wndReportACtrl.ResetContent();

	try
	{
		TPathSubData oTPathSubData;
		GetPathInfoData(a_pTrasitAssignInfo, oTPathSubData);

		CXTPReportRecord*         pRecord     = nullptr;
		CXTPReportRecordItem*     pItem       = nullptr;

		pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dInVehTime,      _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dWaitTime,       _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dTransferTime,   _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dAccessWalkTime, _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dEgressWalkTime, _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dCost,           _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dLength,         _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dAssignTrip,     _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

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


void KMultiModalResultDetailDlg::UpdateEditReportAData(KMTransitEditInfo* a_pTrasitEditInfo)
{
	m_wndReportACtrl.ResetContent();

	try
	{
		TPathSubData oTPathSubData;
		GetEditPathInfoData(a_pTrasitEditInfo, oTPathSubData);

		CXTPReportRecord*         pRecord     = nullptr;
		CXTPReportRecordItem*     pItem       = nullptr;

		pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dInVehTime,      _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dWaitTime,       _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dTransferTime,   _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dAccessWalkTime, _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dEgressWalkTime, _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dCost,           _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dLength,         _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathSubData.dAssignTrip,     _T("%.2f")));
		pItem->SetAlignment(DT_CENTER);

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


void KMultiModalResultDetailDlg::GetPathInfoData(KMBusAssignInfo* a_pTrasitAssignInfo, TPathSubData &a_oTPathSubData)
{
	try
	{
		KEMTransitAssignMode &emAssignMode = a_pTrasitAssignInfo->emTransitAssignMode;

		if(KEMTransitAssignBus == emAssignMode)
		{
			TBusSubData  &oTBusSubData = a_pTrasitAssignInfo->oTBusSubData;

			a_oTPathSubData.dInVehTime       = oTBusSubData.dInVehTime;
			a_oTPathSubData.dWaitTime        = oTBusSubData.dWaitTime;
			a_oTPathSubData.dTransferTime    = oTBusSubData.dTransferTime;
			a_oTPathSubData.dAccessWalkTime  = oTBusSubData.dAccessWalkTime;
			a_oTPathSubData.dEgressWalkTime  = oTBusSubData.dEgressWalkTime;
			a_oTPathSubData.dCost            = oTBusSubData.dCost;
			a_oTPathSubData.dLength          = oTBusSubData.dLength;
			a_oTPathSubData.dAssignTrip      = oTBusSubData.dAssignTrip;
		}
		else if(KEMTransitAssignRail == emAssignMode)
		{
			TRailSubData &oTRailSubData = a_pTrasitAssignInfo->oTRailSubData;

			a_oTPathSubData.dInVehTime       = oTRailSubData.dInVehTime;
			a_oTPathSubData.dWaitTime        = oTRailSubData.dWaitTime;
			a_oTPathSubData.dTransferTime    = oTRailSubData.dTransferTime;
			a_oTPathSubData.dAccessWalkTime  = oTRailSubData.dAccessWalkTime;
			a_oTPathSubData.dEgressWalkTime  = oTRailSubData.dEgressWalkTime;
			a_oTPathSubData.dCost            = oTRailSubData.dCost;
			a_oTPathSubData.dLength          = oTRailSubData.dLength;
			a_oTPathSubData.dAssignTrip      = oTRailSubData.dAssignTrip;
		}
		else
		{
			TBusSubData  &oTBusSubData = a_pTrasitAssignInfo->oTBusSubData;

			a_oTPathSubData.dInVehTime       = oTBusSubData.dInVehTime;
			a_oTPathSubData.dWaitTime        = oTBusSubData.dWaitTime;
			a_oTPathSubData.dTransferTime    = oTBusSubData.dTransferTime;
			a_oTPathSubData.dAccessWalkTime  = oTBusSubData.dAccessWalkTime;
			a_oTPathSubData.dEgressWalkTime  = oTBusSubData.dEgressWalkTime;
			a_oTPathSubData.dCost            = oTBusSubData.dCost;
			a_oTPathSubData.dLength          = oTBusSubData.dLength;
			a_oTPathSubData.dAssignTrip      = oTBusSubData.dAssignTrip;
		}
	}
	catch (...)
	{
		return;
	}
}


void KMultiModalResultDetailDlg::GetEditPathInfoData(KMTransitEditInfo* a_pTrasitEditInfo, TPathSubData &a_oTPathSubData)
{
	try
	{
		KEMTransitAssignMode &emAssignMode = a_pTrasitEditInfo->emTransitAssignMode;

		if(KEMTransitAssignBus == emAssignMode)
		{
			TEditBusSubData  &oTBusSubData = a_pTrasitEditInfo->oTBusSubData;

			a_oTPathSubData.dInVehTime       = oTBusSubData.dInVehTime;
			a_oTPathSubData.dWaitTime        = oTBusSubData.dWaitTime;
			a_oTPathSubData.dTransferTime    = oTBusSubData.dTransferTime;
			a_oTPathSubData.dAccessWalkTime  = oTBusSubData.dAccessWalkTime;
			a_oTPathSubData.dEgressWalkTime  = oTBusSubData.dEgressWalkTime;
			a_oTPathSubData.dCost            = oTBusSubData.dCost;
			a_oTPathSubData.dLength          = oTBusSubData.dLength;
			a_oTPathSubData.dAssignTrip      = 0.0;
		}
		else if(KEMTransitAssignRail == emAssignMode)
		{
			TEditRailSubData &oTRailSubData = a_pTrasitEditInfo->oTRailSubData;

			a_oTPathSubData.dInVehTime       = oTRailSubData.dInVehTime;
			a_oTPathSubData.dWaitTime        = oTRailSubData.dWaitTime;
			a_oTPathSubData.dTransferTime    = oTRailSubData.dTransferTime;
			a_oTPathSubData.dAccessWalkTime  = oTRailSubData.dAccessWalkTime;
			a_oTPathSubData.dEgressWalkTime  = oTRailSubData.dEgressWalkTime;
			a_oTPathSubData.dCost            = oTRailSubData.dCost;
			a_oTPathSubData.dLength          = oTRailSubData.dLength;
			a_oTPathSubData.dAssignTrip      = 0.0;
		}
		else
		{
			TEditBusSubData  &oTBusSubData = a_pTrasitEditInfo->oTBusSubData;

			a_oTPathSubData.dInVehTime       = oTBusSubData.dInVehTime;
			a_oTPathSubData.dWaitTime        = oTBusSubData.dWaitTime;
			a_oTPathSubData.dTransferTime    = oTBusSubData.dTransferTime;
			a_oTPathSubData.dAccessWalkTime  = oTBusSubData.dAccessWalkTime;
			a_oTPathSubData.dEgressWalkTime  = oTBusSubData.dEgressWalkTime;
			a_oTPathSubData.dCost            = oTBusSubData.dCost;
			a_oTPathSubData.dLength          = oTBusSubData.dLength;
			a_oTPathSubData.dAssignTrip      = 0.0;
		}
	}
	catch (...)
	{
		return;
	}
}


void KMultiModalResultDetailDlg::UpdateReportBData(KMBusAssignInfo* a_pTrasitAssignInfo)
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

		std::vector<TIndexResultTransitInfo> &vecResultBusInfo = a_pTrasitAssignInfo->vecResultBusInfo;
		
		int     nIdx(0);
		CString strIdx(_T(""));
		for (size_t i= 0; i< vecResultBusInfo.size(); i++)
		{
			std::vector<TMResultBusInfo> &vecLowResultTransitInfo = vecResultBusInfo[i].vecLowResultTransitInfo;
			KODKey                       &oIndexKey               = vecResultBusInfo[i].oIndexKey;

			if(vecLowResultTransitInfo.size() < 1)
				continue;

			pRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());

			Integer &nxFIndex = oIndexKey.OriginID;
			Integer &nxTIndex = oIndexKey.DestinationID;

			if (KmzSystem::GetLanguage() == KEMKorea) {
				if (0 == nxTIndex) {
					strIdx.Format(_T("구간%I64d"), nxFIndex);
				}
				else {
					strIdx.Format(_T("구간%I64d-%I64d"), nxFIndex, nxTIndex);
				}
			}
			else {
				if (0 == nxTIndex) {
					strIdx.Format(_T("Section%I64d"), nxFIndex);
				}
				else {
					strIdx.Format(_T("Section%I64d-%I64d"), nxFIndex, nxTIndex);
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
				TMResultBusInfo &oTMResultBusInfo = vecLowResultTransitInfo[k];

				CXTPReportRecord* pChildRecord    = pChildRecords->Add(new CXTPReportRecord());
				
				KMultiModalResultInfo::GetStatoinIDFromSEQ(m_pTarget, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartStationID);
				KMultiModalResultInfo::GetStatoinIDFromSEQ(m_pTarget, oTMResultBusInfo.nEndStation,   oTMResultBusInfo.nxTransitID, nxEndStationID);
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


void KMultiModalResultDetailDlg::UpdateEditReportBData(KMTransitEditInfo* a_pTrasitEditInfo)
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

		std::vector<TIndexResultTransitInfo> &vecResultBusInfo = a_pTrasitEditInfo->vecResultBusInfo;

		int     nIdx(0);
		CString strIdx(_T(""));
		for (size_t i= 0; i< vecResultBusInfo.size(); i++)
		{
			std::vector<TMResultBusInfo> &vecLowResultTransitInfo = vecResultBusInfo[i].vecLowResultTransitInfo;
			KODKey                       &oIndexKey               = vecResultBusInfo[i].oIndexKey;

			if(vecLowResultTransitInfo.size() < 1)
				continue;

			pRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());

			Integer &nxFIndex = oIndexKey.OriginID;
			Integer &nxTIndex = oIndexKey.DestinationID;
			
            if (KmzSystem::GetLanguage() == KEMKorea) {
                if (0 == nxTIndex) {
                    strIdx.Format(_T("구간%I64d"), nxFIndex);
                }
                else {
                    strIdx.Format(_T("구간%I64d-%I64d"), nxFIndex, nxTIndex);
                }
            }
            else {
                if (0 == nxTIndex) {
                    strIdx.Format(_T("Section%I64d"), nxFIndex);
                }
                else {
                    strIdx.Format(_T("Section%I64d-%I64d"), nxFIndex, nxTIndex);
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
				TMResultBusInfo &oTMResultBusInfo = vecLowResultTransitInfo[k];

				CXTPReportRecord* pChildRecord    = pChildRecords->Add(new CXTPReportRecord());

				KMultiModalResultInfo::GetStatoinIDFromSEQ(m_pTarget, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartStationID);
				KMultiModalResultInfo::GetStatoinIDFromSEQ(m_pTarget, oTMResultBusInfo.nEndStation,   oTMResultBusInfo.nxTransitID, nxEndStationID);
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

				CString strStartStationName(_T("-")); {
					auto itFind = m_mapNodeName.find(nxStartStationID);
					if (itFind != m_mapNodeName.end()) {
						strStartStationName = itFind->second;
					}
				}
				CString strEndStationName(_T("-")); {
					auto itFind = m_mapNodeName.find(nxEndStationID);
					if (itFind != m_mapNodeName.end()) {
						strEndStationName = itFind->second;
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


void KMultiModalResultDetailDlg::GetTransitName(KTarget* a_pTarget, Integer &a_nxTransitID, CString &a_strTransitName)
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


void KMultiModalResultDetailDlg::OnReportBItemDblClick( NMHDR* pNMHDR, LRESULT* pResult )
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


void KMultiModalResultDetailDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KMultiModalResultDetailDlg::OnItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KMultiModalResultDetailDlg::OnBnClickedCancel()
{
	if (nullptr != m_spTransitPathViewDlg)
		m_spTransitPathViewDlg.reset();
	KResizeDialogEx::OnCancel();
}
