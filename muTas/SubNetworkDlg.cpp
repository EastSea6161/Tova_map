// SubNetworkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SubNetworkDlg.h"
#include "afxdialogex.h"

#include "Target.h"
#include "MapView.h"

#include "Project.h"
#include "Scenario.h"

#include "WriteCodeAgrument.h"

#include "DBaseAssignment.h"
#include "Launcher.h"
#include "KBulkDBase2File.h"
#include "InspectionNetwork.h"
#include "DBaseConnector.h"
#include "DBaseSubNetwork.h"
#include "Purpose.h"
#include "DefineSystemCodeDlg.h"

#include "ImChampDir.h"
#include "KBulkDBase2File.h"
#include "ImChampFrameWindow.h"

// KSubNetworkDlg 대화 상자입니다.
const TCHAR* T_ARG_SUBNETWORK = _T("genSubOD.arg");

IMPLEMENT_DYNAMIC(KSubNetworkDlg, KResizeDialogEx)

KSubNetworkDlg::KSubNetworkDlg(
	KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KSubNetworkDlg::IDD, pParent)
	, m_pMapView(a_pMapView)
	, m_pTarget(a_pTarget)
	, m_nRunOption(-1)
	, m_nZoneThrough(-1)
	, m_strLocation(_T(""))
	, m_bSelectSubArea(false)
	, m_strPrjName(_T(""))
	, m_pSubProject(NULL)
{

}

KSubNetworkDlg::~KSubNetworkDlg()
{
}

void KSubNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReportCtrlInfo);
	DDX_Control(pDX, IDC_REPORT2, m_wndReportCtrlMode);
	DDX_Text(pDX, IDC_PRJEDT_LOCATION, m_strLocation);
}


BEGIN_MESSAGE_MAP(KSubNetworkDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KSubNetworkDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &KSubNetworkDlg::OnBnClickedButtonPolygon)
	ON_BN_CLICKED(IDC_PRJBTN_LOCATION, &KSubNetworkDlg::OnClickedPrjbtnLocation)
	ON_BN_CLICKED(IDCANCEL, &KSubNetworkDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &KSubNetworkDlg::OnBnClickedCheck1)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// KSubNetworkDlg 메시지 처리기입니다.

BOOL KSubNetworkDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
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

CString KSubNetworkDlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s\\%s\\%s"), m_pTarget->GetLocation(), BUSH_PATHVOLUMN_FOLDER_NAME, a_strFileName);

	return strSavePath;
}

void KSubNetworkDlg::ReceiveStatusMessageFromDll( void* pParam1, int nStatus, char* strMsg )
{
	AddStatusMessage(CString(strMsg));
}

void KSubNetworkDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

BOOL KSubNetworkDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);
	
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		TRunOptionInfo oRunOptionInfo = {};
		KDBaseSubNetwork::GetDBRunOptionInfo(m_pTarget, oRunOptionInfo);

		m_nRunOption   = oRunOptionInfo.nRunOption;
		m_nZoneThrough = oRunOptionInfo.nZoneThrough;

		KIOTables* pTables    = m_pTarget->Tables();
		KIOTable*        pLinkTable = pTables->FindTable(TABLE_LINK);

		const KIOColumns* pColumns = pLinkTable->Columns();
		int nIndex = pColumns->GetColumnIndex(oRunOptionInfo.strAvgColumnName);

		KIOColumn* pColumn_LinkTime = NULL;
		if(nIndex < 0)
			pColumn_LinkTime = NULL;
		else
			pColumn_LinkTime = pColumns->GetColumn(nIndex);

		if(NULL != pColumn_LinkTime)
			m_oUserColumnsLink.AddNewColumn(pColumn_LinkTime);

		KReportCtrlSetting::Default(m_wndReportCtrlInfo, FALSE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlInfo);

		KReportCtrlSetting::Default(m_wndReportCtrlMode, FALSE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlMode);

		InitInfoCtrl();

		InitModeCtrl();
		UpdateModeCtrl();

		//Update Location
		CString strPath = theApp.GetProfileString(_T("Common"), _T("ProjectLocation"));
		PathRemoveFileSpec((LPWSTR)(LPCWSTR)strPath);
		if (!strPath.IsEmpty())
		{
			SetDlgItemText(IDC_PRJEDT_LOCATION, strPath);
			m_strLocation = strPath;
		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in init dialog"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KSubNetworkDlg::OnBnClickedButtonPolygon()
{
	CButton* pButton              = (CButton*)GetDlgItem(IDC_CHECK1);
	bool     bIncludeZoneBoundary = false;

	if (pButton->GetCheck() == BST_CHECKED)
		bIncludeZoneBoundary = true;

	m_pMapView->MapSelectionByPolygon(this, 2, bIncludeZoneBoundary);
	
}

void KSubNetworkDlg::MapAnalysisFeedback( vector<TReceiveValue> a_vecReceiveValue )
{
	try
	{
		InitNetworkInfo();
		CalcNetworkSubFiled(a_vecReceiveValue);
		UpdateInfoCtrl();

		m_bSelectSubArea = true;
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{

	}
}

void KSubNetworkDlg::InitNetworkInfo( void )
{
	//Set Node Map

	m_mapNodeSubField.clear();

	vector<__int64> vecNode;
	KBulkDBaseNode::Nodes(m_pTarget, vecNode);

	int nNodeCount = vecNode.size();

	for (int i = 0; i < nNodeCount; ++i)
	{
		m_mapNodeSubField.insert(make_pair(vecNode[i], 0));
	}

	//Set Zone Map;

	m_mapZoneNode.clear();
	vector<__int64> vecZone;
	KBulkDBaseNode::Nodes(m_pTarget, vecZone, 0);

	int nZoneCount = vecZone.size();

	for (int i = 0; i < nZoneCount; ++i)
	{
		m_mapZoneNode.insert(vecZone[i]);
	}

	//Set Link Map

	m_mapLinkSubField.clear();

	vector<__int64> vecLink;
	KBulkDBaseLink::Links(m_pTarget, vecLink);

	int nLinkCount = vecLink.size();

	for (int i = 0; i < nLinkCount; ++i)
	{
		m_mapLinkSubField.insert(make_pair(vecLink[i], 0));
	}
}

void KSubNetworkDlg::CalcNetworkSubFiled( vector<TReceiveValue>& ar_vecReceiveValue )
{
	int nSize = ar_vecReceiveValue.size();

	TReceiveValue oReceiveValue;

	//Node 위치 계산
	//#0, 0 - 경계 밖 외부노드, 1 - Sub Network 내부, 2 - 경계 링크와 연결된 외부 노드;
	map<__int64, int>::iterator findIterNode, endIterNode = m_mapNodeSubField.end();
    
	for (int i = 0; i < nSize; ++i)
	{
		oReceiveValue = ar_vecReceiveValue[i];

		findIterNode = m_mapNodeSubField.find(oReceiveValue.m_nxFNodeID);

		if (findIterNode == endIterNode)
		{
			AfxMessageBox(_T("error link 250"));
			continue;
		}
		
		if (oReceiveValue.m_nFNodeInOut == KEMSubODNodeOuter)
		{
			if (oReceiveValue.m_nTNodeInOut == KEMSubODNodeOuter)
				continue;
			
			findIterNode->second = KEMSubODNodeCross;
		}
		else if(oReceiveValue.m_nFNodeInOut == KEMSubODNodeInner)
		{
			findIterNode->second = KEMSubODNodeInner;
		}
		else
		{
			AfxMessageBox(_T("Error1"));
		}

		findIterNode = m_mapNodeSubField.find(oReceiveValue.m_nxTNodeID);

		if (oReceiveValue.m_nTNodeInOut == KEMSubODNodeOuter)
		{
			if (oReceiveValue.m_nFNodeInOut == KEMSubODNodeOuter)
				continue;

			findIterNode->second = KEMSubODNodeCross;
		}
		else if(oReceiveValue.m_nTNodeInOut == KEMSubODNodeInner)
		{
			findIterNode->second = KEMSubODNodeInner;
		}
		else
		{
			AfxMessageBox(_T("Error3"));
		}
	}

	map<__int64, int> mapNodeCheck;

	for (findIterNode = m_mapNodeSubField.begin(); findIterNode != endIterNode; ++findIterNode)
	{
		if (findIterNode->second == 0)
			continue;

		mapNodeCheck.insert(make_pair(findIterNode->first, findIterNode->second));
	}


	//Link 위치 계산
	//#1, 0 - 외부 링크, 1 - FNode가 외부인 경계교차 링크, 2 - ToNode가 외부인 경계교차 링크, 3 - SubNetwork 내부링크
	map<__int64, int>::iterator findIterLink, endIterLink = m_mapLinkSubField.end();

	for (int i = 0; i < nSize; ++i)
	{
		oReceiveValue = ar_vecReceiveValue[i];

		Integer nxLinkID = oReceiveValue.m_nxLinkID;
				
		findIterLink = m_mapLinkSubField.find(oReceiveValue.m_nxLinkID);

		if (findIterLink == endIterLink)
		{
			AfxMessageBox(_T("링크 ID 매칭에 실패하였습니다."));
			return;
		}

		if (oReceiveValue.m_nFNodeInOut == KEMSubODNodeOuter && oReceiveValue.m_nTNodeInOut == KEMSubODNodeOuter)
		{
			findIterLink->second = KEMSubODLinkFullOut;
		}
		else if (oReceiveValue.m_nFNodeInOut == KEMSubODNodeOuter && oReceiveValue.m_nTNodeInOut == KEMSubODNodeInner)
		{
			findIterLink->second = KEMSubODLinkOnlyFNodeOut;
		}
		else if (oReceiveValue.m_nFNodeInOut == KEMSubODNodeInner && oReceiveValue.m_nTNodeInOut == KEMSubODNodeOuter)
		{
			findIterLink->second = KEMSubODLinkOnlyTNodeOut;
		}
		else if (oReceiveValue.m_nFNodeInOut == KEMSubODNodeInner && oReceiveValue.m_nTNodeInOut == KEMSubODNodeInner)
		{
			findIterLink->second = KEMSubODLinkInner;
		}
		else
		{
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
			return;
		}
	}

	mapNodeCheck.clear();

	for (findIterLink = m_mapLinkSubField.begin(); findIterLink != endIterLink; ++findIterLink)
	{
		if (findIterLink->second == 0)
			continue;

		mapNodeCheck.insert(make_pair(findIterLink->first, findIterLink->second));
	}

	TxLogDebugEndMsg();
	
}


void KSubNetworkDlg::InitInfoCtrl( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nColumnIndex(0);

	pColumn = m_wndReportCtrlInfo.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Item"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("네트워크 구성"));
    }
	
	pColumn = m_wndReportCtrlInfo.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Value"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("개수"));
    }

	m_wndReportCtrlInfo.Populate();
}

void KSubNetworkDlg::UpdateInfoCtrl( void )
{
	m_wndReportCtrlInfo.ResetContent();

	int nInnerZones(0), nInnerNodes(0), nInnerLinks(0), nCutLinks(0);

	map<__int64, int>::iterator findIterNode, endIterNode = m_mapNodeSubField.end();

	set<__int64>::iterator findIterZone, endIterZone = m_mapZoneNode.end();

	for (findIterNode = m_mapNodeSubField.begin(); findIterNode != endIterNode; ++findIterNode)
	{
		if (findIterNode->second != KEMSubODNodeInner)
			continue;
		
		++nInnerNodes;	// Update InnerNode Value

		findIterZone = m_mapZoneNode.find(findIterNode->first);

		if (findIterZone == endIterZone)
			continue;

		++nInnerZones;	// Update InnerZone Value
	}

	map<__int64, int>::iterator findIterLink, endIterLink = m_mapLinkSubField.end();

	for (findIterLink = m_mapLinkSubField.begin(); findIterLink != endIterLink; ++findIterLink)
	{
		if (findIterLink->second == KEMSubODLinkInner)
			++nInnerLinks;

		if (findIterLink->second == KEMSubODLinkOnlyFNodeOut || findIterLink->second == KEMSubODLinkOnlyTNodeOut)
			++nCutLinks;
	}

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem  = nullptr;

	//InnerZones
	pRecord = m_wndReportCtrlInfo.AddRecord(new CXTPReportRecord);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("내부 존")));
	}
	else {
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Inner-Zones")));
	}
	//pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Inner-Zones")));
	pItem	= pRecord->AddItem(new CXTPReportRecordItemNumber((double)nInnerZones, _T("%.0f")));

	//InnerNodes
	pRecord = m_wndReportCtrlInfo.AddRecord(new CXTPReportRecord);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("내부 노드")));
	}
	else {
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Inner-Nodes")));
	}
	//pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Inner-Nodes")));
	pItem	= pRecord->AddItem(new CXTPReportRecordItemNumber((double)nInnerNodes, _T("%.0f")));

	//InnerLinks
	pRecord = m_wndReportCtrlInfo.AddRecord(new CXTPReportRecord);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("내부 링크")));
	}
	else {
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Inner-Links")));
	}
	//pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Inner-Links")));
	pItem	= pRecord->AddItem(new CXTPReportRecordItemNumber((double)nInnerLinks, _T("%.0f")));

	//CutLinks
	pRecord = m_wndReportCtrlInfo.AddRecord(new CXTPReportRecord);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("외부 연결 링크")));
	}
	else {
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Cut Links")));
	}
	//pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Cut Links")));
	pItem	= pRecord->AddItem(new CXTPReportRecordItemNumber((double)nCutLinks, _T("%.0f")));
	
	m_wndReportCtrlInfo.Populate();
}

void KSubNetworkDlg::InitModeCtrl( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nColumnIndex(0);

	pColumn = m_wndReportCtrlMode.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Mode"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }

	pColumn = m_wndReportCtrlMode.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Select"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	m_wndReportCtrlMode.Populate();
}

void KSubNetworkDlg::UpdateModeCtrl( void )
{
	KDBaseSubNetwork::GetDBModeInfo(m_pTarget, m_mapModeInfo);

	map<int, CString>::iterator beginIter, endIter = m_mapModeInfo.end();

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for (beginIter = m_mapModeInfo.begin(); beginIter != endIter; ++beginIter)
	{
		pRecord = m_wndReportCtrlMode.AddRecord(new CXTPReportRecord);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(beginIter->second));
		pItem->SetItemData(beginIter->first);

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->HasCheckbox(TRUE);
		pItem->SetEditable(TRUE);
	}

	m_wndReportCtrlMode.Populate();
}

void KSubNetworkDlg::ValidateInput( void )
{
	//#1. Select Area Check
	if (!m_bSelectSubArea)
		ThrowException(_T("Sub Network를 진행할 영역을 선택하세요."));

	//#2. Select Mode Check
	CXTPReportRecords*    pRecords = m_wndReportCtrlMode.GetRecords();
	CXTPReportRecordItem* pItem = nullptr;

	int  nRecords = pRecords->GetCount();
	bool bChkMode = false;

	for (int i = 0; i < nRecords; ++i)
	{
		pItem = pRecords->GetAt(i)->GetItem(1);

		if (pItem->IsChecked())
			bChkMode = true;
	}

	if (!bChkMode)
		ThrowException(_T("수단을 선택하세요."));

	//#3. New Project Name Check
	CString strPrjName(_T(""));
	GetDlgItemText(IDC_EDIT1, strPrjName);
	m_strPrjName = strPrjName.Trim();

	if (m_strPrjName.IsEmpty())
		ThrowException(_T("신규 프로젝트 명칭을 입력하세요."));

	//#4. Project Location Check
	m_strLocation = m_strLocation.Trim();

	if (m_strLocation.IsEmpty())
		ThrowException(_T("경로를 선택하세요."));

	//#5. Check Project Path
	//CString strPrjPath = m_strLocation + _T("\\") + m_strPrjName;

	CString strPrjPath;
	strPrjPath.Format(_T("%s%s%s"), m_strLocation, _T("\\"), m_strPrjName);

	CFileFind findFile;

	if (findFile.FindFile(strPrjPath))
	{
		findFile.FindNextFile();

		if (findFile.IsDirectory() == TRUE)
			ThrowException(_T("해당 프로젝트 경로가 이미 존재합니다."));
	}
}

void KSubNetworkDlg::OnBnClickedOk()
{
	try
	{
		ValidateInput();

		ThreadPara oPara(this);
		if (true) { // 데이터 검증 및 중간 바이너리 작성
			oPara.TBusiness = 0;
			oPara.TKeyPtr[0] = 0; //생성 완료된 Subnetwork의 KTarget 주소

			RThreadInfo.Init();

			// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
			int nLang = 1;
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				nLang = 0;
			}

			QBicSimpleProgressThread::ExecuteThread(SubNetworkThreadCaller, &oPara, false, nLang); 

			if (RThreadInfo.IsOK() == false) {
				ThrowException(RThreadInfo.ErrorMsg());
			} else {
				AfxMessageBox(_T("부분 네트워크 자르기가 성공적으로 완료되었습니다."));

				KTarget* pSubTarget = (KTarget*)oPara.TKeyPtr[0];
				if (nullptr != pSubTarget) {
					KDefineSystemCodeDlg oSystemDefineDlg(pSubTarget, true);
					oSystemDefineDlg.DoModal();
				}
			}
		}
				
		if (m_pSubProject != NULL)
		{
			delete m_pSubProject;
			m_pSubProject = NULL;
		}
	}
	catch (KExceptionPtr ex)
	{
		if (m_pSubProject != NULL)
		{
			delete m_pSubProject;
			m_pSubProject = NULL;
		}
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		if (m_pSubProject != NULL)
		{
			delete m_pSubProject;
			m_pSubProject = NULL;
		}
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}

	//KResizeDialogEx::OnOK();
}

unsigned __stdcall KSubNetworkDlg::SubNetworkThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	if (pParameter != nullptr) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KSubNetworkDlg* pDlg = (KSubNetworkDlg*)pPara->TWindow;

			if (0 == pPara->TBusiness)
				pDlg->Run(pPara);
		}
	}

	return 0;
}

void KSubNetworkDlg::Run( ThreadPara* pPara )
{
	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 입력 데이터 생성 중..."));
		}
		else {
			AddStatusMessage(_T("Creating Argument File..."));
		}

		if (CreateArgument() == false) {
			ThrowException(_T("Argument-Creat failed"));
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 입력 데이터 생성 완료."));
		}
		else {
			AddStatusMessage(_T("Argument File Created."));
		}

		CString strWindowName;
		GetWindowText(strWindowName);

		KLauncher::Run(strWindowName, 20);

		if (m_nResultCode != DLL_PLANNED_TERMINATION)
		{
			if (m_nResultCode == DLL_LOAD_ERROR)
			{
				ThrowException(_T("dll not found..."));
			}
			else if (m_nResultCode == DLL_USER_STOPPING || QBicSimpleProgressThread::IsUserStopped())
			{
				m_nResultCode = DLL_USER_STOPPING;
				ThrowException(_T("subnetwork analysis stop..."));
			}
		}			

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
		}
		CreateSubNetwork(pPara); // pPara->TKeyPtr[0]에 새로 생성된 Target의 주소가 할당된다.

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("부분 네트워크 지도 데이터 생성 중..."));
		}
		else {
			AddStatusMessage(_T("Creating Sub Network Geometry..."));
		}
		CreateSubGeometry(pPara);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		RThreadInfo.SetErrorFlag(-1, strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	} catch (...) {
		TxLogDebugException();
		RThreadInfo.SetErrorFlag(-1, _T("unknown error"));
	}
}

void KSubNetworkDlg::WriteTextArgument( CStdioFileEx& of )
{
	CString strOutLine;

	KObject* pScenario = m_pTarget->GetParentObject();

	KProject* pProject = (KProject*)pScenario->GetParentObject();

	//Name
	strOutLine.Format(_T("%s_%s_%s\r\n"), pProject->GetName(), pScenario->GetName(), m_pTarget->GetName());
	of.WriteString(strOutLine);

	//int nInputLine = m_mapModeInfo.size() + 1;

	std::vector<TAssignPathInput> vecAssignPathInput;

	KDBaseAssignment::GetAssignPathInputInfo(m_pTarget, vecAssignPathInput);

	int nSize = vecAssignPathInput.size();

	//Input
	strOutLine.Format(_T("Input\t%d\r\n"), nSize + 1);
	of.WriteString(strOutLine);

	//Using Day OD, AnalysisTime
	strOutLine.Format(_T("%d\t%.1f\r\n"), 1, 10.0);
	of.WriteString(strOutLine);

	for (int i = 0; i < nSize; ++i)
	{
		TAssignPathInput oAssignPathInput = vecAssignPathInput[i];

		CXTPReportRecords* pRecords = m_wndReportCtrlMode.GetRecords();

		int nRecordCount = pRecords->GetCount();

		int nSelected = 0;	// 0 - None, 1 - Selected

		for (int j = 0; j < nRecordCount; ++j)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(j);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

			int nModeID = pItem->GetItemData();

			pItem = pRecord->GetItem(1);

			if (nModeID == oAssignPathInput.nModeID && pItem->IsChecked())
				nSelected = 1;
		}

		strOutLine.Format(_T("%f\t%f\t%d\t%d\t%d\r\n"), 
			oAssignPathInput.dOccupancy, oAssignPathInput.dPCU, nSelected, oAssignPathInput.nPreload, (i+1));
		of.WriteString(strOutLine);
	}

	//run_options
	strOutLine.Format(_T("run_options\t%d\r\n"), m_nRunOption);
	of.WriteString(strOutLine);

	//ZoneThru
	strOutLine.Format(_T("ZoneThru\t%d\r\n"), m_nZoneThrough);
	of.WriteString(strOutLine);

	//ReadBushFiles
	strOutLine.Format(_T("%s\\PATHVOLUME\r\n"), m_pTarget->GetLocation());
	of.WriteString(strOutLine);
}

bool KSubNetworkDlg::CreateArgument( void )
{
	TxLogDebugStartMsg();
	
	try 
	{
		ImChampDir::SetCurrentDirectoryToApp();

		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget, BUSH_PATHVOLUMN_FOLDER_NAME);
		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;


		//Argument 파일 생성
		CStdioFileEx outFile(T_ARG_SUBNETWORK, nOpenMode);
		WriteTextArgument(outFile);
		outFile.Close();

		CString strFilePath(_T(""));

		set<int>     setIncludeNodeCode;
		set<int>     setIncludeLinkCode;
		set<Integer> setUsableNodeData;
		set<Integer> setUsableLinkData;

		KDBaseAssignment::GetAssignmentTypeData(m_pTarget, setIncludeNodeCode, setIncludeLinkCode);
		KInspectionNetwork::Inspection(m_pTarget, setIncludeNodeCode, setIncludeLinkCode, setUsableNodeData, setUsableLinkData);

		//Node 파일 생성
		strFilePath = ConvertSavePath(_T("node.dat"));
		KDBaseSubNetwork::ExportDB2NodeFile(m_pTarget, setUsableNodeData, m_mapNodeSubField, strFilePath);

		//Link 파일 생성
		strFilePath = ConvertSavePath(_T("link.dat"));
		KDBaseSubNetwork::ExportDB2LinkFile(m_pTarget, setUsableLinkData, m_mapLinkSubField, m_oUserColumnsLink, strFilePath);

		//Turn 파일 생성
		strFilePath = ConvertSavePath(_T("turn.dat"));
		KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(), strFilePath);
	} catch (...) {
		TxLogDebugException();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}

void KSubNetworkDlg::OnClickedPrjbtnLocation()
{
	KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();
	KProject* pProject   = (KProject*)pScenario->GetParentObject();
	
	//초기경로
	CString strLocation = pProject->GetLocation();

	UpdateData(TRUE);

	ITEMIDLIST *pidlBrowse;
	TCHAR szPathName[1024] = {0};
	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = GetSafeHwnd();
	browseInfo.pidlRoot = NULL;

	memset( &browseInfo, 0, sizeof(browseInfo) );
	browseInfo.pszDisplayName = szPathName;
	browseInfo.lpszTitle = _T("경로 선택");
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	browseInfo.lParam  = (LPARAM)strLocation.GetBuffer( strLocation.GetLength() );
	browseInfo.lpfn    = BrowseCallBackProc;

	browseInfo.hwndOwner = this->GetSafeHwnd();


	pidlBrowse = ::SHBrowseForFolder(&browseInfo);

	if( pidlBrowse )
	{
		::SHGetPathFromIDList(pidlBrowse, szPathName);
		m_strLocation = szPathName;
	}
	UpdateData(FALSE);
}

void KSubNetworkDlg::CreateSubNetwork(ThreadPara* pPara)
{
	TxLogDebugStartMsg();

	CString strOutFile = ConvertSavePath(_T("node_out.dat"));
	ifstream ifs(strOutFile, ios::binary); {
		if (!ifs) {
			ifs.close();
			ThrowException(_T("node_out.out 파일을 읽을 수 없습니다."));
		}
	}

	strOutFile = ConvertSavePath(_T("link_out.dat"));
	ifstream ifsLink(strOutFile, ios::binary); {
		if (!ifsLink) {
			ifsLink.close();
			ThrowException(_T("node_out.out 파일을 읽을 수 없습니다."));
		}
	}

	strOutFile = ConvertSavePath(_T("sub_od.out"));
	ifstream ifsMode(strOutFile, ios::binary); {
		if (!ifsMode) {
			ifsMode.close();
			ThrowException(_T("sub_od.out 파일을 읽을 수 없습니다."));
		}
	}

	KProject* pSrcProject = ((CKmzApp*)AfxGetApp())->GetProject();

	if (m_pSubProject != NULL)
	{
		delete m_pSubProject;
		m_pSubProject = NULL;
	}
	TxLogDebugVisitor();

	m_pSubProject = new KProject;

	m_pSubProject->SetProjectType(KEMUserDefineProject);
	m_pSubProject->SetName(m_strPrjName);

	m_pSubProject->SetInvestigator(pSrcProject->getInvestigator());
	m_pSubProject->SetOrganization(pSrcProject->SetOrganization());
	m_pSubProject->SetDescription(pSrcProject->GetDescription());

	CString strPrjPath;
	strPrjPath.Format(_T("%s%s%s"), m_strLocation, _T("\\"), m_strPrjName);

	m_pSubProject->SetLocation(strPrjPath);
	//-UserDefine 에서는 사용 되지 않는 정보들 - 예외 값으로 셋팅
	m_pSubProject->SetSubJectAreaInfo(-1, _T(""), _T(""));
	m_pSubProject->SetAnalysisAreaId(-1);
	m_pSubProject->SetAnalysisAreaName(_T(""));
	m_pSubProject->SetAnalysisFolderName(_T(""));

	m_pSubProject->SetBaseYear(pSrcProject->getBaseYear());

	vector<int>	vecTargetYears;
	m_pSubProject->SetTargetYear(vecTargetYears);

	//Set PurPose
	m_vecPurpose.clear();

	std::vector<CString> vecPurposeNames;
	std::vector<int>	 vecPurposeIds;

	m_vecPurpose = pSrcProject->GetPurpose();

	int nPurposes = m_vecPurpose.size();

	for (int i = 0; i <nPurposes; ++i)
	{
		KPurpose* pPurpose = m_vecPurpose[i];

		vecPurposeNames.push_back(pPurpose->PurposeName());
		vecPurposeIds.push_back(pPurpose->PurposeID());
	}

	//Set Mode;
	m_vecMode.clear();
	
	std::vector<CString> vecModeNames;
	std::vector<int>	 vecModeIds;

	CXTPReportRecords* pRecords = m_wndReportCtrlMode.GetRecords();

	int nModes = pRecords->GetCount();
	int nModeIndex(1);

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;
	KMode*                pMode   = nullptr;

	for (int i = 0; i < nModes; ++i)
	{
		pRecord =  pRecords->GetAt(i);
		pItem = pRecord->GetItem(1);

		if (!pItem->IsChecked())
			continue;

		pItem = pRecord->GetItem(0);
		
		vecModeNames.push_back(pItem->GetCaption());
		vecModeIds.push_back(nModeIndex);

		pMode = new KMode(KEMModeGroupODBase, nModeIndex, pItem->GetCaption());
		m_vecMode.push_back(pMode);

		++nModeIndex;
	}

	m_pSubProject->SetPurposeName(vecPurposeNames);
	m_pSubProject->SetPurposeId(vecPurposeIds);
	m_pSubProject->SetPurpose(m_vecPurpose);

	m_pSubProject->SetModeName(vecModeNames);
	m_pSubProject->SetModeId(vecModeIds);
	m_pSubProject->SetMode(m_vecMode);

	m_pSubProject->CreateInitialObject();

	//Set Base Scenario

	KScenario* pBaseScenario = m_pSubProject->CreateScenario(_T("BASE"));
	CString    strTargetName;
	strTargetName.Format(_T("%d"), pSrcProject->getBaseYear());

	pBaseScenario->SetMode(m_vecMode);
	
	pBaseScenario->CreateTarget(pSrcProject->getBaseYear(), strTargetName);
	
	CString strFile = m_pSubProject->GetLocation();
	strFile.Append(_T("\\"));
	strFile.Append(m_pSubProject->GetName());

	if (KmzSystem::IsPassengerSystem())
	{
		strFile.Append(_T(".tpx"));
	}
	else
	{
		strFile.Append(_T(".kfx"));
	}

	::WritePrivateProfileString( _T("APP"), _T("DESC"), _T("TOVA Project File"), strFile );
	
	pBaseScenario->ImportBLANK();

	m_pSubProject->SaveProject(strFile);

	
	//iolocal.db 경로를 어떻게 구하낭...
	const KTDTargetMap& targetMap = pBaseScenario->GetTargetMap();

	KTDTargetMap::const_iterator beginIter, endIter = targetMap.end();
	KTarget* pTarget = nullptr;

	CString strIODataFilePath  = _T("");

	for (beginIter = targetMap.begin(); beginIter != endIter; ++beginIter)
	{
		pTarget = beginIter->second;
		strIODataFilePath  = pTarget->GetLocation();
	}

	if (nullptr == pTarget) {
		ThrowException(_T("SubNetwork Target 정보 생성 실패"));
	}

	pPara->TKeyPtr[0] = (DWORD_PTR)pTarget; // 생성될 SubNetwork의 Target 주소

	strIODataFilePath.AppendFormat(_T("\\%s"),  IODATAFILE_NAME);

	KDBaseConPtr spBaseConnectionPtr = KDBase::GetSubODDBConnection(strIODataFilePath);

	//input Node
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	KBulkDBaseNode::AllNodeFixedValue(spDBaseConnection, m_mapNodeFixedInfo);

	CString strQuery = _T("Insert Into Node(");
	strQuery.Append(_T("node_id, node_type, name, x, y, zone_id, target_region, parking_fee"));
	strQuery.Append(_T(") Values("));
	strQuery.Append(_T("?, ?, ?, ?, ?, 0, ?, ?)"));

	KPreparedStatementPtr spPreparedStatementPtr = spBaseConnectionPtr->PrepareStatement(strQuery);

	map<Integer, TNodeFixedColumn>::iterator findNodeIter, endNodeIter = m_mapNodeFixedInfo.end();

	__int64 nxNodeID;
	int     nNodeType;

	set<Integer> setNode;	//nodetype이 존이 아닌 Node;
	set<Integer> setZone;
	set<Integer> setLink;	//linkID;

	set<int> setIncludeNodeType;
	set<int> setIncludeLinkType;

	while (!ifs.eof())
	{
		//node_id
		ifs.read(reinterpret_cast<char*>(&nxNodeID),  sizeof(__int64));
		//node_type
		ifs.read(reinterpret_cast<char*>(&nNodeType), sizeof(int));

		if (ifs.eof())
			break;

		spPreparedStatementPtr->BindInt64(1, nxNodeID);
		spPreparedStatementPtr->BindInt64(2, nNodeType);

		setIncludeNodeType.insert(nNodeType);

		if ((findNodeIter = m_mapNodeFixedInfo.find(nxNodeID)) == endNodeIter)
		{
			AfxMessageBox(_T("노드 ID 매칭에 실패하였습니다."));
			CString strLog;
			strLog.Format(_T("dll nodeID : %I64d unmatching..."), nxNodeID);
			TxLogDebug(strLog.AllocSysString());
		}

		TNodeFixedColumn oNodeFixedColumn = findNodeIter->second;

		spPreparedStatementPtr->BindText  (3, oNodeFixedColumn.strName);
		spPreparedStatementPtr->BindDouble(4, oNodeFixedColumn.dX);
		spPreparedStatementPtr->BindDouble(5, oNodeFixedColumn.dY);
		spPreparedStatementPtr->BindInt   (6, oNodeFixedColumn.nTargetRegion);
		spPreparedStatementPtr->BindDouble(7, oNodeFixedColumn.dParkingFee);

		spPreparedStatementPtr->ExecuteUpdate();
		spPreparedStatementPtr->Reset();

		if (nNodeType != 0)
		{
			setNode.insert(nxNodeID);
		}
		else
		{
			setZone.insert(nxNodeID);
		}
	}

	ifs.close();
	
	//insert zone;
	strQuery = _T("Insert Into Zone(zone_id) Values(?)");
	spPreparedStatementPtr = spBaseConnectionPtr->PrepareStatement(strQuery);

	set<Integer>::iterator beginZoneIter, endZoneIter = setZone.end();

	for (beginZoneIter = setZone.begin(); beginZoneIter != endZoneIter; ++beginZoneIter)
	{
		spPreparedStatementPtr->BindInt64(1, *beginZoneIter);

		spPreparedStatementPtr->ExecuteUpdate();
		spPreparedStatementPtr->Reset();
	}


	//input link

	//KBulkDBaseNode::AllNodeFixedValue(oDBaseConnection, m_mapNodeFixedInfo);
	KBulkDBaseLink::AllLinkFixedValue(spDBaseConnection, m_mapLinkFixedInfo);

	strQuery = _T("Insert Into Link(");
	strQuery.Append(_T("link_id, link_type, fnode_id, tnode_id, name, "));
	strQuery.Append(_T("length, lane, capa, speed, survey_volume, "));
	strQuery.Append(_T("influence, function_id, alpha, beta, a, "));
	strQuery.Append(_T("b"));
	strQuery.Append(_T(") Values("));
	strQuery.Append(_T("?, ?, ?, ?, ?, "));
	strQuery.Append(_T("?, ?, ?, ?, ?, "));
	strQuery.Append(_T("?, ?, ?, ?, ?,"));
	strQuery.Append(_T("?)"));


	spPreparedStatementPtr = spBaseConnectionPtr->PrepareStatement(strQuery);

	map<Integer, TLinkFixedColumn>::iterator findLinkIter, endLinkIter = m_mapLinkFixedInfo.end();

	Integer nxLinkID;
	int     nLinkType;

	while (!ifsLink.eof())
	{
		ifsLink.read(reinterpret_cast<char*>(&nxLinkID),  sizeof(Integer));
		ifsLink.read(reinterpret_cast<char*>(&nLinkType), sizeof(int));

		if (ifsLink.eof())
			break;

		setIncludeLinkType.insert(nLinkType);

		if ((findLinkIter = m_mapLinkFixedInfo.find(nxLinkID)) == endLinkIter)
		{
			AfxMessageBox(_T("링크 ID 매칭에 실패하였습니다."));

			CString strLog;
			strLog.Format(_T("dll linkID : %I64d unmatching..."), nxLinkID);
			TxLogDebug(strLog.AllocSysString());
		}

		TLinkFixedColumn oLinkFixedColumn = findLinkIter->second;

		spPreparedStatementPtr->BindInt64 (1,  nxLinkID);
		spPreparedStatementPtr->BindInt   (2,  nLinkType);
		spPreparedStatementPtr->BindInt64 (3,  oLinkFixedColumn.nxFnodeID);
		spPreparedStatementPtr->BindInt64 (4,  oLinkFixedColumn.nxTNodeID);
		spPreparedStatementPtr->BindText  (5,  oLinkFixedColumn.strName);
		spPreparedStatementPtr->BindDouble(6,  oLinkFixedColumn.dLength);
		spPreparedStatementPtr->BindInt   (7,  oLinkFixedColumn.nLane);
		spPreparedStatementPtr->BindDouble(8,  oLinkFixedColumn.dCapa);
		spPreparedStatementPtr->BindDouble(9,  oLinkFixedColumn.dSpeed);
		spPreparedStatementPtr->BindDouble(10, oLinkFixedColumn.dSurveyVolume);
		spPreparedStatementPtr->BindInt   (11, oLinkFixedColumn.nInfluence);
		spPreparedStatementPtr->BindInt   (12, oLinkFixedColumn.nFunctionID);
		spPreparedStatementPtr->BindDouble(13, oLinkFixedColumn.dAlpha);
		spPreparedStatementPtr->BindDouble(14, oLinkFixedColumn.dBeta);
		spPreparedStatementPtr->BindDouble(15, oLinkFixedColumn.dConstantCost);
		spPreparedStatementPtr->BindDouble(16, oLinkFixedColumn.dVariableCost);

		spPreparedStatementPtr->ExecuteUpdate();
		spPreparedStatementPtr->Reset();

		setLink.insert(nxLinkID);
	}

	ifsLink.close();

	//input Turn;
	KBulkDBaseTurn::AllTurnFixedValue(spDBaseConnection, m_mapTurnFixedInfo);

	set<Integer> setDeleteTurn;

	map<Integer, TTurnFixedColumn>::iterator beginTurnIter, endTurnIter = m_mapTurnFixedInfo.end();


	//경유노드 검증
	for (beginTurnIter = m_mapTurnFixedInfo.begin(); beginTurnIter != endTurnIter; ++beginTurnIter)
	{
		TTurnFixedColumn oTurnFixedColumn = beginTurnIter->second;

		Integer nxNodeId = oTurnFixedColumn.nxNodeID;

		if (setNode.find(nxNodeID) == setNode.end())
			setDeleteTurn.insert(oTurnFixedColumn.nxTurnID);
	}

	//진입링크 검증
	for (beginTurnIter = m_mapTurnFixedInfo.begin(); beginTurnIter != endTurnIter; ++beginTurnIter)
	{
		TTurnFixedColumn oTurnFixedColumn = beginTurnIter->second;

		Integer nxInLinkID = oTurnFixedColumn.nxInLinkID;

		if (setLink.find(nxInLinkID) == setLink.end())
			setDeleteTurn.insert(oTurnFixedColumn.nxTurnID);
	}

	//진출링크 검증
	for (beginTurnIter = m_mapTurnFixedInfo.begin(); beginTurnIter != endTurnIter; ++beginTurnIter)
	{
		TTurnFixedColumn oTurnFixedColumn = beginTurnIter->second;

		Integer nxOutLinkID = oTurnFixedColumn.nxOutLinkID;

		if (setLink.find(nxOutLinkID) == setLink.end())
			setDeleteTurn.insert(oTurnFixedColumn.nxTurnID);
	}

	//Turn 제외
	set<Integer>::iterator beginDelTurnIter, endDelTurnIter = setDeleteTurn.end();

	for (beginDelTurnIter = setDeleteTurn.begin(); beginDelTurnIter != endDelTurnIter; ++beginDelTurnIter)
	{
		m_mapTurnFixedInfo.erase(*beginDelTurnIter);
	}


	//Input Mode

	KIOTables* pIOTables  = pTarget->Tables();
	KIOTable*  pModeTable = pIOTables->FindTable(TABLE_MODE_OD);

	strQuery = _T("Insert Into mode_od Values(");

	const KIOColumns* pColumns = pModeTable->Columns();
	int nModeODColumns = pColumns->ColumnCount();

	for (int i = 0; i < nModeODColumns; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		//strQuery.AppendFormat(_T("%s, "), pColumn->Name());
		strQuery.Append(_T("?, "));
	}

	strQuery.Truncate(strQuery.GetLength() - 2);
	strQuery.Append(_T(")"));

	spPreparedStatementPtr = spBaseConnectionPtr->PrepareStatement(strQuery);

	Integer nxValue;
	double  dValue;

	while (!ifsMode.eof())
	{
		int nBindCount(1);

		//OZoneID
		ifsMode.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));

		if (ifsMode.eof())
			break;

		spPreparedStatementPtr->BindInt64(nBindCount++, nxValue);

		//DzoneID
		ifsMode.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
		spPreparedStatementPtr->BindInt64(nBindCount++, nxValue);

		//m1, m2 ...
		for (int i = 2; i < nModeODColumns; ++i)
		{
			ifsMode.read(reinterpret_cast<char*>(&dValue), sizeof(double));
			spPreparedStatementPtr->BindDouble(nBindCount++, dValue);
		}

		spPreparedStatementPtr->ExecuteUpdate();
		spPreparedStatementPtr->Reset();
	}

	ifsMode.close();

	//TABLE_DEFINE_SYSTEM_CODE 복사...
	strQuery.Format(_T("Delete From %s"), TABLE_DEFINE_SYSTEM_CODE);
	spBaseConnectionPtr->ExecuteUpdate(strQuery);

	CString strQuery1;
	strQuery1.Format(_T("Select user_code_group, user_code_key, system_code_group, system_code_key From %s"), TABLE_DEFINE_SYSTEM_CODE);

	KResultSetPtr spResultSet = spBaseConnectionPtr->ExecuteQuery(strQuery1);

	strQuery.Format(_T("Insert Into %s(user_code_group, user_code_key, system_code_group, system_code_key) Values(?, ?, ?, ?)"), 
		TABLE_DEFINE_SYSTEM_CODE);

	spPreparedStatementPtr = spBaseConnectionPtr->PrepareStatement(strQuery);

	while (spResultSet->Next())
	{
		int nBindIndex(1);

		spPreparedStatementPtr->BindInt(nBindIndex++, spResultSet->GetValueInt(0));
		spPreparedStatementPtr->BindInt(nBindIndex++, spResultSet->GetValueInt(1));
		spPreparedStatementPtr->BindInt(nBindIndex++, spResultSet->GetValueInt(2));
		spPreparedStatementPtr->BindInt(nBindIndex++, spResultSet->GetValueInt(3));

		spPreparedStatementPtr->ExecuteUpdate();
		spPreparedStatementPtr->Reset();
	}
	
	//TABLE_KCODE_DEFINE Code 복사...
	strQuery.Format(_T("Delete From %s Where kcode_group_key = %d"), TABLE_KCODE_DEFINE, USER_NODECODE_GROUP_KEY);
	spBaseConnectionPtr->ExecuteUpdate(strQuery);

	strQuery1.Format(_T("Select kcode_group_key, kcode_key, kcode_display From %s Where kcode_group_key = %d"), 
		TABLE_KCODE_DEFINE, USER_NODECODE_GROUP_KEY);

	spResultSet = spDBaseConnection->ExecuteQuery(strQuery1);

	strQuery.Format(_T("Insert Into %s(kcode_group_key, kcode_key, kcode_display) Values(?, ?, ?)"), 
		TABLE_KCODE_DEFINE);

	spPreparedStatementPtr = spBaseConnectionPtr->PrepareStatement(strQuery);

	while (spResultSet->Next())
	{
		int nBindIndex(1);

		AutoType iterFind = setIncludeNodeType.find(spResultSet->GetValueInt(1));
		AutoType iterEnd  = setIncludeNodeType.end();

		if (iterFind == iterEnd)
			continue;

		spPreparedStatementPtr->BindInt (nBindIndex++, spResultSet->GetValueInt(0));
		spPreparedStatementPtr->BindInt (nBindIndex++, spResultSet->GetValueInt(1));
		spPreparedStatementPtr->BindText(nBindIndex++, spResultSet->GetValueString(2));

		spPreparedStatementPtr->ExecuteUpdate();
		spPreparedStatementPtr->Reset();
	}

	strQuery.Format(_T("Delete From %s Where kcode_group_key = %d"), TABLE_KCODE_DEFINE, USER_LINKCODE_GROUP_KEY);
	spBaseConnectionPtr->ExecuteUpdate(strQuery);

	strQuery1.Format(_T("Select kcode_group_key, kcode_key, kcode_display From %s Where kcode_group_key = %d"), 
		TABLE_KCODE_DEFINE, USER_LINKCODE_GROUP_KEY);

	spResultSet = spDBaseConnection->ExecuteQuery(strQuery1);

	strQuery.Format(_T("Insert Into %s(kcode_group_key, kcode_key, kcode_display) Values(?, ?, ?)"), 
		TABLE_KCODE_DEFINE);

	spPreparedStatementPtr = spBaseConnectionPtr->PrepareStatement(strQuery);

	while (spResultSet->Next())
	{
		int nBindIndex(1);

		AutoType iterFind = setIncludeLinkType.find(spResultSet->GetValueInt(1));
		AutoType iterEnd  = setIncludeLinkType.end();

		if (iterFind == iterEnd)
			continue;

		spPreparedStatementPtr->BindInt (nBindIndex++, spResultSet->GetValueInt(0));
		spPreparedStatementPtr->BindInt (nBindIndex++, spResultSet->GetValueInt(1));
		spPreparedStatementPtr->BindText(nBindIndex++, spResultSet->GetValueString(2));

		spPreparedStatementPtr->ExecuteUpdate();
		spPreparedStatementPtr->Reset();
	}

	KCodeManager* pCodeManager = pTarget->CodeManager();
	pCodeManager->LoadCodeGroups();

	TxLogDebugEndMsg();
}

void KSubNetworkDlg::OnBnClickedCancel()
{
	m_pMapView->StopMapSelectionByPolygon();

	KResizeDialogEx::OnCancel();
}


void KSubNetworkDlg::OnBnClickedCheck1()
{
	m_pMapView->StopMapSelectionByPolygon();
}

int CALLBACK KSubNetworkDlg::BrowseCallBackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lPData )
{
	switch (uMsg)
	{
	case  BFFM_INITIALIZED:
		{
			::SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)lPData);
		}
		break;
	}
	return 0;
}

void KSubNetworkDlg::CreateSubGeometry( ThreadPara* pPara )
{
	CString  strLocation(_T(""));
	KTarget* pSubTarget = nullptr;

	pSubTarget = (KTarget*)pPara->TKeyPtr[0];
	if (nullptr == pSubTarget) {
		ThrowException(_T("Sub Geometry 생성 실패"));
	}
	strLocation = pSubTarget->GetLocation();

	CString strIODataFilePath(strLocation); {
        strIODataFilePath.AppendFormat(_T("\\%s"),  IODATAFILE_NAME);
    }	
        
	KDBaseConPtr spDBase = KDBase::GetSubODDBConnection(strIODataFilePath);
    
	std::vector<Integer> vecSubNodeID;
	LoadSubNodeID(spDBase, vecSubNodeID);
	std::vector<Integer> vecSubLinkID;
	LoadSubLinkID(spDBase, vecSubLinkID);
	std::vector<Integer> vecSubZoneID;
	LoadSubZoneID(spDBase, vecSubZoneID);

	// Origin Map Layer 정보
	ITxMapServerPtr   spOrgMapServer = nullptr;
	TxFeatureLayerPtr spOrgNodeLayer = nullptr;
	TxFeatureLayerPtr spOrgLinkLayer = nullptr;
	TxFeatureLayerPtr spOrgZoneLayer = nullptr;
	LoadLayerInfo(m_pTarget, spOrgMapServer, spOrgNodeLayer, spOrgLinkLayer, spOrgZoneLayer, false); // Layer 정보를 참조해오는 역활이라서 false

	// Dest(Sub) Map Layer 정보
	ITxMapServerPtr   spSubMapServer = nullptr;
	TxFeatureLayerPtr spSubNodeLayer = nullptr;
	TxFeatureLayerPtr spSubLinkLayer = nullptr;
	TxFeatureLayerPtr spSubZoneLayer = nullptr;
	LoadLayerInfo(pSubTarget, spSubMapServer, spSubNodeLayer, spSubLinkLayer, spSubZoneLayer);

	spSubMapServer->BeginTransaction();

	try 
	{
		// Origin Map Layer에서 Geomety 조회
		if (nullptr != spOrgNodeLayer || nullptr != spSubNodeLayer) {
			for (size_t i= 0; i< vecSubNodeID.size(); i++){
				const Integer &nxSubNodeID  = vecSubNodeID[i];
				ITxGeometryPtr spOrgNodeGeo = spOrgNodeLayer->GetGeometry(nxSubNodeID);

				if (nullptr != spOrgNodeGeo) {
					spSubNodeLayer->ProhibitGeoInsert(nxSubNodeID, spOrgNodeGeo->CopyGeometry());
				}
			}
		}

		if (nullptr != spOrgLinkLayer || nullptr != spSubLinkLayer) {
			for (size_t i= 0; i< vecSubLinkID.size(); i++){
				const Integer &nxSubLinkID  = vecSubLinkID[i];
				ITxGeometryPtr spOrgLinkGeo = spOrgLinkLayer->GetGeometry(nxSubLinkID);

				if (nullptr != spOrgLinkGeo) {
					spSubLinkLayer->ProhibitGeoInsert(nxSubLinkID, spOrgLinkGeo->CopyGeometry());
				}
			}
		}

		if (nullptr != spOrgZoneLayer || nullptr != spSubZoneLayer) {
			for (size_t i= 0; i< vecSubZoneID.size(); i++){
				const Integer &nxSubZoneID  = vecSubZoneID[i];
				ITxGeometryPtr spOrgZoneGeo = spOrgZoneLayer->GetGeometry(nxSubZoneID);

				if (nullptr != spOrgZoneGeo) {
					spSubZoneLayer->ProhibitGeoInsert(nxSubZoneID, spOrgZoneGeo->CopyGeometry());
				} else {
					// 노드레이어에서 조회하여, 좌표를 얻는다.
					ITxGeometryPtr spOrgNodeGeo = spOrgNodeLayer->GetGeometry(nxSubZoneID);

					if (nullptr == spOrgNodeGeo) {
						continue;
					}

					TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spOrgNodeGeo->CopyGeometry());
					double dX = spPoint->X;
					double dY = spPoint->Y;

					// square-polygon을 위한 vertex
					std::vector<TxPoint> vecPoint; { //x, y 좌표를 기점으로 +-50
						vecPoint.push_back(TxPoint(dX+50, dY+50)); // 1사분면
						vecPoint.push_back(TxPoint(dX-50, dY+50)); // 2사분면
						vecPoint.push_back(TxPoint(dX-50, dY-50)); // 3사분면
						vecPoint.push_back(TxPoint(dX+50, dY-50)); // 4사분면
						vecPoint.push_back(TxPoint(dX+50, dY+50));
					}

					ITxGeometryPtr spGeometry = TxPolygonPtr(new TxPolygon(vecPoint));
					spSubZoneLayer->ProhibitGeoInsert(nxSubZoneID, spGeometry);
				}
			}
		}

		spSubMapServer->Commit();

		try
		{
			if (nullptr != spSubLinkLayer){
				// RebuildIndex 수행시 트랙젠션 관계로, 외부의 트랙젝션이 끝난후 수행되야 한다.
				spSubLinkLayer->RebuildIndex();
			}

		} catch (...) {
			TxLogDebugException();
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		spSubMapServer->Rollback();
		throw ex;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		spSubMapServer->Rollback();
		throw ex;
	} catch (...) {
		TxLogDebugException();
		spSubMapServer->Rollback();
		ThrowException(_T("Sub Geometry 생성 실패"));
	}
}

void KSubNetworkDlg::LoadSubNodeID(KDBaseConPtr spDBase, std::vector<Integer> &a_vecNodeID)
{
	try 
	{
		CString strSQL(_T("Select node_id From node"));
		KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

		Integer nxNodeID(0);
		while (spResult->Next())
		{
			nxNodeID  = spResult->GetValueInt64(0);
			a_vecNodeID.push_back(nxNodeID);
		}
	} catch (...){
		AfxMessageBox(_T("Data Search Failed"));
		TxLogDebugException();
	}
}

void KSubNetworkDlg::LoadSubLinkID(KDBaseConPtr spDBase, std::vector<Integer> &a_vecLinkID)
{
	try 
	{
		CString strSQL(_T("Select link_id From link"));
		KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

		Integer nxLinkID(0);
		while (spResult->Next())
		{
			nxLinkID  = spResult->GetValueInt64(0);
			a_vecLinkID.push_back(nxLinkID);
		}
	} catch (...){
		AfxMessageBox(_T("Data Search Failed"));
		TxLogDebugException();
	}
}

void KSubNetworkDlg::LoadSubZoneID(KDBaseConPtr spDBase, std::vector<Integer> &a_vecZoneID)
{
	try 
	{
		CString strSQL(_T("Select zone_id From zone"));
		KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

		Integer nxZoneID(0);
		while (spResult->Next())
		{
			nxZoneID  = spResult->GetValueInt64(0);
			a_vecZoneID.push_back(nxZoneID);
		}
	} catch (...){
		AfxMessageBox(_T("Data Search Failed"));
		TxLogDebugException();
	}
}

void KSubNetworkDlg::LoadLayerInfo(KTarget* a_pTarget, ITxMapServerPtr &spMapServer, TxFeatureLayerPtr &spNodeLayer, TxFeatureLayerPtr &spLinkLayer, TxFeatureLayerPtr &spZoneLayer, bool bInitExtent)
{
	try 
	{
		KMapView* pCreatedMapView = ImChampFrameWindow::GetCreatedMapView(a_pTarget);
		if (pCreatedMapView != nullptr) { // MapView가 생성되어 있는지 확인
			ITxFeatureLayerPtr spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Node());
			if (spLayer != nullptr) {
				spNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Link());
			if (spLayer != nullptr) {
				spLinkLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spLayer = pCreatedMapView->MapGetFeatureLayer(KLayerID::Zone());
			if (spLayer != nullptr) {
				spZoneLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
			}

			spMapServer = spNodeLayer->GetServer();
		} else { // MapView가 생성 되어 있지 않으면 서버에 직접 접속
			CString strServerPath = a_pTarget->GetIODataFilePath();
			spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, strServerPath));
			if ( spMapServer->Connect() != 1)  
				throw 1;

			// 서버의 레이어 정보 조회
			std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
			for (size_t j=0; j<vecLayerInfo.size(); j++) {
				TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

				if (_tcsicmp(TABLE_NODE, spLayerInfo->Name()) == 0) { // 노드 레이어 조회
					spNodeLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
				} else if (_tcsicmp(TABLE_LINK, spLayerInfo->Name()) == 0) { // 링크 레이어 조회
					spLinkLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
				} else if (_tcsicmp(TABLE_ZONE, spLayerInfo->Name()) == 0) { // 존 레이어 조회
					spZoneLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
				}
			}
		}

		if (bInitExtent) {
			if (nullptr != spNodeLayer) {
				TxLayerInfoPtr    spLayerInfo    = spNodeLayer->LayerInfo();
				TxLayerInfoSQLPtr spLayerInfoSQL = std::dynamic_pointer_cast<TxLayerInfoSQL>(spLayerInfo);
				spLayerInfoSQL->SetExtent(0.0,0.0,0.0,0.0); // 초기화
			}
			if (nullptr != spLinkLayer) {
				TxLayerInfoPtr    spLayerInfo    = spLinkLayer->LayerInfo();
				TxLayerInfoSQLPtr spLayerInfoSQL = std::dynamic_pointer_cast<TxLayerInfoSQL>(spLayerInfo);
				spLayerInfoSQL->SetExtent(0.0,0.0,0.0,0.0); // 초기화
			}
			if (nullptr != spZoneLayer) {
				TxLayerInfoPtr    spLayerInfo    = spZoneLayer->LayerInfo();
				TxLayerInfoSQLPtr spLayerInfoSQL = std::dynamic_pointer_cast<TxLayerInfoSQL>(spLayerInfo);
				spLayerInfoSQL->SetExtent(0.0,0.0,0.0,0.0); // 초기화
			}
		}
	} catch (int& ex) {
		TxLogDebugException();
		throw ex;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		throw ex;
	} catch (...) {
		TxLogDebugException();
		ThrowException(_T("Layer 조회 실패")); 
	}
}