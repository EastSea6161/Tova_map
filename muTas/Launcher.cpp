#include "stdafx.h"
#include "Launcher.h"
#include "ImChampDir.h"
#include "KmzApp.h"


const int DLL_USER_STOPPING			= -99;
const int DLL_UNEXPECTED_ERROR		= -9;
const int DLL_LOAD_ERROR			= -1;
const int DLL_EXPECTED_ERROR		= 0;
const int DLL_PLANNED_TERMINATION	= 1;
const int DLL_MESSAGE_RECEIVE		= 9;
const int DLL_CREATE_ARGFILE_ERROR  = -11;
const int DLL_READ_OUTPUTFILE_ERROR = -21;
const int KLAUNCHER_USER_FORCE_STOP = 99909;

void KLauncher::Run( CString a_strWindowName, int a_nBusinessProcessID )
{
    DWORD   dwProcessID    = GetCurrentProcessId();
    CString strExcuteFile = _T("\\KLauncher.exe");    
    CString strFullPath   = ImChampDir::GetAppDirectory() + strExcuteFile;

    CString strParameter;
    strParameter.Format(_T("%lu@%s@%d"), dwProcessID, a_strWindowName, a_nBusinessProcessID);

    SHELLEXECUTEINFO ShExecInfo;
    ShExecInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd         = nullptr;
    ShExecInfo.lpVerb       = NULL;
    ShExecInfo.lpFile       = strFullPath;
    ShExecInfo.lpParameters = strParameter;
    ShExecInfo.lpDirectory  = NULL;
    ShExecInfo.nShow        = SW_HIDE;
    ShExecInfo.hInstApp     = NULL;
    ShellExecuteEx(&ShExecInfo);

	//CKmzApp::KillKlauncher에서 사용됨
	theApp.m_hLauncherProc = ShExecInfo.hProcess;
	
    //종료될때까지 대기
    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	theApp.m_hLauncherProc = nullptr;
}

void KLauncher::Run64( CString a_strWindowName, int a_nBusinessProcessID )
{
    DWORD   dwProcessID    = GetCurrentProcessId();
    CString strExcuteFile = _T("\\x64\\KLauncher64.exe");    
    CString strFullPath   = ImChampDir::GetAppDirectory() + strExcuteFile;
        
    CString strParameter;
    strParameter.Format(_T("%lu@%s@%d"), dwProcessID, a_strWindowName, a_nBusinessProcessID);

    SHELLEXECUTEINFO ShExecInfo;
    ShExecInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd         = nullptr;
    ShExecInfo.lpVerb       = NULL;
    ShExecInfo.lpFile       = strFullPath;
    ShExecInfo.lpParameters = strParameter;
    ShExecInfo.lpDirectory  = NULL;
    ShExecInfo.nShow        = SW_HIDE;
    ShExecInfo.hInstApp     = NULL;
    ShellExecuteEx(&ShExecInfo);

	//CKmzApp::KillKlauncher에서 사용됨
	theApp.m_hLauncherProc = ShExecInfo.hProcess;

    //종료될때까지 대기(32->64Bit)
    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	theApp.m_hLauncherProc = nullptr;
}

void KPolicyLauncher::Run( CString a_strPolicyPath, int a_nProcessID )
{
    return;


    // a_strPolicyPath : 경로정보(_T("C:\\xProject\\P1\\Scenario 1\\2013\\Policy"));
    // Policy(시나리오 디렉토리로 이해)
    //   -----Before-(Before Target 디렉토리로 이해 : iolocal.db/network.mdb)
    //   -----After -(After  Target 디렉토리로 이해 : iolocal.db/network.mdb)

    int nSystemType = (int)KmzSystem::GetSystemType();

    CString strExcuteFile = _T("\\Policy.exe");    
    CString strFullPath   = ImChampDir::GetAppDirectory() + strExcuteFile;

    CString strParameter;  /*여객/화물구분, 프로세스ID, 경로*/
    strParameter.Format(_T("%d@%d@%s"), nSystemType, a_nProcessID, a_strPolicyPath);

    //AfxMessageBox(strParameter);
    //strCommandLine = _T("1@1@C:\\xProject\\FreightToy\\BASE\\Policy");

    SHELLEXECUTEINFO ShExecInfo;
    ShExecInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd         = nullptr;
    ShExecInfo.lpVerb       = NULL;
    ShExecInfo.lpFile       = strFullPath;
    ShExecInfo.lpParameters = strParameter;
    ShExecInfo.lpDirectory  = NULL;
    ShExecInfo.nShow        = SW_SHOWMAXIMIZED;
    ShExecInfo.hInstApp     = NULL;
    ShellExecuteEx(&ShExecInfo);

    //향후.. Thread로 처리했을 경우...
    //종료될때까지 대기
    //WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}
