// InterTransitPathEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterTransitPathEditDlg.h"
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

#include "KDBaseAccessibilityModify.h"



const int COLUMN_WIDTH_USE = 55;	// 사용여부 컬럼 너비
const int COLUMN_WIDTH_INFO = 50;	// 상세정보 컬럼 너비


namespace InterTransitENUM
{
	enum
	{
		_0_A_COLUMN_ISAUTO     = 0,
		_1_A_COLUMN_ISBUS      = 1,
		_2_A_COLUMN_ISRAIL     = 2,
		_3_A_COLUMN_ISBUSR     = 3,
		_4_A_COLUMN_ISPARKR    = 4,
		_5_A_COLUMN_ISKISSR    = 5,
		_6_A_COLUMN_ISTAXITR   = 6
	};

	enum
	{
		_0_B_COLUMN_ZONEID        = 0,
		_1_B_COLUMN_TOTAL_TIME    = 1,
		_2_B_COLUMN_LENGTH        = 2,
		_3_B_COLUMN_COST          = 3,
		_4_B_COLUMN_USE           = 4,
		_5_B_COLUMN_DETAIL        = 5
	};
	enum 
	{
		B_COLUMN_TOTAL_COUNT     = 6
	};

	enum
	{
		_0_D_COLUMN_ZONEID        = 0,
		_1_D_COLUMN_TOTAL_TIME    = 1,
		_2_D_COLUMN_WAIT_TIME     = 2,
		_3_D_COLUMN_TRANSFER_TIME = 3,
		_4_D_COLUMN_PROCESS_TIME  = 4,
		_5_D_COLUMN_FREQUENCY     = 5,
		_6_D_COLUMN_LENGTH        = 6,
		_7_D_COLUMN_COST          = 7,
		_8_D_COLUMN_USE           = 8,
		_9_D_COLUMN_DETAIL        = 9
	};
	enum 
	{
		D_COLUMN_TOTAL_COUNT     = 10
	};

	enum
	{
		EXPANDALL_LIMIT_COUNT    = 10 
	};
}

using namespace InterTransitENUM;
// KInterTransitPathEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterTransitPathEditDlg, KResizeDialogEx)

KInterTransitPathEditDlg::KInterTransitPathEditDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterTransitPathEditDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_dRadius(1000.0)
{
	m_oStartNodeIdCaption.TID = _T("-1");
	m_oEndNodeIdCaption.TID   = _T("-1");
	
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

	// 2019.03.12 명지대 요청사항 수정, 2019.04.02 지역간 대화상자 추가
	m_spPathCompareUrbanDlg = nullptr;
	m_spPathCompareRegionDlg = nullptr;
	// 2019.03.12 명지대 요청사항 수정 끝
}

KInterTransitPathEditDlg::~KInterTransitPathEditDlg()
{
	CloseWindowProc();
	m_backBrush.DeleteObject();
}

void KInterTransitPathEditDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,                 m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT_C,                 m_wndReportCCtrl);
	DDX_Control(pDX, IDC_REPORT_B,                 m_wndReportBCtrl);
	DDX_Control(pDX, IDC_REPORT_D,                 m_wndReportDCtrl);
	DDX_Control(pDX, IDC_REPORT_COLORA,            m_wndReportColorACtrl);
	DDX_Control(pDX, IDC_REPORT_COLORB,            m_wndReportColorBCtrl);
}


BEGIN_MESSAGE_MAP(KInterTransitPathEditDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &KInterTransitPathEditDlg::OnBnClickedButtonStartNodeSearch)
	ON_BN_CLICKED(IDC_BUTTON2, &KInterTransitPathEditDlg::OnBnClickedButtonEndNodeSearch)
	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_B, &KInterTransitPathEditDlg::OnReportBItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_B, &KInterTransitPathEditDlg::OnReportBCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_A, &KInterTransitPathEditDlg::OnReportACheckItem)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED,    IDC_REPORT_B, &KInterTransitPathEditDlg::OnReportBValueChanged)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT_B, &KInterTransitPathEditDlg::OnReportBItemButtonClick)

 	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_D, &KInterTransitPathEditDlg::OnReportDItemClick)
 	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_D, &KInterTransitPathEditDlg::OnReportDCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_C, &KInterTransitPathEditDlg::OnReportCCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED,    IDC_REPORT_D, &KInterTransitPathEditDlg::OnReportDValueChanged)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT_D, &KInterTransitPathEditDlg::OnReportDItemButtonClick)

	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_COLORA, &KInterTransitPathEditDlg::OnReportColorAItemClick)
	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_COLORB, &KInterTransitPathEditDlg::OnReportColorBItemClick)

	ON_BN_CLICKED(IDC_BUTTON1,     &KInterTransitPathEditDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON5,     &KInterTransitPathEditDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL,        &KInterTransitPathEditDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3,     &KInterTransitPathEditDlg::OnBnClickedButtonApply)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_COMPARE_PATH, &KInterTransitPathEditDlg::OnBnClickedBtnComparePath)
END_MESSAGE_MAP()


// KInterTransitPathEditDlg 메시지 처리기입니다.
void KInterTransitPathEditDlg::InitializeNode()
{
	CWaitCursor wc;

	GetRegionTransitType();
	GetNodeInfo();

	m_oStartNodeIdCaption.TID = _T("-1");
	SetDlgItemText(IDC_EDIT1, _T(""));	

	InitDestination(true);	
	InitReportCHeader();
	UpdateReportColorB();
}


bool KInterTransitPathEditDlg::GetNodeInfo(void)
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


void KInterTransitPathEditDlg::GetRegionTransitType(void)
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


void KInterTransitPathEditDlg::SetNodeViewData(KIDCaption &a_oIDCaption, CString &a_strNodeViewData)
{
	try
	{
		CString strZoneID = a_oIDCaption.TID;
		Integer nxZoneID = _ttoi64(strZoneID);

		AutoType iter  = m_mapZoneNodeData.find(nxZoneID);
		AutoType itEnd = m_mapZoneNodeData.end();

		CString strName(_T(""));
		if (iter != itEnd)
		{
			strName = iter->second;
		}

		a_oIDCaption.TCaption = strName;

		ConvertEmptyNodeName(a_oIDCaption.TCaption);

		a_strNodeViewData.Format(_T("%s [%s]"), a_oIDCaption.TID, a_oIDCaption.TCaption);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KInterTransitPathEditDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KInterTransitPathEditDlg::ResizeComponent()
{
	SetResize(IDC_EDIT1,                		  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON4,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR5,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_EDIT2,                		  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON2,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_REPORT_A,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_C,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR8,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_REPORT_B,       			      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_REPORT_D,       			      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR9,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC1,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_REPORT_COLORA,       			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_CENTER);
	SetResize(IDC_SEPRATOR10,     			      SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
	SetResize(IDC_STATIC2,       			      SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
	SetResize(IDC_REPORT_COLORB,       			  SZ_BOTTOM_CENTER,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR7,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON5,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON1,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_COMPARE_PATH,				  SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON3,       			      SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


BOOL KInterTransitPathEditDlg::OnInitDialog()
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

	// 2019.04.02 : 편집 부분에서는 경로비교를 안함.
	GetDlgItem(IDC_BTN_COMPARE_PATH)->ShowWindow(SW_HIDE);

	CWaitCursor wc;

	KReportCtrlSetting::Default(m_wndReportACtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_wndReportCCtrl, TRUE, FALSE, FALSE);

	KReportCtrlSetting::DefaultC(m_wndReportBCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportBCtrl);
	KReportCtrlSetting::DefaultC(m_wndReportDCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportDCtrl);

	KReportCtrlSetting::Default(m_wndReportColorACtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::HideHeader(m_wndReportColorACtrl);
	KReportCtrlSetting::Default(m_wndReportColorBCtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::HideHeader(m_wndReportColorBCtrl);

	InitReportAHeader();
	/*InitReportCHeader();*/
	InitReportBHeader();
	InitReportDHeader();

	InitReportColorAHeader();
	UpdateReportColorA();
	InitReportColorBHeader();
	
	ExchangeRegionCode(KEMInterModalPathModeUrban);

	m_backBrush.CreateSolidBrush(RGB(255,255,255));
	ResizeComponent();

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterTransitPathEditDlg::ModelessOKFeedback( UINT nDialogID)
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

				CString strNodeView(_T(""));
				if (true == m_bStart)
				{
					bool bRefresh(false);
					SetZoneNodeViewData(strID, strNodeView, m_oStartNodeIdCaption);
					SetDlgItemText(IDC_EDIT1,  strNodeView);
					InitDestination(bRefresh);
					FindPath();

					bRefresh = true;
					DrawOrgZoneAccess(nxOrgZoneID, bRefresh);
				}
				else
				{
					SetZoneNodeViewData(strID, strNodeView, m_oEndNodeIdCaption);
					SetDlgItemText(IDC_EDIT2,  strNodeView);

					// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
					int nLang = 1;
					if (KmzSystem::GetLanguage() == KEMKorea)
					{
						nLang = 0;
					}

					QBicSimpleProgressThread::ExecuteThread(MakeDrawDataThreadCaller, this, false, nLang);

					UpdateReportData();
				}
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KInterTransitPathEditDlg::InitDestination(bool a_bRefreh)
{
	m_oEndNodeIdCaption.TID = _T("-1");
	SetDlgItemText(IDC_EDIT2, _T(""));	

	m_wndReportACtrl.ResetContent();
	m_wndReportACtrl.Populate();
	m_wndReportCCtrl.ResetContent();
	m_wndReportCCtrl.Populate();

	m_wndReportBCtrl.ResetContent();
	m_wndReportBCtrl.Populate();
	m_wndReportDCtrl.ResetContent();
	m_wndReportDCtrl.Populate();

	AllClearPath(a_bRefreh);

	m_mapEndZonePathMode.clear();
	m_vecEndZoneNodeIDCaption.clear();
	m_vecEndZoneNodeID.clear();
	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(FALSE);
}


void KInterTransitPathEditDlg::DrawOrgZoneAccess(Integer a_nxID, bool a_bRefresh)
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

void KInterTransitPathEditDlg::InitReportAHeader()
{
	try
	{
		m_mapUrbanCodeName.clear();

		CXTPReportColumn*                pColumn      = nullptr;

		pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_0_A_COLUMN_ISAUTO,       _T("PassengerCar/Taxi"),      16));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTooltip(_T("PassengerCar/Taxi"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(_0_A_COLUMN_ISAUTO, _T("승용차/택시")));
			pColumn->SetCaption(_T("승용차/택시"));
			pColumn->SetTooltip(_T("승용차/택시"));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(_0_A_COLUMN_ISAUTO, _T("PassengerCar/Taxi")));
			pColumn->SetCaption(_T("PassengerCar/Taxi"));
		}

		pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_1_A_COLUMN_ISBUS,        _T("Bus"),            14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTooltip(_T("Bus"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(_1_A_COLUMN_ISBUS, _T("버스")));
			pColumn->SetCaption(_T("버스"));
			pColumn->SetTooltip(_T("버스"));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(_1_A_COLUMN_ISBUS, _T("Bus")));
			pColumn->SetCaption(_T("Bus"));
		}

		pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_2_A_COLUMN_ISRAIL,       _T("Rail"),         14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTooltip(_T("Rail"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(_2_A_COLUMN_ISRAIL, _T("도시철도")));
			pColumn->SetCaption(_T("도시철도"));
			pColumn->SetTooltip(_T("도시철도"));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(_2_A_COLUMN_ISRAIL, _T("Rail")));
			pColumn->SetCaption(_T("Rail"));
		}

		pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_3_A_COLUMN_ISBUSR,       _T("B+R"),            14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTooltip(_T("Bus+Rail"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(_3_A_COLUMN_ISBUSR, _T("버스+도시철도")));
			pColumn->SetCaption(_T("버스+도시철도"));
			pColumn->SetTooltip(_T("버스+도시철도"));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(_3_A_COLUMN_ISBUSR, _T("Bus+Rail")));
			pColumn->SetCaption(_T("Bus+Rail"));
		}

		pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_4_A_COLUMN_ISPARKR,      _T("P & R"),            14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTooltip(_T("Park & Ride"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(_4_A_COLUMN_ISPARKR, _T("Park & Ride")));
			pColumn->SetCaption(_T("Park & Ride"));
			pColumn->SetTooltip(_T("Park & Ride"));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(_4_A_COLUMN_ISPARKR, _T("Park & Ride")));
			pColumn->SetCaption(_T("Park & Ride"));
		}

		pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_5_A_COLUMN_ISKISSR,      _T("K & R"),            14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTooltip(_T("Kiss & Ride"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(_5_A_COLUMN_ISKISSR, _T("Kiss & Ride")));
			pColumn->SetCaption(_T("Kiss & Ride"));
			pColumn->SetTooltip(_T("Kiss & Ride"));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(_5_A_COLUMN_ISKISSR, _T("Kiss & Ride")));
			pColumn->SetCaption(_T("Kiss & Ride"));
		}

		pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_6_A_COLUMN_ISTAXITR,     _T("T & T"),            14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTooltip(_T("Taxi & Transit"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(_6_A_COLUMN_ISTAXITR, _T("택시 & 대중교통")));
			pColumn->SetCaption(_T("택시 & 대중교통"));
			pColumn->SetTooltip(_T("택시 & 대중교통"));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(_6_A_COLUMN_ISTAXITR, _T("Taxi & Transit")));
			pColumn->SetCaption(_T("Taxi & Transit"));
		}

		m_wndReportACtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KInterTransitPathEditDlg::InitReportCHeader()
{
	try
	{
		m_wndReportCCtrl.ResetContent();
		m_wndReportCCtrl.GetColumns()->Clear();
		m_wndReportCCtrl.Populate();

		CXTPReportColumn*                pColumn      = nullptr;

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


void KInterTransitPathEditDlg::InitReportBHeader()
{
	try
	{
		CXTPReportColumn*                pColumn      = nullptr;
		CXTPReportRecordItemEditOptions* pEditOptions = nullptr;

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_B_COLUMN_ZONEID,       _T("Paths"),    38));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTreeColumn(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("경로구분"));
		}

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_B_COLUMN_TOTAL_TIME,   _T("Time"),       14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("통행시간"));
		}

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_B_COLUMN_LENGTH,       _T("Length"),     14));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("통행거리"));
		}

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_3_B_COLUMN_COST,         _T("Cost"),       16));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("통행비용"));
		}

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_4_B_COLUMN_USE,          _T("Use"), COLUMN_WIDTH_USE, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("사용여부"));
		}
		pEditOptions = pColumn->GetEditOptions();
		pEditOptions->AddComboButton(TRUE);
		pEditOptions->m_bAllowEdit = TRUE;
		pEditOptions->m_bExpandOnSelect = TRUE;
		InputGridComboData(pEditOptions);

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_5_B_COLUMN_DETAIL,       _T("Info"), COLUMN_WIDTH_INFO, FALSE));
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


void KInterTransitPathEditDlg::InitReportDHeader()
{
	try
	{
		CXTPReportColumn*                pColumn      = nullptr;
		CXTPReportRecordItemEditOptions* pEditOptions = nullptr;

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_0_D_COLUMN_ZONEID,       _T("Paths"),    30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTreeColumn(TRUE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("경로구분"));
        }

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_1_D_COLUMN_TOTAL_TIME,     _T("Travel Time"),       10));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행시간"));
        }

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_2_D_COLUMN_WAIT_TIME,      _T("Setup Time"),       10));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("대기시간"));
        }

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_3_D_COLUMN_TRANSFER_TIME,  _T("Transfer Time"),       10));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("환승시간"));
        }

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_4_D_COLUMN_PROCESS_TIME,   _T("Process Time"),       10));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("탑승수속시간"));
        }

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_5_D_COLUMN_FREQUENCY,      _T("Frequency"),       10));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("운행회수"));
        }

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_6_D_COLUMN_LENGTH,         _T("Length"),     10));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행거리"));
        }

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_7_D_COLUMN_COST,         _T("Cost"),       10));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행비용"));
        }

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_8_D_COLUMN_USE,          _T("Use"), COLUMN_WIDTH_USE, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("사용여부"));
        }

		pEditOptions = pColumn->GetEditOptions();
		pEditOptions->AddComboButton(TRUE);
		pEditOptions->m_bAllowEdit = TRUE;
		pEditOptions->m_bExpandOnSelect = TRUE;
		InputGridComboData(pEditOptions);

		pColumn = m_wndReportDCtrl.AddColumn(new CXTPReportColumn(_9_D_COLUMN_DETAIL,       _T("Info"), COLUMN_WIDTH_INFO, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("상세정보"));
        }

		m_wndReportDCtrl.Populate();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KInterTransitPathEditDlg::InitReportColorAHeader()
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


void KInterTransitPathEditDlg::UpdateReportColorA()
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


void KInterTransitPathEditDlg::InitReportColorBHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportColorBCtrl.AddColumn(new CXTPReportColumn(0,       _T("mode"),           100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReportColorBCtrl.AddColumn(new CXTPReportColumn(1,       _T("Color"),           50,    FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("색상"));
    }


	m_wndReportColorBCtrl.Populate();
}


void KInterTransitPathEditDlg::UpdateReportColorB()
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


void KInterTransitPathEditDlg::InputGridComboData(CXTPReportRecordItemEditOptions* a_pEditOptions)
{
    if (KmzSystem::GetLanguage() == KEMKorea) {
        a_pEditOptions->AddConstraint(_T("씀"),   (DWORD_PTR)1 ); 
        a_pEditOptions->AddConstraint(_T("안 씀"), (DWORD_PTR)0 );
    }
    else {
        a_pEditOptions->AddConstraint(_T("Use"),   (DWORD_PTR)1 ); 
        a_pEditOptions->AddConstraint(_T("Unuse"), (DWORD_PTR)0 );
    }	
}


BOOL KInterTransitPathEditDlg::PreTranslateMessage(MSG* pMsg)
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


void KInterTransitPathEditDlg::ClickedEditStartNode()
{
	m_bStart = true;
	SelectStartNodeList();
}


void KInterTransitPathEditDlg::OnBnClickedButtonStartNodeSearch()
{
	m_bStart = true;
	SelectStartNodeList();
}


void KInterTransitPathEditDlg::ClickedEditEndNode()
{
	if (m_mapEndZonePathMode.size() < 1)
	{
		AfxMessageBox(_T("경로 관련 정보가 존재하지 않습니다."));
		return;
	}

	m_bStart = false;
	SelectEndNodeList();
}


void KInterTransitPathEditDlg::OnBnClickedButtonEndNodeSearch()
{
	if (m_mapEndZonePathMode.size() < 1)
	{
		AfxMessageBox(_T("경로 관련 정보가 존재하지 않습니다."));
		return;
	}

	m_bStart = false;
	SelectEndNodeList();
}


void KInterTransitPathEditDlg::SelectStartNodeList()
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


void KInterTransitPathEditDlg::SelectEndNodeList()
{
	try
	{
		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		bool bZoneNode = true;
		if (bZoneNode)
		{
			m_spImTasIDFind->SetIDCaption(m_vecEndZoneNodeIDCaption);
		}
// 		Integer nxNodeID = _ttoi64(m_oEndNodeIdCaption.TID);
// 		m_spImTasIDFind->SetSelectedID(nxNodeID);
 		m_spImTasIDFind->SetSelectedID(-1);

		// 지도 선택 옵션
		m_spImTasIDFind->SetMapNodeSelectInfo(m_pMapView, m_vecEndZoneNodeID, ImChampMapDefineSymbol::ZoneSymbol);
		m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterTransitPathEditDlg::SetZoneNodeViewData(CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
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


void KInterTransitPathEditDlg::ExchangeRegionCode(KEMInterModalPathMode a_emPathMode)
{
	try
	{
		if (KEMInterModalPathModeUrban == a_emPathMode)
		{
			UrbanRelationCotrol(true);
			RegionalRelationCotrol(false);
		}
		else if (KEMInterModalPathModeRegional == a_emPathMode)
		{
			UrbanRelationCotrol(false);
			RegionalRelationCotrol(true);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterTransitPathEditDlg::UrbanRelationCotrol(bool a_bShow)
{
	try
	{
		int nCmdShow(SW_SHOW);
		if (a_bShow)
			nCmdShow = SW_SHOW;
		else
			nCmdShow = SW_HIDE;

		GetDlgItem(IDC_REPORT_A)->ShowWindow(nCmdShow);
		GetDlgItem(IDC_REPORT_B)->ShowWindow(nCmdShow);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterTransitPathEditDlg::RegionalRelationCotrol(bool a_bShow)
{
	try
	{
		int nCmdShow(SW_SHOW);
		if (a_bShow)
			nCmdShow = SW_SHOW;
		else
			nCmdShow = SW_HIDE;

		GetDlgItem(IDC_REPORT_C)->ShowWindow(a_bShow);
		GetDlgItem(IDC_REPORT_D)->ShowWindow(a_bShow);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterTransitPathEditDlg::UpdateReportData()
{
	try
	{
		Integer  nxEndZoneID = GetSelEndNodeId();
		AutoType iter        = m_mapEndZonePathMode.find(nxEndZoneID);
		AutoType itEnd       = m_mapEndZonePathMode.end();	

		if (iter != itEnd)
		{
			KEMInterModalPathMode emPathMode = iter->second;
			ExchangeRegionCode(emPathMode);
			if (KEMInterModalPathModeUrban == emPathMode)
			{
				UpdateReportAData();
			}
			else if (KEMInterModalPathModeRegional == emPathMode)
			{
				UpdateReportCData();
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KInterTransitPathEditDlg::UpdateReportAData()
{
	try
	{
		m_wndReportACtrl.ResetContent();

		Integer               nxDZoneID = GetSelEndNodeId();
		CXTPReportRecord*     pRecord   = nullptr;
		CXTPReportRecordItem* pItem     = nullptr;
		
		std::vector<KInterPathEditInfo>* pvecTransitAssignInfo = nullptr;

		pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());

		pItem                 = pRecord->AddItem(new CXTPReportRecordItem());
		pvecTransitAssignInfo = IsHaveAutoTaxiAssignData(nxDZoneID); //auto
		SetReportAItem(pItem, pvecTransitAssignInfo);
	
		pItem                 = pRecord->AddItem(new CXTPReportRecordItem());
		pvecTransitAssignInfo = IsHaveBusAssignData(nxDZoneID);
		SetReportAItem(pItem, pvecTransitAssignInfo);

		pItem                 = pRecord->AddItem(new CXTPReportRecordItem());
		pvecTransitAssignInfo = IsHaveRailAssignData(nxDZoneID);
		SetReportAItem(pItem, pvecTransitAssignInfo);

		pItem                 = pRecord->AddItem(new CXTPReportRecordItem());
		pvecTransitAssignInfo = IsHaveBusRailAssignData(nxDZoneID);
		SetReportAItem(pItem, pvecTransitAssignInfo);

		pItem                 = pRecord->AddItem(new CXTPReportRecordItem());
		pvecTransitAssignInfo = IsHaveParkRideAssignData(nxDZoneID);
		SetReportAItem(pItem, pvecTransitAssignInfo);

		pItem                 = pRecord->AddItem(new CXTPReportRecordItem());
		pvecTransitAssignInfo = IsHaveKissRideAssignData(nxDZoneID);
		SetReportAItem(pItem, pvecTransitAssignInfo);

		pItem                 = pRecord->AddItem(new CXTPReportRecordItem());
		pvecTransitAssignInfo = IsHaveTaxiTransitAssignData(nxDZoneID);
		SetReportAItem(pItem, pvecTransitAssignInfo);

		m_wndReportACtrl.Populate();

		UpdateReportBData();
		UpdatePathCompareButtonState();
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


void KInterTransitPathEditDlg::SetReportAItem(CXTPReportRecordItem* a_pItem, std::vector<KInterPathEditInfo>* a_pvecTransitAssignInfo)
{
	try
	{
		if (nullptr == a_pvecTransitAssignInfo)
		{
			a_pItem->SetCaption(_T("-"));
			a_pItem->SetAlignment(DT_CENTER);
			a_pItem->SetEditable(FALSE);
		}
		else
		{
			a_pItem->SetEditable(TRUE);
			a_pItem->HasCheckbox(TRUE);
			a_pItem->SetChecked(TRUE);
			a_pItem->SetItemData((DWORD_PTR)a_pvecTransitAssignInfo);
			a_pItem->SetAlignment(xtpColumnIconCenter);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterTransitPathEditDlg::UpdateReportBData()
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

	try
	{
		CXTPReportRecord*         pParentRecord    = nullptr;
		CXTPReportRecords*        pSeParentRecords = nullptr;
		CXTPReportRecord*         pSeParentRecord  = nullptr;
		CXTPReportRecordItem*     pItem            = nullptr;
		CXTPReportRecordItemText* pItemText        = nullptr;
		CXTPReportRecordItem*     pParentCheckItem = nullptr;

		CXTPReportRecords* pARecords = m_wndReportACtrl.GetRecords();
		int nCount       = pARecords->GetCount();
		if (1 != nCount)
		{
			m_wndReportBCtrl.Populate();
			return;
		}

		CXTPReportRecord* pARecord  = pARecords->GetAt(0);
		for (int k= 0; k< pARecord->GetItemCount(); k++)
		{
			CXTPReportRecordItem* pItemA = pARecord->GetItem(k);

			if (pItemA->GetHasCheckbox() == FALSE)
				continue;

			if (pItemA->IsChecked() == FALSE)
				continue;

			std::vector<KInterPathEditInfo>* pvecAssignInfo = (std::vector<KInterPathEditInfo>*)pItemA->GetItemData();
			if (nullptr == pvecAssignInfo)
				continue;

			pParentRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());

			//#1 Type
			CString strTypeName(_T(""));
			AutoType iter  = m_mapUrbanCodeName.find(k);
			AutoType itEnd = m_mapUrbanCodeName.end();
			if (iter != itEnd)
			{
				strTypeName = iter->second;
			}

			pParentCheckItem         = pParentRecord->AddItem(new CXTPReportRecordItemText(strTypeName));
			for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
			{
				CXTPReportRecordItem*            pItemEmpty   = pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
				pEditOptions->RemoveButtons();
				pItemEmpty->SetEditable(FALSE);
			}
			pParentCheckItem->SetAlignment(DT_LEFT);
			pParentCheckItem->HasCheckbox(TRUE);

			pSeParentRecords = pParentRecord->GetChilds();
			int nChildCheckCount = 0;
			for (size_t m = 0; m < (*pvecAssignInfo).size(); m++)
			{
				KInterPathEditInfo &oTransitAssignInfo = (*pvecAssignInfo)[m];

				pSeParentRecord = pSeParentRecords->Add(new CXTPReportRecord());

				//#2
				CString strDisplay(_T(""));
				CString strIndex(_T(""));

				strIndex.Format(_T("%d"),     m+1);
				pItemText = (CXTPReportRecordItemText*)pSeParentRecord->AddItem(new CXTPReportRecordItemText());
				pItemText->SetAlignment(DT_LEFT);
				pItemText->HasCheckbox(TRUE);
				if( 1 == oTransitAssignInfo.nUseFlag )// 이상 무
				{
					pItemText->SetChecked(TRUE);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strDisplay.Format(_T("경로%d"), m+1);
					}
					else {
						strDisplay.Format(_T("#P%d"), m+1);
					}				

					nChildCheckCount++;
				}
				else
				{
					pItemText->SetChecked(FALSE);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strDisplay.Format(_T("경로%d"), m+1);
					}
					else {
						strDisplay.Format(_T("#P%d"), m+1);
					}				
				}

				pItemText->SetItemData((DWORD_PTR)&oTransitAssignInfo);
				pItemText->SetValue(strIndex);
				pItemText->SetCaption(strDisplay);

				//#3
				double dCost(0.0);
				double dTotalTime(0.0);
				double dLength(0.0);

				GetPathCost(k, oTransitAssignInfo, dTotalTime, dCost, dLength);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pSeParentRecord->AddItem(new CCustomReportRecordItem);
				pItem->SetAlignment(DT_LEFT);
				if( 1 == oTransitAssignInfo.nUseFlag )// 이상 무
				{
					pItem->SetCaption(_T("Use"));
					pItem->SetItemData((DWORD_PTR)1);
				}
				else
				{
					pItem->SetCaption(_T("Unuse"));
					pItem->SetItemData((DWORD_PTR)0);
				}

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
			}

			if (nChildCheckCount > 0)
				pParentCheckItem->SetChecked(TRUE);
			else
				pParentCheckItem->SetChecked(FALSE);
		}
		m_wndReportBCtrl.Populate();
		m_wndReportBCtrl.ExpandAll(TRUE);

		OnBnClickedButtonDraw();
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


void KInterTransitPathEditDlg::GetPathCost(int a_nModeIndex, KInterPathEditInfo &oTransitAssignInfo, double &a_dTotalTime, double &a_dCost, double &a_dLength)
{
	try
	{
		if(_0_A_COLUMN_ISAUTO == a_nModeIndex)
		{
			a_dCost            = oTransitAssignInfo.oTAutoSubData.dAutoCost;
			if (a_dCost < oTransitAssignInfo.oTAutoSubData.dTaxiCost )
			{
				a_dCost        = oTransitAssignInfo.oTAutoSubData.dTaxiCost;
			}
			a_dLength          = oTransitAssignInfo.oTAutoSubData.dTravelLength;
			a_dTotalTime       = oTransitAssignInfo.oTAutoSubData.dTravelTime;
		}
		else if(_1_A_COLUMN_ISBUS == a_nModeIndex || _3_A_COLUMN_ISBUSR == a_nModeIndex)
		{
			a_dCost            = oTransitAssignInfo.oTBusSubData.dCost;
			a_dLength          = oTransitAssignInfo.oTBusSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTBusSubData.dInVehTime
				+ oTransitAssignInfo.oTBusSubData.dWaitTime 
				+ oTransitAssignInfo.oTBusSubData.dTransferTime 
				+ oTransitAssignInfo.oTBusSubData.dAccessWalkTime 
				+ oTransitAssignInfo.oTBusSubData.dEgressWalkTime;
		}
		else if(_2_A_COLUMN_ISRAIL == a_nModeIndex)
		{
			a_dCost            = oTransitAssignInfo.oTRailSubData.dCost;
			a_dLength          = oTransitAssignInfo.oTRailSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTRailSubData.dInVehTime
				+ oTransitAssignInfo.oTRailSubData.dWaitTime 
				+ oTransitAssignInfo.oTRailSubData.dTransferTime 
				+ oTransitAssignInfo.oTRailSubData.dAccessWalkTime 
				+ oTransitAssignInfo.oTRailSubData.dEgressWalkTime;
		}
		else if(_4_A_COLUMN_ISPARKR == a_nModeIndex|| _5_A_COLUMN_ISKISSR == a_nModeIndex|| _6_A_COLUMN_ISTAXITR == a_nModeIndex)
		{
			a_dCost            = oTransitAssignInfo.oTMultiSubData.dVehCost + oTransitAssignInfo.oTMultiSubData.dTransitCost;
			a_dLength          = oTransitAssignInfo.oTMultiSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTMultiSubData.dVehTime
				+ oTransitAssignInfo.oTMultiSubData.dTravelTime
				+ oTransitAssignInfo.oTMultiSubData.dWaitTime
				+ oTransitAssignInfo.oTMultiSubData.dTransferTime
				+ oTransitAssignInfo.oTMultiSubData.dAccessWalkTime
				+ oTransitAssignInfo.oTMultiSubData.dEgressWalkTime;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterTransitPathEditDlg::UpdateReportCData()
{
	try
	{
		m_wndReportCCtrl.ResetContent();

		Integer               nxDZoneID = GetSelEndNodeId();
		CXTPReportRecord*     pRecord   = nullptr;
		CXTPReportRecordItem* pItem     = nullptr;

		pRecord = m_wndReportCCtrl.AddRecord(new CXTPReportRecord());

		std::vector<KRegionModePathInfo>* pvecRegionModePathInfo = nullptr;
		pvecRegionModePathInfo = IsHaveRegionalAssignData(nxDZoneID);

		for (size_t i= 0; i< (*pvecRegionModePathInfo).size(); i++)
		{
			KRegionModePathInfo &oRegionModePathInfo = (*pvecRegionModePathInfo)[i];

			pItem                 = pRecord->AddItem(new CXTPReportRecordItem());
			SetReportCItem(pItem, oRegionModePathInfo);
		}

		m_wndReportCCtrl.Populate();

		UpdateReportDData();
		UpdatePathCompareButtonState();
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


void KInterTransitPathEditDlg::SetReportCItem(CXTPReportRecordItem* a_pItem, KRegionModePathInfo &a_oRegionModePathInfo)
{
	try
	{
		if (a_oRegionModePathInfo.vecOnePathDetail.size() < 1)
		{
			a_pItem->SetCaption(_T("-"));
			a_pItem->SetAlignment(DT_CENTER);
			a_pItem->SetEditable(FALSE);
		}
		else
		{
			a_pItem->SetEditable(TRUE);
			a_pItem->HasCheckbox(TRUE);
			a_pItem->SetChecked(TRUE);
			a_pItem->SetItemData((DWORD_PTR)&a_oRegionModePathInfo);
			a_pItem->SetAlignment(xtpColumnIconCenter);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterTransitPathEditDlg::UpdateReportDData()
{
	m_wndReportDCtrl.ResetContent();

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

	try
	{
		CXTPReportRecord*         pParentRecord    = nullptr;
		CXTPReportRecords*        pSeParentRecords = nullptr;
		CXTPReportRecord*         pSeParentRecord  = nullptr;
		CXTPReportRecords*        pThParentRecords = nullptr;
		CXTPReportRecord*         pThParentRecord  = nullptr;
		CXTPReportRecords*        pFrParentRecords = nullptr;

		CXTPReportRecordItem*     pItem            = nullptr;
		CXTPReportRecordItemText* pItemText        = nullptr;
		CXTPReportRecordItem*     pParentCheckItem = nullptr;

		CXTPReportRecords* pCRecords = m_wndReportCCtrl.GetRecords();
		int nCount       = pCRecords->GetCount();
		if (1 != nCount)
		{
			m_wndReportDCtrl.Populate();
			return;
		}

		CXTPReportRecord* pCRecord  = pCRecords->GetAt(0);
		for (int k= 0; k< pCRecord->GetItemCount(); k++)
		{
			CXTPReportRecordItem* pItemC = pCRecord->GetItem(k);

			if (pItemC->GetHasCheckbox() == FALSE)
				continue;

			if (pItemC->IsChecked() == FALSE)
				continue;

			KRegionModePathInfo* pRegionModePathInfo = (KRegionModePathInfo*)pItemC->GetItemData();
			if (nullptr == pRegionModePathInfo)
				continue;

			pParentRecord = m_wndReportDCtrl.AddRecord(new CXTPReportRecord());

			//#1-1 Type
			CString strTypeName = pRegionModePathInfo->strModeName;
			int     nRegionMode = pRegionModePathInfo->nMode;
			
			pParentCheckItem         = pParentRecord->AddItem(new CXTPReportRecordItemText(strTypeName));
			for (int r= 0; r< D_COLUMN_TOTAL_COUNT -1; r++)
			{
				CXTPReportRecordItem*            pItemEmpty   = pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
				pEditOptions->RemoveButtons();
				pItemEmpty->SetEditable(FALSE);
			}
			pParentCheckItem->SetAlignment(DT_LEFT);
			pParentCheckItem->HasCheckbox(TRUE);
			pParentCheckItem->SetItemData((DWORD_PTR)pRegionModePathInfo);
			if (REGION_MODE_CODE_AUTO == nRegionMode)
			{
				pParentCheckItem->SetChecked(pItemC->IsChecked());
			}
			
			//#1-2 path num
			pSeParentRecords = pParentRecord->GetChilds();

			std::vector<KRegionPathDetail> &vecOnePathDetail = pRegionModePathInfo->vecOnePathDetail;
			for (size_t m = 0; m < vecOnePathDetail.size(); m++)
			{
				KRegionPathDetail &oRegionPathDetail = vecOnePathDetail[m];
				
				pSeParentRecord = pSeParentRecords->Add(new CXTPReportRecord());

				CString strDisplay(_T(""));
				CString strIndex(_T(""));

				strIndex.Format(_T("%d"),     m+1);
				pItemText = (CXTPReportRecordItemText*)pSeParentRecord->AddItem(new CXTPReportRecordItemText());
				for (int r= 0; r< D_COLUMN_TOTAL_COUNT -1; r++)
				{
					CXTPReportRecordItem*            pItemEmpty   = pSeParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
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
					pItemText->SetChecked(pItemC->IsChecked());
				}

				//#1-3 acc / egr
				pThParentRecords = pSeParentRecord->GetChilds();

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

					pThParentRecord = pThParentRecords->Add(new CXTPReportRecord());
					pItemText = (CXTPReportRecordItemText*)pThParentRecord->AddItem(new CXTPReportRecordItemText(strAccessEgress));

					if (0 == n || 2 == n)
					{						 
						for (int r= 0; r< D_COLUMN_TOTAL_COUNT -1; r++)
						{
							CXTPReportRecordItem*            pItemEmpty   = pThParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
							CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
							pEditOptions->RemoveButtons();
							pItemEmpty->SetEditable(FALSE);
						}
						pItemText->SetAlignment(DT_LEFT);
						pItemText->HasCheckbox(TRUE);
					}
					else
					{
						pItemText->SetEditable(FALSE);

						pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainTime,   _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);

						pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainWaitTime,     _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);
						pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainTransferTime, _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);
						pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainProcessTime,  _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);
						pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainFrequency,    _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);

						pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainLength, _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);

						pItem = pThParentRecord->AddItem(new CXTPReportRecordItemNumber(oRegionPathDetail.dMainCost,   _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);

						CXTPReportRecordItem*            pItemEmpty   = pThParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
						CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
						pEditOptions->RemoveButtons();
						pItemEmpty->SetEditable(FALSE);

						pItemEmpty   = pThParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
						pEditOptions = pItemEmpty->GetEditOptions(nullptr);
						pEditOptions->RemoveButtons();
						pItemEmpty->SetEditable(FALSE);
					}

					//#1-4 mode
					pFrParentRecords = pThParentRecord->GetChilds();
					if (0 == n)
					{
						if (KmzSystem::GetLanguage() == KEMKorea) {
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapAccAutoAssign,    _T("승용차")     ,KEMInterRegDetailModeAuto);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapAccBusAssign,     _T("버스")      ,KEMInterRegDetailModeBus);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapAccRailAssign,    _T("도시철도")     ,KEMInterRegDetailModeRail);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapAccBusRailAssign, _T("버스+도시철도") ,KEMInterRegDetailModeBusRail);
						}
						else {
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapAccAutoAssign,    _T("PassengerCar")     ,KEMInterRegDetailModeAuto);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapAccBusAssign,     _T("Bus")      ,KEMInterRegDetailModeBus);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapAccRailAssign,    _T("Rail")     ,KEMInterRegDetailModeRail);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapAccBusRailAssign, _T("Bus+Rail") ,KEMInterRegDetailModeBusRail);
						}
					}
					else if (2 == n)
					{
						if (KmzSystem::GetLanguage() == KEMKorea) {
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapEgrAutoAssign,    _T("승용차")     ,KEMInterRegDetailModeAuto);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapEgrBusAssign,     _T("버스")      ,KEMInterRegDetailModeBus);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapEgrRailAssign,    _T("도시철도")     ,KEMInterRegDetailModeRail);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapEgrBusRailAssign, _T("버스+도시철도") ,KEMInterRegDetailModeBusRail);
						}
						else {
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapEgrAutoAssign,    _T("PassengerCar")     ,KEMInterRegDetailModeAuto);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapEgrBusAssign,     _T("Bus")      ,KEMInterRegDetailModeBus);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapEgrRailAssign,    _T("Rail")     ,KEMInterRegDetailModeRail);
							RegionalReportSurport(pFrParentRecords, oRegionPathDetail.mapEgrBusRailAssign, _T("Bus+Rail") ,KEMInterRegDetailModeBusRail);
						}
					}
				}
			}
		}
		m_wndReportDCtrl.Populate();
		m_wndReportDCtrl.ExpandAll(TRUE);

		OnBnClickedButtonDraw();
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


void KInterTransitPathEditDlg::RegionalReportSurport(CXTPReportRecords* a_pRecords, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapAccAutoAssign, CString a_strTypeName, KEMInterRegDetailMode a_emMode)
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
			for (int r= 0; r< D_COLUMN_TOTAL_COUNT -1; r++)
			{
				CXTPReportRecordItem*            pItemEmpty   = pParentRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				CXTPReportRecordItemEditOptions* pEditOptions = pItemEmpty->GetEditOptions(nullptr);
				pEditOptions->RemoveButtons();
				pItemEmpty->SetEditable(FALSE);
			}
			pParentCheckItem->SetAlignment(DT_LEFT);
			pParentCheckItem->HasCheckbox(TRUE);

			pSeParentRecords = pParentRecord->GetChilds();
			int nChildCheckCount = 0;
			for (size_t m = 0; m < vecAssignInfo.size(); m++)
			{
				KInterPathEditInfo &oTransitAssignInfo = vecAssignInfo[m];

				pSeParentRecord = pSeParentRecords->Add(new CXTPReportRecord());

				//#2
				CString strDisplay(_T(""));
				CString strIndex(_T(""));

				strIndex.Format(_T("%d"),     m+1);
				pItemText = (CXTPReportRecordItemText*)pSeParentRecord->AddItem(new CXTPReportRecordItemText());
				pItemText->SetAlignment(DT_LEFT);
				pItemText->HasCheckbox(TRUE);
				if( 1 == oTransitAssignInfo.nUseFlag )// 이상 무
				{
					pItemText->SetChecked(TRUE);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strDisplay.Format(_T("경로%d"), m+1);
					}
					else {
						strDisplay.Format(_T("#P%d"), m+1);
					}				

					nChildCheckCount++;
				}
				else
				{
					pItemText->SetChecked(FALSE);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strDisplay.Format(_T("경로%d"), m+1);
					}
					else {
						strDisplay.Format(_T("#P%d"), m+1);
					}				
				}

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

				oTransitAssignInfo.emInterRegDetailMode = a_emMode;
				GetRegionPathCost(a_emMode, oTransitAssignInfo, dTotalTime, dCost, dLength, dWaitTime, dTransferTime, dProcessTime, dFrequency);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dWaitTime,     _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);   
				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dTransferTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dProcessTime,  _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dFrequency,    _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pSeParentRecord->AddItem(new CXTPReportRecordItemNumber(dCost,   _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);

				pItem = pSeParentRecord->AddItem(new CCustomReportRecordItem);
				pItem->SetAlignment(DT_LEFT);
				if( 1 == oTransitAssignInfo.nUseFlag )// 이상 무
				{
					pItem->SetCaption(_T("Use"));
					pItem->SetItemData((DWORD_PTR)1);
				}
				else
				{
					pItem->SetCaption(_T("Unuse"));
					pItem->SetItemData((DWORD_PTR)0);
				}

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
			}

			if (nChildCheckCount > 0)
			{
				pParentCheckItem->SetChecked(TRUE);
				
				for (int nDepth= 0; nDepth< 3; nDepth++)
				{
					pParentRecord    = pParentRecord->GetParentRecord();
					pParentCheckItem = pParentRecord->GetItem(_0_D_COLUMN_ZONEID);
					pParentCheckItem->SetChecked(TRUE);
				}
			}
			else
			{
				pParentCheckItem->SetChecked(FALSE);
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


void KInterTransitPathEditDlg::GetRegionPathCost(KEMInterRegDetailMode a_emMode, KInterPathEditInfo &oTransitAssignInfo, double &a_dTotalTime, double &a_dCost, double &a_dLength, 
												double &a_dWaitTime, double &a_dTransferTime, double &a_dProcessTime, double &a_dFrequency)
{
	try
	{
		if(KEMInterRegDetailModeAuto == a_emMode)
		{
			a_dCost            = oTransitAssignInfo.oTRegAutoSubData.dAutoCost;
			a_dLength          = 0.0;
			a_dTotalTime       = oTransitAssignInfo.oTRegAutoSubData.dAutoTime;
			a_dWaitTime        = 0.0;
			a_dTransferTime    = 0.0;
			a_dProcessTime     = 0.0;
			a_dFrequency       = 0.0;
		}
		else if(KEMInterRegDetailModeBus == a_emMode || KEMInterRegDetailModeBusRail == a_emMode)
		{
			a_dCost            = oTransitAssignInfo.oTBusSubData.dCost;
			a_dLength          = oTransitAssignInfo.oTBusSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTBusSubData.dInVehTime
				//+ oTransitAssignInfo.oTBusSubData.dWaitTime 
				+ oTransitAssignInfo.oTBusSubData.dTransferTime 
				+ oTransitAssignInfo.oTBusSubData.dAccessWalkTime 
				+ oTransitAssignInfo.oTBusSubData.dEgressWalkTime;
			a_dWaitTime        = oTransitAssignInfo.oTBusSubData.dWaitTime;
			a_dTransferTime    = oTransitAssignInfo.oTBusSubData.dTransferTime;
			a_dProcessTime     = 0.0;
			a_dFrequency       = 0.0;
		}
		else if(KEMInterRegDetailModeRail == a_emMode)
		{
			a_dCost            = oTransitAssignInfo.oTRailSubData.dCost;
			a_dLength          = oTransitAssignInfo.oTRailSubData.dLength;
			a_dTotalTime       = oTransitAssignInfo.oTRailSubData.dInVehTime
				//+ oTransitAssignInfo.oTRailSubData.dWaitTime 
				+ oTransitAssignInfo.oTRailSubData.dTransferTime 
				+ oTransitAssignInfo.oTRailSubData.dAccessWalkTime 
				+ oTransitAssignInfo.oTRailSubData.dEgressWalkTime;
			a_dWaitTime        = oTransitAssignInfo.oTRailSubData.dWaitTime;
			a_dTransferTime    = oTransitAssignInfo.oTRailSubData.dTransferTime;
			a_dProcessTime     = 0.0;
			a_dFrequency       = 0.0;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}



void KInterTransitPathEditDlg::FindPath()
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
		m_vecEndZoneNodeID.clear();

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
			m_vecEndZoneNodeID.push_back(nxDzoneID);
			++iter;
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


unsigned __stdcall KInterTransitPathEditDlg::CollectDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("생성된 복합수단 경로 데이터 로딩 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Generated Intermodal Path Data..."));
	}

	KInterTransitPathEditDlg* pDlg = (KInterTransitPathEditDlg*)pParameter->GetParameter();
	pDlg->CollectData();

	return 0;
}


void KInterTransitPathEditDlg::CollectData()
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


void KInterTransitPathEditDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KInterTransitPathEditDlg::ReadOutPutFile()
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
		strDataFileName.Format(_T("IntermodalPath\\%I64d_AutoTaxiPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH ,strDataFileName);
		ReadAutoTaxiPathFile(strSavePath);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_BusPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH ,strDataFileName);
		ReadBusPathFile(strSavePath);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_RailPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(),DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH ,strDataFileName);
		ReadRailPathFile(strSavePath);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_BusRailPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH ,strDataFileName);
		ReadBusRailPathFile(strSavePath);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_ParkandRidePath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH ,strDataFileName);
		ReadParkRidePathFile(strSavePath);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_KissandRidePath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH ,strDataFileName);
		ReadKissRidePathFile(strSavePath);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_TaxiandTransitPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH ,strDataFileName);
		ReadTaxiTransitPathFile(strSavePath);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_RegionalPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH ,strDataFileName);
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


Integer KInterTransitPathEditDlg::GetSelStartNodeId()
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


Integer KInterTransitPathEditDlg::GetSelEndNodeId()
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


void KInterTransitPathEditDlg::ReadAutoTaxiPathFile( CString a_strFileName )
{
	TxLogDebugStartMsg();
	size_t szUseFlagDataPoint(0);
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
				szUseFlagDataPoint                                                         += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );
				szUseFlagDataPoint                                                         += sizeof(Integer);

				KInterPathEditInfo oAutoAssignInfo;
				oAutoAssignInfo.emInterMode         = KEMInterModeAutoTaxi;
				oAutoAssignInfo.nxOrginID           = nxOrginID;
				oAutoAssignInfo.nxDestinationID     = nxDestinationID;

				ifs.read( reinterpret_cast<char*>( &(oAutoAssignInfo.oTAutoSubData)),             sizeof(TInEditAutoSubData) );
				szUseFlagDataPoint                                                             += sizeof(TInEditAutoSubData);

				oAutoAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
				ifs.read( reinterpret_cast<char*>( &(oAutoAssignInfo.nUseFlag)),                  sizeof(int) );
				szUseFlagDataPoint                                                             += sizeof(int); 

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

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
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


void KInterTransitPathEditDlg::ReadBusPathFile(CString strFileName)
{
	TxLogDebugStartMsg();
	size_t szUseFlagDataPoint(0);
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
				szUseFlagDataPoint                                                         += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );
				szUseFlagDataPoint                                                         += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),                            sizeof(int) );
				szUseFlagDataPoint                                                         += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 정보 취합
					KInterPathEditInfo oBusAssignInfo;
					oBusAssignInfo.emInterMode         = KEMInterModeBus;
					oBusAssignInfo.nxOrginID           = nxOrginID;
					oBusAssignInfo.nxDestinationID     = nxDestinationID;

					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                         += sizeof(int);
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
							szUseFlagDataPoint                                                      += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),    sizeof(int) );
							szUseFlagDataPoint                                                      += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),      sizeof(int) );
							szUseFlagDataPoint                                                      += sizeof(int);

							if(0 == oTInResultBusInfo.nxTransitID)
								continue;

							vecLowResultTransitInfo.push_back(oTInResultBusInfo);
						}
						vecResulTransitInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.oTBusSubData)),               sizeof(TInEditBusSubData) );
					szUseFlagDataPoint                                                             += sizeof(TInEditBusSubData);
					oBusAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nUseFlag)),                   sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int); 
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

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
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


void KInterTransitPathEditDlg::ReadRailPathFile(CString strFileName)
{
	TxLogDebugStartMsg();
	size_t szUseFlagDataPoint(0);
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
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
				szUseFlagDataPoint                                           += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 정보 취합
					KInterPathEditInfo oRailAssignInfo;
					oRailAssignInfo.emInterMode         = KEMInterModeRail;
					oRailAssignInfo.nxOrginID           = nxOriginID;
					oRailAssignInfo.nxDestinationID     = nxDestinationID;

					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                          += sizeof(int);
					if(ifs.eof())break;

					int nBoardingCnt(0); //탑승횟수
					ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
					szUseFlagDataPoint                                                          += sizeof(int);

					if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					std::vector< TInIndexResultTransitInfo > &vecResulTransitInfo = oRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< nBoardingCnt; i++)
					{
						int nDuplicateCnt(0); //중복 노선 갯수
						ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
						szUseFlagDataPoint                                                      += sizeof(int);
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
							szUseFlagDataPoint                                                       += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);

							if(0 == oTInResultRailInfo.nxTransitID)
								continue;

							vecLowResultBusInfo.push_back(oTInResultRailInfo);
						}
						vecResulTransitInfo.push_back(oTIndexResultTransitInfo);
					}

					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.oTRailSubData)),             sizeof(TInEditRailSubData) );
					szUseFlagDataPoint                                                             += sizeof(TInEditRailSubData);
					oRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nUseFlag)),                  sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
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

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
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


void KInterTransitPathEditDlg::ReadBusRailPathFile(CString strFileName)
{
	TxLogDebugStartMsg();

	m_mapBusRailAssign.clear();
	size_t szUseFlagDataPoint(0);
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
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
				szUseFlagDataPoint                                           += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KInterPathEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emInterMode         = KEMInterModeBusRail;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
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
							szUseFlagDataPoint                                                       += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);

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
					szUseFlagDataPoint                                                             += sizeof(TInEditBusSubData);
					oBusRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );
						szUseFlagDataPoint                                                             += sizeof(int);

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
								szUseFlagDataPoint                                                      += sizeof(int);

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
									szUseFlagDataPoint                                                       += sizeof(Integer);
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
									szUseFlagDataPoint                                                       += sizeof(int);
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
									szUseFlagDataPoint                                                       += sizeof(int);

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

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
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


void KInterTransitPathEditDlg::ReadParkRidePathFile( CString a_strFileName )
{
	TxLogDebugStartMsg();

	m_mapParkRideAssign.clear();
	size_t szUseFlagDataPoint(0);
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
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
				szUseFlagDataPoint                                           += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KInterPathEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emInterMode         = KEMInterModeParkRide;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
					if(ifs.eof())break;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoStartNode)),        sizeof(Integer) );
					szUseFlagDataPoint                                                             += sizeof(Integer);

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoEndNode))  ,        sizeof(Integer) );
					szUseFlagDataPoint                                                             += sizeof(Integer);

					std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< PARKRIDEPATH_ASSIGNSET_COUNT; i++)
					{
						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );
							szUseFlagDataPoint                                                       += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);

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
					szUseFlagDataPoint                                                             += sizeof(TInEditMultiSubData);
					oBusRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );
						szUseFlagDataPoint                                                             += sizeof(int);

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
								szUseFlagDataPoint                                                      += sizeof(int);

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
									szUseFlagDataPoint                                                       += sizeof(Integer);
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
									szUseFlagDataPoint                                                       += sizeof(int);
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
									szUseFlagDataPoint                                                       += sizeof(int);

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

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
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


void KInterTransitPathEditDlg::ReadKissRidePathFile( CString a_strFileName )
{
	TxLogDebugStartMsg();

	m_mapKissRideAssign.clear();
	size_t szUseFlagDataPoint(0);
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
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
				szUseFlagDataPoint                                           += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KInterPathEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emInterMode         = KEMInterModeKissRide;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
					if(ifs.eof())break;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoStartNode)),        sizeof(Integer) );
					szUseFlagDataPoint                                                             += sizeof(Integer);

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoEndNode))  ,        sizeof(Integer) );
					szUseFlagDataPoint                                                             += sizeof(Integer);

					std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< KISSRIDEPATH_ASSIGNSET_COUNT; i++)
					{
						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );
							szUseFlagDataPoint                                                       += sizeof(Integer);

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);

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
					szUseFlagDataPoint                                                             += sizeof(TInEditMultiSubData);
					oBusRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );
						szUseFlagDataPoint                                                             += sizeof(int);

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
								szUseFlagDataPoint                                                      += sizeof(int);

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
									szUseFlagDataPoint                                                       += sizeof(Integer);
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
									szUseFlagDataPoint                                                       += sizeof(int);
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
									szUseFlagDataPoint                                                       += sizeof(int);

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

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
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


void KInterTransitPathEditDlg::ReadTaxiTransitPathFile( CString a_strFileName )
{
	TxLogDebugStartMsg();

	m_mapTaxiTransitAssign.clear();
	size_t szUseFlagDataPoint(0);
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
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
				szUseFlagDataPoint                                           += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
				szUseFlagDataPoint                                           += sizeof(int);

				if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int z= 0; z< nPathCount; z++)
				{
#pragma region 버스 정보 취합
					KInterPathEditInfo oBusRailAssignInfo;
					oBusRailAssignInfo.emInterMode         = KEMInterModeTaxiTransit;
					oBusRailAssignInfo.nxOrginID           = nxOriginID;
					oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
					int nRailIndex = -1;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
					if(ifs.eof())break;

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoStartNode)),        sizeof(Integer) );
					szUseFlagDataPoint                                                             += sizeof(Integer);

					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nxAutoEndNode))  ,        sizeof(Integer) );
					szUseFlagDataPoint                                                             += sizeof(Integer);

					std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
					for (int i= 0; i< TAXITRANSITPATH_ASSIGNSET_COUNT; i++)
					{
						TInIndexResultTransitInfo oTIndexResultTransitInfo;
						std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
						for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
						{
							TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );
							szUseFlagDataPoint                                                       += sizeof(Integer);

							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);

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
					szUseFlagDataPoint                                                             += sizeof(TInEditMultiSubData);
					oBusRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
					ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
					szUseFlagDataPoint                                                             += sizeof(int);
#pragma endregion 버스 정보 취합

					std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
					if (nRailIndex != -1)
					{

#pragma region 레일 정보 취합(multi rail)
						int nRailCount(0);
						ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );
						szUseFlagDataPoint                                                             += sizeof(int);

						if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
						{
							throw 1;
						}

						for (int m= 0; m< nRailCount; m++)
						{
							int     nNoneUsePathID;

							ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							int nBoardingCnt(0); //탑승횟수
							ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
							szUseFlagDataPoint                                                          += sizeof(int);

							if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
							{
								throw 1;
							}

							std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
							for (int i= 0; i< nBoardingCnt; i++)
							{
								int nDuplicateCnt(0); //중복 노선 갯수
								ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
								szUseFlagDataPoint                                                      += sizeof(int);

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
									szUseFlagDataPoint                                                       += sizeof(Integer);
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
									szUseFlagDataPoint                                                       += sizeof(int);
									ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
									szUseFlagDataPoint                                                       += sizeof(int);

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

			CString strResult = _T("");
			strResult.Format(_T("--- end insert result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
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


std::vector<KInterPathEditInfo>* KInterTransitPathEditDlg::IsHaveAutoTaxiAssignData(Integer &a_nxZoneID)
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


std::vector<KInterPathEditInfo>* KInterTransitPathEditDlg::IsHaveBusAssignData(Integer &a_nxZoneID)
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


std::vector<KInterPathEditInfo>* KInterTransitPathEditDlg::IsHaveRailAssignData(Integer &a_nxZoneID)
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


std::vector<KInterPathEditInfo>* KInterTransitPathEditDlg::IsHaveBusRailAssignData(Integer &a_nxZoneID)
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


std::vector<KInterPathEditInfo>* KInterTransitPathEditDlg::IsHaveParkRideAssignData(Integer &a_nxZoneID)
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


std::vector<KInterPathEditInfo>* KInterTransitPathEditDlg::IsHaveKissRideAssignData(Integer &a_nxZoneID)
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


std::vector<KInterPathEditInfo>* KInterTransitPathEditDlg::IsHaveTaxiTransitAssignData(Integer &a_nxZoneID)
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


std::vector<KRegionModePathInfo>* KInterTransitPathEditDlg::IsHaveRegionalAssignData(Integer &a_nxZoneID)
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


void KInterTransitPathEditDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	UpdateReportBData();
	UpdatePathCompareButtonState();
}


void KInterTransitPathEditDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
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

			if(pRecord->HasChildren() == TRUE)
			{
				BOOL bParentChecked = pItem->IsChecked();

				CXTPReportRecords* pChdRecords = pRecord->GetChilds();
				int nCount = pChdRecords->GetCount();
				for (int i= 0; i< nCount; i++)
				{
					CXTPReportRecord*          pChdRecord = pChdRecords->GetAt(i);
					CXTPReportRecordItemText*  pChdItem   = (CXTPReportRecordItemText*)pChdRecord->GetItem(_0_B_COLUMN_ZONEID);
					pChdItem->SetChecked(bParentChecked);
				}
				m_wndReportBCtrl.RedrawControl();
			}
			else
			{
				BOOL bChdChecked     = pItem->IsChecked();
				CXTPReportRecord*      pParentRecord = pRecord->GetParentRecord();
				CXTPReportRecordItem*  pParentItem   = pParentRecord->GetItem(_0_B_COLUMN_ZONEID);
				
				CXTPReportRecords*     pChdRecords   = pParentRecord->GetChilds();
				int nCount           = pChdRecords->GetCount();
				int nCheckedCount = 0;
				for (int i= 0; i< nCount; i++)
				{
					CXTPReportRecord*      pChdRecord = pChdRecords->GetAt(i);
					CXTPReportRecordItem*  pChdItem   = pChdRecord->GetItem(_0_B_COLUMN_ZONEID);
					if( pChdItem->IsChecked() == TRUE)
					{
						nCheckedCount++;
					}
				}

				if (nCheckedCount > 0)
				{
					pParentItem->SetChecked(TRUE);
				}
				else
				{
					pParentItem->SetChecked(FALSE);
				}
				m_wndReportBCtrl.RedrawControl();
			}
		}
		catch (...)
		{
			return;
		}

		OnBnClickedButtonDraw();
		UpdatePathCompareButtonState();
	}
}


void KInterTransitPathEditDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if ( _4_B_COLUMN_USE == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord      = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem        = (CXTPReportRecordItemText*)pRecord->GetItem(_4_B_COLUMN_USE);

			CXTPReportRecordItemText* pItemCheck   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_B_COLUMN_ZONEID);

			if(pItemCheck->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == FALSE)
			{
				int                 nUseFlag           = pItem->GetItemData();
				KInterPathEditInfo* pInterPathEditInfo = (KInterPathEditInfo*)pItemCheck->GetItemData(); 
				if(0 == nUseFlag)
				{
					pInterPathEditInfo->nUseFlag = 0;
				}
				else
				{
					pInterPathEditInfo->nUseFlag = 1;
				}
			}
		}
		catch (...)
		{
			return;
		}
	}
}


void KInterTransitPathEditDlg::OnReportBValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if ( _4_B_COLUMN_USE == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord      = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem        = (CXTPReportRecordItemText*)pRecord->GetItem(_4_B_COLUMN_USE);

			CXTPReportRecordItemText* pItemCheck   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_B_COLUMN_ZONEID);

			if(pItemCheck->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == FALSE)
			{
				int                 nUseFlag           = pItem->GetItemData();
				KInterPathEditInfo* pInterPathEditInfo = (KInterPathEditInfo*)pItemCheck->GetItemData(); 
				
				if(0 == nUseFlag)
				{
					pInterPathEditInfo->nUseFlag = 0;
				}
				else
				{
					pInterPathEditInfo->nUseFlag = 1;
				}
			}
		}
		catch (...)
		{
			return;
		}
	}
}


void KInterTransitPathEditDlg::OnReportBItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecordItem* pItem = pItemNotify->pItem;

	if(_5_B_COLUMN_DETAIL == pItem->GetIndex())
	{
		try
		{
			CXTPReportRecord*     pRecord        = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItemB         = pRecord->GetItem(_0_B_COLUMN_ZONEID);
			KInterPathEditInfo*   pPathEditInfo  = (KInterPathEditInfo*)pItemB->GetItemData();
			CString               strPathIdex    = pItemB->GetCaption(NULL);

			CRect rect;
			GetWindowRect(rect);

			if(nullptr != m_spInterModalResultDetailDlg)
			{
				m_spInterModalResultDetailDlg->InitializeEditData(KEMInterModalPathModeUrban, strPathIdex, pPathEditInfo, rect);
				m_spInterModalResultDetailDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				m_spInterModalResultDetailDlg = KInterResultDetailDlgPtr(new KInterResultDetailDlg(m_pTarget, false, m_mapNodeName, this));
				m_spInterModalResultDetailDlg->Create(KInterResultDetailDlg::IDD, this);
				m_spInterModalResultDetailDlg->InitializeEditData(KEMInterModalPathModeUrban, strPathIdex, pPathEditInfo, rect);
				m_spInterModalResultDetailDlg->ShowWindow(SW_SHOW);
			}
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KInterTransitPathEditDlg::OnReportCCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	UpdateReportDData();
	UpdatePathCompareButtonState();
}


void KInterTransitPathEditDlg::OnReportDCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_D_COLUMN_ZONEID == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_D_COLUMN_ZONEID);

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

			m_wndReportDCtrl.RedrawControl();
		}
		catch (...)
		{
			return;
		}

		OnBnClickedButtonDraw();
		UpdatePathCompareButtonState();
	}
}


void KInterTransitPathEditDlg::OnReportDItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if ( _8_D_COLUMN_USE == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord      = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem        = (CXTPReportRecordItemText*)pRecord->GetItem(_8_D_COLUMN_USE);

			CXTPReportRecordItemText* pItemCheck   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_D_COLUMN_ZONEID);

			if(pItemCheck->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == FALSE)
			{
				int                 nUseFlag           = pItem->GetItemData();
				KInterPathEditInfo* pInterPathEditInfo = (KInterPathEditInfo*)pItemCheck->GetItemData(); 
				if(0 == nUseFlag)
				{
					pInterPathEditInfo->nUseFlag = 0;
				}
				else
				{
					pInterPathEditInfo->nUseFlag = 1;
				}
			}
		}
		catch (...)
		{
			return;
		}
	}
}


void KInterTransitPathEditDlg::OnReportDValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if ( _8_D_COLUMN_USE == nIndex)
	{
		try
		{
			CXTPReportRecord*         pRecord      = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem        = (CXTPReportRecordItemText*)pRecord->GetItem(_8_D_COLUMN_USE);

			CXTPReportRecordItemText* pItemCheck   = (CXTPReportRecordItemText*)pRecord->GetItem(_0_D_COLUMN_ZONEID);

			if(pItemCheck->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == FALSE)
			{
				int                 nUseFlag           = pItem->GetItemData();
				KInterPathEditInfo* pInterPathEditInfo = (KInterPathEditInfo*)pItemCheck->GetItemData(); 
				if(0 == nUseFlag)
				{
					pInterPathEditInfo->nUseFlag = 0;
				}
				else
				{
					pInterPathEditInfo->nUseFlag = 1;
				}
			}
		}
		catch (...)
		{
			return;
		}
	}
}


void KInterTransitPathEditDlg::OnReportDItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	CXTPReportRecordItem* pItem = pItemNotify->pItem;

	if(_9_D_COLUMN_DETAIL == pItem->GetIndex())
	{
		try
		{
			CXTPReportRecord*     pRecord        = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItemB         = pRecord->GetItem(_0_D_COLUMN_ZONEID);
			KInterPathEditInfo*   pPathEditInfo = (KInterPathEditInfo*)pItemB->GetItemData();
			CString               strPathIdex    = pItemB->GetCaption(NULL);

			CRect rect;
			GetWindowRect(rect);

			if(nullptr != m_spInterModalResultDetailDlg)
			{
				m_spInterModalResultDetailDlg->InitializeEditData(KEMInterModalPathModeRegional, strPathIdex, pPathEditInfo, rect);
				m_spInterModalResultDetailDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				m_spInterModalResultDetailDlg = KInterResultDetailDlgPtr(new KInterResultDetailDlg(m_pTarget, false, m_mapNodeName, this));
				m_spInterModalResultDetailDlg->Create(KInterResultDetailDlg::IDD, this);
				m_spInterModalResultDetailDlg->InitializeEditData(KEMInterModalPathModeRegional, strPathIdex, pPathEditInfo, rect);
				m_spInterModalResultDetailDlg->ShowWindow(SW_SHOW);
			}
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KInterTransitPathEditDlg::OnReportColorAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KInterTransitPathEditDlg::OnReportColorBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KInterTransitPathEditDlg::OnBnClickedCancel()
{
	CloseWindowProc();

	KResizeDialogEx::OnCancel();
}


void KInterTransitPathEditDlg::CloseWindowProc()
{
	AllClearPath(true);

	if (nullptr != m_spInterModalResultDetailDlg)
		m_spInterModalResultDetailDlg.reset();

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();

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
}


void KInterTransitPathEditDlg::ActivatedFrameWndNotifyProcess()
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

	// 2019.03.12 명지대 요청사항 수정, 2019.04.02 지역간 대화상자 추가
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


void KInterTransitPathEditDlg::DeActivatedFrameWndNotifyProcess()
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


void KInterTransitPathEditDlg::OnBnClickedButtonApply()
{
	Integer nxNodeID = GetSelStartNodeId();
	if(-1 == nxNodeID)
	{
		return;
	}

	if (AfxMessageBox(_T("변경사항을 저장하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;

	CString strDataFileName(_T(""));
	CString strSavePath(_T(""));

	try
	{
		strDataFileName.Format(_T("IntermodalPath\\%I64d_AutoTaxiPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH, strDataFileName);
		ApplyUrbanChangedUseFlag(strSavePath, m_mapAutoTaxiAssign);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_BusPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH, strDataFileName);
		ApplyUrbanChangedUseFlag(strSavePath, m_mapBusAssign);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_RailPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH, strDataFileName);
		ApplyUrbanChangedUseFlag(strSavePath, m_mapRailAssign);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_BusRailPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH, strDataFileName);
		ApplyUrbanChangedUseFlag(strSavePath, m_mapBusRailAssign);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_ParkandRidePath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH, strDataFileName);
		ApplyUrbanChangedUseFlag(strSavePath, m_mapParkRideAssign);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_KissandRidePath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH, strDataFileName);
		ApplyUrbanChangedUseFlag(strSavePath, m_mapKissRideAssign);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_TaxiandTransitPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH, strDataFileName);
		ApplyUrbanChangedUseFlag(strSavePath, m_mapTaxiTransitAssign);

		strDataFileName.Format(_T("IntermodalPath\\%I64d_RegionalPath.dat"), nxNodeID);
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_GENERATOR_PATH, strDataFileName);
		ApplyRegionChangedUseFlag(strSavePath, m_mapRegionAssign);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	AfxMessageBox(_T("변경사항이 저장되었습니다."));
}


void KInterTransitPathEditDlg::ApplyUrbanChangedUseFlag(CString a_strFileName, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssing)
{
	std::ofstream ofs(a_strFileName, std::ios::binary | std::ios::out | std::ios::in);
	try
	{
		if (! ofs)
		{
			return;
		}

		ApplyChangedUseFlag(ofs, a_mapTransitAssing);

		ofs.close();
	}
	catch(...)
	{
		ofs.close();
		TxLogDebugException();
	}
}


void KInterTransitPathEditDlg::ApplyRegionChangedUseFlag(CString a_strFileName, std::map<Integer, std::vector<KRegionModePathInfo>> &a_mapRegionAssing)
{
	std::ofstream ofs(a_strFileName, std::ios::binary | std::ios::out | std::ios::in);
	try
	{
		if (! ofs)
		{
			return;
		}

		AutoType iter  = a_mapRegionAssing.begin();
		AutoType itEnd = a_mapRegionAssing.end();

		while(iter != itEnd)
		{
			std::vector<KRegionModePathInfo> &vecRegionModePathInfo = iter->second;

			for (size_t i= 0; i< vecRegionModePathInfo.size(); i++)
			{
				KRegionModePathInfo            &oRegionModePathInfo   = vecRegionModePathInfo[i];
				std::vector<KRegionPathDetail> &vecRegionPathDetail   = oRegionModePathInfo.vecOnePathDetail;

				for (size_t k= 0; k< vecRegionPathDetail.size(); k++)
				{
					KRegionPathDetail &oRegionPathDetail = vecRegionPathDetail[k];

					ApplyChangedUseFlag(ofs, oRegionPathDetail.mapAccAutoAssign);
					ApplyChangedUseFlag(ofs, oRegionPathDetail.mapAccBusAssign);
					ApplyChangedUseFlag(ofs, oRegionPathDetail.mapAccBusRailAssign);
					ApplyChangedUseFlag(ofs, oRegionPathDetail.mapAccRailAssign);

					ApplyChangedUseFlag(ofs, oRegionPathDetail.mapEgrAutoAssign);
					ApplyChangedUseFlag(ofs, oRegionPathDetail.mapEgrBusAssign);
					ApplyChangedUseFlag(ofs, oRegionPathDetail.mapEgrBusRailAssign);
					ApplyChangedUseFlag(ofs, oRegionPathDetail.mapEgrRailAssign);
				}
			}
			++iter;
		}

		ofs.close();
	}
	catch(...)
	{
		ofs.close();
		TxLogDebugException();
	}
}


void KInterTransitPathEditDlg::ApplyChangedUseFlag(std::ofstream &ofs, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssing)
{
	try
	{
		AutoType iter  = a_mapTransitAssing.begin();
		AutoType itEnd = a_mapTransitAssing.end();

		while(iter != itEnd)
		{
			std::vector<KInterPathEditInfo> &vecMTransitEditInfo = iter->second;

			for (size_t i= 0; i< vecMTransitEditInfo.size(); i++)
			{
				KInterPathEditInfo &oTransitEditInfo = vecMTransitEditInfo[i];

				ofs.seekp(oTransitEditInfo.szUseFlagDataPoint, std::ios::beg);
				ofs.write(reinterpret_cast<char*>(&oTransitEditInfo.nUseFlag), sizeof(int));
			}
			++iter;
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KInterTransitPathEditDlg::ReadRegionalPathFile(CString strFileName)
{
	TxLogDebugStartMsg();
	size_t szUseFlagDataPoint(0);
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
				szUseFlagDataPoint                                                         += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nxDZoneID)),                             sizeof(Integer) );
				szUseFlagDataPoint                                                         += sizeof(Integer);
				ifs.read( reinterpret_cast<char*>( &(nRegionCode)),                           sizeof(int) );
				szUseFlagDataPoint                                                         += sizeof(int);

				if(ifs.eof())break;

				std::vector<KRegionModePathInfo> vecRegionModePathInfo;

				AutoType iter  = m_mapRegionalCodeName.begin();
				AutoType itEnd = m_mapRegionalCodeName.end();
				
				while (iter != itEnd)
				{
					// 3~8...
					ifs.read( reinterpret_cast<char*>( &(nPathCount)),                            sizeof(int) );
					szUseFlagDataPoint                                                         += sizeof(int);

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
							szUseFlagDataPoint                                                         += sizeof(int);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainTime)),        sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainCost)),        sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainLength)),      sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dTrip)),            sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);

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
							szUseFlagDataPoint                                                         += sizeof(int);
							
							int     nEgnoreData(0);
							double  dEgnoreData(0.0);

							ifs.read( reinterpret_cast<char*>( &(nEgnoreData)),                           sizeof(int) ); // reg mode path seq
							szUseFlagDataPoint                                                         += sizeof(int);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.nxOrgTermID)),      sizeof(Integer) );
							szUseFlagDataPoint                                                         += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.nxDesTermID)),      sizeof(Integer) );
							szUseFlagDataPoint                                                         += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.nxTransTermID)),    sizeof(Integer) );
							szUseFlagDataPoint                                                         += sizeof(Integer);

							ifs.read( reinterpret_cast<char*>( &(nEgnoreData)),                           sizeof(int) ); // accese seq
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nEgnoreData)),                           sizeof(int) ); // egress seq
							szUseFlagDataPoint                                                         += sizeof(int);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainTime)),        sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainWaitTime)),    sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainTransferTime)),sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainProcessTime)), sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainFrequency)),   sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dMainCost)),        sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);
							oRegionOnePathDetail.dMainLength = 0.0; // 프로토콜에 length 없음.

							ifs.read( reinterpret_cast<char*>( &(dEgnoreData)),                           sizeof(double) );// sumtime
							szUseFlagDataPoint                                                         += sizeof(double);

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
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nAccTaxiPathCnt)),                       sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nAccBusPathCnt)),                        sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nAccRailPathCnt)),                       sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nAccBusRailPathCnt)),                    sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nEgrAutoPathCnt)),                       sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nEgrTaxiPathCnt)),                       sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nEgrBusPathCnt)),                        sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nEgrRailPathCnt)),                       sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(nEgrBusRailPathCnt)),                    sizeof(int) );
							szUseFlagDataPoint                                                         += sizeof(int);

							ifs.read( reinterpret_cast<char*>( &(oRegionOnePathDetail.dTrip)),            sizeof(double) );
							szUseFlagDataPoint                                                         += sizeof(double);

							ifs.read( reinterpret_cast<char*>( &(nEgnoreData)),                           sizeof(int) ); // 34:사용유무
							szUseFlagDataPoint                                                         += sizeof(int);

							//35~40
							ReadRegAutoTaxiPathFile(ifs, szUseFlagDataPoint, nAccAutoPathCnt,    oRegionOnePathDetail.mapAccAutoAssign, KEMInterAccEgrTypeAccess);
							int nAccSumPathCount = nAccTaxiPathCnt + nAccBusPathCnt + nAccRailPathCnt + nAccBusRailPathCnt;
							if (nAccSumPathCount > 0)
							{
								ReadRegAutoTaxiPathFile(ifs, szUseFlagDataPoint, nAccTaxiPathCnt,    oRegionOnePathDetail.mapAccTaxiAssign, KEMInterAccEgrTypeAccess);
								ReadRegBusPathFile     (ifs, szUseFlagDataPoint, nAccBusPathCnt,     oRegionOnePathDetail.mapAccBusAssign, KEMInterAccEgrTypeAccess);
								ReadRegRailPathFile    (ifs, szUseFlagDataPoint, nAccRailPathCnt,    oRegionOnePathDetail.mapAccRailAssign, KEMInterAccEgrTypeAccess);
								ReadRegBusRailPathFile (ifs, szUseFlagDataPoint, nAccBusRailPathCnt, oRegionOnePathDetail.mapAccBusRailAssign, KEMInterAccEgrTypeAccess);
							}

							ReadRegAutoTaxiPathFile(ifs, szUseFlagDataPoint, nEgrAutoPathCnt,    oRegionOnePathDetail.mapEgrAutoAssign, KEMInterAccEgrTypeEgress);
							int nEgrSumPathCount = nEgrTaxiPathCnt + nEgrBusPathCnt + nEgrRailPathCnt + nEgrBusRailPathCnt;
							if (nEgrSumPathCount > 0)
							{
								ReadRegAutoTaxiPathFile(ifs, szUseFlagDataPoint, nEgrTaxiPathCnt,    oRegionOnePathDetail.mapEgrTaxiAssign, KEMInterAccEgrTypeEgress);
								ReadRegBusPathFile     (ifs, szUseFlagDataPoint, nEgrBusPathCnt,     oRegionOnePathDetail.mapEgrBusAssign, KEMInterAccEgrTypeEgress);
								ReadRegRailPathFile    (ifs, szUseFlagDataPoint, nEgrRailPathCnt,    oRegionOnePathDetail.mapEgrRailAssign, KEMInterAccEgrTypeEgress);
								ReadRegBusRailPathFile (ifs, szUseFlagDataPoint, nEgrBusRailPathCnt, oRegionOnePathDetail.mapEgrBusRailAssign, KEMInterAccEgrTypeEgress);
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


void KInterTransitPathEditDlg::ReadRegAutoTaxiPathFile(std::ifstream &ifs, size_t &szUseFlagDataPoint, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
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
			szUseFlagDataPoint                                                         += sizeof(Integer);
		
			ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );
			szUseFlagDataPoint                                                         += sizeof(Integer);
			oInterPathEditInfo.nxDestinationID = nxDestinationID;
			oInterPathEditInfo.emInterMode = KEMInterModeAutoTaxi;

			ifs.read( reinterpret_cast<char*>( &(oInterPathEditInfo.oTRegAutoSubData)),   sizeof(TRegAutoSubData) );
			szUseFlagDataPoint                                                         += sizeof(TRegAutoSubData);
		
			oInterPathEditInfo.szUseFlagDataPoint = szUseFlagDataPoint; // point
			ifs.read( reinterpret_cast<char*>( &(oInterPathEditInfo.nUseFlag)),           sizeof(int) );
			szUseFlagDataPoint                                                         += sizeof(int);

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


void KInterTransitPathEditDlg::ReadRegBusPathFile(std::ifstream &ifs, size_t &szUseFlagDataPoint, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
{
	TxLogDebugStartMsg();

	try
	{
		Integer nxOrginID(0);
		Integer nxDestinationID(0);
		int     nPathCount(0);

		ifs.read( reinterpret_cast<char*>( &(nxOrginID)),                             sizeof(Integer) );
		szUseFlagDataPoint                                                         += sizeof(Integer);
		ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),                       sizeof(Integer) );
		szUseFlagDataPoint                                                         += sizeof(Integer);
		ifs.read( reinterpret_cast<char*>( &(nPathCount)),                            sizeof(int) );
		szUseFlagDataPoint                                                         += sizeof(int);

		if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
		{
			throw 1;
		}

		for (int z= 0; z< nPathCount; z++)
		{
#pragma region 정보 취합
			KInterPathEditInfo oBusAssignInfo;
			oBusAssignInfo.emInterAccEgrType   = a_emInterAccEgrType;
			oBusAssignInfo.emInterMode         = KEMInterModeBus;
			oBusAssignInfo.nxOrginID           = nxOrginID;
			oBusAssignInfo.nxDestinationID     = nxDestinationID;

			ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nPathID)),                sizeof(int) );
			szUseFlagDataPoint                                                         += sizeof(int);

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
					szUseFlagDataPoint                                                      += sizeof(Integer);
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),    sizeof(int) );
					szUseFlagDataPoint                                                      += sizeof(int);
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),      sizeof(int) );
					szUseFlagDataPoint                                                      += sizeof(int);

					if(0 == oTInResultBusInfo.nxTransitID)
						continue;

					vecLowResultTransitInfo.push_back(oTInResultBusInfo);
				}
				vecResulTransitInfo.push_back(oTIndexResultTransitInfo);
			}

			ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.oTBusSubData)),               sizeof(TInEditBusSubData) );
			szUseFlagDataPoint                                                             += sizeof(TInEditBusSubData);
			oBusAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
			ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.nUseFlag)),                   sizeof(int) );
			szUseFlagDataPoint                                                             += sizeof(int); 
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


void KInterTransitPathEditDlg::ReadRegRailPathFile(std::ifstream &ifs, size_t &szUseFlagDataPoint, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
{
	TxLogDebugStartMsg();

	try
	{
		Integer nxOriginID(0);
		Integer nxDestinationID(0);
		int     nPathCount(0);

		ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
		szUseFlagDataPoint                                           += sizeof(Integer);
		ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
		szUseFlagDataPoint                                           += sizeof(Integer);
		ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
		szUseFlagDataPoint                                           += sizeof(int);

		if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
		{
			throw 1;
		}

		for (int z= 0; z< nPathCount; z++)
		{
#pragma region 정보 취합
			KInterPathEditInfo oRailAssignInfo;
			oRailAssignInfo.emInterAccEgrType = a_emInterAccEgrType;
			oRailAssignInfo.emInterMode         = KEMInterModeRail;
			oRailAssignInfo.nxOrginID           = nxOriginID;
			oRailAssignInfo.nxDestinationID     = nxDestinationID;

			ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nPathID)),                sizeof(int) );
			szUseFlagDataPoint                                                          += sizeof(int);

			int nBoardingCnt(0); //탑승횟수
			ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
			szUseFlagDataPoint                                                          += sizeof(int);

			if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
			{
				throw 1;
			}

			std::vector< TInIndexResultTransitInfo > &vecResulTransitInfo = oRailAssignInfo.vecResulTransitInfo;
			for (int i= 0; i< nBoardingCnt; i++)
			{
				int nDuplicateCnt(0); //중복 노선 갯수
				ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
				szUseFlagDataPoint                                                      += sizeof(int);
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
					szUseFlagDataPoint                                                       += sizeof(Integer);
					ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
					szUseFlagDataPoint                                                       += sizeof(int);
					ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
					szUseFlagDataPoint                                                       += sizeof(int);

					if(0 == oTInResultRailInfo.nxTransitID)
						continue;

					vecLowResultBusInfo.push_back(oTInResultRailInfo);
				}
				vecResulTransitInfo.push_back(oTIndexResultTransitInfo);
			}

			ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.oTRailSubData)),             sizeof(TInEditRailSubData) );
			szUseFlagDataPoint                                                             += sizeof(TInEditRailSubData);
			oRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
			ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.nUseFlag)),                  sizeof(int) );
			szUseFlagDataPoint                                                             += sizeof(int);
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


void KInterTransitPathEditDlg::ReadRegBusRailPathFile(std::ifstream &ifs, size_t &szUseFlagDataPoint, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
{
	TxLogDebugStartMsg();

	try
	{
		Integer nxOriginID(0);
		Integer nxDestinationID(0);
		int     nPathCount(0);

		ifs.read( reinterpret_cast<char*>( &(nxOriginID)),              sizeof(Integer) );
		szUseFlagDataPoint                                           += sizeof(Integer);
		ifs.read( reinterpret_cast<char*>( &(nxDestinationID)),         sizeof(Integer) );
		szUseFlagDataPoint                                           += sizeof(Integer);
		ifs.read( reinterpret_cast<char*>( &(nPathCount)),              sizeof(int) );
		szUseFlagDataPoint                                           += sizeof(int);

		if (nPathCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
		{
			throw 1;
		}

		for (int z= 0; z< nPathCount; z++)
		{
#pragma region 버스 정보 취합
			KInterPathEditInfo oBusRailAssignInfo;
			oBusRailAssignInfo.emInterAccEgrType   = a_emInterAccEgrType;
			oBusRailAssignInfo.emInterMode         = KEMInterModeBusRail;
			oBusRailAssignInfo.nxOrginID           = nxOriginID;
			oBusRailAssignInfo.nxDestinationID     = nxDestinationID;
			int nRailIndex = -1;

			ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nPathID)),                sizeof(int) );
			szUseFlagDataPoint                                                             += sizeof(int);

			std::vector< TInIndexResultTransitInfo > vecResultBusInfo = oBusRailAssignInfo.vecResulTransitInfo;
			for (int i= 0; i< REG_BUSRAILPATH_ASSIGNSET_COUNT; i++)
			{
				TInIndexResultTransitInfo oTIndexResultTransitInfo;
				std::vector<TInResultTransitInfo> &vecLowResultTransitInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
				for (int k= 0; k< 1; k++)//중복횟수: 버스는 중복 횟수가 없지만 횟수 1이 라는 의미로 사용
				{
					TInResultTransitInfo oTInResultBusInfo = {0,0,0,0.0, KEMInPathTypeBus, RGB(255,255,255), false};

					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nxTransitID)),       sizeof(Integer) );
					szUseFlagDataPoint                                                       += sizeof(Integer);
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nStartStation)),     sizeof(int) );
					szUseFlagDataPoint                                                       += sizeof(int);
					ifs.read( reinterpret_cast<char*>( &(oTInResultBusInfo.nEndStation)),       sizeof(int) );
					szUseFlagDataPoint                                                       += sizeof(int);

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
			szUseFlagDataPoint                                                             += sizeof(TInEditBusSubData);

			oBusRailAssignInfo.szUseFlagDataPoint = szUseFlagDataPoint;
			ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.nUseFlag)),               sizeof(int) );
			szUseFlagDataPoint                                                             += sizeof(int);
#pragma endregion 버스 정보 취합

			std::vector< std::vector< TInIndexResultTransitInfo > > vecMultiResultRailInfo;
			if (nRailIndex != -1)
			{

#pragma region 레일 정보 취합(multi rail)
				int nRailCount(0);
				ifs.read( reinterpret_cast<char*>( &(nRailCount)),                                sizeof(int) );
				szUseFlagDataPoint                                                             += sizeof(int);

				if (nRailCount > 1000) // 쓰레기 값에 의한 무한 루프 방지
				{
					throw 1;
				}

				for (int m= 0; m< nRailCount; m++)
				{
					int     nNoneUsePathID;

					ifs.read( reinterpret_cast<char*>( &(nNoneUsePathID)),                         sizeof(int) );
					szUseFlagDataPoint                                                          += sizeof(int);

					int nBoardingCnt(0); //탑승횟수
					ifs.read( reinterpret_cast<char*>( &nBoardingCnt),                             sizeof(int) );
					szUseFlagDataPoint                                                          += sizeof(int);

					if (nBoardingCnt > 1000) // 쓰레기 값에 의한 무한 루프 방지
					{
						throw 1;
					}

					std::vector< TInIndexResultTransitInfo > vecResultRailInfo;
					for (int i= 0; i< nBoardingCnt; i++)
					{
						int nDuplicateCnt(0); //중복 노선 갯수
						ifs.read( reinterpret_cast<char*>( &nDuplicateCnt),                        sizeof(int) );
						szUseFlagDataPoint                                                      += sizeof(int);

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
							szUseFlagDataPoint                                                       += sizeof(Integer);
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nStartStation)),    sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);
							ifs.read( reinterpret_cast<char*>( &(oTInResultRailInfo.nEndStation)),      sizeof(int) );
							szUseFlagDataPoint                                                       += sizeof(int);

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


HBRUSH KInterTransitPathEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void KInterTransitPathEditDlg::OnBnClickedButtonClear()
{
	AllClearPath(false);
	DrawOrgDestZoneAccess(true);
}


void KInterTransitPathEditDlg::AllClearPath(bool a_bRefresh)
{
	try
	{
		m_pMapView->TxClearCompositeThemes(a_bRefresh);

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


void KInterTransitPathEditDlg::OnBnClickedButtonDraw()
{
	TxLogDebugStartMsg();
	try
	{
		AllClearPath(false);

		CWaitCursor cw;
		SelectDrawData();
		//QBicSimpleProgressThread::ExecuteThread(SelectDrawDataThreadCaller, this);

// 		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
// 			m_pMapView->DrawCompositeNode(m_oMapResult.vecDrawNodeInfo, true,  false);
// 		else
			m_pMapView->TxDrawCompositeNode(m_oMapResult.vecDrawNodeInfo, false, false);

		m_pMapView->TxDrawCompositeLink    (m_oMapResult.vecDrawLinkInfo, false, false);
		//존->정류장 도보제거 요청 : 2013.03.03-명지대학교
		m_pMapView->TxDrawCompositeUserLine(m_oMapResult.vecUserLineInfo, false, false);

		DrawOrgDestZoneAccess(false); // org와 Dest들의 존엑세스를 그린다.

		m_pMapView->MapRefresh();
	}
	catch(...)
	{
		return;
	}
	TxLogDebugEndMsg();
}


void KInterTransitPathEditDlg::ConvertZoneAccessStruct(std::vector<TDrawNodeInfo> &a_vecDrawZoneNodeInfo, std::vector<TDrawZoneAccess> &a_vecDrawZoneAccess)
{
	try
	{
		if (a_vecDrawZoneAccess.size() >= 2)  {
			return;
		}

		size_t nxCount = a_vecDrawZoneNodeInfo.size();

		for (size_t i=0; i<nxCount; i++)
		{
			TDrawNodeInfo&  oDrawNodeInfo   = a_vecDrawZoneNodeInfo[i];
			TDrawZoneAccess oDrawZoneAccess;
			oDrawZoneAccess.TNodeID         = oDrawNodeInfo.TNodeID;
			oDrawZoneAccess.TSymbol = oDrawNodeInfo.TSymbol;
			oDrawZoneAccess.TColorRGB       = oDrawNodeInfo.TColorRGB;
			oDrawZoneAccess.TLabel          = oDrawNodeInfo.TLabel;
			oDrawZoneAccess.TSize           = oDrawNodeInfo.TSize;
			oDrawZoneAccess.TRadius         = m_dRadius;

			a_vecDrawZoneAccess.push_back(oDrawZoneAccess);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterTransitPathEditDlg::DrawOrgDestZoneAccess(bool a_bRefresh)
{
	try
	{
		Integer nxOrgZoneID  = GetSelStartNodeId();
		Integer nxDestZoneID = GetSelEndNodeId();

		if (-1 == nxOrgZoneID || -1 == nxDestZoneID)
		{
			return;
		}

		std::vector<TDrawZoneAccess> vecZoneAccess;

		// Org 그리기
		TDrawZoneAccess oDrawZoneAccess;
		oDrawZoneAccess.TNodeID         = nxOrgZoneID;
		oDrawZoneAccess.TSymbol			= 48;//임시 모양
		oDrawZoneAccess.TColorRGB       = RGB(255,0,0);
		oDrawZoneAccess.TLabel          = m_oStartNodeIdCaption.TID;
		oDrawZoneAccess.TSize           = 18.0; //임시 사이즈
		oDrawZoneAccess.TRadius         = m_dRadius;

		vecZoneAccess.push_back(oDrawZoneAccess);

		oDrawZoneAccess.TNodeID         = nxDestZoneID;
		oDrawZoneAccess.TColorRGB       = RGB(0,0,205);
		oDrawZoneAccess.TLabel          = m_oEndNodeIdCaption.TID;

		vecZoneAccess.push_back(oDrawZoneAccess);

		m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, a_bRefresh);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


unsigned __stdcall KInterTransitPathEditDlg::MakeDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KInterTransitPathEditDlg*     pDlg = (KInterTransitPathEditDlg*)pParameter->GetParameter();
	pDlg->MakeDrawData();

	return 0;
}

unsigned __stdcall KInterTransitPathEditDlg::SelectDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KInterTransitPathEditDlg*     pDlg = (KInterTransitPathEditDlg*)pParameter->GetParameter();
	pDlg->SelectDrawData();

	return 0;
}

void KInterTransitPathEditDlg::SelectDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		// Urban data 수집
		Integer  nxEndZoneID = GetSelEndNodeId();
		AutoType iter        = m_mapEndZonePathMode.find(nxEndZoneID);
		AutoType itEnd       = m_mapEndZonePathMode.end();	

		if (iter != itEnd)
		{
			KEMInterModalPathMode emPathMode = iter->second;
			if (KEMInterModalPathModeUrban == emPathMode)
			{
				SelectUrbanDrawData();
			}
			else if (KEMInterModalPathModeRegional == emPathMode)
			{
				SelectRegionalDrawData();
			}
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

void KInterTransitPathEditDlg::SelectUrbanDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		std::set<DWORD_PTR> setMBusAssignInfo; 

		CXTPReportRecordItem*  pItem    = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord*  pParentRecord    = pRecords->GetAt(i);
			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			for (int k= 0; k<  pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem            = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				if( pItem->GetHasCheckbox() == FALSE )
					continue;
				if( pItem->IsChecked() == FALSE )
					continue;

				KInterPathEditInfo* pAssignInfo = (KInterPathEditInfo*)pItem->GetItemData();
				DWORD_PTR dwPtr = (DWORD_PTR)pAssignInfo;
				setMBusAssignInfo.insert(dwPtr);
			}
		}

		std::map<int/*mode code*/, std::vector<TMainPathInfo>> mapMainPathInfo;
		KInterModalPathInfo::ResultTransitEditInfo3(m_pTarget, m_mapDestResultInputValue, setMBusAssignInfo, KEMInterModalPathModeUrban, mapMainPathInfo, m_oMapResult, m_mapUrbanColor);
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

void KInterTransitPathEditDlg::SelectRegionalDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		std::set<DWORD_PTR>                                    setMBusAssignInfo; 
		std::map<int/*mode code*/, std::vector<TMainPathInfo>> mapMainPathInfo;

		CXTPReportRecordItem*  pItem    = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportDCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord*    pParentRecord    = pRecords->GetAt(i);
			pItem              = pParentRecord->GetItem(_0_D_COLUMN_ZONEID);
			KRegionModePathInfo* pRegionModePathInfo = (KRegionModePathInfo*)pItem->GetItemData();

			if( pItem->GetHasCheckbox() == FALSE )
				continue;
			if( pItem->IsChecked() == FALSE )
				continue;

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			if (nullptr == pSeParentRecords)
				continue;

			for (int k= 0; k<  pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem            = pSeParentRecord->GetItem(_0_D_COLUMN_ZONEID);

				KRegionPathDetail* pRegionPathDetail = (KRegionPathDetail*)pItem->GetItemData();

				if( pItem->GetHasCheckbox() == FALSE )
					continue;

				if( pItem->IsChecked() == FALSE )
					continue;
				else
					MakeMainPathDrawData(pRegionModePathInfo, pRegionPathDetail, mapMainPathInfo);

				CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
				if (nullptr == pThParentRecords)
					continue;

				for (int m= 0; m<  pThParentRecords->GetCount(); m++)
				{
					CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
					pItem            = pThParentRecord->GetItem(_0_D_COLUMN_ZONEID);

					if( pItem->GetHasCheckbox() == FALSE )// true 이면 Main
						continue;
					if( pItem->IsChecked() == FALSE )
						continue;

					CXTPReportRecords* pFrParentRecords = pThParentRecord->GetChilds();
					if (nullptr == pFrParentRecords)
						continue;

					for (int n= 0; n<  pFrParentRecords->GetCount(); n++)
					{
						CXTPReportRecord*  pFrParentRecord = pFrParentRecords->GetAt(n);
						pItem            = pFrParentRecord->GetItem(_0_D_COLUMN_ZONEID);

						if( pItem->GetHasCheckbox() == FALSE )
							continue;
						if( pItem->IsChecked() == FALSE )
							continue;

						CXTPReportRecords* pFivParentRecords = pFrParentRecord->GetChilds();
						if (nullptr == pFivParentRecords)
							continue;

						for (int p= 0; p<  pFivParentRecords->GetCount(); p++)
						{
							CXTPReportRecord*  pFivParentRecord = pFivParentRecords->GetAt(p);
							pItem            = pFivParentRecord->GetItem(_0_D_COLUMN_ZONEID);

							if( pItem->GetHasCheckbox() == FALSE )
								continue;
							if( pItem->IsChecked() == FALSE )
								continue;

							KInterPathEditInfo* pAssignInfo = (KInterPathEditInfo*)pItem->GetItemData();

							DWORD_PTR dwPtr = (DWORD_PTR)pAssignInfo;
							setMBusAssignInfo.insert(dwPtr);
						}
					}
				}
			}
		}

		KInterModalPathInfo::ResultTransitEditInfo3(m_pTarget, m_mapDestResultInputValue, setMBusAssignInfo, KEMInterModalPathModeRegional, mapMainPathInfo, m_oMapResult, m_mapUrbanColor);
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

void KInterTransitPathEditDlg::MakeDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		// Urban data 수집
		Integer  nxEndZoneID = GetSelEndNodeId();
		AutoType iter        = m_mapEndZonePathMode.find(nxEndZoneID);
		AutoType itEnd       = m_mapEndZonePathMode.end();	

		if (iter != itEnd)
		{
			KEMInterModalPathMode emPathMode = iter->second;
			if (KEMInterModalPathModeUrban == emPathMode)
			{
				MakeUrbanDrawData();
			}
			else if (KEMInterModalPathModeRegional == emPathMode)
			{
				MakeRegionalDrawData();
			}
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


void KInterTransitPathEditDlg::MakeUrbanDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		m_mapDestResultInputValue.clear();

		std::set<DWORD_PTR> setMBusAssignInfo;

		Integer nxDZoneID = GetSelEndNodeId();
		std::vector<KInterPathEditInfo>* pvecTransitAssignInfo = nullptr;
		
		pvecTransitAssignInfo = IsHaveAutoTaxiAssignData(nxDZoneID); //auto
		if (nullptr != pvecTransitAssignInfo) {
			for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++) {
				KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;

				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
				setMBusAssignInfo.insert(dwPtr);
			}
		}

		pvecTransitAssignInfo = IsHaveBusAssignData(nxDZoneID);
		if (nullptr != pvecTransitAssignInfo) {
			for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++) {
				KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;

				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
				setMBusAssignInfo.insert(dwPtr);
			}
		}

		pvecTransitAssignInfo = IsHaveRailAssignData(nxDZoneID);
		if (nullptr != pvecTransitAssignInfo) {
			for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++) {
				KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;

				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
				setMBusAssignInfo.insert(dwPtr);
			}
		}

		pvecTransitAssignInfo = IsHaveBusRailAssignData(nxDZoneID);
		if (nullptr != pvecTransitAssignInfo) {
			for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++) {
				KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;

				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
				setMBusAssignInfo.insert(dwPtr);
			}
		}

		pvecTransitAssignInfo = IsHaveParkRideAssignData(nxDZoneID);
		if (nullptr != pvecTransitAssignInfo) {
			for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++) {
				KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;

				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
				setMBusAssignInfo.insert(dwPtr);
			}
		}

		pvecTransitAssignInfo = IsHaveKissRideAssignData(nxDZoneID);
		if (nullptr != pvecTransitAssignInfo) {
			for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++) {
				KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;

				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
				setMBusAssignInfo.insert(dwPtr);
			}
		}

		pvecTransitAssignInfo = IsHaveTaxiTransitAssignData(nxDZoneID);
		if (nullptr != pvecTransitAssignInfo) {
			for (size_t m = 0; m < (*pvecTransitAssignInfo).size(); m++) {
				KInterPathEditInfo &oTransitAssignInfo = (*pvecTransitAssignInfo)[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;

				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
				setMBusAssignInfo.insert(dwPtr);
			}
		}
		
		KInterModalPathInfo::ResultTransitEditInfo2(m_pTarget, setMBusAssignInfo, KEMInterModalPathModeUrban, OUT m_mapDestResultInputValue);
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

	TxLogDebugEndMsg();
}


void KInterTransitPathEditDlg::MakeRegionalDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		m_mapDestResultInputValue.clear();

		std::set<DWORD_PTR> setMBusAssignInfo; 

		Integer nxDZoneID = GetSelEndNodeId();
		std::vector<KRegionModePathInfo>* pvecRegionModePathInfo = nullptr;
		pvecRegionModePathInfo = IsHaveRegionalAssignData(nxDZoneID);

		if (nullptr == pvecRegionModePathInfo) {
			return;
		}

		for (size_t i= 0; i< (*pvecRegionModePathInfo).size(); i++)
		{
			KRegionModePathInfo &oRegionModePathInfo = (*pvecRegionModePathInfo)[i];

			KRegionModePathInfo* pRegionModePathInfo = &oRegionModePathInfo;
			if (nullptr == pRegionModePathInfo)
				continue;

			std::vector<KRegionPathDetail> &vecOnePathDetail = pRegionModePathInfo->vecOnePathDetail;
			for (size_t m = 0; m < vecOnePathDetail.size(); m++)
			{
				KRegionPathDetail &oRegionPathDetail = vecOnePathDetail[m];

				CollectRegionalRawData(oRegionPathDetail.mapAccAutoAssign,    OUT setMBusAssignInfo);
				CollectRegionalRawData(oRegionPathDetail.mapAccBusAssign,     OUT setMBusAssignInfo);
				CollectRegionalRawData(oRegionPathDetail.mapAccRailAssign,    OUT setMBusAssignInfo);
				CollectRegionalRawData(oRegionPathDetail.mapAccBusRailAssign, OUT setMBusAssignInfo);

				CollectRegionalRawData(oRegionPathDetail.mapEgrAutoAssign,    OUT setMBusAssignInfo);
				CollectRegionalRawData(oRegionPathDetail.mapEgrBusAssign,     OUT setMBusAssignInfo);
				CollectRegionalRawData(oRegionPathDetail.mapEgrRailAssign,    OUT setMBusAssignInfo);
				CollectRegionalRawData(oRegionPathDetail.mapEgrBusRailAssign, OUT setMBusAssignInfo);
			}
		}
		
		KInterModalPathInfo::ResultTransitEditInfo2(m_pTarget, setMBusAssignInfo, KEMInterModalPathModeRegional, OUT m_mapDestResultInputValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
	}
	catch (...)
	{
		TxLogDebugException();	
	} 

	TxLogDebugEndMsg();
}


void KInterTransitPathEditDlg::CollectRegionalRawData(std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapRegionalAssign, OUT std::set<DWORD_PTR> &a_setMBusAssignInfo)
{
	try 
	{
		auto iter  = a_mapRegionalAssign.begin();
		auto itEnd = a_mapRegionalAssign.end();

		for (; iter != itEnd; ++iter)
		{
			std::vector<KInterPathEditInfo> &vecAssignInfo = iter->second;
			if (vecAssignInfo.size() < 1)
				continue;

			for (size_t m = 0; m < vecAssignInfo.size(); m++)
			{
				KInterPathEditInfo &oTransitAssignInfo = vecAssignInfo[m];
				KInterPathEditInfo* pTransitAssignInfo = &oTransitAssignInfo;
				DWORD_PTR dwPtr = (DWORD_PTR)pTransitAssignInfo;
				a_setMBusAssignInfo.insert(dwPtr);
			}
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}


void KInterTransitPathEditDlg::MakeMainPathDrawData(KRegionModePathInfo* a_pRegionModePathInfo, KRegionPathDetail* a_pRegionPathDetail, std::map<int, std::vector<TMainPathInfo>> &a_mapMainPathInfo)
{
	try
	{
		if ( nullptr == a_pRegionModePathInfo || nullptr == a_pRegionPathDetail)
		{
			return;
		}

		int nMode = a_pRegionModePathInfo->nMode;
		
		COLORREF clrMode = RGB ( 255, 255, 255 );
		if (REGION_MODE_CODE_AUTO == nMode)
		{
		    AutoType iter  = m_mapUrbanColor.find(URBAN_MODE_AUTO);
			AutoType itEnd = m_mapUrbanColor.end();

			if (iter != itEnd)
			{
				TInterPathColor &oTInterPathColor = iter->second;
				clrMode = oTInterPathColor.clrPath;
			}
		}
		else
		{
			AutoType iter  = m_mapRegionalColor.find(nMode);
			AutoType itEnd = m_mapRegionalColor.end();

			if (iter != itEnd)
			{
				TInterPathColor &oTInterPathColor = iter->second;
				clrMode = oTInterPathColor.clrPath;
			}
		}

		TMainPathInfo oTMainPathInfo(nMode, 
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
			clrMode);

		AutoType iter  = a_mapMainPathInfo.find(nMode);
		AutoType itEnd = a_mapMainPathInfo.end();

		if (iter == itEnd)
		{
			std::vector<TMainPathInfo> vecMainPathInfo;
			vecMainPathInfo.push_back(oTMainPathInfo);
			a_mapMainPathInfo.insert(std::make_pair(nMode, vecMainPathInfo));
		}
		else
		{
			std::vector<TMainPathInfo> &vecMainPathInfo = iter->second;
			vecMainPathInfo.push_back(oTMainPathInfo);
			a_mapMainPathInfo.insert(std::make_pair(nMode, vecMainPathInfo));
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KInterTransitPathEditDlg::ModelessCancleFeedback(UINT nDialogID)
{

}


// 2019.03.11 명지대 요청사항 수정
void KInterTransitPathEditDlg::OnBnClickedBtnComparePath()
{
	std::vector<KInterPathEditInfoWrapper> arrUrban;
	std::vector<KInterPathRegionInfoWrapper> arrRegion;

	if (m_wndReportBCtrl.IsWindowVisible() == TRUE)
	{
		ReadCheckedUrbanInterModalPath(arrUrban);
	}
	else if (m_wndReportDCtrl.IsWindowVisible() == TRUE)
	{
		ReadCheckedRegionalInterModalPath(arrRegion);
	}
	else
	{
		return;
	}

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
		m_spPathCompareRegionDlg->ShowWindow(SW_SHOW);
	}
}



void KInterTransitPathEditDlg::ReadCheckedUrbanInterModalPath(std::vector<KInterPathEditInfoWrapper>& arr)
{
	try
	{
		CXTPReportRecordItem*  pItem = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i = 0; i< nCount; i++)
		{
			CXTPReportRecord*  pParentRecord = pRecords->GetAt(i);
			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			for (int k = 0; k< pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				if (pItem->GetHasCheckbox() == FALSE)
					continue;
				if (pItem->IsChecked() == FALSE)
					continue;

				KInterPathEditInfo* pAssignInfo = (KInterPathEditInfo*)pItem->GetItemData();
				if (NULL != pAssignInfo)
				{
					// pAssignInfo 정보의 InterModalPathMode를 설정해준다(읽어올때 해당 정보가 설정되지 않음)
					pAssignInfo->emInterModalPathMode = KEMInterModalPathModeUrban;
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
					arr.push_back(wr);
				}
			}
		}
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
}


void KInterTransitPathEditDlg::ReadCheckedRegionalInterModalPath(std::vector<KInterPathRegionInfoWrapper>& arr)
{
	try
	{
		CXTPReportRecordItem*  pItem = nullptr;
		CXTPReportRecords*     pRootRecords = m_wndReportDCtrl.GetRecords();
		int nCount = pRootRecords->GetCount();
		for (int i = 0; i< nCount; i++)
		{
			CXTPReportRecord*    pParentRecord = pRootRecords->GetAt(i);
			pItem = pParentRecord->GetItem(_0_D_COLUMN_ZONEID);
			if (pItem->GetHasCheckbox() == FALSE)
				continue;
			if (pItem->IsChecked() == FALSE)
				continue;
			
			KRegionModePathInfo* pRegionModePathInfo = (KRegionModePathInfo*)pItem->GetItemData();
			if (NULL == pRegionModePathInfo)
			{
				continue;
			}

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

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			if (nullptr == pSeParentRecords)
				continue;

			for (int k = 0; k< pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem = pSeParentRecord->GetItem(_0_D_COLUMN_ZONEID);

				if (pItem->GetHasCheckbox() == FALSE)
					continue;

				if (pItem->IsChecked() == FALSE)
					continue;

				CString strPathName = pItem->GetCaption();
				KRegionPathDetail* pRegionPathDetail = (KRegionPathDetail*)pItem->GetItemData();
				if (pRegionPathDetail == NULL)
				{
					continue;
				}

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
				wr.strPathName = strPathName;
				wr.pRegionPathInfo = pRegionModePathInfo;
				wr.pRegionPathDetail = pRegionPathDetail;
				arr.push_back(wr);
			}
		}

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
}
// 2019.03.12 명지대 요청사항 수정 끝



void KInterTransitPathEditDlg::UpdatePathCompareButtonState()
{
	BOOL bEnableButton = FALSE;
	CXTPReportRecordItem*  pItem = nullptr;

	if (m_wndReportBCtrl.IsWindowVisible() == TRUE)
	{
		CXTPReportRecords*     pRecords = m_wndReportBCtrl.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i< nRecordCount; i++)
		{
			CXTPReportRecord*  pParentRecord = pRecords->GetAt(i);
			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			for (int k = 0; k< pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem = pSeParentRecord->GetItem(_0_B_COLUMN_ZONEID);

				if ((pItem->GetHasCheckbox() == FALSE) || (pItem->IsChecked() == FALSE))
				{
					continue;
				}
				else
				{
					bEnableButton = TRUE; 
					break;
				}

			}
		}
	}
	else if (m_wndReportDCtrl.IsWindowVisible() == TRUE)
	{
		CXTPReportRecords*     pRootRecords = m_wndReportDCtrl.GetRecords();
		int nCount = pRootRecords->GetCount();
		for (int i = 0; i< nCount; i++)
		{
			CXTPReportRecord*    pParentRecord = pRootRecords->GetAt(i);
			pItem = pParentRecord->GetItem(_0_D_COLUMN_ZONEID);
			if ((pItem->GetHasCheckbox() == FALSE) || (pItem->IsChecked() == FALSE))
			{
				continue;
			}
			else
			{
				bEnableButton = TRUE;
				break;
			}
		}
	}

	GetDlgItem(IDC_BTN_COMPARE_PATH)->EnableWindow(bEnableButton);
}
