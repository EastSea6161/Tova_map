// kmz.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KmzApp.h"

#include "KStartSplashDlg.h"
#include "KmzSystem.h"

#include "MainFrameWnd.h"
#include "ChildFrm.h"

#include "kmz_common.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "ProjectExplorerWnd.h"
#include "ProjectUserNewDlg.h"

#include "RenameDlg.h"
#include "TargetCopyDlg.h"
#include "ImportTableDlg.h"

#include "PreferenceForm.h"
#include "PurposeManager.h"
#include "ModeManager.h"
#include "DBaseProject.h"
#include "TProjectInfo.h"
#include "DeleteConfirmDlg.h"
#include "ImTasLicenseCheck.h"
#include "CopyProject.h"
#include "SplashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*
// Visual Leak Debug 
// - ArcGIS Engine 체크하면서 시스템 먹통 현상 발생하여 사용하지 않음
// - 좋은 프로그램
#ifdef _DEBUG
#include <vld.h>
#endif
*/

//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
/******************************************************************************
* CAboutDlg                                                                   *
******************************************************************************/
class CAboutDlg : public KDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_9999_AboutKMZDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
};


CAboutDlg::CAboutDlg() : KDialogEx(CAboutDlg::IDD)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if (theApp.IsFreeVersion())
	{
		GetDlgItem(IDC_STATIC_APP)->SetWindowText(_T("TOVA Free Version"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAboutDlg, KDialogEx)
END_MESSAGE_MAP()


/* The one and only CkmzApp object */
CKmzApp theApp;


const int CKmzApp::MAX_NODE_OF_FREEVERSION = 3000;


CKmzApp::CKmzApp()
	:	m_hMDIMenu(NULL),
		m_hMDIAccel(NULL)
{
	m_pProject      = NULL;
	m_hLauncherProc = nullptr;
    KmzSystem::SetSystemType(KEMPASSENGER_SYSTEM);  // 여객 모형
	m_bFreeVersion = true;
}

CKmzApp::~CKmzApp()
{
	KillKlauncher();

    TxLogDebugStartMsg();
	if (m_pProject != nullptr)
	{
		QBicDelete(m_pProject);
	}
    TxLogDebugEndMsg();
}


/**************************************
* Overrides                           *
**************************************/
BOOL CKmzApp::InitInstance()
{
    CSplashDlg oDlg;
    oDlg.ShowSplashScreen(NULL);
    
    // 디버그 모드에서 메모리 누수 체크
    /*#ifdef _DEBUG
    _CrtSetDbgFlag((_CRTDBG_ALLOC_MEM_DF || _CRTDBG_LEAK_CHECK_DF));
    //_CrtSetBreakAlloc(64091);
    //_CrtMemDumpAllObjectsSince(0);
    #endif*/
    
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	SetRegistryKey(_T("ImTova"));
	int nProjetType = GetProfileInt(_T("Startup"), _T("ProjetType"), 0);
    
	CTime t = CTime::GetCurrentTime();
	CString strDate = t.Format(_T("%A, %B %d, %Y"));
	
        /*
    KStartSplashDlg oStartSplashDlg;
	oStartSplashDlg.SetSystemType(nProjetType);
    if ( oStartSplashDlg.DoModal() == IDOK) {
		nProjetType = oStartSplashDlg.GetSystemType();
        KmzSystem::SetSystemType((KEMPSYSTEMTYPE)oStartSplashDlg.GetSystemType());
		WriteProfileInt(_T("Startup"), _T("ProjetType"), nProjetType);
		WriteProfileString(_T("Startup"), _T("LastDate"), strDate);
    } else {
        return FALSE;
    }*/
    
    KmzSystem::SetSystemType(KEMPASSENGER_SYSTEM);
    WriteProfileInt(_T("Startup"), _T("ProjetType"), KEMPASSENGER_SYSTEM);
    WriteProfileString(_T("Startup"), _T("LastDate"), strDate);

	// Initialize OLE libraries
	if (!AfxOleInit()) {
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
               
    if (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Ok) {
        AfxMessageBox(_T("Error:Failed to initialize GDI+"));
        return FALSE;
    }
    
	// 2019.06.14 : splash를 0.5초동안 표출
	Sleep(500);

	// 2019.05.24 : Free 버전 관련 내용 수정
	// 라이센스 인증을 실패하면 Free버전으로 사용
	// 프리버전은 데이터의 크기 등의 제약이 있다.
    ImTasLicenseChecker oImTasLicenseChecker;
    if ( oImTasLicenseChecker.IsOK() == false )
    {
		CString strLicenseMsg = _T("TOVA runs in free mode.");
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			strLicenseMsg = _T("TOVA 프로그램이 Free mode로 실행됩니다.");
		}
		AfxMessageBox(strLicenseMsg);
        // AfxMessageBox(_T("TOVA 라이센스 인증에 실패하였습니다."));
        // return FALSE;
    }
	else
	{
		m_bFreeVersion = false;
	}
	// 라이선스 체크를 안할 경우에는 FreeVersion을 fasle로 설정
	//m_bFreeVersion = false;

	oDlg.HideSplashScreen();
	CMDIFrameWnd* pFrame = new CMainFrameWnd();
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//  additional menu types your application may need
	HINSTANCE hInst = AfxGetResourceHandle();
    
    m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_PASSENGER_TYPE));	
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_kmzTYPE));
    
	// The main window has been initialized, so show and update it
	// pFrame->ShowWindow(m_nCmdShow);
    pFrame->ShowWindow( SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();
    
    ImChampMapDefineLabel::LoadPreferenceLabel();
	ImChampMapDefineSymbol::LoadPreferenceSymbol();

    setlocale(LC_ALL, "");
	return TRUE;
}

// CkmzApp message handlers

int CKmzApp::ExitInstance()
{
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);
	
	GdiplusShutdown(gdiplusToken);

    #ifdef _DEBUG
    _CrtDumpMemoryLeaks();
    #endif // _DEBUG
    
	return CWinApp::ExitInstance();
}

/**************************************
* Operations                          *
**************************************/

bool CKmzApp::CreateUserPassengerProject( void )
{	
	KProjectUserNewDlg dlg;
	if(dlg.DoModal() != IDOK)
	{
		//AfxMessageBox(_T("Abort to create new project."));
		return false;
	}

	// 기존 프로젝트와 관련되 Frame 해제
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	if (pFrame != NULL)
		pFrame->CloseAllChildFrame();

	// 새로운 프로젝트 생성 및 설정
	KProject* pProject = new KProject;

	pProject->SetProjectType(KEMUserDefineProject);
	pProject->SetName(dlg.m_strName);
	pProject->SetInvestigator(dlg.m_strInvestigator);
	pProject->SetOrganization(dlg.m_strOrganization);
	pProject->SetDescription(dlg.m_strDescription);

	CString strLocation = dlg.m_strLocation + _T("\\");
	strLocation += dlg.m_strName;
	pProject->SetLocation( strLocation );
	//-UserDefine 에서는 사용 되지 않는 정보들 - 예외 값으로 셋팅
	pProject->SetSubJectAreaInfo(-1, _T(""), _T(""));
	pProject->SetAnalysisAreaId(-1);
	pProject->SetAnalysisAreaName(_T(""));
	pProject->SetAnalysisFolderName(_T(""));
	//-UserDefine 에서는 사용 되지 않는 정보들 end
	if(dlg.m_bBaseYear == TRUE)
	{
		pProject->SetBaseYear(dlg.m_nBaseYear);	
	}

	std::vector<int>	 vecTargetYears;
	std::vector<CString> vecPurposeNames;
	std::vector<int>	 vecPurposeIds;
	
	std::vector<CString> vecModeNames;
	std::vector<int>	 vecModeIds;

	int nTargetCount =  ::ParseTargetYear (dlg.m_strTargetYear,  vecTargetYears);
	int nPurposeCount = ::ParsePurposeName(dlg.m_strPurposeName, vecPurposeNames);
	int nPIdCount =     ::ParsePurposeId  (dlg.m_strPurposeId,   vecPurposeIds);
	int nModeCount =    ::ParseModeName   (dlg.m_strModeName,    vecModeNames);
	int nMIdCount =		::ParseModeId	  (dlg.m_strModeId,		 vecModeIds);

	pProject->SetTargetYear  (vecTargetYears);
	pProject->SetPurposeName (vecPurposeNames);
	pProject->SetPurposeId   (vecPurposeIds);
	pProject->SetPurposeGroup( _ttoi(dlg.m_strPurposeGroup) );
	pProject->SetPurpose     (dlg.m_vecPurpose);
	
	pProject->SetModeName	 (vecModeNames);
	pProject->SetModeId      (vecModeIds);
	pProject->SetModeGroup   ( _ttoi(dlg.m_strModeGroup) );
	pProject->SetMode		 (dlg.m_vecMode);

	pProject->CreateInitialObject();

	KScenario* pBaseScenario = pProject->CreateScenario( _T("BASE") );
	if( dlg.m_bBaseYear )
	{
		CString strTargetName;
		strTargetName.Format( _T("%d"), dlg.m_nBaseYear );

		pBaseScenario->SetMode	 (dlg.m_vecMode);

		pBaseScenario->CreateTarget( dlg.m_nBaseYear, strTargetName );
	}

	QBicDelete(m_pProject);

	m_pProject = pProject;
	SaveProject();

	return true;
}

bool CKmzApp::CreateUserFreightProject( void )
{	
	return false;
}

bool CKmzApp::SaveProject( void )
{
	//여기 변경될 시 KCopyProject::CreateProjectFile 동일하게 변경
	try
	{
		CString strFile = m_pProject->GetLocation();
		strFile += _T("\\");
		strFile += m_pProject->GetName();

		if (KmzSystem::IsPassengerSystem())
		{
			strFile += _T(".tpx");
		}
		else
		{
			strFile += _T(".kpf");
		}

		remove((CStringA)strFile);

		::WritePrivateProfileString( _T("APP"), _T("DESC"), _T("TOVA Project File"), strFile );

		m_pProject->SaveProject( strFile );
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}

void CKmzApp::DeleteOldProjectFile(CString a_strOldProjectName)
{
	try
	{
		CString strFile = m_pProject->GetLocation();
		strFile += _T("\\");
		strFile += a_strOldProjectName;

		if (KmzSystem::IsPassengerSystem())
		{
			strFile += _T(".tpx");
		}
		else
		{
			strFile += _T(".kpf");
		}

		remove((CStringA)strFile);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void CKmzApp::CloseProject( void )
{
	if( NULL == m_pProject )
        return;

    SaveProject();
	QBicDelete(m_pProject);
}


void CKmzApp::PerformEditProject( void )
{
	std::vector<KTarget*>            vecTarget;
	std::vector<KDBaseConPtr> vecDBase;
	
	TProjectInfo oProjectInfo = {};
	
    if( m_pProject->modifyProject(oProjectInfo) == true )
	{
		try
		{
			//Set KDBaseConPtr Vector
			if (oProjectInfo.bEditedPurpose == true || oProjectInfo.bEditedMode == true)
			{
				//Base Scenario
				KScenario* pScenario = m_pProject->GetBaseScenario();

				const KTDTargetMap&          oTargetMap = pScenario->GetTargetMap();
				KTDTargetMap::const_iterator beginTIter = oTargetMap.begin();
				KTDTargetMap::const_iterator endTIter   = oTargetMap.end();

				while(beginTIter != endTIter)
				{
					KTarget*            pTarget           = beginTIter->second;
					KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

					if (spDBaseConnection == nullptr)
					{
						++beginTIter;
						continue;
					}

					spDBaseConnection->BeginTransaction();
					vecDBase.push_back(spDBaseConnection);
					vecTarget.push_back(pTarget);

					++beginTIter;
				}

				//Other Scenario
				const KTDScenarioMap&          oScenarioMap = m_pProject->GetScenarioMap();
				KTDScenarioMap::const_iterator beginSIter   = oScenarioMap.begin();
				KTDScenarioMap::const_iterator endSIter     = oScenarioMap.end();

				while (beginSIter != endSIter)
				{
					KScenario* pScenario = beginSIter->second;
					
					//Update target purpose...
					const KTDTargetMap&          oTargetMap = pScenario->GetTargetMap();
					KTDTargetMap::const_iterator beginTIter = oTargetMap.begin();
					KTDTargetMap::const_iterator endTIter   = oTargetMap.end();

					while(beginTIter != endTIter)
					{
						KTarget*            pTarget           = beginTIter->second;
						KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

						if (spDBaseConnection == nullptr)
						{
							++beginTIter;
							continue;
						}

						spDBaseConnection->BeginTransaction();
						vecDBase.push_back(spDBaseConnection);
						vecTarget.push_back(pTarget);

						++beginTIter;
					}
					++beginSIter;
				}
			}

			if (vecDBase.size() != vecTarget.size())
			{
				//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
				}
				else {
					AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
				}
				return;
			}

			if (oProjectInfo.bEditedPurpose == true)
			{
				int nSize = vecDBase.size();

				for (int i = 0; i < nSize; ++i)
				{
					KDBaseProject::ModifyPurposeInfo(vecDBase[i], oProjectInfo.vecPurpose);
					KPurposeManager* pPurposeMgr = vecTarget[i]->PurposeManager();
					pPurposeMgr->LoadPurpose(vecDBase[i]);
				}
			}

			if (oProjectInfo.bEditedMode == true)
			{
				int nSize = vecDBase.size();

				for (int i = 0; i < nSize; ++i)
				{
					KDBaseProject::ModifyModeInfo(vecDBase[i], oProjectInfo.vecMode);
					KModeManager* pModeMgr = vecTarget[i]->ModeManager();
					pModeMgr->LoadMode(vecDBase[i]);
				}
			}
			
			const KTDTargetMap& targetMap = m_pProject->GetBaseScenario()->GetTargetMap();

			if (targetMap.empty() == true)
			{
				CString strTargetName;
				strTargetName.Format( _T("%d"), oProjectInfo.nBaseYear );
				m_pProject->GetBaseScenario()->CreateTarget( oProjectInfo.nBaseYear, strTargetName );
			}

			std::vector<int> vecTargetYear;
			::ParseTargetYear(oProjectInfo.strTargetYear, vecTargetYear);

			if( vecTargetYear.empty() == true )
			{
				const KTDScenarioMap&          oScenarioMap = m_pProject->GetScenarioMap();
				KTDScenarioMap::const_iterator beginSIter   = oScenarioMap.begin();
				KTDScenarioMap::const_iterator endSIter     = oScenarioMap.end();

				while (beginSIter != endSIter)
				{
					KScenario* pScenario = beginSIter->second;
					std::vector< int >::iterator itTargetYear = vecTargetYear.begin();
					
					while( vecTargetYear.end() != itTargetYear )
					{
						CString strTargetName;
						strTargetName.Format( _T("%d"), *itTargetYear );
						pScenario->CreateTarget( *itTargetYear, strTargetName );
						++itTargetYear;
					}
					++beginSIter;
				}
			}
			
			//Commit
			for ( size_t i = 0; i < vecDBase.size(); ++i)
			{
				vecDBase[i]->Commit();
			}
		}
		catch (KExceptionPtr ex)
		{
			//rollback
			for ( size_t i = 0; i < vecDBase.size(); ++i)
			{
				vecDBase[i]->RollBack();
			}

			AfxMessageBox(ex->GetErrorMessage());
			return;
		}
		catch (...)
		{
			//rollback
			for ( size_t i = 0; i < vecDBase.size(); ++i)
			{
				vecDBase[i]->RollBack();
			}
			
			//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
			}
			else {
				AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
			}
			return;
		}

		if (oProjectInfo.bEditedPurpose == true)
		{
			std::vector<CString> vecPurposeName;
			std::vector<int>     vecPurposeID;

			::ParsePurposeName(oProjectInfo.strPurposeName, vecPurposeName);
			::ParsePurposeId  (oProjectInfo.strPurposeID, vecPurposeID);

			m_pProject->SetPurposeGroup(_ttoi(oProjectInfo.strPurposeGroup));
			m_pProject->SetPurposeName (vecPurposeName);
			m_pProject->SetPurposeId   (vecPurposeID);
			m_pProject->SetPurpose     (oProjectInfo.vecPurpose);
		}

		if (oProjectInfo.bEditedMode == true)
		{
			std::vector<CString> vecModeName;
			std::vector<int>     vecModeID;

			::ParseModeName(oProjectInfo.strModeName, vecModeName);
			::ParseModeId  (oProjectInfo.strModeID,   vecModeID);

			m_pProject->SetModeGroup(_ttoi(oProjectInfo.strModeGroup));
			m_pProject->SetModeName (vecModeName);
			m_pProject->SetModeId   (vecModeID);
			m_pProject->SetMode     (oProjectInfo.vecMode);
		}

		CString strOldProjectName = m_pProject->GetName();
		
		m_pProject->SetName(oProjectInfo.strProjectName);

		//Add Edit Investigator, organization, Description, TargetYear;
		m_pProject->SetInvestigator(oProjectInfo.strInvestigator);
		m_pProject->SetOrganization(oProjectInfo.strOrganization);
		m_pProject->SetDescription(oProjectInfo.strDescription);

		std::vector<int> vecTargetYear;
		::ParseTargetYear(oProjectInfo.strTargetYear, vecTargetYear);

		m_pProject->SetTargetYear(vecTargetYear);

		if (SaveProject())
		{
			CString strNewProjectName = m_pProject->GetName();
			if (strOldProjectName.CompareNoCase(strNewProjectName) != 0)
			{
				DeleteOldProjectFile(strOldProjectName);
			}
		}

		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

		if( NULL != pwndProjectView )
			pwndProjectView->RedrawTreeItemData();
	}
}


void CKmzApp::performAddScenario( void )
{
	int nCount = m_pProject->addScenario();

	if( nCount > 0 )
	{
		CMainFrameWnd* pFrame = STATIC_DOWNCAST(CMainFrameWnd, m_pMainWnd);

		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
		if( NULL != pwndProjectView )
		{
			pwndProjectView->RedrawTreeItemData();
		}

        SaveProject();
	}
}


void CKmzApp::PerformImportBLANK( void )
{
    // 기존 활성화된 MainFrame 감추기    
    CMainFrameWnd* pFrame = STATIC_DOWNCAST(CMainFrameWnd, m_pMainWnd);
    if (pFrame != NULL)
        pFrame->CloseAllChildFrame();

	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
	if( NULL != pwndProjectView )
	{
		const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) && (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectScenario) && 
			(NULL != pItemData->Object) )
		{
			KScenario* pScenario = (KScenario*)pItemData->Object;
			if( pScenario->ImportBLANK() == true )
			{
				pwndProjectView->RedrawTreeItemData();
                SaveProject();
			}
		}
	}
}


void CKmzApp::PerformScenarioCopyTo( void )
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
    if (pwndProjectView == nullptr)
        return;

    const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
    if (pItemData == nullptr) {
        AfxMessageBox(_T("시나리오를 선택해 주세요"));
        return;
    }

    if (pItemData->Object == nullptr) {
        AfxMessageBox(_T("시나리오를 선택해 주세요"));
        return;
    }

    if( (pItemData->TreeItemType != KEMTreeItemTypeKDTBProjectScenario) && (pItemData->TreeItemType != KEMTreeItemTypeUserDefineProjectScenario) ) {
        AfxMessageBox(_T("시나리오를 선택해 주세요"));
        return;
    }

    KScenario* pScenario = (KScenario*)pItemData->Object;
    if( m_pProject->CopyScenario( pScenario ) == true )
    {
        pwndProjectView->RedrawTreeItemData();
        SaveProject();
    }
}


void CKmzApp::performScenarioDelete( void )
{
	try
	{
		KDeleteConfirmDlg oDlg;

		if (oDlg.DoModal() != IDOK)
			return;
		
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

		if( NULL != pwndProjectView )
		{
			const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
			if( (NULL != pItemData) && 
				(NULL != pItemData->Object) )
			{
				if( (pItemData->TreeItemType != KEMTreeItemTypeKDTBProjectScenario) && (pItemData->TreeItemType != KEMTreeItemTypeUserDefineProjectScenario) )
				{
					return;
				}

				KScenario* pScenario = (KScenario*)pItemData->Object;
				if( NULL != pScenario )
				{
					//Base는 Delete 불가
					if(pScenario->IsBaseScenario() == true)
						return;

					const KTDTargetMap& targetMap = pScenario->GetTargetMap();
					KTDTargetMap::const_iterator citTargetMap = targetMap.begin();
					while( targetMap.end() != citTargetMap )
					{
						KTarget* pTarget = citTargetMap->second;
						if( NULL != pTarget )
						{
							CChildFrame* pChildFrame = pFrame->GetChildFrame( pTarget->GetObjectID() );
							if( NULL != pChildFrame )
							{
								pChildFrame->SendMessage( WM_CLOSE );
							}
						}
						++citTargetMap;
					}

					CString strlocation = pScenario->GetLocation();

					m_pProject->removeScenario( pScenario->GetObjectID() );
					pwndProjectView->RedrawTreeItemData();
					SaveProject();

					if (oDlg.IsDeleteFile())
						DeletePath(strlocation);
				}
			}
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("삭제되었습니다."));
		}
		else {
			AfxMessageBox(_T("Deleted successfully."));
		}
	}
	catch (...)
	{
		AfxMessageBox(_T("삭제중 오류가 발생하였습니다."));
		TxLogDebugException();
	}
}


void CKmzApp::performScenarioAddYear( void )
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

	if( NULL != pwndProjectView )
	{
		const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();

		if( (NULL != pItemData) &&
			(NULL != pItemData->Object) )
		{
			if( (pItemData->TreeItemType != KEMTreeItemTypeKDTBProjectScenario) && (pItemData->TreeItemType != KEMTreeItemTypeUserDefineProjectScenario) )
			{
				return;
			}
			KScenario* pScenario = (KScenario*)pItemData->Object;

			int nBaseTargetCount = 0;
			if(pScenario->IsBaseScenario() == true)		// Base 는 오직 하나의 Year만을 갖는다.
			{
				const KTDTargetMap& targetMap = pScenario->GetTargetMap();
				KTDTargetMap::const_iterator itTargetMap = targetMap.begin();

				while( targetMap.end() != itTargetMap)
				{
					++nBaseTargetCount;
					++itTargetMap;
				}
			}
			
			if( nBaseTargetCount < 1 && pScenario->addNewTarget() == true )
			{
				pScenario->SetMode(m_pProject->GetMode());

				pwndProjectView->RedrawTreeItemData();
                SaveProject();
			}
		}
	}
}


void CKmzApp::performScenarioRename( void )
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

	if( NULL != pwndProjectView )
	{
		const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) && 
			(NULL != pItemData->Object) )
		{
			if( (pItemData->TreeItemType != KEMTreeItemTypeKDTBProjectScenario) && (pItemData->TreeItemType != KEMTreeItemTypeUserDefineProjectScenario) )
			{
				return;
			}

			KScenario* pScenario = (KScenario*)pItemData->Object;
			CString strScenarioName = pScenario->GetName();

			//Base는 Rename 불가 
			if(pScenario->IsBaseScenario() == true)
			{
				return;
			}

			KRenameDlg dlg(NULL, KEMRenameScenario);
			const KTDScenarioMap& scenarioMap = m_pProject->GetScenarioMap();
			KTDScenarioMap::const_iterator citScenarioMap = scenarioMap.begin();
			while( scenarioMap.end() != citScenarioMap )
			{
				dlg.m_NameArray.push_back( citScenarioMap->second->GetName() );
				++citScenarioMap;
			}
			dlg.m_strCaption =  _T("Rename Scenario");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				dlg.m_strCaption = _T("시나리오 이름 바꾸기");
			}
			dlg.m_strName = strScenarioName;
			dlg.m_strOriginScenarioName = strScenarioName;

			if( (dlg.DoModal() == IDOK) && (strScenarioName.CompareNoCase(dlg.m_strName) != 0) )
			{
				pScenario->SetName( dlg.m_strName );
				const KTDTargetMap& targetMap = pScenario->GetTargetMap();
				KTDTargetMap::const_iterator citTargetMap = targetMap.begin();
				while( targetMap.end() != citTargetMap )
				{
					KTarget* pTarget = citTargetMap->second;
					CChildFrame* pChildFrame = pFrame->GetChildFrame( pTarget->GetObjectID() );
					if( NULL != pChildFrame )
					{
						//pChildFrame->SetScenarioName( dlg.m_strName );

						pChildFrame->UpdateTargetFrameTitle();
					}
					++citTargetMap;
				}
				pwndProjectView->RedrawTreeItemData();
                SaveProject();
			}
		}
	}
}


void CKmzApp::performTargetCopyTo( void )
{
	try
	{
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

		if( NULL != pwndProjectView )
		{
			const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
			if( (NULL != pItemData) && 
				(NULL != pItemData->Object) )
			{
				if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget))
				{
					KTarget* pTarget = (KTarget*)pItemData->Object;
					KTargetCopyDlg dlg;
					dlg.SetSourceTarget( pTarget );

					if( dlg.DoModal() == IDOK )
					{
						pwndProjectView->RedrawTreeItemData();
						SaveProject();

						CString strMsg = _T("복사를 완료하였습니다.");
						AfxMessageBox( strMsg );
					}
					else
					{
						AfxMessageBox(_T("복사가 취소되었습니다."));
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


void CKmzApp::performTargetDelete( void )
{
	try
	{
		KDeleteConfirmDlg oDlg;

		if (oDlg.DoModal() != IDOK)
			return;

		CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

		if( NULL != pwndProjectView )
		{
			const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
			if( (NULL != pItemData) && (NULL != pItemData->Object) )
			{
				if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget))
				{
					KTarget* pTarget = (KTarget*)pItemData->Object;
					CChildFrame* pChildFrame = pFrame->GetChildFrame( pTarget->GetObjectID() );
					if( NULL != pChildFrame )
					{
						pChildFrame->SendMessage( WM_CLOSE );
					}

					KScenario* pScenario = (KScenario*)pTarget->GetParentObject();
					if( NULL != pScenario )
					{
						CString strlocation = pTarget->GetLocation();

						pScenario->removeTarget( pTarget );
						pwndProjectView->RedrawTreeItemData();
						SaveProject();

						if (oDlg.IsDeleteFile())
							DeletePath(strlocation);
					}
				}
			}
		}
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("삭제되었습니다."));
		}
		else {
			AfxMessageBox(_T("Deleted successfully."));
		}
	}
	catch (...)
	{
		AfxMessageBox(_T("삭제중 오류가 발생하였습니다."));
		TxLogDebugException();
	}
}


void CKmzApp::performtargetRename( void )
{
	try
	{
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

		if( NULL != pwndProjectView )
		{
			const TTreeItemData* pItemData   = pwndProjectView->GetSelectedTreeItemData();
			if( (NULL != pItemData) && (NULL != pItemData->Object) )
			{
				if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget ) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget ))
				{
					KTarget* pTarget       = (KTarget*)pItemData->Object;
					CString  strTargetName = pTarget->GetName();

					KRenameDlg dlg(NULL, KEMRenameTarget);
					KScenario*                   pScenario    = (KScenario*)pTarget->GetParentObject();
					const KTDTargetMap&          targetMap    = pScenario->GetTargetMap();
					KTDTargetMap::const_iterator citTargetMap = targetMap.begin();
					while( targetMap.end() != citTargetMap )
					{
						dlg.m_NameArray.push_back( citTargetMap->second->GetName() );
						++citTargetMap;
					}

					dlg.m_strCaption = _T("Rename Target Year");
					if (KmzSystem::GetLanguage() == KEMKorea) {
						dlg.m_strCaption = _T("분석 년도 이름 바꾸기");
					}
					dlg.m_strName    = strTargetName;

					if( (dlg.DoModal() == IDOK) && (strTargetName != dlg.m_strName) )
					{
						pTarget->SetName( dlg.m_strName );
						CChildFrame* pChildFrame = pFrame->GetChildFrame( pTarget->GetObjectID() );
						if( NULL != pChildFrame )
						{
							//pChildFrame->SetTargetName( dlg.m_strName );
							pChildFrame->UpdateTargetFrameTitle();
						}
						pwndProjectView->RedrawTreeItemData();
						SaveProject();
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


void CKmzApp::PerformTargetImportBLANK( void )
{
	try
	{
		// 기존 활성화된 MainFrame 감추기    
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST(CMainFrameWnd, m_pMainWnd);
		pFrame->CloseAllChildFrame();

		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
		const TTreeItemData* pItemData       = pwndProjectView->GetSelectedTreeItemData();

		if( (NULL != pItemData) && (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget) && 
			(NULL != pItemData->Object) )
		{
			KTarget*   pTarget   = (KTarget*)pItemData->Object;
			KScenario* pScenario = (KScenario*)pTarget->GetParentObject();

			if (!pTarget->IsEmptyData())
			{
				CString strMsg = _T("해당 선택 대상에 데이터가 존재합니다.\r\n빈 주머니 만들기 실행 후 기존 데이터는 삭제됩니다.\r\n계속하시겠습니까?");

				if (AfxMessageBox(strMsg, MB_YESNO) == IDNO)
					return;
			}

			if( pScenario->ImportSingleTargetBLANK(pTarget) )
			{
				pwndProjectView->RedrawTreeItemData();
				SaveProject();
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


// CkmzApp message handlers

BEGIN_MESSAGE_MAP(CKmzApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CKmzApp::OnAppAbout)
    ON_COMMAND(ID_HELP_ONLINEHELP, &CKmzApp::OnOnlineHelp)
    ON_COMMAND(ID_HELP_CONTENTS, &CKmzApp::OnOnlineHelp)
	ON_COMMAND(ID_USERPROJECT_NEW, &CKmzApp::OnUserProjectNew)
	ON_COMMAND(ID_PROJECT_EDIT, &CKmzApp::OnProjectEdit)
	ON_UPDATE_COMMAND_UI( ID_PROJECT_EDIT, &CKmzApp::OnUpdateProjectEdit )
	ON_COMMAND(ID_PROJECT_OPEN, &CKmzApp::OnProjectOpen)
	ON_COMMAND(ID_PROJECT_CLOSE, &CKmzApp::OnProjectClose)
	ON_UPDATE_COMMAND_UI( ID_PROJECT_CLOSE, &CKmzApp::OnUpdateProjectClose )
	ON_COMMAND(ID_PROJECT_SAVE, &CKmzApp::OnProjectSave)
	ON_UPDATE_COMMAND_UI( ID_PROJECT_SAVE, &CKmzApp::OnUpdateProjectSave )
	ON_COMMAND(ID_PROJECT_COPY, &CKmzApp::OnProjectCopy)
	ON_UPDATE_COMMAND_UI( ID_PROJECT_COPY, &CKmzApp::OnUpdateProjectCopy )
	ON_COMMAND(ID_PROJECT_EXPORT, &CKmzApp::OnProjectExport)
	ON_UPDATE_COMMAND_UI( ID_PROJECT_EXPORT, &CKmzApp::OnUpdateProjectExport )
	ON_COMMAND(ID_PROJECT_IMPORT, &CKmzApp::OnProjectImport)
	ON_COMMAND(ID_SCENARIO_ADD, &CKmzApp::OnScenarioAdd)
	ON_UPDATE_COMMAND_UI( ID_SCENARIO_ADD, &CKmzApp::OnUpdateScenarioAdd )
	ON_COMMAND(ID_SCENARIO_DELETE, &CKmzApp::OnScenarioDelete)
	ON_UPDATE_COMMAND_UI( ID_SCENARIO_DELETE, &CKmzApp::OnUpdateScenarioDelete )
	ON_COMMAND(ID_SCENARIO_COPYTO, &CKmzApp::OnScenarioCopyto)
	ON_UPDATE_COMMAND_UI( ID_SCENARIO_COPYTO, &CKmzApp::OnUpdateScenarioCopyto )
	ON_COMMAND(ID_SCENARIO_RENAME, &CKmzApp::OnScenarioRename)
	ON_UPDATE_COMMAND_UI( ID_SCENARIO_RENAME, &CKmzApp::OnUpdateScenarioRename )
	ON_COMMAND(ID_TARGETYEAR_ADD, &CKmzApp::OnTargetyearAdd)
	ON_UPDATE_COMMAND_UI( ID_TARGETYEAR_ADD, &CKmzApp::OnUpdateTargetyearAdd )
	ON_COMMAND(ID_TARGETYEAR_DELETE, &CKmzApp::OnTargetyearDelete)
	ON_UPDATE_COMMAND_UI( ID_TARGETYEAR_DELETE, &CKmzApp::OnUpdateTargetyearDelete )
	ON_COMMAND(ID_TARGETYEAR_IMPORTUSERDATA, &CKmzApp::OnTargetyearImportuserdata)
	ON_UPDATE_COMMAND_UI( ID_TARGETYEAR_IMPORTUSERDATA, &CKmzApp::OnUpdateTargetyearImportuserdata )
	ON_COMMAND(ID_TARGETYEAR_RENAME, &CKmzApp::OnTargetyearRename)
	ON_UPDATE_COMMAND_UI( ID_TARGETYEAR_RENAME, &CKmzApp::OnUpdateTargetyearRename )
	ON_COMMAND(ID_TARGETYEAR_INFORMATION, &CKmzApp::OnTargetyearInformation)
	ON_UPDATE_COMMAND_UI( ID_TARGETYEAR_INFORMATION, &CKmzApp::OnUpdateTargetyearInformation )
	
	ON_COMMAND(ID_POPUP_PROJECT_ADDSCENARIO, &CKmzApp::OnPopupProjectAddscenario)
	ON_COMMAND(ID_POPUP_PROJECT_EDITPROJECT, &CKmzApp::OnPopupProjectEditproject)
	ON_COMMAND(ID_POPUP_PROJECT_REFRESH,     &CKmzApp::OnPopupProjectRefresh)
	ON_COMMAND(ID_POPUP_SCENARIO_IMPORTBLANK,&CKmzApp::OnPopupScenarioImportBlank)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SCENARIO_IMPORTBLANK,&CKmzApp::OnUpdatePopupScenarioImportBlank)

	ON_COMMAND(ID_POPUP_SCENARIO_COPYTO, &CKmzApp::OnPopupScenarioCopyto)
	ON_COMMAND(ID_POPUP_SCENARIO_DELETE, &CKmzApp::OnPopupScenarioDelete)
	ON_COMMAND(ID_POPUP_SCENARIO_ADDYEAR, &CKmzApp::OnPopupScenarioAddyear)
	ON_COMMAND(ID_POPUP_SCENARIO_RENAME, &CKmzApp::OnPopupScenarioRename)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SCENARIO_COPYTO,  &CKmzApp::OnUpdatePopupScenarioCopyto)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SCENARIO_DELETE,  &CKmzApp::OnUpdatePopupScenarioDelete)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SCENARIO_RENAME,  &CKmzApp::OnUpdatePopupScenarioRename)
	ON_UPDATE_COMMAND_UI(ID_POPUP_SCENARIO_ADDYEAR, &CKmzApp::OnUpdatePopupScenarioAddYear)

	ON_COMMAND(ID_POPUP_TARGET_COPYTO,     &CKmzApp::OnPopupTargetCopyto)
	ON_COMMAND(ID_POPUP_TARGET_RENAMEYEAR, &CKmzApp::OnPopupTargetRename)
	ON_COMMAND(ID_POPUP_TARGET_DELETEYEAR, &CKmzApp::OnPopupTargetDelete)
	
    ON_COMMAND(ID_POPUP_TARGET_IMPORT, &CKmzApp::OnPopupTargetImport)
	ON_UPDATE_COMMAND_UI(ID_POPUP_TARGET_IMPORT, &CKmzApp::OnUpdatePopupTargetImport)

    ON_COMMAND(ID_POPUP_TARGET_EXPORT, &CKmzApp::OnPopupTargetExport)
    ON_UPDATE_COMMAND_UI(ID_POPUP_TARGET_EXPORT, &CKmzApp::OnUpdatePopupTargetExport)

	ON_UPDATE_COMMAND_UI(ID_POPUP_TARGET_COPYTO, &CKmzApp::OnUpdatePopupTargetCopyto)

	ON_COMMAND(ID_POPUP_TARGET_IMPORTBLANK, &CKmzApp::OnPopupTargetImportBlank)

	//Navigation Explorer
	ON_COMMAND(ID_POPUP_NAVI_ADD,    &CKmzApp::OnPopupNaviItemAdd)
	ON_COMMAND(ID_POPUP_NAVI_RENAME, &CKmzApp::OnPopupNaviItemRename)
	ON_COMMAND(ID_POPUP_NAVI_DELETE, &CKmzApp::OnPopupNaviItemDelete)
    ON_COMMAND(ID_NETWORK_NETWORKCOMPARE, &CKmzApp::OnNetworkCompare)
    ON_UPDATE_COMMAND_UI(ID_NETWORK_NETWORKCOMPARE, &CKmzApp::OnUpdateNetworkCompare)
    ON_COMMAND(ID_TOOLS_PREFERENCE, &CKmzApp::OnMenuPreference)
	
    ON_COMMAND(ID_PROJECT_VACCUM, &CKmzApp::OnProjectVaccum)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void CKmzApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CKmzApp::OnOnlineHelp()
{
    ::ShellExecute(NULL, _T("open"), _T("http://k-tova.co.kr/"), NULL, NULL, SW_SHOW );
}


void CKmzApp::OnUserProjectNew()
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );

	bool isNewProject = false;

	if( KmzSystem::IsPassengerSystem() )
	{
		isNewProject = CreateUserPassengerProject();
	}
	else
	{
		isNewProject = CreateUserFreightProject();
	}

	if( !isNewProject )
	{
		return;
	}
	
	CKmzApp*  pApp     = (CKmzApp*)(AfxGetApp());
	KProject* pProject = pApp->GetProject();

	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
	if( NULL != pwndProjectView )
		pwndProjectView->InsertProjectTree();
		
	KNaviExplorerWnd* pWndNaviView       = pFrame->GetNaviTreeView();
	if (NULL != pWndNaviView)
		pWndNaviView->UpdateTree(pProject);

	KScenario*     pBaseScenario         = pProject->GetBaseScenario();
	if( nullptr != pBaseScenario )
	{
		const KTDTargetMap& mapBaseTarget = pBaseScenario->GetTargetMap();

		if( mapBaseTarget.size() > 0) // target year 존재 할 경우 만 'importBLANK' 자동 실행 
		{
			if( pBaseScenario->ImportBLANK() == true )
			{
				pwndProjectView->InsertProjectTree();
				m_pProject->UpgradeProjectDB();
				SaveProject();
			}
		}
	}

	UpdateAppTitle();
}

void CKmzApp::OnProjectOpen()
{
    TxLogDebugStartMsg();

	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );

	if( NULL != m_pProject )
	{
		/*int nSelect = AfxMessageBox( _T("프로젝트를 저장하시겠습니까?"), MB_YESNOCANCEL );
		if( IDCANCEL == nSelect )
		{
			return;
		}
		else if( IDYES == nSelect )
		{
			OnProjectSave();
		}*/
        OnProjectSave();
	}

    CString strFile;
    //TCHAR szFilterPassenger[] = _T("TOVA Project File (*.tpx)|*.tpx|All Files (*.*)|*.*||");
    TCHAR szFilterPassenger[] = _T("TOVA Project File (*.tpx)|*.tpx|");
    TCHAR szFilterFreight[]   = _T("Freight Project Files (*.kpf)|*.kpf|");

	CString strLocation;

    if (KmzSystem::IsPassengerSystem())
    {
	    CFileDialog filedlg( TRUE, _T("tpx"), _T("*.tpx"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilterPassenger );

		strLocation = GetProfileString(_T("Common"), _T("ProjectLocation"));

		if (!strLocation.IsEmpty())
			filedlg.m_ofn.lpstrInitialDir = strLocation;
		
	    if( filedlg.DoModal() != IDOK )
	    {
		    return;
	    }
		strFile = filedlg.GetPathName();
    }
    else
    {
        CFileDialog filedlg( TRUE, _T("kpf"), _T("*.kpf"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilterFreight );

		strLocation = GetProfileString(_T("Common"), _T("ProjectLocation"));

		if (!strLocation.IsEmpty())
			filedlg.m_ofn.lpstrInitialDir = strLocation;

        if( filedlg.DoModal() != IDOK )
        {
            return;
        }
		strFile = filedlg.GetPathName();
    }

	if (pFrame != NULL)
		pFrame->CloseAllChildFrame();

	CWaitCursor wait;
	if( OpenProject( strFile ) == false )
	{
		return;
	}

	CString strRegLocation = strFile.Left(strFile.ReverseFind('\\'));

	WriteProfileString(_T("Common"), _T("ProjectLocation"), strRegLocation);
		
	CKmzApp* pApp = (CKmzApp*)(AfxGetApp());
	KProject* pProject = pApp->GetProject();

	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

	if( NULL != pwndProjectView )
	{
		pwndProjectView->InsertProjectTree( );
	}

	KNaviExplorerWnd* pWndNaviView = pFrame->GetNaviTreeView();
	if (NULL != pWndNaviView)
		pWndNaviView->UpdateTree(pProject);

	UpdateAppTitle();

    TxLogDebugEndMsg();
}

bool CKmzApp::OpenProject( const CString& strFile )
{
    TxLogDebugStartMsg();

    TCHAR   szValue[64] = {0};	
    ::GetPrivateProfileString( _T("APP"), _T("DESC"), _T(""), szValue, sizeof(szValue), strFile);

    CString strValue(szValue);
    if( strValue.CompareNoCase( _T("TOVA Project File") ) != 0 )
    {
        AfxMessageBox( _T("해당 파일은 프로젝트 파일이 아닙니다.") );
        return false;
    }

    int     nIndex  = strFile.ReverseFind( _T('\\') );
    CString strPath = strFile.Left( nIndex );

    KProject* pProject = new KProject();

    pProject->SetLocation    ( strPath );
    pProject->SetParentObject( NULL    );

    if( pProject->OpenProject( strFile ) == false )
    {
        delete pProject;

        TxLogDebugWarning();
        return false;
    }

    QBicDelete(m_pProject);
    m_pProject = pProject;

    // 전체 열기-테스트를 위해 개발(향후 제거)
    /*std::map< unsigned int, KTarget* >::const_iterator iter = m_pProject->GetBaseScenario()->GetTargetMap().cbegin();
    std::map< unsigned int, KTarget* >::const_iterator end  = m_pProject->GetBaseScenario()->GetTargetMap().cend();

    CMainFrameWnd* pMainFrame = (CMainFrameWnd*)AfxGetMainWnd();

    while (iter != end)
    {
        KTarget*       pTarget    = iter->second;
        pMainFrame->ShowNetworkView( pTarget );

        ++iter;
    }

    const KTDScenarioMap& oScenarionMap = m_pProject->GetScenarioMap();
    std::map< unsigned int, KScenario* >::const_iterator sIter = oScenarionMap.cbegin();    
    std::map< unsigned int, KScenario* >::const_iterator sEnd  = oScenarionMap.cend();

    while (sIter != sEnd)
    {
        iter = sIter->second->GetTargetMap().cbegin();
        end  = sIter->second->GetTargetMap().cend();

        while (iter != end)
        {
            KTarget*       pTarget    = iter->second;
            pMainFrame->ShowNetworkView( pTarget );

            ++iter;
        }

        ++sIter;
    }
    */
    TxLogDebugEndMsg();
    return true;
}

void CKmzApp::OnProjectClose()
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	
	if (pFrame != nullptr)
		pFrame->CloseAllChildFrame();

	KProjectExplorerWnd* pWndProject = pFrame->GetProjectView();

	if (pWndProject != nullptr)
		pWndProject->ClearProjectTree();

	KNaviExplorerWnd* pWndNavigation = pFrame->GetNaviTreeView();

	if (pWndNavigation != nullptr)
		pWndNavigation->ClearTree();

    CloseProject();
}


void CKmzApp::OnUpdateProjectClose( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( (NULL != m_pProject ) ? TRUE : FALSE );
}


void CKmzApp::OnProjectSave()
{
	CWaitCursor wait;

	SaveProject();
}


void CKmzApp::OnUpdateProjectSave( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( (NULL != m_pProject ) ? TRUE : FALSE );
}


void CKmzApp::OnProjectExport()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CKmzApp::OnUpdateProjectExport( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( (NULL != m_pProject ) ? TRUE : FALSE );
}


void CKmzApp::OnProjectImport()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CKmzApp::OnProjectEdit()
{
	PerformEditProject();
	UpdateAppTitle();
}


void CKmzApp::OnUpdateProjectEdit( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( (NULL != m_pProject) ? TRUE : FALSE );
}


void CKmzApp::OnScenarioAdd()
{
	performAddScenario();
}


void CKmzApp::OnUpdateScenarioAdd( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( (NULL != m_pProject) ? TRUE : FALSE );
}


void CKmzApp::OnScenarioDelete()
{
	performScenarioDelete();
}

void CKmzApp::OnUpdateScenarioDelete( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pProjectView = pFrame->GetProjectView();
	if( NULL != pProjectView )
	{
		const TTreeItemData* pItemData = pProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) )
		{
			if( (pItemData->TreeItemType == KEMTreeItemTypeKDTBProjectScenario) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectScenario) )
			{
				KScenario* pScenario = (KScenario*)pItemData->Object;
				//Base는 Delete 불가
				if(pScenario->IsBaseScenario() == true)
				{
					bEnable = FALSE;
				}
				else
				{
					bEnable = TRUE;
				}
			}
		}
	}

	pCmdUI->Enable( bEnable );
}


void CKmzApp::OnScenarioCopyto()
{
	PerformScenarioCopyTo();
}


void CKmzApp::OnUpdateScenarioCopyto( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pProjectView = pFrame->GetProjectView();
	if( NULL != pProjectView )
	{
		const TTreeItemData* pItemData = pProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData))
		{
			if( (pItemData->TreeItemType == KEMTreeItemTypeKDTBProjectScenario) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectScenario) )
			{
				bEnable = TRUE;
				KScenario*           pScenario  = (KScenario*)pItemData->Object;

				//From Scenario에 모든 Target에 데이터가 없을시 비활성화
				int                          nTargets(0);
				const KTDTargetMap&          oTargetMap   = pScenario->GetTargetMap();
				KTDTargetMap::const_iterator citTargetMap = oTargetMap.begin();

				while (oTargetMap.end() != citTargetMap)
				{
					KTarget* pTarget = citTargetMap->second;

					if (!pTarget->IsEmptyData())
						++nTargets;

					++citTargetMap;
				}

				if (nTargets == 0)
					bEnable = FALSE;


				const KTDScenarioMap &mapScenario = m_pProject->GetScenarioMap();
				int                  nCopyCnt(0);

				AutoType itScenarioMap = mapScenario.begin();
				while( mapScenario.end() != itScenarioMap )
				{
					if( itScenarioMap->second != pScenario )
					{
						nCopyCnt++;
					}
					++itScenarioMap;
				}

				if (nCopyCnt < 1)
				{
					bEnable = FALSE;
				}
			}
		}
	}

	pCmdUI->Enable( bEnable );
}


void CKmzApp::OnScenarioRename()
{
	performScenarioRename();
}


void CKmzApp::OnUpdateScenarioRename( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pProjectView = pFrame->GetProjectView();
	if( NULL != pProjectView )
	{
		const TTreeItemData* pItemData = pProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) )
		{
			if( (pItemData->TreeItemType == KEMTreeItemTypeKDTBProjectScenario) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectScenario) )
			{
				KScenario* pScenario = (KScenario*)pItemData->Object;
				//Base는 rename 불가
				if(pScenario->IsBaseScenario() == true)
				{
					bEnable = FALSE;
				}
				else
				{
					bEnable = TRUE;
				}
			}
		}
	}

	pCmdUI->Enable( bEnable );
}


void CKmzApp::OnTargetyearAdd()
{
	performScenarioAddYear();
}


void CKmzApp::OnUpdateTargetyearAdd( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pProjectView = pFrame->GetProjectView();
	if( NULL != pProjectView )
	{
		const TTreeItemData* pItemData = pProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) )
		{
			if( (pItemData->TreeItemType == KEMTreeItemTypeKDTBProjectScenario) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectScenario) )
			{
				bEnable = TRUE;

				KScenario* pScenario = (KScenario*)pItemData->Object;

				if(pScenario->IsBaseScenario() == true)		// Base 는 오직 하나의 Year만을 갖는다.
				{
					const KTDTargetMap& targetMap = pScenario->GetTargetMap();

					if (targetMap.size() > 0)
					{
						bEnable = FALSE;
					}
				}
			}
		}
	}

	pCmdUI->Enable( bEnable );
}


void CKmzApp::OnTargetyearDelete()
{
	performTargetDelete();
}


void CKmzApp::OnUpdateTargetyearDelete( CCmdUI* pCmdUI )
{
	try
	{
		BOOL bEnable = FALSE;
		CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pProjectView = pFrame->GetProjectView();
		if( NULL != pProjectView )
		{
			const TTreeItemData* pItemData = pProjectView->GetSelectedTreeItemData();
			if( (NULL != pItemData) )
			{
				if (pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget || pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget)
				{
					bEnable = TRUE;
				}
			}
		}
		pCmdUI->Enable( bEnable );
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void CKmzApp::OnTargetyearImportuserdata()
{
	
}


void CKmzApp::OnUpdateTargetyearImportuserdata( CCmdUI* pCmdUI )
{
	/// $task : Import user data 구현 후 아래 수정
	pCmdUI->Enable( FALSE );
}


void CKmzApp::OnTargetyearRename()
{
	performtargetRename();
}


void CKmzApp::OnUpdateTargetyearRename( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pProjectView = pFrame->GetProjectView();
	if( NULL != pProjectView )
	{
		const TTreeItemData* pItemData = pProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) )
		{
			if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget))
			{
				bEnable = TRUE;
			}
		}
	}
	pCmdUI->Enable( bEnable );
}


void CKmzApp::OnTargetyearInformation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CKmzApp::OnUpdateTargetyearInformation( CCmdUI* pCmdUI )
{
	/// $task : Target year information 구현 후 아래 수정
	pCmdUI->Enable( FALSE );
}


void CKmzApp::OnPopupProjectAddscenario()
{
	ASSERT( NULL != m_pProject );

	performAddScenario();
}


void CKmzApp::OnPopupProjectEditproject()
{
	ASSERT( NULL != m_pProject );

	PerformEditProject();
	UpdateAppTitle();
}


void CKmzApp::OnPopupScenarioImportBlank()
{
	PerformImportBLANK();
	SaveProject();
}


void CKmzApp::OnUpdatePopupScenarioImportBlank( CCmdUI* pCmdUI )
{
	try
	{
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST(CMainFrameWnd, m_pMainWnd);
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
		const TTreeItemData* pItemData       = pwndProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) && (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectScenario) && 
			(NULL != pItemData->Object) )
		{
			KScenario* pScenario = (KScenario*)pItemData->Object;
			const KTDTargetMap &oTargetMap = pScenario->GetTargetMap();
			if (oTargetMap.size() < 1)
			{
				pCmdUI->Enable(FALSE);
				return;
			}
		}
	}
	catch (...)
	{
		pCmdUI->Enable(FALSE);
		TxLogDebugException();	
		return;
	}

	pCmdUI->Enable(TRUE);
}


void CKmzApp::OnPopupProjectRefresh()
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST(CMainFrameWnd, m_pMainWnd);
	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
	if( NULL != pwndProjectView )
	{
		pwndProjectView->InsertProjectTree();
	}
}

void CKmzApp::OnPopupScenarioCopyto()
{
    if (m_pProject == nullptr) {
        AfxMessageBox(_T("프로젝트 정보가 없습니다."));
        return;
    }
    	
	PerformScenarioCopyTo();
}


void CKmzApp::OnPopupScenarioDelete()
{
    if (m_pProject == nullptr) {
        AfxMessageBox(_T("프로젝트 정보가 없습니다."));
        return;
    }

	performScenarioDelete();
}


void CKmzApp::OnPopupScenarioAddyear()
{
	performScenarioAddYear();
}


void CKmzApp::OnPopupScenarioRename()
{
	performScenarioRename();
}


void CKmzApp::OnUpdatePopupScenarioCopyto( CCmdUI* pCmdUI )
{
	try
	{
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
		const TTreeItemData* pItemData       = pwndProjectView->GetSelectedTreeItemData();

		if( (pItemData->TreeItemType != KEMTreeItemTypeKDTBProjectScenario) 
			&& (pItemData->TreeItemType != KEMTreeItemTypeUserDefineProjectScenario) ) {
                pCmdUI->Enable(FALSE);	
                return;
        }

		KScenario* pScenario = (KScenario*)pItemData->Object;

		const KTDTargetMap& mapTarget = pScenario->GetTargetMap();
		
		//★ From Scenario에 Target Data가 없을시 비활성화
		int nTargets(0);
        for (auto iter = mapTarget.begin(); iter != mapTarget.end(); ++iter) {
            KTarget* pTarget = iter->second;

            if (!pTarget->IsEmptyData())
                ++nTargets;
        }
        
		if (nTargets == 0) {
			pCmdUI->Enable(FALSE);
            return;
        }
        
		const KTDScenarioMap& mapScenario = m_pProject->GetScenarioMap();
        if (mapScenario.size() == 0) {
            pCmdUI->Enable(FALSE);
            return;
        }

        pCmdUI->Enable(TRUE);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void CKmzApp::OnUpdatePopupScenarioDelete( CCmdUI* pCmdUI )
{
	try
	{
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
		const TTreeItemData* pItemData       = pwndProjectView->GetSelectedTreeItemData();

		if( (pItemData->TreeItemType != KEMTreeItemTypeKDTBProjectScenario) && (pItemData->TreeItemType != KEMTreeItemTypeUserDefineProjectScenario) )
		{
			return;
		}

		KScenario* pScenario = (KScenario*)pItemData->Object;
		//Base는 Delete 불가
		if(pScenario->IsBaseScenario() == true)
		{
			pCmdUI->Enable(FALSE);
			return;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	pCmdUI->Enable(TRUE);
}


void CKmzApp::OnUpdatePopupScenarioRename( CCmdUI* pCmdUI )
{
	try
	{
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
		const TTreeItemData* pItemData       = pwndProjectView->GetSelectedTreeItemData();

		if( (pItemData->TreeItemType != KEMTreeItemTypeKDTBProjectScenario) && (pItemData->TreeItemType != KEMTreeItemTypeUserDefineProjectScenario) )
		{
			return;
		}

		KScenario* pScenario = (KScenario*)pItemData->Object;
		//Base는 Delete 불가
		if(pScenario->IsBaseScenario() == true)
		{
			pCmdUI->Enable(FALSE);
			return;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	pCmdUI->Enable(TRUE);
}


void CKmzApp::OnUpdatePopupScenarioAddYear( CCmdUI* pCmdUI )
{
	try
	{
		CMainFrameWnd*       pFrame          = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
		KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();
		const TTreeItemData* pItemData       = pwndProjectView->GetSelectedTreeItemData();

		if( (pItemData->TreeItemType != KEMTreeItemTypeKDTBProjectScenario) && (pItemData->TreeItemType != KEMTreeItemTypeUserDefineProjectScenario) )
		{
			return;
		}
		KScenario* pScenario = (KScenario*)pItemData->Object;

		if(pScenario->IsBaseScenario() == true)		// Base 는 오직 하나의 Year만을 갖는다.
		{
			const KTDTargetMap& targetMap = pScenario->GetTargetMap();

			if (targetMap.size() < 1)
			{
				pCmdUI->Enable(TRUE);
			}
			else
			{
				pCmdUI->Enable(FALSE);
			}

			return;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	pCmdUI->Enable(TRUE);
}


void CKmzApp::OnPopupTargetCopyto()
{
	performTargetCopyTo();
}


void CKmzApp::OnPopupTargetDelete( void )
{
	ASSERT( m_pProject );

	performTargetDelete();
}


void CKmzApp::OnPopupTargetRename( void )
{
	performtargetRename();
}


void CKmzApp::OnPopupTargetImportBlank()
{
	PerformTargetImportBLANK();
}

void CKmzApp::OnPopupNaviItemAdd( void )
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST(CMainFrameWnd, m_pMainWnd);
	
	KNaviExplorerWnd* pWndNaviView = pFrame->GetNaviTreeView();
	
	if( NULL != pWndNaviView )
		pWndNaviView->AddNaviItem();
}

void CKmzApp::OnPopupNaviItemRename( void )
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST(CMainFrameWnd, m_pMainWnd);

	KNaviExplorerWnd* pWndNaviView = pFrame->GetNaviTreeView();

	if( NULL != pWndNaviView )
		pWndNaviView->RenameNaviItem();
}

void CKmzApp::OnPopupNaviItemDelete( void )
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST(CMainFrameWnd, m_pMainWnd);

	KNaviExplorerWnd* pWndNaviView = pFrame->GetNaviTreeView();

	if (NULL != pWndNaviView)
		pWndNaviView->RemoveNaviItem();
}

void CKmzApp::OnPopupTargetImport()
{
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

	if( NULL != pwndProjectView )
	{
		const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) && (NULL != pItemData->Object) )
		{
			if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget))
			{
				KTarget* pTarget = (KTarget*)pItemData->Object;
                KImportTableDlg dlg;
                dlg.SetInitValue( pTarget, 0 );
                dlg.DoModal();
			}
		}
	}
}

void CKmzApp::OnUpdatePopupTargetImport( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

	if( NULL != pwndProjectView )
	{
		const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) && (NULL != pItemData->Object) )
		{
			if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget))
			{
				KTarget*   pTarget   = (KTarget*)pItemData->Object;
				KIOTables* pIOTables = pTarget->Tables();
				if( nullptr != pIOTables)
					bEnable = TRUE;
			}
		}
	}
	pCmdUI->Enable( bEnable );
}

void CKmzApp::OnPopupTargetExport()
{
    CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
    KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

    if( NULL != pwndProjectView )
    {
        const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
        if( (NULL != pItemData) && (NULL != pItemData->Object) )
        {
            if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget))
            {
                KTarget* pTarget = (KTarget*)pItemData->Object;
                KImportTableDlg dlg;
                dlg.SetInitValue( pTarget, 1 );
                dlg.DoModal();
            }
        }
    }
}

void CKmzApp::OnUpdatePopupTargetExport( CCmdUI* pCmdUI )
{
    BOOL bEnable = FALSE;
    CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
    KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

    if( NULL != pwndProjectView )
    {
        const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
        if( (NULL != pItemData) && (NULL != pItemData->Object) )
        {
            if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget))
            {
                KTarget*   pTarget   = (KTarget*)pItemData->Object;
                KIOTables* pIOTables = pTarget->Tables();
                if( nullptr != pIOTables)
                    bEnable = TRUE;
            }
        }
    }
    pCmdUI->Enable( bEnable );
}

void CKmzApp::OnUpdatePopupTargetCopyto( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;
	CMainFrameWnd* pFrame = STATIC_DOWNCAST( CMainFrameWnd, m_pMainWnd );
	KProjectExplorerWnd* pwndProjectView = pFrame->GetProjectView();

	if( NULL != pwndProjectView )
	{
		const TTreeItemData* pItemData = pwndProjectView->GetSelectedTreeItemData();
		if( (NULL != pItemData) && (NULL != pItemData->Object) )
		{
			if ((pItemData->TreeItemType == KEMTreeItemTypeKTDBProjectTarget) || (pItemData->TreeItemType == KEMTreeItemTypeUserDefineProjectTarget))
			{
				KTarget*   pSourceTarget  = (KTarget*)pItemData->Object;

				// 2019.05.28 : Free 버전 관련 수정
				if (!theApp.IsFreeVersion() || pSourceTarget->IsUseFreeVersion())
				{
					//base Scenario;
					KScenario*                   pBaseScenario = m_pProject->GetBaseScenario();
					const KTDTargetMap&          mapBaseTarget = pBaseScenario->GetTargetMap();
					KTDTargetMap::const_iterator citTargetMap = mapBaseTarget.begin();

					while (mapBaseTarget.end() != citTargetMap)
					{
						KTarget* pTarget = citTargetMap->second;

						if (pTarget != pSourceTarget)
							bEnable = TRUE;

						++citTargetMap;
					}

					const KTDScenarioMap& scenarioMap = m_pProject->GetScenarioMap();
					KTDScenarioMap::const_iterator citScenarioMap = scenarioMap.begin();
					while (scenarioMap.end() != citScenarioMap)
					{
						KScenario*          pScenario = citScenarioMap->second;
						const KTDTargetMap& targetMap = pScenario->GetTargetMap();

						if (targetMap.size() > 0)
						{
							KTDTargetMap::const_iterator citTargetMap = targetMap.begin();
							while (targetMap.end() != citTargetMap)
							{
								KTarget* pTarget = citTargetMap->second;

								if (pTarget != pSourceTarget)
								{
									bEnable = TRUE;
								}
								++citTargetMap;
							}
						}
						++citScenarioMap;
					}
				}
			}
		}
	}

	pCmdUI->Enable( bEnable );
}


void CKmzApp::OnNetworkCompare()
{
    m_spNetworkCompareDlg = KNetworkCompareDlgPtr(new KNetworkCompareDlg());

    m_spNetworkCompareDlg->Create(KNetworkCompareDlg::IDD);
    m_spNetworkCompareDlg->ShowWindow(SW_SHOW);
    m_spNetworkCompareDlg->CenterWindow();
}


void CKmzApp::OnUpdateNetworkCompare(CCmdUI *pCmdUI)
{
    if (m_pProject == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    auto mapScenario = m_pProject->GetScenarioMap();
    if (mapScenario.size() == 0) {
        pCmdUI->Enable(FALSE);
        return;
    }

    int nCnt(0);
    //★ 베이스
    KScenario* pScenario = m_pProject->GetBaseScenario();
    if (pScenario) {
        auto mapTarget = pScenario->GetTargetMap();
        for (auto iter = mapTarget.begin(); iter != mapTarget.end(); ++iter) {
            KTarget* pTarget = iter->second;
            if (pTarget->IsEmptyData() == false) {
                nCnt++;
            }
        }
    }

    for (auto iter = mapScenario.begin(); iter != mapScenario.end(); ++iter) {
        KScenario* pScenario = iter->second;
        auto mapTarget = pScenario->GetTargetMap();
        for (auto iter = mapTarget.begin(); iter != mapTarget.end(); ++iter) {
            KTarget* pTarget = iter->second;
            if (pTarget->IsEmptyData() == false) {
                nCnt++;
            }

            if (nCnt >= 2) {
                pCmdUI->Enable(TRUE);    
                return;
            }
        }
    }

    pCmdUI->Enable(FALSE);    
}


void CKmzApp::OnMenuPreference()
{
    KPreferenceForm oDlg;
    oDlg.DoModal();
}

void CKmzApp::DeletePath( CString strPath )
{
	CFileFind finder;
	BOOL bContinue = TRUE;

	if(strPath.Right(1) != _T("\\"))
		strPath += _T("\\");

	strPath += _T("*.*");
	bContinue = finder.FindFile(strPath);
	while(bContinue)
	{
		bContinue = finder.FindNextFile();
		if(finder.IsDots()) // Ignore this item.
		{
			continue;
		}
		else if(finder.IsDirectory()) // Delete all sub item.
		{
			DeletePath(finder.GetFilePath());
			::RemoveDirectory((LPCTSTR)finder.GetFilePath());
		}
		else // Delete file.
		{
			::DeleteFile((LPCTSTR)finder.GetFilePath());
		}
	}

	finder.Close();

	strPath = strPath.Left(strPath.ReverseFind('\\'));  
	::RemoveDirectory((LPCTSTR)strPath);
}

void CKmzApp::OnProjectCopy( void )
{
	KCopyProject oDlg;
	oDlg.DoModal();
}

void CKmzApp::OnUpdateProjectCopy( CCmdUI* pCmdUI )
{
    pCmdUI->Enable( (NULL != m_pProject ) ? TRUE : FALSE );
}


void CKmzApp::OnProjectVaccum()
{
    try
    {
        CString strMsg(_T("DB 최적화를 진행하시겠습니까?\nDB 상태에 따라 최적화 시간이 장시간 소요될 수도 있습니다."));
        INT_PTR nResult = AfxMessageBox(strMsg, MB_YESNO);
        if (IDNO == nResult)
            return;

        if (m_pProject)
    	    m_pProject->Vaccum();
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

void CKmzApp::KillKlauncher( void )
{
#pragma region comment 
/*

*/
#pragma endregion comment
	if (m_hLauncherProc == nullptr)
    {
		return;
    }

	try
	{
		DWORD dwExitCode;

        // nombara : 잘못된 코드 -> 현재 버전에서는 유지 시킴.
		::GetExitCodeProcess(m_hLauncherProc, &dwExitCode);
		::TerminateProcess  (m_hLauncherProc, dwExitCode);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	m_hLauncherProc = nullptr;
	TxLogDebugEndMsg();
}

void CKmzApp::UpdateAppTitle( void )
{
#pragma region comment
/* 
시스템 Title 변경
*/
#pragma endregion comment

	CString strTitle(_T("")), strApp(_T(""));

	strApp.LoadString(IDR_MAINFRAME);

	// 2019.05.24 : Free 버전 관련 수정
	if (m_bFreeVersion)
	{
		strApp += _T("(Free)");
	}
	if (m_pProject != nullptr)
	{
		strTitle.Format(_T("%s - %s[%s]"), strApp, m_pProject->GetName(), m_pProject->GetLocation());
	}
	else
	{
		strTitle = strApp;
	}

	m_pMainWnd->SetWindowText(strTitle);
}


bool CKmzApp::IsFreeVersion()
{
	return m_bFreeVersion;
}