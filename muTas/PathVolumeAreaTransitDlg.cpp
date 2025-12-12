// PathVolumeAreaTransitDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PathVolumeAreaTransitDlg.h"
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

#include "KDBaseAccessibilityModify.h"

namespace PathVolumeAreaTransitENUM
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

using namespace PathVolumeAreaTransitENUM;
// KPathVolumeAreaTransitDlg 대화 상자입니다.

const int nZoneCharIndex = 53;
const int nZoneSize		 = 11;

IMPLEMENT_DYNAMIC(KPathVolumeAreaTransitDlg, KResizeDialogEx)

KPathVolumeAreaTransitDlg::KPathVolumeAreaTransitDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPathVolumeAreaTransitDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pProject(a_pProject)
	, m_pMapView(a_pMapView)
	, m_strRunErrMsg(_T(""))
	, m_strMultimodalTripFieldName(_T(""))
	, m_bDrawRatio(false)
	, m_dRadius(1000.0)
{
	m_dMaxVolume             = 0.0;
	m_dMinVolume             = 0.0;
	m_dMaxRatio              = 0.0;
	m_dMinRatio              = 0.0;

	m_clrBus                 = RGB (   0, 128,   0 );
	m_clrRail                = RGB (   0,   0, 255 );
	m_clrWalk                = RGB ( 102, 102, 153 );
	m_clrOutRange            = RGB ( 221, 217, 195 );
}

KPathVolumeAreaTransitDlg::~KPathVolumeAreaTransitDlg()
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

void KPathVolumeAreaTransitDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,                m_ctrlReportA);
	DDX_Control(pDX, IDC_REPORT_B,                m_ctrlReportB);
	DDX_Control(pDX, IDC_COMBO1,                  m_cboMode);
	DDX_Control(pDX, IDC_CLR_BUS,                 m_cpBusColor);
	DDX_Control(pDX, IDC_CLR_RAIL,                m_cpRailColor);
	DDX_Control(pDX, IDC_CLR_WALK,                m_cpWalkColor);
	DDX_Control(pDX, IDC_CLR_OUT,                 m_cpOutRangeColor);
}


BEGIN_MESSAGE_MAP(KPathVolumeAreaTransitDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1,      &KPathVolumeAreaTransitDlg::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON2,        &KPathVolumeAreaTransitDlg::OnBnClickedButtonSearchAreaCodeOrg)
	ON_BN_CLICKED(IDC_BUTTON4,        &KPathVolumeAreaTransitDlg::OnBnClickedButtonSearchAreaCodeDest)
	ON_BN_CLICKED(IDC_BUTTON1,        &KPathVolumeAreaTransitDlg::OnBnClickedButtonPolyDrawOrg)
	ON_BN_CLICKED(IDC_BUTTON3,        &KPathVolumeAreaTransitDlg::OnBnClickedButtonPolyDrawDest)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,              &KPathVolumeAreaTransitDlg::OnReportAItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_A, &KPathVolumeAreaTransitDlg::OnReportACheckItem)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_B,              &KPathVolumeAreaTransitDlg::OnReportBItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_B, &KPathVolumeAreaTransitDlg::OnReportBCheckItem)
	ON_BN_CLICKED(IDC_CHECK1, &KPathVolumeAreaTransitDlg::OnBnClickedCheckASelectAll)
	ON_BN_CLICKED(IDC_CHECK2, &KPathVolumeAreaTransitDlg::OnBnClickedCheckBSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH2, &KPathVolumeAreaTransitDlg::OnBnClickedButtonSearch)
	ON_WM_COPYDATA()
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO2, &KPathVolumeAreaTransitDlg::OnBnClickedRadioVolumeType)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &KPathVolumeAreaTransitDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KPathVolumeAreaTransitDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL, &KPathVolumeAreaTransitDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_OUTRANGE, &KPathVolumeAreaTransitDlg::OnBnClickedCheckOutrange)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KPathVolumeAreaTransitDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_CHECK3, &KPathVolumeAreaTransitDlg::OnBnClickedCheckZone)
END_MESSAGE_MAP()


// KPathVolumeAreaTransitDlg 메시지 처리기입니다.

void KPathVolumeAreaTransitDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
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


void KPathVolumeAreaTransitDlg::ActivatedFrameWndNotifyProcess()
{

}


void KPathVolumeAreaTransitDlg::DeActivatedFrameWndNotifyProcess()
{

}


void KPathVolumeAreaTransitDlg::ResizeComponent()
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

	SetResize(IDC_STATIC2,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO1,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO2,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_MIN_VALUE3,      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_MAX_VALUE3,	  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC22,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC23,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MIN_VALUE,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAX_VALUE,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MIN_RATIO,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAX_RATIO,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MINSIZE,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAXSIZE,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC14,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC15,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC16,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_BUS,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_RAIL,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_WALK,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
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


void KPathVolumeAreaTransitDlg::TargetRegionFeedback( std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate )
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


BOOL KPathVolumeAreaTransitDlg::OnInitDialog()
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

	KColorPickerSetting::DefaultCtrl(m_cpBusColor,      m_clrBus);
	KColorPickerSetting::DefaultCtrl(m_cpRailColor,     m_clrRail);
	KColorPickerSetting::DefaultCtrl(m_cpWalkColor,     m_clrWalk);
	KColorPickerSetting::DefaultCtrl(m_cpOutRangeColor, m_clrOutRange);

	CWaitCursor wc;

	CString strMinPathVol(_T("0.001"));
	SetDlgItemText(IDC_EDIT1, strMinPathVol);

	CheckDlgButton(IDC_CHECK1, BST_CHECKED);
	CheckDlgButton(IDC_CHECK2, BST_CHECKED);
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	HiddenMinMaxControl();
	CheckDlgButton(IDC_CHECK_LABEL, BST_UNCHECKED);
	UpdateOutRange();
	CheckDlgButton(IDC_CHECK3, BST_CHECKED);

	SetDlgItemText(IDC_EDIT_MINSIZE,   _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE,   _T("15.0"));

	GetDBModeInfo();
	GetDBModeVolumeFiledName();
	KDBasePathVolume::GetZoneNodeInfo(m_pTarget, m_mapZoneNodeData);
	SetGuideNodeInfo(m_mapZoneNodeData, m_vecDrawGuideNode);

	InitReportHeader();
	m_ctrlReportA.Populate();
	m_ctrlReportB.Populate();

	ResizeComponent();

	try
	{
		AddImChampTooltip(IDC_RADIO2, _T("비율(%) = Path Volume / Link Volume * 100"));
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}  

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


void KPathVolumeAreaTransitDlg::InitReportHeader()
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


void KPathVolumeAreaTransitDlg::GetDBModeVolumeFiledName()
{
	try
	{
		m_strMultimodalTripFieldName = _T("");
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT trip FROM assign_model WHERE run_seq in (SELECT MAX(run_seq) FROM assign_model WHERE type = '5') "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		CString strSumColumnName(_T(""));
		if( spResult->Next() )
		{
			strSumColumnName	  = spResult->GetValueString(0);
		}
		m_strMultimodalTripFieldName = strSumColumnName;
	}
	catch (...)
	{
		m_strMultimodalTripFieldName = _T("");
		TxLogDebugException();
	}
}


void KPathVolumeAreaTransitDlg::SetGuideNodeInfo(std::map<Integer, CString> &a_mapZoneNodeData, std::vector<TDrawNodeInfo> &a_vecDrawGuideNode)
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


void KPathVolumeAreaTransitDlg::GetDBModeInfo()
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
		m_mapModeInfo.insert(std::make_pair(nIDXCombo, strModeName));

		nIDXCombo++;

		std::map<int, CString> mapModeName;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			mapModeName.insert(std::make_pair(KEMTransitAssignBus,     _T("버스")));
			mapModeName.insert(std::make_pair(KEMTransitAssignRail,    _T("도시철도")));
			mapModeName.insert(std::make_pair(KEMTransitAssignBusRail, _T("버스+도시철도")));
		}
		else {
			mapModeName.insert(std::make_pair(KEMTransitAssignBus,     _T("Bus")));
			mapModeName.insert(std::make_pair(KEMTransitAssignRail,    _T("Rail")));
			mapModeName.insert(std::make_pair(KEMTransitAssignBusRail, _T("Bus+Rail")));
		}

		AutoType iter  = mapModeName.begin();
		AutoType itEnd = mapModeName.end();
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
			m_mapModeInfo.insert(std::make_pair(nIDXCombo, strModeName));

			nIDXCombo++;

			++iter;
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


int KPathVolumeAreaTransitDlg::GetSelMode()
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


void KPathVolumeAreaTransitDlg::OnCbnSelchangeComboMode()
{
	UpdateData(TRUE);

	int nSelCur = m_cboMode.GetCurSel();
	int nIdx    = m_cboMode.GetItemData(nSelCur);

	AutoType iter = m_mapModeInfo.find(nIdx);

	//ResetVolumeData();
}

void KPathVolumeAreaTransitDlg::OnBnClickedButtonSearchAreaCodeOrg()
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


void KPathVolumeAreaTransitDlg::OnBnClickedButtonSearchAreaCodeDest()
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


void KPathVolumeAreaTransitDlg::UpdateReport(CXTPReportControl &a_ctrlReport, std::set<Integer> &a_setZoneID, BOOL a_bSelectAll)
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


void KPathVolumeAreaTransitDlg::OnBnClickedButtonPolyDrawOrg()
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


void KPathVolumeAreaTransitDlg::OnBnClickedButtonPolyDrawDest()
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


void KPathVolumeAreaTransitDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	}

	ResetVolumeData();
}


void KPathVolumeAreaTransitDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK2, BST_UNCHECKED);
	}

	ResetVolumeData();
}


void KPathVolumeAreaTransitDlg::OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KPathVolumeAreaTransitDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KPathVolumeAreaTransitDlg::OnBnClickedCheckASelectAll()
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


void KPathVolumeAreaTransitDlg::OnBnClickedCheckBSelectAll()
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


void KPathVolumeAreaTransitDlg::OnBnClickedButtonSearch()
{
	try
	{
		m_nModeID = GetSelMode();
		if(DEFAULT_MODEID == m_nModeID)
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


void KPathVolumeAreaTransitDlg::GetCheckedOrgDestZone(CXTPReportControl &a_ctrlReport, int a_nCheckBoxItemIndex, std::set<Integer> &a_setSelectedZone)
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


void KPathVolumeAreaTransitDlg::ResetVolumeData()
{
	try
	{
		ClearMapDraw(false);
		DrawSelectedOrgDestination();

		InitVolMinMax();
		InitRatioMinMax();
		SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE,  _T(""));

		SetDlgItemText(IDC_EDIT_MIN_RATIO,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_RATIO,  _T(""));

		TMMapResultInputValue oEmptyObject;
		m_oMMapResultTripInfo  = oEmptyObject; // 초기화
		m_oMMapResultRatioInfo = oEmptyObject;
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


unsigned __stdcall KPathVolumeAreaTransitDlg::PathVolumeThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("대중교통 수단별 구역간 이용경로 통행량 분석 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Area-to-Area Urban Transit Path Analysis Module..."));
	}

	KPathVolumeAreaTransitDlg* pDlg = (KPathVolumeAreaTransitDlg*)pParameter->GetParameter();
	pDlg->GetSearchPathVolume();

	return 0;
}


void KPathVolumeAreaTransitDlg::GetSearchPathVolume()
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


void KPathVolumeAreaTransitDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KPathVolumeAreaTransitDlg::ReadOutPutFile()
{
	InitVolMinMax();
	InitRatioMinMax();

	try
	{
		TMMapResultInputValue oEmptyObject;
		m_oMMapResultTripInfo  = oEmptyObject; // 초기화
		m_oMMapResultRatioInfo = oEmptyObject;

		bool bReadBus    (false);
		bool bReadRail   (false);
		bool bReadBusRail(false);

		if (0 == m_nModeID)
		{
			bReadBus     = true;
			bReadRail    = true;
			bReadBusRail = true;
		}
		else if (KEMTransitAssignBus == m_nModeID)
		{
			bReadBus     = true;
		}
		else if (KEMTransitAssignRail == m_nModeID)
		{
			bReadRail    = true;
		}
		else if (KEMTransitAssignBusRail == m_nModeID)
		{
			bReadBusRail = true;
		}

		CString strDataFileName(_T(""));
		CString strSavePath(_T(""));
		std::vector<KMBusAssignInfo> vecMBusAssignTotalInfo;

		AutoType iter  = m_setSelectedZoneOrg.begin();
		AutoType itEnd = m_setSelectedZoneOrg.end();

		while (iter != itEnd)
		{
			Integer nxOrgZoneID = *iter;

			if (bReadBus)
			{
				strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_BusPathAssign.dat"), nxOrgZoneID);
				strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);

				std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>>  mapOrgBusAssign;
				ReadBusPathFile(strSavePath, m_setSelectedZoneDest, mapOrgBusAssign);

				CollectTotalBusAssignInfo(mapOrgBusAssign, vecMBusAssignTotalInfo);
			}

			if (bReadRail)
			{
				strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_RailPathAssign.dat"), nxOrgZoneID);
				strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);

				std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>>  mapOrgRailAssign;
				ReadRailPathFile(strSavePath, m_setSelectedZoneDest, mapOrgRailAssign);

				CollectTotalBusAssignInfo(mapOrgRailAssign, vecMBusAssignTotalInfo);
			}

			if (bReadBusRail)
			{
				strDataFileName.Format(_T("\\TransitPathAssign\\%I64d_BusRailPathAssign.dat"), nxOrgZoneID);
				strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, strDataFileName);

				std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>>  mapOrgBusRailAssign;
				ReadBusRailPathFile(strSavePath, m_setSelectedZoneDest, mapOrgBusRailAssign);

				CollectTotalBusAssignInfo(mapOrgBusRailAssign, vecMBusAssignTotalInfo);
			}

			++iter;
		}

		KMultiModalResultInfo::ResultBusPathTripInfo(m_pTarget, vecMBusAssignTotalInfo, m_oMMapResultTripInfo);
		SetVolMinMax(m_oMMapResultTripInfo, m_oMMapResultRatioInfo);
	}
	catch (...)
	{
		InitVolMinMax();
		InitRatioMinMax();
		TxLogDebugException();
		return false;
	}    

	return true;
}


void KPathVolumeAreaTransitDlg::CollectTotalBusAssignInfo(std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>>  &a_mapOrgTransitAssign, std::vector<KMBusAssignInfo> &a_vecMBusAssignTotalInfo)
{
	try
	{
		AutoType iter  = a_mapOrgTransitAssign.begin();
		AutoType itEnd = a_mapOrgTransitAssign.end();

		while (iter != itEnd)
		{
			std::vector<KMBusAssignInfo> &vecMBusAssignInfo = iter->second;
			size_t nxCount              = vecMBusAssignInfo.size();

			for (size_t i= 0; i< nxCount; i++)
			{
				a_vecMBusAssignTotalInfo.push_back(vecMBusAssignInfo[i]);
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaTransitDlg::ReadBusPathFile(CString strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapOrgBusAssign)
{
	TxLogDebugStartMsg();

	a_mapOrgBusAssign.clear();
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
						TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeBus, RGB(255,255,255), false};

						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.dTrip)),            sizeof(double) );

						if(0 == oTMResultBusInfo.nxTransitID)
							continue;

						vecLowResultBusInfo.push_back(oTMResultBusInfo);
					}
					vecResultBusInfo.push_back(oTIndexResultTransitInfo);
				}

				ifs.read( reinterpret_cast<char*>( &(oBusAssignInfo.oTBusSubData)),               sizeof(TBusSubData) );
#pragma endregion 정보 취합

				if (oBusAssignInfo.oTBusSubData.dAssignTrip <= m_dMinPathVol)
				{
					continue;
				}

				if (a_setSelectedZoneDest.find(oBusAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
				{
					continue;
				}

#pragma region 취합 정보 등록
				AutoType iter  = a_mapOrgBusAssign.find(oBusAssignInfo.nxDestinationID);
				AutoType itEnd = a_mapOrgBusAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oBusAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oBusAssignInfo);
					a_mapOrgBusAssign.insert(std::make_pair(oBusAssignInfo.nxDestinationID, vecBusAssignInfo));
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
		a_mapOrgBusAssign.clear();
		ifs.close();
		m_bResultRun   = false;
		m_strRunErrMsg =_T("ReadBusPathFile 오류");
		/*ThrowException(_T("ReadBusPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaTransitDlg::ReadRailPathFile(CString strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapOrgRailAssign)
{
	TxLogDebugStartMsg();

	a_mapOrgRailAssign.clear();
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
					oTIndexResultTransitInfo.oIndexKey = KODKey((1+i),0);
					std::vector<TMResultBusInfo> &vecLowResultBusInfo = oTIndexResultTransitInfo.vecLowResultTransitInfo;
					for (int k= 0; k< nDuplicateCnt; k++)
					{
						TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeRail, RGB(255,255,255), false};

						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.dTrip)),            sizeof(double) );

						if(0 == oTMResultBusInfo.nxTransitID)
							continue;

						vecLowResultBusInfo.push_back(oTMResultBusInfo);
					}
					vecResultBusInfo.push_back(oTIndexResultTransitInfo);
				}

				ifs.read( reinterpret_cast<char*>( &(oRailAssignInfo.oTRailSubData)),             sizeof(TRailSubData) );
#pragma endregion 정보 취합

				if (oRailAssignInfo.oTRailSubData.dAssignTrip <= m_dMinPathVol)
				{
					continue;
				}
				if (a_setSelectedZoneDest.find(oRailAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
				{
					continue;
				}

#pragma region 취합 정보 등록
				AutoType iter  = a_mapOrgRailAssign.find(oRailAssignInfo.nxDestinationID);
				AutoType itEnd = a_mapOrgRailAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oRailAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oRailAssignInfo);
					a_mapOrgRailAssign.insert(std::make_pair(oRailAssignInfo.nxDestinationID, vecBusAssignInfo));
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
		a_mapOrgRailAssign.clear();
		ifs.close();
		m_bResultRun   = false;
		m_strRunErrMsg =_T("ReadRailPathFile 오류");
		/*ThrowException(_T("ReadRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaTransitDlg::ReadBusRailPathFile(CString strFileName, std::set<Integer> &a_setSelectedZoneDest, std::map<Integer/*D존ID*/, std::vector<KMBusAssignInfo>> &a_mapOrgBusRailAssign)
{
	TxLogDebugStartMsg();

	a_mapOrgBusRailAssign.clear();

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
						TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeBus, RGB(255,255,255), false};

						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
						ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.dTrip)),            sizeof(double) );

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

				ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTBusSubData)),           sizeof(TBusSubData) );
#pragma endregion 버스 정보 취합

				std::vector< std::vector< TIndexResultTransitInfo > > vecMultiResultRailInfo;
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
								TMResultBusInfo oTMResultBusInfo = {0,0,0,0.0, KEMPathTypeRail, RGB(255,255,255), false};

								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nxTransitID)),      sizeof(Integer) );
								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nStartStation)),    sizeof(int) );
								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.nEndStation)),      sizeof(int) );
								ifs.read( reinterpret_cast<char*>( &(oTMResultBusInfo.dTrip)),            sizeof(double) );

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

						ifs.read( reinterpret_cast<char*>( &(oBusRailAssignInfo.oTRailSubData)),             sizeof(TRailSubData) ); // 맨 마지막의 RailSubData 저장

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

				if (oBusRailAssignInfo.oTBusSubData.dAssignTrip <= m_dMinPathVol) // tree에 Bus의 AssignTrip이 표출 되는 것으로 보아 조건식에 oTBusSubData.dAssignTrip 사용
				{
					continue;
				}

				if (a_setSelectedZoneDest.find(oBusRailAssignInfo.nxDestinationID) == a_setSelectedZoneDest.end()) // 선택한 도착지점만..
				{
					continue;
				}

#pragma region 취합 정보 등록
				AutoType iter  = a_mapOrgBusRailAssign.find(oBusRailAssignInfo.nxDestinationID);
				AutoType itEnd = a_mapOrgBusRailAssign.end();

				if( iter != itEnd )
				{
					std::vector<KMBusAssignInfo> &vecBusAssignInfo = iter->second;
					vecBusAssignInfo.push_back(oBusRailAssignInfo);
				}
				else
				{
					std::vector<KMBusAssignInfo> vecBusAssignInfo;
					vecBusAssignInfo.push_back(oBusRailAssignInfo);
					a_mapOrgBusRailAssign.insert(std::make_pair(oBusRailAssignInfo.nxDestinationID, vecBusAssignInfo));
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
		a_mapOrgBusRailAssign.clear();
		ifs.close();
		m_bResultRun   = false;
		m_strRunErrMsg =_T("ReadBusRailPathFile 오류");
		/*ThrowException(_T("ReadBusRailPathFile 오류"));*/
		return;
	}    

	TxLogDebugEndMsg();
}


void KPathVolumeAreaTransitDlg::InitVolMinMax()
{
	m_dMaxVolume  = 0.0;
	m_dMinVolume  = 0.0;
}


void KPathVolumeAreaTransitDlg::SetVolMinMax(TMMapResultInputValue &a_oMMapResultTripInfo, TMMapResultInputValue &a_oMMapResultRatioInfo)
{
	if (a_oMMapResultTripInfo.mapBusLinkTrip.size() < 1 && a_oMMapResultTripInfo.mapSubwayLinkTrip.size() < 1)
	{
		InitVolMinMax();
		InitRatioMinMax();
		return;
	}

	a_oMMapResultRatioInfo = a_oMMapResultTripInfo;

	double dMaxVolume = -999999999999.0;
	double dMinVolume = 999999999999.0;

	double dMaxRatio = -999999999999.0;
	double dMinRatio = 999999999999.0;
	double dRatio(0.0);

	std::map<Integer, double> mapAllLinkModeVolume;
	GetModeVolume(m_strMultimodalTripFieldName, mapAllLinkModeVolume);
	
	//Bus
	a_oMMapResultRatioInfo.mapBusLinkTrip.clear();
	AutoType iterBus = a_oMMapResultTripInfo.mapBusLinkTrip.begin();
	AutoType endBus  = a_oMMapResultTripInfo.mapBusLinkTrip.end();
	while (iterBus != endBus)
	{
		Integer nxLinkID = iterBus->first;
		double &dTrip    = iterBus->second;

#pragma region dMinVolume, dMaxVolume 구하기
		if (dTrip >= 0)
		{
			if (dMinVolume > dTrip)
			{
				dMinVolume = dTrip;
			}
		}

		if (dMaxVolume < dTrip)
		{
			dMaxVolume = dTrip;
		}
#pragma endregion dMinVolume, dMaxVolume 구하기

		dRatio = 0.0;
#pragma region dMinRatio, dMaxRatio 구하기
		AutoType iterMVol = mapAllLinkModeVolume.find(nxLinkID);
		if (iterMVol != mapAllLinkModeVolume.end())
		{
			double &dModeVolume = iterMVol->second;
			if (dModeVolume <= 0.0)
			{
				dRatio = 0.0;
			}
			else
			{
				dRatio = (dTrip/dModeVolume) * 100.0;
			}
		}

		if (dRatio >= 0.0)
		{
			if (dMinRatio > dRatio)
			{
				dMinRatio = dRatio;
			}
		}

		if (dMaxRatio < dRatio)
		{
			dMaxRatio = dRatio;
		}
#pragma endregion dMinRatio, dMaxRatio 구하기

		a_oMMapResultRatioInfo.mapBusLinkTrip.insert(std::make_pair(nxLinkID, dRatio));

		++iterBus;
	}

	//Rail
	a_oMMapResultRatioInfo.mapSubwayLinkTrip.clear();
	AutoType iterRail = a_oMMapResultTripInfo.mapSubwayLinkTrip.begin();
	AutoType endRail  = a_oMMapResultTripInfo.mapSubwayLinkTrip.end();
	while (iterRail != endRail)
	{
		Integer nxLinkID = iterRail->first;
		double &dTrip    = iterRail->second;

#pragma region dMinVolume, dMaxVolume 구하기
		if (dTrip >= 0)
		{
			if (dMinVolume > dTrip)
			{
				dMinVolume = dTrip;
			}
		}

		if (dMaxVolume < dTrip)
		{
			dMaxVolume = dTrip;
		}
#pragma endregion dMinVolume, dMaxVolume 구하기

		dRatio = 0.0;
#pragma region dMinRatio, dMaxRatio 구하기
		AutoType iterMVol = mapAllLinkModeVolume.find(nxLinkID);
		if (iterMVol != mapAllLinkModeVolume.end())
		{
			double &dModeVolume = iterMVol->second;
			if (dModeVolume <= 0.0)
			{
				dRatio = 0.0;
			}
			else
			{
				dRatio = (dTrip/dModeVolume) * 100.0;
			}
		}

		if (dRatio >= 0.0)
		{
			if (dMinRatio > dRatio)
			{
				dMinRatio = dRatio;
			}
		}

		if (dMaxRatio < dRatio)
		{
			dMaxRatio = dRatio;
		}
#pragma endregion dMinRatio, dMaxRatio 구하기

		a_oMMapResultRatioInfo.mapSubwayLinkTrip.insert(std::make_pair(nxLinkID, dRatio));

		++iterRail;
	}

	std::map<KODKey, double> mapODVolume;

	// 버스에서 버스로 환승 도보
	a_oMMapResultRatioInfo.mapTransferWalkingB2BTrip.clear();
	AutoType iterB2BWalking = a_oMMapResultTripInfo.mapTransferWalkingB2BTrip.begin();
	AutoType endB2BWalking  = a_oMMapResultTripInfo.mapTransferWalkingB2BTrip.end();
	while (iterB2BWalking != endB2BWalking)
	{
		KODKey  oODKey   = iterB2BWalking->first;
		double &dTrip    = iterB2BWalking->second;

		mapODVolume.insert(std::make_pair(oODKey, dTrip));

		a_oMMapResultRatioInfo.mapTransferWalkingB2BTrip.insert(std::make_pair(oODKey, 0.0));

		++iterB2BWalking;
	}

	// 버스에서 전철로 환승 도보
	a_oMMapResultRatioInfo.mapTransferWalkingB2STrip.clear();
	AutoType iterB2SWalking = a_oMMapResultTripInfo.mapTransferWalkingB2STrip.begin();
	AutoType endB2SWalking  = a_oMMapResultTripInfo.mapTransferWalkingB2STrip.end();
	while (iterB2SWalking != endB2SWalking)
	{
		KODKey  oODKey   = iterB2SWalking->first;
		double &dTrip    = iterB2SWalking->second;

		mapODVolume.insert(std::make_pair(oODKey, dTrip));

		a_oMMapResultRatioInfo.mapTransferWalkingB2STrip.insert(std::make_pair(oODKey, 0.0));

		++iterB2SWalking;
	}

	//전철에서 전철로
	a_oMMapResultRatioInfo.mapTransferWalkingS2STrip.clear();
	AutoType iterS2SWalking = a_oMMapResultTripInfo.mapTransferWalkingS2STrip.begin();
	AutoType endS2SWalking  = a_oMMapResultTripInfo.mapTransferWalkingS2STrip.end();
	while (iterS2SWalking != endS2SWalking)
	{
		KODKey  oODKey   = iterS2SWalking->first;
		double &dTrip    = iterS2SWalking->second;

		AutoType iter = mapODVolume.find(oODKey);
		if (iter != mapODVolume.end())
		{
			double &dSumVolume = iter->second;
			dSumVolume += dTrip;
		}
		else
		{
			mapODVolume.insert(std::make_pair(oODKey, dTrip));
		}

		a_oMMapResultRatioInfo.mapTransferWalkingS2STrip.insert(std::make_pair(oODKey, 0.0));

		++iterS2SWalking;
	}

	AutoType iter     = mapODVolume.begin();
	AutoType itEnd    = mapODVolume.end();

	double dVolume(0.0);
	while (iter != itEnd)
	{
		dVolume = iter->second;

#pragma region dMinVolume, dMaxVolume 구하기
		if (dVolume >= 0)
		{
			if (dMinVolume > dVolume)
			{
				dMinVolume = dVolume;
			}
		}

		if (dMaxVolume < dVolume)
		{
			dMaxVolume = dVolume;
		}
#pragma endregion dMinVolume, dMaxVolume 구하기

		++iter;
	}

	m_dMaxVolume = dMaxVolume;
	m_dMinVolume = dMinVolume;

	m_dMaxRatio  = dMaxRatio;
	m_dMinRatio  = dMinRatio;
}


void KPathVolumeAreaTransitDlg::InitRatioMinMax()
{
	m_dMaxRatio = 0.0;
	m_dMinRatio = 0.0;
}


void KPathVolumeAreaTransitDlg::GetModeVolume(CString &a_strFieldName, std::map<Integer, double> &a_mapAllLinkModeVolume)
{
	try
	{
		a_mapAllLinkModeVolume.clear();
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT link_id, %s FROM %s "), a_strFieldName, TABLE_LINK);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		Integer nxLinkID(0);
		double  dVolume (0.0);
		while( spResult->Next() )
		{
			nxLinkID          = spResult->GetValueInt64 (0);
			dVolume     	  = spResult->GetValueDouble(1);

			a_mapAllLinkModeVolume.insert(std::make_pair(nxLinkID, dVolume));
		}
	}
	catch (...)
	{
		a_mapAllLinkModeVolume.clear();
		TxLogDebugException();	
	}
}


void KPathVolumeAreaTransitDlg::UpdateMinMax()
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
		/*strMinVol.Format(_T("%f"), dMinVol);*/
		GetDlgItemText(IDC_EDIT1, strMinVol);
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

	CString strMinRatio(_T(""));
	CString strMaxRatio(_T(""));

	double dMinRatio(0.0);
	dMinRatio = m_dMinRatio - 0.5;
	if(dMinRatio < 0.0)
		dMinRatio = 0.0;

	if (dMinRatio < m_dMinRatio)
	{
		dMinRatio = m_dMinRatio;
	}

	int nMinRatio = (int)dMinRatio;
	dMinRatio     = (double)nMinRatio; // double 버림 처리
	strMinRatio.Format(_T("%.0f"), dMinRatio);

	double dMaxRatio(0.0);
	if (0.0 == m_dMaxRatio)
	{
		dMaxRatio = m_dMaxRatio;
	}
	else
	{
		dMaxRatio = m_dMaxRatio + 0.6;
	}

	strMaxRatio.Format(_T("%.0f"), dMaxRatio);

	/*SetDlgItemText(IDC_EDIT_MIN_RATIO,  strMinRatio);*/
	SetDlgItemText(IDC_EDIT_MIN_RATIO,  _T("0.0"));
	SetDlgItemText(IDC_EDIT_MAX_RATIO,  strMaxRatio);
}


void KPathVolumeAreaTransitDlg::HiddenMinMaxControl()
{
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_EDIT_MAX_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_MIN_RATIO)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_MAX_RATIO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MIN_RATIO)->ShowWindow(SW_SHOW);
	}
}


void KPathVolumeAreaTransitDlg::OnBnClickedRadioVolumeType(UINT nID)
{
	HiddenMinMaxControl();

	ClearMapDraw(false);
	DrawSelectedOrgDestination();
}


void KPathVolumeAreaTransitDlg::OnBnClickedButtonDraw()
{
	try
	{
		if (m_oMMapResultTripInfo.mapBusLinkTrip.size() < 1 && m_oMMapResultTripInfo.mapSubwayLinkTrip.size() < 1)
		{
			AfxMessageBox(_T("경로 분석 결과가 존재하지 않습니다."));
			return;
		}

		if (UserVolumeInputCheck() == false)
		{
			return;
		}

		CWaitCursor cw;
		GetResultSettingInfo(m_oTMResltSettingInfo);

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
			m_bDrawRatio = false;
		else
			m_bDrawRatio = true;

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


bool KPathVolumeAreaTransitDlg::UserVolumeInputCheck()
{
	try
	{
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize(0.0),  dMaxSize(0.0);

		CString strValue(_T(""));

		int nMinID;
		int nMaxID;

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			nMinID = IDC_EDIT_MIN_VALUE;
			nMaxID = IDC_EDIT_MAX_VALUE;
		}
		else
		{
			nMinID = IDC_EDIT_MIN_RATIO;
			nMaxID = IDC_EDIT_MAX_RATIO;
		}

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


void KPathVolumeAreaTransitDlg::GetResultSettingInfo(TMResultSettingInfo &a_oTMResltSettingInfo)
{
	try
	{
		int nMinID;
		int nMaxID;
		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			nMinID = IDC_EDIT_MIN_VALUE;
			nMaxID = IDC_EDIT_MAX_VALUE;
		}
		else
		{
			nMinID = IDC_EDIT_MIN_RATIO;
			nMaxID = IDC_EDIT_MAX_RATIO;
		}

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

		a_oTMResltSettingInfo.clrBus      = m_cpBusColor.GetColor();
		a_oTMResltSettingInfo.clrRail     = m_cpRailColor.GetColor();
		a_oTMResltSettingInfo.clrWalk     = m_cpWalkColor.GetColor();
		a_oTMResltSettingInfo.clrOutRange = m_cpOutRangeColor.GetColor();
		a_oTMResltSettingInfo.dMinTrip    = dMinTrip;
		a_oTMResltSettingInfo.dMaxTrip    = dMaxTrip;
		a_oTMResltSettingInfo.dMinSize    = dMinSize;
		a_oTMResltSettingInfo.dMaxSize    = dMaxSize;
		if(IsDlgButtonChecked(IDC_CHECK_OUTRANGE) == BST_CHECKED)
			a_oTMResltSettingInfo.bUseOutRange = TRUE;
		else
			a_oTMResltSettingInfo.bUseOutRange = FALSE;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


unsigned __stdcall KPathVolumeAreaTransitDlg::MakeDrawDataThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("지도 그리기 데이터 생성 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Loading Data for Drawing..."));
	}

	KPathVolumeAreaTransitDlg*     pDlg = (KPathVolumeAreaTransitDlg*)pParameter->GetParameter();
	pDlg->MakeDrawData();

	return 0;
}


void KPathVolumeAreaTransitDlg::MakeDrawData()
{
	TxLogDebugStartMsg();
	try
	{
		if (false == m_bDrawRatio)
			KMultiModalResultInfo::ResultBusPathDrawInfo(m_pTarget, m_oMMapResultTripInfo, m_oTMResltSettingInfo, m_oMapResult);
		else
			KMultiModalResultInfo::ResultBusPathDrawInfo(m_pTarget, m_oMMapResultRatioInfo, m_oTMResltSettingInfo, m_oMapResult, true);
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



void KPathVolumeAreaTransitDlg::ClearMapDraw(bool bRefresh)
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


void KPathVolumeAreaTransitDlg::OnBnClickedButtonClear()
{
	ClearMapDraw(true);
	DrawSelectedOrgDestination();
}


void KPathVolumeAreaTransitDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KPathVolumeAreaTransitDlg::CloseWindowProc()
{
	ClearMapDraw(true);
}


void KPathVolumeAreaTransitDlg::OnBnClickedCheckLabel()
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


void KPathVolumeAreaTransitDlg::DrawSelectedOrgDestination(bool bRefresh)
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


void KPathVolumeAreaTransitDlg::GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear)
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

void KPathVolumeAreaTransitDlg::OnBnClickedCheckZone()
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


void KPathVolumeAreaTransitDlg::OnBnClickedCheckOutrange()
{
	UpdateOutRange();
}


void KPathVolumeAreaTransitDlg::UpdateOutRange()
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