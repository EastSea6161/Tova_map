// LinkAnalysisDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "LinkAnalysisDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Object.h"
#include "Target.h"
#include "MapView.h"
#include "KEMNode.h"
#include <fstream>
#include <iostream>
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "KExRecordItem.h"
#include "BulkDBaseNode.h"
#include "DBaseAssignment.h"
#include "InspectionNetwork.h"

#include "Launcher.h"
#include "WriteCodeAgrument.h"
#include "ImChampFileExist.h"
#include "ImChampDir.h"

enum
{
	_0_COLUMN_LINKID = 0,
	_1_COLUMN_FNODE,
	_2_COLUMN_TNODE
};


// KLinkAnalysisDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KLinkAnalysisDlg, KDialogEx)

bool KLinkAnalysisDlg::m_bRunLinkVolumeSucceess = false;

KLinkAnalysisDlg::KLinkAnalysisDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KDialogEx(KLinkAnalysisDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pProject(a_pProject)
	, m_pMapView(a_pMapView)
	, m_oMapDisplayDlg(this)
	, m_oTableODDlg(this)
	, m_nRadio(0)
	, m_strRunErrMsg(_T(""))
{
	try
	{
		m_nModeID               = DEFAULT_MODEID;
		m_nModeCount            = 0;
		m_nRunOption            = -1;
		m_nZoneThrough          = -1;

		m_pModeODTable          = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);	

		m_strPreSelectedModeName = _T("");
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

KLinkAnalysisDlg::~KLinkAnalysisDlg()
{
	CloseWindowProc();
	m_pModeODTable->Unregister(this);
	m_backBrush.DeleteObject();
}

void KLinkAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,        m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO1,        m_cboMode);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
}


BEGIN_MESSAGE_MAP(KLinkAnalysisDlg, KDialogEx)
	ON_NOTIFY(NM_CLICK,              IDC_REPORT,        OnReportItemClick)
	ON_BN_CLICKED(IDCANCEL,          &KLinkAnalysisDlg::OnBnClickedOK)
	ON_BN_CLICKED(IDC_BUTTON_O_MAP,  &KLinkAnalysisDlg::OnBnClickedButtonMap)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR,  &KLinkAnalysisDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &KLinkAnalysisDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &KLinkAnalysisDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_RADIO1,        &KLinkAnalysisDlg::OnBnClickedRadioVolumeMap)
	ON_BN_CLICKED(IDC_RADIO2,        &KLinkAnalysisDlg::OnBnClickedRadioTimeOD)
	ON_BN_CLICKED(IDC_BUTTON_DRAW,   &KLinkAnalysisDlg::OnBnClickedButtonDraw)
	ON_WM_COPYDATA()
	ON_CBN_SELCHANGE(IDC_COMBO1, &KLinkAnalysisDlg::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_CHECK1, &KLinkAnalysisDlg::OnBnClickedCheckLabel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


KMapView* KLinkAnalysisDlg::GetMapView(KTarget* a_pTarget)
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


void KLinkAnalysisDlg::CloseWindowProc()
{
	CleanMap();
}


void KLinkAnalysisDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
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


BOOL KLinkAnalysisDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	GetDBRunOption();
	GetDBModeInfo();

	KReportCtrlSetting::DefaultC(m_wndReportCtrl);
	InitReportHeader();
	m_wndReportCtrl.Populate();

	//####
	CWnd* pWnd = GetDlgItem(IDC_FRAME);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_oMapDisplayDlg.Create(IDD_5090_LinkAnalysisMapDisplayDlg, this);
	m_oMapDisplayDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_oMapDisplayDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	m_oTableODDlg.Create(IDD_5090_LinkAnalysisTableODDlg, this);
	m_oTableODDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_oTableODDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	RadioControl();

	m_pModeODTable->Register(this);
	m_backBrush.CreateSolidBrush(RGB(255,255,255));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KLinkAnalysisDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_LINKID,   _T("Link ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("링크 ID"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_FNODE,   _T("F_Node"),  30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("시작노드"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_TNODE,    _T("T_Node"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("끝노드"));
	}
}


bool KLinkAnalysisDlg::GetDBRunOption()
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

	const KIOColumns* pColumns = m_pLinkTable->Columns();
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


bool KLinkAnalysisDlg::GetDBModeInfo()
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

	//UpdateData(FALSE);

	return true;
}


int KLinkAnalysisDlg::GetSelMode()
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

// KLinkAnalysisDlg 메시지 처리기입니다.
void KLinkAnalysisDlg::OnBnClickedOK()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_pMapView->MapRemoveLayer(KLayerID::LayerID_SelectedLinkAnalysis());
    //m_pMapView->MapRefresh();
	CloseWindowProc();
	KDialogEx::OnOK();
}

void KLinkAnalysisDlg::MapSelectionFeedback( Integer a_nxID )
{
    if (-1 == a_nxID) { // 선택 종료
        if (m_vecLinkInfo.size() > 0) {
            DoneSearch(true);
        }
    }
}

void KLinkAnalysisDlg::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
    m_vecLinkInfo.clear();

    size_t nxCount = a_vecSelection.size();
    for (size_t i= 0; i < nxCount ; i++)
    {
        Integer nxFNodeID = 0;
        Integer nxTNodeID = 0;
        if( KBulkDBaseNode::FindFTNode(m_pTarget, a_vecSelection[i], nxFNodeID, nxTNodeID) == false ) 
        {
            InsertReportData();
            return;
        }

        TLinkInfo oTNewLinkInfo;
        oTNewLinkInfo.nxLinkID  = a_vecSelection[i];
        oTNewLinkInfo.nxFNodeID = nxFNodeID;
        oTNewLinkInfo.nxTNodeID = nxTNodeID;

        m_vecLinkInfo.push_back(oTNewLinkInfo);
    }

    InsertReportData();

	m_vecLinkVolume.clear();
	m_oMapDisplayDlg.SetLinkVolume(m_vecLinkVolume);
	m_oMapDisplayDlg.ClearReportData();

	m_vecLinkODVolume.clear();
	m_oTableODDlg.SetLinkODVolume(m_vecLinkODVolume);
	m_oTableODDlg.ClearReportData();

    DrawSelectedLink();
	ClearLinkViaPath();
}

void KLinkAnalysisDlg::MapSelectionNoneFeedback()
{

}

void KLinkAnalysisDlg::InsertReportData()
{
	m_wndReportCtrl.ResetContent(TRUE);
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;
	int nIdx = 0;

	size_t nxCount = m_vecLinkInfo.size();

	for (size_t i = 0; i < nxCount; i++)
	{
		TLinkInfo &oTLinkInfo = m_vecLinkInfo[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		CString strLinkID = _T("");
		strLinkID.Format(_T("%I64d"), oTLinkInfo.nxLinkID);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strLinkID));
		pItem->SetItemData((DWORD_PTR) oTLinkInfo.nxLinkID);
		pItem->SetAlignment(xtpColumnTextCenter);

		CString strFNodeID = _T("");
		strFNodeID.Format(_T("%I64d"), oTLinkInfo.nxFNodeID);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strFNodeID));
		pItem->SetItemData((DWORD_PTR) oTLinkInfo.nxFNodeID);
		pItem->SetAlignment(xtpColumnTextCenter);

		CString strTNodeID = _T("");
		strTNodeID.Format(_T("%I64d"), oTLinkInfo.nxTNodeID);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strTNodeID));
		pItem->SetItemData((DWORD_PTR) oTLinkInfo.nxTNodeID);
		pItem->SetAlignment(xtpColumnTextCenter);
	}

	m_wndReportCtrl.Populate();
}


void KLinkAnalysisDlg::OnBnClickedButtonMap()
{
	try
	{
        m_vecLinkInfo.clear();
        InsertReportData();

        m_pMapView->MapRemoveLayer(KLayerID::LayerID_SelectedLinkAnalysis());

        CleanMap();

		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL != pMapView)
		{
			pMapView->SelectLinkPath(this);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KLinkAnalysisDlg::MakeSelectedLink()
{
	m_vecSelectedLinkID.clear();

	for( size_t i= 0; i< m_vecLinkInfo.size(); i++ )
	{
		TLinkInfo &oTLinkInfo = m_vecLinkInfo[i];
		m_vecSelectedLinkID.push_back(oTLinkInfo.nxLinkID);
	}
}


void KLinkAnalysisDlg::OnBnClickedButtonSearch()
{
	DoneSearch(true);
}


void KLinkAnalysisDlg::DoneSearch(bool a_bShowErrMsg)
{
	try
	{
		CleanMap();

		m_nResultCode = DLL_UNEXPECTED_ERROR;

		m_nModeID = GetSelMode();
		if(DEFAULT_MODEID == m_nModeID)
			return;

		if(m_vecLinkInfo.size() < 1)
		{
			if (a_bShowErrMsg) {
				AfxMessageBox(_T("먼저 링크/구간을 지정해 주세요."));
			}
			
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

			if (a_bShowErrMsg) {
				AfxMessageBox(strMsg);
			}
			
			return;
		}

		if(false == ImChampFileExist::BushFileCheckMessage(m_pTarget)) {
			return;
		}

		m_oMapDisplayDlg.ClearReportData();
		m_oMapDisplayDlg.InitSettingInfo();
		m_oTableODDlg.ClearReportData();

		/*ClearLinkViaPath();*/

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(LinkVolumeThreadCaller, this, false, nLang);

		if( !m_bRunLinkVolumeSucceess)
		{
			m_vecLinkVolume.clear();
			m_vecLinkODVolume.clear();

			if (a_bShowErrMsg) {
				AfxMessageBox(m_strRunErrMsg);
			}
		}
		else
		{
			if (m_vecLinkVolume.size()< 1)
			{
				if (a_bShowErrMsg) {
					AfxMessageBox(_T("경로 분석 결과가 존재하지 않습니다."));
				}
			}
		}

		m_oMapDisplayDlg.SetLinkVolume(m_vecLinkVolume);
		m_oMapDisplayDlg.InitReportData();

		m_oTableODDlg.SetLinkODVolume(m_vecLinkODVolume);
		m_oTableODDlg.InitReportData();

		RadioControl();
		MakeSelectedLink();

		DoneDraw(false);
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


void KLinkAnalysisDlg::OnBnClickedButtonExport()
{
	size_t nxCount = m_vecLinkODVolume.size();

	if (nxCount < 1)
	{
		AfxMessageBox(_T("경로 분석 결과가 존재하지 않습니다."));
		return;
	}

	bool bExportResult = false;

	CString strFileName(_T(""));
	CString strSavePath(_T(""));
	strFileName.Format(_T("LinkODVolume.txt"));
	strSavePath.Format(_T("%s\\PATHVOLUME\\%s"), m_pTarget->GetLocation(), strFileName);

	UINT nWriteFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	UINT nCodePage = 949;

	CStdioFileEx csvFile;
	csvFile.SetCodePage( nCodePage );

	try
	{
		if( csvFile.Open( strSavePath, nWriteFlags ) == FALSE )
		{
			return;
		}

		CString strSeparator(_T("\t"));
		CString strColumnLine(_T(""));
		strColumnLine.Format(_T("\"OrginID\"%s\"DestinationID\"%s\"Volume\""), strSeparator, strSeparator);
		strColumnLine += _T("\r\n");
		csvFile.WriteString(strColumnLine);

		CString strODVolume(_T("")); 
		for (size_t i = 0; i < nxCount; i++)
		{
			TLinkODVolume &oTLinkODVolume = m_vecLinkODVolume[i];

			strODVolume.Format(_T("%I64d%s%I64d%s%f"),oTLinkODVolume.nxOZoneID, strSeparator, oTLinkODVolume.nxDZoneID, strSeparator, oTLinkODVolume.dVolume );

			strODVolume.Append(_T("\r\n"));
			csvFile.WriteString(strODVolume);
		}

		csvFile.Close();

		ShellExecute(NULL, _T("open"), strSavePath, NULL, NULL, SW_SHOW);    // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	}
	catch (...)
	{
		csvFile.Close();
		TxLogDebugException();	
	}
}


unsigned __stdcall KLinkAnalysisDlg::LinkVolumeThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("링크/구간 통과 기종점간 도로 교통량 분석 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Highway Selected Link Analysis Module..."));
	}

	KLinkAnalysisDlg* pDlg = (KLinkAnalysisDlg*)pParameter->GetParameter();
	m_bRunLinkVolumeSucceess = pDlg->GetSearchLinkVolume();
	
	return 0;
}


bool KLinkAnalysisDlg::GetSearchLinkVolume()
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

		// 	if(NULL != m_hAssignment)
		// 	{
		// 		::FreeLibrary(m_hAssignment);
		// 	}
		// 
		// 	// load library
		// 	m_hAssignment = ::LoadLibrary(_T("SelectedLinkAnalysis.dll"));
		// 
		// 	if( NULL == m_hAssignment )
		// 	{
		// 		AfxMessageBox(_T("SelectedLinkAnalysis.dll이 없습니다."));
		// 		return false;
		// 	}
		// 
		// 	AssignmentProcPtr = (SELECTEDLINKANALYSIS)(GetProcAddress(m_hAssignment, "SELECTEDLINKANALYSIS"));
		// 
		// 	if(NULL == AssignmentProcPtr)
		// 	{
		// 		AfxMessageBox(_T("SELECTEDLINKANALYSIS 함수가 없습니다."));
		// 		::FreeLibrary(m_hAssignment);
		// 		return false;
		// 	}
		// 
		// 	AddStatusMessage(_T("> Analysis Start ..."));
		// 	int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessageFromDll);
		// 
		// 	if(NULL != m_hAssignment)
		// 	{
		// 		::FreeLibrary(m_hAssignment);
		// 		TxLogDebug(_T("FreeLibrary"));
		// 	}

		//Step 0. Dll Load

		CString strWindowName(_T(""));
		GetWindowText(strWindowName);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("링크/구간 통과 기종점간 도로 교통량 분석 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run Highway Selected Link Analysis Module..."));
		}

		KLauncher::Run(strWindowName, 7);

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			m_nResultCode = DLL_USER_STOPPING;
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("링크/구간 통과 기종점간 도로 교통량 분석 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("Highway Selected Link Analysis Module Complete."));
			}
		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
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
			ThrowException(_T("Link Volume 검색 중 오류가 발생하였습니다"));
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


bool KLinkAnalysisDlg::CreateArgument( KIOColumns &a_oUserColumnsLink )
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
	CStdioFileEx of(T_ARG_LINKANALYSIS, nOpenMode);

	//SelectedLinkAnalysis.arg

	// .arg 작성
	bResult = WriteLinkVolumeArgument(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteLinkVolumeArgument) 중 오류가 발생하였습니다. !!!"));
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

	// 노드파일 생성
	strFilePath = ConvertSavePath(_T("node.dat"));

	bResult = KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), strFilePath);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	
	// 회전정보 파일 생성
	strFilePath = ConvertSavePath(_T("turn.dat"));

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


bool KLinkAnalysisDlg::WriteLinkVolumeArgument(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		KObject* pScenario = m_pTarget->GetParentObject();

		strOut.Format(_T("%s_%s_%s\r\n"), m_pProject->GetName(), pScenario->GetName(), m_pTarget->GetName());
		of.WriteString(strOut);

		strOut.Format(_T("OD\t%d\t%d\r\n"), m_nModeCount,  m_nModeID);
		of.WriteString(strOut);

		strOut.Format(_T("run_options\t%d\r\n"), m_nRunOption);
		of.WriteString(strOut);

		strOut.Format(_T("ZoneThru\t%d\r\n"), m_nZoneThrough);
		of.WriteString(strOut);

		CString FolderName(_T(""));
		FolderName.Format(_T("%s\\PATHVOLUME\r\n"), m_pTarget->GetLocation());
		of.WriteString(FolderName);

		int nSize = m_vecLinkInfo.size();
		strOut.Format(_T("LinkSelected\t%d\r\n"), nSize);
		of.WriteString(strOut);

		for(size_t i= 0; i< m_vecLinkInfo.size(); i++)
		{
			TLinkInfo &oTLinkInfo = m_vecLinkInfo[i];
			strOut.Format(_T("%I64d\t%I64d\r\n"), oTLinkInfo.nxFNodeID, oTLinkInfo.nxTNodeID);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KLinkAnalysisDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove(ARG_LINKANALYSIS);
		nResult = remove(DAT_NODE);
		nResult = remove(DAT_LINK);
		nResult = remove(DAT_TURN);

        CString strLocation(_T(""));
        strLocation.Format(_T("%s\\%s\\%s"), m_pTarget->GetLocation(), BUSH_PATHVOLUMN_FOLDER_NAME, CString(DAT_OUT_LINKANALYSIS_LINK));
                
		// 결과 파일 삭제 
		nResult = remove((CStringA)strLocation);
        strLocation.Format(_T("%s\\%s\\%s"), m_pTarget->GetLocation(), BUSH_PATHVOLUMN_FOLDER_NAME, CString(DAT_OUT_LINKANALYSIS_OD));
		nResult = remove((CStringA)strLocation);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


void KLinkAnalysisDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KLinkAnalysisDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KLinkAnalysisDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();

	ReadOutputLink();
	ReadOutputOD();

	TxLogDebugEndMsg();
	return true;
}


bool KLinkAnalysisDlg::ReadOutputLink()
{
	m_vecLinkVolume.clear();

	try
	{
		CString strFileName = ConvertSavePath(T_DAT_OUT_LINKANALYSIS_LINK);
		std::ifstream ifs(strFileName, std::ios::binary);
		if (! ifs)
		{ 
			ifs.close();
			return false;
		}

		while(! ifs.eof())
		{
			TLinkVolume oTLinkVolume;

			ifs.read(reinterpret_cast<char*>( &(oTLinkVolume.nxLinkID)), sizeof(Integer) );

			if (ifs.eof())
			{
				break;
			}

			ifs.read(reinterpret_cast<char*>( &(oTLinkVolume.dVolume)),  sizeof(double) );
			
			m_vecLinkVolume.push_back(oTLinkVolume);
		}

		ifs.close();
	}
	catch (...)
	{
		m_vecLinkVolume.clear();
		TxLogDebugException();
		return false;
	}    

	return true;
}


bool KLinkAnalysisDlg::ReadOutputOD()
{
	m_vecLinkODVolume.clear();

	try
	{
		CString strFileName = ConvertSavePath(T_DAT_OUT_LINKANALYSIS_OD);
		std::ifstream ifs(strFileName, std::ios::binary);
		if (! ifs)
		{ 
			ifs.close();
			return false;
		}

		while(! ifs.eof())
		{
			TLinkODVolume oTLinkODVolume;
			ifs.read(reinterpret_cast<char*>( &(oTLinkODVolume.nxOZoneID)), sizeof(Integer) );
			
			if (ifs.eof())
			{
				break;
			}
			
			ifs.read(reinterpret_cast<char*>( &(oTLinkODVolume.nxDZoneID)), sizeof(Integer) );
			ifs.read(reinterpret_cast<char*>( &(oTLinkODVolume.dVolume)),   sizeof(double) );

			m_vecLinkODVolume.push_back(oTLinkODVolume);
		}

		ifs.close();
	}
	catch (...)
	{
		m_vecLinkODVolume.clear();
		TxLogDebugException();
		return false;
	}    

	return true;
}


void KLinkAnalysisDlg::OnBnClickedRadioVolumeMap()
{
	RadioControl();
}


void KLinkAnalysisDlg::OnBnClickedRadioTimeOD()
{
	RadioControl();
}


void KLinkAnalysisDlg::RadioControl()
{
	UpdateData(TRUE);

	if(m_nRadio == 0)
	{
		GetDlgItem(IDC_BUTTON_DRAW)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_BUTTON_EXPORT)->ShowWindow(SW_HIDE);

		m_oMapDisplayDlg.ShowWindow(SW_SHOW);
		m_oTableODDlg.ShowWindow(SW_HIDE);
	}
	else if(m_nRadio == 1)
	{
		GetDlgItem(IDC_BUTTON_DRAW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BUTTON_EXPORT)->ShowWindow(SW_SHOW);

		m_oMapDisplayDlg.ShowWindow(SW_HIDE);
		m_oTableODDlg.ShowWindow(SW_SHOW);
	}
}

void KLinkAnalysisDlg::OnBnClickedButtonDraw()
{
	DoneDraw(true);
}

void KLinkAnalysisDlg::DoneDraw(bool a_bShowErrMsg) 
{
	TxLogDebugStartMsg();
	m_vecDrawLinkInfo.clear();

	m_oMapDisplayDlg.NewGetLinkInfo(m_vecDrawLinkInfo, a_bShowErrMsg);
	//m_oMapDisplayDlg.OldGetLinkInfo(m_vecDrawLinkInfo);

	DrawLinkVolume();
	TxLogDebugEndMsg();
}


void KLinkAnalysisDlg::DrawLinkVolume()
{
	try
	{
		if(m_vecDrawLinkInfo.size()< 1)
			return;

		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL == pMapView)
		{
			return;
		}

		pMapView->ResetScreenDrawEdit();

		if( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1) )
			pMapView->DrawLinkViaPathThemes(m_vecDrawLinkInfo, m_vecSelectedLinkID, true);
		else
			pMapView->DrawLinkViaPathThemes(m_vecDrawLinkInfo, m_vecSelectedLinkID, false);

	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KLinkAnalysisDlg::OnBnClickedButtonClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CleanMap();
}


void KLinkAnalysisDlg::CleanMap()
{
	try
	{
		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL != pMapView)
		{
			ClearLinkViaPath();
			pMapView->ResetScreenDrawEdit();
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KLinkAnalysisDlg::ClearLinkViaPath()
{
	try
	{
		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL != pMapView)
		{
			pMapView->ClearLinkViaPathThemes(); //★ MapRefresh
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KLinkAnalysisDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	CXTPReportRecord*	  pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_LINKID);
	
	Integer nxLinkID= pItem->GetItemData();

	try
	{
		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL != pMapView)
		{
			pMapView->PanToFeature(KLayerID::Link(), nxLinkID);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

BOOL KLinkAnalysisDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
	TxLogDebugVisitor();
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

	return KDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

CString KLinkAnalysisDlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s\\%s\\%s"), m_pTarget->GetLocation(), BUSH_PATHVOLUMN_FOLDER_NAME, a_strFileName);

	return strSavePath;
}


void KLinkAnalysisDlg::DrawSelectedLink()
{
    ITxFeatureLayerPtr spLayerLink = m_pMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayerLink == nullptr)
        return;

    int nLayerID = KLayerID::LayerID_SelectedLinkAnalysis();
    ITxFeatureUserLayerPtr spLayer = m_pMapView->AddUserLayer(nLayerID); {
        spLayer->Delete();
    }

    TxSymbolLinePtr spSymbol = TxSymbolLine::CreateOffsetLineSymbol(2, Gdiplus::Color(150, 0, 0, 255)); {
        spLayer->LayerDispSymbol(spSymbol);
    }

    for (size_t i = 0; i<m_vecLinkInfo.size(); i++) {
        TLinkInfo& oInfo = m_vecLinkInfo[i];
        __int64 nxID = oInfo.nxLinkID;

        ITxGeometryPtr spGeometry = spLayerLink->GetGeometry(nxID);
        if (spGeometry) {
            spLayer->Insert(nxID, spGeometry);
        }
    }
}

void KLinkAnalysisDlg::OnCbnSelchangeComboMode()
{
	UpdateData(TRUE);

	int nSelCur = m_cboMode.GetCurSel();
	int nIdx    = m_cboMode.GetItemData(nSelCur);

	AutoType iter = m_mapModeInfo.find(nIdx);
	if (iter != m_mapModeInfo.end())
	{
		m_strPreSelectedModeName = iter->second;
	}

	DoneSearch(true);
}


void KLinkAnalysisDlg::OnBnClickedCheckLabel()
{
	DrawLinkVolume();
}