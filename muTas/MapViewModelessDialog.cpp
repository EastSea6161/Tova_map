#include "stdafx.h"
#include "MapView.h"
#include "ShortestPathHighwayDlg.h"
#include "ShortestPathNTransitDlg.h"

#include "PathVolumeDlg.h"
#include "PathVolumeAreaDlg.h"
#include "LinkAnalysisDlg.h"
#include "ZoningDivisionDlg.h"
#include "ZoningIntegrationDlg.h"
#include "SCManageDlg.h"
#include "KInspectionNodeDlg.h"
#include "KInspectionLinkDlg.h"
#include "KInspectionZoneDlg.h"
#include "KInspectionTurnDlg.h"
#include "KInspectionTransitDlg.h"
#include "SymbologyThemeNodeDlg.h"
#include "SymbologyThemeLinkDlg.h"
#include "SymbologyThemeZoneDlg.h"
#include "DesireLineDlg.h"
#include "AccessiblityModifyDlg.h"
#include "PrepareChartODDlg.h"
#include "PrepareChartNetworkDlg.h"
#include "PrepareChartPADlg.h"
#include "MultimodalResultDlg.h"
#include "TTransitPath.h"
#include "MapViewSelectionFeedback.h"
#include "MapViewRenderInfo.h"
#include "TransitLineThemesDlg.h"
#include "MapFindDlg.h"
#include "MTransitPathEditDlg.h"
#include "LinkViaTransitSearchDlg.h"
#include "NodeViaTransitSearchDlg.h"
#include "NetworkInspectionDlg.h"
#include "TargetRegionSetting.h"
#include "ODMatrixDlg.h"
#include "AccessTermianlEditerDlg.h"
#include "AccessNodeEditorDlg.h"
#include "GlobalTerminalSettingDlg.h"
#include "InterTransitPathEditDlg.h"
#include "TerminalScheduleDesireDlg.h"
#include "SubNetworkDlg.h"
#include "TerminalScheduleEditDlg.h"
#include "intersection_common.h"
#include "IntersectionTopologyAnalysisDlg.h"
#include "ModelProcessMultiModal.h"
#include "ZoneODGroupGenerationDlg.h"
#include "TurnVolumeViewDlg.h"
#include "ModelProcessInterModal.h"
#include "RulerDlg.h"
#include "PathVolumeAreaTransitDlg.h"
#include "PathVolumeAreaInterModeDlg.h"
#include "TerminalAEModeViewerDlg.h" 
#include "InterModalResultDlg.h"
#include "PathVolumeAreaInterDlg.h"
#include "FDefaultPolicyContainerDlg.h"
#include "StationODCalculatorDlg.h"
#include "PathVolumeTerminalAccEgrDlg.h"
#include "KDlgNetworkEditorMain.h"
#include "KDlgTransitInfluenceArea.h"

void KMapView::CloseModelessDialog()
{
    // 스마트포인터를 먼저 Close
    // - 스마트포인터이기 때문에 KMapView가 Close된 후 자동 소멸되지만
    // - 지도와 관련된 메시지를 각 다이얼로그에서 우선적으로 처리해야 오류 없이 종료 됨.
    //   -> 각 다이얼로그에서 Close 하면서 -> KMapView를 제어할 때 오류 발생...
    if( NULL != m_spShortestPathHighwayDlg ) 
        m_spShortestPathHighwayDlg.reset();
    if( NULL != m_spShortestPathNTransitDlg )
        m_spShortestPathNTransitDlg.reset();
    
    if( NULL != m_spPathVolumeDlg )
        m_spPathVolumeDlg.reset();
    if( NULL != m_spPathVolumeAreaDlg )
        m_spPathVolumeAreaDlg.reset();
    if( NULL != m_spLinkAnalysisDlg )
        m_spLinkAnalysisDlg.reset();
    if( NULL != m_spZoningDivisionDlg )
        m_spZoningDivisionDlg.reset();
    if( NULL != m_spZoningIntegrationDlg )
        m_spZoningIntegrationDlg.reset();
    if( NULL != m_spSCManageDlg )
        m_spSCManageDlg.reset();
    if( NULL != m_spInspectionNodeDlg )
        m_spInspectionNodeDlg.reset();
    if( NULL != m_spInspectionLinkDlg )
        m_spInspectionLinkDlg.reset();
    if( NULL != m_spInspectionZoneDlg )
        m_spInspectionZoneDlg.reset();
    if( NULL != m_spInspectionTurnDlg )
        m_spInspectionTurnDlg.reset();
    if( NULL != m_spInspectionTransitDlg )
        m_spInspectionTransitDlg.reset();
    if( NULL != m_spSymbologyThemeNodeDlg )
        m_spSymbologyThemeNodeDlg.reset();
    if( NULL != m_spSymbologyThemeLinkDlg )
        m_spSymbologyThemeLinkDlg.reset();
    if( NULL != m_spSymbologyThemeZoneDlg )
        m_spSymbologyThemeZoneDlg.reset();
	if (NULL != m_spRulerDlg)
		m_spRulerDlg.reset();
    if (nullptr != m_spDesireLineDlg)
        m_spDesireLineDlg.reset();
    if (nullptr != m_spPrepareChartODDlg)
        m_spPrepareChartODDlg.reset();
    if (nullptr != m_spPrepareChartNetworkDlg)
        m_spPrepareChartNetworkDlg.reset();
    if (nullptr != m_spPrepareChartPADlg)
        m_spPrepareChartPADlg.reset();
    if (nullptr != m_spMultimodalResultDlg)
        m_spMultimodalResultDlg.reset();
	if (nullptr != m_spIntermodalResultDlg)
		m_spIntermodalResultDlg.reset();
    if (nullptr != m_spTransitLineThemesDlg )
        m_spTransitLineThemesDlg.reset();
    if (nullptr != m_spMapFindDlg)
        m_spMapFindDlg.reset();
    if (nullptr != m_spMTransitPathEditDlg)
        m_spMTransitPathEditDlg.reset();

    if (nullptr != m_spAccessStopNodeEditorDlg)
        m_spAccessStopNodeEditorDlg.reset();

    if (nullptr != m_spLinkViaTransitSearchDlg)
        m_spLinkViaTransitSearchDlg.reset();

    if (nullptr != m_spNodeViaTransitSearchDlg)
        m_spNodeViaTransitSearchDlg.reset();

    if (m_spTransitInfluenceAreaDlg != nullptr)
        m_spTransitInfluenceAreaDlg.reset();

    if (nullptr != m_spNetworkInspectionDlg)
        m_spNetworkInspectionDlg.reset();

    if (nullptr != m_spODMatrixDlg)
        m_spODMatrixDlg.reset();

    if (nullptr != m_spInterTransitPathEditDlg)
        m_spInterTransitPathEditDlg.reset();

    if (nullptr != m_spTerminalScheduleDesireDlg)
        m_spTerminalScheduleDesireDlg.reset();

    if (nullptr != m_spTerminalScheduleEditDlg)
        m_spTerminalScheduleEditDlg.reset();

    if (nullptr != m_spGlobalTerminalSettingDlg)
        m_spGlobalTerminalSettingDlg.reset();

    if (nullptr != m_spModelProcessMultiModalDlg)
        m_spModelProcessMultiModalDlg.reset();

    if (nullptr != m_spModelProcessInterModalDlg)
        m_spModelProcessInterModalDlg.reset();
    
    if (nullptr != m_spIntersectionTopologyAnalysisDlg)
        m_spIntersectionTopologyAnalysisDlg.reset();

    if (nullptr != m_spZoneODGroupGenerationDlg)
        m_spZoneODGroupGenerationDlg.reset();

    if (nullptr != m_spTurnVolumeViewDlg)
        m_spTurnVolumeViewDlg.reset();
	
	if( nullptr != m_spPathVolumeAreaTransitDlg )
		m_spPathVolumeAreaTransitDlg.reset();
	
	if( nullptr != m_spPathVolumeAreaInterModeDlg )
		m_spPathVolumeAreaInterModeDlg.reset();

	if (nullptr != m_spTerminalAEModeViewerDlg)
		m_spTerminalAEModeViewerDlg.reset();

	if( nullptr != m_spPathVolumeAreaInterDlg )
		m_spPathVolumeAreaInterDlg.reset();

	if( nullptr != m_spPathVolumeTerminalAccEgrDlg )
		m_spPathVolumeTerminalAccEgrDlg.reset();

	if (m_spSubNetworkDlg != nullptr)
		m_spSubNetworkDlg.reset();

	if (m_spStationODCalculatorDlg != nullptr)
		m_spStationODCalculatorDlg.reset();

    if (m_spDlgNetworkEdior != nullptr)
        m_spDlgNetworkEdior.reset();
}


void KMapView::ActivatedFrameWndNotifyProcess()
{
    if (m_spScreenDrawEdit != NULL)
    {
        m_spScreenDrawEdit->ActivatedFrameWndNotifyProcess();
    }

    if (m_spShortestPathHighwayDlg != NULL)
    {
        if (!m_spShortestPathHighwayDlg->IsUserClosed())
        {
            if (!m_spShortestPathHighwayDlg->IsWindowVisible())
                m_spShortestPathHighwayDlg->ShowWindow(SW_SHOW);
        }
        m_spShortestPathHighwayDlg->ActivatedFrameWndNotifyProcess();
    }

    if (m_spShortestPathNTransitDlg != NULL)
    {
        if (!m_spShortestPathNTransitDlg->IsUserClosed())
        {
            if (!m_spShortestPathNTransitDlg->IsWindowVisible())
                m_spShortestPathNTransitDlg->ShowWindow(SW_SHOW);
        }
        m_spShortestPathNTransitDlg->ActivatedFrameWndNotifyProcess();
    }

    if (m_spPathVolumeDlg != NULL)
    {
        if (!m_spPathVolumeDlg->IsUserClosed())
        {
            if (!m_spPathVolumeDlg->IsWindowVisible())
                m_spPathVolumeDlg->ShowWindow(SW_SHOW);
        }
        m_spPathVolumeDlg->ActivatedFrameWndNotifyProcess();
    }

    if (m_spPathVolumeAreaDlg != NULL)
    {
        if (!m_spPathVolumeAreaDlg->IsUserClosed())
        {
            if (!m_spPathVolumeAreaDlg->IsWindowVisible())
                m_spPathVolumeAreaDlg->ShowWindow(SW_SHOW);
        }
        m_spPathVolumeAreaDlg->ActivatedFrameWndNotifyProcess();
    }

    if (m_spLinkAnalysisDlg != NULL)
    {
        if (!m_spLinkAnalysisDlg->IsUserClosed())
        {
            if (!m_spLinkAnalysisDlg->IsWindowVisible())
                m_spLinkAnalysisDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spZoningDivisionDlg != NULL)
    {
        if (!m_spZoningDivisionDlg->IsUserClosed())
        {
            if (!m_spZoningDivisionDlg->IsWindowVisible())
                m_spZoningDivisionDlg->ShowWindow(SW_SHOW);
        }
        m_spZoningDivisionDlg->ActivatedFrameWndNotifyProcess();
    }


    if (m_spZoningIntegrationDlg != NULL)
    {
        if (!m_spZoningIntegrationDlg->IsUserClosed())
        {
            if (!m_spZoningIntegrationDlg->IsWindowVisible())
                m_spZoningIntegrationDlg->ShowWindow(SW_SHOW);
        }
		/***************************************************************************/
		/* 2019.01.18 : exception 발생부분 수정                                      */
        //m_spZoningDivisionDlg->ActivatedFrameWndNotifyProcess();
		m_spZoningIntegrationDlg->ActivatedFrameWndNotifyProcess();
		/***************************************************************************/
    }

    if (m_spSCManageDlg != NULL)
    {
        if (!m_spSCManageDlg->IsUserClosed())
        {
            if (!m_spSCManageDlg->IsWindowVisible())
                m_spSCManageDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spInspectionNodeDlg != NULL)
    {
        if (!m_spInspectionNodeDlg->IsUserClosed())
        {
            if (!m_spInspectionNodeDlg->IsWindowVisible())
                m_spInspectionNodeDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spInspectionLinkDlg != NULL)
    {
        if (!m_spInspectionLinkDlg->IsUserClosed())
        {
            if (!m_spInspectionLinkDlg->IsWindowVisible())
                m_spInspectionLinkDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spInspectionZoneDlg != NULL)
    {
        if (!m_spInspectionZoneDlg->IsUserClosed())
        {
            if (!m_spInspectionZoneDlg->IsWindowVisible())
                m_spInspectionZoneDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spInspectionTurnDlg != NULL)
    {
        if (!m_spInspectionTurnDlg->IsUserClosed())
        {
            if (!m_spInspectionTurnDlg->IsWindowVisible())
                m_spInspectionTurnDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spInspectionTransitDlg != NULL)
    {
        if (!m_spInspectionTransitDlg->IsUserClosed())
        {
            if (!m_spInspectionTransitDlg->IsWindowVisible())
                m_spInspectionTransitDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spSymbologyThemeNodeDlg != NULL)
    {
        if(!m_spSymbologyThemeNodeDlg->IsUserClosed())
        {
            if(!m_spSymbologyThemeNodeDlg->IsWindowVisible())
                m_spSymbologyThemeNodeDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spSymbologyThemeLinkDlg != NULL)
    {
        if(!m_spSymbologyThemeLinkDlg->IsUserClosed())
        {
            if(!m_spSymbologyThemeLinkDlg->IsWindowVisible())
                m_spSymbologyThemeLinkDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spSymbologyThemeZoneDlg != NULL)
    {
        if(!m_spSymbologyThemeZoneDlg->IsUserClosed())
        {
            if(!m_spSymbologyThemeZoneDlg->IsWindowVisible())
                m_spSymbologyThemeZoneDlg->ShowWindow(SW_SHOW);
        }
    }

    if (m_spDesireLineDlg != nullptr)
    {
        if(!m_spDesireLineDlg->IsUserClosed())
        {
            if(!m_spDesireLineDlg->IsWindowVisible())
                m_spDesireLineDlg->ShowWindow(SW_SHOW);
        }
    }

    if(nullptr != m_spPrepareChartODDlg)
    {
        if(!m_spPrepareChartODDlg->IsUserClosed())
        {
            if(!m_spPrepareChartODDlg->IsWindowVisible())
                m_spPrepareChartODDlg->ShowWindow(SW_SHOW);
        }
    }

    if(nullptr != m_spPrepareChartNetworkDlg)
    {
        if(!m_spPrepareChartNetworkDlg->IsUserClosed())
            if(!m_spPrepareChartNetworkDlg->IsWindowVisible())
                m_spPrepareChartNetworkDlg->ShowWindow(SW_SHOW);
    }

    if(nullptr != m_spPrepareChartPADlg)
    {
        if(!m_spPrepareChartPADlg->IsUserClosed())
        {
            if(!m_spPrepareChartPADlg->IsWindowVisible())
                m_spPrepareChartPADlg->ShowWindow(SW_SHOW);
        }
    }

    if(nullptr != m_spMultimodalResultDlg)
    {
        if(!m_spMultimodalResultDlg->IsUserClosed())
        {
            if(!m_spMultimodalResultDlg->IsWindowVisible())
                m_spMultimodalResultDlg->ShowWindow(SW_SHOW);
        }
        m_spMultimodalResultDlg->ActivatedFrameWndNotifyProcess();
    }

	if(nullptr != m_spIntermodalResultDlg)
	{
		if(!m_spIntermodalResultDlg->IsUserClosed())
		{
			if(!m_spIntermodalResultDlg->IsWindowVisible())
				m_spIntermodalResultDlg->ShowWindow(SW_SHOW);
		}
		m_spIntermodalResultDlg->ActivatedFrameWndNotifyProcess();
	}
        
    if (nullptr != m_spTransitLineThemesDlg)
    {
        if(!m_spTransitLineThemesDlg->IsUserClosed())
        {
            if(!m_spTransitLineThemesDlg->IsWindowVisible())
                m_spTransitLineThemesDlg->ShowWindow(SW_SHOW);
        }
        m_spTransitLineThemesDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spMapFindDlg)
    {
        if(!m_spMapFindDlg->IsUserClosed())
        {
            if(!m_spMapFindDlg->IsWindowVisible())
			{
				m_spMapFindDlg->Initialize();
                m_spMapFindDlg->ShowWindow(SW_SHOW);
			}
        }
    }

    if (nullptr != m_spMTransitPathEditDlg)
    {
        if(!m_spMTransitPathEditDlg->IsUserClosed())
        {
            if(!m_spMTransitPathEditDlg->IsWindowVisible())
                m_spMTransitPathEditDlg->ShowWindow(SW_SHOW);
        }
        m_spMTransitPathEditDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spLinkViaTransitSearchDlg)
    {
        if(!m_spLinkViaTransitSearchDlg->IsUserClosed())
        {
            if(!m_spLinkViaTransitSearchDlg->IsWindowVisible())
                m_spLinkViaTransitSearchDlg->ShowWindow(SW_SHOW);
        }
        m_spLinkViaTransitSearchDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spNodeViaTransitSearchDlg)
    {
        if(!m_spNodeViaTransitSearchDlg->IsUserClosed())
        {
            if(!m_spNodeViaTransitSearchDlg->IsWindowVisible())
                m_spNodeViaTransitSearchDlg->ShowWindow(SW_SHOW);
        }
        m_spNodeViaTransitSearchDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spTransitInfluenceAreaDlg)
    {
        if(!m_spTransitInfluenceAreaDlg->IsUserClosed())
        {
            if(!m_spTransitInfluenceAreaDlg->IsWindowVisible())
                m_spTransitInfluenceAreaDlg->ShowWindow(SW_SHOW);
        }
        m_spTransitInfluenceAreaDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spNetworkInspectionDlg)
    {
        if(!m_spNetworkInspectionDlg->IsUserClosed())
        {
            if(!m_spNetworkInspectionDlg->IsWindowVisible())
                m_spNetworkInspectionDlg->ShowWindow(SW_SHOW);
        }
    }

    if (nullptr != m_spODMatrixDlg)
    {
        if (!m_spODMatrixDlg->IsUserClosed())
        {
            if (!m_spODMatrixDlg->IsWindowVisible())
                m_spODMatrixDlg->ShowWindow(SW_SHOW);
        }
    }

    if (nullptr != m_spInterTransitPathEditDlg)
    {
        if(!m_spInterTransitPathEditDlg->IsUserClosed())
        {
            if(!m_spInterTransitPathEditDlg->IsWindowVisible())
                m_spInterTransitPathEditDlg->ShowWindow(SW_SHOW);
        }
        m_spInterTransitPathEditDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spTerminalScheduleDesireDlg)
    {
        if (!m_spTerminalScheduleDesireDlg->IsUserClosed())
        {
            if (!m_spTerminalScheduleDesireDlg->IsWindowVisible())
                m_spTerminalScheduleDesireDlg->ShowWindow(SW_SHOW);
        }
        m_spTerminalScheduleDesireDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spTerminalScheduleEditDlg)
    {
        if(!m_spTerminalScheduleEditDlg->IsUserClosed())
        {
            if(!m_spTerminalScheduleEditDlg->IsWindowVisible())
                m_spTerminalScheduleEditDlg->ShowWindow(SW_SHOW);
        }
        m_spTerminalScheduleEditDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spGlobalTerminalSettingDlg)
    {
        if(!m_spGlobalTerminalSettingDlg->IsUserClosed())
        {
            if(!m_spGlobalTerminalSettingDlg->IsWindowVisible())
                m_spGlobalTerminalSettingDlg->ShowWindow(SW_SHOW);
        }
        m_spGlobalTerminalSettingDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spAccessStopNodeEditorDlg)
    {
        if(!m_spAccessStopNodeEditorDlg->IsUserClosed())
        {
            if(!m_spAccessStopNodeEditorDlg->IsWindowVisible())
                m_spAccessStopNodeEditorDlg->ShowWindow(SW_SHOW);
        }
        m_spAccessStopNodeEditorDlg->ActivatedFrameWndNotifyProcess();
    }
    
    if (nullptr != m_spIntersectionTopologyAnalysisDlg)
    {
        if(!m_spIntersectionTopologyAnalysisDlg->IsUserClosed())
        {
            if(!m_spIntersectionTopologyAnalysisDlg->IsWindowVisible())
                m_spIntersectionTopologyAnalysisDlg->ShowWindow(SW_SHOW);
        }
        m_spIntersectionTopologyAnalysisDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spZoneODGroupGenerationDlg)
    {
        if(!m_spZoneODGroupGenerationDlg->IsUserClosed())
        {
            if(!m_spZoneODGroupGenerationDlg->IsWindowVisible())
                m_spZoneODGroupGenerationDlg->ShowWindow(SW_SHOW);
        }
        m_spZoneODGroupGenerationDlg->ActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spTurnVolumeViewDlg)
    {
        if(!m_spTurnVolumeViewDlg->IsUserClosed())
        {
            if(!m_spTurnVolumeViewDlg->IsWindowVisible())
                m_spTurnVolumeViewDlg->ShowWindow(SW_SHOW);
        }
        m_spTurnVolumeViewDlg->ActivatedFrameWndNotifyProcess();
    }

	if (m_spRulerDlg != NULL)
	{
		if (!m_spRulerDlg->IsUserClosed())
		{
			if (!m_spRulerDlg->IsWindowVisible())
				m_spRulerDlg->ShowWindow(SW_SHOW);
		}
	}

	if (nullptr != m_spPathVolumeAreaTransitDlg)
	{
		if (!m_spPathVolumeAreaTransitDlg->IsUserClosed())
		{
			if (!m_spPathVolumeAreaTransitDlg->IsWindowVisible())
				m_spPathVolumeAreaTransitDlg->ShowWindow(SW_SHOW);
		}
		m_spPathVolumeAreaTransitDlg->ActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spPathVolumeAreaInterModeDlg)
	{
		if (!m_spPathVolumeAreaInterModeDlg->IsUserClosed())
		{
			if (!m_spPathVolumeAreaInterModeDlg->IsWindowVisible())
				m_spPathVolumeAreaInterModeDlg->ShowWindow(SW_SHOW);
		}
		m_spPathVolumeAreaInterModeDlg->ActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spTerminalAEModeViewerDlg)
	{
		if (!m_spTerminalAEModeViewerDlg->IsUserClosed())
		{
			if (!m_spTerminalAEModeViewerDlg->IsWindowVisible())
				m_spTerminalAEModeViewerDlg->ShowWindow(SW_SHOW);
		}
		m_spTerminalAEModeViewerDlg->ActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spPathVolumeAreaInterDlg)
	{
		if (!m_spPathVolumeAreaInterDlg->IsUserClosed())
		{
			if (!m_spPathVolumeAreaInterDlg->IsWindowVisible())
				m_spPathVolumeAreaInterDlg->ShowWindow(SW_SHOW);
		}
		m_spPathVolumeAreaInterDlg->ActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spPathVolumeTerminalAccEgrDlg)
	{
		if (!m_spPathVolumeTerminalAccEgrDlg->IsUserClosed())
		{
			if (!m_spPathVolumeTerminalAccEgrDlg->IsWindowVisible())
				m_spPathVolumeTerminalAccEgrDlg->ShowWindow(SW_SHOW);
		}
		m_spPathVolumeTerminalAccEgrDlg->ActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spSubNetworkDlg)
	{
		if (!m_spSubNetworkDlg->IsUserClosed())
		{
			if (!m_spSubNetworkDlg->IsWindowVisible())
				m_spSubNetworkDlg->ShowWindow(SW_SHOW);
		}
	}

	if (nullptr != m_spStationODCalculatorDlg)
	{
		if (!m_spStationODCalculatorDlg->IsUserClosed())
		{
			if (!m_spStationODCalculatorDlg->IsWindowVisible())
				m_spStationODCalculatorDlg->ShowWindow(SW_SHOW);
		}
		m_spStationODCalculatorDlg->ActivatedFrameWndNotifyProcess();
	}

    if (nullptr != m_spDlgNetworkEdior)
    {
        if (!m_spDlgNetworkEdior->IsUserClosed())
        {
            if (!m_spDlgNetworkEdior->IsWindowVisible())
                m_spDlgNetworkEdior->ShowWindow(SW_SHOW);
        }
        m_spDlgNetworkEdior->ActivatedFrameWndNotifyProcess();
    }
}

void KMapView::DeActivatedFrameWndNotifyProcess()
{
    if (m_spScreenDrawEdit != NULL)
    {
        m_spScreenDrawEdit->DeActivatedFrameWndNotifyProcess();
    }

    if (m_spShortestPathHighwayDlg != NULL)
    {
        if (!m_spShortestPathHighwayDlg->IsUserClosed())
        {
            if (m_spShortestPathHighwayDlg->IsWindowVisible())
                m_spShortestPathHighwayDlg->ShowWindow(SW_HIDE);
        }
        m_spShortestPathHighwayDlg->DeActivatedFrameWndNotifyProcess();
    }

    if (m_spShortestPathNTransitDlg != NULL)
    {
        if (!m_spShortestPathNTransitDlg->IsUserClosed())
        {
            if (m_spShortestPathNTransitDlg->IsWindowVisible())
                m_spShortestPathNTransitDlg->ShowWindow(SW_HIDE);
        }
        m_spShortestPathNTransitDlg->DeActivatedFrameWndNotifyProcess();
    }

    if (m_spPathVolumeDlg != NULL)
    {
        if (!m_spPathVolumeDlg->IsUserClosed())
        {
            if (m_spPathVolumeDlg->IsWindowVisible())
                m_spPathVolumeDlg->ShowWindow(SW_HIDE);
        }
        m_spPathVolumeDlg->DeActivatedFrameWndNotifyProcess();
    }

    if (m_spPathVolumeAreaDlg != NULL)
    {
        if (!m_spPathVolumeAreaDlg->IsUserClosed())
        {
            if (m_spPathVolumeAreaDlg->IsWindowVisible())
                m_spPathVolumeAreaDlg->ShowWindow(SW_HIDE);
        }
        m_spPathVolumeAreaDlg->DeActivatedFrameWndNotifyProcess();
    }

    if (m_spLinkAnalysisDlg != NULL)
    {
        if (!m_spLinkAnalysisDlg->IsUserClosed())
        {
            if (m_spLinkAnalysisDlg->IsWindowVisible())
                m_spLinkAnalysisDlg->ShowWindow(SW_HIDE);
        }		
    }	

    if (m_spZoningDivisionDlg != NULL)
    {
        if (!m_spZoningDivisionDlg->IsUserClosed())
        {
            if (m_spZoningDivisionDlg->IsWindowVisible())
                m_spZoningDivisionDlg->ShowWindow(SW_HIDE);
        }
        m_spZoningDivisionDlg->DeActivatedFrameWndNotifyProcess();
    }	

    if (m_spZoningIntegrationDlg != NULL)
    {
        if (!m_spZoningIntegrationDlg->IsUserClosed())
        {
            if (m_spZoningIntegrationDlg->IsWindowVisible())
                m_spZoningIntegrationDlg->ShowWindow(SW_HIDE);
        }		
        m_spZoningIntegrationDlg->DeActivatedFrameWndNotifyProcess();
    }	

    if(m_spSCManageDlg != NULL)
    {
        if (!m_spSCManageDlg->IsUserClosed())
        {
            if (m_spSCManageDlg->IsWindowVisible())
                m_spSCManageDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spInspectionNodeDlg != NULL)
    {
        if (!m_spInspectionNodeDlg->IsUserClosed())
        {
            if (m_spInspectionNodeDlg->IsWindowVisible())
                m_spInspectionNodeDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spInspectionLinkDlg != NULL)
    {
        if (!m_spInspectionLinkDlg->IsUserClosed())
        {
            if (m_spInspectionLinkDlg->IsWindowVisible())
                m_spInspectionLinkDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spInspectionZoneDlg != NULL)
    {
        if (!m_spInspectionZoneDlg->IsUserClosed())
        {
            if (m_spInspectionZoneDlg->IsWindowVisible())
                m_spInspectionZoneDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spInspectionTurnDlg != NULL)
    {
        if (!m_spInspectionTurnDlg->IsUserClosed())
        {
            if (m_spInspectionTurnDlg->IsWindowVisible())
                m_spInspectionTurnDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spInspectionTransitDlg != NULL)
    {
        if (!m_spInspectionTransitDlg->IsUserClosed())
        {
            if (m_spInspectionTransitDlg->IsWindowVisible())
                m_spInspectionTransitDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spSymbologyThemeNodeDlg != NULL)
    {
        if (!m_spSymbologyThemeNodeDlg->IsUserClosed())
        {
            if (m_spSymbologyThemeNodeDlg->IsWindowVisible())
                m_spSymbologyThemeNodeDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spSymbologyThemeLinkDlg != NULL)
    {
        if (!m_spSymbologyThemeLinkDlg->IsUserClosed())
        {
            if (m_spSymbologyThemeLinkDlg->IsWindowVisible())
                m_spSymbologyThemeLinkDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spSymbologyThemeZoneDlg != NULL)
    {
        if (!m_spSymbologyThemeZoneDlg->IsUserClosed())
        {
            if (m_spSymbologyThemeZoneDlg->IsWindowVisible())
                m_spSymbologyThemeZoneDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(m_spDesireLineDlg != NULL)
    {
        if (!m_spDesireLineDlg->IsUserClosed())
        {
            if (m_spDesireLineDlg->IsWindowVisible())
                m_spDesireLineDlg->ShowWindow(SW_HIDE);
        }	
    }

    if(nullptr != m_spPrepareChartODDlg)
    {
        if(!m_spPrepareChartODDlg->IsUserClosed())
        {
            if(m_spPrepareChartODDlg->IsWindowVisible())
                m_spPrepareChartODDlg->ShowWindow(SW_HIDE);
        }
    }


    if(nullptr != m_spPrepareChartNetworkDlg)
    {
        if(!m_spPrepareChartNetworkDlg->IsUserClosed())
        {
            if(m_spPrepareChartNetworkDlg->IsWindowVisible())
                m_spPrepareChartNetworkDlg->ShowWindow(SW_HIDE);
        }
    }

    if(nullptr != m_spPrepareChartPADlg)
    {
        if(!m_spPrepareChartPADlg->IsUserClosed())
        {
            if(m_spPrepareChartPADlg->IsWindowVisible())
                m_spPrepareChartPADlg->ShowWindow(SW_HIDE);
        }
    }

    if(nullptr != m_spMultimodalResultDlg)
    {
        if(!m_spMultimodalResultDlg->IsUserClosed())
        {
            if(m_spMultimodalResultDlg->IsWindowVisible())
                m_spMultimodalResultDlg->ShowWindow(SW_HIDE);
        }
        m_spMultimodalResultDlg->DeActivatedFrameWndNotifyProcess();
    }

	if(nullptr != m_spIntermodalResultDlg)
	{
		if(!m_spIntermodalResultDlg->IsUserClosed())
		{
			if(m_spIntermodalResultDlg->IsWindowVisible())
				m_spIntermodalResultDlg->ShowWindow(SW_HIDE);
		}
		m_spIntermodalResultDlg->DeActivatedFrameWndNotifyProcess();
	}
	
    if(nullptr != m_spTransitLineThemesDlg)
    {
        if(!m_spTransitLineThemesDlg->IsUserClosed())
        {
            if(m_spTransitLineThemesDlg->IsWindowVisible())
                m_spTransitLineThemesDlg->ShowWindow(SW_HIDE);
        }
        m_spTransitLineThemesDlg->DeActivatedFrameWndNotifyProcess();
    }

    if(nullptr != m_spMapFindDlg)
    {
        if(!m_spMapFindDlg->IsUserClosed())
        {
            if(m_spMapFindDlg->IsWindowVisible())
                m_spMapFindDlg->ShowWindow(SW_HIDE);
        }
    }

    if(nullptr != m_spMTransitPathEditDlg)
    {
        if(!m_spMTransitPathEditDlg->IsUserClosed())
        {
            if(m_spMTransitPathEditDlg->IsWindowVisible())
                m_spMTransitPathEditDlg->ShowWindow(SW_HIDE);
        }
        m_spMTransitPathEditDlg->DeActivatedFrameWndNotifyProcess();
    }

    if(nullptr != m_spLinkViaTransitSearchDlg)
    {
        if(!m_spLinkViaTransitSearchDlg->IsUserClosed())
        {
            if(m_spLinkViaTransitSearchDlg->IsWindowVisible())
                m_spLinkViaTransitSearchDlg->ShowWindow(SW_HIDE);
        }
        m_spLinkViaTransitSearchDlg->DeActivatedFrameWndNotifyProcess();
    }

    if(nullptr != m_spNodeViaTransitSearchDlg)
    {
        if(!m_spNodeViaTransitSearchDlg->IsUserClosed())
        {
            if(m_spNodeViaTransitSearchDlg->IsWindowVisible())
                m_spNodeViaTransitSearchDlg->ShowWindow(SW_HIDE);
        }
        m_spNodeViaTransitSearchDlg->DeActivatedFrameWndNotifyProcess();
    }

    if(nullptr != m_spTransitInfluenceAreaDlg)
    {
        if(!m_spTransitInfluenceAreaDlg->IsUserClosed())
        {
            if(m_spTransitInfluenceAreaDlg->IsWindowVisible())
                m_spTransitInfluenceAreaDlg->ShowWindow(SW_HIDE);
        }
        m_spTransitInfluenceAreaDlg->DeActivatedFrameWndNotifyProcess();
    }

    if(nullptr != m_spNetworkInspectionDlg)
    {
        if(!m_spNetworkInspectionDlg->IsUserClosed())
        {
            if(m_spNetworkInspectionDlg->IsWindowVisible())
                m_spNetworkInspectionDlg->ShowWindow(SW_HIDE);
        }
    }

    if(nullptr != m_spODMatrixDlg)
    {
        if(!m_spODMatrixDlg->IsUserClosed())
        {
            if(m_spODMatrixDlg->IsWindowVisible())
                m_spODMatrixDlg->ShowWindow(SW_HIDE);
        }
    }

    if(nullptr != m_spInterTransitPathEditDlg)
    {
        if(!m_spInterTransitPathEditDlg->IsUserClosed())
        {
            if(m_spInterTransitPathEditDlg->IsWindowVisible())
                m_spInterTransitPathEditDlg->ShowWindow(SW_HIDE);
        }
        m_spInterTransitPathEditDlg->DeActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spTerminalScheduleDesireDlg)
    {
        if (!m_spTerminalScheduleDesireDlg->IsUserClosed())
        {
            if (m_spTerminalScheduleDesireDlg->IsWindowVisible())
                m_spTerminalScheduleDesireDlg->ShowWindow(SW_HIDE);
        }
        m_spTerminalScheduleDesireDlg->DeActivatedFrameWndNotifyProcess();
    }

    if(nullptr != m_spTerminalScheduleEditDlg)
    {
        if(!m_spTerminalScheduleEditDlg->IsUserClosed())
        {
            if(m_spTerminalScheduleEditDlg->IsWindowVisible())
                m_spTerminalScheduleEditDlg->ShowWindow(SW_HIDE);
        }
        m_spTerminalScheduleEditDlg->DeActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spGlobalTerminalSettingDlg)
    {
        if(!m_spGlobalTerminalSettingDlg->IsUserClosed())
        {
            if(m_spGlobalTerminalSettingDlg->IsWindowVisible())
                m_spGlobalTerminalSettingDlg->ShowWindow(SW_HIDE);
        }
        m_spGlobalTerminalSettingDlg->DeActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spAccessStopNodeEditorDlg)
    {
        if(!m_spAccessStopNodeEditorDlg->IsUserClosed())
        {
            if(m_spAccessStopNodeEditorDlg->IsWindowVisible())
                m_spAccessStopNodeEditorDlg->ShowWindow(SW_HIDE);
        }
        m_spAccessStopNodeEditorDlg->DeActivatedFrameWndNotifyProcess();
    }
    
    if (nullptr != m_spIntersectionTopologyAnalysisDlg)
    {
        if(!m_spIntersectionTopologyAnalysisDlg->IsUserClosed())
        {
            if(m_spIntersectionTopologyAnalysisDlg->IsWindowVisible())
                m_spIntersectionTopologyAnalysisDlg->ShowWindow(SW_HIDE);
        }
        m_spIntersectionTopologyAnalysisDlg->DeActivatedFrameWndNotifyProcess();
    }

    if (nullptr != m_spZoneODGroupGenerationDlg)
    {
        if(!m_spZoneODGroupGenerationDlg->IsUserClosed())
        {
            if(m_spZoneODGroupGenerationDlg->IsWindowVisible())
                m_spZoneODGroupGenerationDlg->ShowWindow(SW_HIDE);
        }
        m_spZoneODGroupGenerationDlg->DeActivatedFrameWndNotifyProcess();
    }



    if (nullptr != m_spTurnVolumeViewDlg)
    {
        if(!m_spTurnVolumeViewDlg->IsUserClosed())
        {
            if(m_spTurnVolumeViewDlg->IsWindowVisible())
                m_spTurnVolumeViewDlg->ShowWindow(SW_HIDE);
        }
        m_spTurnVolumeViewDlg->DeActivatedFrameWndNotifyProcess();
    }

	if (m_spRulerDlg != NULL)
	{
		if (!m_spRulerDlg->IsUserClosed())
		{
			if (m_spRulerDlg->IsWindowVisible())
				m_spRulerDlg->ShowWindow(SW_HIDE);
		}		
	}

	if (nullptr != m_spPathVolumeAreaTransitDlg)
	{
		if (!m_spPathVolumeAreaTransitDlg->IsUserClosed())
		{
			if (m_spPathVolumeAreaTransitDlg->IsWindowVisible())
				m_spPathVolumeAreaTransitDlg->ShowWindow(SW_HIDE);
		}
		m_spPathVolumeAreaTransitDlg->DeActivatedFrameWndNotifyProcess();
	}


	if (nullptr != m_spPathVolumeAreaInterModeDlg)
	{
		if (!m_spPathVolumeAreaInterModeDlg->IsUserClosed())
		{
			if (m_spPathVolumeAreaInterModeDlg->IsWindowVisible())
				m_spPathVolumeAreaInterModeDlg->ShowWindow(SW_HIDE);
		}
		m_spPathVolumeAreaInterModeDlg->DeActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spTerminalAEModeViewerDlg)
	{
		if (!m_spTerminalAEModeViewerDlg->IsUserClosed())
		{
			if (m_spTerminalAEModeViewerDlg->IsWindowVisible())
				m_spTerminalAEModeViewerDlg->ShowWindow(SW_HIDE);
		}
		m_spTerminalAEModeViewerDlg->DeActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spPathVolumeAreaInterDlg)
	{
		if (!m_spPathVolumeAreaInterDlg->IsUserClosed())
		{
			if (m_spPathVolumeAreaInterDlg->IsWindowVisible())
				m_spPathVolumeAreaInterDlg->ShowWindow(SW_HIDE);
		}
		m_spPathVolumeAreaInterDlg->DeActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spPathVolumeTerminalAccEgrDlg)
	{
		if (!m_spPathVolumeTerminalAccEgrDlg->IsUserClosed())
		{
			if (m_spPathVolumeTerminalAccEgrDlg->IsWindowVisible())
				m_spPathVolumeTerminalAccEgrDlg->ShowWindow(SW_HIDE);
		}
		m_spPathVolumeTerminalAccEgrDlg->DeActivatedFrameWndNotifyProcess();
	}

	if (nullptr != m_spSubNetworkDlg)
	{
		if (!m_spSubNetworkDlg->IsUserClosed())
		{
			if (m_spSubNetworkDlg->IsWindowVisible())
				m_spSubNetworkDlg->ShowWindow(SW_HIDE);
		}
	}

	if (nullptr != m_spStationODCalculatorDlg)
	{
		if (!m_spStationODCalculatorDlg->IsUserClosed())
		{
			if (m_spStationODCalculatorDlg->IsWindowVisible())
				m_spStationODCalculatorDlg->ShowWindow(SW_HIDE);
		}

		m_spStationODCalculatorDlg->DeActivatedFrameWndNotifyProcess();
	}

    if (nullptr != m_spDlgNetworkEdior)
    {
        if (!m_spDlgNetworkEdior->IsUserClosed())
        {
            if (m_spDlgNetworkEdior->IsWindowVisible())
                m_spDlgNetworkEdior->ShowWindow(SW_HIDE);
        }

        m_spDlgNetworkEdior->DeActivatedFrameWndNotifyProcess();
    }
}