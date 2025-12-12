#include "stdafx.h"
#include "KmzApp.h"
#include "KmzSystem.h"
#include "MainFrameWnd.h"
#include "ImChampFrameWindow.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "IOTableView.h"
#include "IOView.h"
#include "Target.h"
#include "Scenario.h"
#include "DBaseCheckData.h"
#include "DBaseInterModal.h"
#include "ImChampDir.h"
#include "Launcher.h"

unsigned __stdcall CMainFrameWnd::PolicyThreadCaller( void* p )
{
    return 0;
}


void CMainFrameWnd::PolicyStart()
{
}

CString CMainFrameWnd::PolicyInitialData( KTarget* pTarget, QBicSimpleProgressPtr spSimpleProgress )
{
    return _T("");
}

void CMainFrameWnd::OnFPolicyRailConv()
{
}

void CMainFrameWnd::OnApplicationRoadConstruction()
{
}

void CMainFrameWnd::OnApplicationRoadimprovement()
{
}

void CMainFrameWnd::OnApplicationEvent()
{
}

void CMainFrameWnd::OnApplicationTransitComplexConstruction()
{
}

void CMainFrameWnd::OnApplicationSkipandStopAnalysis()
{
}

void CMainFrameWnd::OnApplicationCO2Interpolation( void )
{
}

void CMainFrameWnd::OnApplicationCO2Emission( void )
{
}

void CMainFrameWnd::OnApplicationBusOnlyLane()
{	
}

void CMainFrameWnd::OnFPolicySOCConstruction()
{
}


void CMainFrameWnd::OnFPolicyContainer()
{
}

