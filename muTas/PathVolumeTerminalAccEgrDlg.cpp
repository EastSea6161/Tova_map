// PathVolumeTerminalAccEgrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PathVolumeTerminalAccEgrDlg.h"
#include "afxdialogex.h"
#include "Scenario.h"
#include "Target.h"
#include "MapView.h"
#include "DBaseTerminalAEModeViewer.h"
#include "KExRecordItem.h"
#include "MultiModalResultInfo.h"
#include "KDBaseAccessibilityModify.h"

namespace PathVolumeTerminalENUM
{
#define UM_TERMINAL_SEARCH_COMPLETE		WM_USER + 1001

	enum
	{
		_0_COLUMN_SELECT     = 0,
		_1_COLUMN_NAME       = 1
	};

	enum
	{
		_0_B_COLUMN_ZONEID        = 0,
		_1_B_COLUMN_TRIP          = 1
	};
	enum 
	{
		B_COLUMN_TOTAL_COUNT     = 2
	};

	enum
	{
		EXPANDALL_LIMIT_COUNT    = 20 
	};
}

using namespace PathVolumeTerminalENUM;
// KPathVolumeTerminalAccEgrDlg 대화 상자입니다.

const int nZoneCharIndex = 53;
const int nZoneSize		 = 11;

IMPLEMENT_DYNAMIC(KPathVolumeTerminalAccEgrDlg, KResizeDialogEx)

KPathVolumeTerminalAccEgrDlg::KPathVolumeTerminalAccEgrDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPathVolumeTerminalAccEgrDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_strAccSearchedIDs(_T(""))
	, m_strEgrSearchedIDs(_T(""))
	, m_bFlagAccess(false)
	, m_dMaxVolume(0.0)
	, m_dMinVolume(0.0)
	, m_strErrMsg(_T(""))
	, m_bResult(false)
	, m_dRadius(1000.0)
{
	GetModeInfo();
	m_oTerminalNodeIdCaption.TID = _T("-1");

	m_clrOutRange = RGB ( 221, 217, 195 );
	m_clrOrgZone  = RGB ( 255,   0,   0 );
	m_clrDestZone = RGB ( 0,     0,  205);
}

KPathVolumeTerminalAccEgrDlg::~KPathVolumeTerminalAccEgrDlg()
{
	try
	{
		CloseWindowProc();

		KIOTables* pIOTables = m_pTarget->Tables();	
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);

		pIOTable->Unregister(this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KPathVolumeTerminalAccEgrDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT_A,                 m_ctrlReportAcc);
	DDX_Control(pDX, IDC_REPORT_C,                 m_ctrlReportEgr);
	DDX_Control(pDX, IDC_REPORT_B,                 m_ctrlReportPath);
	DDX_Control(pDX, IDC_REPORT_D,                 m_ctrlReportColor);
	
	DDX_Control(pDX, IDC_CLR_OUT,                  m_cpOutRangeColor);
}


BEGIN_MESSAGE_MAP(KPathVolumeTerminalAccEgrDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KPathVolumeTerminalAccEgrDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON4, &KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonTerminalNodeSearch)
	ON_BN_CLICKED(IDC_BUTTON6, &KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonAccessZoneSearch)
	ON_BN_CLICKED(IDC_BUTTON7, &KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonEgressZoneSearch)
	ON_MESSAGE(UM_TERMINAL_SEARCH_COMPLETE, &KPathVolumeTerminalAccEgrDlg::OnTerminalSearchCompleted)

	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_A, &KPathVolumeTerminalAccEgrDlg::OnReportAccCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_C, &KPathVolumeTerminalAccEgrDlg::OnReportEgrCheckItem)
	ON_BN_CLICKED(IDC_CHECK_ALL,                           &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckAllAcc)
	ON_BN_CLICKED(IDC_CHECK_ALL3,                          &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckAllEgr)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT_B, &KPathVolumeTerminalAccEgrDlg::OnReportPathCheckItem)

	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,					   &KPathVolumeTerminalAccEgrDlg::OnReportAccItemClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_C,					   &KPathVolumeTerminalAccEgrDlg::OnReportEgrItemClick)

	ON_BN_CLICKED(IDC_CHECK_OUTRANGE,                      &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckOutrange)
	ON_BN_CLICKED(IDC_CHECK_LABEL,                         &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckLabel)

	ON_BN_CLICKED(IDC_CHECK_A, &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckA)
	ON_BN_CLICKED(IDC_CHECK_T, &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckT)
	ON_BN_CLICKED(IDC_CHECK_B, &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckB)
	ON_BN_CLICKED(IDC_CHECK_R, &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckR)
	ON_BN_CLICKED(IDC_CHECK_TR, &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckTR)
	ON_BN_CLICKED(IDC_BUTTON2, &KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonExport)

	ON_NOTIFY(NM_CLICK,                      IDC_REPORT_D, &KPathVolumeTerminalAccEgrDlg::OnReportColorItemClick)
	ON_BN_CLICKED(IDC_BUTTON1, &KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON5, &KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_CHECK_ZONEACCESS, &KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckZoneaccess)
END_MESSAGE_MAP()


// KPathVolumeTerminalAccEgrDlg 메시지 처리기입니다.

void KPathVolumeTerminalAccEgrDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);
		if(strSubjectName.CompareNoCase(TABLE_NODE) == 0)
		{
			m_oTerminalNodeIdCaption.TID = _T("-1");
			SetDlgItemText(IDC_EDIT1,  _T(""));

			InitializeTerminalSearchResultData(true);

			UpdateSearchDataCount(IDC_STATIC6, m_mapAccessNodeData, m_setAccSearchedID);
			UpdateSearchDataCount(IDC_STATIC7, m_mapEgressNodeData, m_setEgrSearchedID);

			CollectData();
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


void KPathVolumeTerminalAccEgrDlg::ActivatedFrameWndNotifyProcess()
{
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
}


void KPathVolumeTerminalAccEgrDlg::DeActivatedFrameWndNotifyProcess()
{
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
}


void KPathVolumeTerminalAccEgrDlg::GetModeInfo()
{
	try
	{
        if (KmzSystem::GetLanguage() == KEMKorea) {
            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeAuto, _T("승용차")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeAuto,   TInterPathColor(_T("승용차"),    RGB ( 228, 108,  10))));

            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeTaxi, _T("택시")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeTaxi,   TInterPathColor(_T("택시"),    RGB ( 255, 255, 100))));

            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeBus, _T("버스")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeBus,   TInterPathColor(_T("버스"),    RGB ( 0, 128,   0))));

            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeRail, _T("도시철도")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeRail,   TInterPathColor(_T("도시철도"),    RGB ( 0,   0, 255))));

            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeTransit, _T("버스+도시철도")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeTransit,   TInterPathColor(_T("버스+도시철도"),    RGB ( 102, 102, 153))));
        }
        else {
            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeAuto, _T("PassengerCar")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeAuto,   TInterPathColor(_T("PassengerCar"),    RGB ( 228, 108,  10))));

            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeTaxi, _T("Taxi")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeTaxi,   TInterPathColor(_T("Taxi"),    RGB ( 255, 255, 100))));

            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeBus, _T("Bus")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeBus,   TInterPathColor(_T("Bus"),    RGB ( 0, 128,   0))));

            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeRail, _T("Rail")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeRail,   TInterPathColor(_T("Rail"),    RGB ( 0,   0, 255))));

            m_mapModeCodeName.insert(std::make_pair(KEMInterPathTerminalModeTransit, _T("Bus+Rail")));
            m_mapModeColor.insert(std::make_pair(KEMInterPathTerminalModeTransit,   TInterPathColor(_T("Bus+Rail"),    RGB ( 102, 102, 153))));
        }		
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KPathVolumeTerminalAccEgrDlg::GetNodeInfo(void)
{
	TxLogDebugStartMsg();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format (_T(" SELECT zone_id, name, full_name FROM %s ORDER BY zone_id "), TABLE_ZONE);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(pResult == NULL)
		{
			return false;
		}

		m_mapZoneName.clear();

		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));
		CString strNodeFullName(_T(""));

		while( pResult->Next() )
		{
			nxNodeId	    = pResult->GetValueInt64 (0);
			strNodeName     = pResult->GetValueString(1);
			strNodeFullName	= pResult->GetValueString(2);

			m_mapZoneName.insert(std::make_pair(nxNodeId, strNodeName));
		}
	}
	catch (...)
	{
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


void KPathVolumeTerminalAccEgrDlg::ResizeComponent()
{
	try
	{
		SetResize(IDC_EDIT1,          			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON4,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
		SetResize(IDC_SEPRATOR5,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);

		SetResize(IDC_EDIT2,                		  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
		SetResize(IDC_BUTTON6,       			      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
		SetResize(IDC_STATIC6,       			      SZ_TOP_LEFT,  SZ_TOP_RIGHT);

		SetResize(IDC_REPORT_A,       			      SZ_TOP_LEFT,  CXTPResizePoint(1.0, 0.2));
		
		SetResize(IDC_STATIC13,       			      CXTPResizePoint(0.0, 0.2),  CXTPResizePoint(0.0, 0.2));
		SetResize(IDC_EDIT3,                		  CXTPResizePoint(1.0, 0.2),  CXTPResizePoint(1.0, 0.2));
		SetResize(IDC_BUTTON7,       			      CXTPResizePoint(1.0, 0.2),  CXTPResizePoint(1.0, 0.2));
		SetResize(IDC_CHECK_ALL3,     			      CXTPResizePoint(0.0, 0.2),  CXTPResizePoint(0.0, 0.2));
		SetResize(IDC_STATIC7,       			      CXTPResizePoint(0.0, 0.2),  CXTPResizePoint(1.0, 0.2));

		SetResize(IDC_REPORT_C,       			      CXTPResizePoint(0.0, 0.2),  CXTPResizePoint(1.0, 0.4));
		SetResize(IDC_SEPRATOR8,       			      CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(1.0, 0.4));
		
		SetResize(IDC_CHECK_A,       			      CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(0.0, 0.4));
		SetResize(IDC_CHECK_T,       			      CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(0.0, 0.4));
		SetResize(IDC_CHECK_B,       			      CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(0.0, 0.4));
		SetResize(IDC_CHECK_R,       			      CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(0.0, 0.4));
		SetResize(IDC_CHECK_TR,       			      CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(0.0, 0.4));

		SetResize(IDC_REPORT_B,       			      CXTPResizePoint(0.0, 0.4),  SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR4,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC18,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC19,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC20,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC21,       			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_MINTRIP,       		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_MAXTRIP,       		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_MINSIZE,       		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_MAXSIZE,       		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_OUTRANGE,       		  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_CLR_OUT,       	      	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_ZONEACCESS,       		  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

		SetResize(IDC_REPORT_D,       	      	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

		SetResize(IDC_SEPRATOR7,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
		SetResize(IDC_BUTTON5,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_BUTTON1,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_LABEL,       		      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);

		SetResize(IDC_BUTTON2,       			      SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KPathVolumeTerminalAccEgrDlg::OnInitDialog()
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

	CollectData();

	KReportCtrlSetting::Default(m_ctrlReportAcc, FALSE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_ctrlReportEgr, FALSE, FALSE, FALSE);

	KReportCtrlSetting::DefaultC(m_ctrlReportPath, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_ctrlReportPath);

	KReportCtrlSetting::Default(m_ctrlReportColor, TRUE, FALSE, FALSE);
	KReportCtrlSetting::HideHeader(m_ctrlReportColor);

	KColorPickerSetting::DefaultCtrl(m_cpOutRangeColor, m_clrOutRange);

	InitReportHeaderAcc();
	InitReportHeaderEgr();
	InitReportHeaderPath();
	InitReportHeaderColor();
	UpdateReportColorData();

	m_ctrlReportAcc.Populate();
	m_ctrlReportEgr.Populate();
	m_ctrlReportPath.Populate();
	m_ctrlReportColor.Populate();

	CheckDlgButton(IDC_CHECK_ZONEACCESS, BST_CHECKED);

	CheckDlgButton(IDC_CHECK_A, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_T, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_B, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_R, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_TR, BST_UNCHECKED);

	SetDlgItemText(IDC_EDIT_MINSIZE, _T("2.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE, _T("10.0"));
	CheckDlgButton(IDC_CHECK_LABEL, BST_UNCHECKED);
	UpdateOutRange();

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


void KPathVolumeTerminalAccEgrDlg::InitReportHeaderAcc()
{
	try
	{
		CXTPReportColumn*  pColumn   = nullptr;

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT,       _T("Select"),      40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(_1_COLUMN_NAME,         _T("From Zone"), 120));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("출발존"));
        }

		AutoType iter  = m_mapModeCodeName.begin();
		AutoType itEnd = m_mapModeCodeName.end();
		int nIndex = _1_COLUMN_NAME +1;
		while (iter != itEnd)
		{
			CString &strModeName = iter->second;

			pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(nIndex++,        strModeName, 45));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetEditable(FALSE);

			++iter;
		}

		m_ctrlReportAcc.Populate();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KPathVolumeTerminalAccEgrDlg::InitReportHeaderEgr()
{
	try
	{
		CXTPReportColumn*  pColumn   = nullptr;

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT,       _T("Select"),      40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(_1_COLUMN_NAME,         _T("To Zone"), 120));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("도착존"));
        }

		AutoType iter  = m_mapModeCodeName.begin();
		AutoType itEnd = m_mapModeCodeName.end();
		int nIndex = _1_COLUMN_NAME +1;
		while (iter != itEnd)
		{
			CString &strModeName = iter->second;

			pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(nIndex++,        strModeName, 45));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetEditable(FALSE);

			++iter;
		}

		m_ctrlReportEgr.Populate();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KPathVolumeTerminalAccEgrDlg::InitReportHeaderPath()
{
	try
	{
		CXTPReportColumn*  pColumn   = nullptr;

		m_ctrlReportPath.SetTreeIndent(15);

		pColumn = m_ctrlReportPath.AddColumn(new CXTPReportColumn(_0_B_COLUMN_ZONEID,       _T("Terminal-Zone Path"),      70));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
		pColumn->SetTreeColumn(TRUE);

        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("접근 수단"));
        }

		pColumn = m_ctrlReportPath.AddColumn(new CXTPReportColumn(_1_B_COLUMN_TRIP,         _T("Trip"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행량(인)"));
        }
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KPathVolumeTerminalAccEgrDlg::InitReportHeaderColor()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReportColor.AddColumn(new CXTPReportColumn(0,		_T("mode"),			100));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);

		pColumn = m_ctrlReportColor.AddColumn(new CXTPReportColumn(1,		_T("Color"),		50,		FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);

		m_ctrlReportColor.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::UpdateReportColorData()
{
	try
	{
		m_ctrlReportColor.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = m_mapModeColor.begin();
		AutoType itEnd = m_mapModeColor.end();

		while (iter != itEnd)
		{
			TInterPathColor &oTModeColor = iter->second;

			pRecord = m_ctrlReportColor.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTModeColor.strName));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetItemData((DWORD_PTR) &oTModeColor);

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetBackgroundColor(oTModeColor.clrPath);
			pItem->SetEditable(TRUE);

			++iter;
		}

		m_ctrlReportColor.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::CollectData()
{
	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(CollectDataThreadCaller, this, false, nLang);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


unsigned __stdcall KPathVolumeTerminalAccEgrDlg::CollectDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("터미널 접근 통행량 데이터 로딩 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Terminal Access/Egress Data..."));
	}

	KPathVolumeTerminalAccEgrDlg* pDlg = (KPathVolumeTerminalAccEgrDlg*)pParameter->GetParameter();
	pDlg->ReadOutPutFile();

	return 0;
}


void KPathVolumeTerminalAccEgrDlg::ReadOutPutFile( void )
{
	TxLogDebugStartMsg();

	try
	{
		GetNodeInfo();
		KDBaseTerminalAEModeViewer::GetTerminalResultModeVolume(m_pTarget, m_mapTerminalResultInfo);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	m_mapTerminalNodeData.clear();
	m_vecTerminalNodeIDCaption.clear();

	try
	{
		CString strDataFileName(_T(""));
		CString strSavePath(_T(""));
        		
		strDataFileName = _T("Out_AccessZoneToTerminal.dat");
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME, strDataFileName);
		ReadTerminalZoneFile(strSavePath, m_mapAccZoneModeTrip);

		strDataFileName = _T("Out_EgressTerminalToZone.dat");
		strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME, strDataFileName);
		ReadTerminalZoneFile(strSavePath, m_mapEgrZoneModeTrip);
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

	try
	{
		CollectUsableTerminalInfo(m_mapAccZoneModeTrip, m_mapTerminalNodeData, m_vecTerminalNodeIDCaption); 
		CollectUsableTerminalInfo(m_mapEgrZoneModeTrip, m_mapTerminalNodeData, m_vecTerminalNodeIDCaption); 
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	TxLogDebugEndMsg();
}


void KPathVolumeTerminalAccEgrDlg::ReadTerminalZoneFile( CString a_strFileName, std::map<Integer, std::map<Integer, std::map<int, double>>> &a_mapTerminalZoneModeTrip )
{
	TxLogDebugStartMsg();
	a_mapTerminalZoneModeTrip.clear();
	std::ifstream ifs( a_strFileName, std::ios::binary );    
	try
	{
		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert result"));
			int nCount(0);

			while(! ifs.eof())
			{
				Integer nxZoneID(0);
				Integer nxTerminalID(0);

				ifs.read( reinterpret_cast<char*>( &(nxZoneID)),                       sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &(nxTerminalID)),                   sizeof(Integer) );

				if (ifs.eof())
				{
					break;
				}

				std::map<int, double> mapModeTrip;
				AutoType iter  = m_mapModeCodeName.begin();
				AutoType itEnd = m_mapModeCodeName.end();

				while (iter != itEnd)
				{
					int    nModeID = iter->first;
					double dTrip(0.0);

					ifs.read( reinterpret_cast<char*>( &(dTrip)),                       sizeof(double) );

					if (dTrip > 0.0)
					{
						mapModeTrip.insert(std::make_pair(nModeID, dTrip));
					}

					++iter;
				}

				AutoType itFindTerminal = a_mapTerminalZoneModeTrip.find(nxTerminalID);
				if (itFindTerminal != a_mapTerminalZoneModeTrip.end())
				{
					std::map<Integer, std::map<int, double>> &mapZoneModeTrip = itFindTerminal->second;
					mapZoneModeTrip.insert(std::make_pair(nxZoneID, mapModeTrip));
				}
				else
				{
					std::map<Integer, std::map<int, double>> mapZoneModeTrip;
					mapZoneModeTrip.insert(std::make_pair(nxZoneID, mapModeTrip));

					a_mapTerminalZoneModeTrip.insert(std::make_pair(nxTerminalID, mapZoneModeTrip));
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
		a_mapTerminalZoneModeTrip.clear();
		ifs.close();
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeTerminalAccEgrDlg::CollectUsableTerminalInfo(std::map<Integer, std::map<Integer, std::map<int, double>>> &a_mapZoneModeTrip,
														     KTDNodeDataMap &a_mapTerminalNodeData, std::vector<KIDCaption> &a_vecTerminalNodeIDCaption)
{
	try
	{
		AutoType iter  = a_mapZoneModeTrip.begin();
		AutoType itEnd = a_mapZoneModeTrip.end();

		CString strTerminalID(_T(""));

		while (iter != itEnd)
		{
			Integer nxTerminalID = iter->first;

			AutoType itFindTerminal = m_mapTerminalResultInfo.find(nxTerminalID);
			if (itFindTerminal == m_mapTerminalResultInfo.end())
			{
				++iter;
				continue;
			}

			if (a_mapTerminalNodeData.find(nxTerminalID) != a_mapTerminalNodeData.end())
			{
				++iter;
				continue;
			}

			TTerminalResultInfo &oTTerminalResultInfo = itFindTerminal->second;
			a_mapTerminalNodeData.insert(std::make_pair(nxTerminalID, oTTerminalResultInfo.strTerminalName));

			KIDCaption oIDCaption;
			strTerminalID.Format(_T("%I64d"), nxTerminalID);
			oIDCaption.TID      = strTerminalID;
			oIDCaption.TCaption = oTTerminalResultInfo.strTerminalName;

			a_vecTerminalNodeIDCaption.push_back(oIDCaption);

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


BOOL KPathVolumeTerminalAccEgrDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT1)->m_hWnd)
		{
			SelectTerminalNodeList();
			return TRUE;
		}
		else if (hwnd == GetDlgItem(IDC_EDIT2)->m_hWnd)
		{
			SelectAccEgrNodeList(true);
			return TRUE;
		}
		else if (hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd)
		{
			SelectAccEgrNodeList(false);
			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonTerminalNodeSearch()
{
	SelectTerminalNodeList();
}


void KPathVolumeTerminalAccEgrDlg::SelectTerminalNodeList()
{
	try
	{
		CWaitCursor cw;

		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		m_spImTasIDFind->SetIDCaption(m_vecTerminalNodeIDCaption);
		m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapTerminalNodeData, ImChampMapDefineSymbol::NodeSymbol);

// 		Integer nxNodeID = _ttoi64(m_oTerminalNodeIdCaption.TID);
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


void KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonAccessZoneSearch()
{
	SelectAccEgrNodeList(true);
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonEgressZoneSearch()
{
	SelectAccEgrNodeList(false);
}


void KPathVolumeTerminalAccEgrDlg::SelectAccEgrNodeList(bool a_bAccess)
{
	try
	{
		CWaitCursor cw;

		m_bFlagAccess = a_bAccess;

		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();
		// 항상 초기화해준다. vecIDCaption, vecID를 멤버로 갖고 있으면 좋음...
		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		if (a_bAccess)
		{
			m_spImTasMultiIDFind->SetIDCaption(m_vecAccessNodeIDCaption);
			m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapAccessNodeData, ImChampMapDefineSymbol::ZoneSymbol);
			m_spImTasMultiIDFind->SetSelectedID(m_strAccSearchedIDs);
		}
		else
		{
			m_spImTasMultiIDFind->SetIDCaption(m_vecEgressNodeIDCaption);
			m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapEgressNodeData, ImChampMapDefineSymbol::ZoneSymbol);
			m_spImTasMultiIDFind->SetSelectedID(m_strEgrSearchedIDs);
		}
		
		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KPathVolumeTerminalAccEgrDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampIDFind::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				Integer nxTerminalID = m_spImTasIDFind->GetSelectedID();
				CString strID(_T(""));
				strID.Format(_T("%I64d"), nxTerminalID);

				CString strTerminalView(_T(""));
				SetNodeViewData(m_mapTerminalNodeData, strID, strTerminalView, m_oTerminalNodeIdCaption);
				SetDlgItemText(IDC_EDIT1,  strTerminalView);

				PostMessage(UM_TERMINAL_SEARCH_COMPLETE, (WPARAM)&m_oTerminalNodeIdCaption);
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
				if (m_bFlagAccess)
				{
					m_setAccSearchedID.clear();
					m_setAccSearchedID  = m_spImTasMultiIDFind->GetSelectedIDSet();
					m_strAccSearchedIDs = m_spImTasMultiIDFind->GetSelectedID();
					SetDlgItemText(IDC_EDIT2, m_strAccSearchedIDs);
					CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);

					CheckDlgButton(IDC_CHECK_A, BST_CHECKED);
					CheckDlgButton(IDC_CHECK_T, BST_CHECKED);
					CheckDlgButton(IDC_CHECK_B, BST_CHECKED);
					CheckDlgButton(IDC_CHECK_R, BST_CHECKED);
					CheckDlgButton(IDC_CHECK_TR, BST_CHECKED);

					UpdateReportAccData();
					UpdateSearchDataCount(IDC_STATIC6, m_mapAccessNodeData, m_setAccSearchedID);
				}
				else
				{
					m_setEgrSearchedID.clear();
					m_setEgrSearchedID  = m_spImTasMultiIDFind->GetSelectedIDSet();
					m_strEgrSearchedIDs = m_spImTasMultiIDFind->GetSelectedID();
					SetDlgItemText(IDC_EDIT3, m_strEgrSearchedIDs);
					CheckDlgButton(IDC_CHECK_ALL3, BST_CHECKED);

					CheckDlgButton(IDC_CHECK_A, BST_CHECKED);
					CheckDlgButton(IDC_CHECK_T, BST_CHECKED);
					CheckDlgButton(IDC_CHECK_B, BST_CHECKED);
					CheckDlgButton(IDC_CHECK_R, BST_CHECKED);
					CheckDlgButton(IDC_CHECK_TR, BST_CHECKED);

					UpdateReportEgrData();
					UpdateSearchDataCount(IDC_STATIC7, m_mapEgressNodeData, m_setEgrSearchedID);
				}
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


LRESULT KPathVolumeTerminalAccEgrDlg::OnTerminalSearchCompleted( WPARAM a_wParam, LPARAM a_lParam )
{
	try
	{
		bool bRefresh(false);

		KIDCaption* pTerminalNodeIdCaption = (KIDCaption*)a_wParam;
		Integer nxTerminalID = _ttoi64(pTerminalNodeIdCaption->TID);
		
		InitializeTerminalSearchResultData(bRefresh);

		KIDCaption oIDCaption;
		CString    strZoneID(_T(""));

		AutoType itFindAccZone = m_mapAccZoneModeTrip.find(nxTerminalID);
		if (itFindAccZone != m_mapAccZoneModeTrip.end())
		{
			std::map<Integer, std::map<int, double>> &mapZoneModeTrip = itFindAccZone->second;
			AutoType iter  = mapZoneModeTrip.begin();
			AutoType itEnd = mapZoneModeTrip.end();
			while (iter != itEnd)
			{
				Integer nxZoneID = iter->first;
				CString strZoneName(_T(""));
				AutoType itFindZoneName = m_mapZoneName.find(nxZoneID);
				if (itFindZoneName != m_mapZoneName.end())
				{
					strZoneName = itFindZoneName->second;
				}

				strZoneID.Format(_T("%I64d"), nxZoneID);
				oIDCaption.TID      = strZoneID;
				oIDCaption.TCaption = strZoneName;

				m_vecAccessNodeIDCaption.push_back(oIDCaption);
				m_mapAccessNodeData.insert(std::make_pair(nxZoneID, strZoneName));

				++iter;
			}
		}
		AutoType itFindEgrZone = m_mapEgrZoneModeTrip.find(nxTerminalID);
		if (itFindEgrZone != m_mapEgrZoneModeTrip.end())
		{
			std::map<Integer, std::map<int, double>> &mapZoneModeTrip = itFindEgrZone->second;
			AutoType iter  = mapZoneModeTrip.begin();
			AutoType itEnd = mapZoneModeTrip.end();
			while (iter != itEnd)
			{
				Integer nxZoneID = iter->first;
				CString strZoneName(_T(""));
				AutoType itFindZoneName = m_mapZoneName.find(nxZoneID);
				if (itFindZoneName != m_mapZoneName.end())
				{
					strZoneName = itFindZoneName->second;
				}

				strZoneID.Format(_T("%I64d"), nxZoneID);
				oIDCaption.TID      = strZoneID;
				oIDCaption.TCaption = strZoneName;

				m_vecEgressNodeIDCaption.push_back(oIDCaption);
				m_mapEgressNodeData.insert(std::make_pair(nxZoneID, strZoneName));

				++iter;
			}
		}

		UpdateSearchDataCount(IDC_STATIC6, m_mapAccessNodeData, m_setAccSearchedID);
		UpdateSearchDataCount(IDC_STATIC7, m_mapEgressNodeData, m_setEgrSearchedID);

		DrawSelectedOrgDestination(bRefresh);
		bRefresh = true;
		PanToNodeFeature(nxTerminalID, bRefresh);

		if (m_mapAccessNodeData.size() < 1 && m_mapEgressNodeData.size() < 1)
			AfxMessageBox(_T("검색한 존 관련 정보가 존재하지 않습니다."));
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return 0;
}


void KPathVolumeTerminalAccEgrDlg::InitializeTerminalSearchResultData(bool a_bRefresh)
{
	try
	{
		InitVolMinMax();
		SetDlgItemText(IDC_EDIT_MINTRIP, _T("0.0"));
		SetDlgItemText(IDC_EDIT_MAXTRIP, _T("0.0"));
		m_ctrlReportAcc.ResetContent();
		m_ctrlReportEgr.ResetContent();
		m_ctrlReportPath.ResetContent();
		AllClearPath(a_bRefresh);
#pragma region 도착 정보 초기화
		m_setAccSearchedID.clear();
		m_setEgrSearchedID.clear();
		m_strAccSearchedIDs = _T("");
		m_strEgrSearchedIDs = _T("");
		SetDlgItemText(IDC_EDIT2, m_strAccSearchedIDs);
		SetDlgItemText(IDC_EDIT3, m_strEgrSearchedIDs);
		if (nullptr != m_spImTasMultiIDFind)
		{
			m_spImTasMultiIDFind.reset();
		}
#pragma endregion 도착 정보 초기화
		m_vecAccessNodeIDCaption.clear();
		m_mapAccessNodeData.clear();
		m_vecEgressNodeIDCaption.clear();
		m_mapEgressNodeData.clear();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::GetSelectedTerminalTrip(std::map<Integer, std::map<Integer, std::map<int, double>>> &a_mapTerminalZoneModeTrip, std::map<Integer, std::map<int, double>> &a_mapZoneModeTrip)
{
	try
	{
		Integer nxSelTerminalID = _ttoi64(m_oTerminalNodeIdCaption.TID);
		AutoType itFind = a_mapTerminalZoneModeTrip.find(nxSelTerminalID);
		if (itFind == a_mapTerminalZoneModeTrip.end())
		{
			return;
		}

		a_mapZoneModeTrip = itFind->second;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::UpdateReportAccData()
{
	try
	{
		m_ctrlReportAcc.ResetContent();

		std::map<Integer, std::map<int, double>> mapZoneModeTrip;
		GetSelectedTerminalTrip(m_mapAccZoneModeTrip, mapZoneModeTrip);

		std::vector<int> vecModeCode;
		AutoType iterB  = m_mapModeCodeName.begin();
		AutoType itEndB = m_mapModeCodeName.end();
		while (iterB != itEndB)
		{
			vecModeCode.push_back(iterB->first);
			++iterB;
		}

		BOOL bAllSelected(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bAllSelected = TRUE;
		else
			bAllSelected = FALSE;

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		CString strZoneID(_T(""));
		CString strZoneNode(_T(""));
		size_t  nxModeCnt = vecModeCode.size();
		CString strCheck(_T("")); 

		AutoType iter  = m_mapAccessNodeData.begin();
		AutoType itEnd = m_mapAccessNodeData.end();

		while(iter != itEnd)
		{
			Integer nxZoneID     = iter->first;
			CString &strZoneName = iter->second;
			strZoneID.Format(_T("%I64d"), nxZoneID);

			if (m_setAccSearchedID.find(strZoneID) == m_setAccSearchedID.end())
			{
				++iter;
				continue;
			}

			pRecord = m_ctrlReportAcc.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetChecked(bAllSelected);

			if(strZoneName.Compare(_T("")) == 0)
				strZoneName = _T("-");
			strZoneNode.Format(_T("%I64d[%s]"), nxZoneID, strZoneName);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneNode));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR) nxZoneID);

			std::map<int, double> mapModeTrip;
			AutoType itFindZoneModeTrip = mapZoneModeTrip.find(nxZoneID);
			if (itFindZoneModeTrip != mapZoneModeTrip.end())
			{
				mapModeTrip = itFindZoneModeTrip->second;
			}

			for (size_t i= 0; i< nxModeCnt; i++)
			{
				int &nModeCode = vecModeCode[i];
				
				if (mapModeTrip.find(nModeCode) == mapModeTrip.end())
					strCheck = _T("-");
				else
					strCheck = _T("v");

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCheck));
				pItem->SetAlignment(DT_CENTER);
			}

			++iter;
		}

		m_ctrlReportAcc.Populate();

		SetVolMinMax();
		UpdateMinMaxTrip();
		UpdateReportPathData();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::UpdateReportEgrData()
{
	try
	{
		m_ctrlReportEgr.ResetContent();

		std::map<Integer, std::map<int, double>> mapZoneModeTrip;
		GetSelectedTerminalTrip(m_mapEgrZoneModeTrip, mapZoneModeTrip);

		std::vector<int> vecModeCode;
		AutoType iterB  = m_mapModeCodeName.begin();
		AutoType itEndB = m_mapModeCodeName.end();
		while (iterB != itEndB)
		{
			vecModeCode.push_back(iterB->first);
			++iterB;
		}

		BOOL bAllSelected(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL3) == BST_CHECKED)
			bAllSelected = TRUE;
		else
			bAllSelected = FALSE;

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		CString strZoneID(_T(""));
		CString strZoneNode(_T(""));
		size_t  nxModeCnt = vecModeCode.size();
		CString strCheck(_T("")); 

		AutoType iter  = m_mapEgressNodeData.begin();
		AutoType itEnd = m_mapEgressNodeData.end();

		while(iter != itEnd)
		{
			Integer nxZoneID     = iter->first;
			CString &strZoneName = iter->second;
			strZoneID.Format(_T("%I64d"), nxZoneID);

			if (m_setEgrSearchedID.find(strZoneID) == m_setEgrSearchedID.end())
			{
				++iter;
				continue;
			}

			pRecord = m_ctrlReportEgr.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetChecked(bAllSelected);

			if(strZoneName.Compare(_T("")) == 0)
				strZoneName = _T("-");
			strZoneNode.Format(_T("%I64d[%s]"), nxZoneID, strZoneName);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strZoneNode));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR) nxZoneID);

			std::map<int, double> mapModeTrip;
			AutoType itFindZoneModeTrip = mapZoneModeTrip.find(nxZoneID);
			if (itFindZoneModeTrip != mapZoneModeTrip.end())
			{
				mapModeTrip = itFindZoneModeTrip->second;
			}

			for (size_t i= 0; i< nxModeCnt; i++)
			{
				int &nModeCode = vecModeCode[i];

				if (mapModeTrip.find(nModeCode) == mapModeTrip.end())
					strCheck = _T("-");
				else
					strCheck = _T("v");

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCheck));
				pItem->SetAlignment(DT_CENTER);
			}

			++iter;
		}

		m_ctrlReportEgr.Populate();

		SetVolMinMax();
		UpdateMinMaxTrip();
		UpdateReportPathData();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::UpdateSearchDataCount(int a_nUID, KTDNodeDataMap &a_mapNodeData, std::set<CString> &a_setSearchedID)
{
	try
	{
		CString strCount(_T(""));

		int nTotalCnt             = (int)a_mapNodeData.size();
		int nSearchCnt            = (int)a_setSearchedID.size();

        if (KmzSystem::GetLanguage() == KEMKorea) {
            strCount.Format(_T("탐색결과(선택/전체):  %d / %d"), nSearchCnt, nTotalCnt);
        }
        else {
            strCount.Format(_T("Records:  %d / %d"), nSearchCnt, nTotalCnt);
        }
		
		SetDlgItemText(a_nUID, strCount);	
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::UpdateReportPathData()
{
	try
	{
		m_ctrlReportPath.ResetContent();

//		AllClearPath(true);

		std::set<int> setCheckMode;
		if (IsDlgButtonChecked(IDC_CHECK_A) == BST_CHECKED)
			setCheckMode.insert(KEMInterPathTerminalModeAuto);
		if (IsDlgButtonChecked(IDC_CHECK_T) == BST_CHECKED)
			setCheckMode.insert(KEMInterPathTerminalModeTaxi);
		if (IsDlgButtonChecked(IDC_CHECK_B) == BST_CHECKED)
			setCheckMode.insert(KEMInterPathTerminalModeBus);
		if (IsDlgButtonChecked(IDC_CHECK_R) == BST_CHECKED)
			setCheckMode.insert(KEMInterPathTerminalModeRail);
		if (IsDlgButtonChecked(IDC_CHECK_TR) == BST_CHECKED)
			setCheckMode.insert(KEMInterPathTerminalModeTransit);

		std::map<Integer, std::map<int, double>> mapZoneModeTrip;
		GetSelectedTerminalTrip(m_mapAccZoneModeTrip, mapZoneModeTrip);
		std::vector<Integer> vecCheckedAccZone;
		GetCheckedZone(m_ctrlReportAcc, vecCheckedAccZone);

		AddReportPathRecord(KEMInterAccEgrTypeAccess, vecCheckedAccZone, setCheckMode, mapZoneModeTrip);

		mapZoneModeTrip.clear();
		GetSelectedTerminalTrip(m_mapEgrZoneModeTrip, mapZoneModeTrip);
		std::vector<Integer> vecCheckedEgrZone;
		GetCheckedZone(m_ctrlReportEgr, vecCheckedEgrZone);

		AddReportPathRecord(KEMInterAccEgrTypeEgress, vecCheckedEgrZone, setCheckMode, mapZoneModeTrip);
	
		m_ctrlReportPath.Populate();
		if ((vecCheckedAccZone.size() + vecCheckedEgrZone.size()) < EXPANDALL_LIMIT_COUNT)
		{
			m_ctrlReportPath.ExpandAll(TRUE);
		}

		if (vecCheckedAccZone.size() < 1 && vecCheckedEgrZone.size() < 1) {
			DoneDraw(false);
		}
		else {
			DoneDraw(true);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::AddReportPathRecord(KEMInterAccEgrType a_emInterAccEgrType, std::vector<Integer> &a_vecCheckedZone, std::set<int> &a_setCheckMode, std::map<Integer, std::map<int, double>> &a_mapZoneModeTrip)
{
	try
	{
		CString strAccEgr(_T(""));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			if(KEMInterAccEgrTypeAccess == a_emInterAccEgrType) {
				strAccEgr = _T("출발");
			}
			else {
				strAccEgr = _T("도착");
			}
		}
		else {
			if(KEMInterAccEgrTypeAccess == a_emInterAccEgrType) {
				strAccEgr = _T("Access");
			}
			else {
				strAccEgr = _T("Egress");
			}
		}

		int nAccEgrType = a_emInterAccEgrType;

		CXTPReportRecord*     pParRecord       = nullptr;
		CXTPReportRecords*    pSeParRecords    = nullptr;
		CXTPReportRecord*     pSeParRecord     = nullptr;
		CXTPReportRecords*    pThParRecords    = nullptr;
		CXTPReportRecord*     pThParRecord     = nullptr;

		CXTPReportRecordItem* pItem            = nullptr;
		CXTPReportRecordItem* pParentCheckItem = nullptr;

		size_t nxCheckedZoneCnt = a_vecCheckedZone.size();
		if (nxCheckedZoneCnt > 0)
		{
			pParRecord = m_ctrlReportPath.AddRecord(new CXTPReportRecord());

			pItem      = pParRecord->AddItem(new CXTPReportRecordItemText(strAccEgr));
			for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
			{
				pParRecord->AddItem(new CXTPReportRecordItemText(_T("")));
			}
			pItem->SetAlignment(DT_LEFT);
			pItem->SetEditable(FALSE);
			pItem->SetItemData((DWORD_PTR) nAccEgrType);

			CString strZoneDisplay(_T(""));

			// zone
			pSeParRecords      = pParRecord->GetChilds();
			for (size_t i= 0; i< nxCheckedZoneCnt; i++)
			{
				Integer &nxZoneID = a_vecCheckedZone[i];
				SetZoneDisplayName(nxZoneID, strZoneDisplay);

				pSeParRecord       = pSeParRecords->Add(new CXTPReportRecord());
				pParentCheckItem   = pSeParRecord->AddItem(new CXTPReportRecordItemText(strZoneDisplay));
				for (int r= 0; r< B_COLUMN_TOTAL_COUNT -1; r++)
				{
					pSeParRecord->AddItem(new CXTPReportRecordItemText(_T("")));
				}
				pParentCheckItem->SetAlignment(DT_LEFT);
				pParentCheckItem->SetItemData((DWORD_PTR)nxZoneID);

				BOOL bHasCheckBox(FALSE);

				AutoType itFindModeTrip  = a_mapZoneModeTrip.find(nxZoneID);
				if (itFindModeTrip != a_mapZoneModeTrip.end())
				{
					std::map<int, double> &mapModeTrip = itFindModeTrip->second;

					AutoType iterMode    = mapModeTrip.begin();
					AutoType itEndMode   = mapModeTrip.end();

					// mode
					pThParRecords        = pSeParRecord->GetChilds();
					while (iterMode != itEndMode)
					{
						int     nModeCode = iterMode->first;
						double  &dTrip    = iterMode->second;

						if (a_setCheckMode.find(nModeCode) == a_setCheckMode.end())
						{
							++iterMode;
							continue;
						}

						CString strModeName(_T(""));
						AutoType itFindModeName = m_mapModeCodeName.find(nModeCode);
						if (itFindModeName != m_mapModeCodeName.end())
							strModeName = itFindModeName->second;

						pThParRecord = pThParRecords->Add(new CXTPReportRecord());

						pItem        = pThParRecord->AddItem(new CXTPReportRecordItemText(strModeName));
						pItem->SetAlignment(DT_LEFT);
						pItem->SetEditable(TRUE);
						pItem->SetItemData((DWORD_PTR) nModeCode);
						pItem->HasCheckbox(TRUE);
						pItem->SetChecked(TRUE);

						pItem        = pThParRecord->AddItem(new CXTPReportRecordItemNumber(dTrip, _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);

						bHasCheckBox = TRUE;

						++iterMode;
					}
				}

				pParentCheckItem->HasCheckbox(bHasCheckBox);
				pParentCheckItem->SetChecked(bHasCheckBox);
				pParentCheckItem->SetEditable(bHasCheckBox);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::InitVolMinMax()
{
	m_dMaxVolume  = 0.0;
	m_dMinVolume  = 0.0;
}


void KPathVolumeTerminalAccEgrDlg::SetVolMinMax()
{
	try
	{
		InitVolMinMax();

		bool   bDoneMinMax(false);
		double dMaxVolume = -999999999999.0;
		double dMinVolume = 999999999999.0;

		//acc
		std::map<Integer, std::map<int, double>> mapZoneModeTrip;
		GetSelectedTerminalTrip(m_mapAccZoneModeTrip, mapZoneModeTrip);
		std::vector<Integer> vecCheckedAccZone;
		GetCheckedZone(m_ctrlReportAcc, vecCheckedAccZone);

		size_t nxCheckCount = vecCheckedAccZone.size();
		for (size_t i= 0; i< nxCheckCount; i++)
		{
			Integer &nxAccZoneID = vecCheckedAccZone[i];

			AutoType itFindZone = mapZoneModeTrip.find(nxAccZoneID);
			if (itFindZone == mapZoneModeTrip.end())
			{
				continue;
			}
			
			std::map<int, double>& mapModeTrip = itFindZone->second;
			if (mapModeTrip.size() > 0)
				bDoneMinMax = true;

			CollectMinMaxModeTrip(mapModeTrip, dMaxVolume, dMinVolume);
		}

		//egr
		mapZoneModeTrip.clear();
		GetSelectedTerminalTrip(m_mapEgrZoneModeTrip, mapZoneModeTrip);
		std::vector<Integer> vecCheckedEgrZone;
		GetCheckedZone(m_ctrlReportEgr, vecCheckedEgrZone);

		nxCheckCount = vecCheckedEgrZone.size();
		for (size_t i= 0; i< nxCheckCount; i++)
		{
			Integer &nxEgrZoneID = vecCheckedEgrZone[i];

			AutoType itFindZone = mapZoneModeTrip.find(nxEgrZoneID);
			if (itFindZone == mapZoneModeTrip.end())
			{
				continue;
			}

			std::map<int, double>& mapModeTrip = itFindZone->second;
			if (mapModeTrip.size() > 0)
				bDoneMinMax = true;

			CollectMinMaxModeTrip(mapModeTrip, dMaxVolume, dMinVolume);
		}

		if (bDoneMinMax)
		{
			m_dMaxVolume = dMaxVolume;
			m_dMinVolume = dMinVolume;
		}
	}
	catch (...)
	{
		InitVolMinMax();
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::UpdateMinMaxTrip()
{
	CString strMinTrip(_T(""));
	CString strMaxTrip(_T(""));

	double dMinTrip = m_dMinVolume - 0.5;
	if(dMinTrip < 0.0)
		dMinTrip = 0.0;

	double dMaxTrip(0.0);
	if (m_dMaxVolume > 0.0)
	{
		dMaxTrip = m_dMaxVolume + 0.6;
	}

	strMinTrip.Format(_T("%.0f"), dMinTrip);
	strMaxTrip.Format(_T("%.0f"), dMaxTrip);

	/*SetDlgItemText(IDC_EDIT_MINTRIP, strMinTrip);*/
	SetDlgItemText(IDC_EDIT_MINTRIP, _T("0"));
	SetDlgItemText(IDC_EDIT_MAXTRIP, strMaxTrip);
}


void KPathVolumeTerminalAccEgrDlg::CollectMinMaxModeTrip(std::map<int, double>& a_mapModeTrip, double &a_dMaxVolume, double &a_dMinVolume)
{
	try
	{
		AutoType iter  = a_mapModeTrip.begin();
		AutoType itEnd = a_mapModeTrip.end();

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


void KPathVolumeTerminalAccEgrDlg::GetCheckedZone(CXTPReportControl &a_ctrlReport, std::vector<Integer> &a_vecCheckedZone)
{
	try
	{
		CXTPReportRecords*    pRecords = a_ctrlReport.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_SELECT);

			if (pItem->IsChecked() == FALSE)
				continue;

			pItem   = pRecord->GetItem(_1_COLUMN_NAME);
			Integer nxZoneID = (Integer)pItem->GetItemData();

			a_vecCheckedZone.push_back(nxZoneID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::OnReportAccCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_0_COLUMN_SELECT == nIndex)
	{
		CXTPReportRecords* pRecords = m_ctrlReportAcc.GetRecords();
		int nTotalCount = pRecords->GetCount();

		int nCheckedCount = 0;
		for (int i=0; i<nTotalCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_SELECT);

			if (pItem->IsChecked() == TRUE)
				nCheckedCount++;
		}

		if(nCheckedCount == nTotalCount)
			CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
		if(nCheckedCount < nTotalCount || 0 == nTotalCount)
			CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);

		SetVolMinMax();
		UpdateMinMaxTrip();
		UpdateReportPathData();
	}
}


void KPathVolumeTerminalAccEgrDlg::OnReportEgrCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_0_COLUMN_SELECT == nIndex)
	{
		CXTPReportRecords* pRecords = m_ctrlReportEgr.GetRecords();
		int nTotalCount = pRecords->GetCount();

		int nCheckedCount = 0;
		for (int i=0; i<nTotalCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_SELECT);

			if (pItem->IsChecked() == TRUE)
				nCheckedCount++;
		}

		if(nCheckedCount == nTotalCount)
			CheckDlgButton(IDC_CHECK_ALL3, BST_CHECKED);
		if(nCheckedCount < nTotalCount || 0 == nTotalCount)
			CheckDlgButton(IDC_CHECK_ALL3, BST_UNCHECKED);

		SetVolMinMax();
		UpdateMinMaxTrip();
		UpdateReportPathData();
	}
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckAllAcc()
{
	CheckAllAcc(true);
}


void KPathVolumeTerminalAccEgrDlg::CheckAllAcc(bool a_bUpdateVol)
{
	try
	{
		BOOL bSelectedAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
		{
			bSelectedAll = TRUE;
		}

		CXTPReportRecords*    pReocrds = m_ctrlReportAcc.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt   = pReocrds->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pReocrds->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
			pItem->SetChecked(bSelectedAll);
		}

		m_ctrlReportAcc.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	if (a_bUpdateVol) {
		SetVolMinMax();
		UpdateMinMaxTrip();
		UpdateReportPathData();
	}
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckAllEgr()
{
	CheckAllEgr(true);
}


void KPathVolumeTerminalAccEgrDlg::CheckAllEgr(bool a_bUpdateVol)
{
	try
	{
		BOOL bSelectedAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL3) == BST_CHECKED)
		{
			bSelectedAll = TRUE;
		}

		CXTPReportRecords*    pReocrds = m_ctrlReportEgr.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt   = pReocrds->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pReocrds->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
			pItem->SetChecked(bSelectedAll);
		}

		m_ctrlReportEgr.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	if (a_bUpdateVol) {
		SetVolMinMax();
		UpdateMinMaxTrip();
		UpdateReportPathData();
	}
}


void KPathVolumeTerminalAccEgrDlg::OnReportPathCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_0_B_COLUMN_ZONEID == nIndex)
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

			m_ctrlReportPath.RedrawControl();
		}
		catch (...)
		{
			TxLogDebugException();	
		}

		std::vector<Integer> vecCheckedAccZone;
		GetCheckedZone(m_ctrlReportAcc, vecCheckedAccZone);
		std::vector<Integer> vecCheckedEgrZone;
		GetCheckedZone(m_ctrlReportEgr, vecCheckedEgrZone);

		if (vecCheckedAccZone.size() < 1 && vecCheckedEgrZone.size() < 1) {
			DoneDraw(false);
		}
		else {
			DoneDraw(true);
		}
	}
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckOutrange()
{
	UpdateOutRange();
}


void KPathVolumeTerminalAccEgrDlg::UpdateOutRange()
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


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckLabel()
{
	try
	{
		bool bLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bLabel = true;
		else
			bLabel = false;

		m_pMapView->TxDrawCompositeNodeLabel(bLabel, false);
		//m_pMapView->CacheDrawCompositeUserPointLabel(bLabel, false);
		m_pMapView->TxDrawShowCompositeUserLineLablel(bLabel, true);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeTerminalAccEgrDlg::SetNodeViewData(KTDNodeDataMap &a_mapNodeData, CString a_strNodeID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
{
	try
	{
		Integer nxNodelID = _ttoi64(a_strNodeID);

		AutoType iter  = a_mapNodeData.find(nxNodelID);
		AutoType itEnd = a_mapNodeData.end();

		CString strName(_T(""));
		if (iter != itEnd)
		{
			strName = iter->second;
		}

		a_oIDCaption.TID      = a_strNodeID;
		a_oIDCaption.TCaption = strName;

		ConvertEmptyNodeName(strName);

		a_strNodeViewData.Format(_T("%s [%s]"), a_strNodeID, strName);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::SetZoneDisplayName(Integer &a_nxZoneID, CString &a_strZoneDisplay)
{
	try
	{
		CString strZoneName(_T(""));

		AutoType itFindName = m_mapZoneName.find(a_nxZoneID);
		if (itFindName != m_mapZoneName.end())
			strZoneName = itFindName->second;

		ConvertEmptyNodeName(strZoneName);

		a_strZoneDisplay.Format(_T("%I64d [%s]"), a_nxZoneID, strZoneName);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KPathVolumeTerminalAccEgrDlg::PanToNodeFeature( Integer &a_nxNodeID, bool a_bRefresh )
{
	try
	{
		m_pMapView->PanToFeature(KLayerID::Node(), a_nxNodeID, a_bRefresh);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckA()
{
	UpdateReportPathData();
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckT()
{
	UpdateReportPathData();
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckB()
{
	UpdateReportPathData();
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckR()
{
	UpdateReportPathData();
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckTR()
{
	UpdateReportPathData();
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonExport()
{
	try
	{
		std::vector<Integer> vecCheckedAccZone;
		GetCheckedZone(m_ctrlReportAcc, vecCheckedAccZone);

		std::vector<Integer> vecCheckedEgrZone;
		GetCheckedZone(m_ctrlReportEgr, vecCheckedEgrZone);

		std::map<Integer, std::map<int, double>> mapAccessZoneModeTrip;
		GetSelectedTerminalTrip(m_mapAccZoneModeTrip, mapAccessZoneModeTrip);

		std::map<Integer, std::map<int, double>> mapEgressZoneModeTrip;
		GetSelectedTerminalTrip(m_mapEgrZoneModeTrip, mapEgressZoneModeTrip);

		if (vecCheckedAccZone.size() < 1 && vecCheckedEgrZone.size() < 1)
		{
			return;
		}

		KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();

		CString strInitFileName(_T(""));
		strInitFileName.Format(_T("%s_%s_TerminalAccessEgressZone"), pScenario->GetName(), m_pTarget->GetName());

		TCHAR szFilter[] = _T("Text Files (*.txt)|*.txt|");
		CFileDialog fileDlg( FALSE, _T("txt"), strInitFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );

		CString strLocation = m_pTarget->GetLocation();
		if (!strLocation.IsEmpty())
		{
			fileDlg.m_ofn.lpstrInitialDir = strLocation;
		}
		if (fileDlg.DoModal() != IDOK)
		{
			return;
		}
		CString strFilePath = fileDlg.GetPathName();

		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(strFilePath, nOpenMode);

		CWaitCursor cw;
		CString strExport(_T(""));
		try
		{
			CString strTerminalID   = m_oTerminalNodeIdCaption.TID;
			CString strTerminalName = m_oTerminalNodeIdCaption.TCaption;

			// write title 
			CString strTitle(_T(""));
			strTitle.Format(_T("Terminal: %s"), strTerminalID);
			if (strTerminalName.IsEmpty() == FALSE)
			{
				strTitle.AppendFormat(_T("_%s"), strTerminalName);
			}

			// write header
			CString strHeader(_T(""));
			strHeader.Append(_T("Acc/Egr\tZoneID\tName"));
			AutoType iterName  = m_mapModeCodeName.begin();
			AutoType itEndName = m_mapModeCodeName.end();
			while (iterName != itEndName)
			{
				CString strModeName = iterName->second;
				strModeName.Trim();

				strHeader.AppendFormat(_T("\t%s"), strModeName);

				++iterName;
			}

			// write
			strExport.AppendFormat(_T("%s"), strTitle);
			strExport.Append(_T("\r\n"));

			strExport.AppendFormat(_T("%s"), strHeader);
			strExport.Append(_T("\r\n"));
			of.WriteString(strExport);

			WriteCheckZoneModeTrip(of, _T("Access"), vecCheckedAccZone, mapAccessZoneModeTrip);
			WriteCheckZoneModeTrip(of, _T("Egress"), vecCheckedEgrZone, mapEgressZoneModeTrip);
			
			of.Close();
		}
		catch (...)
		{
			TxLogDebugException();	
			of.Close();
			return;
		}

		ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::WriteCheckZoneModeTrip(CStdioFileEx &a_of, CString a_strAccEgr, std::vector<Integer> &a_vecCheckedZone, std::map<Integer, std::map<int, double>> &a_mapZoneModeTrip)
{
	try
	{
		CString strExport(_T(""));

		size_t nxCheckZoneCnt = a_vecCheckedZone.size();
		for (size_t i= 0; i< nxCheckZoneCnt; i++)
		{
			Integer &nxZoneID = a_vecCheckedZone[i];
			CString strZoneName(_T(""));

			AutoType itFindZoneName = m_mapZoneName.find(nxZoneID);
			if (itFindZoneName != m_mapZoneName.end())
			{
				strZoneName = itFindZoneName->second;
			}

			std::map<int, double> mapModeTrip;
			AutoType itFindModeTrip = a_mapZoneModeTrip.find(nxZoneID);
			if (itFindModeTrip != a_mapZoneModeTrip.end())
			{
				mapModeTrip = itFindModeTrip->second;
			}

			strExport.Format(_T("%s\t%I64d\t%s"), a_strAccEgr, nxZoneID, strZoneName);
			AutoType iter  = m_mapModeCodeName.begin();
			AutoType itEnd = m_mapModeCodeName.end();
			while (iter != itEnd)
			{
				int    nModeCode = iter->first;
				double dTrip(0.0);

				AutoType itFindTrip = mapModeTrip.find(nModeCode);
				if (itFindTrip != mapModeTrip.end())
				{
					dTrip = itFindTrip->second;
				}

				strExport.AppendFormat(_T("\t%f"), dTrip);

				++iter;
			}

			strExport.Append(_T("\r\n"));
			a_of.WriteString(strExport);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::OnReportColorItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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
				m_ctrlReportColor.RedrawControl();

				CXTPReportRecord*	  pRecord      = pItemNotify->pRow->GetRecord();
				CXTPReportRecordItem* pItem        = pRecord->GetItem(0);
				TInterPathColor*      pTModeColor  = (TInterPathColor*)pItem->GetItemData();

				pTModeColor->clrPath = clrBack;
			}
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KPathVolumeTerminalAccEgrDlg::OnReportAccItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_1_COLUMN_NAME == nIndex)
	{
		try
		{
			CXTPReportRecord*	     pRecord     = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem*    pItem       = pRecord->GetItem(_1_COLUMN_NAME);
			Integer nxZoneID = (Integer)pItem->GetItemData();

			m_pMapView->PanToFeature(KLayerID::Node(), nxZoneID);
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KPathVolumeTerminalAccEgrDlg::OnReportEgrItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_1_COLUMN_NAME == nIndex)
	{
		try
		{
			CXTPReportRecord*	     pRecord     = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem*    pItem       = pRecord->GetItem(_1_COLUMN_NAME);
			Integer nxZoneID = (Integer)pItem->GetItemData();

			m_pMapView->PanToFeature(KLayerID::Node(), nxZoneID);
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KPathVolumeTerminalAccEgrDlg::CloseWindowProc()
{
	AllClearPath(true);

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonDraw()
{
	TxLogDebugStartMsg();

	DoneDraw(true);

	TxLogDebugEndMsg();
}

void KPathVolumeTerminalAccEgrDlg::DoneDraw(bool a_bShowErrMsg)
{
	TxLogDebugStartMsg();

	try
	{
		m_oMapResult.vecDrawNodeInfo.clear();
		m_oMapResult.vecDrawZoneNodeInfo.clear();
		m_oMapResult.vecUserLineInfo.clear();
		m_oMapResult.vecUserPointInfo.clear();

		if (InvalidateInputInfo(a_bShowErrMsg) == false) {
			throw 1;
		}

		GetResultSettingInfo(m_oTResltSettingInfo);

		//QBicSimpleProgressThread::ExecuteThread(MakeDrawDataThreadCaller, this);
		CWaitCursor cw;
		MakeDrawData();

		if (!m_bResult) {
			if (a_bShowErrMsg) {
				AfxMessageBox(m_strErrMsg);
			}
			throw 1;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	try 
	{
		AllClearPath(false);

		bool bLabel(false);
		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bLabel = true;
		else
			bLabel = false;

		GetSelectedDrawNodeInfo(m_oMapResult.vecDrawNodeInfo, false); // select zone 추가

		m_pMapView->TxDrawCompositeNode(m_oMapResult.vecDrawNodeInfo, bLabel, false);
		m_pMapView->TxDrawCompositeUserLine(m_oMapResult.vecUserLineInfo, bLabel, false);
		if (IsDlgButtonChecked(IDC_CHECK_ZONEACCESS) == BST_CHECKED) {
			DrawAccEgrsZoneAccess(false); // org와 Dest들의 존엑세스를 그린다.
		}

		m_pMapView->MapRefresh();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

void KPathVolumeTerminalAccEgrDlg::DrawAccEgrsZoneAccess(bool a_bRefresh)
{
	std::vector<Integer> vecCheckedAccZone;
	GetCheckedZone(m_ctrlReportAcc, vecCheckedAccZone);

	std::vector<Integer> vecCheckedEgrZone;
	GetCheckedZone(m_ctrlReportEgr, vecCheckedEgrZone);

	if (vecCheckedAccZone.size() < 1 && vecCheckedEgrZone.size() < 1) {
		return;
	}

	std::vector<TDrawZoneAccess> vecZoneAccess;
	TDrawZoneAccess oDrawZoneAccess;
	
	oDrawZoneAccess.TSymbol = 48;//임시 모양
	oDrawZoneAccess.TRadius = m_dRadius;
	oDrawZoneAccess.TSize   = 18.0; //임시 사이즈

	// Access Zone 그리기
	Integer nxID(0);
	CString strID(_T(""));
	for (size_t i= 0; i< vecCheckedAccZone.size(); i++) {
		nxID = vecCheckedAccZone[i];
		strID.Format(_T("%I64d"), nxID);

		oDrawZoneAccess.TNodeID   = nxID;
		oDrawZoneAccess.TColorRGB = RGB(255,0,0);
		oDrawZoneAccess.TLabel    = strID;

		vecZoneAccess.push_back(oDrawZoneAccess);
	}

	// Egress zone 그리기
	for (size_t i= 0; i< vecCheckedEgrZone.size(); i++) {
		nxID = vecCheckedEgrZone[i];
		strID.Format(_T("%I64d"), nxID);

		oDrawZoneAccess.TNodeID   = nxID;
		oDrawZoneAccess.TColorRGB = RGB(0,0,205);
		oDrawZoneAccess.TLabel    = strID;

		vecZoneAccess.push_back(oDrawZoneAccess);
	}

	m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, a_bRefresh);
}

void KPathVolumeTerminalAccEgrDlg::DrawSelectedOrgDestination(bool bRefresh)
{
	try
	{
		std::vector<TDrawNodeInfo> vecSelectedDrawNodeInfo;

		GetSelectedDrawNodeInfo(vecSelectedDrawNodeInfo);

		m_pMapView->TxDrawCompositeNode(vecSelectedDrawNodeInfo, false, bRefresh);

	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear)
{
	try
	{
		if (a_bClear)
		{
			a_vecSelectedDrawNodeInfo.clear();
		}

		Integer nxSelTerminalID = _ttoi64(m_oTerminalNodeIdCaption.TID);

#pragma region 터미널
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = nxSelTerminalID;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), nxSelTerminalID);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = RGB(128,0,192);

		//임시 사이즈
		oNodeInfo.TSize = 15.0;

		//임시 모양
		oNodeInfo.TSymbol = 72;
#pragma endregion 터미널

		a_vecSelectedDrawNodeInfo.push_back(oNodeInfo);
		
		std::vector<Integer> vecCheckedAccZone;
		GetCheckedZone(m_ctrlReportAcc, vecCheckedAccZone);

		std::vector<Integer> vecCheckedEgrZone;
		GetCheckedZone(m_ctrlReportEgr, vecCheckedEgrZone);

		Integer nxZoneID(0);

		size_t nxCheckCnt = vecCheckedAccZone.size();
		for (size_t i= 0; i< nxCheckCnt; i++)
		{
			nxZoneID = vecCheckedAccZone[i];

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID         = nxZoneID;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxZoneID);
			//oNodeInfo.TLabel          = strNodeLable;
			oNodeInfo.TLabel          = _T("");

			oNodeInfo.TSymbol = nZoneCharIndex;
			oNodeInfo.TSize           = nZoneSize;
			//oNodeInfo.TColorRGB       = RGB(255,  66,  0);
			oNodeInfo.TColorRGB       = m_clrOrgZone;

			a_vecSelectedDrawNodeInfo.push_back(oNodeInfo);
		}

		nxCheckCnt = vecCheckedEgrZone.size();
		for (size_t i= 0; i< nxCheckCnt; i++)
		{
			nxZoneID = vecCheckedEgrZone[i];

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID         = nxZoneID;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxZoneID);
			//oNodeInfo.TLabel          = strNodeLable;
			oNodeInfo.TLabel          = _T("");

			oNodeInfo.TSymbol = nZoneCharIndex;
			oNodeInfo.TSize           = nZoneSize;
			//oNodeInfo.TColorRGB       = RGB(0,  128,  192);
			oNodeInfo.TColorRGB       = m_clrDestZone;

			a_vecSelectedDrawNodeInfo.push_back(oNodeInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KPathVolumeTerminalAccEgrDlg::InvalidateInputInfo(bool a_bShowErrMsg)
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


unsigned __stdcall KPathVolumeTerminalAccEgrDlg::MakeDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KPathVolumeTerminalAccEgrDlg*     pDlg = (KPathVolumeTerminalAccEgrDlg*)pParameter->GetParameter();
	pDlg->MakeDrawData();

	return 0;
}


void KPathVolumeTerminalAccEgrDlg::MakeDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		Integer nxSelTerminalID = _ttoi64(m_oTerminalNodeIdCaption.TID);

		std::map<Integer, std::map<int, double>> mapAccessZoneModeTrip;
		GetSelectedTerminalTrip(m_mapAccZoneModeTrip, mapAccessZoneModeTrip);
		std::map<Integer, std::map<int, double>> mapEgressZoneModeTrip;
		GetSelectedTerminalTrip(m_mapEgrZoneModeTrip, mapEgressZoneModeTrip);

		std::map<int, std::map<KODKey, std::map<int, double>>> mapTypeODModeTrip;

		CXTPReportRecordItem*   pItem    = nullptr;
		CXTPReportRecords*      pRecords = m_ctrlReportPath.GetRecords();
		int nCount            = pRecords->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord*   pParRecord = pRecords->GetAt(i);
			pItem             = pParRecord->GetItem(_0_B_COLUMN_ZONEID);

			int nAccEgrType = (int)pItem->GetItemData();
			std::map<KODKey, std::map<int, double>> mapODModeTrip;

			CXTPReportRecords*  pSeParRecords = pParRecord->GetChilds();
			if (nullptr == pSeParRecords)
			{
				continue;
			}

			for (int k= 0; k< pSeParRecords->GetCount(); k++)
			{
				CXTPReportRecord* pSeParRecord = pSeParRecords->GetAt(k);
				pItem           = pSeParRecord->GetItem(_0_B_COLUMN_ZONEID);

				if (pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
					continue;

				Integer nxZoneID = (Integer)pItem->GetItemData();
				std::map<int, double> mapModeTrip;
				if (KEMInterAccEgrTypeAccess == nAccEgrType)
				{
					AutoType itFindModeTrip = mapAccessZoneModeTrip.find(nxZoneID);
					if (itFindModeTrip != mapAccessZoneModeTrip.end())
						mapModeTrip = itFindModeTrip->second;
				}
				else
				{
					AutoType itFindModeTrip = mapEgressZoneModeTrip.find(nxZoneID);
					if (itFindModeTrip != mapEgressZoneModeTrip.end())
						mapModeTrip = itFindModeTrip->second;
				}

				CXTPReportRecords* pThParRecords = pSeParRecord->GetChilds();
				if (nullptr == pThParRecords)
					continue;

				for (int m = 0; m < pThParRecords->GetCount(); m++)
				{
					CXTPReportRecord* pThParRecord = pThParRecords->GetAt(m);
					pItem           = pThParRecord->GetItem(_0_B_COLUMN_ZONEID);

					if (pItem->GetHasCheckbox() == FALSE || pItem->IsChecked() == FALSE)
						continue;

					int nModeCode = (int)pItem->GetItemData();
					AutoType itFindTrip = mapModeTrip.find(nModeCode);
					if (itFindTrip == mapModeTrip.end())
						continue;
					double &dTrip = itFindTrip->second;

					KODKey oODKey;
					if (KEMInterAccEgrTypeAccess == nAccEgrType)
					{
						oODKey.OriginID      = nxZoneID;
						oODKey.DestinationID = nxSelTerminalID;
					}
					else
					{
						oODKey.OriginID      = nxSelTerminalID;
						oODKey.DestinationID = nxZoneID;
					}

					AutoType itFindOD = mapODModeTrip.find(oODKey);
					if (itFindOD != mapODModeTrip.end())
					{
						std::map<int, double> &mapInModeTrip = itFindOD->second;
						mapInModeTrip.insert(std::make_pair(nModeCode, dTrip));
					}
					else
					{
						std::map<int, double> mapInModeTrip;
						mapInModeTrip.insert(std::make_pair(nModeCode, dTrip));

						mapODModeTrip.insert(std::make_pair(oODKey, mapInModeTrip));
					}
				}
			}

			mapTypeODModeTrip.insert(std::make_pair(nAccEgrType, mapODModeTrip));
		}

		if (mapTypeODModeTrip.size()< 1)
		{
			m_strErrMsg = _T("복합수단 통행배정 결과 데이터가 존재하지 않습니다.");
			m_bResult = false;
			return;
		}
		
		KInterModalPathInfo::TerminalAccEgrTripInfo(m_pTarget, nxSelTerminalID, mapTypeODModeTrip, m_oTResltSettingInfo, m_oMapResult);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		m_strErrMsg = _T("Draw Data 수집 오류 발생");
		m_bResult = false;
	}
	catch(...)
	{
		TxLogDebugException();
		m_strErrMsg = _T("Draw Data 수집 오류 발생");
		m_bResult = false;
	}

	m_bResult = true;

	TxLogDebugEndMsg();
}


void KPathVolumeTerminalAccEgrDlg::GetResultSettingInfo(TInTerminalAccEgrSettingInfo &a_oTResltSettingInfo)
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

		a_oTResltSettingInfo.mapModeColor   = m_mapModeColor;

		a_oTResltSettingInfo.clrOutRange    = m_cpOutRangeColor.GetColor();
		a_oTResltSettingInfo.clrOrgZone     = m_clrOrgZone;
		a_oTResltSettingInfo.clrDestZone    = m_clrDestZone;

		a_oTResltSettingInfo.dMinTrip    = dMinTrip;
		a_oTResltSettingInfo.dMaxTrip    = dMaxTrip;
		a_oTResltSettingInfo.dMinSize    = dMinSize;
		a_oTResltSettingInfo.dMaxSize    = dMaxSize;
		if(IsDlgButtonChecked(IDC_CHECK_OUTRANGE) == BST_CHECKED)
			a_oTResltSettingInfo.bUseOutRange = TRUE;
		else
			a_oTResltSettingInfo.bUseOutRange = FALSE;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeTerminalAccEgrDlg::OnBnClickedButtonClear()
{
	if (false) { // 이전
//		AllClearPath(true);
	}
	
	/************************************************************************/
	/* ▷ Clear버튼 클릭 시 모든 체크 옵션 선택이 취소되고 선택한 Target Terminal과 Access Zone 혹은 Egress Zone만 표출되어야 함.                                                                     */
	/************************************************************************/
	// all check
	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_ALL3, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_A, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_T, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_B, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_R, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_TR, BST_UNCHECKED);

	CheckAllAcc(false);
	CheckAllEgr(true);
}


void KPathVolumeTerminalAccEgrDlg::AllClearPath(bool a_bRefresh)
{
	try
	{
		m_pMapView->TxClearCompositeThemes(a_bRefresh);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KPathVolumeTerminalAccEgrDlg::OnBnClickedCheckZoneaccess()
{
	std::vector<Integer> vecCheckedAccZone;
	GetCheckedZone(m_ctrlReportAcc, vecCheckedAccZone);
	std::vector<Integer> vecCheckedEgrZone;
	GetCheckedZone(m_ctrlReportEgr, vecCheckedEgrZone);

	if (vecCheckedAccZone.size() < 1 && vecCheckedEgrZone.size() < 1) {
		DoneDraw(false);
	}
	else {
		DoneDraw(true);
	}
}
