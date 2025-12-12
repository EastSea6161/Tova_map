
// KLauncher.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include <vector>

// CKLauncherApp:
// 이 클래스의 구현에 대해서는 KLauncher.cpp을 참조하십시오.
//

class KCallProcessInfo
{
public:
    KCallProcessInfo(DWORD a_dwProcessID, CString a_strWindowName) : ProcessID(a_dwProcessID), WindowName(a_strWindowName)
    {
    }
    ~KCallProcessInfo() {}
public:
    DWORD   ProcessID;
    CString WindowName;
};

class CKLauncherApp : public CWinApp
{
public:
	CKLauncherApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	DECLARE_MESSAGE_MAP()
private:
    static HWND m_hwndTasOpener;
private:
    static BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam);

private:
    static std::vector<CString> ParseString(CString a_strInput, TCHAR a_chSep);
public:
    static void ReceiveStatusMessage(void* pParam1, int nStatus, char* strMsg);
public:
    static void SendResult(int nResult);
public:
    //64Bit Test
    int Hello64();

	/*Highway경로탐색*/ 
    int HighwayShortestPath();
	
	/*Highway Assign Model */ 
	int HighwayAssignModel();
	
	int HighwayAssignModelUEOB();

	int HighwayAssignModelUEOBMC();

	int HighwayAssignModelOBWarmStart();

	int MultiModalAssignModel();

	int PathVolumeAnalysis();

	//Selected link
	int LinkAnalysis();
	//Tool>Pathfind>Transit
	int TransitShortestPath(void);
	//Tool>Pathfind>Intermodal
	int InterModalShortestPath(void);
	//Statistics>신뢰수준
	int MultiStatisticsAnalysis(void);
	//Tools>TimeOD
	int TimeODAnalysis(void);

	//Transit Path Generator
	int GenerationTransitPath(void);

	//Passenger>ModeChoice
	int ModeChoiceAnalysis(void);

	int AccessStopNodeSetting(void);

	int ParameterODCalculater(void);

	int NetworkInspection(void);

	int AccessTerminalNodeSetting(void);

	int InterModalPathGenerator(void);

	int InterModalAssignment(void);

	int SubNetworkAnalysis(void);

	int PolicyImprovement(void);

	int PolicyConstuctionDemolition(void);

	int PolicyEventGrowAnalysis(void);

	int PolicySkipandStopGenerate(void);
	int PolicySkipandStopAssign(void);

	int PolicyTransitComplexGenerator( void );

	int PolicyTransitComplexAssign( void );

	int PolicyTransitComplexTerminalNodeAssign( void );

	int PathVolumeAreaAnalysis();

	int Zonning();

	int ScreenCordon();

	int DistributionModel();

	int HighwayAssignOBUE_32();

	int HighwayAssignOBUEBalacingP1_32();

	int HighwayAssignOBUEBalacingP2_32();

	int GravityCalibration();

	int PA2ODModel();

	int BusExclusiveGenerator();

	int BusExclusiveAssign();

	int HighwayAssignSegment_32(void);

//64bit
public:
	int HighwayAssignModelOBWarmStart_64();
	int HighwayAssignOBUE_64();
	int HighwayAssignOBUEBalacingP1_64();
	int HighwayAssignOBUEBalacingP2_64();

	//HighwayAssign-Segment-64
	int HighwayAssignSegment_64(void);

    int InterModalAssignment_64(void);

    int HighwayAssignModelUEOBMC_64(void);
};

extern CKLauncherApp theApp;