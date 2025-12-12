// ShortestPathNTransitDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "ShortestPathNTransitDlg.h"
#include "afxdialogex.h"
#include "Target.h"

//^#include "DBaseConnector.h"
#include "..\QBicCommon\ColorRangeGen.h"
#include "InterDefineCostPublicSector.h"
#include "KEMNode.h"
#include "KExRecordItem.h"
//#include "StringChecker.h"
//^^ #include "IOColumn.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
//^#include "MunsellColorCircle.h"

#include "Launcher.h"
#include "WriteCodeAgrument.h"
#include "ImChampDir.h"

enum
{
	_0_R_COLUMN_ROUTE_NAME,
	_1_R_COLUMN_TOTAL_LENGTH,
	_2_R_COLUMN_TOTAL_COST,
	_3_R_COLUMN_TOTAL_ALLTIME,
	_4_R_COLUMN_TOTAL_TIME_VEH,
	_5_R_COLUMN_TOTAL_TIME_WAIT,
	_6_R_COLUMN_TOTAL_TIME_TRANS,
	_7_R_COLUMN_TOTAL_TIME_ACCESSWALK,
	_8_R_COLUMN_TOTAL_TIME_EGRESSWALK
};
#define R_COLUMN_TOTAL_COUNT 9

enum
{
	_0_P_COLUMN_TRANSITID = 0,
	_1_P_COLUMN_ROUTE,
	_2_P_COLUMN_PATHTYPE,
	_3_P_COLUMN_NODE_START, 
	_4_P_COLUMN_NODE_END,
};
#define P_COLUMN_TOTAL_COUNT 6

enum KEMTimeMethod
{
	KEMTimeMethodInitTime     = 1,
	KEMTimeMethodUsingField   = 2
};

enum KENUMMode
{
	KENUMModeBus      = 1,
	KENUMModeRail     = 2,
	KENUMModeBusRail  = 3
};

// KShortestPathNTransitDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KShortestPathNTransitDlg, KResizeDialogEx)

KShortestPathNTransitDlg::KShortestPathNTransitDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KShortestPathNTransitDlg::IDD, pParent)
	, m_nRouteCount(INIT_ROUTECOUNT)
	, m_nWalkTime(INIT_WALKTIME)
	, m_bZone(FALSE)
{
	m_nxStartNodeID = -1;
	m_nxEndNodeID   = -1;

	m_clrBus     = RGB (   0, 128,   0 );
	m_clrRail    = RGB (   0,   0, 255 );
	m_clrWalk    = RGB ( 102, 102, 153 );

	m_strPreSelectedColumnName = _T("");

	m_oStartNodeIdCaption.TID = _T("-1");
	m_oEndNodeIdCaption.TID   = _T("-1");
}

KShortestPathNTransitDlg::~KShortestPathNTransitDlg()
{
	ClearInterDefineCostPublicSectorVector();
	CloseWindowProc();
	m_pLinkTable->Unregister(this);
	m_backBrush.DeleteObject();
}

void KShortestPathNTransitDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1,                 m_ReportCtrlRoute);
	DDX_Control(pDX, IDC_REPORT2,                 m_ReportCtrlPath);

	DDX_Text(pDX,    IDC_EDIT1,  m_nRouteCount);
	DDX_Control(pDX, IDC_CHECK_ZONESTART,        m_chkStartZone);
	DDX_Control(pDX, IDC_CHECK_ZONEEND,          m_chkEndZone);

	DDX_Control(pDX, IDC_EDIT1, m_edtRouteCount);
	DDV_MinMaxInt(pDX, m_nRouteCount, 0, 9999);
	DDX_Control(pDX, IDC_EDIT2, m_edtWalkTime);
	DDX_Text(pDX, IDC_EDIT2, m_nWalkTime);
	DDX_Control(pDX, IDC_COMBO_USINGFIELD, m_cboUsingLinkField);
	DDX_Control(pDX, IDC_CLR_BUS,                  m_cpBusColor);
	DDX_Control(pDX, IDC_CLR_RAIL,                 m_cpRailColor);
	DDX_Control(pDX, IDC_CLR_WALK,              m_cpWalkColor);
}


BEGIN_MESSAGE_MAP(KShortestPathNTransitDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK,                    IDC_REPORT1, OnRouteReportItemClick)
	ON_NOTIFY(NM_CLICK,                    IDC_REPORT2, OnPathReportItemClick)
	ON_NOTIFY (XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT2, &KShortestPathNTransitDlg::OnItemButtonClick)
	
	ON_BN_CLICKED(IDC_BUTTON1, &KShortestPathNTransitDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BUTTON4, &KShortestPathNTransitDlg::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDC_BUTTON5, &KShortestPathNTransitDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDCANCEL,    &KShortestPathNTransitDlg::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_EDIT1, &KShortestPathNTransitDlg::OnEnSetfocusRouteCount)
	ON_EN_SETFOCUS(IDC_EDIT2, &KShortestPathNTransitDlg::OnEnSetfocusWalkTime)
	ON_WM_COPYDATA()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, &KShortestPathNTransitDlg::OnBnClickedRadioTimeMethod)
	ON_BN_CLICKED(IDC_CHECK_ALL, &KShortestPathNTransitDlg::OnBnClickedCheckAll)
	ON_CPN_XTP_SELENDOK(IDC_CLR_BUS, &KShortestPathNTransitDlg::OnSelEndOkBusClr)
	ON_CPN_XTP_SELENDOK(IDC_CLR_RAIL, &KShortestPathNTransitDlg::OnSelEndOkRailClr)
	ON_CBN_SELCHANGE(IDC_COMBO_USINGFIELD, &KShortestPathNTransitDlg::OnCbnSelchangeComboUsingfield)

	ON_BN_CLICKED(IDC_BUTTON6, &KShortestPathNTransitDlg::OnBnClickedButtonStartNodeSearch)
	ON_BN_CLICKED(IDC_BUTTON7, &KShortestPathNTransitDlg::OnBnClickedButtonEndNodeSearch)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


KMapView* KShortestPathNTransitDlg::GetMapView(KTarget* a_pTarget)
{
	KMapView* pMapView = NULL;
	unsigned int nID = a_pTarget->GetObjectID();

	CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)AfxGetMainWnd();
	CChildFrame*   pChildFrame = pMainFrame->GetChildFrame(nID);

	if(NULL != pChildFrame)
	{
		pMapView = pChildFrame->GetMapView();
	}

	return pMapView;
}


void KShortestPathNTransitDlg::CloseWindowProc()
{
	AllClearShortestPath(true);
	if (nullptr != m_spTransitPathViewDlg)
		m_spTransitPathViewDlg.reset();

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();
}


void KShortestPathNTransitDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
    try
    {
        CString strSubjectName(a_strSubjectName);

        if(strSubjectName.CompareNoCase(TABLE_LINK) == 0)
        {
            InitComboLinkField();
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}


void KShortestPathNTransitDlg::ActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(!m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_SHOW);
		}
	}
}


void KShortestPathNTransitDlg::DeActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_HIDE);
		}
	}
}


BOOL KShortestPathNTransitDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWaitCursor wc;

	if(m_pTarget != NULL)
	{
		CString strTargetYear = DisplayScenarioTitle( m_pTarget->Caption() );
		SetDlgItemText(IDC_STATIC_TARGETYEAR, strTargetYear);
	}
	CheckDlgButton(IDC_CHECK_B,  BST_CHECKED);
	CheckDlgButton(IDC_CHECK_R,  BST_CHECKED);
	CheckDlgButton(IDC_CHECK_BR, BST_CHECKED);

	CheckRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, IDC_RADIO_INITTIME);
	OnBnClickedRadioTimeMethod(IDC_RADIO_INITTIME);

	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);

	m_edtRouteCount.SetLimitText(LIMIT_ROUTETEXT);
	m_edtWalkTime.SetLimitText(LIMIT_ROUTETEXT);

	KColorPickerSetting::DefaultCtrl(m_cpBusColor,      m_clrBus);
	KColorPickerSetting::DefaultCtrl(m_cpRailColor,     m_clrRail);
	KColorPickerSetting::DefaultCtrl(m_cpWalkColor,     m_clrWalk);
	
	InitLinkData();
	InitDefineCostList();
	InitComboLinkField();

	KReportCtrlSetting::DefaultC(m_ReportCtrlPath);
	KReportCtrlSetting::DefaultB(m_ReportCtrlRoute);
	InitRouteReportHeader();
	InitPathReportHeader();
	m_ReportCtrlRoute.Populate();
	m_ReportCtrlPath.Populate();

	InitializeNode();

	ResizeComponent();

	UpdateData(FALSE);

	m_pLinkTable->Register(this);
	m_backBrush.CreateSolidBrush(RGB(255,255,255));

	m_argFilePath = ImChampDir::GetAppDirectory() + _T("\\x64\\FTransitShortestPath.arg");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KShortestPathNTransitDlg::ResizeComponent()
{
	SetResize(IDC_EDIT1,				  SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC8,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC9,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC13,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_B,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_R,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_BR,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	
	SetResize(IDC_CHECK_ZONESTART,        SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT3,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON6,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_CHECK_ZONEEND,          SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT4,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON7,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_BUTTON1,				  SZ_TOP_RIGHT,  SZ_TOP_RIGHT);

	SetResize(IDC_SEPRATOR1,			  SZ_TOP_LEFT,   SZ_TOP_RIGHT);			// 가로 라인

	SetResize(IDC_REPORT1,				  SZ_TOP_LEFT,   SZ_TOP_RIGHT);
	SetResize(IDC_STATIC14,				  SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_STATIC15,				  SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_STATIC16,				  SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_CLR_BUS,				  SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_CLR_RAIL,		          SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_CLR_WALK,		          SZ_TOP_RIGHT,   SZ_TOP_RIGHT);

	SetResize(IDC_REPORT2,				  SZ_TOP_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_ALL,			  SZ_TOP_RIGHT,  SZ_TOP_RIGHT);

	SetResize(IDC_SEPRATOR2,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);			// 가로 라인

	SetResize(IDC_BUTTON5,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON4,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDC_EDIT2,                  SZ_TOP_LEFT,    SZ_TOP_LEFT);
}


void KShortestPathNTransitDlg::InitLinkData(void)
{

	KBulkDBase::BulkLinkSelect(m_pLinkTable, m_mapLinkDB);
}


void KShortestPathNTransitDlg::InitDefineCostList(void)
{
	ClearInterDefineCostPublicSectorVector();

	/*DB select해서 Class에 값 셋팅(default) 해서... vector로 담기... */
	std::vector<CString> tempPublicData;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		tempPublicData.push_back(_T("버스"));
		tempPublicData.push_back(_T("도시철도"));
	}
	else {
		tempPublicData.push_back(_T("Bus"));
		tempPublicData.push_back(_T("Rail"));
	}
	
	/*DB로 대체*/

	/*initialize Define Cost Public Sector*/
	int nCountPublicData = tempPublicData.size();
	for (int i = 0; i < nCountPublicData; ++i)
	{
		KInterDefineCostPublicSector* pInterDefineCostPublicSector = new KInterDefineCostPublicSector(i+1, tempPublicData[i]);
		pInterDefineCostPublicSector->SetBaseCharge(1050.0);
		pInterDefineCostPublicSector->SetBaseDistance(10.0);
		pInterDefineCostPublicSector->SetAddDistance(5.0);
		pInterDefineCostPublicSector->SetAddCharge(100.0);

		m_vecInterDefineCostPublicSector.push_back(pInterDefineCostPublicSector);
	}
}


void KShortestPathNTransitDlg::ClearInterDefineCostPublicSectorVector(void)
{
	int nCountPublicSectorVector = m_vecInterDefineCostPublicSector.size();
	for (int i = 0; i < nCountPublicSectorVector; ++i)
	{
		KInterDefineCostPublicSector* pInterDefineCostPublicSector = m_vecInterDefineCostPublicSector[i];
		delete pInterDefineCostPublicSector;
	}
	m_vecInterDefineCostPublicSector.clear();
}


void KShortestPathNTransitDlg::InitComboLinkField()
{
	m_cboUsingLinkField.ResetContent();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_LINK);
	const KIOColumns* pIOColumns = pIOTable->Columns();

	int nColCount = pIOColumns->ColumnCount();

	int nCur		= 0;
	int nInputCount = 0;
	int nPreSelectedIndex = -1;
	for (int i= 0; i< nColCount; i++)
	{
		KIOColumn*       pIOColumn      = pIOColumns->GetColumn(i);
		KEMIOColumnType  emColumnType   = pIOColumn->ColumnType();
		KEMIODataType    emDataType     = pIOColumn->DataType();

		if(KEMIOColumnTypeUserDefine == emColumnType && KEMIODataTypeDouble == emDataType)
		{
			CString strDisplayName    = pIOColumn->Caption();
			nCur   = m_cboUsingLinkField.AddString(strDisplayName);
			m_cboUsingLinkField.SetItemData( nCur, (DWORD_PTR)pIOColumn );

			if( m_strPreSelectedColumnName.CompareNoCase(pIOColumn->Name()) == 0)
			{
				nPreSelectedIndex = nCur;
			}
			nInputCount++;
		}
	}

	if(nInputCount > 0)
	{
		if (nPreSelectedIndex >= 0)
		{
			m_cboUsingLinkField.SetCurSel(nPreSelectedIndex);
		}
		else
		{
			m_cboUsingLinkField.SetCurSel(0);
		}
	}
}


void KShortestPathNTransitDlg::InitRouteReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_0_R_COLUMN_ROUTE_NAME,   _T("Name"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("경로 구분"));
    }

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_1_R_COLUMN_TOTAL_LENGTH,    _T("Length"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행거리"));
    }

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_2_R_COLUMN_TOTAL_COST,       _T("Cost"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행비용"));
    }

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_3_R_COLUMN_TOTAL_ALLTIME,      _T("All Time"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetVisible(FALSE);

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_4_R_COLUMN_TOTAL_TIME_VEH,      _T("Veh Time"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("차내통행시간"));
    }

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_5_R_COLUMN_TOTAL_TIME_WAIT,      _T("Wait Time"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("대기시간"));
    }

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_6_R_COLUMN_TOTAL_TIME_TRANS,      _T("Trans Time"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("환승시간"));
    }

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_7_R_COLUMN_TOTAL_TIME_ACCESSWALK,      _T("Accesswalk Time"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("출발접근시간"));
    }

	pColumn = m_ReportCtrlRoute.AddColumn(new CXTPReportColumn(_8_R_COLUMN_TOTAL_TIME_EGRESSWALK,      _T("Egresswalk Time"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도착접근시간"));
    }
}


void KShortestPathNTransitDlg::InitPathReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ReportCtrlPath.AddColumn(new CXTPReportColumn(_0_P_COLUMN_TRANSITID,       _T("Line ID[Name]"),    40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("노선ID[노선명]"));
    }

	pColumn = m_ReportCtrlPath.AddColumn(new CXTPReportColumn(_1_P_COLUMN_ROUTE,            _T("Route"),            30, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("구간"));
    }

	pColumn = m_ReportCtrlPath.AddColumn(new CXTPReportColumn(_2_P_COLUMN_PATHTYPE,        _T("Type"),          10));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("유형"));
    }

	pColumn = m_ReportCtrlPath.AddColumn(new CXTPReportColumn(_3_P_COLUMN_NODE_START,      _T("Start Station"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("구간시점"));
    }

	pColumn = m_ReportCtrlPath.AddColumn(new CXTPReportColumn(_4_P_COLUMN_NODE_END,        _T("End Station"),   20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("구간종점"));
    }
}


void KShortestPathNTransitDlg::InitializeNode()
{
	CWaitCursor wc;

	m_oStartNodeIdCaption.TID = _T("-1");
	m_oEndNodeIdCaption.TID   = _T("-1");
	SetDlgItemText(IDC_EDIT3, _T(""));	
	SetDlgItemText(IDC_EDIT4, _T(""));	

	m_mapBusAssign.clear();
	m_mapRailAssign.clear();
	m_mapBusRailAssign.clear();

	m_mapNodeData.clear();
	m_vecNodeIDCaption.clear();
	m_mapZoneNodeData.clear();
	m_vecZoneNodeIDCaption.clear();

	GetNodeInfo();
	GetZoneNodeInfo();

	m_chkStartZone.SetCheck(TRUE);
	m_chkEndZone.SetCheck(TRUE);

	m_ReportCtrlRoute.ResetContent();
	m_ReportCtrlPath.ResetContent();
	m_ReportCtrlRoute.Populate();
	m_ReportCtrlPath.Populate();
}


bool KShortestPathNTransitDlg::GetNodeInfo(void)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type FROM %s WHERE Node_Type != %d ORDER BY Node_Id "), TABLE_NODE, SUBSATION_CODE_KEY);
	
	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(spResult == NULL)
	{
		return false;
	}

	m_mapNodeData.clear();
	m_vecNodeIDCaption.clear();

	Integer nxNodeId		= 0;
	CString strNodeName		= _T("");
	int		nNodeType		= 0;
	CString strNodeID(_T(""));

	while( spResult->Next() )
	{
		nxNodeId	= spResult->GetValueInt64 (0);
		strNodeName = spResult->GetValueString(1);
		nNodeType	= spResult->GetValueInt   (2);

		if (SUBSATION_CODE_KEY == nNodeType) // 전철 정류장 노드 선택시 오류가 나는 알고리즘이라고 하여 이 노드는 선택 하지 못하도록 명지대 요청
		{
			continue;
		}

		m_mapNodeData.insert(std::make_pair(nxNodeId, strNodeName));

		KIDCaption oIDCaption;
		strNodeID.Format(_T("%I64d"), nxNodeId);
		oIDCaption.TID      = strNodeID;
		oIDCaption.TCaption = strNodeName;

		m_vecNodeIDCaption.push_back(oIDCaption);
	}

	TxLogDebugEndMsg();
	return true;
}


bool KShortestPathNTransitDlg::GetZoneNodeInfo(void)
{
	TxLogDebugStartMsg();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format (_T(" SELECT zone_id, name, full_name FROM %s ORDER BY zone_id "), TABLE_ZONE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(nullptr == spResult)
		{
			return false;
		}

		m_mapZoneNodeData.clear();
		m_vecZoneNodeIDCaption.clear();


		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));
		CString strNodeFullName(_T(""));

		while( spResult->Next() )
		{
			nxNodeId	    = spResult->GetValueInt64 (0);
			strNodeName     = spResult->GetValueString(1);
			strNodeFullName	= spResult->GetValueString(2);

			m_mapZoneNodeData.insert(std::make_pair(nxNodeId, strNodeName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxNodeId);
			oIDCaption.TID      = strNodeID;
			oIDCaption.TCaption = strNodeName;

			m_vecZoneNodeIDCaption.push_back(oIDCaption);
		}
	}
	catch (...)
	{
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


void KShortestPathNTransitDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KShortestPathNTransitDlg::SetNodeViewData(const Integer &a_nxNodeID, CString &a_strNodeName, CString &a_strNodeViewData)
{
	ConvertEmptyNodeName(a_strNodeName);

	a_strNodeViewData.Format(_T("%I64d [%s]"), a_nxNodeID, a_strNodeName);
}


void KShortestPathNTransitDlg::ZoomToNodeFeature( Integer &a_nxNodeID )
{
	KMapView* pMapView = GetMapView(m_pTarget);
	if(NULL != pMapView)
	{
		pMapView->PanToFeature(KLayerID::Node(), a_nxNodeID );
	}
}


void KShortestPathNTransitDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampIDFind::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				Integer nxOrgZoneID = m_spImTasIDFind->GetSelectedID();
				CString strID(_T(""));
				strID.Format(_T("%I64d"), nxOrgZoneID);

				CString    strNodeView(_T(""));
				KIDCaption oIDCaption;
				if (TRUE == m_bZone)
				{
					SetNodeViewData(m_mapZoneNodeData, strID, strNodeView, oIDCaption);
				}
				else
				{
					SetNodeViewData(m_mapNodeData,     strID, strNodeView, oIDCaption);
				}

				if (MAP_NODE_START == m_bSelectMapNode)
				{
					m_oStartNodeIdCaption = oIDCaption;
					SetDlgItemText(IDC_EDIT3,  strNodeView);
				}
				else
				{
					m_oEndNodeIdCaption = oIDCaption;
					SetDlgItemText(IDC_EDIT4,  strNodeView);
				}

				m_ReportCtrlRoute.ResetContent(TRUE);
				m_ReportCtrlRoute.Populate();
				m_ReportCtrlPath.ResetContent(TRUE);
				m_ReportCtrlPath.Populate();

				AllClearShortestPath(false);
				std::vector<TDrawNodeInfo> vecEmptyNode;
				DrawOrgDestNode(vecEmptyNode, false);

				m_pMapView->PanToFeature(KLayerID::Node(),nxOrgZoneID, true);
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KShortestPathNTransitDlg::DrawOrgDestNode(std::vector<TDrawNodeInfo> &a_vecNode, bool a_bRefresh)
{
	std::vector<TDrawZoneAccess> vecZoneAccess;
	
	// 기점 그리기 (존이 아닐경우 아닌것처럼 보이기 위한 Radius를 1로 준다.)
	{
		CString strID = m_oStartNodeIdCaption.TID;
		Integer nxID  = _ttoi64(strID);

		if (m_mapNodeData.find(nxID) != m_mapNodeData.end()) {
			// Zone 여부
			bool bZone(false);
			if (m_mapZoneNodeData.find(nxID) != m_mapZoneNodeData.end()) {
				bZone = true;
			}

			CString strNodeLable = strID;
			strNodeLable.Format(_T("%I64d"), strID);

			TDrawNodeInfo   oDrawNodeInfo;
			oDrawNodeInfo.TNodeID   = nxID;
			oDrawNodeInfo.TSymbol   = 48;
			oDrawNodeInfo.TColorRGB = RGB(255,0,0);
			oDrawNodeInfo.TSize     = 18.0;
			
			TDrawZoneAccess oDrawZoneAccess;
			oDrawZoneAccess.TNodeID         = oDrawNodeInfo.TNodeID;
			oDrawZoneAccess.TSymbol			= oDrawNodeInfo.TSymbol;//임시 모양
			oDrawZoneAccess.TColorRGB       = oDrawNodeInfo.TColorRGB;
			oDrawZoneAccess.TLabel          = strNodeLable;
			oDrawZoneAccess.TSize           = oDrawNodeInfo.TSize; //임시 사이즈
			oDrawZoneAccess.TRadius         = 1000.0;
			if (bZone) {
				vecZoneAccess.push_back(oDrawZoneAccess);
			}
			a_vecNode.push_back(oDrawNodeInfo);
		}
	}
	// 종점 그리기 (존이 아닐경우 아닌것처럼 보이기 위한 Radius를 1로 준다.)
	{
		CString strID = m_oEndNodeIdCaption.TID;
		Integer nxID  = _ttoi64(strID);

		if (m_mapNodeData.find(nxID) != m_mapNodeData.end()) {
			// Zone 여부
			bool bZone(false);
			if (m_mapZoneNodeData.find(nxID) != m_mapZoneNodeData.end()) {
				bZone = true;
			}

			CString strNodeLable = strID;
			strNodeLable.Format(_T("%I64d"), strID);

			TDrawNodeInfo   oDrawNodeInfo;
			oDrawNodeInfo.TNodeID   = nxID;
			oDrawNodeInfo.TSymbol   = 48;
			oDrawNodeInfo.TColorRGB = RGB(0,0,205);
			oDrawNodeInfo.TSize     = 18.0;

			TDrawZoneAccess oDrawZoneAccess;
			oDrawZoneAccess.TNodeID         = oDrawNodeInfo.TNodeID;
			oDrawZoneAccess.TSymbol			= oDrawNodeInfo.TSymbol;//임시 모양
			oDrawZoneAccess.TColorRGB       = oDrawNodeInfo.TColorRGB;
			oDrawZoneAccess.TLabel          = strNodeLable;
			oDrawZoneAccess.TSize           = oDrawNodeInfo.TSize; //임시 사이즈
			oDrawZoneAccess.TRadius         = 1000.0;
			if (bZone) {
				vecZoneAccess.push_back(oDrawZoneAccess);
			}
			a_vecNode.push_back(oDrawNodeInfo);
		}
	}

	m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, false);
	m_pMapView->TxDrawCompositeNode(a_vecNode, false, false);

	if (a_bRefresh) {
		m_pMapView->MapRefresh();
	}
}


void KShortestPathNTransitDlg::SetNodeViewData(KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
{
	try
	{
		Integer nxZoneID = _ttoi64(a_strZoneID);

		AutoType iter  = a_mapNodeData.find(nxZoneID);
		AutoType itEnd = a_mapNodeData.end();

		CString strName(_T(""));
		if (iter != itEnd)
		{
			strName = iter->second;
		}

		a_oIDCaption.TID      = a_strZoneID;
		a_oIDCaption.TCaption = strName;

		ConvertEmptyNodeName(strName);

		a_strNodeViewData.Format(_T("%s [%s]"), a_strZoneID, strName);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KShortestPathNTransitDlg::OnBnClickedBtnSearch()
{
	UpdateData(TRUE);
	m_nResultCode = DLL_UNEXPECTED_ERROR;

	m_nxStartNodeID = GetSelStartNodeId();
	if(-1 == m_nxStartNodeID)	
	{
		AfxMessageBox(_T("기점을 선택해 주세요."));
		return;
	}

	m_nxEndNodeID = GetSelEndNodeId();
	if(-1 == m_nxEndNodeID)	
	{
		AfxMessageBox(_T("종점을 선택해 주세요."));
		return;
	}

	if(m_nRouteCount == 0)
	{
		AfxMessageBox(_T("노선의 수를 1개이상 입력 하세요"));
		m_edtRouteCount.SetFocus();
		return;
	}

	// link column 수집
	m_oUseLinkColumns.Clear();
	
	KIOTables*   pIOTables  = m_pTarget->Tables();
	KIOTable*    pIOTable   = pIOTables->FindTable(TABLE_LINK);
	const KIOColumns* pIOColumns = pIOTable->Columns();

	int nColCount = pIOColumns->ColumnCount();
	for (int i= 0; i< nColCount; i++)
	{
		KIOColumn*       pIOColumn      = pIOColumns->GetColumn(i);
		CString          strColumnName  = pIOColumn->Name();

		if(strColumnName.CompareNoCase(COLUMN_LINK_SPEED) == 0)
		{
			m_oUseLinkColumns.AddNewColumn(pIOColumn);
		}
	}

	if( IDC_RADIO_INITTIME == GetCheckedRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD) )
	{
		m_nTimeMethod     = KEMTimeMethodInitTime;
	}
	else
	{
		m_nTimeMethod     = KEMTimeMethodUsingField;

		int nCurSel         = m_cboUsingLinkField.GetCurSel();
		if(nCurSel < 0)
		{
			AfxMessageBox(_T("링크통행시간 관련 컬럼을 선택해 주세요."));
			m_cboUsingLinkField.SetFocus();
			return;
		}

		KIOColumn*       pIOColumn = (KIOColumn*)m_cboUsingLinkField.GetItemData(nCurSel);
		m_oUseLinkColumns.AddNewColumn(pIOColumn);
	}

	KDBaseDefineTransitCost::GetTransitModeCostValue2DB( m_pTarget,  m_vecTransitCost);

	//AllClearShortestPath(true);
	m_ReportCtrlRoute.ResetContent(TRUE);
	m_ReportCtrlRoute.Populate();
	m_ReportCtrlPath.ResetContent(TRUE);
	m_ReportCtrlPath.Populate();

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(ShortestPathThreadCaller, this, false, nLang);
	bool bSuccess(false);
	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		/*AfxMessageBox(_T("성공적으로 완료되었습니다."));*/
		if (m_mapBusAssign.size() < 1 && m_mapRailAssign.size() < 1 && m_mapBusRailAssign.size() < 1) {
			AfxMessageBox(_T("경로 탐색 결과가 존재하지 않습니다."));
		}
		else {
			bSuccess = true;
		}
	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		AfxMessageBox(_T("모형 실행을 중단하였습니다."));
		m_mapBusAssign.clear();
		m_mapRailAssign.clear();
		m_mapBusRailAssign.clear();
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("Not Found Dll"));
		m_mapBusAssign.clear();
		m_mapRailAssign.clear();
		m_mapBusRailAssign.clear();
	}
	else if (DLL_CREATE_ARGFILE_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
		m_mapBusAssign.clear();
		m_mapRailAssign.clear();
		m_mapBusRailAssign.clear();
	}
	else if (DLL_READ_OUTPUTFILE_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("수행 결과 파일 정보 입력에 실패하였습니다."));
		m_mapBusAssign.clear();
		m_mapRailAssign.clear();
		m_mapBusRailAssign.clear();
	}
	else
	{
		AfxMessageBox(_T("경로탐색 중 오류가 발생하였습니다."));
		m_mapBusAssign.clear();
		m_mapRailAssign.clear();
		m_mapBusRailAssign.clear();
	}
	
	if (bSuccess) {
		// MakeDrawData

		QBicSimpleProgressThread::ExecuteThread(MakeDrawDataThreadCaller, this, false, nLang);

		CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
		UpdateRouteReportData();
	}
}


unsigned __stdcall KShortestPathNTransitDlg::ShortestPathThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("대중교통 최단 경로 탐색 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Transit Path Find Module..."));
	}

	KShortestPathNTransitDlg* pDlg = (KShortestPathNTransitDlg*)pParameter->GetParameter();
	pDlg->GetSearchShortestPath();

	return 0;
}


void KShortestPathNTransitDlg::GetSearchShortestPath()
{
	TxLogDebugStartMsg();

	if(! CreateArgument(m_oUserColumnsLink))
	{
		m_nResultCode = DLL_CREATE_ARGFILE_ERROR;
		return;
	}

	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		m_nResultCode = DLL_USER_STOPPING;
		return;
	}

	CString strWindowName(_T(""));
	GetWindowText(strWindowName);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("최단 경로 탐색(대중교통) 모듈 실행 중..."));
	}
	else {
		AddStatusMessage(_T("Run Path Find(Transit) Module..."));
	}

	//KLauncher::Run(strWindowName, 8);
	KLauncher::Run64(strWindowName, 8);

	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		m_nResultCode = DLL_USER_STOPPING;
		return;
	}

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("최단 경로 탐색(대중교통) 모듈 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Path Find(Transit) Module Complete."));
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

		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Shortest Path Dll Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));

		return;
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

		return;
	}
	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		m_nResultCode =  DLL_USER_STOPPING;
		return;
	}
	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Saving Output Data..."));
	}
	if (! ReadOutPutFile() )
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data Failed."));
		}
		TxLogDebug(_T("Output Path 정보 입력 실패..."));
		m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
		return;
	}

	TxLogDebugEndMsg();
}


void KShortestPathNTransitDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KShortestPathNTransitDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


// void KShortestPathNTransitDlg::Pow(sqlite3_context* pContext, int argc, sqlite3_value **argv)
// {
// 	for(int i= 0; i< argc; i++)
// 	{
// 		if(sqlite3_value_type(argv[i]) == SQLITE_NULL)
// 			return;
// 	}
// 
// 	double dValue1 = sqlite3_value_double(argv[0]);
// 	int    nValue2 = sqlite3_value_int(argv[1]);
// 
// 	double dPowValue = 1.0;
// 
// 	for(int i= 0; i< nValue2; i++)
// 	{
// 		dPowValue *= dValue1;
// 	}
// 
// 	sqlite3_result_double(pContext, dPowValue);
// }


bool KShortestPathNTransitDlg::IsNearbyStation( Integer &a_nxNodeID )
{
	TxLogDebugStartMsg();

// 	try
// 	{
// 		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
// 
// 		CString strSQL = _T("Select ozone_id, dzone_id ");
// 
// 		for (int i=0; i<nColumnCount; i++)
// 		{
// 			strSQL = strSQL + _T(",") + columnCollection.GetColumn(i)->Name();
// 		}
// 
// 		strSQL = strSQL + _T(" From ") + strTableName;
// 
// 		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
// 
// 		while(spResult->Next())
// 		{
// 			Integer nOZoneID = spResult->GetValueInt64(0);
// 			Integer nDZoneID = spResult->GetValueInt64(1);
// 		}
// 	}
// 	catch (...)
// 	{
// 		TxLogDebugException();
// 		return false;
// 	}

	TxLogDebugEndMsg();
	return true;
}


bool KShortestPathNTransitDlg::CreateArgument( KIOColumns &a_oUserColumnsLink)
{
	TxLogDebugStartMsg();

	try
	{
		ImChampDir::SetCurrentDirectoryToApp();
		if(! ClearFiles())
		{
			return false;
		}
		
		CString strDirectory;
		strDirectory.Format(_T("%s\\%s\\"),m_pTarget->GetLocation(),DLL_MODEL_FOLDER_NAME);
		CreateDirectory(strDirectory, NULL);

		CString strResultPath = ConvertSavePath(TRANST_SHORTEST_RESULT_PATH);
		DeleteAFolderFile(strResultPath);//결과 파일 삭제
		CreateDirectory(strResultPath, NULL);

		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget);

		//TxLogDebug("END CLEAR FILE ------ ");
		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(m_argFilePath, nOpenMode);

		bool bResult = false;

		// .arg 작성
		bResult = WriteShortestPathArgument(of);
		if(! bResult)
		{
			of.Close();
			AfxMessageBox(_T("Argument 생성(WriteShortestPathArgument) 중 오류가 발생하였습니다. !!!"));
			return false;
		}

		of.Close();

		//TxLogDebug("END ARG CREAETE FILE ------ ");
		// 링크파일 생성
		if ( !KBulkDBase2File::ExportDB2LinkFileForShortestPathTransit(m_pTarget->GetDBaseConnection(), m_oUseLinkColumns, ConvertSavePath(T_DAT_LINK) ))
		{
			AfxMessageBox(_T("Dat 생성(ExportDB2LinkFile) 중 오류가 발생하였습니다. !!!"));
			return false;
		}

		//TxLogDebug("END LINK CREAETE FILE ------ ");
		// 노드파일 생성
		if ( !KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_NODE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		}
		//TxLogDebug("END NODE CREAETE FILE ------ ");

		//TxLogDebug("START turn CREAETE FILE ------ ");
		// 회전정보 파일 생성
		bResult = KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_TURN));

		if(! bResult)
		{
			AfxMessageBox(_T("Dat 생성(ExportDB2TurnFile) 중 오류가 발생하였습니다. !!!"));
			return false;
		}
		//TxLogDebug("end turn CREAETE FILE ------ ");

		//TxLogDebug("START line CREAETE FILE ------ ");
		// transit line정보 파일 생성
		bResult = KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_LINEINFO), ConvertSavePath(T_DAT_LINE));
		if (!bResult)
		{
			AfxMessageBox(_T("Binary 생성(ExportDB2LineFile) 중 오류가 발생하였습니다. !!!"));
			return false;
		}
		//TxLogDebug("end line CREAETE FILE ------ ");
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KShortestPathNTransitDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();

	CString strDataFileName(_T(""));

	try
	{
		strDataFileName.Format(_T("%sR_BusPath.dat"),     TRANST_SHORTEST_RESULT_PATH);
		ReadBusPathFile(ConvertSavePath(strDataFileName));

		strDataFileName.Format(_T("%sR_RailPath.dat"),    TRANST_SHORTEST_RESULT_PATH);
		ReadRailPathFile(ConvertSavePath(strDataFileName));

		strDataFileName.Format(_T("%sR_BusRailPath.dat"), TRANST_SHORTEST_RESULT_PATH);
		ReadBusRailPathFile(ConvertSavePath(strDataFileName));
		/*m_vecRouteInfo.clear();
		MakeRouteInfo(ifs, nTotalPathCount);*/
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}


void KShortestPathNTransitDlg::ReadBusPathFile(CString strFileName)
{
	TxLogDebugStartMsg();

	m_mapBusAssign.clear();
	std::ifstream ifs( strFileName, std::ios::binary );    

	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				KMBusAssignInfo oBusAssignInfo;
				oBusAssignInfo.emTransitAssignMode = KEMTransitAssignBus;
#pragma region 정보 취합
				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nxOrginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nxDestinationID)),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nPathID)),                sizeof(int) );
				if(ifs.eof())break;



				std::vector< TIndexResultTransitInfo > &vecResultBusInfo = oBusAssignInfo.vecResultBusInfo;
				for (int i= 0; i< BUSPATH_ASSIGNSET_COUNT; i++)
				{
					TIndexResultTransitInfo oTIndexResultTransitInfo;
					oTIndexResultTransitInfo.oIndexKey = KODKey((i+1),0);
					std::vector<TMResultBusInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
					for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
					{
						TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeBus, m_cpBusColor.GetColor(), false};

						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );

						if(0 == oTMResultBusInfo.nxTransitID)
							continue;

						vecLowResultBusInfo.push_back(oTMResultBusInfo);
					}
					vecResultBusInfo.push_back(oTIndexResultTransitInfo);
				}

				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.oTPathBusSubData)),               sizeof(TShortPathBusSubData) );
#pragma endregion 정보 취합

#pragma region 취합 정보 등록
				AutoType iter  = m_mapBusAssign.find(oBusAssignInfo.nxDestinationID);
				AutoType itEnd = m_mapBusAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oBusAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oBusAssignInfo);
					m_mapBusAssign.insert(std::make_pair(oBusAssignInfo.nxDestinationID, vecBusAssignInfo));
				}
#pragma endregion 취합 정보 등록

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
		m_mapBusAssign.clear();
		ifs.close();
		/*ThrowException(_T("ReadBusPathFile 오류"));*/
		return;
	}    
	TxLogDebugEndMsg();
}


void KShortestPathNTransitDlg::ReadRailPathFile(CString strFileName)
{
	TxLogDebugStartMsg();

	m_mapRailAssign.clear();
	std::ifstream ifs( strFileName, std::ios::binary );    

	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				KMBusAssignInfo oRailAssignInfo;
				oRailAssignInfo.emTransitAssignMode = KEMTransitAssignRail;
#pragma region 정보 취합
				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nxOrginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nxDestinationID)),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nPathID)),                sizeof(int) );
				if(ifs.eof())break;

				int nBoardingCnt(0); //탑승횟수
				ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

				if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				std::vector< TIndexResultTransitInfo > &vecResultBusInfo = oRailAssignInfo.vecResultBusInfo;
				for (int i= 0; i< nBoardingCnt; i++)
				{
					int nDuplicateCnt(0); //중복 노선 갯수
					ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

					if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					TIndexResultTransitInfo oTIndexResultTransitInfo;
					oTIndexResultTransitInfo.oIndexKey = KODKey((i+1),0);
					std::vector<TMResultBusInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
					for (int k= 0; k< nDuplicateCnt; k++)
					{
						TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeRail,  m_cpRailColor.GetColor(), false};

						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );

						if(0 == oTMResultBusInfo.nxTransitID)
							continue;

						vecLowResultBusInfo.push_back(oTMResultBusInfo);
					}
					vecResultBusInfo.push_back(oTIndexResultTransitInfo);
				}

				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.oTPathRailSubData)),             sizeof(TShortPathRailSubData) );
#pragma endregion 정보 취합

#pragma region 취합 정보 등록
				AutoType iter  = m_mapRailAssign.find(oRailAssignInfo.nxDestinationID);
				AutoType itEnd = m_mapRailAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oRailAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oRailAssignInfo);
					m_mapRailAssign.insert(std::make_pair(oRailAssignInfo.nxDestinationID, vecBusAssignInfo));
				}
#pragma endregion 취합 정보 등록

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
		m_mapRailAssign.clear();
		ifs.close();
		/*ThrowException(_T("ReadRailPathFile 오류"));*/
		return;
	}    
	TxLogDebugEndMsg();
}


void KShortestPathNTransitDlg::ReadBusRailPathFile(CString strFileName)
{
	TxLogDebugStartMsg();

	m_mapBusRailAssign.clear();

	std::ifstream ifs( strFileName, std::ios::binary );    

	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				KMBusAssignInfo oBusRailAssignInfo;
				oBusRailAssignInfo.emTransitAssignMode = KEMTransitAssignBusRail;
				int nRailIndex = -1;

#pragma region 버스 정보 취합
				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxOrginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxDestinationID)),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
				if(ifs.eof())break;

				std::vector< TIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResultBusInfo;
				for (int i= 0; i< BUSRAILPATH_ASSIGNSET_COUNT; i++)
				{
					TIndexResultTransitInfo oTIndexResultTransitInfo;
					std::vector<TMResultBusInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
					for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
					{
						TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeBus,  m_cpBusColor.GetColor(), false};

						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );

						if(0 == oTMResultBusInfo.nxTransitID)
							continue;

						vecLowResultBusInfo.push_back(oTMResultBusInfo);

						if(oTMResultBusInfo.nxTransitID < 0) // 음수는 rail 이라고 판단한다 고 함
						{
							nRailIndex = i;
						}
					}
					vecResultBusInfo.push_back(oTIndexResultTransitInfo);
				}

				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTPathBusSubData)),           sizeof(TShortPathBusSubData) );
#pragma endregion 버스 정보 취합

				std::vector< std::vector< TIndexResultTransitInfo > > vecMultiResultRailInfo;
				if (nRailIndex != -1)
				{
#pragma region 레일 정보 취합
					int nRailCount(0);
					ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );

					if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					for (int m= 0; m< nRailCount; m++)
					{
						Integer nxNoneUseOriginID;
						Integer nxNoneUseDestinationID;
						int     nNoneUsePathID;

						ifs.read( reinterpret_cast<char*>( &(nxNoneUseOriginID)),                      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(nxNoneUseDestinationID)),                 sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );

						int nBoardingCnt(0); //탑승횟수
						ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

						if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						std::vector< TIndexResultTransitInfo > vecResultRailInfo;
						for (int i= 0; i< nBoardingCnt; i++)
						{
							int nDuplicateCnt(0); //중복 노선 갯수
							ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

							if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							TIndexResultTransitInfo oTIndexResultTransitInfo;
							std::vector<TMResultBusInfo> &vecLowResultRailInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
							for (int k= 0; k< nDuplicateCnt; k++)
							{
								TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeRail,  m_cpRailColor.GetColor(), false};

								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );

								if(0 == oTMResultBusInfo.nxTransitID)
									continue;

								if(m > 0 && 0 == i)
								{
									oTMResultBusInfo.bDuplicationRail = true;
								}

								vecLowResultRailInfo.push_back(oTMResultBusInfo);
							}
							vecResultRailInfo.push_back(oTIndexResultTransitInfo);
						}

						vecMultiResultRailInfo.push_back(vecResultRailInfo);
					}
#pragma endregion 레일 정보 취합
				}

				std::vector< TIndexResultTransitInfo > &vecResultTotalBusInfo = oBusRailAssignInfo.vecResultBusInfo;
				Integer nSingleIdx = 0;
				for (size_t i= 0; i< vecResultBusInfo.size(); i++)
				{
					if(i != nRailIndex)
					{
						TIndexResultTransitInfo &oTIndexResultTransitInfo = vecResultBusInfo[i];
						oTIndexResultTransitInfo.oIndexKey = KODKey( ((nSingleIdx++)+1), 0 );
						vecResultTotalBusInfo.push_back(oTIndexResultTransitInfo);
					}
					else
					{
						size_t nxSize = vecMultiResultRailInfo.size();
						for (size_t m= 0; m< nxSize; m++)
						{
							std::vector< TIndexResultTransitInfo > &vecTempResultRailInfo = vecMultiResultRailInfo[m];

							for (size_t k= 0; k< vecTempResultRailInfo.size(); k++)
							{
								TIndexResultTransitInfo &oTIndexResultTransitInfo = vecTempResultRailInfo[k];

								nSingleIdx = (i+k+1);
								if (1 == nxSize)
								{
									oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, 0);
								}
								else
								{
									oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, (m+1));
								}

								vecResultTotalBusInfo.push_back(vecTempResultRailInfo[k]);
							}
						}
					}
				}

#pragma region 취합 정보 등록
				AutoType iter  = m_mapBusRailAssign.find(oBusRailAssignInfo.nxDestinationID);
				AutoType itEnd = m_mapBusRailAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oBusRailAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oBusRailAssignInfo);
					m_mapBusRailAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusAssignInfo));
				}
#pragma endregion 취합 정보 등록

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
		m_mapBusRailAssign.clear();
		ifs.close();
		/*ThrowException(_T("ReadRailPathFile 오류"));*/
		return;
	}    
	TxLogDebugEndMsg();
}


void KShortestPathNTransitDlg::MakeRouteInfo( std::ifstream& ifs, int &a_nTotalPathCount )
{
	TTransitRouteInfo oTTransitRouteInfo;
	
	//  part 1. dll 정보 read

	//-Path count
	int nPathCount = 0;
	ifs.read( reinterpret_cast<char*>( &nPathCount),  sizeof(int) );

	//-TransitRoute
	ifs.read( reinterpret_cast<char*>( &(oTTransitRouteInfo.oTRouteCost) ),  sizeof(TTransitRouteCostData) );

	//-TransitPath
	Integer nxPrevePathEndNodeID   = 0;

	KMunsellColor* pMunSellColor = new KMunsellColor( (nPathCount/10), true);
	COLORREF colorRef;
	for(int k= 0; k< nPathCount; k++)
	{
		// input base
		TTransitPathInfo  oTTransitPathInfo;
		ifs.read( reinterpret_cast<char*>( &(oTTransitPathInfo.oTPathBase)),    sizeof(TTransitPathBase) );

		// input main
		colorRef = pMunSellColor->GetColor(a_nTotalPathCount);
		MakePathInfo(oTTransitPathInfo, colorRef);

//################# start jyk : 추후 dll 로 부터 도보 정보 받아 올시 해당 구문 삭제 바람 ################# 
		// check the Route's first_path 
		if(k == 0)
		{
			// 검색 시작 노드와 첫번째 path의 노드가 다르다면 '도보' 가 발생 했다는 판단하에.  '도보'정보 생성
			if(m_nxStartNodeID != oTTransitPathInfo.nxNodeId_PathStart)
			{
				TTransitPathInfo  oTTransitStartPathInfo;

				oTTransitStartPathInfo.oTPathBase.nxTransitId = -1;
				oTTransitStartPathInfo.oTPathBase.nxSeqStart  = m_nxStartNodeID;
				oTTransitStartPathInfo.oTPathBase.nxSeqEnd    = oTTransitPathInfo.nxNodeId_PathStart;
				oTTransitStartPathInfo.oTPathBase.dLength     = 0.0;
				oTTransitStartPathInfo.oTPathBase.dTime		  = 0.0;
				oTTransitStartPathInfo.oTPathBase.dCost       = 0.0;

				oTTransitStartPathInfo.nTransitType           = ShortestPathInfo::TRANSIT_TYPE_WALK;
				oTTransitStartPathInfo.nxNodeId_PathStart     = m_nxStartNodeID;
				oTTransitStartPathInfo.nxNodeId_PathEnd       = oTTransitPathInfo.nxNodeId_PathStart;
				oTTransitStartPathInfo.color                  = TRANSIT_LINECOLOR_WALK;

				oTTransitRouteInfo.vecPathInfo.push_back(oTTransitStartPathInfo);
			}
		}

		// check the walk section 
		if( k >0 && k< (nPathCount -1) )
		{
			// 이전 path의 종료 노드와 현재 path의 시작 노드가 다르다면 '도보' 가 발생 했다는 판단하에.  '도보'정보 생성 
			if( nxPrevePathEndNodeID != oTTransitPathInfo.nxNodeId_PathStart)
			{
				TTransitPathInfo  oTTransitWalkPathInfo;

				oTTransitWalkPathInfo.oTPathBase.nxTransitId = -1;
				oTTransitWalkPathInfo.oTPathBase.nxSeqStart  = nxPrevePathEndNodeID;
				oTTransitWalkPathInfo.oTPathBase.nxSeqEnd    = oTTransitPathInfo.nxNodeId_PathStart;
				oTTransitWalkPathInfo.oTPathBase.dLength     = 0.0;
				oTTransitWalkPathInfo.oTPathBase.dTime		 = 0.0;
				oTTransitWalkPathInfo.oTPathBase.dCost       = 0.0;

				oTTransitWalkPathInfo.nTransitType           = ShortestPathInfo::TRANSIT_TYPE_WALK;
				oTTransitWalkPathInfo.nxNodeId_PathStart     = nxPrevePathEndNodeID;
				oTTransitWalkPathInfo.nxNodeId_PathEnd       = oTTransitPathInfo.nxNodeId_PathStart;
				oTTransitWalkPathInfo.color                  = TRANSIT_LINECOLOR_WALK;

				oTTransitRouteInfo.vecPathInfo.push_back(oTTransitWalkPathInfo);
			}
		}
//################# end jyk : 추후 dll 로 부터 도보 정보 받아 올시 해당 구문 삭제 바람 ################# 

		oTTransitRouteInfo.vecPathInfo.push_back(oTTransitPathInfo);

//################# start jyk : 추후 dll 로 부터 도보 정보 받아 올시 해당 구문 삭제 바람 ################# 
		nxPrevePathEndNodeID = oTTransitPathInfo.nxNodeId_PathEnd;

		// check the Route's last_path 
		if( k == (nPathCount -1) )
		{
			// 검색 종료 노드와 마지막 path의 노드가 다르다면 '도보' 가 발생 했다는 판단하에.  '도보'정보 생성
			if(m_nxEndNodeID != oTTransitPathInfo.nxNodeId_PathEnd)
			{
				TTransitPathInfo  oTTransitEndPathInfo;

				oTTransitEndPathInfo.oTPathBase.nxTransitId = -1;
				oTTransitEndPathInfo.oTPathBase.nxSeqStart  = oTTransitPathInfo.nxNodeId_PathEnd;
				oTTransitEndPathInfo.oTPathBase.nxSeqEnd    = m_nxEndNodeID;
				oTTransitEndPathInfo.oTPathBase.dLength     = 0.0;
				oTTransitEndPathInfo.oTPathBase.dTime       = 0.0;
				oTTransitEndPathInfo.oTPathBase.dCost       = 0.0;

				oTTransitEndPathInfo.nTransitType           = ShortestPathInfo::TRANSIT_TYPE_WALK;
				oTTransitEndPathInfo.nxNodeId_PathStart     = oTTransitPathInfo.nxNodeId_PathEnd;
				oTTransitEndPathInfo.nxNodeId_PathEnd       = m_nxEndNodeID;
				oTTransitEndPathInfo.color                  = TRANSIT_LINECOLOR_WALK;

				oTTransitRouteInfo.vecPathInfo.push_back(oTTransitEndPathInfo);
			}
		}
//################# end jyk : 추후 dll 로 부터 도보 정보 받아 올시 해당 구문 삭제 바람 ################# 
		a_nTotalPathCount++;
		if(a_nTotalPathCount >1000)
		{
			a_nTotalPathCount = 0;
		}
	}
	if(nPathCount == 0)
	{
		TTransitPathInfo  oTTransitPathInfo;
		ifs.read( reinterpret_cast<char*>( &(oTTransitPathInfo.oTPathBase)),    sizeof(TTransitPathBase) );
	}

	//  part 2. 부가 정보 input
	oTTransitRouteInfo.nxNodeIdStart = m_nxStartNodeID;
	oTTransitRouteInfo.nxNodeIdEnd   = m_nxEndNodeID;

	m_vecRouteInfo.push_back(oTTransitRouteInfo);
	delete pMunSellColor;
}


void KShortestPathNTransitDlg::MakePathInfo(TTransitPathInfo &a_oTTransitPathInfo, COLORREF &a_colorRef)
{
	Integer nxTransitID = a_oTTransitPathInfo.oTPathBase.nxTransitId;

	// path type - Walk 
	if(nxTransitID < 0)
	{
		a_oTTransitPathInfo.nxNodeId_PathStart = a_oTTransitPathInfo.oTPathBase.nxSeqStart;
		a_oTTransitPathInfo.nxNodeId_PathEnd   = a_oTTransitPathInfo.oTPathBase.nxSeqEnd;

		a_oTTransitPathInfo.color = TRANSIT_LINECOLOR_WALK;
		a_oTTransitPathInfo.nTransitType = ShortestPathInfo::TRANSIT_TYPE_WALK;

		return;
	}

	// path type - bus or rail
	SelectTransitLinkVector(a_oTTransitPathInfo);

	TFixedLinkDB oTLinkDB;

	//-jyk 모듈 구동 상에 transit데이터가 없다면 모듈에서 transit_id 에  0이라는 값을 넘겨주고, start_seq와 end_seq 에 path의 시작 노드 , 종료 노드를 넘겨 준다. 이 케이스에 대한 처리
	if( a_oTTransitPathInfo.oTPathBase.nxTransitId == 0 && a_oTTransitPathInfo.vecLink.size() < 1 )
	{
		a_oTTransitPathInfo.nxNodeId_PathStart = a_oTTransitPathInfo.oTPathBase.nxSeqStart;
		a_oTTransitPathInfo.nxNodeId_PathEnd = a_oTTransitPathInfo.oTPathBase.nxSeqEnd;
	}
	else
	{
		Integer nxFirstLinkID  = a_oTTransitPathInfo.vecLink.front();
		Integer nxLastLinkID   = a_oTTransitPathInfo.vecLink.back();

		std::map<Integer, TFixedLinkDB>::iterator iter;
		iter =  m_mapLinkDB.find(nxFirstLinkID);
		if(iter != m_mapLinkDB.end())
		{
			oTLinkDB = iter->second;
			a_oTTransitPathInfo.nxNodeId_PathStart = oTLinkDB.fnode_id;
		}

		iter =  m_mapLinkDB.find(nxLastLinkID);
		if(iter != m_mapLinkDB.end())
		{
			oTLinkDB = iter->second;
			a_oTTransitPathInfo.nxNodeId_PathEnd = oTLinkDB.tnode_id;
		}
	}
		
	a_oTTransitPathInfo.color = a_colorRef;
	a_oTTransitPathInfo.nTransitType = ShortestPathInfo::TRANSIT_TYPE_BUS;
}


bool KShortestPathNTransitDlg::SelectTransitLinkVector( TTransitPathInfo &a_oTTransitPathInfo )
{
	TxLogDebugStartMsg();
	a_oTTransitPathInfo.vecLink.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL = _T("");
	strSQL.Format(_T(" Select Link_ID, Seq ")
				  _T(" From Transit_LInks ")
				  _T(" Where Transit_ID = %I64d ")
				  _T(" And Seq >= %I64d And Seq < %I64d ")
				  _T(" Order By Seq "), a_oTTransitPathInfo.oTPathBase.nxTransitId, a_oTTransitPathInfo.oTPathBase.nxSeqStart, a_oTTransitPathInfo.oTPathBase.nxSeqEnd);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	while (spResult->Next())
	{
		Integer nxlinkID     = spResult->GetValueInt64(0);
		//spResult->GetValueInt(1);
	
		a_oTTransitPathInfo.vecLink.push_back(nxlinkID);
	}

	return true;
}


bool KShortestPathNTransitDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove((CStringA)m_argFilePath);
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TURN));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINEINFO));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINE));
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


void KShortestPathNTransitDlg::DeleteAFolderFile( CString a_strLocation )
{
	CFileFind finder;
	BOOL bContinue = TRUE;

	if(a_strLocation.Right(1) != _T("\\"))
		a_strLocation += _T("\\");

	a_strLocation += _T("*.*");
	bContinue = finder.FindFile(a_strLocation);
	while(bContinue)
	{
		bContinue = finder.FindNextFile();
		if(finder.IsDots()) // Ignore this item.
		{
			continue;
		}
		else if(finder.IsDirectory()) // Delete all sub item.
		{
			DeleteAFolderFile(finder.GetFilePath());
			::RemoveDirectory((LPCTSTR)finder.GetFilePath());
		}
		else // Delete file.
		{
			::DeleteFile((LPCTSTR)finder.GetFilePath());
		}
	}
}


bool KShortestPathNTransitDlg::WriteShortestPathArgument(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		//TimeMethod
		strOut.Format(_T("%s\t%d\r\n"),  _T("TimeMethod"), m_nTimeMethod);
		of.WriteString(strOut);

		//input
		strOut.Format(_T("%s\t%d\r\n"),  _T("input"), 4);
		of.WriteString(strOut);
		strOut.Format(_T("%d\t%I64d\r\n"),         1, m_nxStartNodeID);
		of.WriteString(strOut);
		strOut.Format(_T("%d\t%I64d\r\n"),         2, m_nxEndNodeID);
		of.WriteString(strOut);
		strOut.Format(_T("%d\t%d\r\n"),            3, m_nRouteCount);
		of.WriteString(strOut);

		int nUseBus     = IsDlgButtonChecked(IDC_CHECK_B)  == BST_CHECKED ? 1 : 0;
		int nUseRail    = IsDlgButtonChecked(IDC_CHECK_R)  == BST_CHECKED ? 1 : 0;
		int nUseBusRail = IsDlgButtonChecked(IDC_CHECK_BR) == BST_CHECKED ? 1 : 0;

		strOut.Format(_T("%d\t%d\t%d\t%d\r\n"),    4, nUseBus, nUseRail, nUseBusRail);
		of.WriteString(strOut);

		KDBaseDefineTransitCost::WriteDefineCostArgument(of, m_vecTransitCost);

		//walktime
		/*strOut.Format(_T("%s\t%d\r\n"), _T("walktime"), 1);
		of.WriteString(strOut);
		strOut.Format(_T("%d\t%d\r\n"),              1, m_nWalkTime);
		of.WriteString(strOut);*/

		//In Folder
		strOut.Format(_T("%s\r\n"),              ConvertSavePath(_T("")));
		of.WriteString(strOut);

		//Out Folder
		strOut.Format(_T("%s\r\n"),              ConvertSavePath(TRANST_SHORTEST_RESULT_PATH));
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


Integer KShortestPathNTransitDlg::GetSelStartNodeId()
{
	Integer nxNodeId      = -1;

	try
	{
		nxNodeId = _ttoi64(m_oStartNodeIdCaption.TID);
	}
	catch (KExceptionPtr ex)
	{
		return -1;
		TxLogDebugException();
	}
	catch (...)
	{
		return -1;
		TxLogDebugException();
	}

	return nxNodeId;
}


Integer KShortestPathNTransitDlg::GetSelEndNodeId()
{
	Integer nxNodeId      = -1;

	try
	{
		nxNodeId = _ttoi64(m_oEndNodeIdCaption.TID);
	}
	catch (KExceptionPtr ex)
	{
		return -1;
		TxLogDebugException();
	}
	catch (...)
	{
		return -1;
		TxLogDebugException();
	}

	return nxNodeId;
}


void KShortestPathNTransitDlg::AllClearShortestPath(bool a_bRefresh)
{
	try
	{
		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL != pMapView)
		{
			pMapView->TxClearCompositeThemes(a_bRefresh);
		}

		if (nullptr != m_spTransitPathViewDlg)
		{
			m_spTransitPathViewDlg.reset();
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KShortestPathNTransitDlg::UpdateRouteReportData()
{
	m_ReportCtrlRoute.ResetContent(TRUE);

	BOOL bAllChecked(TRUE);
	if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
	{
		bAllChecked = TRUE;
	}
	else
	{
		bAllChecked = FALSE;
	}

	UpdateRouteReportChildData(bAllChecked, KEMTransitAssignBus,     m_mapBusAssign);
	UpdateRouteReportChildData(bAllChecked, KEMTransitAssignRail,    m_mapRailAssign);
	UpdateRouteReportChildData(bAllChecked, KEMTransitAssignBusRail, m_mapBusRailAssign);

	UpdatePathReportRecord();

	m_ReportCtrlRoute.Populate();
	m_ReportCtrlRoute.ExpandAll(TRUE);

	OnBnClickedBtnDraw();
}


void KShortestPathNTransitDlg::UpdateRouteReportChildData(BOOL a_bAllChecked, KEMTransitAssignMode a_emTransitAssignMode, std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapTransitAssing)
{
	try
	{
		AutoType iter  = a_mapTransitAssing.find(m_nxEndNodeID);
		AutoType itEnd = a_mapTransitAssing.end();

		if(iter == itEnd)
			return;

		std::vector<KMBusAssignInfo> &vecTransitAssignInfo = iter->second;

		if(vecTransitAssignInfo.size() < 1)
			return;

		int nMode;
		CString strTransitMode(_T(""));
		if(KEMTransitAssignBus == a_emTransitAssignMode) {
			strTransitMode = _T("Bus");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strTransitMode = _T("버스");
			}
			nMode = KENUMModeBus;
		}
		else if(KEMTransitAssignRail == a_emTransitAssignMode) {
			strTransitMode = _T("Rail");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strTransitMode = _T("도시철도");
			}
			nMode = KENUMModeRail;
		}
		else {
			strTransitMode = _T("Bus+Rail");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strTransitMode = _T("버스+도시철도");
			}
			nMode = KENUMModeBusRail;
		}

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		pRecord = m_ReportCtrlRoute.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTransitMode));
		pItem->SetItemData((DWORD_PTR)nMode);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(a_bAllChecked);
		for (int i= 0; i< R_COLUMN_TOTAL_COUNT -1; i++)
		{
			pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		}
		pItem->SetAlignment(DT_LEFT);

		CXTPReportRecords*     pChRecords = pRecord->GetChilds();
		CXTPReportRecord*      pChRecord  = nullptr;

		for (size_t i= 0; i< vecTransitAssignInfo.size(); i++)
		{
			pChRecord = pChRecords->Add(new CXTPReportRecord());

			KMBusAssignInfo &oTransitAssignInfo = vecTransitAssignInfo[i];

			// - Name
			CString strName  (_T(""));
			strName.Format(_T("#P%d"), (i+1));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strName.Format(_T("경로%d"), (i+1));
			}

			pItem = pChRecord->AddItem(new CXTPReportRecordItemText(strName));
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked (a_bAllChecked);
			pItem->SetItemData((DWORD_PTR)&oTransitAssignInfo);

			TTransitRouteCostData oTTransitRouteCostData;
			GetTotalCostData(a_emTransitAssignMode, oTransitAssignInfo, oTTransitRouteCostData);

			pItem = pChRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitRouteCostData.dTotal_Length, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);

			pItem = pChRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitRouteCostData.dTotal_Cost,   _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);

			pItem = pChRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitRouteCostData.dTotal_Alltime, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);

			pItem = pChRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitRouteCostData.dTotal_TimeVeh, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);

			pItem = pChRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitRouteCostData.dTotal_TimeWait, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);

			pItem = pChRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitRouteCostData.dTotal_TimeTrans, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);

			pItem = pChRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitRouteCostData.dTotal_TimeAccessWalk, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);

			pItem = pChRecord->AddItem(new CXTPReportRecordItemNumber(oTTransitRouteCostData.dTotal_TimeEgressWalk, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KShortestPathNTransitDlg::GetTotalCostData(KEMTransitAssignMode a_emTransitAssignMode, KMBusAssignInfo &a_oTransitAssignInfo, TTransitRouteCostData &a_oTTransitRouteCostData)
{
	if(KEMTransitAssignBus == a_emTransitAssignMode || KEMTransitAssignBusRail == a_emTransitAssignMode)
	{
		a_oTTransitRouteCostData.dTotal_Length         = a_oTransitAssignInfo.oTPathBusSubData.dLength;
		a_oTTransitRouteCostData.dTotal_Alltime        = 0.0;
		a_oTTransitRouteCostData.dTotal_Cost           = a_oTransitAssignInfo.oTPathBusSubData.dCost;
		a_oTTransitRouteCostData.dTotal_TimeVeh        = a_oTransitAssignInfo.oTPathBusSubData.dInVehTime;
		a_oTTransitRouteCostData.dTotal_TimeWait       = a_oTransitAssignInfo.oTPathBusSubData.dWaitTime;
		a_oTTransitRouteCostData.dTotal_TimeTrans      = a_oTransitAssignInfo.oTPathBusSubData.dTransferTime;
		a_oTTransitRouteCostData.dTotal_TimeAccessWalk = a_oTransitAssignInfo.oTPathBusSubData.dAccessWalkTime;
		a_oTTransitRouteCostData.dTotal_TimeEgressWalk = a_oTransitAssignInfo.oTPathBusSubData.dEgressWalkTime;
	}
	else if(KEMTransitAssignRail == a_emTransitAssignMode)
	{
		a_oTTransitRouteCostData.dTotal_Length         = a_oTransitAssignInfo.oTPathRailSubData.dLength;
		a_oTTransitRouteCostData.dTotal_Alltime        = 0.0;
		a_oTTransitRouteCostData.dTotal_Cost           = a_oTransitAssignInfo.oTPathRailSubData.dCost;
		a_oTTransitRouteCostData.dTotal_TimeVeh        = a_oTransitAssignInfo.oTPathRailSubData.dInVehTime;
		a_oTTransitRouteCostData.dTotal_TimeWait       = a_oTransitAssignInfo.oTPathRailSubData.dWaitTime;
		a_oTTransitRouteCostData.dTotal_TimeTrans      = a_oTransitAssignInfo.oTPathRailSubData.dTransferTime;
		a_oTTransitRouteCostData.dTotal_TimeAccessWalk = a_oTransitAssignInfo.oTPathRailSubData.dAccessWalkTime;
		a_oTTransitRouteCostData.dTotal_TimeEgressWalk = a_oTransitAssignInfo.oTPathRailSubData.dEgressWalkTime;
	}
}


void KShortestPathNTransitDlg::OnBnClickedBtnDraw()
{
	try
	{
		DrawShortestPath();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KShortestPathNTransitDlg::DrawShortestPath()
{
	try
	{
		AllClearShortestPath(false);

		//QBicSimpleProgressThread::ExecuteThread(SelectDrawDataThreadCaller, this);
		CWaitCursor cw;
		SelectDrawData();

		//m_pMapView->TxDrawCompositeNode(m_oMapResult.vecDrawNodeInfo, false, false);

		m_pMapView->TxDrawCompositeLink    (m_oMapResult.vecDrawLinkInfo, false, false);
		//존->정류장 도보제거 요청 : 2013.03.03-명지대학교
		m_pMapView->TxDrawCompositeUserLine(m_oMapResult.vecUserLineInfo, false, false);

		DrawOrgDestNode(m_oMapResult.vecDrawNodeInfo, false); // 기점과 종점 정보를 추가하여, Draw 함

		m_pMapView->MapRefresh();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


unsigned __stdcall KShortestPathNTransitDlg::MakeDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KShortestPathNTransitDlg*     pDlg = (KShortestPathNTransitDlg*)pParameter->GetParameter();
	pDlg->MakeDrawData();

	return 0;
}


unsigned __stdcall KShortestPathNTransitDlg::SelectDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KShortestPathNTransitDlg*     pDlg = (KShortestPathNTransitDlg*)pParameter->GetParameter();
	pDlg->SelectDrawData();

	return 0;
}


void KShortestPathNTransitDlg::SelectDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		TMResultSettingInfo oTMResltSettingInfo;
		oTMResltSettingInfo.clrBus      = RGB (   0, 128,   0 );
		oTMResltSettingInfo.clrRail     = RGB (   0,   0, 255 );
		oTMResltSettingInfo.clrWalk     = RGB ( 102, 102, 153 );
		oTMResltSettingInfo.clrOutRange = RGB ( 221, 217, 195 );
		oTMResltSettingInfo.dMinTrip    = 1.0;
		oTMResltSettingInfo.dMaxTrip    = 2.0;
		oTMResltSettingInfo.dMinSize    = 3.0;
		oTMResltSettingInfo.dMaxSize    = 4.0;
		oTMResltSettingInfo.bUseOutRange = FALSE;

		std::map<int, std::set<int>> mapModePIndex;

		CXTPReportRecordItem*  pItem    = nullptr;
		CXTPReportRecords*     pRecords = m_ReportCtrlRoute.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord* pParentRecord = pRecords->GetAt(i);
			pItem     = pParentRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);
			int nMode = (int)pItem->GetItemData();

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			if (nullptr == pSeParentRecords)
				continue;

			for (int k= 0; k<  pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem            = pSeParentRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);

				if( pItem->GetHasCheckbox() == FALSE )
					continue;
				if( pItem->IsChecked() == FALSE )
					continue;

				std::set<int> &setPIndex = mapModePIndex[nMode];
				setPIndex.insert(k);
			}
		}

		KMultiModalResultInfo::ResultTransitShortestInfo3(m_pTarget, m_mapModeResultInputValue, mapModePIndex, oTMResltSettingInfo, m_oMapResult, m_cpWalkColor.GetColor());
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

	TxLogDebugEndMsg();
}


void KShortestPathNTransitDlg::MakeDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		m_mapModeResultInputValue.clear();

		std::map<int, std::vector<KMBusAssignInfo>*> mapModeMAssignInfo;

		std::vector<KMBusAssignInfo>* pvecBusAssignInfo     = nullptr;
		std::vector<KMBusAssignInfo>* pvecRailAssignInfo    = nullptr;
		std::vector<KMBusAssignInfo>* pvecBusRailAssignInfo = nullptr;
		pvecBusAssignInfo     = IsHaveBusAssignData    (m_nxEndNodeID);
		pvecRailAssignInfo    = IsHaveRailAssignData   (m_nxEndNodeID);
		pvecBusRailAssignInfo = IsHaveBusRailAssignData(m_nxEndNodeID);

		if (nullptr == pvecBusAssignInfo && nullptr == pvecRailAssignInfo && nullptr == pvecBusRailAssignInfo) {
			return;
		}

		if (pvecBusAssignInfo != nullptr) {
			mapModeMAssignInfo.insert(std::make_pair((int)KENUMModeBus, pvecBusAssignInfo));
		}

		if (pvecRailAssignInfo != nullptr) {
			mapModeMAssignInfo.insert(std::make_pair((int)KENUMModeRail, pvecRailAssignInfo));
		}

		if (pvecBusRailAssignInfo != nullptr) {
			mapModeMAssignInfo.insert(std::make_pair((int)KENUMModeBusRail, pvecBusRailAssignInfo));
		}

		m_mapModeResultInputValue.clear();
		KMultiModalResultInfo::ResultTransitShortestInfo2(m_pTarget, mapModeMAssignInfo, OUT m_mapModeResultInputValue);
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

	TxLogDebugEndMsg();
}

std::vector<KMBusAssignInfo>* KShortestPathNTransitDlg::IsHaveBusAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapBusAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapBusAssign.end();

	if(iter != itEnd)
	{
		std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}


std::vector<KMBusAssignInfo>* KShortestPathNTransitDlg::IsHaveRailAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapRailAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapRailAssign.end();

	if(iter != itEnd)
	{
		std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}


std::vector<KMBusAssignInfo>* KShortestPathNTransitDlg::IsHaveBusRailAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapBusRailAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapBusRailAssign.end();

	if(iter != itEnd)
	{
		std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
		return (&vecBusAssignInfo);
	}

	return nullptr;
}


void KShortestPathNTransitDlg::OnBnClickedBtnClear()
{
	/************************************************************************/
	/* Clear버튼 클릭 시 모든 체크 옵션 선택이 취소되고 선택한 Origin/Destination에 Access Boundary만 표출되어야 함.                                                                     */
	/************************************************************************/
	CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
	OnBnClickedCheckAll();
}


void KShortestPathNTransitDlg::OnRouteReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	SendMessage(DM_SETDEFID, IDC_BUTTON4, 0);

	if( _0_R_COLUMN_ROUTE_NAME == nIndex)
	{
		try
		{
			CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);

			if(pItem->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == TRUE)
			{
				BOOL bParentChecked = pItem->IsChecked();
				if (FALSE == bParentChecked) {
					CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
				}

				CXTPReportRecords* pChdRecords = pRecord->GetChilds();
				int nCount = pChdRecords->GetCount();
				for (int i= 0; i< nCount; i++)
				{
					CXTPReportRecord*      pChdRecord = pChdRecords->GetAt(i);
					CXTPReportRecordItem*  pChdItem   = pChdRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);
					pChdItem->SetChecked(bParentChecked);
				}
				m_ReportCtrlRoute.RedrawControl();
			}
			else
			{
				CXTPReportRecord*   pParentRecord = pRecord->GetParentRecord();
				BOOL bCheked = pItem->IsChecked();
				if (FALSE == bCheked) {
					CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
				}

				CXTPReportRecords*  pChdRecords   = pParentRecord->GetChilds();
				int nCount        = pChdRecords->GetCount();
				int nCheckedCount = 0;
				for (int i= 0; i< nCount; i++)
				{
					CXTPReportRecord*      pChdRecord = pChdRecords->GetAt(i);
					CXTPReportRecordItem*  pChdItem   = pChdRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);
					if( pChdItem->IsChecked() == TRUE)
					{
						nCheckedCount++;
					}
				}

				CXTPReportRecordItem*  pParentItem = pParentRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);
				pParentItem->SetChecked(bCheked);

				if (nCheckedCount > 0)
				{
					pParentItem->SetChecked(TRUE);
				}
				m_ReportCtrlRoute.RedrawControl();
			}
		}
		catch (...)
		{
			TxLogDebugException();
			return;
		}

		UpdatePathReportRecord();
		OnBnClickedBtnDraw();
	}
	else
	{
		/*ZoomToNodeFeature(oTRouteInfo.nxNodeIdStart);*/
	}
}


void KShortestPathNTransitDlg::UpdatePathReportRecord()
{
	//m_ReportCtrlPath.GetRecords()->RemoveAll();
	m_ReportCtrlPath.ResetContent(TRUE);

	CXTPReportRecords* pRouteRecords = m_ReportCtrlRoute.GetRecords();
	int nRouteRecordCount = pRouteRecords->GetCount();

	for(int i= 0; i< nRouteRecordCount; i++)
	{
		CXTPReportRecord*     pRouteRecord    = pRouteRecords->GetAt(i);
		CXTPReportRecordItem* pRouteCheckItem = pRouteRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);

		if( pRouteCheckItem->GetHasCheckbox() == FALSE )
			continue;
		if( pRouteCheckItem->IsChecked() == FALSE )
			continue;

		CString strAssignMode = pRouteCheckItem->GetCaption(NULL);

		CXTPReportRecords* pRecords = pRouteRecord->GetChilds();
		for (int m= 0; m<  pRecords->GetCount(); m++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(m);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);

			if( pItem->IsChecked() == FALSE )
				continue;

			CString strSeq = pItem->GetCaption(NULL);
			KMBusAssignInfo* pAssignInfo = (KMBusAssignInfo*)pItem->GetItemData();

			//----------------------
			CString strRouteName(_T(""));
			strRouteName.Format(_T("%s %s"), strAssignMode, strSeq);

			UpdatePathReportChildRecord(strRouteName, pAssignInfo);
		}
	}

	m_ReportCtrlPath.Populate();
	m_ReportCtrlPath.ExpandAll(TRUE);
}


void KShortestPathNTransitDlg::UpdatePathReportChildRecord(CString a_strRouteName, KMBusAssignInfo* a_pAssignInfo)
{
	CXTPReportRecord*         pParRecord     = nullptr;
	CXTPReportRecords*        pSeParRecords  = nullptr;
	CXTPReportRecord*         pSeParRecord   = nullptr;
	CXTPReportRecordItem*     pItem          = nullptr;
	CXTPReportRecordItemText* pItemText      = nullptr;

	Integer nxStartStationID(0);
	Integer nxEndStationID(0);
	CString strTransitName(_T(""));

	pParRecord = m_ReportCtrlPath.AddRecord(new CXTPReportRecord());
	pItem      = pParRecord->AddItem(new CXTPReportRecordItemText(a_strRouteName));
	for (int n= 0; n< P_COLUMN_TOTAL_COUNT-1; n++)
	{
		pParRecord->AddItem(new CXTPReportRecordItemText());
	}
	pItem->SetAlignment(DT_LEFT);
	pSeParRecords = pParRecord->GetChilds();

	std::vector<TIndexResultTransitInfo> &vecResultBusInfo = a_pAssignInfo->vecResultBusInfo;

	int     nIdx(0);
	CString strIdx(_T(""));
	for (size_t i= 0; i< vecResultBusInfo.size(); i++)
	{
		std::vector<TMResultBusInfo> &vecLowResultTransitInfo = vecResultBusInfo[i].vecLowResultTransitInfo;
		KODKey                       &oIndexKey               = vecResultBusInfo[i].oIndexKey;

		if(vecLowResultTransitInfo.size() < 1)
			continue;

		pSeParRecord = pSeParRecords->Add(new CXTPReportRecord());

		Integer &nxFIndex = oIndexKey.OriginID;
		Integer &nxTIndex = oIndexKey.DestinationID;

		if (0 == nxTIndex)
		{
			strIdx.Format(_T("#R%I64d"), nxFIndex);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strIdx.Format(_T("구간%I64d"), nxFIndex);
			}
		}
		else
		{
			strIdx.Format(_T("#R%I64d-%I64d"), nxFIndex, nxTIndex);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strIdx.Format(_T("구간%I64d-%I64d"), nxFIndex, nxTIndex);
			}
		}
		pItem  = pSeParRecord->AddItem(new CXTPReportRecordItemText(strIdx));
		for (int n= 0; n< P_COLUMN_TOTAL_COUNT-1; n++)
		{
			pSeParRecord->AddItem(new CXTPReportRecordItemText());
		}
		pItem->SetAlignment(DT_LEFT);

		CXTPReportRecords* pThParRecords = pSeParRecord->GetChilds();

		for (size_t k= 0; k< vecLowResultTransitInfo.size(); k++)
		{
			TMResultBusInfo &oTMResultBusInfo = vecLowResultTransitInfo[k];

			CXTPReportRecord* pThParRecord    = pThParRecords->Add(new CXTPReportRecord());

			KMultiModalResultInfo::GetStatoinIDFromSEQ(m_pTarget, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartStationID);
			KMultiModalResultInfo::GetStatoinIDFromSEQ(m_pTarget, oTMResultBusInfo.nEndStation,   oTMResultBusInfo.nxTransitID, nxEndStationID);
			GetTransitName(m_pTarget, oTMResultBusInfo.nxTransitID, strTransitName); 

			CString strTransit(_T(""));
			strTransit.Format(_T("%I64d [%s]"), oTMResultBusInfo.nxTransitID, strTransitName);
			CString strTransitID(_T(""));
			strTransitID.Format(_T("%I64d"), oTMResultBusInfo.nxTransitID);
			pItemText = (CXTPReportRecordItemText*)pThParRecord->AddItem(new CXTPReportRecordItemText());
			pItemText->SetValue(strTransitID);
			pItemText->SetCaption(strTransit);
			pItemText->SetAlignment(DT_LEFT);
			pItemText->SetItemData((DWORD_PTR)&oTMResultBusInfo);

			pItem = pThParRecord->AddItem(new CXTPReportRecordItem());
			CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			pButton->SetAlignment(xtpItemControlUnknown);
			pButton->SetCaption(_T("R"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pButton->SetCaption(_T("표출"));
			}
			pButton->SetSize(CSize(30, 0));
			pButton->SetThemed(TRUE);

			CString strType(_T(""));
			if(KEMPathTypeBus == oTMResultBusInfo.emPathType) {
				strType=_T("Bus");
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strType = _T("버스");
				}
			}
			else {
				strType=_T("Rail");
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strType = _T("도시철도");
				}
			}
			pItem = pThParRecord->AddItem(new CXTPReportRecordItemText(strType));
			pItem->SetAlignment(DT_CENTER);

			int &nStartStationSeq = oTMResultBusInfo.nStartStation;
			CString strStartStation(_T(""));
			if (nStartStationSeq <= 1)
			{
				strStartStation.Format(_T("%I64d(%d)"), nxStartStationID, nStartStationSeq);
			}
			else
			{
				strStartStation.Format(_T("%I64d(%d)"), nxStartStationID, nStartStationSeq-1);
			}

			pItem = pThParRecord->AddItem(new CXTPReportRecordItemText(strStartStation));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR)nxStartStationID);

			int &nEndStationSeq = oTMResultBusInfo.nEndStation;
			CString strEndStation(_T(""));
			strEndStation.Format(_T("%I64d(%d)"), nxEndStationID, nEndStationSeq-1);
			pItem = pThParRecord->AddItem(new CXTPReportRecordItemText(strEndStation));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR)nxEndStationID);
		}
	}
}


void KShortestPathNTransitDlg::GetTransitName(KTarget* a_pTarget, Integer &a_nxTransitID, CString &a_strTransitName)
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


CString KShortestPathNTransitDlg::GetTransitTypeName(ShortestPathInfo::KEMTransitType &a_emTransitType)
{
	CString strType =_T("");

	switch (a_emTransitType)
	{
	case ShortestPathInfo::TRANSIT_TYPE_WALK:
		strType = _T("walk");
		break;
	case ShortestPathInfo::TRANSIT_TYPE_BUS:
		strType = _T("bus");
		break;
	case ShortestPathInfo::TRANSIT_TYPE_RAIL:
		strType = _T("rail");
		break;
	default:
		strType = _T("");
		break;
	}

	return strType;
}


void KShortestPathNTransitDlg::OnPathReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	SendMessage(DM_SETDEFID, IDC_BUTTON4, 0);

	CXTPReportRecord*	  pRecord = pItemNotify->pRow->GetRecord();
	int nIndex = pItemNotify->pColumn->GetItemIndex();

	try
	{
		if(_3_P_COLUMN_NODE_START == nIndex || _4_P_COLUMN_NODE_END == nIndex )
		{
			CXTPReportRecordItem* pItem   = pRecord->GetItem(nIndex);
			Integer nxNodeID = (Integer)pItem->GetItemData();

			KMapView* pMapView = GetMapView(m_pTarget);
			if (nullptr == pMapView)
				throw 1;

			//pMapView->FlashToFeature(KLayerID::Node(), nxNodeID);
			pMapView->PanToFeature(KLayerID::Node(), nxNodeID, true);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KShortestPathNTransitDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KShortestPathNTransitDlg::OnEnSetfocusRouteCount()
{
	SendMessage(DM_SETDEFID, IDC_BUTTON1, 0);
}


void KShortestPathNTransitDlg::OnEnSetfocusWalkTime()
{
	SendMessage(DM_SETDEFID, IDC_BUTTON1, 0);
}


BOOL KShortestPathNTransitDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
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


void KShortestPathNTransitDlg::OnBnClickedRadioTimeMethod(UINT nID)
{
	if(IDC_RADIO_INITTIME == nID)
	{
		GetDlgItem(IDC_COMBO_USINGFIELD)->EnableWindow(FALSE);
	}
	else if(IDC_RADIO_USINGFIELD == nID)
	{
		GetDlgItem(IDC_COMBO_USINGFIELD)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_USINGFIELD)->SetFocus();
	}
}


CString KShortestPathNTransitDlg::ConvertSavePath(CString a_strFileName)
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, a_strFileName);

	return strSavePath;
}


void KShortestPathNTransitDlg::OnItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
		if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
			return;

		CXTPReportRecordItem* pItem = pItemNotify->pItem;

		if(_1_P_COLUMN_ROUTE == pItem->GetIndex())
		{
			pItemNotify->pRow->SetSelected(TRUE);	

			CXTPReportRecord*     pRecord        = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItemB         = pRecord->GetItem(_0_P_COLUMN_TRANSITID);

			CXTPReportRecordItemText* pRecordItemText = dynamic_cast<CXTPReportRecordItemText*>(pItemB);
			if (nullptr == pRecordItemText)
				throw 1;

			KMapView* pMapView = GetMapView(m_pTarget);
			if (nullptr == pMapView)
				throw 1;

			Integer nxTransitID = _ttoi64(pRecordItemText->GetValue());

			if( nullptr == m_spTransitPathViewDlg)
			{
				m_spTransitPathViewDlg = KTransitPathViewDlgPtr(new KTransitPathViewDlg(m_pTarget, pMapView));
				m_spTransitPathViewDlg->Create(KTransitPathViewDlg::IDD);
			}

			m_spTransitPathViewDlg->SetViewTransit(nxTransitID);
			m_spTransitPathViewDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KShortestPathNTransitDlg::OnBnClickedCheckAll()
{
	UpdateRouteReportData();
}


void KShortestPathNTransitDlg::OnSelEndOkBusClr()
{
	try
	{
		CXTPReportRecords* pRouteRecords = m_ReportCtrlRoute.GetRecords();
		int nRouteRecordCount = pRouteRecords->GetCount();

		for(int i= 0; i< nRouteRecordCount; i++)
		{
			CXTPReportRecord*     pRouteRecord    = pRouteRecords->GetAt(i);
			CXTPReportRecordItem* pRouteCheckItem = pRouteRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);

			if( pRouteCheckItem->GetHasCheckbox() == FALSE )
				continue;

			CXTPReportRecords* pRecords = pRouteRecord->GetChilds();
			for (int m= 0; m<  pRecords->GetCount(); m++)
			{
				CXTPReportRecord*     pRecord = pRecords->GetAt(m);
				CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);

				KMBusAssignInfo* pAssignInfo = (KMBusAssignInfo*)pItem->GetItemData();
				std::vector<TIndexResultTransitInfo> &vecResultBusInfo = pAssignInfo->vecResultBusInfo;

				for (size_t n= 0; n< vecResultBusInfo.size(); n++)
				{
					std::vector<TMResultBusInfo> &vecTMResult = vecResultBusInfo[n].vecLowResultTransitInfo;

					for (size_t p= 0; p< vecTMResult.size(); p++)
					{
						TMResultBusInfo &oTMResultBusInfo = vecTMResult[p];
						if(KEMPathTypeBus == oTMResultBusInfo.emPathType)
						{
							oTMResultBusInfo.clrSect = m_cpBusColor.GetColor();
						}
					}
				}
			}
		}

		UpdatePathReportRecord();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KShortestPathNTransitDlg::OnSelEndOkRailClr()
{
	try
	{
		CXTPReportRecords* pRouteRecords = m_ReportCtrlRoute.GetRecords();
		int nRouteRecordCount = pRouteRecords->GetCount();

		for(int i= 0; i< nRouteRecordCount; i++)
		{
			CXTPReportRecord*     pRouteRecord    = pRouteRecords->GetAt(i);
			CXTPReportRecordItem* pRouteCheckItem = pRouteRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);

			if( pRouteCheckItem->GetHasCheckbox() == FALSE )
				continue;

			CXTPReportRecords* pRecords = pRouteRecord->GetChilds();
			for (int m= 0; m<  pRecords->GetCount(); m++)
			{
				CXTPReportRecord*     pRecord = pRecords->GetAt(m);
				CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_R_COLUMN_ROUTE_NAME);

				KMBusAssignInfo* pAssignInfo = (KMBusAssignInfo*)pItem->GetItemData();
				std::vector<TIndexResultTransitInfo> &vecResultBusInfo = pAssignInfo->vecResultBusInfo;

				for (size_t n= 0; n< vecResultBusInfo.size(); n++)
				{
					std::vector<TMResultBusInfo> &vecTMResult = vecResultBusInfo[n].vecLowResultTransitInfo;

					for (size_t p= 0; p< vecTMResult.size(); p++)
					{
						TMResultBusInfo &oTMResultBusInfo = vecTMResult[p];
						if(KEMPathTypeRail == oTMResultBusInfo.emPathType)
						{
							oTMResultBusInfo.clrSect = m_cpRailColor.GetColor();
						}
					}
				}
			}
		}

		UpdatePathReportRecord();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KShortestPathNTransitDlg::OnCbnSelchangeComboUsingfield()
{
	UpdateData(TRUE);

	CString strCriterionData = _T("");
	int nSelCur	= m_cboUsingLinkField.GetCurSel();

	KIOColumn* pSelColumn      = (KIOColumn*)(m_cboUsingLinkField.GetItemData(nSelCur));
	if(NULL == pSelColumn)
	{
		return;
	}

	m_strPreSelectedColumnName = pSelColumn->Name();
}


BOOL KShortestPathNTransitDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			HWND hwnd = pMsg->hwnd;

			if (hwnd == GetDlgItem(IDC_BUTTON6)->m_hWnd   ||
				hwnd == GetDlgItem(IDC_BUTTON7)->m_hWnd   || 
				hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd     ||
				hwnd == GetDlgItem(IDC_EDIT4)->m_hWnd     || 
				hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
			{
				OnBnClickedBtnSearch();
				return TRUE;
			}
		}
	}
	else if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd)
		{
			ClickedEditStartNode();
			return TRUE;
		}
		else if (hwnd == GetDlgItem(IDC_EDIT4)->m_hWnd)
		{
			ClickedEditEndNode();
			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KShortestPathNTransitDlg::ClickedEditStartNode()
{
	SelectStartNodeList();
}


void KShortestPathNTransitDlg::ClickedEditEndNode()
{
	SelectEndNodeList();
}


void KShortestPathNTransitDlg::OnBnClickedButtonStartNodeSearch()
{
	SelectStartNodeList();
}


void KShortestPathNTransitDlg::OnBnClickedButtonEndNodeSearch()
{
	SelectEndNodeList();
}


void KShortestPathNTransitDlg::SelectStartNodeList()
{
	try
	{
		m_bSelectMapNode = MAP_NODE_START;

		CWaitCursor cw;

		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		if (IsDlgButtonChecked(IDC_CHECK_ZONESTART) == BST_CHECKED)
		{
			m_bZone = TRUE;
			m_spImTasIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);
		}
		else
		{
			m_bZone = FALSE;
			m_spImTasIDFind->SetIDCaption(m_vecNodeIDCaption);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapNodeData, ImChampMapDefineSymbol::NodeSymbol);
		}

// 		Integer nxNodeID = _ttoi64(m_oStartNodeIdCaption.TID);
// 		m_spImTasIDFind->SetSelectedID(nxNodeID);
		m_spImTasIDFind->SetSelectedID(-1);
		m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KShortestPathNTransitDlg::SelectEndNodeList()
{
	try
	{
		m_bSelectMapNode = MAP_NODE_END;

		CWaitCursor cw;

		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		if (IsDlgButtonChecked(IDC_CHECK_ZONEEND) == BST_CHECKED)
		{
			m_bZone = TRUE;
			m_spImTasIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);
		}
		else
		{
			m_bZone = FALSE;
			m_spImTasIDFind->SetIDCaption(m_vecNodeIDCaption);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapNodeData, ImChampMapDefineSymbol::NodeSymbol);
		}

// 		Integer nxNodeID = _ttoi64(m_oEndNodeIdCaption.TID);
// 		m_spImTasIDFind->SetSelectedID(nxNodeID);
		m_spImTasIDFind->SetSelectedID(-1);
		m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}

HBRUSH KShortestPathNTransitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_EDIT3 || ID == IDC_EDIT4 )
    {        
        /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
        pDC->SetBkMode(TRANSPARENT);*/
        pDC->SetBkMode(TRANSPARENT);

		hbr = (HBRUSH)m_backBrush;
        //return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }
	return hbr;
}

void KShortestPathNTransitDlg::ModelessCancleFeedback(UINT nDialogID)
{

}
