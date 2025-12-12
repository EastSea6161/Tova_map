 
// KLauncher.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "KLauncher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKLauncherApp

BEGIN_MESSAGE_MAP(CKLauncherApp, CWinApp)
	//ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKLauncherApp 생성

CKLauncherApp::CKLauncherApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CKLauncherApp 개체입니다.

CKLauncherApp theApp;
HWND CKLauncherApp::m_hwndTasOpener = nullptr;

// CKLauncherApp 초기화
BOOL CKLauncherApp::InitInstance()
{	
	CWinApp::InitInstance();

    CString strCommandLine(m_lpCmdLine);
    if (strCommandLine.GetLength() == 0)
    {
        AfxMessageBox(_T("Unknown Command 1"));
        return FALSE;
    }

    //strParameter.Format(_T("%lu@%s@%d"), dwProcessID, a_strWindowName, a_nBusinessProcessID);

    std::vector<CString> vecCommand = ParseString(strCommandLine, _T('@'));
    size_t nxCount = vecCommand.size();
    if (nxCount != 3)
    {
        AfxMessageBox(_T("Unknown Command 2"));
        
        return FALSE;
    }

    DWORD dwProcessID = (DWORD)_ttoi64(vecCommand[0]);

    KCallProcessInfo oCallProcessInfo(dwProcessID, vecCommand[1]);
    BOOL bResult = EnumWindows(EnumWindowCallBack, (LPARAM)&oCallProcessInfo);

    if (bResult == TRUE)
    {
        AfxMessageBox(_T("Unknown Command 3"));
        return FALSE;
    }
    /* Old Version
    HWND hWnd = ::FindWindow(NULL, vecCommand[1]); 
    if (hWnd == NULL)
    {
        AfxMessageBox(_T("Unknown Command 3"));
        return FALSE;
    }*/
    
    int nBusinessProcessID(-1);
    try
    {
        nBusinessProcessID= _ttoi(vecCommand[2]);	
    }
    catch (...)
    {
        AfxMessageBox(_T("Unknown Command 4"));
        return FALSE;
    }
    
    int nResult = 0;

	TCHAR    szAppPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szAppPath, _MAX_PATH));		

	CString  strAppPath(szAppPath);

	int nIndex  = strAppPath.ReverseFind(_T('\\'));
	if (nIndex > 0) 
	{
		strAppPath = strAppPath.Left(nIndex);
	}
	else 
	{
		strAppPath.Empty();
	}       

	::SetCurrentDirectory(strAppPath);

    switch(nBusinessProcessID)
    {
    case 0: // 0번은 테스트 용도만 사용할 것
        nResult = Hello64();
        break;
    case 1:
        nResult = HighwayShortestPath();
        break;

	case 2:
		nResult = HighwayAssignModel();
		break;

	case 3:
		nResult = HighwayAssignModelUEOB();
		break;

	case 4:
		nResult = HighwayAssignModelUEOBMC();
		break;

	case 5:
		nResult = MultiModalAssignModel();
		break;

	case 6:
		nResult = PathVolumeAnalysis();
		break;

	case 7:
		nResult = LinkAnalysis();
		break;

	case 8:
		nResult = TransitShortestPath();
		break;

	case 9:
		nResult = InterModalShortestPath();
		break;

	case 10:
		nResult = MultiStatisticsAnalysis();
		break;

	case 11:
		nResult = TimeODAnalysis();
		break;

	case 12:
		nResult = ModeChoiceAnalysis();
		break;


	case 13:
		nResult = GenerationTransitPath();
		break;

	case 14:
		nResult = AccessStopNodeSetting();
		break;

	case 15:
		nResult = ParameterODCalculater();
		break;

	case 16:
		nResult = NetworkInspection();
		break;

	case 17:
		nResult = AccessTerminalNodeSetting();
		break;

	case 18:
		nResult = InterModalPathGenerator();
		break;

	case 19:
		nResult = InterModalAssignment();
		break;

	case 20:
		nResult = SubNetworkAnalysis();
		break;

	case 21:
		nResult = PolicyImprovement();
		break;

	case 22:
		nResult = PolicyConstuctionDemolition();
		break;

	case 23:
		nResult = PolicyEventGrowAnalysis();
		break;

	case 24:
		nResult = PolicySkipandStopGenerate();
		break;

	case 25:
		nResult = PolicySkipandStopAssign();
		break;

	case 26:
		nResult = PolicyTransitComplexGenerator();
		break;

	case 27:
		nResult = PolicyTransitComplexAssign();
		break;

	case 28:
		nResult = PolicyTransitComplexTerminalNodeAssign();
		break;

	case 29:
		nResult = PathVolumeAreaAnalysis();
		break;

	case 30:
		nResult = Zonning();
		break;

	case 31:
		nResult = HighwayAssignModelOBWarmStart();
		break;

	case 32:
		nResult = ScreenCordon();
		break;

	case 33:
		nResult = DistributionModel();
		break;

	case 34:
		nResult = HighwayAssignOBUE_32();
		break;

	case 35:
		nResult = HighwayAssignOBUEBalacingP1_32();
		break;

	case 36:
		nResult = HighwayAssignOBUEBalacingP2_32();
		break;

	case 37:
		nResult = GravityCalibration();
		break;

	case 38:
		nResult = PA2ODModel();
		break;

	case 39:
		nResult = BusExclusiveGenerator();
		break;

	case 40:
		nResult = BusExclusiveAssign();
		break;

	case 41:
		nResult = HighwayAssignSegment_32();
		break;
		
		//64bit
	case 1001:
		nResult = HighwayAssignOBUE_64();
		break;

	case 1002:
		nResult = HighwayAssignOBUEBalacingP1_64();
		break;

	case 1003:
		nResult = HighwayAssignOBUEBalacingP2_64();
		break;

	case 1004:
		nResult = HighwayAssignModelOBWarmStart_64();
		break;

	case 1005:
		nResult = HighwayAssignSegment_64();
		break;

    case 1006:
        nResult = InterModalAssignment_64();
        break;

    case 1007:
        nResult = HighwayAssignModelUEOBMC_64();
        break;

	default:
        AfxMessageBox(_T("Unknown Command 5"));
        return FALSE;
        break;

    }

    SendResult(nResult);
    
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

std::vector<CString> CKLauncherApp::ParseString( CString a_strInput, TCHAR a_chSep )
{
    std::vector<CString> vecParseString;

    int     nTokenPos = 0; 
    CString strTok;

    while (AfxExtractSubString(strTok, a_strInput, nTokenPos++, a_chSep))
    {
        // 스페이스도 데이터 
        vecParseString.push_back(strTok.Trim());
    }

    return vecParseString;
}

void CKLauncherApp::ReceiveStatusMessage(void* pParam1, int nStatus, char* szMessage)
{
    if (m_hwndTasOpener == nullptr)
        return;
        
    try 
    {
        TCHAR lpszString[1024] = {0};
#ifdef UNICODE
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szMessage, strlen(szMessage), lpszString, 1024); 
#else
        _tcscpy(lpszString, szMessage);
#endif

        COPYDATASTRUCT cds;
        cds.dwData = 9; /*Dll로 부터 받은 메시지*/ 
        cds.cbData = sizeof(TCHAR) * (_tcslen(lpszString) + 1);
        cds.lpData = lpszString;
        SendMessage(m_hwndTasOpener, WM_COPYDATA, 0/*(WPARAM)nStatus*/, (LPARAM)(LPVOID)&cds);
    }
    catch (...)
    {
    }    
}

void CKLauncherApp::SendResult( int nResult )
{
    if (m_hwndTasOpener == nullptr)
        return;

    try
    {
        TCHAR lpszString[1024] = {0};

        COPYDATASTRUCT cds;
        cds.dwData = nResult; /*결과정보*/ 
        cds.cbData = sizeof(TCHAR) * (_tcslen(lpszString) + 1);
        cds.lpData = lpszString;
        SendMessage(m_hwndTasOpener, WM_COPYDATA, 0/*(WPARAM)nResult*/, (LPARAM)(LPVOID)&cds);
    }
    catch (...) 
    {
    }    
}

// Only 64Bit..
int CKLauncherApp::Hello64()
{
    try
    {
        HINSTANCE hAssignment;
        typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
        FASSIGNMENT AssignmentProcPtr;

        hAssignment = ::LoadLibrary(_T("FAssignment64.dll"));

        if( NULL == hAssignment )
        {
            AfxMessageBox(_T("Hello64 : null"));
            return -1;
        }

        AssignmentProcPtr = (FASSIGNMENT)(GetProcAddress(hAssignment, "FASSIGNMENT"));

        if(NULL == AssignmentProcPtr)
        {
            ::FreeLibrary(hAssignment);
            AfxMessageBox(_T("Hello64 : Error"));
            return -1;
        }

        int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
        return nResult;
    }
    catch (...)
    {   
    }    
    return 0;
}


int CKLauncherApp::HighwayShortestPath()
{
    try
    {
		HINSTANCE hAssignment;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcPtr;

		hAssignment = ::LoadLibrary(_T("FAssignment.dll"));

		if( NULL == hAssignment )
		{
			// AfxMessageBox(_T("FAssignment.dll이 없습니다."));
			return -1;
		}

		AssignmentProcPtr = (FASSIGNMENT)(GetProcAddress(hAssignment, "FHIGHWAYSHORTESTPATH"));

		if(NULL == AssignmentProcPtr)
		{
			//AfxMessageBox(_T("FHIGHWAYSHORTESTPATH 함수가 없습니다."));
			::FreeLibrary(hAssignment);
			return -1;
		}

        int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
        return nResult;
    }
    catch (...)
    {   
    }    
    return 0;
}

int CKLauncherApp::HighwayAssignModel()
{
	try
	{
		HINSTANCE hAssignment;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcPtr;

		hAssignment = ::LoadLibrary(_T("FAssignment.dll"));

		if( NULL == hAssignment )
		{
			return -1;
		}

		AssignmentProcPtr = (FASSIGNMENT)(GetProcAddress(hAssignment, "FASSIGNMENT"));

		if(NULL == AssignmentProcPtr)
		{
			::FreeLibrary(hAssignment);
			return -1;
		}

		int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignModelUEOB()
{
	try
	{
		HINSTANCE hAssignment;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcPtr;

		hAssignment = ::LoadLibrary(_T("FOriginBasedUE.dll"));

		if( NULL == hAssignment )
		{
			return -1;
		}

		AssignmentProcPtr = (FASSIGNMENT)(GetProcAddress(hAssignment, "FASSIGNMENT"));

		if(NULL == AssignmentProcPtr)
		{
			::FreeLibrary(hAssignment);
			return -1;
		}

		int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignModelUEOBMC()
{
	try
	{
		HINSTANCE hAssignment;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcPtr;

		hAssignment = ::LoadLibrary(_T("FOriginBasedUE_MC.dll"));

		if( NULL == hAssignment )
		{
			return -1;
		}

		AssignmentProcPtr = (FASSIGNMENT)(GetProcAddress(hAssignment, "FASSIGNMENT"));

		if(NULL == AssignmentProcPtr)
		{
			::FreeLibrary(hAssignment);
			return -1;
		}

		int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::MultiModalAssignModel()
{
	try
	{
		HINSTANCE hAssignment;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcPtr;

		hAssignment = ::LoadLibrary(_T("FMultiModalTA.dll"));

		if( NULL == hAssignment )
		{
			return -1;
		}

		AssignmentProcPtr = (FASSIGNMENT)(GetProcAddress(hAssignment, "FMULTIMODALTA"));

		if(NULL == AssignmentProcPtr)
		{
			::FreeLibrary(hAssignment);
			return -1;
		}

		int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::PathVolumeAnalysis()
{
	try
	{
		HINSTANCE hAssignment;
		typedef int (*PATHVOLUMEANALYSIS)(void* pParam, void* pCallback);    
		PATHVOLUMEANALYSIS AssignmentProcPtr;

		hAssignment = ::LoadLibrary(_T("PathVolumeAnalysis.dll"));

		if( NULL == hAssignment )
		{
			return -1;
		}

		AssignmentProcPtr = (PATHVOLUMEANALYSIS)(GetProcAddress(hAssignment, "PATHVOLUMEANALYSIS"));

		if(NULL == AssignmentProcPtr)
		{
			::FreeLibrary(hAssignment);
			return -1;
		}

		int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::LinkAnalysis()
{
	try
	{
		HINSTANCE hAssignment;
		typedef int (*SELECTEDLINKANALYSIS)(void* pParam, void* pCallback);    
		SELECTEDLINKANALYSIS AssignmentProcPtr;

		hAssignment = ::LoadLibrary(_T("SelectedLinkAnalysis.dll"));

		if( NULL == hAssignment )
		{
			return -1;
		}

		AssignmentProcPtr = (SELECTEDLINKANALYSIS)(GetProcAddress(hAssignment, "SELECTEDLINKANALYSIS"));

		if(NULL == AssignmentProcPtr)
		{
			::FreeLibrary(hAssignment);
			return -1;
		}

		int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{
		
	}
	return 0;
}

int CKLauncherApp::TransitShortestPath( void )
{
	try
	{
		HINSTANCE hAssignment;
		typedef int (*FTRANSITSHORTESTPATH)(void* pParam, void* pCallback);    
		FTRANSITSHORTESTPATH AssignmentProcPtr;

		hAssignment = ::LoadLibrary(_T("FTransitShortestPath.dll"));

		if( NULL == hAssignment )
		{
			return -1;
		}

		AssignmentProcPtr = (FTRANSITSHORTESTPATH)(GetProcAddress(hAssignment, "FTRANSITSHORTESTPATH"));

		if(NULL == AssignmentProcPtr)
		{
			::FreeLibrary(hAssignment);
			return -1;
		}

		int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::InterModalShortestPath( void )
{
	try
	{
		HINSTANCE hIntermodal;
		typedef int (*FINTERMODALSHORTESTPATH)(void* pParam, void* pCallback);    
		FINTERMODALSHORTESTPATH IntermodalProcPtr;

		hIntermodal = ::LoadLibrary(_T("FUrbanIntermodal.dll"));

		if( NULL == hIntermodal )
		{
			return -1;
		}

		IntermodalProcPtr = (FINTERMODALSHORTESTPATH)(GetProcAddress(hIntermodal, "FINTERMODALSHORTESTPATH"));

		if(NULL == IntermodalProcPtr)
		{
			::FreeLibrary(hIntermodal);
			return -1;
		}

		int nResult = IntermodalProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::MultiStatisticsAnalysis( void )
{
	try
	{
		HINSTANCE hStatistics;
		typedef int (*STATISTICALANALYSIS)(void* pParam, void* pCallback);    
		STATISTICALANALYSIS StatisticsProcPtr;

		hStatistics = ::LoadLibrary(_T("StaAnalysis.dll"));

		if( NULL == hStatistics )
		{
			return -1;
		}

		StatisticsProcPtr = (STATISTICALANALYSIS)(GetProcAddress(hStatistics, "STATISTICALANALYSIS"));

		if(NULL == StatisticsProcPtr)
		{
			::FreeLibrary(hStatistics);
			return -1;
		}

		int nResult = StatisticsProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::TimeODAnalysis( void )
{
	try
	{
		HINSTANCE hTimeOD;
		typedef int (*FTIMEODGENERATION)(void* pParam, void* pCallback);    
		FTIMEODGENERATION TimeODProcPtr;

		hTimeOD = ::LoadLibrary(_T("FTimeODGeneration.dll"));

		if( NULL == hTimeOD )
		{
			return -1;
		}

		TimeODProcPtr = (FTIMEODGENERATION)(GetProcAddress(hTimeOD, "FTIMEODGENERATION"));

		if(NULL == TimeODProcPtr)
		{
			::FreeLibrary(hTimeOD);
			return -1;
		}

		int nResult = TimeODProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::GenerationTransitPath( void )
{
	try
	{

		HINSTANCE hGenerationBusPath;
		typedef int (*FGENERATEBUSPATH)(void* pParam, void* pCallback);    
		FGENERATEBUSPATH SetGenerationBusPathProcPtr;

		hGenerationBusPath = ::LoadLibrary(_T("FGenerateTransitPath.dll"));

		if( NULL == hGenerationBusPath )
		{
			return -1;
		}

		SetGenerationBusPathProcPtr = (FGENERATEBUSPATH)(GetProcAddress(hGenerationBusPath, "FGENERATETRANSITPATH"));

		if(NULL == SetGenerationBusPathProcPtr)
		{
			::FreeLibrary(hGenerationBusPath);
			return -1;
		}

		int nResult = SetGenerationBusPathProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::ModeChoiceAnalysis( void )
{
	try
	{
		HINSTANCE hTimeOD;
		typedef int (*FMODECHOICE)(void* pParam, void* pCallback);    
		FMODECHOICE FModeChoiceProPtr;

		hTimeOD = ::LoadLibrary(_T("FModeChoice.dll"));

		if( NULL == hTimeOD )
		{
			return -1;
		}

		FModeChoiceProPtr = (FMODECHOICE)(GetProcAddress(hTimeOD, "FMODECHOICE"));

		if(NULL == FModeChoiceProPtr)
		{
			::FreeLibrary(hTimeOD);
			return -1;
		}

		int nResult = FModeChoiceProPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::AccessStopNodeSetting( void )
{
	try
	{
		HINSTANCE hAccessNodeSetting;
		typedef int (*FSETACCESSIBILITY)(void* pParam, void* pCallback);    
		FSETACCESSIBILITY SetAccessibilityProcPtr;

		hAccessNodeSetting = ::LoadLibrary(_T("FSetAccessibility.dll"));

		if( NULL == hAccessNodeSetting )
		{
			return -1;
		}

		SetAccessibilityProcPtr = (FSETACCESSIBILITY)(GetProcAddress(hAccessNodeSetting, "FSETACCESSIBILITY"));

		if(NULL == SetAccessibilityProcPtr)
		{
			::FreeLibrary(hAccessNodeSetting);
			return -1;
		}

		int nResult = SetAccessibilityProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::ParameterODCalculater( void )
{
	try
	{
		HINSTANCE hParameterSetting;
		typedef int (*FODPARAMETER)(void* pParam, void* pCallback);    
		FODPARAMETER SetParameterProcPtr;

		hParameterSetting = ::LoadLibrary(_T("FODParameter.dll"));

		if( NULL == hParameterSetting )
		{
			return -1;
		}

		SetParameterProcPtr = (FODPARAMETER)(GetProcAddress(hParameterSetting, "FODPARAMETER"));

		if(NULL == SetParameterProcPtr)
		{
			::FreeLibrary(hParameterSetting);
			return -1;
		}

		int nResult = SetParameterProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}


int CKLauncherApp::NetworkInspection( void )
{
	try
	{
		HINSTANCE hDataInspection;
		typedef int (*FDATAINSPECTION)(void* pParam, void* pCallback);    
		FDATAINSPECTION DataInspectionProcPtr;

		hDataInspection = ::LoadLibrary(_T("FDataInspection.dll"));

		if( NULL == hDataInspection )
		{
			return -1;
		}

		DataInspectionProcPtr = (FDATAINSPECTION)(GetProcAddress(hDataInspection, "FDATAINSPECTION"));

		if(NULL == DataInspectionProcPtr)
		{
			::FreeLibrary(hDataInspection);
			return -1;
		}

		int nResult = DataInspectionProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::AccessTerminalNodeSetting( void )
{
	try
	{
		HINSTANCE hAccessTerminalSetting;
		typedef int (*FTERMINALNODESETTING)(void* pParam, void* pCallback);    
		FTERMINALNODESETTING FTerminalNodeSettingPtr;

		hAccessTerminalSetting = ::LoadLibrary(_T("FTerminalNodeSetting.dll"));

		if( NULL == hAccessTerminalSetting )
		{
			return -1;
		}

		FTerminalNodeSettingPtr = (FTERMINALNODESETTING)(GetProcAddress(hAccessTerminalSetting, "FTERMINALNODESETTING"));

		if(NULL == FTerminalNodeSettingPtr)
		{
			::FreeLibrary(hAccessTerminalSetting);
			return -1;
		}

		int nResult = FTerminalNodeSettingPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::InterModalPathGenerator( void )
{
	try
	{
		HINSTANCE hInterModalPathGenerator;
		typedef int (*FINTERMODALPATHGENERATOR)(void* pParam, void* pCallback);    
		FINTERMODALPATHGENERATOR FInterModalPathGeneratorPtr;

		hInterModalPathGenerator = ::LoadLibrary(_T("FIntermodalPathGenerator.dll"));

		if( NULL == hInterModalPathGenerator )
		{
			return -1;
		}

		FInterModalPathGeneratorPtr = (FINTERMODALPATHGENERATOR)(GetProcAddress(hInterModalPathGenerator, "FINTERMODALPATHGENERATOR"));

		if(NULL == FInterModalPathGeneratorPtr)
		{
			::FreeLibrary(hInterModalPathGenerator);
			return -1;
		}

		int nResult = FInterModalPathGeneratorPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::InterModalAssignment( void )
{
	try
	{
		HINSTANCE hInterModalAssignment;
		typedef int (*FINTERMODALTA)(void* pParam, void* pCallback);    
		FINTERMODALTA FInterModalAssignmentPtr;

		hInterModalAssignment = ::LoadLibrary(_T("FIntermodalTA.dll"));

		if( NULL == hInterModalAssignment )
		{
			return -1;
		}

		FInterModalAssignmentPtr = (FINTERMODALTA)(GetProcAddress(hInterModalAssignment, "FINTERMODALTA"));

		if(NULL == FInterModalAssignmentPtr)
		{
			::FreeLibrary(hInterModalAssignment);
			return -1;
		}

		int nResult = FInterModalAssignmentPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::SubNetworkAnalysis( void )
{
	try
	{
		HINSTANCE hSubNetwork;
		typedef int (*FSUBNETWORK)(void* pParam, void* pCallback);
		FSUBNETWORK FSubnetworkPtr;

		hSubNetwork = ::LoadLibrary(_T("FGenSubOD.dll"));

		if (NULL == hSubNetwork)
			return -1;

		FSubnetworkPtr = (FSUBNETWORK)(GetProcAddress(hSubNetwork, "FGENSUBOD"));

		if (NULL == FSubnetworkPtr)
		{
			::FreeLibrary(hSubNetwork);
			return -1;
		}

		int nResult = FSubnetworkPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{
		
	}
	return 0;
}

int CKLauncherApp::PolicyImprovement( void )
{
	try
	{
		HINSTANCE hPolicyImprovement;
		typedef int (*FPIMPROVEMENT)(void* pParam, void* pCallback);
		FPIMPROVEMENT FPImporvementPtr;

		hPolicyImprovement = ::LoadLibrary(_T("FExpAndImpOfRoads.dll"));

		if (NULL == hPolicyImprovement)
			return -1;

		FPImporvementPtr = (FPIMPROVEMENT)(GetProcAddress(hPolicyImprovement, "FEXPANDIMPOFROADS"));

		if (NULL == FPImporvementPtr)
		{
			::FreeLibrary(hPolicyImprovement);
			return -1;
		}

		int nResult = FPImporvementPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::PolicyConstuctionDemolition( void )
{
	try
	{
		HINSTANCE hPolicyNewDemolition;
		typedef int (*FNEWANDDEMOLITIONOFROADS)(void* pParam, void* pCallback);
		FNEWANDDEMOLITIONOFROADS FPolicyNewDemolitionPtr;

		hPolicyNewDemolition = ::LoadLibrary(_T("FNewAndDemolitionOfRoads.dll"));

		if (NULL == hPolicyNewDemolition)
			return -1;

		FPolicyNewDemolitionPtr = (FNEWANDDEMOLITIONOFROADS)(GetProcAddress(hPolicyNewDemolition, "FNEWANDDEMOLITIONOFROADS"));

		if (NULL == FPolicyNewDemolitionPtr)
		{
			::FreeLibrary(hPolicyNewDemolition);
			return -1;
		}

		int nResult = FPolicyNewDemolitionPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::PolicyEventGrowAnalysis( void )
{
	try
	{
		HINSTANCE hPolicyEventGrow;
		typedef int (*FPOLICYEVENTGROW)(void* pParam, void* pCallback);
		FPOLICYEVENTGROW FPolicyEventGrowPtr;

		hPolicyEventGrow = ::LoadLibrary(_T("FProdAttrChange.dll"));

		if (NULL == hPolicyEventGrow)
			return -1;

		FPolicyEventGrowPtr = (FPOLICYEVENTGROW)(GetProcAddress(hPolicyEventGrow, "FPRODATTRCHANGE"));

		if (NULL == FPolicyEventGrowPtr)
		{
			::FreeLibrary(hPolicyEventGrow);
			return -1;
		}

		int nResult = FPolicyEventGrowPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::PolicySkipandStopGenerate( void )
{
	try
	{
		HINSTANCE hPolicySkipandStopGeneration;
		typedef int (*FPOLICYSKIPANDSTOPGENER)(void* pParam, void* pCallback);
		FPOLICYSKIPANDSTOPGENER FPolicySkipandStopPtr;

		hPolicySkipandStopGeneration = ::LoadLibrary(_T("FSkipAndStopTransitPath.DLL"));

		if (NULL == hPolicySkipandStopGeneration)
			return -1;

		FPolicySkipandStopPtr = (FPOLICYSKIPANDSTOPGENER)(GetProcAddress(hPolicySkipandStopGeneration, "FSKIPANDSTOPTRANSITPATH"));

		if (NULL == FPolicySkipandStopPtr)
		{
			::FreeLibrary(hPolicySkipandStopGeneration);
			return -1;
		}

		int nResult = FPolicySkipandStopPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}

int CKLauncherApp::PolicySkipandStopAssign( void )
{
	try
	{
		HINSTANCE hPolicySkipAndStopAssign;
		typedef int (*FPOLICYSKIPANDSTOPASSIGN)(void* pParam, void* pCallback);
		FPOLICYSKIPANDSTOPASSIGN FPolicySkipAndStopAssignPtr;

		hPolicySkipAndStopAssign = ::LoadLibrary(_T("FSkipAndStopAssign.dll"));

		if (NULL == hPolicySkipAndStopAssign)
			return -1;

		FPolicySkipAndStopAssignPtr = (FPOLICYSKIPANDSTOPASSIGN)(GetProcAddress(hPolicySkipAndStopAssign, "FSKIPANDSTOPASSIGN"));

		if (NULL == FPolicySkipAndStopAssignPtr)
		{
			::FreeLibrary(hPolicySkipAndStopAssign);
			return -1;
		}

		int nResult = FPolicySkipAndStopAssignPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}


int CKLauncherApp::PolicyTransitComplexGenerator( void )
{
	try
	{
		HINSTANCE hPolicyTransitComplexGenerator;
		typedef int (*FPOLICYTRANSITCOMPLEXGENER)(void* pParam, void* pCallback);
		FPOLICYTRANSITCOMPLEXGENER FPolicyTransitComplexGenPtr;

		hPolicyTransitComplexGenerator = ::LoadLibrary(_T("FIntermodalPathGenerator_P.dll"));

		if (NULL == hPolicyTransitComplexGenerator)
			return -1;

		FPolicyTransitComplexGenPtr = (FPOLICYTRANSITCOMPLEXGENER)(GetProcAddress(hPolicyTransitComplexGenerator, "FINTERMODALPATHGENERATOR"));

		if (NULL == FPolicyTransitComplexGenPtr)
		{
			::FreeLibrary(hPolicyTransitComplexGenerator);
			return -1;
		}

		int nResult = FPolicyTransitComplexGenPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}


int CKLauncherApp::PolicyTransitComplexAssign( void )
{
	try
	{
		HINSTANCE hPolicyTransitComplexAssign;
		typedef int (*FPOLICYTRANSITCOMPLEXASSIGN)(void* pParam, void* pCallback);
		FPOLICYTRANSITCOMPLEXASSIGN FPolicyTransitComplexAssignPtr;

		hPolicyTransitComplexAssign = ::LoadLibrary(_T("FIntermodalTA_P.dll"));

		if (NULL == hPolicyTransitComplexAssign)
			return -1;

		FPolicyTransitComplexAssignPtr = (FPOLICYTRANSITCOMPLEXASSIGN)(GetProcAddress(hPolicyTransitComplexAssign, "FINTERMODALTA"));

		if (NULL == FPolicyTransitComplexAssignPtr)
		{
			::FreeLibrary(hPolicyTransitComplexAssign);
			return -1;
		}

		int nResult = FPolicyTransitComplexAssignPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}


int CKLauncherApp::PolicyTransitComplexTerminalNodeAssign( void )
{
	try
	{
		HINSTANCE hPolicyTransitComplexTerminalAssign;
		typedef int (*FPOLICYTRANSITCOMPLEXTERMINALASSIGN)(void* pParam, void* pCallback);
		FPOLICYTRANSITCOMPLEXTERMINALASSIGN FPolicyTransitComplexTerminalPtr;

		hPolicyTransitComplexTerminalAssign = ::LoadLibrary(_T("FTerminalNodeSetting_P.DLL"));

		if (NULL == hPolicyTransitComplexTerminalAssign)
			return -1;

		FPolicyTransitComplexTerminalPtr = (FPOLICYTRANSITCOMPLEXTERMINALASSIGN)(GetProcAddress(hPolicyTransitComplexTerminalAssign, "FTERMINALNODESETTING"));

		if (NULL == FPolicyTransitComplexTerminalPtr)
		{
			::FreeLibrary(hPolicyTransitComplexTerminalAssign);
			return -1;
		}

		int nResult = FPolicyTransitComplexTerminalPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}


int CKLauncherApp::PathVolumeAreaAnalysis()
{
	try
	{
		HINSTANCE hAssignment;
		typedef int (*FAREATOAREAHIGHWAYPATHANALYSIS)(void* pParam, void* pCallback);    
		FAREATOAREAHIGHWAYPATHANALYSIS PathVolumeProcPtr;

		hAssignment = ::LoadLibrary(_T("FAreaToAreaHighwayPathAnalysis.dll"));

		if( NULL == hAssignment )
		{
			return -1;
		}

		PathVolumeProcPtr = (FAREATOAREAHIGHWAYPATHANALYSIS)(GetProcAddress(hAssignment, "FAREATOAREAHIGHWAYPATHANALYSIS"));

		if(NULL == PathVolumeProcPtr)
		{
			::FreeLibrary(hAssignment);
			return -1;
		}

		int nResult = PathVolumeProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}


int CKLauncherApp::Zonning()
{
	try
	{
		HINSTANCE hIns;
		typedef int (*FZONING)(void *pParam, void* pCallback);
		FZONING ZoningProcPtr;

		hIns = ::LoadLibrary(_T("FZoning.dll"));

		if( NULL == hIns )
		{
			return -1;
		}

		ZoningProcPtr = (FZONING)(GetProcAddress(hIns, "FZONING"));

		if(NULL == ZoningProcPtr)
		{
			::FreeLibrary(hIns);
			return -1;
		}

		int nResult = ZoningProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{
		
	}
	return 0;
}

int CKLauncherApp::HighwayAssignModelOBWarmStart()
{
	try
	{
		HINSTANCE hAssignmentWarm;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentWarm = ::LoadLibrary(_T("FOriginBasedUEW.dll"));

		if( NULL == hAssignmentWarm )
		{
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentWarm, "FASSIGNMENT"));

		if(NULL == AssignmentProcWarmPtr)
		{
			::FreeLibrary(hAssignmentWarm);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::ScreenCordon()
{
	try
	{
		HINSTANCE hSCAnalysis;
		typedef int (*FSCREENCORDON)(void* pParam, void* pCallback);    
		FSCREENCORDON ScreenCordonPtr;

		hSCAnalysis = ::LoadLibrary(_T("FScreenCordon.dll"));

		if( NULL == hSCAnalysis )
		{
			return -1;
		}

		ScreenCordonPtr = (FSCREENCORDON)(GetProcAddress(hSCAnalysis, "FSCREENCORDON"));

		if(NULL == ScreenCordonPtr)
		{
			::FreeLibrary(hSCAnalysis);
			return -1;
		}

		int nResult = ScreenCordonPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}


int CKLauncherApp::DistributionModel( void )
{
	try
	{
		HINSTANCE hIns;
		typedef int (*FDistribution)(void* pParam, void* pCallback);
		FDistribution lfFDistribution;

		hIns = ::LoadLibrary(_T("FDistribution.dll"));
		if( hIns == NULL )
		{
			return -1;
		}

		lfFDistribution = (FDistribution)GetProcAddress( hIns, /*"FDistribution"*/ "FDISTRIBUTION" );

		if(NULL == lfFDistribution)
		{
			AfxMessageBox(_T("FDistribution 함수가 없습니다. "));
			FreeLibrary(hIns);
			return -1;
		}

		int nResult = lfFDistribution((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}


int CKLauncherApp::GravityCalibration()
{
	try
	{
		HINSTANCE hIns;
		typedef int (*FGRAVITYCALIBRATION)(void* pParam, void* pCallback);
		FGRAVITYCALIBRATION CalibrationProcPtr;

		hIns = ::LoadLibrary(_T("FGravityCalibration"));
		if( hIns == NULL )
		{
			return -1;
		}

		CalibrationProcPtr = (FGRAVITYCALIBRATION)GetProcAddress( hIns, "FGRAVITYCALIBRATION" );

		if(NULL == CalibrationProcPtr)
		{
			AfxMessageBox(_T("FGRAVITYCALIBRATION 함수가 없습니다. "));
			FreeLibrary(hIns);
			return -1;
		}

		int nResult = CalibrationProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{
		
	}
	return 0;
}


int CKLauncherApp::PA2ODModel()
{
	try
	{
		HINSTANCE hIns;
		typedef int (*FPATOOD)(void* pParam, void* pCallback);
		FPATOOD PAtoODProcPtr;

		hIns = ::LoadLibrary(_T("FPATOOD.dll"));
		if( hIns == NULL )
		{
			return -1;
		}

		PAtoODProcPtr = (FPATOOD)GetProcAddress( hIns, "FPATOOD" );

		if(NULL == PAtoODProcPtr)
		{
			AfxMessageBox(_T("FPATOOD 함수가 없습니다. "));
			FreeLibrary(hIns);
			return -1;
		}

		int nResult = PAtoODProcPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{

	}
	return 0;
}


BOOL CALLBACK CKLauncherApp::EnumWindowCallBack( HWND hWnd, LPARAM lParam )
{
    KCallProcessInfo* pCallProcessInfo = (KCallProcessInfo*)lParam;
    DWORD dwFindProcessID = pCallProcessInfo->ProcessID;

    DWORD dwWindowProcessID(0);
    ::GetWindowThreadProcessId(hWnd, &dwWindowProcessID);

    if (dwFindProcessID == dwWindowProcessID)
    {
        TCHAR buffer[MAX_PATH];
        ZeroMemory(buffer, MAX_PATH );
        ::GetWindowText(hWnd, buffer, MAX_PATH);

        int nTextLength = ::GetWindowTextLength(hWnd);  
        CString strWindowName(buffer);
        CString strFindWindowName = pCallProcessInfo->WindowName;

		//CString strMsg;
		//strMsg.Format(_T("%s - %s"), strWindowName, strFindWindowName);

		// Visible Option 제거 : Policy
        if (/*::IsWindowVisible(hWnd) &&*/ nTextLength > 0 && strFindWindowName.CompareNoCase(strWindowName) == 0)
        {
            m_hwndTasOpener = hWnd;
            return FALSE;
        }
    }

    return TRUE;
}

int CKLauncherApp::HighwayAssignOBUE_64()
{
	try
	{	
		HINSTANCE hAssignmentOBUE64;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentOBUE64 = ::LoadLibrary(_T("FOriginBasedUE_64.dll"));

		if( NULL == hAssignmentOBUE64 )
		{
			AfxMessageBox(_T("LoadError"));
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentOBUE64, "FASSIGNMENT"));

		if(NULL == AssignmentProcWarmPtr)
		{
			AfxMessageBox(_T("Function Error"));
			::FreeLibrary(hAssignmentOBUE64);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignOBUEBalacingP1_64()
{
	try
	{
		HINSTANCE hAssignmentOBUE64P1;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentOBUE64P1 = ::LoadLibrary(_T("FProportionalityForOBUE_P1_64.dll"));

		if( NULL == hAssignmentOBUE64P1 )
		{
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentOBUE64P1, "FPROPORTIONALITYFOROBUE_P1"));

		if(NULL == AssignmentProcWarmPtr)
		{
			::FreeLibrary(hAssignmentOBUE64P1);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignOBUEBalacingP2_64()
{
	try
	{
		HINSTANCE hAssignmentOBUE64P2;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentOBUE64P2 = ::LoadLibrary(_T("FProportionalityForOBUE_P2_64.dll"));

		if( NULL == hAssignmentOBUE64P2 )
		{
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentOBUE64P2, "FPROPORTIONALITYFOROBUE_P2"));

		if(NULL == AssignmentProcWarmPtr)
		{
			::FreeLibrary(hAssignmentOBUE64P2);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignOBUE_32()
{
	try
	{	
		HINSTANCE hAssignmentOBUE64;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentOBUE64 = ::LoadLibrary(_T("FOriginBasedUE.dll"));

		if( NULL == hAssignmentOBUE64 )
		{
			AfxMessageBox(_T("LoadError"));
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentOBUE64, "FASSIGNMENT"));

		if(NULL == AssignmentProcWarmPtr)
		{
			AfxMessageBox(_T("Function Error"));
			::FreeLibrary(hAssignmentOBUE64);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignOBUEBalacingP1_32()
{
	try
	{
		HINSTANCE hAssignmentOBUE64P1;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentOBUE64P1 = ::LoadLibrary(_T("FProportionalityForOBUE_P1_32.dll"));

		if( NULL == hAssignmentOBUE64P1 )
		{
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentOBUE64P1, "FPROPORTIONALITYFOROBUE_P1"));

		if(NULL == AssignmentProcWarmPtr)
		{
			::FreeLibrary(hAssignmentOBUE64P1);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignOBUEBalacingP2_32()
{
	try
	{	
		HINSTANCE hAssignmentOBUE64;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentOBUE64 = ::LoadLibrary(_T("FProportionalityForOBUE_P2_32.dll"));

		if( NULL == hAssignmentOBUE64 )
		{
			AfxMessageBox(_T("LoadError"));
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentOBUE64, "FPROPORTIONALITYFOROBUE_P2"));

		if(NULL == AssignmentProcWarmPtr)
		{
			AfxMessageBox(_T("Function Error"));
			::FreeLibrary(hAssignmentOBUE64);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignModelOBWarmStart_64()
{
	try
	{
		HINSTANCE hAssignmentWarm;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentWarm = ::LoadLibrary(_T("FOriginBasedUEW_64.dll"));

		if( NULL == hAssignmentWarm )
		{
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentWarm, "FASSIGNMENT"));

		if(NULL == AssignmentProcWarmPtr)
		{
			::FreeLibrary(hAssignmentWarm);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::BusExclusiveGenerator()
{
	try
	{
		HINSTANCE hAssignmentWarm;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentWarm = ::LoadLibrary(_T("FIntermodalPathGenerator_P_Bus.dll"));

		if( NULL == hAssignmentWarm )
		{
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentWarm, "FINTERMODALPATHGENERATOR"));

		if(NULL == AssignmentProcWarmPtr)
		{
			::FreeLibrary(hAssignmentWarm);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::BusExclusiveAssign()
{
	try
	{
		HINSTANCE hAssignmentWarm;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignmentWarm = ::LoadLibrary(_T("FIntermodalTA_P_Bus.dll"));

		if( NULL == hAssignmentWarm )
		{
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignmentWarm, "FINTERMODALTA"));

		if(NULL == AssignmentProcWarmPtr)
		{
			::FreeLibrary(hAssignmentWarm);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignSegment_32( void )
{
	try
	{	
		HINSTANCE hAssignSegment_32;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignSegment_32 = ::LoadLibrary(_T("SegmentBasedUE.DLL"));

		if( NULL == hAssignSegment_32 )
		{
			AfxMessageBox(_T("LoadError"));
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignSegment_32, "FASSIGNMENT"));

		if(NULL == AssignmentProcWarmPtr)
		{
			AfxMessageBox(_T("Function Error"));
			::FreeLibrary(hAssignSegment_32);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::HighwayAssignSegment_64( void )
{
	try
	{	
		HINSTANCE hAssignSegment_64;
		typedef int (*FASSIGNMENT)(void* pParam, void* pCallback);    
		FASSIGNMENT AssignmentProcWarmPtr;

		hAssignSegment_64 = ::LoadLibrary(_T("SegmentBasedUE_64.DLL"));

		if( NULL == hAssignSegment_64 )
		{
			AfxMessageBox(_T("LoadError"));
			return -1;
		}

		AssignmentProcWarmPtr = (FASSIGNMENT)(GetProcAddress(hAssignSegment_64, "FASSIGNMENT"));

		if(NULL == AssignmentProcWarmPtr)
		{
			AfxMessageBox(_T("Function Error"));
			::FreeLibrary(hAssignSegment_64);
			return -1;
		}

		int nResult = AssignmentProcWarmPtr((void*)this, (void*)ReceiveStatusMessage);
		return nResult;
	}
	catch (...)
	{   
	}    
	return 0;
}

int CKLauncherApp::InterModalAssignment_64(void)
{
    try
    {
        HINSTANCE hInterModalAssignment;
        typedef int(*FINTERMODALTA)(void* pParam, void* pCallback);
        FINTERMODALTA FInterModalAssignmentPtr;

        hInterModalAssignment = ::LoadLibrary(_T("FIntermodalTA_64.dll"));

        if (NULL == hInterModalAssignment)
        {
            return -1;
        }

        FInterModalAssignmentPtr = (FINTERMODALTA)(GetProcAddress(hInterModalAssignment, "FINTERMODALTA"));

        if (NULL == FInterModalAssignmentPtr)
        {
            ::FreeLibrary(hInterModalAssignment);
            return -1;
        }

        int nResult = FInterModalAssignmentPtr((void*)this, (void*)ReceiveStatusMessage);
        return nResult;
    }
    catch (...)
    {

    }
    return 0;
}

int CKLauncherApp::HighwayAssignModelUEOBMC_64()
{
    try
    {
        HINSTANCE hAssignment;
        typedef int(*FASSIGNMENT)(void* pParam, void* pCallback);
        FASSIGNMENT AssignmentProcPtr;

        hAssignment = ::LoadLibrary(_T("FOriginBasedUE_MC_64.dll"));

        if (NULL == hAssignment)
        {
            return -1;
        }

        AssignmentProcPtr = (FASSIGNMENT)(GetProcAddress(hAssignment, "FASSIGNMENT"));

        if (NULL == AssignmentProcPtr)
        {
            ::FreeLibrary(hAssignment);
            return -1;
        }

        int nResult = AssignmentProcPtr((void*)this, (void*)ReceiveStatusMessage);
        return nResult;
    }
    catch (...)
    {
    }
    return 0;
}