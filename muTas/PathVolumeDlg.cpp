// PathVolumeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "PathVolumeDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Object.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
#include "MapView.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
#include "KEMNode.h"
//#include "StringChecker.h"
//^#include "DBaseConnector.h"
#include <fstream>
#include <iostream>
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
//^#include "MunsellColorCircle.h"
#include "KExRecordItem.h"
#include "ImChampFrameWindow.h"
#include "DBaseAssignment.h"
#include "InspectionNetwork.h"

#include "Launcher.h"
#include "WriteCodeAgrument.h"

#include "NodeSelectionInfo.h"
#include "ImChampFileExist.h"

#include "ImChampMapDefine.h"
#include "ImChampDir.h"

enum
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_PATH_NO,
	_2_COLUMN_VOLUME,
	_3_COLUMN_WIDTH,
	_4_COLUMN_DISTANCE,
	_5_COLUMN_COLOR
};

#define WIDTH_COMBO_INITVALUE     1.0	/* 'width combo'  init value */
#define WIDTH_COMBO_INCREMENT     1.0	/* 'width combo'  increment */
#define WIDTH_COMBO_DATACOUNT     10	/* 'width combo'  data count */	
#define WIDTH_COMBO_INITINDEX     0	    /* 'width combo'  init index */	

#define INCREMENT_COMBO_INITVALUE 0.0	/* 'increment combo'  init value */
#define INCREMENT_COMBO_INCREMENT 0.5	/* 'increment combo'  increment */
#define INCREMENT_COMBO_DATACOUNT 11	/* 'increment combo'  data count */	
#define INCREMENT_COMBO_INITINDEX 1	    /* 'increment combo'  init index */	

using namespace std;

// KPathVolumeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPathVolumeDlg, KResizeDialogEx)

bool KPathVolumeDlg::m_bRunPathVolumeSucceess = false;

KPathVolumeDlg::KPathVolumeDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPathVolumeDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pProject(a_pProject)
	, m_pMapView(a_pMapView)
	, m_strRunErrMsg(_T(""))
	, m_dMinPathVol(0.001)
	, m_dMaxBushVol(0.0)
	, m_dMinBushVol(0.0)
	, m_dMaxGogumaVol(0.0)
	, m_dMinGogumaVol(0.0)
{
	try
	{
		m_bResearch				= true;
		m_nxStartNodeID         = DEFAULT_NODEID;
		m_nxEndNodeID           = DEFAULT_NODEID;
		m_nModeID               = DEFAULT_MODEID;
		m_nModeCount            = 0;
		m_nRunOption            = -1;
		m_nZoneThrough          = -1;

		m_clrBushVol   = RGB(255,   0,   0);
		m_clrGogumaVol = RGB(  0, 255,   0);
		m_clrPathVol   = RGB(  0,   0, 255);

		m_strPreSelectedModeName = _T("");

		m_oStartNodeIdCaption.TID = _T("-1");
		m_oEndNodeIdCaption.TID   = _T("-1");

		m_pModeODTable          = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);	
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

KPathVolumeDlg::~KPathVolumeDlg()
{
	CloseWindowProc();
	m_backBrush.DeleteObject();
	m_pModeODTable->Unregister(this);
}

void KPathVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO_WidthB,			m_cboWidth);
	DDX_Control(pDX, IDC_COMBO_WIDTHP,			m_cboIncrement);
	DDX_Control(pDX, IDC_COMBO1, m_cboMode);

	DDX_Control(pDX, IDC_CLR_BUSH,              m_cpBushColor );
	DDX_Control(pDX, IDC_CLR_GOGU,              m_cpGogumaColor );
	DDX_Control(pDX, IDC_CLR_PATH,              m_cpPathColor );
}


BEGIN_MESSAGE_MAP(KPathVolumeDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, &KPathVolumeDlg::OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KPathVolumeDlg::OnReportCheckItem)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &KPathVolumeDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &KPathVolumeDlg::OnBnClickedButtonDraw)

	ON_CBN_SELCHANGE(IDC_COMBO_WidthB, &KPathVolumeDlg::OnCbnSelchangeComboWidthb)
	ON_CBN_SELCHANGE(IDC_COMBO_WIDTHP, &KPathVolumeDlg::OnCbnSelchangeComboWidthp)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KPathVolumeDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL, &KPathVolumeDlg::OnBnClickedCancel)
	ON_CPN_XTP_SELENDOK(IDC_CLR_PATH, &KPathVolumeDlg::OnSelEndOkPathClr)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON2, &KPathVolumeDlg::OnBnClickedButtonDrawBushVol)
	ON_BN_CLICKED(IDC_BUTTON1, &KPathVolumeDlg::OnBnClickedButtonClearBushVol)
	ON_BN_CLICKED(IDC_BUTTON4, &KPathVolumeDlg::OnBnClickedButtonDrawGogumaVol)
	ON_BN_CLICKED(IDC_BUTTON3, &KPathVolumeDlg::OnBnClickedButtonClearGogumaVol)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KPathVolumeDlg::OnCbnSelchangeComboMode)
	ON_EN_CHANGE(IDC_EDIT1, &KPathVolumeDlg::OnEnChangeEditPathVolume)

	ON_BN_CLICKED(IDC_CHECK_LABEL_BUSH, &KPathVolumeDlg::OnBnClickedCheckLabelBush)
	ON_BN_CLICKED(IDC_CHECK_LABEL_GOGUMA, &KPathVolumeDlg::OnBnClickedCheckLabelGoguma)
	ON_BN_CLICKED(IDC_CHECK_LABEL_PATH, &KPathVolumeDlg::OnBnClickedCheckLabelPath)

	ON_BN_CLICKED(IDC_BUTTON6, &KPathVolumeDlg::OnBnClickedButtonStartNodeSearch)
	ON_BN_CLICKED(IDC_BUTTON7, &KPathVolumeDlg::OnBnClickedButtonEndNodeSearch)

	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK1, &KPathVolumeDlg::OnBnClickedCheckSelectAll)
END_MESSAGE_MAP()


KMapView* KPathVolumeDlg::GetMapView(KTarget* a_pTarget)
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


void KPathVolumeDlg::ActivatedFrameWndNotifyProcess()
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


void KPathVolumeDlg::DeActivatedFrameWndNotifyProcess()
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


void KPathVolumeDlg::CloseWindowProc()
{
	AllClearShortestPath();

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();
}


void KPathVolumeDlg::AllClearShortestPath()
{
	try
	{
		ClearMapDraw(true);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampIDFind::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				Integer nxZoneID = m_spImTasIDFind->GetSelectedID();
				CString strID(_T(""));
				strID.Format(_T("%I64d"), nxZoneID);

				CString    strNodeView(_T(""));
				KIDCaption oIDCaption;
				SetNodeViewData(m_mapZoneNodeData, strID, strNodeView, oIDCaption);

				if (MAP_NODE_START == m_bSelectMapNode)
				{
					m_oStartNodeIdCaption = oIDCaption;
					SetDlgItemText(IDC_EDIT3,  strNodeView);

					ResetVolumeData();
					ZoomToNodeFeature(nxZoneID);
				}
				else
				{
					m_oEndNodeIdCaption = oIDCaption;
					SetDlgItemText(IDC_EDIT4,  strNodeView);

					AutoType iter  = m_mapPathVolumeInfo.find(nxZoneID);
					AutoType itEnd = m_mapPathVolumeInfo.end();

					if (iter != itEnd)
					{
						ZoomToNodeFeature(nxZoneID);

						UpdateGogumaVolMinMax(nxZoneID);
						ActivateResultButton();
						InsertReportData(nxZoneID);
						SetDefaultWidthInfo();

						CheckDlgButton(IDC_CHECK_GOGUMA, BST_UNCHECKED);
						CheckDlgButton(IDC_CHECK_PATH,   BST_UNCHECKED);
						DrawVolumeMap();
					}
					else
					{
						AfxMessageBox(_T("기점 존에서 입력한 존까지의 경로 분석 결과가 존재하지 않습니다."));
					}
				}
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KPathVolumeDlg::SetNodeViewData(KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
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


void KPathVolumeDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		//AfxMessageBox(a_strSubjectName);
		CString strSubjectName(a_strSubjectName);

		if(strSubjectName.CompareNoCase(TABLE_MODE_OD) == 0)
		{
			GetDBModeInfo();
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


BOOL KPathVolumeDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWaitCursor wc;

	CString strMinPathVol(_T("0.001"));
	SetDlgItemText(IDC_EDIT1, strMinPathVol);

	EnableResultButton();
	GetZoneNodeInfo();
	GetDBModeInfo();
	GetDBRunOption();

	/*KReportCtrlSetting::DefaultB(m_wndReportCtrl, TRUE);*/
	KReportCtrlSetting::DefaultC(m_wndReportCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);
	KReportCtrlSetting::AllowColumnsort(m_wndReportCtrl);

	KColorPickerSetting::DefaultCtrl(m_cpBushColor,     m_clrBushVol);
	KColorPickerSetting::DefaultCtrl(m_cpGogumaColor,   m_clrGogumaVol);
	KColorPickerSetting::DefaultCtrl(m_cpPathColor,     m_clrPathVol);

	SetDlgItemText(IDC_EDIT_MINSIZE,   _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE,   _T("15.0"));
	SetDlgItemText(IDC_EDIT_MINSIZE2,  _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE2,  _T("5.0"));
	
	CheckDlgButton(IDC_CHECK_LABEL_BUSH,   BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_LABEL_GOGUMA, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_LABEL_PATH,   BST_UNCHECKED);

	CheckDlgButton(IDC_CHECK1, BST_CHECKED);

	InitReportHeader();
	m_wndReportCtrl.Populate();

	InitWidthValCombos();

	ResizeComponent();

	m_backBrush.CreateSolidBrush(RGB(255,255,255));

	m_pModeODTable->Register(this);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPathVolumeDlg::ResizeComponent()
{
	SetResize(IDC_EDIT3,			      SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON6,			      SZ_TOP_RIGHT,  SZ_TOP_RIGHT);

	SetResize(IDC_STATIC3,			      SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(IDC_EDIT1,			      SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR8,			  SZ_TOP_LEFT,   SZ_TOP_RIGHT);			// 가로 라인
	SetResize(IDC_SEPRATOR5,			  SZ_TOP_LEFT,   SZ_TOP_RIGHT);			// 가로 라인
	SetResize(IDC_BUTTON_SEARCH,		  SZ_TOP_RIGHT,  SZ_TOP_RIGHT);

	SetResize(IDC_EDIT4,			      SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON7,			      SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR7,			  SZ_TOP_LEFT,   SZ_TOP_RIGHT);			// 가로 라인

	SetResize(IDC_CHECK_LABEL_BUSH,       SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_LABEL_GOGUMA,     SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_LABEL_PATH,       SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_REPORT,				  SZ_TOP_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR3,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);			// 가로 라인

	SetResize(IDC_BUTTON_DRAW,		      SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CLEAR,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_PATH,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK1,	    		  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);

	SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KPathVolumeDlg::EnableResultButton()
{
	((CButton*)GetDlgItem(IDC_COMBO_WidthB))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_COMBO_WIDTHP))->EnableWindow(FALSE);

	UpdateData(FALSE);
}


void KPathVolumeDlg::ActivateResultButton()
{
	((CButton*)GetDlgItem(IDC_COMBO_WidthB))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_COMBO_WIDTHP))->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void KPathVolumeDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,      _T("Select"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_PATH_NO,   _T("Path no."), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("번호"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_VOLUME,    _T("Volume"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("교통량"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_WIDTH,    _T("Width"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("두께"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_DISTANCE,   _T("Distance"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("거리"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_5_COLUMN_COLOR,	   _T("Color"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("색상"));
    }
}


bool KPathVolumeDlg::GetDBRunOption()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format (_T(" SELECT run_option, avrtime_column, zone_through FROM assign_path_model "));

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
		return false;

	CString strColumnName(_T(""));
	while( spResult->Next() )
	{
		m_nRunOption	  = spResult->GetValueInt (0);
		strColumnName     = spResult->GetValueString(1);
		m_nZoneThrough    = spResult->GetValueInt(2);
	}

	KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTable*          pTable        = pIOTables->FindTable(TABLE_LINK);

	const KIOColumns* pColumns = pTable->Columns();
	int nIndex =       pColumns->GetColumnIndex(strColumnName);

	KIOColumn* pColumn_LinkTime = NULL;
	if(nIndex < 0)
		pColumn_LinkTime = NULL;
	else
		pColumn_LinkTime = pColumns->GetColumn(nIndex);

	if(NULL != pColumn_LinkTime)
		m_oUserColumnsLink.AddNewColumn(pColumn_LinkTime);

	return true;
}


bool KPathVolumeDlg::GetDBModeInfo()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL(_T(""));
	strSQL.Format (_T(" SELECT TBL1.mode_id AS mode_id, TBL2.mode_name AS mode_name ")
		_T(" FROM (SELECT mode_id FROM assign_path_input where preload = 0)TBL1 ")
		_T(" LEFT OUTER JOIN (SELECT detail_mode_id, mode_name FROM detail_mode)TBL2 ")
		_T(" ON TBL1.mode_id = TBL2.detail_mode_id "));

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
		return false;

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
	m_nModeCount        = 0;

	int nPreSelectedIndex = -1;
	//int nCur              = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
	int nCur = 0;
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

	if( m_strPreSelectedModeName.CompareNoCase(strModeName) == 0 )
	{
		nPreSelectedIndex = nCur;
	}

	nIDXCombo++;

	while( spResult->Next() )
	{
		nModeID	    = spResult->GetValueInt (0);
		strModeName = spResult->GetValueString(1);

		strView.Format(_T("%d: %s"), nIDXCombo, strModeName);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strView.Format(_T("%s"), strModeName);
		}
		nCur = m_cboMode.AddString(strView);
		m_cboMode.SetItemData( nCur, nIDXCombo );
		m_mapModeInfo.insert(std::make_pair(nIDXCombo, strModeName));

		if( m_strPreSelectedModeName.CompareNoCase(strModeName) == 0 )
		{
			nPreSelectedIndex = nCur;
		}
	
		m_nModeCount++;
		nIDXCombo++;
	}

	if(m_nModeCount < 1)
	{
		m_cboMode.ResetContent();
		m_cboMode.Clear();
		m_mapModeInfo.clear();

		//int nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
		int nCur = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG_K);
		}
		else {
			nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
		}
		m_cboMode.SetItemData( nCur, DEFAULT_MODEID );
	}
	else
	{
		if (nPreSelectedIndex >= 0)
		{
			m_cboMode.SetCurSel(nPreSelectedIndex);
		}
		else
		{
			m_cboMode.SetCurSel(1);	
		}
	}

	UpdateData(FALSE);

	return true;
}


bool KPathVolumeDlg::GetZoneNodeInfo()
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T(" SELECT zone_id, name FROM %s ORDER BY zone_id "), TABLE_ZONE);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return false;
	}

	m_mapZoneNodeData.clear();
	m_vecZoneNodeIDCaption.clear();

	Integer nxNodeId		= 0;
	CString strNodeID(_T(""));
	CString strNodeName		= _T("");

	while( spResult->Next() )
	{
		nxNodeId	    = spResult->GetValueInt64 (0);
		strNodeName     = spResult->GetValueString(1);

		m_mapZoneNodeData.insert(std::make_pair(nxNodeId, strNodeName));

		KIDCaption oIDCaption;
		strNodeID.Format(_T("%I64d"), nxNodeId);
		oIDCaption.TID      = strNodeID;
		oIDCaption.TCaption = strNodeName;

		m_vecZoneNodeIDCaption.push_back(oIDCaption);
	}

	UpdateData(FALSE);

	return true;
}


void KPathVolumeDlg::UpdateDZoneCombo()
{
	try
	{
		m_mapEndZoneNodeData.clear();
		m_vecEndZoneNodeIDCaption.clear();

		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));

		AutoType iter  = m_mapPathVolumeInfo.begin();
		AutoType itEnd = m_mapPathVolumeInfo.end();

		while(iter != itEnd)
		{
			nxNodeId = iter->first;

			AutoType iterB = m_mapZoneNodeData.find(nxNodeId);
			AutoType itEnd = m_mapZoneNodeData.end();

			strNodeName		= _T("");
			if (iterB != itEnd)
			{
				strNodeName = iterB->second;
			}

			m_mapEndZoneNodeData.insert(std::make_pair(nxNodeId, strNodeName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxNodeId);
			oIDCaption.TID      = strNodeID;
			oIDCaption.TCaption = strNodeName;

			m_vecEndZoneNodeIDCaption.push_back(oIDCaption);

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	UpdateData(FALSE);
}


void KPathVolumeDlg::SetNodeViewData( const Integer &a_nxNodeID, CString &a_strNodeName, CString &a_strNodeViewData )
{
	ConvertEmptyNodeName(a_strNodeName);

	a_strNodeViewData.Format(_T("%I64d [%s]"), a_nxNodeID, a_strNodeName);
}


void KPathVolumeDlg::ConvertEmptyNodeName( CString &a_strNodeName )
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KPathVolumeDlg::OnBnClickedButtonSearch()
{
	m_nResultCode = DLL_UNEXPECTED_ERROR;

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

	m_nxStartNodeID = GetSelStartNodeId();
	if(DEFAULT_NODEID == m_nxStartNodeID)
	{
		AfxMessageBox(_T("기점을 선택해 주세요."));
		return;
	}

	if( m_nRunOption < 0 || m_nZoneThrough < 0 || m_oUserColumnsLink.ColumnCount() < 1 )
	{
		CString strMsg(_T(""));
		strMsg.Append(_T("경로 분석 결과 정보가 없습니다.\r\n"));
		strMsg.Append(_T("메뉴 > 모형 > 도로 통행배정 모형을 먼저 실행해 주세요.\r\n"));
		strMsg.Append(_T("(실행 시,\r\n"));
		strMsg.Append(_T(" > 균형 배정법 : SOBA 선택 \r\n"));
		strMsg.Append(_T(" > 실행 옵션 선택에서 경로 교통량 분석을 선택해야 함)\r\n"));

		AfxMessageBox(strMsg);
		return;
	}

	if(false == ImChampFileExist::BushFileCheckMessage(m_pTarget))
	{
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

	if( !m_bRunPathVolumeSucceess)
	{
		AfxMessageBox(m_strRunErrMsg);
		return;
	}

	UpdateBushMinMax();
	UpdateDZoneCombo();

	UpdateData(FALSE);
}


void KPathVolumeDlg::ResetVolumeData()
{
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);
		m_wndReportCtrl.Populate();
		AllClearShortestPath();

		InitBushVolMinMax();
		InitGogumaVolMinMax();
		SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE,  _T(""));
		SetDlgItemText(IDC_EDIT_MIN_VALUE2, _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE2, _T(""));

		m_mapEndZoneNodeData.clear();
		m_vecEndZoneNodeIDCaption.clear();
		m_oEndNodeIdCaption.TID   = _T("-1");
		SetDlgItemText(IDC_EDIT4, _T(""));

		m_mapPathVolumeInfo.clear();

		CheckDlgButton(IDC_CHECK_BUSH,   BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_GOGUMA, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_PATH,   BST_UNCHECKED);
		
		m_vecDrawBushLinkInfo.clear();
		m_vecDrawGogumaLinkInfo.clear();
		m_vecDrawEachLinkInfo.clear();

		m_vecDrawBushLinkInfoNoLabel.clear();
		m_vecDrawGogumaLinkInfoNoLabel.clear();
		m_vecDrawEachLinkInfoNoLabel.clear();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KPathVolumeDlg::UpdateBushMinMax()
{
	CString strMinBushVol(_T(""));
	CString strMaxBushVol(_T(""));

	double dMinBushVol(0.0);
	dMinBushVol = m_dMinBushVol - 0.5;
	if(dMinBushVol < 0.0)
		dMinBushVol = 0.0;

	if (dMinBushVol < m_dMinPathVol)
	{
		dMinBushVol = m_dMinPathVol;
	}

	int nMinBushVol = (int)dMinBushVol;
	dMinBushVol     = (double)nMinBushVol; // double 버림 처리
	if (dMinBushVol < m_dMinPathVol)
	{
		dMinBushVol = m_dMinPathVol;
		/*strMinBushVol.Format(_T("%f"), dMinBushVol);*/
		GetDlgItemText(IDC_EDIT1, strMinBushVol);
	}
	else
	{
		strMinBushVol.Format(_T("%.0f"), dMinBushVol);
	}

	double dMaxBushVol(0.0);
	dMaxBushVol = m_dMaxBushVol + 0.6;
	strMaxBushVol.Format(_T("%.0f"), dMaxBushVol);

	SetDlgItemText(IDC_EDIT_MIN_VALUE,  strMinBushVol);
	SetDlgItemText(IDC_EDIT_MAX_VALUE,  strMaxBushVol);
}


unsigned __stdcall KPathVolumeDlg::PathVolumeThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("기종점간 이용경로 도로 교통량 분석 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Zone-to-Zone Highway Path Analysis Module..."));
	}

	KPathVolumeDlg* pDlg = (KPathVolumeDlg*)pParameter->GetParameter();
	m_bRunPathVolumeSucceess = pDlg->GetSearchPathVolume();

	return 0;
}


bool KPathVolumeDlg::GetSearchPathVolume()
{
	TxLogDebugStartMsg();
	m_strRunErrMsg = _T("");

	try
	{
		if(! CreateArgument(m_oUserColumnsLink))
		{
			m_nResultCode = DLL_CREATE_ARGFILE_ERROR;
			ThrowException(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
		}

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			m_nResultCode = DLL_USER_STOPPING;
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}

		//Step 0. Dll load
		CString strWindowName;
		GetWindowText(strWindowName);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("기종점간 이용경로 도로 교통량 분석 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run Zone-to-Zone Highway Path Analysis Module..."));
		}

		KLauncher::Run(strWindowName, 6);

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			m_nResultCode = DLL_USER_STOPPING;
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("기종점간 이용경로 도로 교통량 분석 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("Zone-to-Zone Highway Path Analysis Module Complete."));
			}
		}
		else if (DLL_USER_STOPPING == m_nResultCode || QBicSimpleProgressThread::IsUserStopped())
		{
            m_nResultCode = DLL_USER_STOPPING;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			AddStatusMessage(_T("> Assignment Dll Not Found ..."));
			ThrowException(_T("DLL NOT Found."));
		}
		else
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Running Failed."));
			}
			ThrowException(_T("Path Volume 검색 중 오류가 발생하였습니다"));
		}

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			m_nResultCode = DLL_USER_STOPPING;
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
		}
		if (! ReadOutPutFile() )
		{
			m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
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
		return false;
	}
	catch(...)
	{
		m_strRunErrMsg = _T("모듈 실행 중 예상치 못한 오류가 발생하였습니다.");
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KPathVolumeDlg::CreateArgument( KIOColumns &a_oUserColumnsLink )
{
	TxLogDebugStartMsg();

	ImChampDir::SetCurrentDirectoryToApp();

	if(! ClearFiles())
	{
		return false;
	}

	bool bResult = false;

	try 
	{
		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget, BUSH_PATHVOLUMN_FOLDER_NAME);
	} catch (...) {
		AfxMessageBox(_T("Dat 생성(DefineNetworkType.arg) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	//TxLogDebug("END CLEAR FILE ------ ");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(T_ARG_PATHVOLUME, nOpenMode);

	// .arg 작성
	bResult = WritePathVolumeArgument(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WritePathVolumeArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	of.Close();

	//TxLogDebug("END ARG CREAETE FILE ------ ");
	// 링크파일 생성
	std::set<int> setIncludeNodeCode;
	std::set<int> setIncludeLinkCode;
	std::set<Integer> setUsableNodeData;
	std::set<Integer> setUsableLinkData;
	std::set<Integer> setUsableTurnData;


	KDBaseAssignment::GetAssignmentTypeData(m_pTarget, setIncludeNodeCode, setIncludeLinkCode);

	KInspectionNetwork::Inspection(m_pTarget, setIncludeNodeCode, setIncludeLinkCode, setUsableNodeData, setUsableLinkData);

	CString strFilePath;

	strFilePath = ConvertSavePath(_T("link.dat"));

	bResult = KBulkDBase2File::ExportDB2LinkFileExcludeType(m_pTarget->GetDBaseConnection(), a_oUserColumnsLink, setUsableLinkData, strFilePath);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2LinkFileB) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	strFilePath = ConvertSavePath(_T("node.dat"));

	// 노드파일 생성
	bResult = KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), strFilePath);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	strFilePath = ConvertSavePath(_T("turn.dat"));
	// 회전정보 파일 생성
	bResult = KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(), strFilePath);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2TurnFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	//TxLogDebug("end turn CREAETE FILE ------ ");

	TxLogDebugEndMsg();
	return true;
}


bool KPathVolumeDlg::WritePathVolumeArgument(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		KObject* pScenario = m_pTarget->GetParentObject();

		strOut.Format(_T("%s_%s_%s\r\n"), m_pProject->GetName(), pScenario->GetName(), m_pTarget->GetName());
		of.WriteString(strOut);

		//선택한 수단값으로 업데이트를 하면되고
		//strOut.Format(_T("OD\t%d\t%d\r\n"), m_nModeCount,  m_nModeID);
		strOut.Format(_T("OD\t%d\t%d\r\n"), m_nModeCount,  m_nModeID);
		of.WriteString(strOut);

		strOut.Format(_T("run_options\t%d\r\n"), m_nRunOption);
		of.WriteString(strOut);

		strOut.Format(_T("ZoneThru\t%d\r\n"), m_nZoneThrough);
		of.WriteString(strOut);

		strOut.Format(_T("OZone\t%I64d\r\n"), m_nxStartNodeID);
		of.WriteString(strOut);

		strOut.Format(_T("PathVolumeLimit\t%f\r\n"), m_dMinPathVol);
		of.WriteString(strOut);

		CString FolderName(_T(""));
		FolderName.Format(_T("%s\\PATHVOLUME\r\n"), m_pTarget->GetLocation());
		of.WriteString(FolderName);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KPathVolumeDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove(ARG_PATHVOLUME);
		nResult = remove(DAT_NODE);
		nResult = remove(DAT_LINK);
		nResult = remove(DAT_TURN);

		// 결과 파일 삭제 
		nResult = remove(DAT_OUT_PATHVOLUME);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


void KPathVolumeDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KPathVolumeDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KPathVolumeDlg::ReadOutPutFile()
{
	m_mapPathVolumeInfo.clear();
	InitBushVolMinMax();
	
	try
	{
		CString strPath = ConvertSavePath(T_DAT_OUT_PATHVOLUME);
		ifstream ifs(strPath, ios::binary);
		if (! ifs)
		{ 
			ifs.close();
			return false;
		}

		std::map<Integer, double> mapLinkVolume;
		while(! ifs.eof())
		{
			Integer nxDesZoneID(0);
			ifs.read(reinterpret_cast<char*>( &nxDesZoneID), sizeof(Integer) );

			double dDistance = 0.0;
			ifs.read(reinterpret_cast<char*>( &dDistance), sizeof(double) );

			if (ifs.eof())
			{
				break;
			}

			double dTimeCost = 0.0;
			ifs.read(reinterpret_cast<char*>( &dTimeCost), sizeof(double) );

			double dVolume   = 0.0;
			ifs.read(reinterpret_cast<char*>( &dVolume), sizeof(double));

			int nCount       = 0;
			ifs.read(reinterpret_cast<char*>( &nCount), sizeof(int));

			std::vector<Integer> OutputLinkList;
			for (int i = 0; i< nCount; i++)
			{
				Integer nxLinkID = 0;
				ifs.read(reinterpret_cast<char*>( &nxLinkID), sizeof(Integer));
				OutputLinkList.push_back(nxLinkID);

				AutoType iter = mapLinkVolume.find(nxLinkID);
				if (iter != mapLinkVolume.end())
				{
					double &dSumVolume = iter->second;
					dSumVolume += dVolume;
					SetMaxBushVol(dSumVolume);
				}
				else
				{
					mapLinkVolume.insert(std::make_pair(nxLinkID, dVolume));
					SetMaxBushVol(dVolume);
				}
			}

			if(nCount < 1)
			{
				continue;
			}

			SetMinBushVol(dVolume);
			
			TPathVolumeInfo oTPathInfo;
			oTPathInfo.TnxOriginID		= m_nxStartNodeID; //임시로 만듬;
			oTPathInfo.TnxDestinationID	= nxDesZoneID;   //임시 데이터;
			oTPathInfo.TdVolume         = dVolume;
			oTPathInfo.TvecLink         = OutputLinkList;
			oTPathInfo.TdDistance       = dDistance;
			oTPathInfo.TdTimeCost       = dTimeCost;
						
			AutoType iter  = m_mapPathVolumeInfo.find(nxDesZoneID);
			AutoType itEnd = m_mapPathVolumeInfo.end();

			if (iter != itEnd)
			{
				std::vector<TPathVolumeInfo> &vecPathVolumeInfo = iter->second;
				vecPathVolumeInfo.push_back(oTPathInfo);
			}
			else
			{
				std::vector<TPathVolumeInfo> vecPathVolumeInfo;
				vecPathVolumeInfo.push_back(oTPathInfo);
				m_mapPathVolumeInfo.insert(std::make_pair(nxDesZoneID, vecPathVolumeInfo));
			}
		}
		
		AutoType iter  = m_mapPathVolumeInfo.begin();
		AutoType itEnd = m_mapPathVolumeInfo.end();

		while (iter != itEnd)
		{
			std::vector<TPathVolumeInfo> &vecPathVolumeInfo = iter->second;
			std::sort( vecPathVolumeInfo.begin(), vecPathVolumeInfo.end(), Descending );    

			size_t nxCount = vecPathVolumeInfo.size();

			for (size_t i= 0; i< nxCount; i++)
			{
				TPathVolumeInfo &oTPathInfo = vecPathVolumeInfo[i];
				oTPathInfo.TnxPathNo = (i+1);
			}

			++iter;
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapPathVolumeInfo.clear();
		InitBushVolMinMax();
		TxLogDebugException();
		return false;
	}    

	return true;
}


void KPathVolumeDlg::InitBushVolMinMax()
{
	m_dMaxBushVol = 0.0;
	m_dMinBushVol = 0.0;
}


void KPathVolumeDlg::SetMinBushVol(double &a_dBushVol)
{
	if( a_dBushVol > 0 )
	{
		if(0.0 == m_dMinBushVol)
		{
			m_dMinBushVol = a_dBushVol;
		}

		if(m_dMinBushVol > a_dBushVol)
		{
			m_dMinBushVol = a_dBushVol;
		}
	}
}


void KPathVolumeDlg::SetMaxBushVol(double &a_dBushVol)
{
	if( a_dBushVol > 0 )
	{
		if(m_dMaxBushVol < a_dBushVol)
		{
			m_dMaxBushVol = a_dBushVol;
		}
	}
}


void KPathVolumeDlg::InitGogumaVolMinMax()
{
	m_dMaxGogumaVol = 0.0;
	m_dMinGogumaVol = 0.0;
}


void KPathVolumeDlg::SetMinGogumaVol(double &a_dGogumaVol)
{
	if( a_dGogumaVol > 0 )
	{
		if(0.0 == m_dMinGogumaVol)
		{
			m_dMinGogumaVol = a_dGogumaVol;
		}

		if(m_dMinGogumaVol > a_dGogumaVol)
		{
			m_dMinGogumaVol = a_dGogumaVol;
		}
	}
}


void KPathVolumeDlg::SetMaxGogumaVol(double &a_dGogumaVol)
{
	if( a_dGogumaVol > 0 )
	{
		if(m_dMaxGogumaVol < a_dGogumaVol)
		{
			m_dMaxGogumaVol = a_dGogumaVol;
		}
	}
}


Integer KPathVolumeDlg::GetSelStartNodeId()
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


Integer KPathVolumeDlg::GetSelEndNodeId()
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


int KPathVolumeDlg::GetSelMode()
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


void KPathVolumeDlg::InsertReportData(Integer a_nxDZoneID)
{
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		int nIdx(0);

		KMunsellColor* pMunSellColor = new KMunsellColor(0, true);
		COLORREF    colorRef = m_cpPathColor.GetColor();

		AutoType iter  = m_mapPathVolumeInfo.find(a_nxDZoneID);
		AutoType itEnd = m_mapPathVolumeInfo.end();

		if(iter == itEnd)
			throw 1;

		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;

		std::vector<TPathVolumeInfo> &vecPathVolumeInfo = iter->second;

		size_t nxCount = vecPathVolumeInfo.size();
		for (size_t i = 0; i < nxCount; i++)
		{
			TPathVolumeInfo& oTPathInfo = vecPathVolumeInfo[i];

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)&oTPathInfo);
			pItem->SetChecked(bSelectAll);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber ((double)oTPathInfo.TnxPathNo));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathInfo.TdVolume, _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(1, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetEditable(TRUE);

			//CalculateLInkLength(oTPathInfo.TvecLink, dLinkLength);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTPathInfo.TdDistance, _T("%.2f")));
			pItem->SetAlignment(xtpColumnTextRight);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			colorRef = pMunSellColor->GetColor(nIdx);
			pItem->SetBackgroundColor(colorRef);

			++nIdx;
		}

		if (nxCount < 1)
		{
			SetDlgItemText(IDC_EDIT_TIME, _T(""));
		}
		else
		{
			CString strTime(_T(""));
			strTime.Format(_T("%.2f"), vecPathVolumeInfo[0].TdTimeCost);
			SetDlgItemText(IDC_EDIT_TIME, strTime);
		}

		m_wndReportCtrl.Populate();
		delete pMunSellColor;
	}
	catch (...)
	{
		TxLogDebugException();
	}

	UpdateData(FALSE);
}


void KPathVolumeDlg::OnSelEndOkPathClr()
{
	try
	{
		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		COLORREF clrPath = m_cpPathColor.GetColor();

		for(int i= 0; i< nRecordCount; i++)
		{
			CXTPReportRecord*     pRecord    = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem      = pRecord->GetItem(_0_COLUMN_CHECK);

			TPathVolumeInfo* pTPathInfo = (TPathVolumeInfo*)pItem->GetItemData();
			pTPathInfo->Tcolor = clrPath;

			pItem = pRecord->GetItem(_5_COLUMN_COLOR);
			pItem->SetBackgroundColor(clrPath);
		}

		m_wndReportCtrl.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeDlg::OnBnClickedButtonDraw()
{
	std::vector<TPathVolumeInfo> vecPathVolumeInfo;

	try
	{
		GetDrawLineData(vecPathVolumeInfo);

		if (vecPathVolumeInfo.size() == 0)
			throw(1);

		KPathVolumeInfo::GetDrawPathVolumeInfo(vecPathVolumeInfo, m_vecDrawEachLinkInfo, m_vecDrawEachLinkInfoNoLabel);

		CheckDlgButton(IDC_CHECK_PATH, BST_CHECKED);
		DrawVolumeMap();
	}
	catch(int i)
	{
		switch (i)
		{
		case 1:
			AfxMessageBox(_T("선택한 노선이 없습니다."));
			break;

		default:
			break;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeDlg::OnBnClickedButtonClear()
{
	CheckDlgButton(IDC_CHECK_PATH, BST_UNCHECKED);
	DrawVolumeMap();
}


void KPathVolumeDlg::GetDrawLineData( std::vector<TPathVolumeInfo>& a_vecPathInfo )
{
	CXTPReportRecords*				pExtRecords			= NULL;
	CXTPReportRecord*				pExtRecord			= NULL;
	CXTPReportRecordItem*			pExtItemCheck		= NULL;
	CXTPReportRecordItem*			pExtItemColor		= NULL;
	CXTPReportRecordItem*			pExtItemNo			= NULL;
	CXTPReportRecordItemNumber*		pItemNumber			= NULL;

	pExtRecords      = m_wndReportCtrl.GetRecords();
	int nRecordCount = pExtRecords->GetCount();

	for(int i= 0; i< nRecordCount; i++)
	{
		pExtRecord    = pExtRecords->GetAt(i);
		pExtItemCheck = pExtRecord->GetItem(_0_COLUMN_CHECK);
		TPathVolumeInfo* pTPathVolumeInfo = (TPathVolumeInfo*)pExtItemCheck->GetItemData();

		if( pExtItemCheck->GetHasCheckbox() == FALSE || pExtItemCheck->IsChecked() == FALSE )
			continue;

		pItemNumber			= (CXTPReportRecordItemNumber*)pExtRecord->GetItem(_1_COLUMN_PATH_NO);
		CXTPReportColumn*     pColumn = m_wndReportCtrl.GetColumns()->GetAt(_1_COLUMN_PATH_NO);
		//CString strPathNo	= pExtItemNo->GetCaption(pColumn);
		Integer nxPathNo	= (Integer)pItemNumber->GetValue();

		pItemNumber			= (CXTPReportRecordItemNumber*)pExtRecord->GetItem(_3_COLUMN_WIDTH);
		double dPathWidth	= pItemNumber->GetValue();

		pExtItemColor		= pExtRecord->GetItem(_5_COLUMN_COLOR);
		COLORREF color   	= pExtItemColor->GetBackgroundColor();

		pTPathVolumeInfo->Tcolor = color;
		pTPathVolumeInfo->TdWidth = dPathWidth;
		a_vecPathInfo.push_back(*pTPathVolumeInfo);
	}
}


void KPathVolumeDlg::InitWidthValCombos()
{
	int     nCurIndex   = 0;
	CString strValue    = _T("");

	double  dValue = WIDTH_COMBO_INITVALUE;
	for(int i= 0; i< WIDTH_COMBO_DATACOUNT; i++)
	{
		strValue.Format(_T("%.1f"), dValue);

		nCurIndex = m_cboWidth.AddString(strValue);
		m_vecWidth.push_back(dValue);

		dValue +=    WIDTH_COMBO_INCREMENT;
	}

	dValue         = INCREMENT_COMBO_INITVALUE;
	for(int i= 0; i< INCREMENT_COMBO_DATACOUNT; i++)
	{
		strValue.Format(_T("%.1f"), dValue);

		nCurIndex = m_cboIncrement.AddString(strValue);
		m_vecIncrement.push_back(dValue);

		dValue +=    INCREMENT_COMBO_INCREMENT;
	}

	m_cboWidth.SetCurSel    (1);
	m_cboIncrement.SetCurSel(0);
}


void KPathVolumeDlg::SetDefaultWidthInfo()
{
	UpdateData(TRUE);

	int nWidthIdx     = m_cboWidth.GetCurSel();
	int nIncrementIdx = m_cboIncrement.GetCurSel();

	if( nWidthIdx < 0 || nIncrementIdx < 0)
	{
		return;
	}

	double dValue        = m_vecWidth[nWidthIdx];
	double dIncrementVal = m_vecIncrement[nIncrementIdx];

	CXTPReportRecords*          pRecords = NULL;
	CXTPReportRecord*           pRecord  = NULL;
	CXTPReportRecordItemNumber* pItemNumber    = NULL;

	m_wndReportCtrl.GetRows();
	int nTotalRowCount = m_wndReportCtrl.GetRows()->GetCount();

	double dSumValue = dValue + (dIncrementVal * (nTotalRowCount - 1));

	for(int i= 0; i< nTotalRowCount; i++)
	{
		CXTPReportRow* pRow = m_wndReportCtrl.GetRows()->GetAt(i);
		CXTPReportRecord* pRecord = pRow->GetRecord();

		pItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_WIDTH);

		pItemNumber->SetValue(dSumValue);

		dSumValue -= dIncrementVal;
	}

	m_wndReportCtrl.RedrawControl();
}


void KPathVolumeDlg::OnCbnSelchangeComboWidthb()
{
	SetDefaultWidthInfo();

	this->SetFocus();
}


void KPathVolumeDlg::OnCbnSelchangeComboWidthp()
{
	SetDefaultWidthInfo();

	this->SetFocus();
}


bool KPathVolumeDlg::Descending( TPathVolumeInfo left, TPathVolumeInfo right )
{
	return left.TdVolume > right.TdVolume;
}


bool KPathVolumeDlg::Ascennding( TPathVolumeInfo left, TPathVolumeInfo right )
{
	return left.TdVolume < right.TdVolume;
}


void KPathVolumeDlg::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_5_COLUMN_COLOR == nIndex)
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor());
		if( IDOK == dlg.DoModal())
		{
			COLORREF clrPath = dlg.GetColor();

			CXTPReportRecord*	  pRecord    = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItem      = pRecord->GetItem(_0_COLUMN_CHECK);
			TPathVolumeInfo*      pTPathInfo = (TPathVolumeInfo*)pItem->GetItemData();
			pTPathInfo->Tcolor  = clrPath;

			pItemNotify->pItem->SetBackgroundColor(clrPath);
			m_wndReportCtrl.RedrawControl();
		}
	}
}


void KPathVolumeDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	}
}


void KPathVolumeDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnOK();
}


bool KPathVolumeDlg::CalculateLInkLength( std::vector<Integer> vecLink, double& dLengthValue )
{
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		size_t nCount = vecLink.size();

		if (nCount == 0)
		{
			return false;
		}

		CString strLinkSql = _T("");

		strLinkSql.Format(_T("%I64d"), vecLink[0]);

		for (size_t i = 1; i < nCount ; i++)
		{
			Integer nxLinkID = vecLink[i];
			CString strLinkID = _T("");
			strLinkID.Format(_T("%I64d"), nxLinkID);

			CString strAstok = _T(",");

			strLinkSql = strLinkSql + strAstok + strLinkID;

		}

		CString strSQL;
		strSQL.Format (_T(" SELECT SUM(length) FROM %s WHERE link_id in (%s)"), TABLE_LINK, strLinkSql);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(spResult == NULL)
		{
			return false;
		}

		while( spResult->Next() )
		{
			dLengthValue = spResult->GetValueDouble(0);
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}


void KPathVolumeDlg::ZoomToNodeFeature( Integer &a_nxNodeID )
{
	try
	{
		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL != pMapView)
		{
			pMapView->PanToFeature(KLayerID::Node(), a_nxNodeID );
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeDlg::UpdateGogumaVolMinMax(Integer a_nxNodeID)
{
	try
	{
		InitGogumaVolMinMax();

		AutoType iter  = m_mapPathVolumeInfo.find(a_nxNodeID);
		AutoType itEnd = m_mapPathVolumeInfo.end();

		if(iter == itEnd)
			throw 1;

		std::map<Integer, double> mapLinkVolume;

		std::vector<TPathVolumeInfo> &vecPathVolumeInfo = iter->second;
		size_t nxCount = vecPathVolumeInfo.size();
		for (size_t i = 0; i < nxCount; i++)
		{
			TPathVolumeInfo      &oTPathInfo = vecPathVolumeInfo[i];
			
			std::vector<Integer> &vecLink    = oTPathInfo.TvecLink;
			for (size_t k= 0; k< vecLink.size(); k++)
			{
				Integer &nxLinkID = vecLink[k];

				AutoType iterLinkVol = mapLinkVolume.find(nxLinkID);
				if(iterLinkVol != mapLinkVolume.end())
				{
					double &dSumVolume = iterLinkVol->second;
					dSumVolume += oTPathInfo.TdVolume;
					SetMaxGogumaVol(dSumVolume);
				}
				else
				{
					mapLinkVolume.insert(std::make_pair(nxLinkID, oTPathInfo.TdVolume));
					SetMaxGogumaVol(oTPathInfo.TdVolume);
				}
			}

			SetMinGogumaVol(oTPathInfo.TdVolume);
		}

		CString strMinGogumaVol(_T(""));
		CString strMaxGogumaVol(_T(""));

		double dMinGogumaVol(0.0);
		dMinGogumaVol = m_dMinGogumaVol - 0.5;
		if(dMinGogumaVol < 0.0)
			dMinGogumaVol = 0.0;

		int nMinGogumaVol = (int)dMinGogumaVol;
		dMinGogumaVol     = (double)nMinGogumaVol; // double 버림 처리
		if (dMinGogumaVol < m_dMinPathVol)
		{
			dMinGogumaVol = m_dMinPathVol;
			/*strMinGogumaVol.Format(_T("%f"), dMinGogumaVol);*/
			GetDlgItemText(IDC_EDIT1, strMinGogumaVol);
		}
		else
		{
			strMinGogumaVol.Format(_T("%.0f"), dMinGogumaVol);
		}

		double dMaxGogumaVol(0.0);
		dMaxGogumaVol = m_dMaxGogumaVol + 0.6;
		strMaxGogumaVol.Format(_T("%.0f"), dMaxGogumaVol);

		SetDlgItemText(IDC_EDIT_MIN_VALUE2,  strMinGogumaVol);
		SetDlgItemText(IDC_EDIT_MAX_VALUE2,  strMaxGogumaVol);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


BOOL KPathVolumeDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
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


CString KPathVolumeDlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s\\%s\\%s"), m_pTarget->GetLocation(), BUSH_PATHVOLUMN_FOLDER_NAME, a_strFileName);

	return strSavePath;
}


void KPathVolumeDlg::OnBnClickedButtonDrawBushVol()
{
	try
	{
		if (UserBushInputCheck() == false)
		{
			return;
		}

		CWaitCursor cw;

		TPathVolumeSettingInfo oTBushVolumeSettingInfo;
		oTBushVolumeSettingInfo.bUseOutRange = false;
		oTBushVolumeSettingInfo.clrOutRange  = RGB(255, 255, 255);
		oTBushVolumeSettingInfo.clrPath      = m_cpBushColor.GetColor();

		CString strValue(_T(""));
		GetDlgItemText(IDC_EDIT_MIN_VALUE, strValue);
		oTBushVolumeSettingInfo.dMinTrip    = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MAX_VALUE, strValue);
		oTBushVolumeSettingInfo.dMaxTrip    = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
		oTBushVolumeSettingInfo.dMinSize    = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
		oTBushVolumeSettingInfo.dMaxSize    = _ttof(strValue);

		KPathVolumeInfo::GetDrawBushVolumeInfo(m_mapPathVolumeInfo, oTBushVolumeSettingInfo, m_vecDrawBushLinkInfo, m_vecDrawBushLinkInfoNoLabel);

		CheckDlgButton(IDC_CHECK_BUSH, BST_CHECKED);
		DrawVolumeMap();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


bool KPathVolumeDlg::UserBushInputCheck()
{
	try
	{
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize(0.0),  dMaxSize(0.0);

		CString strValue(_T(""));

		if (m_mapPathVolumeInfo.size() < 1)
		{
			AfxMessageBox(_T("기점을 선택해 주세요."));
			return false;
		}

		//min value 체크
		GetDlgItemText(IDC_EDIT_MIN_VALUE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MIN_VALUE)->SetFocus();		
			return false;
		}
		dMinValue = _ttof(strValue);

		//IDC_EDIT_MAX_VALUE
		GetDlgItemText(IDC_EDIT_MAX_VALUE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MAX_VALUE)->SetFocus();		
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
			GetDlgItem(IDC_EDIT_MIN_VALUE)->SetFocus();		
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


void KPathVolumeDlg::OnBnClickedButtonClearBushVol()
{
	CheckDlgButton(IDC_CHECK_BUSH, BST_UNCHECKED);
	DrawVolumeMap();
}


void KPathVolumeDlg::ClearMapDraw(bool bRefresh)
{
	try
	{
		m_pMapView->TxClearCompositeThemes(bRefresh);
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


void KPathVolumeDlg::OnBnClickedButtonDrawGogumaVol()
{
	try
	{
		if (UserGogumaInputCheck() == false)
		{
			return;
		}

		AutoType iter  = m_mapPathVolumeInfo.find(m_nxEndNodeID);
		std::vector<TPathVolumeInfo> &vecPathVolumeInfo = iter->second;

		TPathVolumeSettingInfo oTGogumaVolumeSettingInfo;
		oTGogumaVolumeSettingInfo.bUseOutRange = false;
		oTGogumaVolumeSettingInfo.clrOutRange  = RGB(255, 255, 255);
		oTGogumaVolumeSettingInfo.clrPath      = m_cpGogumaColor.GetColor();

		CString strValue(_T(""));
		GetDlgItemText(IDC_EDIT_MIN_VALUE2, strValue);
		oTGogumaVolumeSettingInfo.dMinTrip    = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MAX_VALUE2, strValue);
		oTGogumaVolumeSettingInfo.dMaxTrip    = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MINSIZE2, strValue);
		oTGogumaVolumeSettingInfo.dMinSize    = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MAXSIZE2, strValue);
		oTGogumaVolumeSettingInfo.dMaxSize    = _ttof(strValue);

		KPathVolumeInfo::GetDrawGogumaVolumeInfo(vecPathVolumeInfo, oTGogumaVolumeSettingInfo, m_vecDrawGogumaLinkInfo, m_vecDrawGogumaLinkInfoNoLabel);

		CheckDlgButton(IDC_CHECK_GOGUMA, BST_CHECKED);
		DrawVolumeMap();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


bool KPathVolumeDlg::UserGogumaInputCheck()
{
	try
	{
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize(0.0),  dMaxSize(0.0);

		CString strValue(_T(""));

		m_nxEndNodeID = GetSelEndNodeId();
		if(DEFAULT_NODEID == m_nxEndNodeID)
		{
			AfxMessageBox(_T("종점을 선택해 주세요."));
			return false;
		}

		//min value 체크
		GetDlgItemText(IDC_EDIT_MIN_VALUE2, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MIN_VALUE2)->SetFocus();		
			return false;
		}
		dMinValue = _ttof(strValue);

		//IDC_EDIT_MAX_VALUE
		GetDlgItemText(IDC_EDIT_MAX_VALUE2, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MAX_VALUE2)->SetFocus();		
			return false;
		}
		dMaxValue = _ttof(strValue);

		//IDC_EDIT_MINSIZE
		GetDlgItemText(IDC_EDIT_MINSIZE2, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MINSIZE2)->SetFocus();		
			return false;
		}
		dMinSize = _ttof(strValue);

		//IDC_EDIT_MAXSIZE
		GetDlgItemText(IDC_EDIT_MAXSIZE2, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MAXSIZE2)->SetFocus();		
			return false;
		}
		dMaxSize = _ttof(strValue);

		if (dMinValue > dMaxValue)
		{
			AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MIN_VALUE2)->SetFocus();		
			return false;
		}

		if (dMinSize > dMaxSize)
		{
			AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MINSIZE2)->SetFocus();		
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


void KPathVolumeDlg::OnBnClickedButtonClearGogumaVol()
{
	CheckDlgButton(IDC_CHECK_GOGUMA, BST_UNCHECKED);
	DrawVolumeMap();
}


void KPathVolumeDlg::DrawVolumeMap()
{
	try
	{
		ClearMapDraw(false);

		std::vector<TDrawLinkInfo> vecDrawLinkInfo;

		if (IsDlgButtonChecked(IDC_CHECK_BUSH) == BST_CHECKED)
		{
			if (IsDlgButtonChecked(IDC_CHECK_LABEL_BUSH) == BST_CHECKED)
			{
				vecDrawLinkInfo = m_vecDrawBushLinkInfo;
			}
			else
			{
				vecDrawLinkInfo = m_vecDrawBushLinkInfoNoLabel;
			}
		}

		if (IsDlgButtonChecked(IDC_CHECK_GOGUMA) == BST_CHECKED)
		{
			if (IsDlgButtonChecked(IDC_CHECK_LABEL_GOGUMA) == BST_CHECKED)
			{
				size_t nxCount = m_vecDrawGogumaLinkInfo.size();
				for (size_t i= 0; i< nxCount; i++)
				{
					TDrawLinkInfo &oTDrawLinkInfo = m_vecDrawGogumaLinkInfo[i];
					vecDrawLinkInfo.push_back(oTDrawLinkInfo);
				}
			}
			else
			{
				size_t nxCount = m_vecDrawGogumaLinkInfoNoLabel.size();
				for (size_t i= 0; i< nxCount; i++)
				{
					TDrawLinkInfo &oTDrawLinkInfo = m_vecDrawGogumaLinkInfoNoLabel[i];
					vecDrawLinkInfo.push_back(oTDrawLinkInfo);
				}
			}
		}

		if(IsDlgButtonChecked(IDC_CHECK_PATH) == BST_CHECKED)
		{
			if (IsDlgButtonChecked(IDC_CHECK_LABEL_PATH) == BST_CHECKED)
			{
				size_t nxCount = m_vecDrawEachLinkInfo.size();
				for (size_t i= 0; i< nxCount; i++)
				{
					TDrawLinkInfo &oTDrawLinkInfo = m_vecDrawEachLinkInfo[i];
					vecDrawLinkInfo.push_back(oTDrawLinkInfo);
				}
			}
			else
			{
				size_t nxCount = m_vecDrawEachLinkInfoNoLabel.size();
				for (size_t i= 0; i< nxCount; i++)
				{
					TDrawLinkInfo &oTDrawLinkInfo = m_vecDrawEachLinkInfoNoLabel[i];
					vecDrawLinkInfo.push_back(oTDrawLinkInfo);
				}
			}
		}

		m_pMapView->TxDrawCompositeLink(vecDrawLinkInfo, true, true);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeDlg::OnCbnSelchangeComboMode()
{
	UpdateData(TRUE);

	int nSelCur = m_cboMode.GetCurSel();
	int nIdx    = m_cboMode.GetItemData(nSelCur);

	AutoType iter = m_mapModeInfo.find(nIdx);
	if (iter != m_mapModeInfo.end())
	{
		m_strPreSelectedModeName = iter->second;
	}
	
	ResetVolumeData();
}


void KPathVolumeDlg::OnEnChangeEditPathVolume()
{
	ResetVolumeData();
}


void KPathVolumeDlg::OnBnClickedCheckLabelBush()
{
	DrawVolumeMap();
}


void KPathVolumeDlg::OnBnClickedCheckLabelGoguma()
{
	DrawVolumeMap();
}


void KPathVolumeDlg::OnBnClickedCheckLabelPath()
{
	DrawVolumeMap();
}


BOOL KPathVolumeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			HWND hwnd = pMsg->hwnd;

			if (hwnd == m_cboMode.GetSafeHwnd()   || 
				hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd)
			{
				OnBnClickedButtonSearch();
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


HBRUSH KPathVolumeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_EDIT_TIME || ID == IDC_EDIT3 || ID == IDC_EDIT4 )
    {        
        /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
        pDC->SetBkMode(TRANSPARENT);*/
        pDC->SetBkMode(TRANSPARENT);

		hbr = (HBRUSH)m_backBrush;
        //return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }
	return hbr;
}


void KPathVolumeDlg::ClickedEditStartNode()
{
	SelectStartNodeList();
}


void KPathVolumeDlg::ClickedEditEndNode()
{
	SelectEndNodeList();
}


void KPathVolumeDlg::OnBnClickedButtonStartNodeSearch()
{
	SelectStartNodeList();
}


void KPathVolumeDlg::OnBnClickedButtonEndNodeSearch()
{
	SelectEndNodeList();
}


void KPathVolumeDlg::SelectStartNodeList()
{
	try
	{
		m_bSelectMapNode = MAP_NODE_START;

		CWaitCursor cw;

		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		m_spImTasIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
		m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);

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


void KPathVolumeDlg::SelectEndNodeList()
{
	try
	{
		m_bSelectMapNode = MAP_NODE_END;

		CWaitCursor cw;

		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		m_spImTasIDFind->SetIDCaption(m_vecEndZoneNodeIDCaption);
		m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapEndZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);

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


void KPathVolumeDlg::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;

		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nCount          = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_CHECK);
			pItem->SetChecked(bSelectAll);
		}

		m_wndReportCtrl.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KPathVolumeDlg::ModelessCancleFeedback(UINT nDialogID)
{

}
