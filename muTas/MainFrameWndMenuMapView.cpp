#include "stdafx.h"
#include "MainFrameWnd.h"
#include "ImChampFrameWindow.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "IOView.h"

#include "Target.h"
#include "Scenario.h"

//여기서 부터 MapView
void CMainFrameWnd::OnToolsScreenline()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetShowMapView(pTarget);
		pMapView->OnToolsScreenline();
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

void CMainFrameWnd::OnToolsCordonline()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetShowMapView(pTarget);
		pMapView->OnToolsCordonline();
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

void CMainFrameWnd::OnToolsScreenCordonLineAnalysis()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetShowMapView(pTarget);
		pMapView->OnToolsScreenCordonLineAnalysis();
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

void CMainFrameWnd::OnUpdateToolsScreenline(CCmdUI *pCmdUI)
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateToolsScreenline(pCmdUI);
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

void CMainFrameWnd::OnUpdateToolsCordonline(CCmdUI *pCmdUI)
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateToolsCordonline(pCmdUI);
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

void CMainFrameWnd::OnUpdateInfluenceCircle(CCmdUI *pCmdUI)
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateInfluenceCircle(pCmdUI);
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

void CMainFrameWnd::OnUpdateInfluenceEnvelope(CCmdUI *pCmdUI)
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateInfluenceEnvelope(pCmdUI);
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

void CMainFrameWnd::OnUpdateInfluencePolygon(CCmdUI *pCmdUI)
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateInfluencePolygon(pCmdUI);
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

void CMainFrameWnd::OnSymbologyThemeNode()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnSymbologyThemeNode();
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

void CMainFrameWnd::OnClearThemeNode()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnClearThemeNode();
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

void CMainFrameWnd::OnSymbologyThemeLink()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnSymbologyThemeLink();
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

void CMainFrameWnd::OnClearThemeLink()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnClearThemeLink();
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

void CMainFrameWnd::OnSymbologyThemeZone()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnSymbologyThemeZone();
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

void CMainFrameWnd::OnClearThemeZone()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnClearThemeZone();
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

void CMainFrameWnd::OnDesireLineZone()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnDesireLineZone();
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

void CMainFrameWnd::OnDesireLineClearZone()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnDesireLineClearZone();
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

void CMainFrameWnd::OnShortestpathHighway()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnShortestpathHighway();
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

void CMainFrameWnd::OnShortestpathTransit()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnShortestpathTransit();
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

void CMainFrameWnd::OnUpdateShortestpathTransit( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateShortestpathTransit(pCmdUI);
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

void CMainFrameWnd::OnFShortestpathHighway()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnFShortestpathHighway();
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

void CMainFrameWnd::OnFShortestpathRail()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnFShortestpathRail();
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

void CMainFrameWnd::OnFShortestpathViaFacilities()
{
	
}

void CMainFrameWnd::OnFShortestpathTotal()
{
	
}

void CMainFrameWnd::OnFLinkModeODVolume()
{
}

void CMainFrameWnd::OnPathAnalysis()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPathAnalysis();
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

void CMainFrameWnd::OnPathVolumeHighwayAreaToArea()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPathVolumeHighwayAreaToArea();
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

void CMainFrameWnd::OnPathVolumeTransitAreaToArea()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPathVolumeTransitAreaToArea();
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

void CMainFrameWnd::OnPathVolumeInterModeAreaToArea()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPathVolumeInterModeAreaToArea();
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

void CMainFrameWnd::OnPathVolumeInterAreaToArea()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPathVolumeInterAreaToArea();
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

void CMainFrameWnd::OnUpdatePathVolumeInterAreaToArea( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdatePathVolumeInterAreaToArea(pCmdUI);
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

void CMainFrameWnd::OnLinkAnalysis()
{
	try
	{
        //★ Selected Link Analysis
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnLinkAnalysis();
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

void CMainFrameWnd::OnInspectionNetworkNode()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInspectionNetworkNode();
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

void CMainFrameWnd::OnInspectionNetworkLink()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInspectionNetworkLink();
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

void CMainFrameWnd::OnInspectionNetworkZone()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInspectionNetworkZone();
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

void CMainFrameWnd::OnInspectionNetworkTurn()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInspectionNetworkTurn();
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

void CMainFrameWnd::OnDataInspectionTransit()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnDataInspectionTransit();
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

void CMainFrameWnd::OnNetworkInspectionRun()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkInspectionRun();
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

void CMainFrameWnd::OnUpdateNetworkInspectionRun( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateNetworkInspectionRun(pCmdUI);
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

void CMainFrameWnd::OnNetworkInspectionHighwayError()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkInspectionHighwayError();
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

void CMainFrameWnd::OnNetworkInspectionHighwayWarning()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkInspectionHighwayWarning();
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

void CMainFrameWnd::OnNetworkInspectionTransitError()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkInspectionTransitError();
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

void CMainFrameWnd::OnNetworkInspectionTransitWarning()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkInspectionTransitWarning();
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

void CMainFrameWnd::OnNetworkInspectionViewLog()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkInspectionViewLog();
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

void CMainFrameWnd::OnUpdateNetworkInspectionHighwayError( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateNetworkInspectionHighwayError(pCmdUI);
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

void CMainFrameWnd::OnUpdateNetworkInspectionHighwayWarning( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateNetworkInspectionHighwayWarning(pCmdUI);
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

void CMainFrameWnd::OnUpdateNetworkInspectionTransitError( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateNetworkInspectionTransitError(pCmdUI);
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

void CMainFrameWnd::OnUpdateNetworkInspectionTransitWarning( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateNetworkInspectionTransitWarning(pCmdUI);
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

void CMainFrameWnd::OnUpdateNetworkInspectionViewLog( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateNetworkInspectionViewLog(pCmdUI);
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

void CMainFrameWnd::OnScreenCordonManage()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnScreenCordonManage();
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

void CMainFrameWnd::OnUpdateScreenCordonManage( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateScreenCordonManage(pCmdUI);
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

void CMainFrameWnd::OnZoningDivision()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnZoningDivision();
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

void CMainFrameWnd::OnZoningIntegration()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnZoningIntegration();
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

void CMainFrameWnd::OnPrepareChartNetworkZone()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPrepareChartNetworkZone();
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

void CMainFrameWnd::OnPrepareChartNetworkNode()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPrepareChartNetworkNode();
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

void CMainFrameWnd::OnPrepareChartNetworkLink()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPrepareChartNetworkLink();
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

void CMainFrameWnd::OnPrepareChartPC()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPrepareChartPC();
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

void CMainFrameWnd::OnUpdatePrepareChartPC( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}


void CMainFrameWnd::OnPrepareChartOD()
{
    try
    {
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

        KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
        pMapView->OnPrepareChartOD();
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

void CMainFrameWnd::OnUpdatePrepareChartOD( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}


void CMainFrameWnd::OnAccessibilityModify()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnAccessibilityModify();
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

void CMainFrameWnd::OnMultiModalAssignResult()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnMultiModalAssignResult();
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

void CMainFrameWnd::OnUpdateMultiModalAssignResult( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateMultiModalAssignResult(pCmdUI);
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

void CMainFrameWnd::OnMultiModalTransitPathEdit()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnMultiModalTransitPathEdit();
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

void CMainFrameWnd::OnInterModalAssignResult()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInterModalAssignResult();
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

void CMainFrameWnd::OnUpdateInterModalAssignResult( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateInterModalAssignResult(pCmdUI);
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

void CMainFrameWnd::OnPathVolumeTerminalAccessEgress()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnPathVolumeTerminalAccessEgress();
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

void CMainFrameWnd::OnUpdatePathVolumeTerminalAccessEgress( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdatePathVolumeTerminalAccessEgress(pCmdUI);
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

void CMainFrameWnd::OnNetworkEditNode()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkEditNode();
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

void CMainFrameWnd::OnNetworkEditLink()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkEditLink();
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

void CMainFrameWnd::OnNetworkEditTurn()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkEditTurn();
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

void CMainFrameWnd::OnNetworkEditTransit()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkEditTransit();
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

void CMainFrameWnd::OnMatrixPurposeOD()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnMatrixPurposeOD();
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

void CMainFrameWnd::OnMatrixModeOD()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnMatrixModeOD();
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

void CMainFrameWnd::OnMatrixChainOD()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnMatrixChainOD();
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

void CMainFrameWnd::OnMatrixVehicleModeOD()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnMatrixVehicleModeOD();
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

void CMainFrameWnd::OnInterModalTargetRegionSetting()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInterModalTargetRegionSetting();
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

void CMainFrameWnd::OnInterModalAccessTerminalEditor()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInterModalAccessTerminalEditor();
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

void CMainFrameWnd::OnInterModalGlobalTerminalSetting()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInterModalGlobalTerminalSetting();
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

void CMainFrameWnd::OnInterModalPathEdit()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInterModalPathEdit();
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

void CMainFrameWnd::OnInterModalTerminalDesireLine()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnInterModalTerminalDesireLine();
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

void CMainFrameWnd::OnTerminalScheduleEdit()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnTerminalScheduleEdit();
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

void CMainFrameWnd::OnMenuToolsSubNetWork( void )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnMenuToolsSubNetWork();
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

void CMainFrameWnd::OnUpdateMenuToolsSubNetWork( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}

void CMainFrameWnd::OnNetworkeditEditlog()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNetworkeditEditlog();
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

void CMainFrameWnd::OnUpdateNetworkeditEditlog( CCmdUI *pCmdUI )
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnUpdateNetworkeditEditlog(pCmdUI);
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

void CMainFrameWnd::OnHelpNombara()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnHelpNombara();
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

void CMainFrameWnd::OnNombaraTesterA()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNombaraTesterA();
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

void CMainFrameWnd::OnNombaraTesterB()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnNombaraTesterB();
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

void CMainFrameWnd::OnModelsProcessmultiModal()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnModelsProcessmultiModal();
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

void CMainFrameWnd::OnModelsProcessInterModal()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnModelsProcessInterModal();
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

void CMainFrameWnd::OnZoneODGroupGeneration()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnZoneODGroupGeneration();
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

void CMainFrameWnd::OnViewTurnVolume()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnViewTurnVolume();
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

void CMainFrameWnd::OnThemesTerminalAEModeViewer()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnThemesTerminalAEModeViewer();
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

void CMainFrameWnd::OnUpdateThemesTerminalAEModeViewer( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}


void CMainFrameWnd::OnFDefaultPolicyContainer()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KMapView* pMapView = ImChampFrameWindow::GetMapView(pTarget);
		pMapView->OnFDefaultPolicyContainer();
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


void CMainFrameWnd::OnTransitLineThemes()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

		KIOTableView* pTableView = ImChampFrameWindow::ShowTableView(pTarget, TABLE_TRANSIT);
		if (pTableView == nullptr)
			return;

		// ShowTransitLineThemes 내부에서 ShowMapview는 실행됨
		pTableView->OnTableTransitLineThemes();
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

void CMainFrameWnd::OnUpdateTransitTransitlineThemes( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}

void CMainFrameWnd::OnTransitChart()
{
    try
    {
        KTarget*      pTarget    = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            return;
        }

        KIOTableView* pTableView = ImChampFrameWindow::ShowTableView(pTarget, TABLE_TRANSIT);
        if (pTableView == nullptr)
            return;

        // ShowTransitLineThemes 내부에서 ShowMapview는 실행됨
        pTableView->OnTableChartTransit();
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


void CMainFrameWnd::OnUpdateTransitChart( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}