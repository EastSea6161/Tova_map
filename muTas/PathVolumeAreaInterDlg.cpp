// PathVolumeAreaInterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PathVolumeAreaInterDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Object.h"
#include "Target.h"
#include "MapView.h"
#include "DBasePathVolume.h"
#include "ImChampMultiZoneFind.h"
#include "TCoordinate.h"

#include "KBulkDBase.h"
#include "KBulkDBase2File.h"

#include "DBaseAssignment.h"
#include "InspectionNetwork.h"
#include "Launcher.h"
#include "WriteCodeAgrument.h"

#include "NodeSelectionInfo.h"
#include "ImChampFileExist.h"

#include "ImChampMapDefine.h"
#include "ImChampDir.h"

#include "..\QBicCommon\ColorRangeGen.h"
//^#include "MunsellColorCircle.h"
#include "KDBaseAccessibilityModify.h"

namespace PathVolumeAreaInterENUM
{
	enum
	{
		_0_A_COLUMN_SELECT     = 0,
		_1_A_COLUMN_ZONEID     = 1,
		_2_A_COLUMN_NAME       = 2
	};

	enum
	{
		_0_B_COLUMN_SELECT     = 0,
		_1_B_COLUMN_ZONEID     = 1,
		_2_B_COLUMN_NAME       = 2
	};
}

using namespace PathVolumeAreaInterENUM;
// KPathVolumeAreaInterDlg 대화 상자입니다.

const int nZoneCharIndex = 53;
const int nZoneSize		 = 11;

IMPLEMENT_DYNAMIC(KPathVolumeAreaInterDlg, KResizeDialogEx)

KPathVolumeAreaInterDlg::KPathVolumeAreaInterDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPathVolumeAreaInterDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pProject(a_pProject)
	, m_pMapView(a_pMapView)
	, m_strRunErrMsg(_T(""))
	, m_bResultRun(false)
	, m_dMaxVolume(0.0)
	, m_dMinVolume(0.0)
	, m_clrOutRange(RGB ( 221, 217, 195 ))
	, m_clrOriginZone(RGB(255,0,0))
    , m_clrDestinationZone(RGB(1, 0, 255))
	, m_nComboModeIdx(-1)
	, m_dRadius(1000.0)
{
	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_AUTO,   TInterPathColor(_T("승용차/택시"),    RGB ( 228, 108,  10))));
			//m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_TAXI,   TInterPathColor(_T("Taxi"),    RGB ( 255, 255, 100))));
			m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_BUS,    TInterPathColor(_T("버스"),     RGB (   0, 128,   0))));
			m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_SUBWAY, TInterPathColor(_T("도시철도"),  RGB (   0,   0, 255))));
			m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_WALK,   TInterPathColor(_T("도보"),    RGB ( 102, 102, 153))));
		}
		else {
			m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_AUTO,   TInterPathColor(_T("PassengerCar/Taxi"),    RGB ( 228, 108,  10))));
			//m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_TAXI,   TInterPathColor(_T("Taxi"),    RGB ( 255, 255, 100))));
			m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_BUS,    TInterPathColor(_T("Bus"),     RGB (   0, 128,   0))));
			m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_SUBWAY, TInterPathColor(_T("Rail"),  RGB (   0,   0, 255))));
			m_mapUrbanColor.insert(std::make_pair(URBAN_MODE_WALK,   TInterPathColor(_T("Walk"),    RGB ( 102, 102, 153))));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

KPathVolumeAreaInterDlg::~KPathVolumeAreaInterDlg()
{
	try
	{
		KIOTables* pIOTables = m_pTarget->Tables();	
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);

		pIOTable->Unregister(this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	CloseWindowProc();
}

void KPathVolumeAreaInterDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,                m_ctrlReportA);
	DDX_Control(pDX, IDC_REPORT_B,                m_ctrlReportB);
	DDX_Control(pDX, IDC_COMBO1,                  m_cboMode);
	DDX_Control(pDX, IDC_CLR_OUT,                 m_cpOutRangeColor);

	DDX_Control(pDX, IDC_REPORT_COLORA,            m_wndReportColorACtrl);
	DDX_Control(pDX, IDC_REPORT_COLORB,            m_wndReportColorBCtrl);
}


BEGIN_MESSAGE_MAP(KPathVolumeAreaInterDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1,      &KPathVolumeAreaInterDlg::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON2,        &KPathVolumeAreaInterDlg::OnBnClickedButtonSearchAreaCodeOrg)
	ON_BN_CLICKED(IDC_BUTTON4,        &KPathVolumeAreaInterDlg::OnBnClickedButtonSearchAreaCodeDest)
	ON_BN_CLICKED(IDC_BUTTON1,        &KPathVolumeAreaInterDlg::OnBnClickedButtonPolyDrawOrg)
	ON_BN_CLICKED(IDC_BUTTON3,        &KPathVolumeAreaInterDlg::OnBnClickedButtonPolyDrawDest)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,              &KPathVolumeAreaInterDlg::OnReportAItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_A, &KPathVolumeAreaInterDlg::OnReportACheckItem)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_B,              &KPathVolumeAreaInterDlg::OnReportBItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_B, &KPathVolumeAreaInterDlg::OnReportBCheckItem)
	ON_BN_CLICKED(IDC_CHECK1, &KPathVolumeAreaInterDlg::OnBnClickedCheckASelectAll)
	ON_BN_CLICKED(IDC_CHECK2, &KPathVolumeAreaInterDlg::OnBnClickedCheckBSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH2, &KPathVolumeAreaInterDlg::OnBnClickedButtonSearch)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &KPathVolumeAreaInterDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KPathVolumeAreaInterDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL, &KPathVolumeAreaInterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_OUTRANGE, &KPathVolumeAreaInterDlg::OnBnClickedCheckOutrange)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KPathVolumeAreaInterDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_CHECK3, &KPathVolumeAreaInterDlg::OnBnClickedCheckZone)
	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_COLORA, &KPathVolumeAreaInterDlg::OnReportColorAItemClick)
	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_COLORB, &KPathVolumeAreaInterDlg::OnReportColorBItemClick)
END_MESSAGE_MAP()


// KPathVolumeAreaInterDlg 메시지 처리기입니다.

void KPathVolumeAreaInterDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);
		if(strSubjectName.CompareNoCase(TABLE_NODE) == 0)
		{
			m_ctrlReportA.ResetContent();
			m_ctrlReportB.ResetContent();
			ResetVolumeData();

			KDBasePathVolume::GetZoneNodeInfo(m_pTarget, m_mapZoneNodeData);
			SetGuideNodeInfo(m_mapZoneNodeData, m_vecDrawGuideNode);
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


void KPathVolumeAreaInterDlg::ActivatedFrameWndNotifyProcess()
{

}


void KPathVolumeAreaInterDlg::DeActivatedFrameWndNotifyProcess()
{

}


void KPathVolumeAreaInterDlg::ResizeComponent()
{
	SetResize(IDC_STATIC3,			      SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_EDIT1,			      SZ_TOP_LEFT,     SZ_TOP_LEFT);

	SetResize(IDC_CHECK1,	    		  SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_REPORT_A,				  SZ_TOP_LEFT,     SZ_MIDDLE_RIGHT);

	SetResize(IDC_STATIC4,			      SZ_MIDDLE_LEFT,  SZ_MIDDLE_LEFT);
	SetResize(IDC_BUTTON3,			      SZ_MIDDLE_LEFT,  SZ_MIDDLE_LEFT);
	SetResize(IDC_BUTTON4,			      SZ_MIDDLE_LEFT,  SZ_MIDDLE_LEFT);
	SetResize(IDC_CHECK2,			      SZ_MIDDLE_LEFT,  SZ_MIDDLE_LEFT);
	SetResize(IDC_REPORT_B,				  SZ_MIDDLE_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SEARCH2,		  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR8,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC1,       		  SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_REPORT_COLORA,       	  SZ_BOTTOM_LEFT,    SZ_BOTTOM_CENTER);
	SetResize(IDC_SEPRATOR11,     	      SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
	SetResize(IDC_STATIC2, 			      SZ_BOTTOM_CENTER,  SZ_BOTTOM_CENTER);
	SetResize(IDC_REPORT_COLORB,       	  SZ_BOTTOM_CENTER,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_MIN_VALUE3,      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_MAX_VALUE3,	  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC22,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC23,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MIN_VALUE,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAX_VALUE,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MINSIZE,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAXSIZE,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CLEAR,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DRAW,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);		
	SetResize(IDC_CHECK3,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_ZONEACCESS,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_STATION,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_CHECK_OUTRANGE,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_OUT,       	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR9,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);		
	SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KPathVolumeAreaInterDlg::TargetRegionFeedback( std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate )
{
	try
	{
		ClearMapDraw(true);

		BOOL bSelectAll(FALSE);
		size_t nxCount = a_vecSelection.size();
		if (MAP_NODE_START == m_bPointFlag)
		{
			if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
				bSelectAll = TRUE;
			else
				bSelectAll = FALSE;

			m_setZoneOrg.clear();
			for (size_t i= 0; i< nxCount; i++)
			{
				Integer &nxID = a_vecSelection[i];

				if (m_mapZoneNodeData.find(nxID) == m_mapZoneNodeData.end())
					continue;

				m_setZoneOrg.insert(nxID);
			}

			UpdateReport(m_ctrlReportA, m_setZoneOrg, bSelectAll);
		}
		else
		{
			if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
				bSelectAll = TRUE;
			else
				bSelectAll = FALSE;

			m_setZoneDest.clear();
			for (size_t i= 0; i< nxCount; i++)
			{
				Integer &nxID = a_vecSelection[i];

				if (m_mapZoneNodeData.find(nxID) == m_mapZoneNodeData.end())
					continue;

				m_setZoneDest.insert(nxID);
			}

			UpdateReport(m_ctrlReportB, m_setZoneDest, bSelectAll);
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


void KPathVolumeAreaInterDlg::GetUrbanTransitType()
{
	try
	{
		m_mapUrbanCodeName.clear();

		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeAutoTaxi, _T("승용차/택시(도시부)")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeBus, _T("버스")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeRail, _T("도시철도")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeBusRail, _T("버스+도시철도")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeParkRide, _T("Park & Ride")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeKissRide, _T("Kiss & Ride")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeTaxiTransit, _T("택시 & 대중교통")));
		}
		else {
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeAutoTaxi, _T("Urban PassengerCar/Taxi")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeBus, _T("Bus")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeRail, _T("Rail")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeBusRail, _T("Bus+Rail")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeParkRide, _T("Park & Ride")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeKissRide, _T("Kiss & Ride")));
			m_mapUrbanCodeName.insert(std::make_pair(KEMInterModeTaxiTransit, _T("Taxi & Transit")));
		}
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KPathVolumeAreaInterDlg::GetRegionTransitType(void)
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
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_mapRegionalCodeName.insert(std::make_pair(REGION_MODE_CODE_AUTO, _T("Auto(지역간)")));
		}
		else {
			m_mapRegionalCodeName.insert(std::make_pair(REGION_MODE_CODE_AUTO, _T("PassengerCar")));
		}
		//m_mapRegionalCodeName.insert(std::make_pair(REGION_MODE_CODE_AUTO, _T("Auto")));

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



BOOL KPathVolumeAreaInterDlg::OnInitDialog()
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

	KReportCtrlSetting::Default(m_ctrlReportA, FALSE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_ctrlReportB, FALSE, FALSE, FALSE);

	KReportCtrlSetting::Default(m_wndReportColorACtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::HideHeader(m_wndReportColorACtrl);
	KReportCtrlSetting::Default(m_wndReportColorBCtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::HideHeader(m_wndReportColorBCtrl);

	KColorPickerSetting::DefaultCtrl(m_cpOutRangeColor, m_clrOutRange);

	CWaitCursor wc;

	CString strMinPathVol(_T("0.001"));
	SetDlgItemText(IDC_EDIT1, strMinPathVol);

	CheckDlgButton(IDC_CHECK1, BST_CHECKED);
	CheckDlgButton(IDC_CHECK2, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_LABEL, BST_UNCHECKED);
	UpdateOutRange();
	CheckDlgButton(IDC_CHECK3, BST_CHECKED);

	SetDlgItemText(IDC_EDIT_MINSIZE,   _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE,   _T("15.0"));

	GetUrbanTransitType();
	GetRegionTransitType();
	GetDBModeInfo();
	KDBasePathVolume::GetZoneNodeInfo(m_pTarget, m_mapZoneNodeData);
	SetGuideNodeInfo(m_mapZoneNodeData, m_vecDrawGuideNode);

	InitReportHeader();
	m_ctrlReportA.Populate();
	m_ctrlReportB.Populate();

	InitReportColorAHeader();
	UpdateReportColorA();
	InitReportColorBHeader();
	UpdateReportColorB();

	ResizeComponent();

	try
	{
		KIOTables* pIOTables = m_pTarget->Tables();	
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);

		pIOTable->Register(this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPathVolumeAreaInterDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_0_A_COLUMN_SELECT,   _T("Select"),   40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_1_A_COLUMN_ZONEID,   _T("Origin ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("기점 존 노드 ID"));
    }

	pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_2_A_COLUMN_NAME,     _T("Name"),     60));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("존 명칭"));
    }

	pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_0_B_COLUMN_SELECT,   _T("Select"),   40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_1_B_COLUMN_ZONEID,   _T("Destination ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("종점 존 노드 ID"));
    }

	pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_2_B_COLUMN_NAME,     _T("Name"),     60));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("존 명칭"));
    }
}


void KPathVolumeAreaInterDlg::InitReportColorAHeader()
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


void KPathVolumeAreaInterDlg::UpdateReportColorA()
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


void KPathVolumeAreaInterDlg::InitReportColorBHeader()
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


void KPathVolumeAreaInterDlg::UpdateReportColorB()
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


void KPathVolumeAreaInterDlg::SetGuideNodeInfo(std::map<Integer, CString> &a_mapZoneNodeData, std::vector<TDrawNodeInfo> &a_vecDrawGuideNode)
{
	try
	{
		AutoType iter  = a_mapZoneNodeData.begin();
		AutoType itEnd = a_mapZoneNodeData.end();

		a_vecDrawGuideNode.clear();

		TPreferenceSymbol &oZoneSymbol = ImChampMapDefineSymbol::ZoneSymbol;

		while (iter != itEnd)
		{
			Integer nxNodeID = iter->first;

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID         = nxNodeID;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxNodeID);
			oNodeInfo.TLabel          = strNodeLable;

			oNodeInfo.TSymbol = oZoneSymbol.nCharIndex;
			oNodeInfo.TSize           = oZoneSymbol.dSize;
			oNodeInfo.TColorRGB       = oZoneSymbol.clrSymbol;

			a_vecDrawGuideNode.push_back(oNodeInfo);

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::GetDBModeInfo()
{
	try
	{
		m_cboMode.ResetContent();
		m_cboMode.Clear();
		m_mapModeInfo.clear();

		int     nIDXCombo   = 0; //ComboBox의 Index all - 0, 수단1 ... 수단n
		int     nModeID     = 0;
		CString strModeName = _T("All");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strModeName = _T("전체");
		}
		CString strView     = _T("");

		//int nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
		int nCur              = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG_K);
		}
		else {
			nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
		}
		m_cboMode.SetItemData( nCur, DEFAULT_MODEID );

		strView.Format(_T("%d: %s"), nIDXCombo, strModeName);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strView.Format(_T("%s"), strModeName);
		}
		nCur = m_cboMode.AddString(strView);
		m_cboMode.SetItemData( nCur, nIDXCombo );
		nIDXCombo++;

		AutoType iter  = m_mapUrbanCodeName.begin();
		AutoType itEnd = m_mapUrbanCodeName.end();
		while (iter != itEnd)
		{
			int     nModeID     = iter->first;
			CString strModeName = iter->second;

			strView.Format(_T("%d: %s"), nIDXCombo, strModeName);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strView.Format(_T("%s"), strModeName);
			}
			nCur = m_cboMode.AddString(strView);
			m_cboMode.SetItemData( nCur, nIDXCombo );
			
			TInAreaComboModeInfo oTInAreaComboModeInfo;
			oTInAreaComboModeInfo.emInterModalPathMode = KEMInterModalPathModeUrban;
			oTInAreaComboModeInfo.nModeID = nModeID;

			m_mapModeInfo.insert(std::make_pair(nIDXCombo, oTInAreaComboModeInfo));

			nIDXCombo++;
			++iter;
		}

		AutoType iterB  = m_mapRegionalCodeName.begin();
		AutoType itEndB = m_mapRegionalCodeName.end();
		while (iterB != itEndB)
		{
			int     nModeID     = iterB->first;
			CString strModeName = iterB->second;

			strView.Format(_T("%d: %s"), nIDXCombo, strModeName);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strView.Format(_T("%s"), strModeName);
			}
			nCur = m_cboMode.AddString(strView);
			m_cboMode.SetItemData( nCur, nIDXCombo );

			TInAreaComboModeInfo oTInAreaComboModeInfo;
			oTInAreaComboModeInfo.emInterModalPathMode = KEMInterModalPathModeRegional;
			oTInAreaComboModeInfo.nModeID = nModeID;

			m_mapModeInfo.insert(std::make_pair(nIDXCombo, oTInAreaComboModeInfo));

			nIDXCombo++;
			++iterB;
		}

		if (m_cboMode.GetCount() > 1)
		{
			m_cboMode.SetCurSel(1);	
		}

		UpdateData(FALSE);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


int KPathVolumeAreaInterDlg::GetSelMode()
{
	UpdateData(TRUE);

	int nSelCur = m_cboMode.GetCurSel();
	int nModeID = (int)m_cboMode.GetItemData(nSelCur);

	if(DEFAULT_MODEID == nModeID)
	{
		AfxMessageBox(_T("통행수단을 선택해 주세요."));
		m_cboMode.SetFocus();
	}

	return nModeID;
}


void KPathVolumeAreaInterDlg::OnCbnSelchangeComboMode()
{
	UpdateData(TRUE);

	int nSelCur = m_cboMode.GetCurSel();
	int nIdx    = m_cboMode.GetItemData(nSelCur);

	AutoType iter = m_mapModeInfo.find(nIdx);

	//ResetVolumeData();
}

void KPathVolumeAreaInterDlg::OnBnClickedButtonSearchAreaCodeOrg()
{
	try
	{
		KImChampMultiZoneFind oFindDlg(m_pTarget);
		if (oFindDlg.DoModal() == IDOK)
		{
			oFindDlg.GetSelectedID(m_setZoneOrg);

			BOOL bSelectAll(FALSE);
			if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
				bSelectAll = TRUE;
			else
				bSelectAll = FALSE;

			UpdateReport(m_ctrlReportA, m_setZoneOrg, bSelectAll);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::OnBnClickedButtonSearchAreaCodeDest()
{
	try
	{
		KImChampMultiZoneFind oFindDlg(m_pTarget);
		if (oFindDlg.DoModal() == IDOK)
		{
			oFindDlg.GetSelectedID(m_setZoneDest);

			BOOL bSelectAll(FALSE);
			if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
				bSelectAll = TRUE;
			else
				bSelectAll = FALSE;

			UpdateReport(m_ctrlReportB, m_setZoneDest, bSelectAll);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::UpdateReport(CXTPReportControl &a_ctrlReport, std::set<Integer> &a_setZoneID, BOOL a_bSelectAll)
{
	a_ctrlReport.ResetContent();

	try
	{
		AutoType iter  = a_setZoneID.begin();
		AutoType itEnd = a_setZoneID.end();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		CString strID(_T(""));
		while (iter != itEnd)
		{
			Integer nxID = *iter;

			pRecord = a_ctrlReport.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(a_bSelectAll);
			pItem->SetItemData((DWORD_PTR)nxID);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			strID.Format(_T("%I64d"), nxID);
			pItem->SetCaption(strID);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			CString strName(_T(""));
			AutoType iterZone = m_mapZoneNodeData.find(nxID);
			if (iterZone != m_mapZoneNodeData.end())
			{
				strName = iterZone->second;
			}
			pItem->SetCaption(strName);
			pItem->SetAlignment(DT_LEFT);

			++iter;
		}
	}
	catch (...)
	{
		a_ctrlReport.ResetContent();
		TxLogDebugException();	
	}

	a_ctrlReport.Populate();

	ResetVolumeData();
}


void KPathVolumeAreaInterDlg::OnBnClickedButtonPolyDrawOrg()
{
	try
	{
		m_bPointFlag = MAP_NODE_START;

		std::vector<TDrawNodeInfo> vecDrawTotalNode = m_vecDrawGuideNode;

		GetSelectedDrawNodeInfo(vecDrawTotalNode, false);

		m_pMapView->TxDrawCompositeNode(vecDrawTotalNode, false, true);
		m_pMapView->TargetRegionInsert(0, this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::OnBnClickedButtonPolyDrawDest()
{
	try
	{
		m_bPointFlag = MAP_NODE_END;

		std::vector<TDrawNodeInfo> vecDrawTotalNode = m_vecDrawGuideNode;

		GetSelectedDrawNodeInfo(vecDrawTotalNode, false);

		m_pMapView->TxDrawCompositeNode(vecDrawTotalNode, false, true);
		m_pMapView->TargetRegionInsert(0, this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	}

	ResetVolumeData();
}


void KPathVolumeAreaInterDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK2, BST_UNCHECKED);
	}

	ResetVolumeData();
}


void KPathVolumeAreaInterDlg::OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_0_A_COLUMN_SELECT == nIndex)
	{
		return;
	}

	try
	{
		CXTPReportRecord*	     pRecord     = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem*    pItem       = pRecord->GetItem(_0_A_COLUMN_SELECT);
		Integer nxZoneID = (Integer)pItem->GetItemData();

		m_pMapView->PanToFeature(KLayerID::Node(), nxZoneID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_0_B_COLUMN_SELECT == nIndex)
	{
		return;
	}

	try
	{
		CXTPReportRecord*	     pRecord     = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem*    pItem       = pRecord->GetItem(_0_B_COLUMN_SELECT);
		Integer nxZoneID = (Integer)pItem->GetItemData();

		m_pMapView->PanToFeature(KLayerID::Node(), nxZoneID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::OnBnClickedCheckASelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;

		CXTPReportRecords*    pRecords = m_ctrlReportA.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);

			pItem->SetChecked(bSelectAll);
		}

		m_ctrlReportA.RedrawControl();

		ResetVolumeData();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::OnBnClickedCheckBSelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;

		CXTPReportRecords*    pRecords = m_ctrlReportB.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_B_COLUMN_SELECT);

			pItem->SetChecked(bSelectAll);
		}

		m_ctrlReportB.RedrawControl();

		ResetVolumeData();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::OnBnClickedButtonSearch()
{
	try
	{
		m_nComboModeIdx = GetSelMode();
		if(DEFAULT_MODEID == m_nComboModeIdx)
			return;

		CString strPathVol(_T(""));
		GetDlgItemText(IDC_EDIT1, strPathVol);

		if (QBicStringChecker::IsNumeric(strPathVol) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		m_dMinPathVol = _ttof(strPathVol);
		if (m_dMinPathVol < 0.0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}
		GetCheckedOrgDestZone(m_ctrlReportA, _0_A_COLUMN_SELECT, m_setSelectedZoneOrg);
		if (m_setSelectedZoneOrg.size() < 1)
		{
			AfxMessageBox(_T("기점을 선택해 주세요."));
			return;
		}

		GetCheckedOrgDestZone(m_ctrlReportB, _0_B_COLUMN_SELECT, m_setSelectedZoneDest);
		if (m_setSelectedZoneDest.size() < 1)
		{
			AfxMessageBox(_T("종점을 선택해 주세요."));
			return;
		}

		ResetVolumeData();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(PathVolumeThreadCaller, this, false, nLang);

		if( !m_bResultRun)
		{
			AfxMessageBox(m_strRunErrMsg);
			return;
		}

		UpdateMinMax();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::GetCheckedOrgDestZone(CXTPReportControl &a_ctrlReport, int a_nCheckBoxItemIndex, std::set<Integer> &a_setSelectedZone)
{
	try
	{
		a_setSelectedZone.clear();	

		CXTPReportRecords*    pRecords = a_ctrlReport.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt = pRecords->GetCount();

		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);

			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}

			Integer nxZoneID = (Integer)pItem->GetItemData();

			a_setSelectedZone.insert(nxZoneID);
		}
	}
	catch (...)
	{
		a_setSelectedZone.clear();
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::ResetVolumeData()
{
	try
	{
		ClearMapDraw(false);
		DrawSelectedOrgDestination(true);

		InitVolMinMax();
		SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE,  _T(""));

		TInMapResultInputValue oEmptyObject;
		m_oInMapResultTripInfo  = oEmptyObject; // 초기화
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


unsigned __stdcall KPathVolumeAreaInterDlg::PathVolumeThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("복합수단 수단별 구역간 이용경로 통행량 분석 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Area-to-Area Intermodal Path Analysis Module..."));
	}

	KPathVolumeAreaInterDlg* pDlg = (KPathVolumeAreaInterDlg*)pParameter->GetParameter();
	pDlg->GetSearchPathVolume();

	return 0;
}


void KPathVolumeAreaInterDlg::GetSearchPathVolume()
{
	TxLogDebugStartMsg();
	m_strRunErrMsg = _T("");
	m_bResultRun   = true;
	try
	{
		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
		}
		if (! ReadOutPutFile())
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Saving Output Data Failed."));
			}
			ThrowException(_T("Output Volume 정보 입력 실패."));
		}
	}
	catch(KExceptionPtr ex)
	{
		m_strRunErrMsg = ex->GetErrorMessage();
		m_bResultRun = false;
		return;
	}
	catch(...)
	{
		m_strRunErrMsg = _T("모듈 실행 중 예상치 못한 오류가 발생하였습니다.");
		m_bResultRun = false;
		return;
	}

	TxLogDebugEndMsg();
	m_bResultRun = true;
}


void KPathVolumeAreaInterDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KPathVolumeAreaInterDlg::ReadOutPutFile()
{
	InitVolMinMax();

	try
	{
		TInMapResultInputValue oEmptyObject;
		m_oInMapResultTripInfo = oEmptyObject; // 초기화

		std::vector<int> vecSelectedUrbanCode;
		std::set<int>    setSelectedRegionalCode;

#pragma region 선택한 Mode 정보 수집
		KEMInterModalPathMode emInterModalPathMode;
		int nModeID(-1);

		if (0 == m_nComboModeIdx) // all
		{
			AutoType iter  = m_mapUrbanCodeName.begin();
			AutoType itEnd = m_mapUrbanCodeName.end();
			while (iter != itEnd)
			{
				nModeID = iter->first;
				vecSelectedUrbanCode.push_back(nModeID);
				++iter;
			}

			AutoType iterB  = m_mapRegionalCodeName.begin();
			AutoType itEndB = m_mapRegionalCodeName.end();
			while (iterB != itEndB)
			{
				nModeID     = iterB->first;
				setSelectedRegionalCode.insert(nModeID);
				++iterB;
			}
		}
		else
		{
			AutoType itFindMode = m_mapModeInfo.find(m_nComboModeIdx);
			if (itFindMode != m_mapModeInfo.end())
			{
				TInAreaComboModeInfo &oTInAreaComboModeInfo = itFindMode->second;
				emInterModalPathMode = oTInAreaComboModeInfo.emInterModalPathMode;
				nModeID              = oTInAreaComboModeInfo.nModeID;

				if (KEMInterModalPathModeUrban == emInterModalPathMode)
					vecSelectedUrbanCode.push_back(nModeID);
				else
					setSelectedRegionalCode.insert(nModeID);
			}
			else
			{
				throw 1;
			}
		}
#pragma endregion 선택한 Mode 정보 수집

		CString strDataFileName(_T(""));
		CString strSavePath(_T(""));
	
		std::vector<KInterPathEditInfo> vecInterAssignTotalInfo;
		std::vector<TMainPathInfo>      vecMainPathInfo;  // regional mode Main 정보 수집
	
		AutoType iter  = m_setSelectedZoneOrg.begin();
		AutoType itEnd = m_setSelectedZoneOrg.end();

		size_t nxSelUrbanCnt  = vecSelectedUrbanCode.size();
		size_t nxSelRegionCnt = setSelectedRegionalCode.size();

		while (iter != itEnd)
		{
			Integer nxOrgZoneID = *iter;

			// Urban Assign
			for (size_t i= 0; i< nxSelUrbanCnt; i++)
			{
				int &nUrbanCode = vecSelectedUrbanCode[i];

				std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>>  mapOrgInterAssign;

				if (KEMInterModeAutoTaxi == nUrbanCode)
				{
					strDataFileName.Format(_T("%s%I64d_AutoTaxiPath.dat"), INTERMODAL_RESULT_PATH,  nxOrgZoneID);
					strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
					ReadAutoTaxiPathFile(strSavePath, m_setSelectedZoneDest, mapOrgInterAssign);
				}
				else if (KEMInterModeBus == nUrbanCode)
				{
					strDataFileName.Format(_T("%s%I64d_BusPath.dat"), INTERMODAL_RESULT_PATH, nxOrgZoneID);
					strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
					ReadBusPathFile(strSavePath, m_setSelectedZoneDest, mapOrgInterAssign);
				}
				else if (KEMInterModeRail == nUrbanCode)
				{
					strDataFileName.Format(_T("%s%I64d_RailPath.dat"), INTERMODAL_RESULT_PATH, nxOrgZoneID);
					strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(),DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
					ReadRailPathFile(strSavePath, m_setSelectedZoneDest, mapOrgInterAssign);
				}
				else if (KEMInterModeBusRail == nUrbanCode)
				{
					strDataFileName.Format(_T("%s%I64d_BusRailPath.dat"), INTERMODAL_RESULT_PATH, nxOrgZoneID);
					strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
					ReadBusRailPathFile(strSavePath, m_setSelectedZoneDest, mapOrgInterAssign);
				}
				else if (KEMInterModeParkRide == nUrbanCode)
				{
					strDataFileName.Format(_T("%s%I64d_ParkandRidePath.dat"), INTERMODAL_RESULT_PATH, nxOrgZoneID);
					strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
					ReadParkRidePathFile(strSavePath, m_setSelectedZoneDest, mapOrgInterAssign);
				}
				else if (KEMInterModeKissRide == nUrbanCode)
				{
					strDataFileName.Format(_T("%s%I64d_KissandRidePath.dat"), INTERMODAL_RESULT_PATH, nxOrgZoneID);
					strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
					ReadKissRidePathFile(strSavePath, m_setSelectedZoneDest, mapOrgInterAssign);
				}
				else if (KEMInterModeTaxiTransit == nUrbanCode)
				{
					strDataFileName.Format(_T("%s%I64d_TaxiandTransitPath.dat"), INTERMODAL_RESULT_PATH, nxOrgZoneID);
					strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
					ReadTaxiTransitPathFile(strSavePath, m_setSelectedZoneDest, mapOrgInterAssign);
				}

				CollectTotalTransitAssignInfo(mapOrgInterAssign, vecInterAssignTotalInfo);
			}

			// Regional Assign
			if (nxSelRegionCnt > 0)
			{
				std::map<Integer, std::vector<KRegionModePathInfo>> mapRegionAssign;

				strDataFileName.Format(_T("%s%I64d_RegionalPath.dat"), INTERMODAL_RESULT_PATH, nxOrgZoneID);
				strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME ,strDataFileName);
				ReadRegionalPathFile(strSavePath, m_setSelectedZoneDest, mapRegionAssign);

				AutoType iterRegion  = mapRegionAssign.begin();
				AutoType itEndRegion = mapRegionAssign.end();

				//setSelectedRegionalCode

				while (iterRegion != itEndRegion)
				{
					std::vector<KRegionModePathInfo> vecRegionAssign = iterRegion->second;
					for (size_t i= 0; i< vecRegionAssign.size(); i++)
					{
						KRegionModePathInfo &oRegionModePathInfo = vecRegionAssign[i];
						int nRegionCode    = oRegionModePathInfo.nMode;

						if (setSelectedRegionalCode.find(nRegionCode) == setSelectedRegionalCode.end())
						{
							continue;
						}

						std::vector<KRegionPathDetail> &vecOnePathDetail = oRegionModePathInfo.vecOnePathDetail;
						for (size_t m = 0; m < vecOnePathDetail.size(); m++)
						{
							KRegionPathDetail &oRegionPathDetail = vecOnePathDetail[m];

							MakeMainPathData(&oRegionModePathInfo, &oRegionPathDetail, vecMainPathInfo);

							CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccAutoAssign,		vecInterAssignTotalInfo);
							CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccBusAssign,		vecInterAssignTotalInfo);
							CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccRailAssign,		vecInterAssignTotalInfo);
							CollectRegionalModeAssignInfo(oRegionPathDetail.mapAccBusRailAssign,	vecInterAssignTotalInfo);
							CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrAutoAssign,		vecInterAssignTotalInfo);
							CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrBusAssign,		vecInterAssignTotalInfo);
							CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrRailAssign,		vecInterAssignTotalInfo);
							CollectRegionalModeAssignInfo(oRegionPathDetail.mapEgrBusRailAssign,	vecInterAssignTotalInfo);
						}
					}

					++iterRegion;
				}
			}

			++iter;
		}

		KInterModalPathInfo::ResultTransitTripInfo(m_pTarget, vecInterAssignTotalInfo, vecMainPathInfo, m_oInMapResultTripInfo);
 		SetVolMinMax(m_oInMapResultTripInfo);
	}
	catch (...)
	{
		InitVolMinMax();
		TxLogDebugException();
		return false;
	}    

	return true;
}


void KPathVolumeAreaInterDlg::CollectTotalTransitAssignInfo(std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>>  &a_mapOrgInterAssign, std::vector<KInterPathEditInfo> &a_vecInterAssignTotalInfo)
{
	try
	{
		AutoType iter  = a_mapOrgInterAssign.begin();
		AutoType itEnd = a_mapOrgInterAssign.end();

		while (iter != itEnd)
		{
			std::vector<KInterPathEditInfo> &vecInterAssignInfo = iter->second;
			size_t nxCount              = vecInterAssignInfo.size();

			for (size_t i= 0; i< nxCount; i++)
			{
				a_vecInterAssignTotalInfo.push_back(vecInterAssignInfo[i]);
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::MakeMainPathData(KRegionModePathInfo* a_pRegionModePathInfo, KRegionPathDetail* a_pRegionPathDetail, std::vector<TMainPathInfo> &a_vecMainPathInfo)
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


void KPathVolumeAreaInterDlg::CollectRegionalModeAssignInfo(std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapRegionalModeAssign, std::vector<KInterPathEditInfo> &a_vecInterAssignInfo)
{
	try
	{
		AutoType iter  = a_mapRegionalModeAssign.begin();
		AutoType itEnd = a_mapRegionalModeAssign.end();

		while (iter != itEnd)
		{
			std::vector<KInterPathEditInfo> &vecAssignInfo = iter->second;
			for (size_t m = 0; m < vecAssignInfo.size(); m++)
			{
				KInterPathEditInfo &oTransitAssignInfo = vecAssignInfo[m];
				a_vecInterAssignInfo.push_back(oTransitAssignInfo);
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::ReadAutoTaxiPathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapAutoTaxiAssign)
{
	TxLogDebugStartMsg();
	TxLogDebugStartMsg();
	a_mapAutoTaxiAssign.clear();
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

				if (oAutoAssignInfo.oTAutoSubData.dTrip <= m_dMinPathVol)
				{
					continue;
				}
				if (a_setSelectedZoneDest.find(oAutoAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
				{
					continue;
				}
#pragma region 취합 정보 등록
				AutoType iter  = a_mapAutoTaxiAssign.find(oAutoAssignInfo.nxDestinationID);
				AutoType itEnd = a_mapAutoTaxiAssign.end();

				if( iter != itEnd )
				{
					std::vector<KInterPathEditInfo> &vecAutoAssignInfo = iter->second;
					vecAutoAssignInfo.push_back(oAutoAssignInfo);
				}
				else
				{
					std::vector<KInterPathEditInfo> vecAutoAssignInfo;
					vecAutoAssignInfo.push_back(oAutoAssignInfo);
					a_mapAutoTaxiAssign.insert(std::make_pair(oAutoAssignInfo.nxDestinationID, vecAutoAssignInfo));
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
		a_mapAutoTaxiAssign.clear();
		ifs.close();

		m_bResultRun = false;
		m_strRunErrMsg =_T("ReadAutoTaxiPathFile 오류");

		/*ThrowException(_T("ReadAutoTaxiPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaInterDlg::ReadBusPathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapBusAssign)
{
	TxLogDebugStartMsg();
	a_mapBusAssign.clear();
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

					if (oBusAssignInfo.oTBusSubData.dTrip <= m_dMinPathVol)
					{
						continue;
					}

					if (a_setSelectedZoneDest.find(oBusAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
					{
						continue;
					}

#pragma region 취합 정보 등록
					AutoType iter  = a_mapBusAssign.find(oBusAssignInfo.nxDestinationID);
					AutoType itEnd = a_mapBusAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusAssignInfo = iter->second;
						vecBusAssignInfo.push_back(oBusAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusAssignInfo;
						vecBusAssignInfo.push_back(oBusAssignInfo);
						a_mapBusAssign.insert(std::make_pair(oBusAssignInfo.nxDestinationID, vecBusAssignInfo));
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
		a_mapBusAssign.clear();
		ifs.close();

		m_bResultRun = false;
		m_strRunErrMsg =_T("ReadBusPathFile 오류");

		/*ThrowException(_T("ReadBusPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaInterDlg::ReadRailPathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapRailAssign)
{
	TxLogDebugStartMsg();
	a_mapRailAssign.clear();
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
					if (oRailAssignInfo.oTRailSubData.dTrip <= m_dMinPathVol)
					{
						continue;
					}
					if (a_setSelectedZoneDest.find(oRailAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
					{
						continue;
					}
#pragma region 취합 정보 등록
					AutoType iter  = a_mapRailAssign.find(oRailAssignInfo.nxDestinationID);
					AutoType itEnd = a_mapRailAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecRailAssignInfo = iter->second;
						vecRailAssignInfo.push_back(oRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecRailAssignInfo;
						vecRailAssignInfo.push_back(oRailAssignInfo);
						a_mapRailAssign.insert(std::make_pair(oRailAssignInfo.nxDestinationID, vecRailAssignInfo));
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
		a_mapRailAssign.clear();
		ifs.close();
		m_bResultRun = false;
		m_strRunErrMsg =_T("ReadRailPathFile 오류");
		/*ThrowException(_T("ReadRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaInterDlg::ReadBusRailPathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapBusRailAssign)
{
	TxLogDebugStartMsg();

	a_mapBusRailAssign.clear();
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

					if (oBusRailAssignInfo.oTBusSubData.dTrip <= m_dMinPathVol)
					{
						continue;
					}
					if (a_setSelectedZoneDest.find(oBusRailAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
					{
						continue;
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
					AutoType iter  = a_mapBusRailAssign.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = a_mapBusRailAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
						a_mapBusRailAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
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
		a_mapBusRailAssign.clear();
		ifs.close();
		m_bResultRun = false;
		m_strRunErrMsg =_T("ReadBusRailPathFile 오류");
		/*ThrowException(_T("ReadBusRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaInterDlg::ReadParkRidePathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapParkRideAssign)
{
	TxLogDebugStartMsg();

	a_mapParkRideAssign.clear();
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

					if (oBusRailAssignInfo.oTMultiSubData.dTrip <= m_dMinPathVol)
					{
						continue;
					}

					if (a_setSelectedZoneDest.find(oBusRailAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
					{
						continue;
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
					AutoType iter  = a_mapParkRideAssign.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = a_mapParkRideAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
						a_mapParkRideAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
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
		a_mapParkRideAssign.clear();
		ifs.close();
		m_bResultRun = false;
		m_strRunErrMsg =_T("ReadParkRidePathFile 오류");
		/*ThrowException(_T("ReadParkRidePathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaInterDlg::ReadKissRidePathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapKissRideAssign)
{
	TxLogDebugStartMsg();

	a_mapKissRideAssign.clear();
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

					if (oBusRailAssignInfo.oTMultiSubData.dTrip <= m_dMinPathVol) 
					{
						continue;
					}
					if (a_setSelectedZoneDest.find(oBusRailAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
					{
						continue;
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
					AutoType iter  = a_mapKissRideAssign.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = a_mapKissRideAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
						a_mapKissRideAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
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
		a_mapKissRideAssign.clear();
		ifs.close();
		m_bResultRun = false;
		m_strRunErrMsg =_T("ReadKissRidePathFile 오류");
		/*ThrowException(_T("ReadKissRidePathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaInterDlg::ReadTaxiTransitPathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KInterPathEditInfo>> &a_mapTaxiTransitAssign)
{
	TxLogDebugStartMsg();

	a_mapTaxiTransitAssign.clear();
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
					
					if (oBusRailAssignInfo.oTMultiSubData.dTrip <= m_dMinPathVol)
					{
						continue;
					}
					if (a_setSelectedZoneDest.find(oBusRailAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
					{
						continue;
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
					AutoType iter  = a_mapTaxiTransitAssign.find(oBusRailAssignInfo.nxDestinationID);
					AutoType itEnd = a_mapTaxiTransitAssign.end();

					if( iter != itEnd )
					{
						std::vector<KInterPathEditInfo> &vecBusRailAssignInfo = iter->second;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
					}
					else
					{
						std::vector<KInterPathEditInfo> vecBusRailAssignInfo;
						vecBusRailAssignInfo.push_back(oBusRailAssignInfo);
						a_mapTaxiTransitAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusRailAssignInfo));
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
		a_mapTaxiTransitAssign.clear();
		ifs.close();
		m_bResultRun = false;
		m_strRunErrMsg =_T("ReadTaxiTransitPathFile 오류");
		/*ThrowException(_T("ReadTaxiTransitPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaInterDlg::ReadRegionalPathFile(CString a_strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer, std::vector<KRegionModePathInfo>> &a_mapRegionAssign)
{
	TxLogDebugStartMsg();
	a_mapRegionAssign.clear();
	std::ifstream ifs( a_strFileName, std::ios::binary );    
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

							if (oRegionOnePathDetail.dTrip <= m_dMinPathVol) 
							{
								continue;
							}

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

							if (oRegionOnePathDetail.dTrip <= m_dMinPathVol) 
							{
								continue;
							}

							vecOnePathDetail.push_back(oRegionOnePathDetail);
						}
					}
				}

				if (a_setSelectedZoneDest.find(nxDZoneID) != a_setSelectedZoneDest.end()) // 선택한 도착지점만..
				{
					AutoType iterB  = a_mapRegionAssign.find(nxDZoneID);
					AutoType itEndB = a_mapRegionAssign.end();

					if( iterB == itEndB )
					{
						a_mapRegionAssign.insert(std::make_pair(nxDZoneID, vecRegionModePathInfo));
					}
				}

				nCount++;
			}

// 			CString strResult = _T("");
// 			strResult.Format(_T("--- end insert result size : %d"), nCount);
// 			TxLogDebug( (LPCTSTR)strResult );
		}

		ifs.close();
	}
	catch(KExceptionPtr ex)
	{
		a_mapRegionAssign.clear();
		ifs.close();

		m_bResultRun = false;
		m_strRunErrMsg = ex->GetErrorMessage();
		TxLogDebugException();
	}
	catch (...)
	{
		a_mapRegionAssign.clear();
		ifs.close();

		m_bResultRun = false;
		m_strRunErrMsg =_T("ReadRegionalPathFile 오류");
		TxLogDebugException();
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaInterDlg::ReadRegAutoTaxiPathFile(std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
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


void KPathVolumeAreaInterDlg::ReadRegBusPathFile(std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
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


void KPathVolumeAreaInterDlg::ReadRegRailPathFile(std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
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


void KPathVolumeAreaInterDlg::ReadRegBusRailPathFile(std::ifstream &ifs, int &a_nPathCnt, std::map<Integer, std::vector<KInterPathEditInfo>> &a_mapTransitAssign, KEMInterAccEgrType a_emInterAccEgrType)
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


void KPathVolumeAreaInterDlg::InitVolMinMax()
{
	m_dMaxVolume  = 0.0;
	m_dMinVolume  = 0.0;
}


void KPathVolumeAreaInterDlg::SetVolMinMax(TInMapResultInputValue &a_oInMapResultTripInfo)
{
	try
	{
		if (a_oInMapResultTripInfo.mapAutoTrip.size() < 1 && 
			a_oInMapResultTripInfo.mapRegionalModeMainTrip.size() < 1 &&
			a_oInMapResultTripInfo.mapBusLinkTrip.size() < 1 && 
			a_oInMapResultTripInfo.mapSubwayLinkTrip.size() < 1)
		{
			InitVolMinMax();
			return;
		}

		double dMaxVolume = -999999999999.0;
		double dMinVolume = 999999999999.0;

		//Bus
		CollectMinMaxLinkTrip(a_oInMapResultTripInfo.mapBusLinkTrip, dMaxVolume, dMinVolume);
		//Rail
		CollectMinMaxLinkTrip(a_oInMapResultTripInfo.mapSubwayLinkTrip, dMaxVolume, dMinVolume);

		std::map<KODKey, double> mapODVolume;
		CollectUserODVolume(a_oInMapResultTripInfo.mapTransferWalkingB2BTrip, mapODVolume); // 버스에서 버스로 환승 도보
		CollectUserODVolume(a_oInMapResultTripInfo.mapTransferWalkingB2STrip, mapODVolume); // 버스에서 전철로 환승 도보
		CollectUserODVolume(a_oInMapResultTripInfo.mapTransferWalkingS2STrip, mapODVolume); // 전철에서 전철로
		CollectUserODVolume(a_oInMapResultTripInfo.mapTransferWalkingA2TTrip, mapODVolume); // Auto 에서 Transit : kiss&ride 등...
		// a_oInMapResultTripInfo.mapWalkingTrip 제외
		CollectUserODVolume(a_oInMapResultTripInfo.mapTerminalConnetWalkingTrip, mapODVolume); // access or egress 에서 터미널
		CollectUserODVolume(a_oInMapResultTripInfo.mapAutoTrip, mapODVolume); // Auto Trip

		AutoType iterA  = a_oInMapResultTripInfo.mapRegionalModeMainTrip.begin();
		AutoType itEndA = a_oInMapResultTripInfo.mapRegionalModeMainTrip.end();
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
	catch (...)
	{
		InitVolMinMax();
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::CollectMinMaxLinkTrip(std::map<Integer, double> &a_mapLinkTrip, double &a_dMaxVolume, double &a_dMinVolume)
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


void KPathVolumeAreaInterDlg::CollectUserODVolume(std::map<KODKey, double> &a_mapUserODTrip, std::map<KODKey, double> &a_mapODSumVolume)
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


void KPathVolumeAreaInterDlg::CollectMinMaxUserODVolume(std::map<KODKey, double> &a_mapODSumVolume, double &a_dMaxVolume, double &a_dMinVolume)
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


void KPathVolumeAreaInterDlg::UpdateMinMax()
{
	CString strMinVol(_T(""));
	CString strMaxVol(_T(""));

	double dMinVol(0.0);
	dMinVol = m_dMinVolume - 0.5;
	if(dMinVol < 0.0)
		dMinVol = 0.0;

	if (dMinVol < m_dMinVolume)
	{
		dMinVol = m_dMinVolume;
	}

	int nMinVol = (int)dMinVol;
	dMinVol     = (double)nMinVol; // double 버림 처리
	if (dMinVol < m_dMinVolume)
	{
		dMinVol = m_dMinVolume;
		strMinVol.Format(_T("%f"), dMinVol);
	}
	else
	{
		strMinVol.Format(_T("%.0f"), dMinVol);
	}

	double dMaxVol(0.0);
	if (0.0 == m_dMaxVolume)
	{
		dMaxVol = m_dMaxVolume;
	}
	else
	{
		dMaxVol = m_dMaxVolume + 0.6;
	}

	strMaxVol.Format(_T("%.0f"), dMaxVol);

	/*SetDlgItemText(IDC_EDIT_MIN_VALUE,  strMinVol);*/
	SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T("0.0"));
	SetDlgItemText(IDC_EDIT_MAX_VALUE,  strMaxVol);
}


void KPathVolumeAreaInterDlg::OnBnClickedButtonDraw()
{
	try
	{
		if (m_oInMapResultTripInfo.mapAutoTrip.size() < 1 && 
			m_oInMapResultTripInfo.mapRegionalModeMainTrip.size() < 1 &&
			m_oInMapResultTripInfo.mapBusLinkTrip.size() < 1 && 
			m_oInMapResultTripInfo.mapSubwayLinkTrip.size() < 1)
		{
			AfxMessageBox(_T("경로 분석 결과가 존재하지 않습니다."));
			return;
		}

		if (UserVolumeInputCheck() == false)
		{
			return;
		}

		CWaitCursor cw;
		GetResultSettingInfo(m_oTInResultSettingInfo);

		ClearMapDraw(false);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(MakeDrawDataThreadCaller, this, false, nLang);

		bool bLabel(false);
		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bLabel = true;
		else
			bLabel = false;

		if (IsDlgButtonChecked(IDC_CHECK_STATION) == BST_CHECKED)
		{
			GetSelectedDrawNodeInfo(m_oMapResult.vecDrawNodeInfo, false); // 추가: 기점, 종점
			m_pMapView->TxDrawCompositeNode(m_oMapResult.vecDrawNodeInfo, false,  false);
		}
		else
		{
			DrawSelectedOrgDestination(false); // 기점, 종점만
		}

		//존->정류장 도보제거 요청 : 2013.03.03-명지대학교
		m_pMapView->CacheDrawCompositeUserLine(m_oMapResult.vecUserLineInfo, bLabel, false);

		if (IsDlgButtonChecked(IDC_CHECK_ZONEACCESS) == BST_CHECKED)
		{
			std::vector<TDrawZoneAccess> vecZoneAccess;
			std::vector<TDrawNodeInfo>&  vecDrawZoneNodeInfo = m_oMapResult.vecDrawZoneNodeInfo;
			size_t nxCount = vecDrawZoneNodeInfo.size();

			for (size_t i= 0; i< nxCount; i++)
			{
				TDrawNodeInfo&  oDrawNodeInfo   = vecDrawZoneNodeInfo[i];
				TDrawZoneAccess oDrawZoneAccess;
				oDrawZoneAccess.TNodeID         = oDrawNodeInfo.TNodeID;
				oDrawZoneAccess.TSymbol = oDrawNodeInfo.TSymbol;
				oDrawZoneAccess.TColorRGB       = oDrawNodeInfo.TColorRGB;
				oDrawZoneAccess.TLabel          = oDrawNodeInfo.TLabel;
				oDrawZoneAccess.TSize           = oDrawNodeInfo.TSize;
				oDrawZoneAccess.TRadius         = m_dRadius;

				vecZoneAccess.push_back(oDrawZoneAccess);
			}
			m_pMapView->CacheDrawCompositeZoneAccess(vecZoneAccess, false);
		}

		m_pMapView->CacheDrawCompositeLink(m_oMapResult.vecDrawLinkInfo, bLabel,  true);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KPathVolumeAreaInterDlg::UserVolumeInputCheck()
{
	try
	{
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize(0.0),  dMaxSize(0.0);

		CString strValue(_T(""));

		int nMinID;
		int nMaxID;

		nMinID = IDC_EDIT_MIN_VALUE;
		nMaxID = IDC_EDIT_MAX_VALUE;

		//min value 체크
		GetDlgItemText(nMinID, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(nMinID)->SetFocus();		
			return false;
		}
		dMinValue = _ttof(strValue);

		//IDC_EDIT_MAX_VALUE
		GetDlgItemText(nMaxID, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(nMaxID)->SetFocus();		
			return false;
		}
		dMaxValue = _ttof(strValue);

		//IDC_EDIT_MINSIZE
		GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
			return false;
		}
		dMinSize = _ttof(strValue);

		//IDC_EDIT_MAXSIZE
		GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MAXSIZE)->SetFocus();		
			return false;
		}
		dMaxSize = _ttof(strValue);

		if (dMinValue > dMaxValue)
		{
			AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
			GetDlgItem(nMinID)->SetFocus();		
			return false;
		}

		if (dMinSize > dMaxSize)
		{
			AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
			return false;
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
	return true;
}


void KPathVolumeAreaInterDlg::GetResultSettingInfo(TInResultSettingInfo &a_oTInResultSettingInfo)
{
	try
	{
		int nMinID = IDC_EDIT_MIN_VALUE;
		int nMaxID = IDC_EDIT_MAX_VALUE;

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
		a_oTInResultSettingInfo.clrOrgZone  = m_clrOriginZone;
		a_oTInResultSettingInfo.clrDestZone = m_clrDestinationZone;

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


unsigned __stdcall KPathVolumeAreaInterDlg::MakeDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KPathVolumeAreaInterDlg*     pDlg = (KPathVolumeAreaInterDlg*)pParameter->GetParameter();
	pDlg->MakeDrawData();

	return 0;
}


void KPathVolumeAreaInterDlg::MakeDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		KInterModalPathInfo::ResultTransitPathDrawInfo(m_pTarget, m_oInMapResultTripInfo, m_oTInResultSettingInfo, m_oMapResult);
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


void KPathVolumeAreaInterDlg::ClearMapDraw(bool bRefresh)
{
	try
	{
		m_pMapView->CacheClearCompositeThemes(false);
		m_pMapView->ClearScreenDrawEdit(false);

        if (bRefresh)
            m_pMapView->MapRefresh();
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


void KPathVolumeAreaInterDlg::OnBnClickedButtonClear()
{
	ClearMapDraw(false);
	DrawSelectedOrgDestination(true);
}


void KPathVolumeAreaInterDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KPathVolumeAreaInterDlg::CloseWindowProc()
{
	ClearMapDraw(true);
}


void KPathVolumeAreaInterDlg::OnBnClickedCheckLabel()
{
	try
	{
		bool bLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			bLabel = true;
		}
		else
		{
			bLabel = false;
		}

		//m_pMapView->CacheShowCompositeNodeLabel    (bLabel, false);
		m_pMapView->CacheShowCompositeUserLineLabel(bLabel, false);
		m_pMapView->CacheShowCompositeLinkLabel    (bLabel, true);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeAreaInterDlg::DrawSelectedOrgDestination(bool bRefresh)
{
	try
	{
		std::vector<TDrawNodeInfo> vecSelectedDrawNodeInfo;

		GetSelectedDrawNodeInfo(vecSelectedDrawNodeInfo);

// 		bool bLabel(false);
// 		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
// 			bLabel = true;
// 		else
// 			bLabel = false;

		m_pMapView->TxDrawCompositeNode(vecSelectedDrawNodeInfo, false, bRefresh);

	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterDlg::GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear)
{
	try
	{
		if (a_bClear)
		{
			a_vecSelectedDrawNodeInfo.clear();
		}

		CXTPReportRecords*    pRecords(nullptr);
		CXTPReportRecord*     pRecord (nullptr);
		CXTPReportRecordItem* pItem   (nullptr);
		int                   nRecordCnt(0);
		Integer               nxZoneID(0);

		pRecords   = m_ctrlReportA.GetRecords();
		nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);

			if (pItem->IsChecked() == FALSE)
				continue;

			nxZoneID = (Integer)pItem->GetItemData();

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID         = nxZoneID;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxZoneID);
			oNodeInfo.TLabel          = strNodeLable;

			oNodeInfo.TSymbol = nZoneCharIndex;
			oNodeInfo.TSize           = nZoneSize;
			oNodeInfo.TColorRGB       = RGB(255,  66,  0);

			a_vecSelectedDrawNodeInfo.push_back(oNodeInfo);
		}

		pRecords   = m_ctrlReportB.GetRecords();
		nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_B_COLUMN_SELECT);

			if (pItem->IsChecked() == FALSE)
				continue;

			nxZoneID = (Integer)pItem->GetItemData();

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID         = nxZoneID;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxZoneID);
			oNodeInfo.TLabel          = strNodeLable;

			oNodeInfo.TSymbol = nZoneCharIndex;
			oNodeInfo.TSize           = nZoneSize;
			oNodeInfo.TColorRGB       = RGB(0,  128,  192);

			a_vecSelectedDrawNodeInfo.push_back(oNodeInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KPathVolumeAreaInterDlg::OnBnClickedCheckZone()
{
	try
	{
// 		ClearMapDraw();
// 		if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
// 		{
// 			DrawSelectedOrgDestination();
// 		}
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


void KPathVolumeAreaInterDlg::OnBnClickedCheckOutrange()
{
	UpdateOutRange();
}


void KPathVolumeAreaInterDlg::UpdateOutRange()
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


void KPathVolumeAreaInterDlg::OnReportColorAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KPathVolumeAreaInterDlg::OnReportColorBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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