// InterModalResultDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterModalResultDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
//^#include "DBaseConnector.h"
#include "CustomReportRecordItem.h"
#include "KExRecordItem.h"
#include "NodeSelectionInfo.h"
#include "..\QBicCommon\ColorRangeGen.h"
//^#include "MunsellColorCircle.h"
#include "ImChampMapDefine.h"
#include "MultiModalResultInfo.h"

#include "KDBaseAccessibilityModify.h"


const int COLUMN_WIDTH_INFO = 50;	// 상세정보 컬럼 너비


namespace InterResultENUM
{
	#define UM_SEARCH_COMPLETE		WM_USER + 1001

	enum
	{
		_0_E_COLUMN_SELECT     = 0,
		_1_E_COLUMN_NAME       = 1
	};

	enum
	{
		_0_B_COLUMN_ZONEID        = 0,
		_1_B_COLUMN_TOTAL_TIME    = 1,
		_2_B_COLUMN_COST          = 2,
		_3_B_COLUMN_TRIP          = 3,
		_4_B_COLUMN_DETAIL        = 4
	};
	enum 
	{
		B_COLUMN_TOTAL_COUNT     = 5
	};

	enum
	{
		EXPANDALL_LIMIT_COUNT    = 10 
	};
}

using namespace InterResultENUM;
// KInterModalResultDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterModalResultDlg, KResizeDialogEx)

KInterModalResultDlg::KInterModalResultDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterModalResultDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_dMinVolume(0.0)
	, m_dMaxVolume(0.0)
	, m_dRadius(1000.0)
{
	m_oStartNodeIdCaption.TID = _T("-1");

	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_AUTO,   TInterPathColor(_T("승용차"),    RGB ( 228, 108,  10))));
		//m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_TAXI,   TInterPathColor(_T("택시"),    RGB ( 255, 255, 100))));
		m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_BUS,    TInterPathColor(_T("버스"),     RGB (   0, 128,   0))));
		m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_SUBWAY, TInterPathColor(_T("도시철도"),  RGB (   0,   0, 255))));
		m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_WALK,   TInterPathColor(_T("도보"),    RGB ( 102, 102, 153))));
	}
	else {
		m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_AUTO,   TInterPathColor(_T("PassengerCar"),    RGB ( 228, 108,  10))));
		//m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_TAXI,   TInterPathColor(_T("Taxi"),    RGB ( 255, 255, 100))));
		m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_BUS,    TInterPathColor(_T("Bus"),     RGB (   0, 128,   0))));
		m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_SUBWAY, TInterPathColor(_T("Rail"),  RGB (   0,   0, 255))));
		m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_WALK,   TInterPathColor(_T("Walk"),    RGB ( 102, 102, 153))));
	}

	m_clrOutRange        = RGB ( 221, 217, 195 );
	m_clrOrgZone         = RGB(255,0,0);
	m_clrDestZone        = RGB(0,0,205);

	m_spPathCompareUrbanDlg = nullptr;
	m_spPathCompareRegionDlg = nullptr;
}

KInterModalResultDlg::~KInterModalResultDlg()
{
	CloseWindowProc();
	m_backBrush.DeleteObject();
}

void KInterModalResultDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_E,                 m_ctrlReportDest);
	
	DDX_Control(pDX, IDC_REPORT_A,                 m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT_C,                 m_wndReportCCtrl);
	DDX_Control(pDX, IDC_REPORT_B,                 m_wndReportBCtrl);
	DDX_Control(pDX, IDC_REPORT_COLORA,            m_wndReportColorACtrl);
	DDX_Control(pDX, IDC_REPORT_COLORB,            m_wndReportColorBCtrl);
	DDX_Control(pDX, IDC_CLR_OUT,                  m_cpOutRangeColor);
}


BEGIN_MESSAGE_MAP(KInterModalResultDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &KInterModalResultDlg::OnBnClickedButtonStartNodeSearch)
	ON_BN_CLICKED(IDC_BUTTON2, &KInterModalResultDlg::OnBnClickedButtonEndNodeSearch)

	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_E, &KInterModalResultDlg::OnReportDestCheckItem)
	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_E, &KInterModalResultDlg::OnReportDestItemClick)

	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_A, &KInterModalResultDlg::OnReportACheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_C, &KInterModalResultDlg::OnReportCCheckItem)

	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_B, &KInterModalResultDlg::OnReportBItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_B, &KInterModalResultDlg::OnReportBCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT_B, &KInterModalResultDlg::OnReportBItemButtonClick)

	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_COLORA, &KInterModalResultDlg::OnReportColorAItemClick)
	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_COLORB, &KInterModalResultDlg::OnReportColorBItemClick)

	ON_BN_CLICKED(IDC_BUTTON1,     &KInterModalResultDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON5,     &KInterModalResultDlg::OnBnClickedButtonClear)
	
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_ALL, &KInterModalResultDlg::OnBnClickedCheckAll)
	ON_MESSAGE(UM_SEARCH_COMPLETE, OnNodeSearchCompleted)
	ON_BN_CLICKED(IDC_CHECK_OUTRANGE, &KInterModalResultDlg::OnBnClickedCheckOutrange)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KInterModalResultDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDCANCEL, &KInterModalResultDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_COMPARE_PATH, &KInterModalResultDlg::OnBnClickedBtnComparePath)
END_MESSAGE_MAP()


// KInterModalResultDlg 메시지 처리기입니다.
void KInterModalResultDlg::ActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spInterModalResultDetailDlg)
	{
		if(!m_spInterModalResultDetailDlg->IsUserClosed())
		{
			if(!m_spInterModalResultDetailDlg->IsWindowVisible())
				m_spInterModalResultDetailDlg->ShowWindow(SW_SHOW);
		}
		m_spInterModalResultDetailDlg->ActivatedFrameWndNotifyProcess();
	}

	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(!m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_SHOW);
		}
	}

	if(nullptr != m_spImTasMultiIDFind)
	{
		if(!m_spImTasMultiIDFind->IsUserClosed())
		{
			if(!m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
		}
	}

	// 2019.03.12 명지대 요청사항 수정, 2019.04.02, 지역간 대화상자 추가
	if (nullptr != m_spPathCompareUrbanDlg)
	{
		if (!m_spPathCompareUrbanDlg->IsUserClosed())
		{
			if (!m_spPathCompareUrbanDlg->IsWindowVisible())
			{
				m_spPathCompareUrbanDlg->ShowWindow(SW_SHOW);
			}
		}
	}

	if (nullptr != m_spPathCompareRegionDlg)
	{
		if (!m_spPathCompareRegionDlg->IsUserClosed())
		{
			if (!m_spPathCompareRegionDlg->IsWindowVisible())
			{
				m_spPathCompareRegionDlg->ShowWindow(SW_SHOW);
			}
		}
	}
	// 2019.03.12 명지대 요청사항 수정 끝
}


void KInterModalResultDlg::DeActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spInterModalResultDetailDlg)
	{
		if(!m_spInterModalResultDetailDlg->IsUserClosed())
		{
			if(m_spInterModalResultDetailDlg->IsWindowVisible())
				m_spInterModalResultDetailDlg->ShowWindow(SW_HIDE);
		}
		m_spInterModalResultDetailDlg->DeActivatedFrameWndNotifyProcess();
	}

	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_HIDE);
		}
	}

	if(nullptr != m_spImTasMultiIDFind)
	{
		if(!m_spImTasMultiIDFind->IsUserClosed())
		{
			if(m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
		}
	}

	// 2019.03.12 명지대 요청사항 수정, 2019.04.02 지역간 대화상자 추가
	if (nullptr != m_spPathCompareUrbanDlg)
	{
		if (!m_spPathCompareUrbanDlg->IsUserClosed())
		{
			if (m_spPathCompareUrbanDlg->IsWindowVisible())
			{
				m_spPathCompareUrbanDlg->ShowWindow(SW_HIDE);
			}
		}
	}

	if (nullptr != m_spPathCompareRegionDlg)
	{
		if (!m_spPathCompareRegionDlg->IsUserClosed())
		{
			if (m_spPathCompareRegionDlg->IsWindowVisible())
			{
				m_spPathCompareRegionDlg->ShowWindow(SW_HIDE);
			}
		}
	}
	// 2019.03.12 명지대 요청사항 수정 끝
}

void KInterModalResultDlg::InitializeNode()
{
// 	CWaitCursor wc;
// 
// 	m_mapAutoTaxiAssign.clear();
// 	m_mapBusAssign.clear();
// 	m_mapRailAssign.clear();
// 	m_mapBusRailAssign.clear();
// 	m_mapParkRideAssign.clear();
// 	m_mapKissRideAssign.clear();
// 	m_mapTaxiTransitAssign.clear();
// 
// 	m_mapRegionAssign.clear();
// 
// 	GetRegionTransitType();
// 	GetNodeInfo();
// 
// 	m_oStartNodeIdCaption.TID = _T("-1");
// 	SetDlgItemText(IDC_EDIT1, _T(""));	
// 
// 	m_setSearchedID.clear();
// 	m_strSearchedIDs = _T("");
// 	SetDlgItemText(IDC_EDIT2, m_strSearchedIDs);
// 
// 	InitDestination();	
// 	InitReportCHeader();
// 	InitReportCData();
// 	UpdateReportColorB();
// 	InitReportHeaderDest();
}


bool KInterModalResultDlg::GetNodeInfo(void)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL;
		strSQL.Format (_T(" SELECT node_id, name FROM %s "), TABLE_NODE);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			throw 1;
		}
		m_mapNodeName.clear();

		Integer nxNodeId(0);
		CString strNodeName(_T(""));

		while( pResult->Next() )
		{
			nxNodeId	    = pResult->GetValueInt64 (0);
			strNodeName     = pResult->GetValueString(1);

			m_mapNodeName.insert(std::make_pair(nxNodeId, strNodeName));
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQL(_T(""));
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


void KInterModalResultDlg::GetRegionTransitType(void)
{
	TxLogDebugStartMsg();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format (_T(" Select KCode_Key, KCode_Display from %s where KCode_Group_Key = %d "), TABLE_KCODE_DEFINE, TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		
		m_mapRegionalColor.clear();
		m_mapRegionalCodeName.clear();
		//m_mapRegionalCodeName.insert(std::make_pair(REGION_MODE_CODE_AUTO, _T("Auto")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapRegionalCodeName.insert(std::make_pair(REGION_MODE_CODE_AUTO, _T("Auto")));
		}
		else
		{
			m_mapRegionalCodeName.insert(std::make_pair(REGION_MODE_CODE_AUTO, _T("PassengerCar")));
		}

		int     nCode(0);
		CString strCodeName(_T(""));
		while( spResult->Next() )
		{
			nCode	        = spResult->GetValueInt   (0);
			strCodeName     = spResult->GetValueString(1);

			m_mapRegionalCodeName.insert(std::make_pair(nCode, strCodeName));
			m_mapRegionalColor.insert(std::make_pair(nCode, TInterPathColor(strCodeName,    RGB ( 255, 128, 87))));
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KInterModalResultDlg::ResizeComponent()
{
	SetResize(IDC_EDIT1,                		  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON4,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR5,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_EDIT2,                		  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON2,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_CHECK_ALL,       			      SZ_TOP_LEFT,  SZ_TOP_LEFT);
	SetResize(IDC_REPORT_E,       			      SZ_TOP_LEFT,      CXTPResizePoint(1.0, 0.2));

	SetResize(IDC_REPORT_A,       			      CXTPResizePoint(0.0, 0.2),   CXTPResizePoint(1.0, 0.2));
	SetResize(IDC_REPORT_C,       			      CXTPResizePoint(0.0, 0.2),   CXTPResizePoint(1.0, 0.2));
	SetResize(IDC_SEPRATOR8,       			      CXTPResizePoint(0.0, 0.2),   CXTPResizePoint(1.0, 0.2));

	SetResize(IDC_REPORT_B,       			      CXTPResizePoint(0.0, 0.2),   SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR9,       			      SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC1,       			      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_REPORT_COLORA,       			  SZ_BOTTOM_LEFT,    SZ_BOTTOM_CENTER);
	SetResize(IDC_SEPRATOR10,     			      SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
	SetResize(IDC_STATIC2,       			      SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
	SetResize(IDC_REPORT_COLORB,       			  SZ_BOTTOM_CENTER,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC18,       			      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC19,       			      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC20,       			      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC21,       			      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MINTRIP,       		      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAXTRIP,       		      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MINSIZE,       		      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAXSIZE,       		      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_OUTRANGE,       	      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_OUT,       		          SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR7,       			      SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON5,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON1,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_COMPARE_PATH,				  SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,       	          SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


BOOL KInterModalResultDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	double dRadiousKm(0.0);
	KDBaseAccessibilityModify::GetZoneRadius(m_pTarget, dRadiousKm);
	if (0.0 == dRadiousKm) {
		m_dRadius = 1000.0;
	}
	else {
		m_dRadius = dRadiousKm * 1000;
	}

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWaitCursor wc;

	GetRegionTransitType();
	GetNodeInfo();

	KReportCtrlSetting::Default(m_ctrlReportDest, FALSE, FALSE, FALSE);
	KReportCtrlSetting::AutoColumnSizeFalse(m_ctrlReportDest);

	KReportCtrlSetting::Default(m_wndReportACtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_wndReportCCtrl, TRUE, FALSE, FALSE);

	KReportCtrlSetting::DefaultC(m_wndReportBCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportBCtrl);

	KReportCtrlSetting::Default(m_wndReportColorACtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::HideHeader(m_wndReportColorACtrl);
	KReportCtrlSetting::Default(m_wndReportColorBCtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::HideHeader(m_wndReportColorBCtrl);

	KColorPickerSetting::DefaultCtrl(m_cpOutRangeColor,   m_clrOutRange);

	m_oStartNodeIdCaption.TID = _T("-1");
	SetDlgItemText(IDC_EDIT1, _T(""));	

	m_setSearchedID.clear();
	m_strSearchedIDs = _T("");
	SetDlgItemText(IDC_EDIT2, m_strSearchedIDs);
		
	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);

	InitReportAHeader();
	InitReportAData();

	InitReportCHeader();
	InitReportCData();

	InitReportBHeader();

	InitReportColorAHeader();
	UpdateReportColorA();
	InitReportColorBHeader();
	
	InitReportHeaderDest();
	UpdateReportColorB();

	SetDlgItemText(IDC_EDIT_MINSIZE, _T("2.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE, _T("20.0"));
	CheckDlgButton(IDC_CHECK_LABEL, BST_UNCHECKED);
	UpdateOutRange();

	m_backBrush.CreateSolidBrush(RGB(255,255,255));

	ResizeComponent();

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterModalResultDlg::ModelessOKFeedback( UINT nDialogID)
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

				bool bRefresh(false);

				CString strNodeView(_T(""));
				SetZoneNodeViewData(strID, strNodeView, m_oStartNodeIdCaption);
				SetDlgItemText(IDC_EDIT1,  strNodeView);
				InitDestination(bRefresh);
				FindPath();

				if (m_pMapView)
					m_pMapView->PanToFeature(KLayerID::Node(), nxOrgZoneID, bRefresh);

				bRefresh = true;
				DrawOrgZoneAccess(nxOrgZoneID, bRefresh);

				if (m_mapEndZoneNodeData.size() < 1)
				{
					AfxMessageBox(_T("검색한 종점 관련 정보가 존재하지 않습니다."));
				}
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
	else if (KImChampMultiIDFind::IDD == nDialogID)
	{
		if (m_spImTasMultiIDFind)
		{
			try
			{
				m_setSearchedID.clear();
				m_setSearchedID  = m_spImTasMultiIDFind->GetSelectedIDSet();
				m_strSearchedIDs = m_spImTasMultiIDFind->GetSelectedID();
				SetDlgItemText(IDC_EDIT2, m_strSearchedIDs);
				
				CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);

				PostMessage(UM_SEARCH_COMPLETE);
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


LRESULT KInterModalResultDlg::OnNodeSearchCompleted( WPARAM a_wParam, LPARAM a_lParam )
{
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(MakeDrawDataThreadCaller, this, false, nLang);

	UpdateReportDestData();

	return 0;
}


void KInterModalResultDlg::ZoomToNodeFeature( Integer &a_nxNodeID )
{
	try
	{
        m_pMapView->ZoomToFeature(KLayerID::Node(), a_nxNodeID);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KInterModalResultDlg::InitDestination(bool a_bRefresh)
{
	m_setSearchedID.clear();
	m_strSearchedIDs = _T("");
	SetDlgItemText(IDC_EDIT2, m_strSearchedIDs);
	if (nullptr != m_spImTasMultiIDFind)
	{
		m_spImTasMultiIDFind.reset();
	}

	InitVolMinMax();
	SetDlgItemText(IDC_EDIT_MINTRIP, _T("0.0"));
	SetDlgItemText(IDC_EDIT_MAXTRIP, _T("0.0"));

	m_ctrlReportDest.ResetContent();

	InitReportAData();
	InitReportCData();

	m_wndReportBCtrl.ResetContent();

	AllClearPath(a_bRefresh);

	m_mapEndZonePathMode.clear();
	m_vecEndZoneNodeIDCaption.clear();
	m_mapEndZoneNodeData.clear();

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(FALSE);
}

void KInterModalResultDlg::DrawOrgZoneAccess(Integer a_nxID, bool a_bRefresh)
{
	CString strNodeLable(_T(""));
	strNodeLable.Format(_T("%I64d"), a_nxID);

	std::vector<TDrawZoneAccess> vecZoneAccess;

	TDrawZoneAccess oDrawZoneAccess;
	oDrawZoneAccess.TNodeID         = a_nxID;
	oDrawZoneAccess.TSymbol			= 48;//임시 모양
	oDrawZoneAccess.TColorRGB       = RGB(255,0,0);
	oDrawZoneAccess.TLabel          = strNodeLable;
	oDrawZoneAccess.TSize           = 18.0; //임시 사이즈
	oDrawZoneAccess.TRadius         = m_dRadius;

	vecZoneAccess.push_back(oDrawZoneAccess);

	m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, a_bRefresh);
}

void KInterModalResultDlg::InitReportHeaderDest()
{
	try
	{
		CXTPReportColumns* pAColumns = m_wndReportACtrl.GetColumns();
		CXTPReportColumn*  pAColumn  = nullptr;

		CXTPReportColumns* pCColumns = m_wndReportCCtrl.GetColumns();
		CXTPReportColumn*  pCColumn  = nullptr;

		CXTPReportColumn*  pColumn   = nullptr;

		pColumn = m_ctrlReportDest.AddColumn(new CXTPReportColumn(_0_E_COLUMN_SELECT,       _T("Select"),      40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_ctrlReportDest.AddColumn(new CXTPReportColumn(_1_E_COLUMN_NAME,         _T("Destination"), 120));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("종점"));
        }

		int nIndex = _1_E_COLUMN_NAME + 1;
		for (int i= 0; i< pAColumns->GetCount(); i++)
		{
			pAColumn = pAColumns->GetAt(i);
			pAColumn->GetTooltip();

			pColumn = m_ctrlReportDest.AddColumn(new CXTPReportColumn(nIndex++, pAColumn->GetCaption(), 53));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetEditable(FALSE);
			pColumn->SetTooltip(pAColumn->GetTooltip());
			pColumn->SetAutoSize(FALSE);
		}

		for (int i= 0; i< pCColumns->GetCount(); i++)
		{
			pCColumn = pCColumns->GetAt(i);
			pCColumn->GetTooltip();

			pColumn = m_ctrlReportDest.AddColumn(new CXTPReportColumn(nIndex++, pCColumn->GetCaption(), 53));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetEditable(FALSE);
			pColumn->SetTooltip(pCColumn->GetTooltip());
			pColumn->SetAutoSize(FALSE);
		}

		m_ctrlReportDest.SetFreezeColumnsCount(2);
		m_ctrlReportDest.Populate();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KInterModalResultDlg::InitReportAHeader()
{
	try
	{
		m_mapUrbanCodeName.clear();

		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeAutoTaxi, _T("승용차/택시")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeBus, _T("버스")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeRail, _T("도시철도")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeBusRail, _T("버스+도시철도")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeParkRide, _T("Park & Ride")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeKissRide, _T("Kiss & Ride")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeTaxiTransit, _T("택시 & 대중교통")));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeAutoTaxi, _T("PassengerCar/Taxi")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeBus, _T("Bus")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeRail, _T("Rail")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeBusRail, _T("Bus+Rail")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeParkRide, _T("Park & Ride")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeKissRide, _T("Kiss & Ride")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeTaxiTransit, _T("Taxi & Transit")));
		}

		CXTPReportColumn*                pColumn      = nullptr;

		AutoType iter   = m_mapUrbanCodeName.begin();
		AutoType itEnd  = m_mapUrbanCodeName.end();

		int nColumnIdx(0);
		while (iter != itEnd)
		{
			int     nCode       = iter->first;
			CString strCodeName = iter->second;

			pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(nColumnIdx,       strCodeName,      100));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetEditable(TRUE);
			pColumn->SetTooltip(strCodeName);

			++nColumnIdx;
			++iter;
		}

		m_wndReportACtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KInterModalResultDlg::InitReportAData()
{
	try
	{
		m_wndReportACtrl.ResetContent();

		CXTPReportRecord*     pRecord   = nullptr;
		CXTPReportRecordItem* pItem     = nullptr;

		pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());

		AutoType iter  = m_mapUrbanCodeName.begin();
		AutoType itEnd = m_mapUrbanCodeName.end();

		while (iter != itEnd)
		{
			int nUrbanCode = iter->first;

			pItem = pRecord->AddItem(new CXTPReportRecordItem());

			pItem->SetEditable(TRUE);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetCaption(_T(""));
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)nUrbanCode);

			++iter;
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


void KInterModalResultDlg::InitReportCHeader()
{
	try
	{
		m_wndReportCCtrl.ResetContent();
		m_wndReportCCtrl.GetColumns()->Clear();
		m_wndReportCCtrl.Populate();

		CXTPReportColumn* pColumn      = nullptr;

		AutoType iter   = m_mapRegionalCodeName.begin();
		AutoType itEnd  = m_mapRegionalCodeName.end();

		int nColumnIdx(0);
		while (iter != itEnd)
		{
			int     nCode       = iter->first;
			CString strCodeName = iter->second;

			pColumn = m_wndReportCCtrl.AddColumn(new CXTPReportColumn(nColumnIdx,       strCodeName,      100));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetEditable(TRUE);
			pColumn->SetTooltip(strCodeName);

			++nColumnIdx;
			++iter;
		}

		m_wndReportCCtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KInterModalResultDlg::InitReportCData()
{
	try
	{
		m_wndReportCCtrl.ResetContent();

		CXTPReportRecord*     pRecord   = nullptr;
		CXTPReportRecordItem* pItem     = nullptr;

		pRecord = m_wndReportCCtrl.AddRecord(new CXTPReportRecord());

		AutoType iter  = m_mapRegionalCodeName.begin();
		AutoType itEnd = m_mapRegionalCodeName.end();

		while (iter != itEnd)
		{
			int nRegionalCode = iter->first;

			pItem = pRecord->AddItem(new CXTPReportRecordItem());

			pItem->SetEditable(TRUE);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetCaption(_T(""));
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem->SetItemData((DWORD_PTR)nRegionalCode);

			++iter;
		}

		m_wndReportCCtrl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
	}
	catch (...)
	{
		TxLogDebugException();	
	} 
}


void KInterModalResultDlg::InitReportBHeader()
{
	try
	{
		CXTPReportColumn*                pColumn      = nullptr;
		CXTPReportRecordItemEditOptions* pEditOptions = nullptr;

		m_wndReportBCtrl.SetTreeIndent(15);

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_B_COLUMN_ZONEID,       _T("Paths"),    38));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTreeColumn(TRUE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("경로 구분"));
        }

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_B_COLUMN_TOTAL_TIME,   _T("Time"),       14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("시간"));
        }

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_B_COLUMN_COST,       _T("Cost"),     14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("비용"));
        }

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_3_B_COLUMN_TRIP,         _T("Trip"),       16));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행량(인)"));
        }

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_4_B_COLUMN_DETAIL,       _T("Info"), COLUMN_WIDTH_INFO, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("상세정보"));
        }

		m_wndReportBCtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KInterModalResultDlg::InitReportColorAHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportColorACtrl.AddColumn(new CXTPReportColumn(0,       _T("mode"),           100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	
	pColumn = m_wndReportColorACtrl.AddColumn(new CXTPReportColumn(1,       _T("Color"),           50,    FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	m_wndReportColorACtrl.Populate();
}


void KInterModalResultDlg::UpdateReportColorA()
{
	try
	{
		m_wndReportColorACtrl.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = m_mapUrbanColor.begin();
		AutoType itEnd = m_mapUrbanColor.end();

		while (iter != itEnd)
		{
			TInterPathColor &oTUrbanColor = iter->second;

			pRecord = m_wndReportColorACtrl.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTUrbanColor.strName));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR) &oTUrbanColor);

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetBackgroundColor(oTUrbanColor.clrPath);
			pItem->SetEditable(TRUE);

			++iter;
		}

		m_wndReportColorACtrl.Populate();
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


void KInterModalResultDlg::InitReportColorBHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportColorBCtrl.AddColumn(new CXTPReportColumn(0,       _T("mode"),           100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReportColorBCtrl.AddColumn(new CXTPReportColumn(1,       _T("Color"),           50,    FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	m_wndReportColorBCtrl.Populate();
}


void KInterModalResultDlg::UpdateReportColorB()
{
	try
	{
		COLORREF clrStart;
		COLORREF clrEnd;
		int nSelectIndex = 5;
		switch(nSelectIndex)
		{
		case 0:
			clrStart = RGB ( 190, 207, 219 );
			clrEnd   = RGB (  84,  93, 100 );
			break;
		case 1 :
			clrStart = RGB ( 119, 221, 221 );
			clrEnd   = RGB (   8,  38,  67 );
			break;
		case 2 :
			clrStart = RGB ( 234, 158,  69 );
			clrEnd   = RGB ( 105,   2,   5 );
			break;
		case 3 :
			clrStart = RGB ( 255, 255, 102 );
			clrEnd   = RGB ( 102,   2, 153 );
			break;
		case 4 :
			clrStart = RGB ( 255, 102, 130 );
			clrEnd   = RGB (   0,  79, 153 );
		case 5 :
			clrStart = RGB ( 255, 102, 0 );
			clrEnd   = RGB (   0, 102, 255 );
			break;
		default:
			return;
			break;
		}
		int nSize = (int)m_mapRegionalColor.size();
		KColorRangeGen oColorRangeGen;
		oColorRangeGen.setStartColor(clrStart);
		oColorRangeGen.setEndColor  (clrEnd);
		oColorRangeGen.setSize(nSize);
		oColorRangeGen.createColorRange();

		m_wndReportColorBCtrl.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = m_mapRegionalColor.begin();
		AutoType itEnd = m_mapRegionalColor.end();

		KMunsellColor* pMunSellColor = new KMunsellColor(1, true);
		COLORREF       colorRef;

		int nIdx(0);
		while (iter != itEnd)
		{
			TInterPathColor &oTRegionalColor = iter->second;

			pRecord = m_wndReportColorBCtrl.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTRegionalColor.strName));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR) &oTRegionalColor);

			/*colorRef = oColorRangeGen.getNextColor();*/
			colorRef = pMunSellColor->GetColor(nIdx);
			oTRegionalColor.clrPath = colorRef;

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetBackgroundColor(colorRef);
			pItem->SetEditable(TRUE);

			++nIdx;
			++iter;
		}

		m_wndReportColorBCtrl.Populate();

		delete pMunSellColor;
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


BOOL KInterModalResultDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
		{
			ClickedEditStartNode();

			return TRUE;
		}
		else if (hwnd == GetDlgItem(IDC_EDIT2)->m_hWnd)
		{
			ClickedEditEndNode();

			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KInterModalResultDlg::ClickedEditStartNode()
{
	SelectStartNodeList();
}


void KInterModalResultDlg::OnBnClickedButtonStartNodeSearch()
{
	SelectStartNodeList();
}


void KInterModalResultDlg::ClickedEditEndNode()
{
	if (m_mapEndZonePathMode.size() < 1)
	{
		AfxMessageBox(_T("경로 관련 정보가 존재하지 않습니다."));
		return;
	}

	SelectEndNodeList();
}


void KInterModalResultDlg::OnBnClickedButtonEndNodeSearch()
{
	if (m_mapEndZonePathMode.size() < 1)
	{
		AfxMessageBox(_T("경로 관련 정보가 존재하지 않습니다."));
		return;
	}

	SelectEndNodeList();
}


void KInterModalResultDlg::SelectStartNodeList()
{
	try
	{
		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		bool bZoneNode = true;
		if (bZoneNode)
		{
			m_spImTasIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
		}
// 		Integer nxNodeID = _ttoi64(m_oStartNodeIdCaption.TID);
// 		m_spImTasIDFind->SetSelectedID(nxNodeID);
 		m_spImTasIDFind->SetSelectedID(-1);

		// 지도 선택 옵션
		m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);
		m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KInterModalResultDlg::SelectEndNodeList()
{
	try
	{
		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();
		// 항상 초기화해준다. vecIDCaption, vecID를 멤버로 갖고 있으면 좋음...
		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		m_spImTasMultiIDFind->SetIDCaption(m_vecEndZoneNodeIDCaption);
		m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapEndZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);

		m_spImTasMultiIDFind->SetSelectedID(m_strSearchedIDs);
		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::SetZoneNodeViewData(CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
{
	try
	{
		Integer nxZoneID = _ttoi64(a_strZoneID);

		AutoType iter  = m_mapZoneNodeData.find(nxZoneID);
		AutoType itEnd = m_mapZoneNodeData.end();

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


void KInterModalResultDlg::UpdateReportDestData()
{
	CWaitCursor cw;

	BOOL bAllSelected(TRUE);
	if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
		bAllSelected = TRUE;
	else
		bAllSelected = FALSE;

	try
	{
		m_ctrlReportDest.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		CString strZoneID(_T(""));
		CString strDZoneNode(_T(""));

		AutoType iter  = m_mapEndZoneNodeData.begin();
		AutoType itEnd = m_mapEndZoneNodeData.end();

		while (iter != itEnd)
		{
			Integer nxDzoneID    = iter->first;
			CString &strZoneName = iter->second;
			strZoneID.Format(_T("%I64d"), nxDzoneID);

			if (m_setSearchedID.find(strZoneID) == m_setSearchedID.end())
			{
				++iter;
				continue;
			}

			std::vector<KInterPathEditInfo>* pvecTransitAssignInfo = nullptr;

			pRecord = m_ctrlReportDest.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetChecked(bAllSelected);

			if(strZoneName.Compare(_T("")) == 0)
				strZoneName = _T("-");
			strDZoneNode.Format(_T("%I64d[%s]"), nxDzoneID, strZoneName);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strDZoneNode));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR) nxDzoneID);

			// urban
			CString strCheck(_T(""));

			AutoType iterUrban   = m_mapUrbanCodeName.begin();
			AutoType itUrbanEnd  = m_mapUrbanCodeName.end();

			while (iterUrban != itUrbanEnd)
			{
				int nCode             = iterUrban->first;
				pvecTransitAssignInfo = nullptr;
				if (KEMInterModeAutoTaxi == nCode)
				{
					pvecTransitAssignInfo = IsHaveAutoTaxiAssignData(nxDzoneID); //auto
				}
				else if (KEMInterModeBus == nCode)
				{
					pvecTransitAssignInfo = IsHaveBusAssignData(nxDzoneID); 
				}
				else if (KEMInterModeRail == nCode)
				{
					pvecTransitAssignInfo = IsHaveRailAssignData(nxDzoneID); 
				}
				else if (KEMInterModeBusRail == nCode)
				{
					pvecTransitAssignInfo = IsHaveBusRailAssignData(nxDzoneID); 
				}
				else if (KEMInterModeParkRide == nCode)
				{
					pvecTransitAssignInfo = IsHaveParkRideAssignData(nxDzoneID); 
				}
				else if (KEMInterModeKissRide == nCode)
				{
					pvecTransitAssignInfo = IsHaveKissRideAssignData(nxDzoneID); 
				}
				else if (KEMInterModeTaxiTransit == nCode)
				{
					pvecTransitAssignInfo = IsHaveTaxiTransitAssignData(nxDzoneID); 
				}

				strCheck = (pvecTransitAssignInfo) == nullptr ? _T("-") : _T("v");
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCheck));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetItemData((DWORD_PTR)pvecTransitAssignInfo);

				++iterUrban;
			}

			// regional
			std::vector<KRegionModePathInfo>* pvecRegionModePathInfo = nullptr;
			pvecRegionModePathInfo = IsHaveRegionalAssignData(nxDzoneID);
			if (nullptr == pvecRegionModePathInfo)
			{
				for (size_t i= 0; i< m_mapRegionalCodeName.size(); i++)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetItemData((DWORD_PTR)nullptr);
				}
			}
			else
			{
				for (size_t i= 0; i< (*pvecRegionModePathInfo).size(); i++) // m_mapRegionalCodeName 사이즈와 (*pvecRegionModePathInfo).size() 동일
				{
					KRegionModePathInfo &oRegionModePathInfo = (*pvecRegionModePathInfo)[i];

					if (oRegionModePathInfo.vecOnePathDetail.size() < 1)
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
						pItem->SetAlignment(DT_CENTER);
						pItem->SetItemData((DWORD_PTR)nullptr);
					}
					else
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("v")));
						pItem->SetAlignment(DT_CENTER);
						pItem->SetItemData((DWORD_PTR)&oRegionModePathInfo);
					}
				}
			}
			
			++iter;
		}

		m_ctrlReportDest.Populate();

		UpdateReportAData(TRUE);
		UpdateReportCData(TRUE);
		UpdateReportBData();
		UpdatePathCompareButtonState();

// 		QBicSimpleProgressThread::ExecuteThread(SetVolMinMaxThreadCaller, this);
// 		UpdateMinMaxTrip();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


unsigned __stdcall KInterModalResultDlg::SetVolMinMaxThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("최소/최대 통행량 값 계산 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Calculating Min/Max Trip Value..."));
	}

	KInterModalResultDlg*     pDlg = (KInterModalResultDlg*)pParameter->GetParameter();
	pDlg->SetVolMinMax();

	return 0;
}

void KInterModalResultDlg::UpdateMinMaxTrip()
{
	CString strMinTrip(_T(""));
	CString strMaxTrip(_T(""));

	double dMinTrip = m_dMinVolume - 0.5;
	if(dMinTrip < 0.0)
		dMinTrip = 0.0;

	double dMaxTrip = m_dMaxVolume + 0.6;

	strMinTrip.Format(_T("%.0f"), dMinTrip);
	strMaxTrip.Format(_T("%.0f"), dMaxTrip);

	/*SetDlgItemText(IDC_EDIT_MINTRIP, strMinTrip);*/
	SetDlgItemText(IDC_EDIT_MINTRIP, _T("0.0"));
	SetDlgItemText(IDC_EDIT_MAXTRIP, strMaxTrip);
}


void KInterModalResultDlg::InitVolMinMax()
{
	m_dMaxVolume  = 0.0;
	m_dMinVolume  = 0.0;
}


void KInterModalResultDlg::SetVolMinMax()
{
// 	std::vector<KInterPathEditInfo> vecInterAssignInfo;
// 	std::vector<TMainPathInfo>      vecMainPathInfo;  // regional mode Main 정보 수집
// 
// 	try
// 	{
// #pragma region Destination에서 KInterPathEditInfo정보 수집
// 		int nUrbanCodeCnt = (int)m_mapUrbanCodeName.size();
// 		int nRegionalCnt  = (int)m_mapRegionalCodeName.size();
// 
// 		CXTPReportRecord*         pDestRecord  = nullptr;
// 		CXTPReportRecordItem*     pItemDest    = nullptr;
// 		CXTPReportRecords*        pDestRecords = m_ctrlReportDest.GetRecords();
// 		int nDestRecordCnt      = pDestRecords->GetCount(); 
// 
// 		for (int i= 0; i< nDestRecordCnt; i++)
// 		{
// 			pDestRecord         = pDestRecords->GetAt(i);
// 			pItemDest           = pDestRecord->GetItem(_0_E_COLUMN_SELECT);
// 			/*if (pItemDest->IsChecked() == FALSE)
// 				continue; 수집 시간관계로 일단 Destination이 수집될때만 min max */ 
// 			pItemDest           = pDestRecord->GetItem(_1_E_COLUMN_NAME);
// 			Integer nxDZoneID   = (Integer)pItemDest->GetItemData();
// 
// 			AutoType itFindPathMode = m_mapEndZonePathMode.find(nxDZoneID);
// 			if (itFindPathMode == m_mapEndZonePathMode.end())
// 				continue;
// 			KEMInterModalPathMode emInterModalPathMode = itFindPathMode->second;
// 
// 			if (KEMInterModalPathModeUrban == emInterModalPathMode) // KEMInterModalPathModeRegional
// 			{
// 				int nIndex = _1_E_COLUMN_NAME + 1;
// 				for (int k= 0; k< nUrbanCodeCnt; k++)
// 				{
// 					pItemDest = pDestRecord->GetItem(nIndex++);
// 					std::vector<KInterPathEditInfo>* pvecTransitAssignInfo = (std::vector<KInterPathEditInfo>*)pItemDest->GetItemData();
// 
// 					if (nullptr == pvecTransitAssignInfo)
// 					{
// 						continue;
// 					}
// 
// 					size_t nxCount = (*pvecTransitAssignInfo).size();
// 					for (size_t m = 0; m < nxCount; m++)
// 					{
// 						KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
// 						vecInterAssignInfo.push_back(oTransitAssignInfo);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				int nIndex = _1_E_COLUMN_NAME + (int)m_mapUrbanCodeName.size() + 1;
// 				for (int k= 0; k< nRegionalCnt; k++)
// 				{
// 					pItemDest = pDestRecord->GetItem(nIndex++);
// 					KRegionModePathInfo* pRegionModePathInfo = (KRegionModePathInfo*)pItemDest->GetItemData();
// 
// 					if (nullptr == pRegionModePathInfo)
// 					{
// 						continue;
// 					}
// 
// 					std::vector<KRegionPathDetail> &vecOnePathDetail = pRegionModePathInfo->vecOnePathDetail;
// 					for (size_t m = 0; m < vecOnePathDetail.size(); m++)
// 					{
// 						KRegionPathDetail &oRegionPathDetail = vecOnePathDetail[m];
// 
// 						MakeMainPathData(pRegionModePathInfo, &oRegionPathDetail, vecMainPathInfo);
// 
// 						CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccAutoAssign,		vecInterAssignInfo);
// 						CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccBusAssign,		vecInterAssignInfo);
// 						CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccRailAssign,		vecInterAssignInfo);
// 						CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccBusRailAssign,	vecInterAssignInfo);
// 						CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrAutoAssign,		vecInterAssignInfo);
// 						CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrBusAssign,		vecInterAssignInfo);
// 						CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrRailAssign,		vecInterAssignInfo);
// 						CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrBusRailAssign,	vecInterAssignInfo);
// 					}
// 				}
// 			}
// 		}
// #pragma endregion Destination에서 KInterPathEditInfo정보 수집
// 
// 		TInMapResultInputValue oInMapResultTripInfo;
// 		KInterModalPathInfo::ResultTransitTripInfo(m_pTarget, vecInterAssignInfo, vecMainPathInfo, oInMapResultTripInfo);
// 
// 		if (oInMapResultTripInfo.mapAutoTrip.size() < 1 && 
// 			oInMapResultTripInfo.mapRegionalModeMainTrip.size() < 1 &&
// 			oInMapResultTripInfo.mapBusLinkTrip.size() < 1 && 
// 			oInMapResultTripInfo.mapSubwayLinkTrip.size() < 1)
// 		{
// 			InitVolMinMax();
// 			return;
// 		}
// 
// 		double dMaxVolume = -999999999999.0;
// 		double dMinVolume = 999999999999.0;
// 
// 		//Bus
// 		CollectMinMaxLinkTrip(oInMapResultTripInfo.mapBusLinkTrip, dMaxVolume, dMinVolume);
// 		//Rail
// 		CollectMinMaxLinkTrip(oInMapResultTripInfo.mapSubwayLinkTrip, dMaxVolume, dMinVolume);
// 
// 		std::map<KODKey, double> mapODVolume;
// 		CollectUserODVolume(oInMapResultTripInfo.mapTransferWalkingB2BTrip, mapODVolume); // 버스에서 버스로 환승 도보
// 		CollectUserODVolume(oInMapResultTripInfo.mapTransferWalkingB2STrip, mapODVolume); // 버스에서 전철로 환승 도보
// 		CollectUserODVolume(oInMapResultTripInfo.mapTransferWalkingS2STrip, mapODVolume); // 전철에서 전철로
// 		CollectUserODVolume(oInMapResultTripInfo.mapTransferWalkingA2TTrip, mapODVolume); // Auto 에서 Transit : kiss&ride 등...
// 		// oInMapResultTripInfo.mapWalkingTrip 제외
// 		CollectUserODVolume(oInMapResultTripInfo.mapTerminalConnetWalkingTrip, mapODVolume); // access or egress 에서 터미널
// 		CollectUserODVolume(oInMapResultTripInfo.mapAutoTrip, mapODVolume); // Auto Trip
// 
// 		AutoType iterA  = oInMapResultTripInfo.mapRegionalModeMainTrip.begin();
// 		AutoType itEndA = oInMapResultTripInfo.mapRegionalModeMainTrip.end();
// 		while (iterA != itEndA)
// 		{
// 			std::map<KODKey, double> &mapMainTrip = iterA->second;
// 			CollectUserODVolume(mapMainTrip, mapODVolume); // main Trip
// 			++iterA;
// 		}
// 
// 		CollectMinMaxUserODVolume(mapODVolume, dMaxVolume, dMinVolume);
// 
// 		m_dMaxVolume = dMaxVolume;
// 		m_dMinVolume = dMinVolume;
// 	}
// 	catch (...)
// 	{
// 		InitVolMinMax();
// 		TxLogDebugException();	
// 	}
}


void KInterModalResultDlg::MakeMainPathData(KRegionModePathInfo* a_pRegionModePathInfo, KRegionPathDetail* a_pRegionPathDetail, std::vector<TMainPathInfo> &a_vecMainPathInfo)
{
	try
	{
		if ( nullptr == a_pRegionModePathInfo || nullptr == a_pRegionPathDetail)
		{
			return;
		}
		COLORREF clrEmptyColor = RGB(255,255,255);
		TMainPathInfo oTMainPathInfo(a_pRegionModePathInfo->nMode, 
			a_pRegionModePathInfo->strModeName, 
			a_pRegionPathDetail->nxOrgTermID, 
			a_pRegionPathDetail->nxTransTermID, 
			a_pRegionPathDetail->nxDesTermID,
			a_pRegionPathDetail->dMainTime, 
			a_pRegionPathDetail->dMainCost, 
			a_pRegionPathDetail->dMainLength, 
			a_pRegionPathDetail->dTrip, 
			a_pRegionModePathInfo->nxOZoneID, 
			a_pRegionModePathInfo->nxDZoneID,
			clrEmptyColor);

		a_vecMainPathInfo.push_back(oTMainPathInfo);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KInterModalResultDlg::CollectRegionalModeAssignInfo(std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapRegionalModeAssign, std::set<DWORD_PTR> &a_setInterAssignInfo)
{
	try
	{
		AutoType iter  = a_mapRegionalModeAssign.begin();
		AutoType itEnd = a_mapRegionalModeAssign.end();

		for (; iter != itEnd; ++iter)
		{
			std::vector<KInterPathEditInfo> &vecAssignInfo = iter->second;
			for (size_t m = 0; m < vecAssignInfo.size(); m++)
			{
				KInterPathEditInfo &oTransitAssignInfo = vecAssignInfo[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;
				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;

				a_setInterAssignInfo.insert(dwPtr);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::CollectMinMaxLinkTrip(std::map<Integer, double> &a_mapLinkTrip, double &a_dMaxVolume, double &a_dMinVolume)
{
	try
	{
		AutoType iter  = a_mapLinkTrip.begin();
		AutoType itEnd = a_mapLinkTrip.end();

		while (iter != itEnd)
		{
			double &dTrip    = iter->second;

			if (dTrip >= 0)
			{
				if (a_dMinVolume > dTrip)
				{
					a_dMinVolume = dTrip;
				}
			}

			if (a_dMaxVolume < dTrip)
			{
				a_dMaxVolume = dTrip;
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::CollectUserODVolume(std::map<KODKey, double> &a_mapUserODTrip, std::map<KODKey, double> &a_mapODSumVolume)
{
	try
	{
		AutoType iter  = a_mapUserODTrip.begin();
		AutoType itEnd = a_mapUserODTrip.end();
		while (iter != itEnd)
		{
			KODKey  oODKey   = iter->first;
			double &dTrip    = iter->second;

			AutoType itFind = a_mapODSumVolume.find(oODKey);
			if (itFind != a_mapODSumVolume.end())
			{
				double &dSumVolume = itFind->second;
				dSumVolume += dTrip;
			}
			else
			{
				a_mapODSumVolume.insert(std::make_pair(oODKey, dTrip));
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::CollectMinMaxUserODVolume(std::map<KODKey, double> &a_mapODSumVolume, double &a_dMaxVolume, double &a_dMinVolume)
{
	try
	{
		AutoType iter     = a_mapODSumVolume.begin();
		AutoType itEnd    = a_mapODSumVolume.end();

		double dVolume(0.0);
		while (iter != itEnd)
		{
			dVolume = iter->second;

			if (dVolume >= 0)
			{
				if (a_dMinVolume > dVolume)
				{
					a_dMinVolume = dVolume;
				}
			}

			if (a_dMaxVolume < dVolume)
			{
				a_dMaxVolume = dVolume;
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::UpdateReportAData(BOOL a_bInitCheck)
{
	try
	{
		std::set<int> setSearchedUrbanCode;

		CXTPReportRecord*     pRecordDest   = nullptr;
		CXTPReportRecordItem* pItemDest     = nullptr;
		CXTPReportRecords*    pRecords      = m_ctrlReportDest.GetRecords();

		int  nDestRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nDestRecordCnt; i++)
		{
			pRecordDest = pRecords->GetAt(i);
			pItemDest   = pRecordDest->GetItem(_0_E_COLUMN_SELECT);

			if (pItemDest->IsChecked() == FALSE)
			{
				continue;
			}

			AutoType iter  = m_mapUrbanCodeName.begin();
			AutoType itEnd = m_mapUrbanCodeName.end();

			int nIndex = _1_E_COLUMN_NAME + 1;
			while (iter != itEnd)
			{
				pItemDest = pRecordDest->GetItem(nIndex++);
				std::vector<KInterPathEditInfo>* pvecTransitAssignInfo = (std::vector<KInterPathEditInfo>*)pItemDest->GetItemData();

				if (nullptr != pvecTransitAssignInfo)
				{
					std::vector<KInterPathEditInfo> &vecTransitAssignInfo = *pvecTransitAssignInfo;
					if (vecTransitAssignInfo.size() > 0)
					{
						setSearchedUrbanCode.insert(vecTransitAssignInfo[0].emInterMode);
					}
					
				}

				++iter;
			}
		}

		CXTPReportRecords* pRecordsA = m_wndReportACtrl.GetRecords();
		CXTPReportRecord*  pRecordA  = pRecordsA->GetAt(0);
		for (int i= 0; i< pRecordA->GetItemCount(); i++)
		{
			CXTPReportRecordItem* pItemA   = pRecordA->GetItem(i);
			int nUrbanCode = (int)pItemA->GetItemData();
			
			if (setSearchedUrbanCode.find(nUrbanCode) == setSearchedUrbanCode.end())
			{
				if (pItemA->GetHasCheckbox() == TRUE) // 기존에 체크박스가 있다면 없애라.
				{
					pItemA->HasCheckbox(FALSE);
					pItemA->SetChecked(FALSE);
					pItemA->SetEditable(FALSE);
					pItemA->SetCaption(_T("-"));
					pItemA->SetAlignment(DT_CENTER);
				}
			}
			else
			{
				if (pItemA->GetHasCheckbox() == FALSE) // 기존에 체크박스가 없다면 추가.
				{
					pItemA->HasCheckbox(TRUE);
					pItemA->SetChecked(a_bInitCheck);
					pItemA->SetEditable(TRUE);
					pItemA->SetCaption(_T(""));
					pItemA->SetAlignment(xtpColumnIconCenter);
				}
				else {
					pItemA->SetChecked(a_bInitCheck);
				}
			}
		}

		m_wndReportACtrl.RedrawControl();
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


void KInterModalResultDlg::UpdateReportCData(BOOL a_bInitCheck)
{
	try
	{
		std::set<int> setSearchedRegionCode;

		CXTPReportRecord*     pRecordDest   = nullptr;
		CXTPReportRecordItem* pItemDest     = nullptr;
		CXTPReportRecords*    pRecords      = m_ctrlReportDest.GetRecords();

		int  nDestRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nDestRecordCnt; i++)
		{
			pRecordDest = pRecords->GetAt(i);
			pItemDest   = pRecordDest->GetItem(_0_E_COLUMN_SELECT);

			if (pItemDest->IsChecked() == FALSE)
			{
				continue;
			}

			AutoType iter  = m_mapRegionalCodeName.begin();
			AutoType itEnd = m_mapRegionalCodeName.end();

			int nIndex = _1_E_COLUMN_NAME + (int)m_mapUrbanCodeName.size() + 1;
			while (iter != itEnd)
			{
				pItemDest = pRecordDest->GetItem(nIndex++);
				KRegionModePathInfo* oRegionModePathInfo = (KRegionModePathInfo*)pItemDest->GetItemData();

				if (nullptr != oRegionModePathInfo)
				{
					setSearchedRegionCode.insert(oRegionModePathInfo->nMode);
				}

				++iter;
			}
		}

		CXTPReportRecords* pRecordsC = m_wndReportCCtrl.GetRecords();
		CXTPReportRecord*  pRecordC  = pRecordsC->GetAt(0);
		for (int i= 0; i< pRecordC->GetItemCount(); i++)
		{
			CXTPReportRecordItem* pItemC   = pRecordC->GetItem(i);
			int nRegionalCode              = (int)pItemC->GetItemData();

			if (setSearchedRegionCode.find(nRegionalCode) == setSearchedRegionCode.end())
			{
				if (pItemC->GetHasCheckbox() == TRUE) // 기존에 체크박스가 있다면 없애라.
				{
					pItemC->HasCheckbox(FALSE);
					pItemC->SetChecked(FALSE);
					pItemC->SetEditable(FALSE);
					pItemC->SetCaption(_T("-"));
					pItemC->SetAlignment(DT_CENTER);
				}
			}
			else
			{
				if (pItemC->GetHasCheckbox() == FALSE) // 기존에 체크박스가 없다면 추가.
				{
					pItemC->HasCheckbox(TRUE);
					pItemC->SetChecked(a_bInitCheck);
					pItemC->SetEditable(TRUE);
					pItemC->SetCaption(_T(""));
					pItemC->SetAlignment(xtpColumnIconCenter);
				}
				else {
					pItemC->SetChecked(a_bInitCheck);
				}
			}
		}

		m_wndReportCCtrl.RedrawControl();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
	}
	catch (...)
	{
		TxLogDebugException();	
	} 
}


void KInterModalResultDlg::UpdateReportBData()
{
	m_wndReportBCtrl.ResetContent();

	if(nullptr != m_spInterModalResultDetailDlg)
	{
		m_spInterModalResultDetailDlg.reset();
	}

	// 2019.03.12 명지대 요청사항 수정, 2019.04.02 지역간 대화상자 추가
	if (nullptr != m_spPathCompareUrbanDlg)
	{
		m_spPathCompareUrbanDlg.reset();
	}

	if (nullptr != m_spPathCompareRegionDlg)
	{
		m_spPathCompareRegionDlg.reset();
	}
	// 2019.03.12 명지대 요청사항 수정 끝

 	//AllClearPath(false);

	CWaitCursor wc;

	std::map<int, BOOL> mapUsedTransit;
	std::map<int, BOOL> mapUsedRegional;
#pragma region mapUsedTransit, mapUsedRegional
	try
	{
		CXTPReportRecords* pARecords = m_wndReportACtrl.GetRecords();
		CXTPReportRecords* pCRecords = m_wndReportCCtrl.GetRecords();
		if (pARecords->GetCount() != 1 || pCRecords->GetCount() != 1)
		{
			m_wndReportBCtrl.Populate();
			return;
		}
		
		CXTPReportRecord* pARecord  = pARecords->GetAt(0);
		for (int k= 0; k< pARecord->GetItemCount(); k++)
		{
			CXTPReportRecordItem* pItemA = pARecord->GetItem(k);

			BOOL bUsedTransitCode(FALSE);
			if (pItemA->GetHasCheckbox() == FALSE || pItemA->IsChecked() == FALSE)
				bUsedTransitCode = FALSE;
			else
				bUsedTransitCode = TRUE;

			int nTransitCode = (int)pItemA->GetItemData();
			mapUsedTransit.insert(std::make_pair(nTransitCode, bUsedTransitCode));
		}

		CXTPReportRecord* pCRecord = pCRecords->GetAt(0);
		for (int k= 0; k< pCRecord->GetItemCount(); k++)
		{
			CXTPReportRecordItem* pItemC = pCRecord->GetItem(k);

			BOOL bUsedRegionalCode(FALSE);
			if ( pItemC->GetHasCheckbox() == FALSE || pItemC->IsChecked() == FALSE)
				bUsedRegionalCode = FALSE;
			else
				bUsedRegionalCode = TRUE;

			int nRegionalCode = (int)pItemC->GetItemData();

			mapUsedRegional.insert(std::make_pair(nRegionalCode, bUsedRegionalCode));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
#pragma endregion mapUsedTransit, mapUsedRegional

	try
	{
		CXTPReportRecord*         pParentRecord    = nullptr;
		CXTPReportRecords*        pSeParentRecords = nullptr;
		CXTPReportRecordItem*     pItem            = nullptr;
		
		CXTPReportRecords*        pDestRecords = m_ctrlReportDest.GetRecords();
		int nDestRecordCnt      = pDestRecords->GetCount(); 
		int nSelectCount(0);

		for (int k= 0; k< nDestRecordCnt; k++)
		{
			CXTPReportRecord*     pDestRecord = pDestRecords->GetAt(k);
			CXTPReportRecordItem* pItemDest   = pDestRecord->GetItem(_0_E_COLUMN_SELECT);

			if (pItemDest->IsChecked() == FALSE)
				continue;
			pItemDest           = pDestRecord->GetItem(_1_E_COLUMN_NAME);
			Integer nxDZoneID   = (Integer)pItemDest->GetItemData();

			AutoType itFindPathMode = m_mapEndZonePathMode.find(nxDZoneID);
			if (itFindPathMode == m_mapEndZonePathMode.end())
				continue;
			KEMInterModalPathMode emInterModalPathMode = itFindPathMode->second;
			
			//#1 존 ID
			pParentRecord    = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());
			CString strDZone = DisplayNodeName(nxDZoneID);
			pItem = pParentRecord->AddItem(new CXTPReportRecordItemText(strDZone));
			for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
			{
				pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
			}
			pItem->SetItemData((DWORD_PTR)nxDZoneID);
			pItem->SetAlignment(DT_LEFT);
			pItem->SetEditable(FALSE);

			//#2 수단
			pSeParentRecords = pParentRecord->GetChilds();
			if (KEMInterModalPathModeUrban == emInterModalPathMode) // KEMInterModalPathModeRegional
			{
				std::map<int, std::vector<KInterPathEditInfo>*> mapModeTransitAssignInfo;

				AutoType iter  = m_mapUrbanCodeName.begin();
				AutoType itEnd = m_mapUrbanCodeName.end();

				int nIndex = _1_E_COLUMN_NAME + 1;
				while (iter != itEnd)
				{
					pItemDest = pDestRecord->GetItem(nIndex++);
					std::vector<KInterPathEditInfo>* pvecTransitAssignInfo = (std::vector<KInterPathEditInfo>*)pItemDest->GetItemData();

					if (nullptr != pvecTransitAssignInfo)
					{
						if ((*pvecTransitAssignInfo).size() > 0)
						{
							mapModeTransitAssignInfo.insert(std::make_pair((*pvecTransitAssignInfo)[0].emInterMode, pvecTransitAssignInfo)); 
						}
					}

					++iter;
				}

				AddReportRecordUrban(pSeParentRecords, mapUsedTransit, mapModeTransitAssignInfo);
			}
			else
			{
				std::map<int, KRegionModePathInfo*> mapRegionModePathInfo;

				AutoType iter  = m_mapRegionalCodeName.begin();
				AutoType itEnd = m_mapRegionalCodeName.end();

				int nIndex = _1_E_COLUMN_NAME + (int)m_mapUrbanCodeName.size() + 1;
				while (iter != itEnd)
				{
					pItemDest = pDestRecord->GetItem(nIndex++);
					KRegionModePathInfo* pRegionModePathInfo = (KRegionModePathInfo*)pItemDest->GetItemData();
					if (nullptr != pRegionModePathInfo)
					{
						mapRegionModePathInfo.insert(std::make_pair(pRegionModePathInfo->nMode, pRegionModePathInfo));
					}

					++iter;
				}

				AddReportRecordRegional(pSeParentRecords, mapUsedRegional, mapRegionModePathInfo);
			}

			nSelectCount++;
		}

		m_wndReportBCtrl.Populate();
		if(nSelectCount < EXPANDALL_LIMIT_COUNT) // all 체크한 데이터를 모두 expandall 하기엔 부하가 걸린다.
		{
			m_wndReportBCtrl.ExpandAll(TRUE);
		}

        // 2018.08.17 명지대 요청사항 수정
        updateMinMaxEditControl();
        // 2018.08.17 명지대 요청사항 수정 끝

		if (m_setSearchedID.size() > 0) {
			DoneDraw(true);
		}
		else {
			DoneDraw(false);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
	}
	catch (...)
	{
		TxLogDebugException();	
	} 
}


CString KInterModalResultDlg::DisplayNodeName(Integer a_nxZoneID)
{
	CString strDisplayNodeName(_T(""));
	try
	{
		CString strZoneName(_T(""));
		AutoType iterName  = m_mapZoneNodeData.find(a_nxZoneID);
		AutoType itEndName = m_mapZoneNodeData.end();
		if(iterName != itEndName)
		{
			strZoneName = iterName->second;
		}
		if(strZoneName.Compare(_T("")) == 0)
			strZoneName = _T("-");

		strDisplayNodeName.Format(_T("%I64d[%s]"), a_nxZoneID, strZoneName);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return strDisplayNodeName;
}


void KInterModalResultDlg::AddReportRecordUrban(CXTPReportRecords* a_pSeParentRecords, std::map<int, BOOL> &a_mapUsedTransit, std::map<int, std::vector<KInterPathEditInfo>*> &a_mapModeTransitAssignInfo)
{
	try
	{
		CXTPReportRecord*         pSeParentRecord  = nullptr;
		CXTPReportRecords*        pThParentRecords = nullptr;
		CXTPReportRecord*         pThParentRecord  = nullptr;
		CXTPReportRecordItem*     pItem            = nullptr;
		CXTPReportRecordItemText* pItemText        = nullptr;
		CXTPReportRecordItem*     pParentCheckItem = nullptr;

		AutoType iterB  = a_mapUsedTransit.begin();
		AutoType itEndB = a_mapUsedTransit.end();
		while (iterB != itEndB)
		{
			int  nTransitCode = iterB->first;
			BOOL bUsedTransit = iterB->second;

			if (FALSE == bUsedTransit)
			{
				++iterB;
				continue;
			}

			CString strType(_T(""));
			std::vector<KInterPathEditInfo>* pvecTransitAssignInfo = nullptr;

			AutoType itFind = m_mapUrbanCodeName.find(nTransitCode);
			strType = itFind->second;

			AutoType itFindAssign = a_mapModeTransitAssignInfo.find(nTransitCode);
			if (itFindAssign == a_mapModeTransitAssignInfo.end())
			{
				++iterB;
				continue;
			}
			pvecTransitAssignInfo = itFindAssign->second;

			if (nullptr == pvecTransitAssignInfo)
			{
				++iterB;
				continue;
			}

			//#2 Type
			pSeParentRecord  = a_pSeParentRecords->Add(new CXTPReportRecord());
			pParentCheckItem = pSeParentRecord->AddItem(new CXTPReportRecordItemText(strType));
			for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
			{
				CXTPReportRecordItem*            pItemEmpty   = pSeParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
				pEditOptions->RemoveButtons();
				pItemEmpty->SetEditable(FALSE);

			}
			pParentCheckItem->SetAlignment(DT_LEFT);
			pParentCheckItem->HasCheckbox(TRUE);
			pParentCheckItem->SetChecked(TRUE);

// 			if (nullptr == pvecTransitAssignInfo)
// 			{
// 				++iterB;
// 				continue;
// 			}

			pThParentRecords = pSeParentRecord->GetChilds();

			for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++)
			{
				KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];

				pThParentRecord = pThParentRecords->Add(new CXTPReportRecord());
				//#3
				CString strDisplay(_T(""));
				CString strIndex(_T(""));

				strIndex.Format(_T("%d"),     m+1);
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    strDisplay.Format(_T("경로%d"), m+1);
                }
                else {
                    strDisplay.Format(_T("#P%d"), m+1);
                }				

				pItemText = (CXTPReportRecordItemText*)pThParentRecord->AddItem(new CXTPReportRecordItemText());
				pItemText->SetAlignment(DT_LEFT);
				pItemText->HasCheckbox(TRUE);
				pItemText->SetChecked(TRUE);
				pItemText->SetItemData((DWORD_PTR)&oTransitAssignInfo);
				pItemText->SetValue(strIndex);
				pItemText->SetCaption(strDisplay);

				//#4
				double dCost(0.0);
				double dTotalTime(0.0);
				double dLength(0.0);
				double dTrip(0.0);

				GetPathCost(oTransitAssignInfo, dTotalTime, dCost, dLength, dTrip);

				pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

// 				pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dLength, _T("%.2f")));
// 				pItem->SetAlignment(DT_CENTER);
				pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(dTrip, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				//#5
				pItem = pThParentRecord->AddItem(new CXTPReportRecordItem());
				CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				pButton->SetAlignment(xtpItemControlUnknown);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("보기"));
					pButton->SetSize(CSize(COLUMN_WIDTH_INFO, 0));
				}
				else {
					pButton->SetCaption(_T("Info"));
					pButton->SetSize(CSize(COLUMN_WIDTH_INFO, 0));
				}
				int nInterModalPathMode = KEMInterModalPathModeUrban;
				pItem->SetItemData((DWORD_PTR)nInterModalPathMode);
			}

			++iterB;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::GetPathCost(KInterPathEditInfo &oTransitAssignInfo, double &a_dTotalTime, double &a_dCost, double &a_dLength, double &a_dTrip)
{
	try
	{
		if(KEMInterModeAutoTaxi == oTransitAssignInfo.emInterMode)
		{
			a_dCost            = oTransitAssignInfo.oTAutoSubData.dAutoCost;
			if (a_dCost < oTransitAssignInfo.oTAutoSubData.dTaxiCost )
			{
				a_dCost        = oTransitAssignInfo.oTAutoSubData.dTaxiCost;
			}
			a_dLength          = oTransitAssignInfo.oTAutoSubData.dTravelLength;
			a_dTotalTime       = oTransitAssignInfo.oTAutoSubData.dTravelTime;

			a_dTrip = oTransitAssignInfo.oTAutoSubData.dTrip;
		}
		else if(KEMInterModeBus == oTransitAssignInfo.emInterMode || KEMInterModeBusRail == oTransitAssignInfo.emInterMode)
		{
			a_dCost            = oTransitAssignInfo.oTBusSubData.dCost;
			a_dLength          = oTransitAssignInfo.oTBusSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTBusSubData.dInVehTime
				+ oTransitAssignInfo.oTBusSubData.dWaitTime 
				+ oTransitAssignInfo.oTBusSubData.dTransferTime 
				+ oTransitAssignInfo.oTBusSubData.dAccessWalkTime 
				+ oTransitAssignInfo.oTBusSubData.dEgressWalkTime;

			a_dTrip = oTransitAssignInfo.oTBusSubData.dTrip;
		}
		else if(KEMInterModeRail == oTransitAssignInfo.emInterMode)
		{
			a_dCost            = oTransitAssignInfo.oTRailSubData.dCost;
			a_dLength          = oTransitAssignInfo.oTRailSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTRailSubData.dInVehTime
				+ oTransitAssignInfo.oTRailSubData.dWaitTime 
				+ oTransitAssignInfo.oTRailSubData.dTransferTime 
				+ oTransitAssignInfo.oTRailSubData.dAccessWalkTime 
				+ oTransitAssignInfo.oTRailSubData.dEgressWalkTime;

			a_dTrip = oTransitAssignInfo.oTRailSubData.dTrip;
		}
		else if(KEMInterModeParkRide == oTransitAssignInfo.emInterMode|| KEMInterModeKissRide == oTransitAssignInfo.emInterMode|| KEMInterModeTaxiTransit == oTransitAssignInfo.emInterMode)
		{
			a_dCost            = oTransitAssignInfo.oTMultiSubData.dVehCost + oTransitAssignInfo.oTMultiSubData.dTransitCost;
			a_dLength          = oTransitAssignInfo.oTMultiSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTMultiSubData.dVehTime
				+ oTransitAssignInfo.oTMultiSubData.dTravelTime
				+ oTransitAssignInfo.oTMultiSubData.dWaitTime
				+ oTransitAssignInfo.oTMultiSubData.dTransferTime
				+ oTransitAssignInfo.oTMultiSubData.dAccessWalkTime
				+ oTransitAssignInfo.oTMultiSubData.dEgressWalkTime;

			a_dTrip = oTransitAssignInfo.oTMultiSubData.dTrip;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::AddReportRecordRegional(CXTPReportRecords* a_pSeParentRecords, std::map<int, BOOL> &a_mapUsedRegional, std::map<int, KRegionModePathInfo*> &a_mapRegionModePathInfo)
{
	try
	{
		CXTPReportRecord*         pSeParentRecord  = nullptr;
		CXTPReportRecords*        pThParentRecords = nullptr;
		CXTPReportRecord*         pThParentRecord  = nullptr;
		CXTPReportRecords*        pFrParentRecords = nullptr;

		CXTPReportRecord*         pFrParentRecord  = nullptr;
		CXTPReportRecords*        pFvParentRecords = nullptr;

		CXTPReportRecordItem*     pItem            = nullptr;
		CXTPReportRecordItemText* pItemText        = nullptr;
		CXTPReportRecordItem*     pParentCheckItem = nullptr;

		AutoType iterB  = a_mapUsedRegional.begin();
		AutoType itEndB = a_mapUsedRegional.end();
		while (iterB != itEndB)
		{
			int  nRegionalCode      = iterB->first;
			BOOL bUsedRegionalCode  = iterB->second;

			if (FALSE == bUsedRegionalCode)
			{
				++iterB;
				continue;
			}

			AutoType itFindAssign = a_mapRegionModePathInfo.find(nRegionalCode);
			if (itFindAssign == a_mapRegionModePathInfo.end())
			{
				++iterB;
				continue;
			}

			KRegionModePathInfo* pRegionModePathInfo = itFindAssign->second;
			if (nullptr == pRegionModePathInfo)
			{
				++iterB;
				continue;
			}

			CString strTypeName = pRegionModePathInfo->strModeName;
			int     nRegionMode = pRegionModePathInfo->nMode;

			pSeParentRecord  = a_pSeParentRecords->Add(new CXTPReportRecord());
			pParentCheckItem = pSeParentRecord->AddItem(new CXTPReportRecordItemText(strTypeName));

			for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
			{
				CXTPReportRecordItem*            pItemEmpty   = pSeParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
				pEditOptions->RemoveButtons();
				pItemEmpty->SetEditable(FALSE);
			}
			pParentCheckItem->SetAlignment(DT_LEFT);
			pParentCheckItem->HasCheckbox(TRUE);
			pParentCheckItem->SetItemData((DWORD_PTR)pRegionModePathInfo);
			if (REGION_MODE_CODE_AUTO == nRegionMode)
			{
				//pParentCheckItem->SetChecked(pItemC->IsChecked());
				pParentCheckItem->SetChecked(TRUE);
			}
			else
			{
				pParentCheckItem->SetChecked(TRUE);
			}

			//#1-2 path num
			pThParentRecords = pSeParentRecord->GetChilds();

			std::vector<KRegionPathDetail> &vecOnePathDetail = pRegionModePathInfo->vecOnePathDetail;
			for (size_t m = 0; m < vecOnePathDetail.size(); m++)
			{
				KRegionPathDetail &oRegionPathDetail = vecOnePathDetail[m];

				pThParentRecord = pThParentRecords->Add(new CXTPReportRecord());

				CString strDisplay(_T(""));
				CString strIndex(_T(""));

				strIndex.Format(_T("%d"),     m+1);
				pItemText = (CXTPReportRecordItemText*)pThParentRecord->AddItem(new CXTPReportRecordItemText());
				for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
				{
					CXTPReportRecordItem*            pItemEmpty   = pThParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
					CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
					pEditOptions->RemoveButtons();
					pItemEmpty->SetEditable(FALSE);
				}
				pItemText->SetAlignment(DT_LEFT);
				pItemText->HasCheckbox(TRUE);
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    strDisplay.Format(_T("경로%d"), m+1);
                }
                else {
                    strDisplay.Format(_T("#P%d"), m+1);
                }
				
				pItemText->SetItemData((DWORD_PTR)&oRegionPathDetail);
				pItemText->SetValue(strIndex);
				pItemText->SetCaption(strDisplay);
				if (REGION_MODE_CODE_AUTO == nRegionMode)
				{
					//pItemText->SetChecked(pItemC->IsChecked());
					pItemText->SetChecked(TRUE);
				}
				else
				{
					pItemText->SetChecked(TRUE);
				}

				//#1-3 acc / egr
				pFrParentRecords = pThParentRecord->GetChilds();

				for (int n= 0; n< 3; n++)
				{
					CString strAccessEgress;
					if (0 == n) {
						if (KmzSystem::GetLanguage() == KEMKorea) {
							strAccessEgress = _T("출발 접근");
						}
						else {
							strAccessEgress = _T("Access");
						}
					}
					else if (1 == n) {
						if (KmzSystem::GetLanguage() == KEMKorea) {
							strAccessEgress = _T("주요 경로");
						}
						else {
							strAccessEgress = _T("Main");
						}
					}
					else {
						if (KmzSystem::GetLanguage() == KEMKorea) {
							strAccessEgress = _T("도착 접근");
						}
						else {
							strAccessEgress = _T("Egress");
						}
					}

					if (REGION_MODE_CODE_AUTO == nRegionMode)
					{
						if (0 == n || 2 == n)
						{
							continue;
						}
					}

					pFrParentRecord = pFrParentRecords->Add(new CXTPReportRecord());
					pItemText = (CXTPReportRecordItemText*)pFrParentRecord->AddItem(new CXTPReportRecordItemText(strAccessEgress));

					if (0 == n || 2 == n)// access / egress
					{	
						for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
						{
							CXTPReportRecordItem*            pItemEmpty   = pFrParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
							CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
							pEditOptions->RemoveButtons();
							pItemEmpty->SetEditable(FALSE);
						}
						pItemText->SetAlignment(DT_LEFT);
						pItemText->HasCheckbox(TRUE);
						pItemText->SetChecked(TRUE);

						// #1-4 mode
						pFvParentRecords = pFrParentRecord->GetChilds();
						if (0 == n)
						{
							if (KmzSystem::GetLanguage() == KEMKorea) {
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapAccAutoAssign,    _T("승용차"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapAccBusAssign,     _T("버스"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapAccRailAssign,    _T("도시철도"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapAccBusRailAssign, _T("버스+도시철도"));
							}
							else {
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapAccAutoAssign,    _T("PassengerCar"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapAccBusAssign,     _T("Bus"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapAccRailAssign,    _T("Rail"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapAccBusRailAssign, _T("Bus+Rail"));
							}
						}
						else if (2 == n)
						{
							if (KmzSystem::GetLanguage() == KEMKorea) {
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapEgrAutoAssign,    _T("승용차"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapEgrBusAssign,     _T("버스"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapEgrRailAssign,    _T("도시철도"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapEgrBusRailAssign, _T("버스+도시철도"));
							}
							else {
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapEgrAutoAssign,    _T("PassengerCar"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapEgrBusAssign,     _T("Bus"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapEgrRailAssign,    _T("Rail"));
								RegionalReportSurport(pFvParentRecords, oRegionPathDetail.mapEgrBusRailAssign, _T("Bus+Rail"));
							}
						}
					}
					else // main
					{
						pItemText->SetEditable(FALSE);

						double dTotalTime = oRegionPathDetail.dMainTime 
										  + oRegionPathDetail.dMainWaitTime 
										  + oRegionPathDetail.dMainTransferTime 
										  + oRegionPathDetail.dMainProcessTime; 
// 						pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainTime,   _T("%.2f")));
// 						pItem->SetAlignment(DT_CENTER);
						pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime,   _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);

// 						pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainWaitTime,     _T("%.2f")));
// 						pItem->SetAlignment(DT_CENTER);
// 						pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainTransferTime, _T("%.2f")));
// 						pItem->SetAlignment(DT_CENTER);
// 						pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainProcessTime,  _T("%.2f")));
// 						pItem->SetAlignment(DT_CENTER);
// 						pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainFrequency,    _T("%.2f")));
// 						pItem->SetAlignment(DT_CENTER);

						pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainCost,   _T("%.2f")));
						//pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainLength, _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);

						pItem = pFrParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dTrip,       _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);

						CXTPReportRecordItem*            pItemEmpty   = pFrParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
						CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
						pEditOptions->RemoveButtons();
						pItemEmpty->SetEditable(FALSE);
					}
				}
			}

			++iterB;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::RegionalReportSurport(CXTPReportRecords* a_pRecords, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapAccAutoAssign, CString a_strTypeName)
{
	try
	{
		CXTPReportRecord*         pParentRecord    = nullptr;
		CXTPReportRecords*        pSeParentRecords = nullptr;
		CXTPReportRecord*         pSeParentRecord  = nullptr;
		CXTPReportRecordItem*     pItem            = nullptr;
		CXTPReportRecordItemText* pItemText        = nullptr;
		CXTPReportRecordItem*     pParentCheckItem = nullptr;

		AutoType iter  = a_mapAccAutoAssign.begin();
		AutoType itEnd = a_mapAccAutoAssign.end();

		while (iter != itEnd)
		{

			std::vector<KInterPathEditInfo> &vecAssignInfo = iter->second;
			if (vecAssignInfo.size() < 1)
			{
				++iter;
				continue;
			}

			pParentRecord = a_pRecords->Add(new CXTPReportRecord());

			//#1 Type
			pParentCheckItem         = pParentRecord->AddItem(new CXTPReportRecordItemText(a_strTypeName));
			for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
			{
				CXTPReportRecordItem*            pItemEmpty   = pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
				pEditOptions->RemoveButtons();
				pItemEmpty->SetEditable(FALSE);
			}
			pParentCheckItem->SetAlignment(DT_LEFT);
			pParentCheckItem->HasCheckbox(TRUE);
			pParentCheckItem->SetChecked(TRUE);

			CXTPReportRecord* pTempParentRecord = pParentRecord;
			for (int nDepth= 0; nDepth< 3; nDepth++)
			{
				pTempParentRecord    = pTempParentRecord->GetParentRecord();
				pParentCheckItem     = pTempParentRecord->GetItem(_0_B_COLUMN_ZONEID);
				pParentCheckItem->SetChecked(TRUE);
			}

			pSeParentRecords = pParentRecord->GetChilds();

			for (size_t m = 0; m < vecAssignInfo.size(); m++)
			{
				KInterPathEditInfo &oTransitAssignInfo = vecAssignInfo[m];

				pSeParentRecord = pSeParentRecords->Add(new CXTPReportRecord());

				//#2
				CString strDisplay(_T(""));
				CString strIndex(_T(""));

				strIndex.Format(_T("%d"),     m+1);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strDisplay.Format(_T("경로%d"), m+1);
				}
				else {
					strDisplay.Format(_T("#p%d"), m+1);
				}	

				pItemText = (CXTPReportRecordItemText*)pSeParentRecord->AddItem(new CXTPReportRecordItemText());
				pItemText->SetAlignment(DT_LEFT);
				pItemText->HasCheckbox(TRUE);
				pItemText->SetChecked(TRUE);
				pItemText->SetItemData((DWORD_PTR)&oTransitAssignInfo);
				pItemText->SetValue(strIndex);
				pItemText->SetCaption(strDisplay);

				//#3
				double dCost(0.0);
				double dTotalTime(0.0);
				double dLength(0.0);

				double dWaitTime(0.0);
				double dTransferTime(0.0);
				double dProcessTime(0.0);
				double dFrequency(0.0);

				double dTrip(0.0);

				GetRegionPathCost(oTransitAssignInfo, dTotalTime, dCost, dLength, dWaitTime, dTransferTime, dProcessTime, dFrequency, dTrip);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

// 				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dWaitTime,     _T("%.2f")));
// 				pItem->SetAlignment(DT_CENTER);   
// 				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dTransferTime, _T("%.2f")));
// 				pItem->SetAlignment(DT_CENTER);
// 				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dProcessTime,  _T("%.2f")));
// 				pItem->SetAlignment(DT_CENTER);
// 				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dFrequency,    _T("%.2f")));
// 				pItem->SetAlignment(DT_CENTER);

// 				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dLength, _T("%.2f")));
// 				pItem->SetAlignment(DT_CENTER);
				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dCost,   _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dTrip,   _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				//#4
				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItem());
				CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
				pButton->SetAlignment(xtpItemControlUnknown);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("보기"));
					pButton->SetSize(CSize(COLUMN_WIDTH_INFO, 0));
				}
				else {
					pButton->SetCaption(_T("Info"));
					pButton->SetSize(CSize(COLUMN_WIDTH_INFO, 0));
				}
				int nInterModalPathMode = KEMInterModalPathModeRegional;
				pItem->SetItemData((DWORD_PTR)nInterModalPathMode);
			}

			++iter;
		}
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


void KInterModalResultDlg::GetRegionPathCost(KInterPathEditInfo &oTransitAssignInfo, double &a_dTotalTime, double &a_dCost, double &a_dLength, 
												double &a_dWaitTime, double &a_dTransferTime, double &a_dProcessTime, double &a_dFrequency, double &a_dTrip)
{
	try
	{
		if(KEMInterRegDetailModeAuto == oTransitAssignInfo.emInterRegDetailMode)
		{
			a_dCost            = oTransitAssignInfo.oTRegAutoSubData.dAutoCost;
			a_dLength          = 0.0;
			a_dTotalTime       = oTransitAssignInfo.oTRegAutoSubData.dAutoTime;
			a_dWaitTime        = 0.0;
			a_dTransferTime    = 0.0;
			a_dProcessTime     = 0.0;
			a_dFrequency       = 0.0;
			a_dTrip            = oTransitAssignInfo.oTRegAutoSubData.dTrip;
		}
		else if(KEMInterRegDetailModeBus == oTransitAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oTransitAssignInfo.emInterRegDetailMode)
		{
			a_dCost            = oTransitAssignInfo.oTBusSubData.dCost;
			a_dLength          = oTransitAssignInfo.oTBusSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTBusSubData.dInVehTime
				+ oTransitAssignInfo.oTBusSubData.dWaitTime 
				+ oTransitAssignInfo.oTBusSubData.dTransferTime 
				+ oTransitAssignInfo.oTBusSubData.dAccessWalkTime 
				+ oTransitAssignInfo.oTBusSubData.dEgressWalkTime;
			a_dWaitTime        = oTransitAssignInfo.oTBusSubData.dWaitTime;
			a_dTransferTime    = oTransitAssignInfo.oTBusSubData.dTransferTime;
			a_dProcessTime     = 0.0;
			a_dFrequency       = 0.0;
			a_dTrip            = oTransitAssignInfo.oTBusSubData.dTrip;
		}
		else if(KEMInterRegDetailModeRail == oTransitAssignInfo.emInterRegDetailMode)
		{
			a_dCost            = oTransitAssignInfo.oTRailSubData.dCost;
			a_dLength          = oTransitAssignInfo.oTRailSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTRailSubData.dInVehTime
				+ oTransitAssignInfo.oTRailSubData.dWaitTime 
				+ oTransitAssignInfo.oTRailSubData.dTransferTime 
				+ oTransitAssignInfo.oTRailSubData.dAccessWalkTime 
				+ oTransitAssignInfo.oTRailSubData.dEgressWalkTime;
			a_dWaitTime        = oTransitAssignInfo.oTRailSubData.dWaitTime;
			a_dTransferTime    = oTransitAssignInfo.oTRailSubData.dTransferTime;
			a_dProcessTime     = 0.0;
			a_dFrequency       = 0.0;
			a_dTrip            = oTransitAssignInfo.oTRailSubData.dTrip;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::FindPath()
{
	m_bResult = true;
	m_strErrMsg =_T("");
	
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}
	
	QBicSimpleProgressThread::ExecuteThread(CollectDataThreadCaller, this, false, nLang);

	if (!m_bResult)
	{
		AfxMessageBox(m_strErrMsg);
	}

	try
	{
		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = m_mapZoneNodeData.begin();
		AutoType itEnd = m_mapZoneNodeData.end();

		m_mapEndZonePathMode.clear();
		m_vecEndZoneNodeIDCaption.clear();
		m_mapEndZoneNodeData.clear();

		while(iter != itEnd)
		{
			Integer nxDzoneID    = iter->first;
			CString strNodeName  = iter->second;
			
			if (nullptr != IsHaveRegionalAssignData(nxDzoneID))
				m_mapEndZonePathMode.insert(std::make_pair(nxDzoneID, KEMInterModalPathModeRegional));
			else if (nullptr != IsHaveAutoTaxiAssignData(nxDzoneID))
				m_mapEndZonePathMode.insert(std::make_pair(nxDzoneID, KEMInterModalPathModeUrban));
			else if (nullptr != IsHaveBusAssignData(nxDzoneID))
				m_mapEndZonePathMode.insert(std::make_pair(nxDzoneID, KEMInterModalPathModeUrban));
			else if (nullptr != IsHaveRailAssignData(nxDzoneID))
				m_mapEndZonePathMode.insert(std::make_pair(nxDzoneID, KEMInterModalPathModeUrban));
			else if (nullptr != IsHaveBusRailAssignData(nxDzoneID))
				m_mapEndZonePathMode.insert(std::make_pair(nxDzoneID, KEMInterModalPathModeUrban));
			else if (nullptr != IsHaveParkRideAssignData(nxDzoneID))
				m_mapEndZonePathMode.insert(std::make_pair(nxDzoneID, KEMInterModalPathModeUrban));
			else if (nullptr != IsHaveKissRideAssignData(nxDzoneID))
				m_mapEndZonePathMode.insert(std::make_pair(nxDzoneID, KEMInterModalPathModeUrban));
			else if (nullptr != IsHaveTaxiTransitAssignData(nxDzoneID))
				m_mapEndZonePathMode.insert(std::make_pair(nxDzoneID, KEMInterModalPathModeUrban));
			else
			{
				++iter;
				continue;
			}

			KIDCaption oIDCaption;
			CString strNodeID(_T(""));
			strNodeID.Format(_T("%I64d"), nxDzoneID);
			oIDCaption.TID      = strNodeID;
			oIDCaption.TCaption = strNodeName;

			m_vecEndZoneNodeIDCaption.push_back(oIDCaption);
			m_mapEndZoneNodeData.insert(std::make_pair(nxDzoneID, strNodeName));
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		m_vecEndZoneNodeIDCaption.clear();
		m_mapEndZoneNodeData.clear();
	}
	catch (...)
	{
		TxLogDebugException();	
		m_vecEndZoneNodeIDCaption.clear();
		m_mapEndZoneNodeData.clear();
	} 
}


unsigned __stdcall KInterModalResultDlg::CollectDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("복합수단 이용경로 통행량 데이터 로딩 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Intermodal Assign Result Data..."));
	}

	KInterModalResultDlg* pDlg = (KInterModalResultDlg*)pParameter->GetParameter();
	pDlg->CollectData();

	return 0;
}


void KInterModalResultDlg::CollectData()
{
	TxLogDebugStartMsg();
	try
	{
		if ( !ReadOutPutFile() )
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Saving Output Data Failed."));
			}
			ThrowException(_T("Output 정보 입력 실패"));
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

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KInterModalResultDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();

	Integer nxNodeID = GetSelStartNodeId();
	if(-1 == nxNodeID)
	{
		return false;
	}

	CString strDataFileName(_T(""));
	CString strSavePath(_T(""));

	try
	{
		strDataFileName.Format(_T("%s%I64d_AutoTaxiPath.dat"), INTERMODAL_RESULT_PATH,  nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
		ReadAutoTaxiPathFile(strSavePath);

		strDataFileName.Format(_T("%s%I64d_BusPath.dat"), INTERMODAL_RESULT_PATH, nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
		ReadBusPathFile(strSavePath);

		strDataFileName.Format(_T("%s%I64d_RailPath.dat"), INTERMODAL_RESULT_PATH, nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(),DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
		ReadRailPathFile(strSavePath);

		strDataFileName.Format(_T("%s%I64d_BusRailPath.dat"), INTERMODAL_RESULT_PATH, nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
		ReadBusRailPathFile(strSavePath);

		strDataFileName.Format(_T("%s%I64d_ParkandRidePath.dat"), INTERMODAL_RESULT_PATH, nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
		ReadParkRidePathFile(strSavePath);

		strDataFileName.Format(_T("%s%I64d_KissandRidePath.dat"), INTERMODAL_RESULT_PATH, nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
		ReadKissRidePathFile(strSavePath);

		strDataFileName.Format(_T("%s%I64d_TaxiandTransitPath.dat"), INTERMODAL_RESULT_PATH, nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
		ReadTaxiTransitPathFile(strSavePath);

		strDataFileName.Format(_T("%s%I64d_RegionalPath.dat"), INTERMODAL_RESULT_PATH, nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
		ReadRegionalPathFile(strSavePath);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
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


Integer KInterModalResultDlg::GetSelStartNodeId()
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


void KInterModalResultDlg::ReadAutoTaxiPathFile( CString a_strFileName )
{
	TxLogDebugStartMsg();
	m_mapAutoTaxiAssign.clear();
	std::ifstream ifs( a_strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOrginID(0);
				Integer nxDestinationID(0);

				ifs.read( reinterpret_cast<char*>( &(nxOrginID)),                             sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );

				KInterPathEditInfo oAutoAssignInfo;
				oAutoAssignInfo.emInterModalPathMode = KEMInterModalPathModeUrban;
				oAutoAssignInfo.emInterMode         = KEMInterModeAutoTaxi;
				oAutoAssignInfo.nxOrginID           = nxOrginID;
				oAutoAssignInfo.nxDestinationID     = nxDestinationID;

				ifs.read( reinterpret_cast<char*>( &(oAutoAssignInfo.oTAutoSubData)),             sizeof(TInEditAutoSubData) );

				ifs.read( reinterpret_cast<char*>( &(oAutoAssignInfo.nUseFlag)),                  sizeof(int) );

#pragma region 취합 정보 등록
				AutoType iter  = m_mapAutoTaxiAssign.find(oAutoAssignInfo.nxDestinationID);
				AutoType itEnd = m_mapAutoTaxiAssign.end();

				if( iter != itEnd )
				{
					std::vector<KInterPathEditInfo> &vecAutoAssignInfo = iter->second;
					vecAutoAssignInfo.push_back(oAutoAssignInfo);
				}
				else
				{
					std::vector<KInterPathEditInfo> vecAutoAssignInfo;
					vecAutoAssignInfo.push_back(oAutoAssignInfo);
					m_mapAutoTaxiAssign.insert(std::make_pair(oAutoAssignInfo.nxDestinationID, vecAutoAssignInfo));
				}
#pragma endregion 취합 정보 등록

				nCount++;
			}

// 			CString strResult = _T("");
// 			strResult.Format(_T("--- end insert result size : %d"), nCount);
// 			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapAutoTaxiAssign.clear();
		ifs.close();

		m_bResult = false;
		m_strErrMsg =_T("ReadAutoTaxiPathFile 오류");

		/*ThrowException(_T("ReadAutoTaxiPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadBusPathFile(CString strFileName)
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
				Integer nxOrginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOrginID)),                             sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),                            sizeof(int) );

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 정보 취합
					KInterPathEditInfo oBusAssignInfo;
					oBusAssignInfo.emInterModalPathMode = KEMInterModalPathModeUrban;
					oBusAssignInfo.emInterMode         = KEMInterModeBus;
					oBusAssignInfo.nxOrginID           = nxOrginID;
					oBusAssignInfo.nxDestinationID     = nxDestinationID;

					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nPathID)),                sizeof(int) );
					if(ifs.eof())break;

					std::vector< TInIndexResultTransitInfo > &vecResulTransitInfo = oBusAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< BUSPATH_INTER_ASSIGNSET_COUNT; i++)
					{
						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						oTIndexResultTransitInfo.oIndexKey = KODKey((i+1),0);
						std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),      sizeof(Integer) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),    sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),      sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.dTrip)),            sizeof(double));

							if(0 == oTInResultBusInfo.nxTransitID)
								continue;

							vecLowResultTransitInfo.push_back(oTInResultBusInfo);
						}
						vecResulTransitInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.oTBusSubData)),               sizeof(TInEditBusSubData) );
					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nUseFlag)),                   sizeof(int) );
#pragma endregion 정보 취합

#pragma region 취합 정보 등록
					AutoType iter  = m_mapBusAssign.find(oBusAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapBusAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusAssignInfo = iter->second;
						vecBusAssignInfo.push_back(oBusAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusAssignInfo;
						vecBusAssignInfo.push_back(oBusAssignInfo);
						m_mapBusAssign.insert(std::make_pair(oBusAssignInfo.nxDestinationID, vecBusAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}

				nCount++;
			}

// 			CString strResult = _T("");
// 			strResult.Format(_T("--- end insert result size : %d"), nCount);
// 			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapBusAssign.clear();
		ifs.close();

		m_bResult = false;
		m_strErrMsg =_T("ReadBusPathFile 오류");

		/*ThrowException(_T("ReadBusPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadRailPathFile(CString strFileName)
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
				Integer nxOriginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 정보 취합
					KInterPathEditInfo oRailAssignInfo;
					oRailAssignInfo.emInterModalPathMode = KEMInterModalPathModeUrban;
					oRailAssignInfo.emInterMode         = KEMInterModeRail;
					oRailAssignInfo.nxOrginID           = nxOriginID;
					oRailAssignInfo.nxDestinationID     = nxDestinationID;

					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nPathID)),                sizeof(int) );
					if(ifs.eof())break;

					int nBoardingCnt(0); //탑승횟수
					ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

					if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					std::vector< TInIndexResultTransitInfo > &vecResulTransitInfo = oRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< nBoardingCnt; i++)
					{
						int nDuplicateCnt(0); //중복 노선 갯수
						ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
						if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						oTIndexResultTransitInfo.oIndexKey = KODKey((1+i),0);
						std::vector<TInResultTransitInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< nDuplicateCnt; k++)
						{
							TInResultTransitInfo oTInResultRailInfo = {0,0,0,0.0, KEMInPathTypeRail, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nxTransitID)),      sizeof(Integer) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.dTrip)),            sizeof(double) );

							if(0 == oTInResultRailInfo.nxTransitID)
								continue;

							vecLowResultBusInfo.push_back(oTInResultRailInfo);
						}
						vecResulTransitInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.oTRailSubData)),             sizeof(TInEditRailSubData) );
					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nUseFlag)),                  sizeof(int) );
#pragma endregion 정보 취합

#pragma region 취합 정보 등록
					AutoType iter  = m_mapRailAssign.find(oRailAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapRailAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecRailAssignInfo = iter->second;
						vecRailAssignInfo.push_back(oRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecRailAssignInfo;
						vecRailAssignInfo.push_back(oRailAssignInfo);
						m_mapRailAssign.insert(std::make_pair(oRailAssignInfo.nxDestinationID, vecRailAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}
				nCount++;
			}

// 			CString strResult = _T("");
// 			strResult.Format(_T("--- end insert result size : %d"), nCount);
// 			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapRailAssign.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadRailPathFile 오류");
		/*ThrowException(_T("ReadRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadBusRailPathFile(CString strFileName)
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
				Integer nxOriginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KInterPathEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emInterModalPathMode = KEMInterModalPathModeUrban;
					oBusRailAssignInfo.emInterMode         = KEMInterModeBusRail;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					if(ifs.eof())break;

					std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< BUSRAILPATH_INTER_ASSIGNSET_COUNT; i++)
					{
						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.dTrip)),             sizeof(double) );

							if(0 == oTInResultBusInfo.nxTransitID)
								continue;

							vecLowResultTransitInfo.push_back(oTInResultBusInfo);

							if(oTInResultBusInfo.nxTransitID < 0) // 음수는 rail 이라고 판단한다 고 함
							{
								nRailIndex = i;
							}
						}
						vecResultBusInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTBusSubData)),           sizeof(TInEditBusSubData) );
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

								if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
								{
									throw 1;
								}

								TInIndexResultTransitInfo oTIndexResultTransitInfo;
								std::vector<TInResultTransitInfo> &vecLowResultRailInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
								for (int k= 0; k< nDuplicateCnt; k++)
								{
									TInResultTransitInfo oTInResultRailInfo = {0,0,0,0.0, KEMInPathTypeRail, RGB(255,255,255), false};

									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nxTransitID)),      sizeof(Integer) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.dTrip)),            sizeof(double) );

									if(0 == oTInResultRailInfo.nxTransitID)
										continue;

									if(m > 0 && 0 == i)
									{
										oTInResultRailInfo.bDuplicationRail = true;
									}

									vecLowResultRailInfo.push_back(oTInResultRailInfo);
								}
								vecResultRailInfo.push_back(oTIndexResultTransitInfo);
							}
							vecMultiResultRailInfo.push_back(vecResultRailInfo);
						}
#pragma endregion 레일 정보 취합
					}

					std::vector< TInIndexResultTransitInfo> &vecResultTotalTransitInfo = oBusRailAssignInfo.vecResulTransitInfo;
					Integer nSingleIdx = 0;
					for (size_t i= 0; i< vecResultBusInfo.size(); i++)
					{
						if(i != nRailIndex)
						{
							TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecResultBusInfo[i];
							oTIndexResultTransitInfo.oIndexKey = KODKey( ((nSingleIdx++)+1), 0 );
							vecResultTotalTransitInfo.push_back(oTIndexResultTransitInfo);
						}
						else
						{
							size_t nxSize = vecMultiResultRailInfo.size();
							for (size_t m= 0; m< nxSize; m++)
							{
								std::vector< TInIndexResultTransitInfo > &vecTempResultRailInfo = vecMultiResultRailInfo[m];

								for (size_t k= 0; k< vecTempResultRailInfo.size(); k++)
								{
									TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecTempResultRailInfo[k];

									nSingleIdx = (i+k+1);
									if (1 == nxSize)
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, 0);
									}
									else
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, (m+1));
									}

									vecResultTotalTransitInfo.push_back(vecTempResultRailInfo[k]);
								}
							}
						}
					}

#pragma region 취합 정보 등록
					AutoType iter  = m_mapBusRailAssign.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapBusRailAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
						m_mapBusRailAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}
				nCount++;
			}

// 			CString strResult = _T("");
// 			strResult.Format(_T("--- end insert result size : %d"), nCount);
// 			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapBusRailAssign.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadBusRailPathFile 오류");
		/*ThrowException(_T("ReadBusRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadParkRidePathFile( CString a_strFileName )
{
	TxLogDebugStartMsg();

	m_mapParkRideAssign.clear();
	std::ifstream ifs( a_strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOriginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KInterPathEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emInterModalPathMode = KEMInterModalPathModeUrban;
					oBusRailAssignInfo.emInterMode         = KEMInterModeParkRide;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					if(ifs.eof())break;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoStartNode)),        sizeof(Integer) );

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoEndNode))  ,        sizeof(Integer) );

					std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< PARKRIDEPATH_ASSIGNSET_COUNT; i++)
					{
						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.dTrip)),             sizeof(double) );

							if(0 == oTInResultBusInfo.nxTransitID)
								continue;

							vecLowResultTransitInfo.push_back(oTInResultBusInfo);

							if(oTInResultBusInfo.nxTransitID < 0) // 음수는 rail 이라고 판단한다 고 함
							{
								nRailIndex = i;
							}
						}
						vecResultBusInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTMultiSubData)),         sizeof(TInEditMultiSubData) );
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

								if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
								{
									throw 1;
								}

								TInIndexResultTransitInfo oTIndexResultTransitInfo;
								std::vector<TInResultTransitInfo> &vecLowResultRailInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
								for (int k= 0; k< nDuplicateCnt; k++)
								{
									TInResultTransitInfo oTInResultRailInfo = {0,0,0,0.0, KEMInPathTypeRail, RGB(255,255,255), false};

									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nxTransitID)),      sizeof(Integer) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.dTrip)),            sizeof(double) );

									if(0 == oTInResultRailInfo.nxTransitID)
										continue;

									if(m > 0 && 0 == i)
									{
										oTInResultRailInfo.bDuplicationRail = true;
									}

									vecLowResultRailInfo.push_back(oTInResultRailInfo);
								}
								vecResultRailInfo.push_back(oTIndexResultTransitInfo);
							}
							vecMultiResultRailInfo.push_back(vecResultRailInfo);
						}
#pragma endregion 레일 정보 취합
					}

					std::vector< TInIndexResultTransitInfo> &vecResultTotalTransitInfo = oBusRailAssignInfo.vecResulTransitInfo;
					Integer nSingleIdx = 0;
					for (size_t i= 0; i< vecResultBusInfo.size(); i++)
					{
						if(i != nRailIndex)
						{
							TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecResultBusInfo[i];
							oTIndexResultTransitInfo.oIndexKey = KODKey( ((nSingleIdx++)+1), 0 );
							vecResultTotalTransitInfo.push_back(oTIndexResultTransitInfo);
						}
						else
						{
							size_t nxSize = vecMultiResultRailInfo.size();
							for (size_t m= 0; m< nxSize; m++)
							{
								std::vector< TInIndexResultTransitInfo > &vecTempResultRailInfo = vecMultiResultRailInfo[m];

								for (size_t k= 0; k< vecTempResultRailInfo.size(); k++)
								{
									TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecTempResultRailInfo[k];

									nSingleIdx = (i+k+1);
									if (1 == nxSize)
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, 0);
									}
									else
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, (m+1));
									}

									vecResultTotalTransitInfo.push_back(vecTempResultRailInfo[k]);
								}
							}
						}
					}

#pragma region 취합 정보 등록
					AutoType iter  = m_mapParkRideAssign.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapParkRideAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
						m_mapParkRideAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}
				nCount++;
			}

// 			CString strResult = _T("");
// 			strResult.Format(_T("--- end insert result size : %d"), nCount);
// 			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapParkRideAssign.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadParkRidePathFile 오류");
		/*ThrowException(_T("ReadParkRidePathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadKissRidePathFile( CString a_strFileName )
{
	TxLogDebugStartMsg();

	m_mapKissRideAssign.clear();
	std::ifstream ifs( a_strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOriginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KInterPathEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emInterModalPathMode = KEMInterModalPathModeUrban;
					oBusRailAssignInfo.emInterMode         = KEMInterModeKissRide;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					if(ifs.eof())break;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoStartNode)),        sizeof(Integer) );

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoEndNode))  ,        sizeof(Integer) );

					std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< KISSRIDEPATH_ASSIGNSET_COUNT; i++)
					{
						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.dTrip)),             sizeof(double) );

							if(0 == oTInResultBusInfo.nxTransitID)
								continue;

							vecLowResultTransitInfo.push_back(oTInResultBusInfo);

							if(oTInResultBusInfo.nxTransitID < 0) // 음수는 rail 이라고 판단한다 고 함
							{
								nRailIndex = i;
							}
						}
						vecResultBusInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTMultiSubData)),         sizeof(TInEditMultiSubData) );
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

								if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
								{
									throw 1;
								}

								TInIndexResultTransitInfo oTIndexResultTransitInfo;
								std::vector<TInResultTransitInfo> &vecLowResultRailInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
								for (int k= 0; k< nDuplicateCnt; k++)
								{
									TInResultTransitInfo oTInResultRailInfo = {0,0,0,0.0, KEMInPathTypeRail, RGB(255,255,255), false};

									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nxTransitID)),      sizeof(Integer) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.dTrip)),            sizeof(double) );

									if(0 == oTInResultRailInfo.nxTransitID)
										continue;

									if(m > 0 && 0 == i)
									{
										oTInResultRailInfo.bDuplicationRail = true;
									}

									vecLowResultRailInfo.push_back(oTInResultRailInfo);
								}
								vecResultRailInfo.push_back(oTIndexResultTransitInfo);
							}
							vecMultiResultRailInfo.push_back(vecResultRailInfo);
						}
#pragma endregion 레일 정보 취합
					}

					std::vector< TInIndexResultTransitInfo> &vecResultTotalTransitInfo = oBusRailAssignInfo.vecResulTransitInfo;
					Integer nSingleIdx = 0;
					for (size_t i= 0; i< vecResultBusInfo.size(); i++)
					{
						if(i != nRailIndex)
						{
							TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecResultBusInfo[i];
							oTIndexResultTransitInfo.oIndexKey = KODKey( ((nSingleIdx++)+1), 0 );
							vecResultTotalTransitInfo.push_back(oTIndexResultTransitInfo);
						}
						else
						{
							size_t nxSize = vecMultiResultRailInfo.size();
							for (size_t m= 0; m< nxSize; m++)
							{
								std::vector< TInIndexResultTransitInfo > &vecTempResultRailInfo = vecMultiResultRailInfo[m];

								for (size_t k= 0; k< vecTempResultRailInfo.size(); k++)
								{
									TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecTempResultRailInfo[k];

									nSingleIdx = (i+k+1);
									if (1 == nxSize)
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, 0);
									}
									else
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, (m+1));
									}

									vecResultTotalTransitInfo.push_back(vecTempResultRailInfo[k]);
								}
							}
						}
					}

#pragma region 취합 정보 등록
					AutoType iter  = m_mapKissRideAssign.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapKissRideAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
						m_mapKissRideAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}
				nCount++;
			}

// 			CString strResult = _T("");
// 			strResult.Format(_T("--- end insert result size : %d"), nCount);
// 			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapKissRideAssign.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadKissRidePathFile 오류");
		/*ThrowException(_T("ReadKissRidePathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadTaxiTransitPathFile( CString a_strFileName )
{
	TxLogDebugStartMsg();

	m_mapTaxiTransitAssign.clear();
	std::ifstream ifs( a_strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOriginID(0);
				Integer nxDestinationID(0);
				int     nPathCount(0);

				ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KInterPathEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emInterModalPathMode = KEMInterModalPathModeUrban;
					oBusRailAssignInfo.emInterMode         = KEMInterModeTaxiTransit;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					if(ifs.eof())break;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoStartNode)),        sizeof(Integer) );

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoEndNode))  ,        sizeof(Integer) );

					std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< TAXITRANSITPATH_ASSIGNSET_COUNT; i++)
					{
						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.dTrip)),             sizeof(double) );

							if(0 == oTInResultBusInfo.nxTransitID)
								continue;

							vecLowResultTransitInfo.push_back(oTInResultBusInfo);

							if(oTInResultBusInfo.nxTransitID < 0) // 음수는 rail 이라고 판단한다 고 함
							{
								nRailIndex = i;
							}
						}
						vecResultBusInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTMultiSubData)),         sizeof(TInEditMultiSubData) );
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

								if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
								{
									throw 1;
								}

								TInIndexResultTransitInfo oTIndexResultTransitInfo;
								std::vector<TInResultTransitInfo> &vecLowResultRailInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
								for (int k= 0; k< nDuplicateCnt; k++)
								{
									TInResultTransitInfo oTInResultRailInfo = {0,0,0,0.0, KEMInPathTypeRail, RGB(255,255,255), false};

									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nxTransitID)),      sizeof(Integer) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.dTrip)),            sizeof(double) );

									if(0 == oTInResultRailInfo.nxTransitID)
										continue;

									if(m > 0 && 0 == i)
									{
										oTInResultRailInfo.bDuplicationRail = true;
									}

									vecLowResultRailInfo.push_back(oTInResultRailInfo);
								}
								vecResultRailInfo.push_back(oTIndexResultTransitInfo);
							}
							vecMultiResultRailInfo.push_back(vecResultRailInfo);
						}
#pragma endregion 레일 정보 취합
					}

					std::vector< TInIndexResultTransitInfo> &vecResultTotalTransitInfo = oBusRailAssignInfo.vecResulTransitInfo;
					Integer nSingleIdx = 0;
					for (size_t i= 0; i< vecResultBusInfo.size(); i++)
					{
						if(i != nRailIndex)
						{
							TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecResultBusInfo[i];
							oTIndexResultTransitInfo.oIndexKey = KODKey( ((nSingleIdx++)+1), 0 );
							vecResultTotalTransitInfo.push_back(oTIndexResultTransitInfo);
						}
						else
						{
							size_t nxSize = vecMultiResultRailInfo.size();
							for (size_t m= 0; m< nxSize; m++)
							{
								std::vector< TInIndexResultTransitInfo > &vecTempResultRailInfo = vecMultiResultRailInfo[m];

								for (size_t k= 0; k< vecTempResultRailInfo.size(); k++)
								{
									TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecTempResultRailInfo[k];

									nSingleIdx = (i+k+1);
									if (1 == nxSize)
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, 0);
									}
									else
									{
										oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, (m+1));
									}

									vecResultTotalTransitInfo.push_back(vecTempResultRailInfo[k]);
								}
							}
						}
					}

#pragma region 취합 정보 등록
					AutoType iter  = m_mapTaxiTransitAssign.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = m_mapTaxiTransitAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
						m_mapTaxiTransitAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
					}
#pragma endregion 취합 정보 등록
				}
				nCount++;
			}

// 			CString strResult = _T("");
// 			strResult.Format(_T("--- end insert result size : %d"), nCount);
// 			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapTaxiTransitAssign.clear();
		ifs.close();
		m_bResult = false;
		m_strErrMsg =_T("ReadTaxiTransitPathFile 오류");
		/*ThrowException(_T("ReadTaxiTransitPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


std::vector<KInterPathEditInfo>* KInterModalResultDlg::IsHaveAutoTaxiAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapAutoTaxiAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapAutoTaxiAssign.end();

	if(iter != itEnd)
	{
		std::vector<KInterPathEditInfo> &vecTransitAssignInfo = iter->second;
		return (&vecTransitAssignInfo);
	}

	return nullptr;
}


std::vector<KInterPathEditInfo>* KInterModalResultDlg::IsHaveBusAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapBusAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapBusAssign.end();

	if(iter != itEnd)
	{
		std::vector<KInterPathEditInfo> &vecTransitAssignInfo = iter->second;
		return (&vecTransitAssignInfo);
	}

	return nullptr;
}


std::vector<KInterPathEditInfo>* KInterModalResultDlg::IsHaveRailAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapRailAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapRailAssign.end();

	if(iter != itEnd)
	{
		std::vector<KInterPathEditInfo> &vecTransitAssignInfo = iter->second;
		return (&vecTransitAssignInfo);
	}

	return nullptr;
}


std::vector<KInterPathEditInfo>* KInterModalResultDlg::IsHaveBusRailAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapBusRailAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapBusRailAssign.end();

	if(iter != itEnd)
	{
		std::vector<KInterPathEditInfo> &vecTransitAssignInfo = iter->second;
		return (&vecTransitAssignInfo);
	}

	return nullptr;
}


std::vector<KInterPathEditInfo>* KInterModalResultDlg::IsHaveParkRideAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapParkRideAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapParkRideAssign.end();

	if(iter != itEnd)
	{
		std::vector<KInterPathEditInfo> &vecTransitAssignInfo = iter->second;
		return (&vecTransitAssignInfo);
	}

	return nullptr;
}


std::vector<KInterPathEditInfo>* KInterModalResultDlg::IsHaveKissRideAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapKissRideAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapKissRideAssign.end();

	if(iter != itEnd)
	{
		std::vector<KInterPathEditInfo> &vecTransitAssignInfo = iter->second;
		return (&vecTransitAssignInfo);
	}

	return nullptr;
}


std::vector<KInterPathEditInfo>* KInterModalResultDlg::IsHaveTaxiTransitAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapTaxiTransitAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapTaxiTransitAssign.end();

	if(iter != itEnd)
	{
		std::vector<KInterPathEditInfo> &vecTransitAssignInfo = iter->second;
		return (&vecTransitAssignInfo);
	}

	return nullptr;
}


std::vector<KRegionModePathInfo>* KInterModalResultDlg::IsHaveRegionalAssignData(Integer &a_nxZoneID)
{
	AutoType iter  = m_mapRegionAssign.find(a_nxZoneID);
	AutoType itEnd = m_mapRegionAssign.end();

	if(iter != itEnd)
	{
		std::vector<KRegionModePathInfo> &vecTransitAssignInfo = iter->second;
		return (&vecTransitAssignInfo);
	}

	return nullptr;
}


void KInterModalResultDlg::OnBnClickedCheckAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;
		
		CXTPReportRecords*    pRecords = m_ctrlReportDest.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt      = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_E_COLUMN_SELECT);

			pItem->SetChecked(bSelectAll);
		}

		m_ctrlReportDest.RedrawControl();

		UpdateReportAData(bSelectAll);
		UpdateReportCData(bSelectAll);
		UpdateReportBData();
		UpdatePathCompareButtonState();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::OnReportDestCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_0_E_COLUMN_SELECT == nIndex)
	{
		if (pItemNotify->pItem->IsChecked() == FALSE)
		{
			CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
		}

		UpdateReportAData(TRUE);
		UpdateReportCData(TRUE);
		UpdateReportBData();
		UpdatePathCompareButtonState();
	}
}


	void KInterModalResultDlg::OnReportDestItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	try
	{
		if (_1_E_COLUMN_NAME == nIndex)
		{
			Integer nxDestinationID = (Integer)pItemNotify->pItem->GetItemData();
			
			m_pMapView->PanToFeature(KLayerID::Node(), nxDestinationID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KInterModalResultDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 
	
	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_B_COLUMN_ZONEID == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_B_COLUMN_ZONEID);

			if (TRUE == pItem->IsChecked())
			{
				KInterModalPathInfo::CheckChildItem(pRecord, TRUE);
				KInterModalPathInfo::CheckParentItem(pRecord);
			}
			else
			{
				KInterModalPathInfo::CheckChildItem(pRecord, FALSE);
				KInterModalPathInfo::UnCheckParentItem(pRecord);
			}

			m_wndReportBCtrl.RedrawControl();
		}
		catch (...)
		{
			return;
		}

		// 2018.08.17 명지대 요청사항 수정
		updateMinMaxEditControl();
		// 2018.08.17 명지대 요청사항 수정 끝

		if (m_setSearchedID.size() > 0) {
			DoneDraw(true);
		}
		else {
			DoneDraw(false);
		}
	}

	UpdatePathCompareButtonState();
}


void KInterModalResultDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
}


void KInterModalResultDlg::OnReportBItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecordItem* pItem = pItemNotify->pItem;

	if(_4_B_COLUMN_DETAIL == pItem->GetIndex())
	{
		try
		{
			pItem = pItemNotify->pItem;

			KEMInterModalPathMode emInterModalPathMode;
			int nInterModalPathMode = (int)pItem->GetItemData();
			if (KEMInterModalPathModeUrban == nInterModalPathMode)
				emInterModalPathMode = KEMInterModalPathModeUrban;
			else if (KEMInterModalPathModeRegional == nInterModalPathMode)
				emInterModalPathMode = KEMInterModalPathModeRegional;
			else
				return;

			CXTPReportRecord*     pRecord        = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItemB         = pRecord->GetItem(_0_B_COLUMN_ZONEID);
			KInterPathEditInfo*   pPathEditInfo  = (KInterPathEditInfo*)pItemB->GetItemData();
			CString               strPathIdex    = pItemB->GetCaption(NULL);

			CRect rect;
			GetWindowRect(rect);

			if(nullptr != m_spInterModalResultDetailDlg)
			{
				m_spInterModalResultDetailDlg->InitializeEditData(emInterModalPathMode, strPathIdex, pPathEditInfo, rect);
				m_spInterModalResultDetailDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				m_spInterModalResultDetailDlg = KInterResultDetailDlgPtr(new KInterResultDetailDlg(m_pTarget, true, m_mapNodeName, this));
				m_spInterModalResultDetailDlg->Create(KInterResultDetailDlg::IDD, this);
				m_spInterModalResultDetailDlg->InitializeEditData(emInterModalPathMode, strPathIdex, pPathEditInfo, rect);
				m_spInterModalResultDetailDlg->ShowWindow(SW_SHOW);
			}
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KInterModalResultDlg::OnReportCCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KInterModalResultDlg::OnReportColorAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if( 1 == nIndex)
	{
		try
		{
			CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor());
			if( IDOK == dlg.DoModal())
			{
				COLORREF clrBack = dlg.GetColor();
				pItemNotify->pItem->SetBackgroundColor(clrBack);
				m_wndReportColorACtrl.RedrawControl();

				CXTPReportRecord*	  pRecord      = pItemNotify->pRow->GetRecord();
				CXTPReportRecordItem* pItem        = pRecord->GetItem(0);
				TInterPathColor*      pTUrbanColor = (TInterPathColor*)pItem->GetItemData();

				pTUrbanColor->clrPath = clrBack;
			}
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KInterModalResultDlg::OnReportColorBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if( 1 == nIndex)
	{
		try
		{
			CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor());
			if( IDOK == dlg.DoModal())
			{
				COLORREF clrBack = dlg.GetColor();
				pItemNotify->pItem->SetBackgroundColor(clrBack);
				m_wndReportColorBCtrl.RedrawControl();

				CXTPReportRecord*	  pRecord         = pItemNotify->pRow->GetRecord();
				CXTPReportRecordItem* pItem           = pRecord->GetItem(0);
				TInterPathColor*      pTRegionalColor = (TInterPathColor*)pItem->GetItemData();

				pTRegionalColor->clrPath = clrBack;
			}
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KInterModalResultDlg::ReadRegionalPathFile(CString strFileName)
{
	TxLogDebugStartMsg();
	m_mapRegionAssign.clear();
	std::ifstream ifs( strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxOrginID  (0);
				Integer nxDZoneID  (0);
				int     nRegionCode(0);
				int     nPathCount (0);
				// 0~2
				ifs.read( reinterpret_cast<char*>( &(nxOrginID)),                             sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxDZoneID)),                             sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nRegionCode)),                           sizeof(int) );

				if(ifs.eof())break;

				std::vector<KRegionModePathInfo> vecRegionModePathInfo;

				AutoType iter  = m_mapRegionalCodeName.begin();
				AutoType itEnd = m_mapRegionalCodeName.end();
				
				while (iter != itEnd)
				{
					// 3~8...
					ifs.read( reinterpret_cast<char*>( &(nPathCount)),                            sizeof(int) );

					if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					KRegionModePathInfo oRegionModePathInfo;
					oRegionModePathInfo.nxOZoneID   = nxOrginID;
					oRegionModePathInfo.nxDZoneID   = nxDZoneID;
					oRegionModePathInfo.strModeName = iter->second;
					oRegionModePathInfo.nMode       = iter->first;
					oRegionModePathInfo.nPathCount  = nPathCount;

					vecRegionModePathInfo.push_back(oRegionModePathInfo);
					++iter;
				}
				
				for (size_t i= 0; i< vecRegionModePathInfo.size(); i++)
				{
					KRegionModePathInfo            &oRegionModePathInfo = vecRegionModePathInfo[i];
					std::vector<KRegionPathDetail> &vecOnePathDetail    = oRegionModePathInfo.vecOnePathDetail;
					
					if (REGION_MODE_CODE_AUTO == oRegionModePathInfo.nMode) // Auto
					{
						for (int k= 0; k< oRegionModePathInfo.nPathCount; k++)
						{
							KRegionPathDetail oRegionOnePathDetail;
							//8~12
							int nRegMode(0);
							ifs.read( reinterpret_cast<char*>( &(nRegMode)),                              sizeof(int) );

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainTime)),        sizeof(double) );
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainCost)),        sizeof(double) );
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainLength)),      sizeof(double) );
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dTrip)),            sizeof(double) );

							oRegionOnePathDetail.dMainWaitTime     = 0.0;  // 프로토콜에 dMainWaitTime 없음.
							oRegionOnePathDetail.dMainTransferTime = 0.0;  // 프로토콜에 dMainTransferTime 없음.
							oRegionOnePathDetail.dMainProcessTime  = 0.0;  // 프로토콜에 dMainProcessTime 없음.
							oRegionOnePathDetail.dMainFrequency    = 0.0;  // 프로토콜에 dMainFrequency 없음.

							oRegionOnePathDetail.nxOrgTermID   = 0;
							oRegionOnePathDetail.nxTransTermID = 0;
							oRegionOnePathDetail.nxDesTermID   = 0;

							vecOnePathDetail.push_back(oRegionOnePathDetail);
						}
					}
					else
					{
						for (int k= 0; k< oRegionModePathInfo.nPathCount; k++)
						{
							KRegionPathDetail oRegionOnePathDetail;
							//13~34
							int nRegMode(0);
							ifs.read( reinterpret_cast<char*>( &(nRegMode)),                              sizeof(int) );
							
							int     nEgnoreData(0);
							double  dEgnoreData(0.0);

							ifs.read( reinterpret_cast<char*>( &(nEgnoreData)),                           sizeof(int) ); // reg mode path seq

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.nxOrgTermID)),      sizeof(Integer) );
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.nxDesTermID)),      sizeof(Integer) );
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.nxTransTermID)),    sizeof(Integer) );

							ifs.read( reinterpret_cast<char*>( &(nEgnoreData)),                           sizeof(int) ); // accese seq
							ifs.read( reinterpret_cast<char*>( &(nEgnoreData)),                           sizeof(int) ); // egress seq

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainTime)),        sizeof(double) );

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainWaitTime)),    sizeof(double) );

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainTransferTime)),sizeof(double) );

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainProcessTime)), sizeof(double) );

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainFrequency)),   sizeof(double) );

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainCost)),        sizeof(double) );
							oRegionOnePathDetail.dMainLength = 0.0; // 프로토콜에 length 없음.

							ifs.read( reinterpret_cast<char*>( &(dEgnoreData)),                           sizeof(double) );// sumtime

							int nAccAutoPathCnt(0);
							int nAccTaxiPathCnt(0);
							int nAccBusPathCnt(0);
							int nAccRailPathCnt(0);
							int nAccBusRailPathCnt(0);
							int nEgrAutoPathCnt(0);
							int nEgrTaxiPathCnt(0);
							int nEgrBusPathCnt(0);
							int nEgrRailPathCnt(0);
							int nEgrBusRailPathCnt(0);
							ifs.read( reinterpret_cast<char*>( &(nAccAutoPathCnt)),                       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nAccTaxiPathCnt)),                       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nAccBusPathCnt)),                        sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nAccRailPathCnt)),                       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nAccBusRailPathCnt)),                    sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nEgrAutoPathCnt)),                       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nEgrTaxiPathCnt)),                       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nEgrBusPathCnt)),                        sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nEgrRailPathCnt)),                       sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(nEgrBusRailPathCnt)),                    sizeof(int) );

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dTrip)),            sizeof(double) );

							ifs.read( reinterpret_cast<char*>( &(nEgnoreData)),                           sizeof(int) ); // 34:사용유무

							//35~40
							ReadRegAutoTaxiPathFile(ifs, nAccAutoPathCnt,    oRegionOnePathDetail.mapAccAutoAssign, KEMInterAccEgrTypeAccess);
							int nAccSumPathCount = nAccTaxiPathCnt + nAccBusPathCnt + nAccRailPathCnt + nAccBusRailPathCnt;
							if (nAccSumPathCount > 0)
							{
								ReadRegAutoTaxiPathFile(ifs, nAccTaxiPathCnt,    oRegionOnePathDetail.mapAccTaxiAssign, KEMInterAccEgrTypeAccess);
								ReadRegBusPathFile     (ifs, nAccBusPathCnt,     oRegionOnePathDetail.mapAccBusAssign, KEMInterAccEgrTypeAccess);
								ReadRegRailPathFile    (ifs, nAccRailPathCnt,    oRegionOnePathDetail.mapAccRailAssign, KEMInterAccEgrTypeAccess);
								ReadRegBusRailPathFile (ifs, nAccBusRailPathCnt, oRegionOnePathDetail.mapAccBusRailAssign, KEMInterAccEgrTypeAccess);
							}

							ReadRegAutoTaxiPathFile(ifs, nEgrAutoPathCnt,    oRegionOnePathDetail.mapEgrAutoAssign, KEMInterAccEgrTypeEgress);
							int nEgrSumPathCount = nEgrTaxiPathCnt + nEgrBusPathCnt + nEgrRailPathCnt + nEgrBusRailPathCnt;
							if (nEgrSumPathCount > 0)
							{
								ReadRegAutoTaxiPathFile(ifs, nEgrTaxiPathCnt,    oRegionOnePathDetail.mapEgrTaxiAssign, KEMInterAccEgrTypeEgress);
								ReadRegBusPathFile     (ifs, nEgrBusPathCnt,     oRegionOnePathDetail.mapEgrBusAssign, KEMInterAccEgrTypeEgress);
								ReadRegRailPathFile    (ifs, nEgrRailPathCnt,    oRegionOnePathDetail.mapEgrRailAssign, KEMInterAccEgrTypeEgress);
								ReadRegBusRailPathFile (ifs, nEgrBusRailPathCnt, oRegionOnePathDetail.mapEgrBusRailAssign, KEMInterAccEgrTypeEgress);
							}

							vecOnePathDetail.push_back(oRegionOnePathDetail);
						}
					}
				}

				AutoType iterB  = m_mapRegionAssign.find(nxDZoneID);
				AutoType itEndB = m_mapRegionAssign.end();

				if( iterB == itEndB )
				{
					m_mapRegionAssign.insert(std::make_pair(nxDZoneID, vecRegionModePathInfo));
				}

				nCount++;
			}

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch(KExceptionPtr ex)
	{
		m_mapRegionAssign.clear();
		ifs.close();

		m_bResult = false;
		m_strErrMsg = ex->GetErrorMessage();
		TxLogDebugException();
	}
	catch (...)
	{
		m_mapRegionAssign.clear();
		ifs.close();

		m_bResult = false;
		m_strErrMsg =_T("ReadRegionalPathFile 오류");
		TxLogDebugException();
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadRegAutoTaxiPathFile(std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
{
	TxLogDebugStartMsg();
	try
	{
		for (int i= 0; i< a_nPathCnt; i++)
		{
			KInterPathEditInfo oInterPathEditInfo;
			oInterPathEditInfo.emInterAccEgrType = a_emInterAccEgrType;

			Integer nxDestinationID(0);
			ifs.read( reinterpret_cast<char*>( &(oInterPathEditInfo.nxOrginID)),          sizeof(Integer) );
		
			ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );
			oInterPathEditInfo.nxDestinationID = nxDestinationID;
			
			oInterPathEditInfo.emInterModalPathMode = KEMInterModalPathModeRegional;
			oInterPathEditInfo.emInterMode          = KEMInterModeAutoTaxi;
			oInterPathEditInfo.emInterRegDetailMode = KEMInterRegDetailModeAuto;

			ifs.read( reinterpret_cast<char*>( &(oInterPathEditInfo.oTRegAutoSubData)),   sizeof(TRegAutoSubData) );
		
			ifs.read( reinterpret_cast<char*>( &(oInterPathEditInfo.nUseFlag)),           sizeof(int) );

			AutoType iter = a_mapTransitAssign.find(nxDestinationID);
			if (iter != a_mapTransitAssign.end())
			{
				 std::vector<KInterPathEditInfo> &vecInterPathEditInfo = iter->second;
				 vecInterPathEditInfo.push_back(oInterPathEditInfo);
			}
			else
			{
				std::vector<KInterPathEditInfo> vecInterPathEditInfo;
				vecInterPathEditInfo.push_back(oInterPathEditInfo);
				a_mapTransitAssign.insert(std::make_pair(nxDestinationID, vecInterPathEditInfo));
			}
		}
	}
	catch (...)
	{
		ThrowException(_T("ReadRegAutoTaxiPathFile 오류"));
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadRegBusPathFile(std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
{
	TxLogDebugStartMsg();

	try
	{
		Integer nxOrginID(0);
		Integer nxDestinationID(0);
		int     nPathCount(0);
		double  dTotalTrip(0.0);

		ifs.read( reinterpret_cast<char*>( &(nxOrginID)),                             sizeof(Integer) );
		ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );
		ifs.read( reinterpret_cast<char*>( &(nPathCount)),                            sizeof(int) );
		ifs.read( reinterpret_cast<char*>( &(dTotalTrip)),                            sizeof(double) );

		if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
		{
			throw 1;
		}

		for (int z= 0; z< nPathCount; z++)
		{
#pragma region 정보 취합
			KInterPathEditInfo oBusAssignInfo;
			oBusAssignInfo.emInterAccEgrType   = a_emInterAccEgrType;
			oBusAssignInfo.emInterModalPathMode = KEMInterModalPathModeRegional;
			oBusAssignInfo.emInterMode          = KEMInterModeBus;
			oBusAssignInfo.emInterRegDetailMode = KEMInterRegDetailModeBus;
			oBusAssignInfo.nxOrginID           = nxOrginID;
			oBusAssignInfo.nxDestinationID     = nxDestinationID;

			ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nPathID)),                sizeof(int) );

			std::vector< TInIndexResultTransitInfo > &vecResulTransitInfo = oBusAssignInfo.vecResulTransitInfo;
			for (int i= 0; i< REG_BUSPATH_ASSIGNSET_COUNT; i++)
			{
				TInIndexResultTransitInfo oTIndexResultTransitInfo;
				oTIndexResultTransitInfo.oIndexKey = KODKey((i+1),0);
				std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
				for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
				{
					TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),      sizeof(Integer) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),    sizeof(int) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),      sizeof(int) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.dTrip)),            sizeof(double) );

					if(0 == oTInResultBusInfo.nxTransitID)
						continue;

					vecLowResultTransitInfo.push_back(oTInResultBusInfo);
				}
				vecResulTransitInfo.push_back(oTIndexResultTransitInfo);
			}

			ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.oTBusSubData)),               sizeof(TInEditBusSubData) );
			ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nUseFlag)),                   sizeof(int) );
#pragma endregion 정보 취합

#pragma region 취합 정보 등록
			AutoType iter  = a_mapTransitAssign.find(oBusAssignInfo.nxDestinationID);
			AutoType itEnd = a_mapTransitAssign.end();

			if( iter != itEnd )
			{
				std::vector<KInterPathEditInfo> &vecBusAssignInfo = iter->second;
				vecBusAssignInfo.push_back(oBusAssignInfo);
			}
			else
			{
				std::vector<KInterPathEditInfo> vecBusAssignInfo;
				vecBusAssignInfo.push_back(oBusAssignInfo);
				a_mapTransitAssign.insert(std::make_pair(oBusAssignInfo.nxDestinationID, vecBusAssignInfo));
			}
#pragma endregion 취합 정보 등록
		}
	}
	catch (...)
	{

		ThrowException(_T("ReadRegBusPathFile 오류"));
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadRegRailPathFile(std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
{
	TxLogDebugStartMsg();

	try
	{
		Integer nxOriginID(0);
		Integer nxDestinationID(0);
		int     nPathCount(0);
		double  dTotalTrip(0.0);

		ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
		ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
		ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
		ifs.read( reinterpret_cast<char*>( &(dTotalTrip)),              sizeof(double) );

		if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
		{
			throw 1;
		}

		for (int z= 0; z< nPathCount; z++)
		{
#pragma region 정보 취합
			KInterPathEditInfo oRailAssignInfo;
			oRailAssignInfo.emInterAccEgrType = a_emInterAccEgrType;
			oRailAssignInfo.emInterModalPathMode = KEMInterModalPathModeRegional;
			oRailAssignInfo.emInterMode          = KEMInterModeRail;
			oRailAssignInfo.emInterRegDetailMode = KEMInterRegDetailModeRail;
			oRailAssignInfo.nxOrginID            = nxOriginID;
			oRailAssignInfo.nxDestinationID      = nxDestinationID;

			ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nPathID)),                sizeof(int) );

			int nBoardingCnt(0); //탑승횟수
			ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

			if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
			{
				throw 1;
			}

			std::vector< TInIndexResultTransitInfo > &vecResulTransitInfo = oRailAssignInfo.vecResulTransitInfo;
			for (int i= 0; i< nBoardingCnt; i++)
			{
				int nDuplicateCnt(0); //중복 노선 갯수
				ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
				if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				TInIndexResultTransitInfo oTIndexResultTransitInfo;
				oTIndexResultTransitInfo.oIndexKey = KODKey((1+i),0);
				std::vector<TInResultTransitInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
				for (int k= 0; k< nDuplicateCnt; k++)
				{
					TInResultTransitInfo oTInResultRailInfo = {0,0,0,0.0, KEMInPathTypeRail, RGB(255,255,255), false};

					ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nxTransitID)),      sizeof(Integer) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.dTrip)),            sizeof(double) );

					if(0 == oTInResultRailInfo.nxTransitID)
						continue;

					vecLowResultBusInfo.push_back(oTInResultRailInfo);
				}
				vecResulTransitInfo.push_back(oTIndexResultTransitInfo);
			}

			ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.oTRailSubData)),             sizeof(TInEditRailSubData) );
			ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nUseFlag)),                  sizeof(int) );
#pragma endregion 정보 취합

#pragma region 취합 정보 등록
			AutoType iter  = a_mapTransitAssign.find(oRailAssignInfo.nxDestinationID);
			AutoType itEnd = a_mapTransitAssign.end();

			if( iter != itEnd )
			{
				std::vector<KInterPathEditInfo> &vecRailAssignInfo = iter->second;
				vecRailAssignInfo.push_back(oRailAssignInfo);
			}
			else
			{
				std::vector<KInterPathEditInfo> vecRailAssignInfo;
				vecRailAssignInfo.push_back(oRailAssignInfo);
				a_mapTransitAssign.insert(std::make_pair(oRailAssignInfo.nxDestinationID, vecRailAssignInfo));
			}
#pragma endregion 취합 정보 등록
		}
	}
	catch (...)
	{
		ThrowException(_T("ReadRegRailPathFile 오류"));
		return;
	}    

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::ReadRegBusRailPathFile(std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
{
	TxLogDebugStartMsg();

	try
	{
		Integer nxOriginID(0);
		Integer nxDestinationID(0);
		int     nPathCount(0);
		double  dTotalTrip(0.0);

		ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
		ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
		ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
		ifs.read( reinterpret_cast<char*>( &(dTotalTrip)),              sizeof(double) );

		if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
		{
			throw 1;
		}

		for (int z= 0; z< nPathCount; z++)
		{
#pragma region 버스 정보 취합
			KInterPathEditInfo oBusRailAssignInfo;
			oBusRailAssignInfo.emInterAccEgrType    = a_emInterAccEgrType;
			oBusRailAssignInfo.emInterModalPathMode = KEMInterModalPathModeRegional;
			oBusRailAssignInfo.emInterMode          = KEMInterModeBusRail;
			oBusRailAssignInfo.emInterRegDetailMode = KEMInterRegDetailModeBusRail;
			
			oBusRailAssignInfo.nxOrginID            = nxOriginID;
			oBusRailAssignInfo.nxDestinationID      = nxDestinationID;
			int nRailIndex = -1;

			ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );

			std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
			for (int i= 0; i< REG_BUSRAILPATH_ASSIGNSET_COUNT; i++)
			{
				TInIndexResultTransitInfo oTIndexResultTransitInfo;
				std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
				for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
				{
					TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.dTrip)),             sizeof(double) );

					if(0 == oTInResultBusInfo.nxTransitID)
						continue;

					vecLowResultTransitInfo.push_back(oTInResultBusInfo);

					if(oTInResultBusInfo.nxTransitID < 0) // 음수는 rail 이라고 판단한다 고 함
					{
						nRailIndex = i;
					}
				}
				vecResultBusInfo.push_back(oTIndexResultTransitInfo);
			}

			ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTBusSubData)),           sizeof(TInEditBusSubData) );

			ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
#pragma endregion 버스 정보 취합

			std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
			if (nRailIndex != -1)
			{

#pragma region 레일 정보 취합(multi rail)
				int nRailCount(0);
				ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );

				if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int m= 0; m< nRailCount; m++)
				{
					int     nNoneUsePathID;

					ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );

					int nBoardingCnt(0); //탑승횟수
					ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );

					if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
					for (int i= 0; i< nBoardingCnt; i++)
					{
						int nDuplicateCnt(0); //중복 노선 갯수
						ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );

						if (nDuplicateCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TInResultTransitInfo> &vecLowResultRailInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< nDuplicateCnt; k++)
						{
							TInResultTransitInfo oTInResultRailInfo = {0,0,0,0.0, KEMInPathTypeRail, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nxTransitID)),      sizeof(Integer) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.dTrip)),            sizeof(double) );

							if(0 == oTInResultRailInfo.nxTransitID)
								continue;

							if(m > 0 && 0 == i)
							{
								oTInResultRailInfo.bDuplicationRail = true;
							}

							vecLowResultRailInfo.push_back(oTInResultRailInfo);
						}
						vecResultRailInfo.push_back(oTIndexResultTransitInfo);
					}
					vecMultiResultRailInfo.push_back(vecResultRailInfo);
				}
#pragma endregion 레일 정보 취합
			}

			std::vector< TInIndexResultTransitInfo> &vecResultTotalTransitInfo = oBusRailAssignInfo.vecResulTransitInfo;
			Integer nSingleIdx = 0;
			for (size_t i= 0; i< vecResultBusInfo.size(); i++)
			{
				if(i != nRailIndex)
				{
					TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecResultBusInfo[i];
					oTIndexResultTransitInfo.oIndexKey = KODKey( ((nSingleIdx++)+1), 0 );
					vecResultTotalTransitInfo.push_back(oTIndexResultTransitInfo);
				}
				else
				{
					size_t nxSize = vecMultiResultRailInfo.size();
					for (size_t m= 0; m< nxSize; m++)
					{
						std::vector< TInIndexResultTransitInfo > &vecTempResultRailInfo = vecMultiResultRailInfo[m];

						for (size_t k= 0; k< vecTempResultRailInfo.size(); k++)
						{
							TInIndexResultTransitInfo &oTIndexResultTransitInfo = vecTempResultRailInfo[k];

							nSingleIdx = (i+k+1);
							if (1 == nxSize)
							{
								oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, 0);
							}
							else
							{
								oTIndexResultTransitInfo.oIndexKey = KODKey((Integer)nSingleIdx, (m+1));
							}

							vecResultTotalTransitInfo.push_back(vecTempResultRailInfo[k]);
						}
					}
				}
			}

#pragma region 취합 정보 등록
			AutoType iter  = a_mapTransitAssign.find(oBusRailAssignInfo.nxDestinationID);
			AutoType itEnd = a_mapTransitAssign.end();

			if( iter != itEnd )
			{
				std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
				vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
			}
			else
			{
				std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
				vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
				a_mapTransitAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
			}
#pragma endregion 취합 정보 등록
		}
	}
	catch (...)
	{
		ThrowException(_T("ReadRegBusRailPathFile 오류"));
	}    

	TxLogDebugEndMsg();
}


HBRUSH KInterModalResultDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_EDIT1 || ID == IDC_EDIT2 )
    {        
        /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
        pDC->SetBkMode(TRANSPARENT);*/
        pDC->SetBkMode(TRANSPARENT);

		hbr = (HBRUSH)m_backBrush;
        //return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }
	return hbr;
}


void KInterModalResultDlg::OnBnClickedButtonClear()
{
	/************************************************************************/
	/* Clear버튼 클릭 시 모든 체크 옵션 선택이 취소되고 선택한 Origin/Destination에 Access Boundary만 표출되어야 함.                                                                     */
	/************************************************************************/

	CXTPReportRecords*    pRecords = m_ctrlReportDest.GetRecords();
	CXTPReportRecord*     pRecord  = nullptr;
	CXTPReportRecordItem* pItem    = nullptr;

	int nRecordCnt      = pRecords->GetCount();

	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
	for (int i= 0; i< nRecordCnt; i++)
	{
		pRecord = pRecords->GetAt(i);
		pItem   = pRecord->GetItem(_0_E_COLUMN_SELECT);

		pItem->SetChecked(TRUE);
	}

	m_ctrlReportDest.RedrawControl();

	UpdateReportAData(FALSE);
	UpdateReportCData(FALSE);
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KInterModalResultDlg::AllClearPath(bool a_bRefresh)
{
	try
	{
		m_pMapView->CacheClearCompositeThemes(a_bRefresh);

		m_oMapResult.vecDrawLinkInfo.clear();
		m_oMapResult.vecDrawNodeInfo.clear();
		m_oMapResult.vecDrawZoneNodeInfo.clear();
		m_oMapResult.vecUserLineInfo.clear();
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KInterModalResultDlg::OnBnClickedButtonDraw()
{
	TxLogDebugStartMsg();
	DoneDraw(true);
	TxLogDebugEndMsg();
}

void KInterModalResultDlg::DoneDraw(bool a_bShowErrMsg)
{
	TxLogDebugStartMsg();
	try
	{
		if (InvalidateInputInfo(a_bShowErrMsg) == false)
			throw 1;

		AllClearPath(false);

		//QBicSimpleProgressThread::ExecuteThread(SelectDrawDataThreadCaller, this);
		CWaitCursor cw;
		SelectDrawData();

		bool bLabel(false);
		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bLabel = true;
		else
			bLabel = false;

		m_pMapView->TxDrawCompositeNode(m_oMapResult.vecDrawNodeInfo, false,  false);
		m_pMapView->TxDrawCompositeUserLine(m_oMapResult.vecUserLineInfo, bLabel, false);//존->정류장 도보제거 요청 : 2013.03.03-명지대학교

		m_pMapView->TxDrawCompositeLink(m_oMapResult.vecDrawLinkInfo, bLabel,  false);

		DrawOrgDestZoneAccess(false); // org와 Dest들의 존엑세스를 그린다.

		m_pMapView->MapRefresh();
	}
	catch(...)
	{
		AllClearPath(true);
		return;
	}
	TxLogDebugEndMsg();
}

unsigned __stdcall KInterModalResultDlg::SelectDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KInterModalResultDlg*     pDlg = (KInterModalResultDlg*)pParameter->GetParameter();
	pDlg->SelectDrawData();

	return 0;
}

void KInterModalResultDlg::SelectDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		GetResultSettingInfo(m_oTInResultSettingInfo);

		std::set<DWORD_PTR>        setInterAssignInfo;
		std::vector<TMainPathInfo> vecDrawMainPathInfo;

		CXTPReportRecordItem*  pItem    = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord*   pParentRecord    = pRecords->GetAt(i);
			pItem             = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
			Integer nxDZoneID = (Integer)pItem->GetItemData();

			AutoType itFindPathMode = m_mapEndZonePathMode.find(nxDZoneID);
			if (itFindPathMode == m_mapEndZonePathMode.end())
				continue;
			KEMInterModalPathMode emInterModalPathMode = itFindPathMode->second;

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			if (nullptr == pSeParentRecords)
			{
				continue;
			}

			if (KEMInterModalPathModeUrban == emInterModalPathMode) // KEMInterModalPathModeRegional
			{
				GetSelectedUrbanTransitInfo(pSeParentRecords, setInterAssignInfo);
			}
			else
			{
				GetSelectedRegionalTransitInfo(pSeParentRecords, setInterAssignInfo, vecDrawMainPathInfo);
			}
		}

		if (setInterAssignInfo.size()< 1 && vecDrawMainPathInfo.size()< 1)
		{
			throw 1;
		}

		KInterModalPathInfo::ResultTransitPathDrawInfo3(m_pTarget, m_mapDestResultInputValue, setInterAssignInfo, vecDrawMainPathInfo, m_oTInResultSettingInfo, m_oMapResult);
		//KInterModalPathInfo::ResultTransitPathDrawInfo(m_pTarget, oInMapResultTripInfo, m_oTInResultSettingInfo, m_oMapResult);
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

bool KInterModalResultDlg::InvalidateInputInfo(bool a_bShowErrMsg)
{
	try
	{
		CString strMinTrip(_T(""));
		CString strMaxTrip(_T(""));
		GetDlgItemText(IDC_EDIT_MINTRIP, strMinTrip);
		GetDlgItemText(IDC_EDIT_MAXTRIP, strMaxTrip);

		if (QBicStringChecker::IsNumeric(strMinTrip) == false)
		{
			KControlMessage oControlMessage(IDC_EDIT_MINTRIP, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		if (QBicStringChecker::IsNumeric(strMaxTrip) == false)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXTRIP, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}

		double dMinTrip = _ttof(strMinTrip);
		if (dMinTrip < 0.0)
		{
			KControlMessage oControlMessage(IDC_EDIT_MINTRIP, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		double dMaxTrip = _ttof(strMaxTrip);
		if (dMaxTrip < 0.0)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXTRIP, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		if (dMinTrip > dMaxTrip)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXTRIP, _T("최소값보다 큰 값을 입력해 주세요."));
			throw oControlMessage;
		}

		CString strMinSize(_T(""));
		CString strMaxSize(_T(""));
		GetDlgItemText(IDC_EDIT_MINSIZE, strMinSize);
		GetDlgItemText(IDC_EDIT_MAXSIZE, strMaxSize);

		if (QBicStringChecker::IsNumeric(strMinSize) == false)
		{
			KControlMessage oControlMessage(IDC_EDIT_MINSIZE, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		if (QBicStringChecker::IsNumeric(strMaxSize) == false)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXSIZE, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}

		double dMinSize = _ttof(strMinSize);
		if (dMinSize < 0.0)
		{ 
			KControlMessage oControlMessage(IDC_EDIT_MINSIZE, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		double dMaxSize = _ttof(strMaxSize);
		if (dMaxSize < 0.0)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXSIZE, _T("0보다 큰 숫자를 입력해 주세요."));
			throw oControlMessage;
		}
		if (dMinSize >= dMaxSize)
		{
			KControlMessage oControlMessage(IDC_EDIT_MAXSIZE, _T("최소값보다 큰 값을 입력해 주세요."));
			throw oControlMessage;
		}
	}
	catch(KControlMessage oControlMessage)
	{
		if (a_bShowErrMsg) {
			AfxMessageBox(oControlMessage.strMsg);
		}
		
		GetDlgItem(oControlMessage.nID)->SetFocus();
		return false;
	}
	catch(...)
	{
		return false;
	}

	return true;
}


void KInterModalResultDlg::GetSelectedUrbanTransitInfo(CXTPReportRecords* a_pSeParentRecords, std::set<DWORD_PTR> &a_setInterAssignInfo)
{
	try
	{
		CXTPReportRecordItem* pItem = nullptr;

		for (int k= 0; k< a_pSeParentRecords->GetCount(); k++)
		{
			CXTPReportRecord*  pSeParentRecord = a_pSeParentRecords->GetAt(k);
			pItem            = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

			if( pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE )
				continue;

			CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
			if (nullptr == pThParentRecords)
				continue;

			for (int m= 0; m< pThParentRecords->GetCount(); m++)
			{
				CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
				pItem            = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				if( pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE )
					continue;

				KInterPathEditInfo* pAssignInfo = (KInterPathEditInfo*)pItem->GetItemData();

				DWORD_PTR dwPtr = (DWORD_PTR)pAssignInfo;
				a_setInterAssignInfo.insert(dwPtr);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::GetSelectedRegionalTransitInfo(CXTPReportRecords* a_pSeParentRecords, std::set<DWORD_PTR> &a_setInterAssignInfo, std::vector<TMainPathInfo> &a_vecMainPathInfo)
{
	try
	{
		CXTPReportRecordItem* pItem = nullptr;

		for (int k= 0; k< a_pSeParentRecords->GetCount(); k++)
		{
			CXTPReportRecord*  pSeParentRecord = a_pSeParentRecords->GetAt(k);
			pItem            = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

			if( pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE )
				continue;

			KRegionModePathInfo* pRegionModePathInfo = (KRegionModePathInfo*)pItem->GetItemData();

			CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
			if (nullptr == pThParentRecords)
				continue;

			for (int m= 0; m< pThParentRecords->GetCount(); m++)
			{
				CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
				pItem            = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				KRegionPathDetail* pRegionPathDetail = (KRegionPathDetail*)pItem->GetItemData();

				if( pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE )
					continue;

				MakeMainPathData(pRegionModePathInfo, pRegionPathDetail, a_vecMainPathInfo);

				// acc, egr, main
				CXTPReportRecords* pFrParentRecords = pThParentRecord->GetChilds();
				if (nullptr == pFrParentRecords)
					continue;

				for (int n= 0; n< pFrParentRecords->GetCount(); n++)
				{
					CXTPReportRecord*  pFrParentRecord = pFrParentRecords->GetAt(n);
					pItem            = pFrParentRecord->GetItem(_0_B_COLUMN_ZONEID);

					if( pItem->GetHasCheckbox() == FALSE )// false 이면 Main
						continue;
					if( pItem->IsChecked() == FALSE )
						continue;

					CXTPReportRecords* pFvParentRecords = pFrParentRecord->GetChilds();
					if (nullptr == pFvParentRecords)
						continue;

					for (int p= 0; p< pFvParentRecords->GetCount(); p++)
					{
						CXTPReportRecord*  pFvParentRecord = pFvParentRecords->GetAt(p);
						pItem            = pFvParentRecord->GetItem(_0_B_COLUMN_ZONEID);

						if( pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
							continue;

						CXTPReportRecords* pSixParentRecords = pFvParentRecord->GetChilds();
						if (nullptr == pSixParentRecords)
							continue;

						for (int q= 0; q< pSixParentRecords->GetCount(); q++)
						{
							CXTPReportRecord*  pSixParentRecord = pSixParentRecords->GetAt(q);
							pItem            = pSixParentRecord->GetItem(_0_B_COLUMN_ZONEID);

							if( pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
								continue;

							KInterPathEditInfo* pAssignInfo = (KInterPathEditInfo*)pItem->GetItemData();

							DWORD_PTR dwPtr = (DWORD_PTR)pAssignInfo;
							a_setInterAssignInfo.insert(dwPtr);
						}
					}
				}
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::GetResultSettingInfo(TInResultSettingInfo &a_oTInResultSettingInfo)
{
	try
	{
		int nMinID = IDC_EDIT_MINTRIP;
		int nMaxID = IDC_EDIT_MAXTRIP;

		CString strMinSize(_T(""));
		CString strMaxSize(_T(""));
		GetDlgItemText(IDC_EDIT_MINSIZE, strMinSize);
		GetDlgItemText(IDC_EDIT_MAXSIZE, strMaxSize);
		double dMinSize = _ttof(strMinSize);
		double dMaxSize = _ttof(strMaxSize);

		CString strMinTrip(_T(""));
		CString strMaxTrip(_T(""));
		GetDlgItemText(nMinID, strMinTrip);
		GetDlgItemText(nMaxID, strMaxTrip);
		double dMinTrip = _ttof(strMinTrip);
		double dMaxTrip = _ttof(strMaxTrip);

		a_oTInResultSettingInfo.mapUrbanColor    = m_mapUrbanColor;
		a_oTInResultSettingInfo.mapRegionalColor = m_mapRegionalColor;

		a_oTInResultSettingInfo.clrOutRange = m_cpOutRangeColor.GetColor();
		a_oTInResultSettingInfo.clrOrgZone  = m_clrOrgZone;
		a_oTInResultSettingInfo.clrDestZone = m_clrDestZone;

		a_oTInResultSettingInfo.dMinTrip    = dMinTrip;
		a_oTInResultSettingInfo.dMaxTrip    = dMaxTrip;
		a_oTInResultSettingInfo.dMinSize    = dMinSize;
		a_oTInResultSettingInfo.dMaxSize    = dMaxSize;
		if(IsDlgButtonChecked(IDC_CHECK_OUTRANGE) == BST_CHECKED)
			a_oTInResultSettingInfo.bUseOutRange = TRUE;
		else
			a_oTInResultSettingInfo.bUseOutRange = FALSE;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


unsigned __stdcall KInterModalResultDlg::MakeDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KInterModalResultDlg*     pDlg = (KInterModalResultDlg*)pParameter->GetParameter();
	pDlg->MakeDrawData();

	return 0;
}


void KInterModalResultDlg::MakeDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		std::set<DWORD_PTR> setInterAssignInfo;
		std::vector<TMainPathInfo> vecMainPathInfo;  // regional mode Main 정보 수집

		AutoType iter  = m_mapEndZoneNodeData.begin();
		AutoType itEnd = m_mapEndZoneNodeData.end();

		CString strZoneID(_T(""));
		for (; iter != itEnd; ++iter)
		{
			Integer nxDzoneID = iter->first;
			strZoneID.Format(_T("%I64d"), nxDzoneID);

			if (m_setSearchedID.find(strZoneID) == m_setSearchedID.end()) {
				continue;
			}

			std::vector<KInterPathEditInfo>* pvecTransitAssignInfo = nullptr;

			AutoType iterUrban   = m_mapUrbanCodeName.begin();
			AutoType itUrbanEnd  = m_mapUrbanCodeName.end();

			for (; iterUrban != itUrbanEnd; ++iterUrban)
			{
				int nCode             = iterUrban->first;
				pvecTransitAssignInfo = nullptr;
				if (KEMInterModeAutoTaxi == nCode)
				{
					pvecTransitAssignInfo = IsHaveAutoTaxiAssignData(nxDzoneID); //auto
				}
				else if (KEMInterModeBus == nCode)
				{
					pvecTransitAssignInfo = IsHaveBusAssignData(nxDzoneID); 
				}
				else if (KEMInterModeRail == nCode)
				{
					pvecTransitAssignInfo = IsHaveRailAssignData(nxDzoneID); 
				}
				else if (KEMInterModeBusRail == nCode)
				{
					pvecTransitAssignInfo = IsHaveBusRailAssignData(nxDzoneID); 
				}
				else if (KEMInterModeParkRide == nCode)
				{
					pvecTransitAssignInfo = IsHaveParkRideAssignData(nxDzoneID); 
				}
				else if (KEMInterModeKissRide == nCode)
				{
					pvecTransitAssignInfo = IsHaveKissRideAssignData(nxDzoneID); 
				}
				else if (KEMInterModeTaxiTransit == nCode)
				{
					pvecTransitAssignInfo = IsHaveTaxiTransitAssignData(nxDzoneID); 
				}

				if (nullptr != pvecTransitAssignInfo) {
					for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++) {
						KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
						KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;

						DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
						setInterAssignInfo.insert(dwPtr);
					}
				}
			}

			// regional
			std::vector<KRegionModePathInfo>* pvecRegionModePathInfo = nullptr;
			pvecRegionModePathInfo = IsHaveRegionalAssignData(nxDzoneID);
			if (nullptr != pvecRegionModePathInfo)
			{
				for (size_t i= 0; i< (*pvecRegionModePathInfo).size(); i++) // m_mapRegionalCodeName 사이즈와 (*pvecRegionModePathInfo).size() 동일
				{
					KRegionModePathInfo &oRegionModePathInfo = (*pvecRegionModePathInfo)[i];

					std::vector<KRegionPathDetail> &vecOnePathDetail = oRegionModePathInfo.vecOnePathDetail;
					for (size_t m = 0; m < vecOnePathDetail.size(); m++)
					{
						KRegionPathDetail &oRegionPathDetail = vecOnePathDetail[m];

						MakeMainPathData(&oRegionModePathInfo, &oRegionPathDetail, vecMainPathInfo);

						CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccAutoAssign,		setInterAssignInfo);
						CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccBusAssign,		setInterAssignInfo);
						CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccRailAssign,		setInterAssignInfo);
						CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccBusRailAssign,	setInterAssignInfo);
						CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrAutoAssign,		setInterAssignInfo);
						CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrBusAssign,		setInterAssignInfo);
						CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrRailAssign,		setInterAssignInfo);
						CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrBusRailAssign,	setInterAssignInfo);
					}
				}
			}
		}

		m_mapDestResultInputValue.clear();
		std::map<int/*regionalmode*/, std::map<KODKey, double>> mapRegionalModeMainTrip;
		KInterModalPathInfo::ResultTransitTripInfo2(m_pTarget, setInterAssignInfo, vecMainPathInfo, OUT m_mapDestResultInputValue, mapRegionalModeMainTrip);

#pragma region Trip Min Max 계산하기
		{ // Trip의 Min Max 계산 하기
			std::map<KODKey, double>	mapWalkingTrip;
			std::map<KODKey, double>    mapAutoTrip;
			std::map<KODKey, double>	mapTransferWalkingB2BTrip;
			std::map<KODKey, double>	mapTransferWalkingB2STrip;
			std::map<KODKey, double>	mapTransferWalkingS2STrip;
			std::map<KODKey, double>	mapTransferWalkingA2TTrip;
			std::map<KODKey, double>	mapTerminalConnetWalkingTrip;

			std::map<Integer, double>	mapBusLinkTrip;
			std::map<Integer, double>	mapSubwayLinkTrip;

			auto iter  = m_mapDestResultInputValue.begin();
			auto itEnd = m_mapDestResultInputValue.end();
			for (; iter != itEnd; ++iter)
			{
				TInMapResultInputValue &oInMapResultInputValue = iter->second;

				// 2018.08.17 명지대 요청사항
				//mapWalkingTrip.insert(oInMapResultInputValue.mapWalkingTrip.begin(), oInMapResultInputValue.mapWalkingTrip.end());
				//mapAutoTrip.insert(oInMapResultInputValue.mapAutoTrip.begin(), oInMapResultInputValue.mapAutoTrip.end());
				//mapTransferWalkingB2BTrip.insert(oInMapResultInputValue.mapTransferWalkingB2BTrip.begin(), oInMapResultInputValue.mapTransferWalkingB2BTrip.end());
				//mapTransferWalkingB2STrip.insert(oInMapResultInputValue.mapTransferWalkingB2STrip.begin(), oInMapResultInputValue.mapTransferWalkingB2STrip.end());
				//mapTransferWalkingS2STrip.insert(oInMapResultInputValue.mapTransferWalkingS2STrip.begin(), oInMapResultInputValue.mapTransferWalkingS2STrip.end());
				//mapTransferWalkingA2TTrip.insert(oInMapResultInputValue.mapTransferWalkingA2TTrip.begin(), oInMapResultInputValue.mapTransferWalkingA2TTrip.end());
				//mapTerminalConnetWalkingTrip.insert(oInMapResultInputValue.mapTerminalConnetWalkingTrip.begin(), oInMapResultInputValue.mapTerminalConnetWalkingTrip.end());

				//mapBusLinkTrip.insert(oInMapResultInputValue.mapBusLinkTrip.begin(), oInMapResultInputValue.mapBusLinkTrip.end());
				//mapSubwayLinkTrip.insert(oInMapResultInputValue.mapSubwayLinkTrip.begin(), oInMapResultInputValue.mapSubwayLinkTrip.end());
				
				KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapWalkingTrip, mapWalkingTrip);
				KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapAutoTrip, mapAutoTrip);
				KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTransferWalkingB2BTrip, mapTransferWalkingB2BTrip);
				KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTransferWalkingB2STrip, mapTransferWalkingB2STrip);
				KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTransferWalkingS2STrip, mapTransferWalkingS2STrip);
				KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTransferWalkingA2TTrip, mapTransferWalkingA2TTrip);

				KInterModalPathInfo::updateLinkTripMap(oInMapResultInputValue.mapBusLinkTrip, mapBusLinkTrip);
				KInterModalPathInfo::updateLinkTripMap(oInMapResultInputValue.mapSubwayLinkTrip, mapSubwayLinkTrip);
				// 2018.08.17 명지대 요청사항 끝
			}

			if (mapAutoTrip.size() < 1 && 
				mapRegionalModeMainTrip.size() < 1 &&
				mapBusLinkTrip.size() < 1 && 
				mapSubwayLinkTrip.size() < 1) {

				InitVolMinMax();
			} 
			else {
				double dMaxVolume = -999999999999.0;
				double dMinVolume = 999999999999.0;

				//Bus
				CollectMinMaxLinkTrip(mapBusLinkTrip, dMaxVolume, dMinVolume);
				//Rail
				CollectMinMaxLinkTrip(mapSubwayLinkTrip, dMaxVolume, dMinVolume);

				std::map<KODKey, double> mapODVolume;
				CollectUserODVolume(mapTransferWalkingB2BTrip, mapODVolume); // 버스에서 버스로 환승 도보
				CollectUserODVolume(mapTransferWalkingB2STrip, mapODVolume); // 버스에서 전철로 환승 도보
				CollectUserODVolume(mapTransferWalkingS2STrip, mapODVolume); // 전철에서 전철로
				CollectUserODVolume(mapTransferWalkingA2TTrip, mapODVolume); // Auto 에서 Transit : kiss&ride 등...
				// oInMapResultTripInfo.mapWalkingTrip 제외
				CollectUserODVolume(mapTerminalConnetWalkingTrip, mapODVolume); // access or egress 에서 터미널
				CollectUserODVolume(mapAutoTrip, mapODVolume); // Auto Trip

				AutoType iterA  = mapRegionalModeMainTrip.begin();
				AutoType itEndA = mapRegionalModeMainTrip.end();
				while (iterA != itEndA)
				{
					std::map<KODKey, double> &mapMainTrip = iterA->second;
					CollectUserODVolume(mapMainTrip, mapODVolume); // main Trip
					++iterA;
				}

				CollectMinMaxUserODVolume(mapODVolume, dMaxVolume, dMinVolume);

				m_dMaxVolume = dMaxVolume;
				m_dMinVolume = dMinVolume;
			}

			UpdateMinMaxTrip();
		}
#pragma endregion Trip Min Max 계산하기
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KInterModalResultDlg::DrawOrgDestZoneAccess(bool a_bRefresh)
{
	CXTPReportRecords* pRecords = m_ctrlReportDest.GetRecords();
	int nCount = pRecords->GetCount();

	try
	{
		std::vector<TDrawZoneAccess> vecZoneAccess;
		Integer nxOrgZoneID  = GetSelStartNodeId();

		// Org 그리기
		TDrawZoneAccess oDrawZoneAccess;
		oDrawZoneAccess.TNodeID         = nxOrgZoneID;
		oDrawZoneAccess.TSymbol			= 48;//임시 모양
		oDrawZoneAccess.TColorRGB       = RGB(255,0,0);
		oDrawZoneAccess.TLabel          = m_oStartNodeIdCaption.TID;
		oDrawZoneAccess.TSize           = 18.0; //임시 사이즈
		oDrawZoneAccess.TRadius         = m_dRadius;

		vecZoneAccess.push_back(oDrawZoneAccess);

		// Dest 그리기
		for (int i=0; i<nCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_E_COLUMN_SELECT);
			if (pItem->IsChecked() == FALSE) {
				continue;
			}

			pItem             = pRecord->GetItem(_1_E_COLUMN_NAME);
			Integer nxDZoneID = (Integer)pItem->GetItemData();

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxDZoneID);

			oDrawZoneAccess.TNodeID         = nxDZoneID;
			oDrawZoneAccess.TColorRGB       = RGB(0,0,205);
			oDrawZoneAccess.TLabel          = strNodeLable;

			vecZoneAccess.push_back(oDrawZoneAccess);
		}

		m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, a_bRefresh);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalResultDlg::OnBnClickedCheckOutrange()
{
	UpdateOutRange();
}


void KInterModalResultDlg::UpdateOutRange()
{
	if(IsDlgButtonChecked(IDC_CHECK_OUTRANGE) == BST_CHECKED)
	{
		GetDlgItem(IDC_CLR_OUT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_CLR_OUT)->ShowWindow(SW_HIDE);
	}
}


void KInterModalResultDlg::OnBnClickedCheckLabel()
{
	try
	{
		bool bLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bLabel = true;
		else
			bLabel = false;

		//m_pMapView->CacheShowCompositeNodeLabel    (bLabel, false);
		m_pMapView->CacheShowCompositeUserLineLabel(bLabel, false);
		m_pMapView->CacheShowCompositeLinkLabel    (bLabel, true);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KInterModalResultDlg::CloseWindowProc()
{
	AllClearPath(true);

	if (nullptr != m_spInterModalResultDetailDlg)
		m_spInterModalResultDetailDlg.reset();

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();

	if (nullptr != m_spPathCompareUrbanDlg)
	{
		m_spPathCompareUrbanDlg.reset();
	}

	if (nullptr != m_spPathCompareRegionDlg)
	{
		m_spPathCompareRegionDlg.reset();
	}
}


void KInterModalResultDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}



// 2018.08.17 명지대 요청사항 수정
void KInterModalResultDlg::updateMinMaxEditControl()
{
	TxLogDebugStartMsg();
	try
	{
		std::set<DWORD_PTR>        setInterAssignInfo;
		std::vector<TMainPathInfo> vecDrawMainPathInfo;

		CXTPReportRecordItem*  pItem = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i = 0; i< nCount; i++)
		{
			CXTPReportRecord*   pParentRecord = pRecords->GetAt(i);
			pItem = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
			Integer nxDZoneID = (Integer)pItem->GetItemData();

			AutoType itFindPathMode = m_mapEndZonePathMode.find(nxDZoneID);
			if (itFindPathMode == m_mapEndZonePathMode.end())
				continue;
			KEMInterModalPathMode emInterModalPathMode = itFindPathMode->second;

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			if (nullptr == pSeParentRecords)
			{
				continue;
			}

			if (KEMInterModalPathModeUrban == emInterModalPathMode) // KEMInterModalPathModeRegional
			{
				GetSelectedUrbanTransitInfo(pSeParentRecords, setInterAssignInfo);
			}
			else
			{
				GetSelectedRegionalTransitInfo(pSeParentRecords, setInterAssignInfo, vecDrawMainPathInfo);
			}
		}

		if (setInterAssignInfo.size()< 1 && vecDrawMainPathInfo.size()< 1)
		{
			throw 1;
		}

		std::map<KODKey, double>	mapWalkingTrip;
		std::map<KODKey, double>    mapAutoTrip;
		std::map<KODKey, double>	mapTransferWalkingB2BTrip;
		std::map<KODKey, double>	mapTransferWalkingB2STrip;
		std::map<KODKey, double>	mapTransferWalkingS2STrip;
		std::map<KODKey, double>	mapTransferWalkingA2TTrip; // Auto 에서 Transit : kiss&ride 등...
		std::map<KODKey, double>	mapTerminalConnetWalkingTrip;
		std::map<int/*regionalmode*/, std::map<KODKey, double>> mapRegionalModeMainTrip;

		std::map<Integer, double>	mapBusLinkTrip;
		std::map<Integer, double>	mapSubwayLinkTrip;

		auto iterKey = setInterAssignInfo.begin();
		auto itKeyEnd = setInterAssignInfo.end();
		for (; iterKey != itKeyEnd; ++iterKey)
		{
			DWORD_PTR dwPtr = *iterKey;

			auto itFind = m_mapDestResultInputValue.find(dwPtr);
			if (itFind == m_mapDestResultInputValue.end()) {
				continue;
			}

			TInMapResultInputValue &oInMapResultInputValue = itFind->second;

			mapWalkingTrip.insert(oInMapResultInputValue.mapWalkingTrip.begin(), oInMapResultInputValue.mapWalkingTrip.end());

			// 2018.08.01 명지대 요청사항 수정
			KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapAutoTrip, mapAutoTrip);
			KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTransferWalkingB2BTrip, mapTransferWalkingB2BTrip);
			KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTransferWalkingB2STrip, mapTransferWalkingB2STrip);
			KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTransferWalkingS2STrip, mapTransferWalkingS2STrip);
			KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTransferWalkingA2TTrip, mapTransferWalkingA2TTrip);
			KInterModalPathInfo::updateODTripMap(oInMapResultInputValue.mapTerminalConnetWalkingTrip, mapTerminalConnetWalkingTrip);

			KInterModalPathInfo::updateLinkTripMap(oInMapResultInputValue.mapBusLinkTrip, mapBusLinkTrip);
			KInterModalPathInfo::updateLinkTripMap(oInMapResultInputValue.mapSubwayLinkTrip, mapSubwayLinkTrip);
		}

		int nMaxTrip = 0;
		int nAutoTrip = getMaxODTrip(mapAutoTrip);
		if (nMaxTrip < nAutoTrip)
		{
			nMaxTrip = nAutoTrip;
		}

		int nWalkingB2BTrip = getMaxODTrip(mapTransferWalkingB2BTrip);
		if (nMaxTrip < nWalkingB2BTrip)
		{
			nMaxTrip = nWalkingB2BTrip;
		}

		int nWalkingB2STrip = getMaxODTrip(mapTransferWalkingB2STrip);
		if (nMaxTrip < nWalkingB2STrip)
		{
			nMaxTrip = nWalkingB2STrip;
		}

		int nWalkingS2STrip = getMaxODTrip(mapTransferWalkingS2STrip);
		if (nMaxTrip < nWalkingS2STrip)
		{
			nMaxTrip = nWalkingS2STrip;
		}

		int nWalkingA2TTrip= getMaxODTrip(mapTransferWalkingA2TTrip);
		if (nMaxTrip < nWalkingA2TTrip)
		{
			nMaxTrip = nWalkingA2TTrip;
		}

		int nTCWalingTrip = getMaxODTrip(mapTerminalConnetWalkingTrip);
		if (nMaxTrip < nTCWalingTrip)
		{
			nMaxTrip = nTCWalingTrip;
		}

		int nBusTrip = getMaxLinkTrip(mapBusLinkTrip);
		if (nMaxTrip < nBusTrip)
		{
			nMaxTrip = nBusTrip;
		}

		int nSubwayTrip = getMaxLinkTrip(mapSubwayLinkTrip);
		if (nMaxTrip < nSubwayTrip)
		{
			nMaxTrip = nSubwayTrip;
		}

		CString strTrip;
		strTrip.Format(_T("%d"), nMaxTrip);
		
		SetDlgItemText(IDC_EDIT_MAXTRIP, strTrip);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}

int KInterModalResultDlg::getMaxODTrip(std::map<KODKey, double>& mapOD)
{
	double nMaxValue = 0;
	for (std::map<KODKey, double>::iterator it = mapOD.begin(); it != mapOD.end(); ++it)
	{
		if (it->second > nMaxValue)
		{
			nMaxValue = it->second;
		}
	}

	return ceil(nMaxValue);
}


int KInterModalResultDlg::getMaxLinkTrip(std::map<Integer, double>& mapLink)
{
	double nMaxValue = 0;
	for (std::map<Integer, double>::iterator it = mapLink.begin(); it != mapLink.end(); ++it)
	{
		if (it->second > nMaxValue)
		{
			nMaxValue = it->second;
		}
	}

	return ceil(nMaxValue);
}
// 2018.08.17 명지대 요청사항 수정 끝


void KInterModalResultDlg::OnBnClickedBtnComparePath()
{
	std::vector<KInterPathEditInfoWrapper> arrUrban;
	std::vector<KInterPathRegionInfoWrapper> arrRegion;

	try
	{
		CXTPReportRecordItem*  pItem = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CXTPReportRecord*   pParentRecord = pRecords->GetAt(i);
			pItem = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
			Integer nxDZoneID = (Integer)pItem->GetItemData();

			AutoType itFindPathMode = m_mapEndZonePathMode.find(nxDZoneID);
			if (itFindPathMode == m_mapEndZonePathMode.end())
				continue;
			KEMInterModalPathMode emInterModalPathMode = itFindPathMode->second;

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			if (nullptr == pSeParentRecords)
			{
				continue;
			}

			CXTPReportRecordItem* pItem = nullptr;
			for (int j = 0; j < pSeParentRecords->GetCount(); ++j)
			{
				CXTPReportRecord* pSeParentRecord = pSeParentRecords->GetAt(j);
				pItem = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				if (pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
				{
					continue;
				}

				CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
				if (nullptr == pThParentRecords)
				{
					continue;
				}

				// 도시부 경로
				if (KEMInterModalPathModeUrban == emInterModalPathMode)
				{
					for (int m = 0; m < pThParentRecords->GetCount(); m++)
					{
						CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
						pItem = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

						if (pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
							continue;

						KInterPathEditInfo* pAssignInfo = (KInterPathEditInfo*)pItem->GetItemData();
						if (NULL != pAssignInfo)
						{
							KInterPathEditInfoWrapper wr;
							std::map<Integer, CString>::iterator itNode = m_mapZoneNodeData.find(pAssignInfo->nxDestinationID);
							if (m_mapZoneNodeData.end() != itNode)
							{
								wr.strDestNode.Format(_T("%I64d [%s]"), pAssignInfo->nxDestinationID, itNode->second);
							}
							else
							{
								wr.strDestNode.Format(_T("%I64d [-]"), pAssignInfo->nxDestinationID);
							}
							wr.strPathName = pItem->GetCaption();
							wr.pInfo = pAssignInfo;
							arrUrban.push_back(wr);
						}
					}
				}
				else
				{
					// 지역간 경로
					KRegionModePathInfo* pRegionModePathInfo = (KRegionModePathInfo*)pItem->GetItemData();

					CString strDestNode = _T("");
					std::map<Integer, CString>::iterator itNode = m_mapZoneNodeData.find(pRegionModePathInfo->nxDZoneID);
					if (m_mapZoneNodeData.end() != itNode)
					{
						strDestNode.Format(_T("%I64d [%s]"), pRegionModePathInfo->nxDZoneID, itNode->second);
					}
					else
					{
						strDestNode.Format(_T("%I64d [-]"), pRegionModePathInfo->nxDZoneID);
					}

					for (int m = 0; m < pThParentRecords->GetCount(); m++)
					{
						CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
						pItem = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

						if (pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
							continue;

						KRegionPathDetail* pRegionPathDetail = (KRegionPathDetail*)pItem->GetItemData();
						CString strPathName1 = pItem->GetCaption();
						CString strFromTerminal = _T("");
						CString strToTerminal = _T("");
						if (pRegionModePathInfo->nMode != REGION_MODE_CODE_AUTO)
						{
							std::map<Integer, CString>::iterator itTerminal = m_mapZoneNodeData.find(pRegionPathDetail->nxOrgTermID);
							if (m_mapZoneNodeData.end() != itTerminal)
							{
								strFromTerminal.Format(_T("%I64d [%s]"), pRegionPathDetail->nxOrgTermID, itTerminal->second);
							}
							else
							{
								strFromTerminal.Format(_T("%I64d [-]"), pRegionPathDetail->nxOrgTermID);
							}

							itTerminal = m_mapZoneNodeData.find(pRegionPathDetail->nxDesTermID);
							if (m_mapZoneNodeData.end() != itTerminal)
							{
								strToTerminal.Format(_T("%I64d [%s]"), pRegionPathDetail->nxDesTermID, itTerminal->second);
							}
							else
							{
								strToTerminal.Format(_T("%I64d [-]"), pRegionPathDetail->nxDesTermID);
							}
						}

						KInterPathRegionInfoWrapper wr;
						wr.strDestNode = strDestNode;
						wr.strFromTerminal = strFromTerminal;
						wr.strToTerminal = strToTerminal;
						wr.strPathName = strPathName1;
						wr.pRegionPathInfo = pRegionModePathInfo;
						wr.pRegionPathDetail = pRegionPathDetail;
						arrRegion.push_back(wr);
					}
				}
			}	// for (int j = 0; j < pSeParentRecords->GetCount(); ++j)
		}	// for (int i = 0; i < nCount; i++)
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}

	bool bShowUrban = false;
	if (!arrUrban.empty())
	{
		if (nullptr != m_spPathCompareUrbanDlg)
		{
			m_spPathCompareUrbanDlg.reset();
		}
		m_spPathCompareUrbanDlg = std::make_shared<KSelectedPathCompareUrbanDlg>(false, EMPathCompareMode_Intermodal, this);
		m_spPathCompareUrbanDlg->SetInterPathEditInfos(arrUrban);
		m_spPathCompareUrbanDlg->Create(IDD_6929_KSelectedPathCompareUrban, this);
		CString strDlgCaption = _T("");
		m_spPathCompareUrbanDlg->GetWindowText(strDlgCaption);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			m_spPathCompareUrbanDlg->SetWindowText(strDlgCaption + _T("(도시부)"));
		}
		else
		{
			m_spPathCompareUrbanDlg->SetWindowText(strDlgCaption + _T("(Urban)"));
		}
		
		m_spPathCompareUrbanDlg->ShowWindow(SW_SHOW);
		bShowUrban = true;
	}
	if (!arrRegion.empty())
	{
		if (nullptr != m_spPathCompareRegionDlg)
		{
			m_spPathCompareRegionDlg.reset();
		}

		m_spPathCompareRegionDlg = std::make_shared<KSelectedPathCompareRegionDlg>(false, this);
		m_spPathCompareRegionDlg->SetInterPathRegionInfos(arrRegion);
		m_spPathCompareRegionDlg->Create(IDD_6929_KSelectedPathCompareRegion, this);
		if (bShowUrban && (m_spPathCompareUrbanDlg != nullptr))
		{
			CRect rectUrban;
			m_spPathCompareUrbanDlg->GetWindowRect(&rectUrban);

			CRect rect;
			m_spPathCompareRegionDlg->GetWindowRect(rect);
			m_spPathCompareRegionDlg->MoveWindow(rectUrban.left, rectUrban.top + rectUrban.Height(), rect.Width(), rect.Height());
		}		
		m_spPathCompareRegionDlg->ShowWindow(SW_SHOW);
	}
}



void KInterModalResultDlg::UpdatePathCompareButtonState()
{
	BOOL bEnableButton = FALSE;
	CXTPReportRecordItem*  pItem = nullptr;
	CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for (int i = 0; i < nRecordCount; i++)
	{
		CXTPReportRecord*   pParentRecord = pRecords->GetAt(i);
		CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
		if (nullptr == pSeParentRecords)
		{
			continue;
		}

		CXTPReportRecordItem* pItem = nullptr;
		for (int j = 0; j < pSeParentRecords->GetCount(); ++j)
		{
			CXTPReportRecord* pSeParentRecord = pSeParentRecords->GetAt(j);
			pItem = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

			if (pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
			{
				continue;
			}
			else
			{
				bEnableButton = TRUE;
				break;
			}

			//CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
			//if (nullptr == pThParentRecords)
			//{
			//	continue;
			//}

			//for (int m = 0; m < pThParentRecords->GetCount(); m++)
			//{
			//	CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
			//	pItem = pThParentRecord->GetItem(_0_B_COLUMN_ZONEID);

			//	if (pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
			//	{
			//		continue;
			//	}
			//	else
			//	{
			//		bEnableButton = TRUE;
			//		break;
			//	}
			//}
		}	// for (int j = 0; j < pSeParentRecords->GetCount(); ++j)
	}	// for (int i = 0; i < nRecordCount; i++)

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(bEnableButton);
}
