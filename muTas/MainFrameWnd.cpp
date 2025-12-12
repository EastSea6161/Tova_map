// MainFrm.cpp : implementation of the CMainFrame class
//

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

#include "IOTableController.h"

#include "KBulkDBase.h"
#include "ImChampFileExist.h"
#include "KInspectionNodeDlg.h"
#include "KInspectionLinkDlg.h"
#include "KInspectionZoneDlg.h"
#include "KInspectionTurnDlg.h"
#include "KInspectionTransitDlg.h"
#include "KPASumDlg.h"
#include "KOD2PASumDlg.h"
#include "KCalculateTrafficVolumeDlg.h"
#include "KInfluenceStatisticsDlg.h"

#include "GravityCalibrationDlg.h"

#include "CalculatePathFindInfoDlg.h"
#include "LinkAnalysisDlg.h"
#include "DefineSystemCodeDlg.h"
#include "DefineTransitCodeCostDlg.h"

#include "ResultViewAssignmentDlg.h"
#include "ResultViewDistributionDlg.h"
#include "ResultViewModeDlg.h"

#include "DBaseAssignment.h"
#include "PA2ODModelDlg.h"
#include "DevPlanMgrDlg.h"
#include "DevPlanModelDlg.h"

#include "NetworkOnOffDlg.h"

#include "DefineNames.h"
#include "ImportTableDlg.h"

#include "SetAccessibility.h"
#include "GenerationBusPathStep1Dlg.h"
#include "GenerationBusPathStep2Dlg.h"
#include "MultiModalMainDlg.h"

#include "TableManagerVectorDlg.h"
#include "TableManagerMatrixDlg.h"
#include "DBaseAssignment.h"
#include "DBaseCheckData.h"
#include "CalculateParameterDlg.h"
#include "DefineRegionMode.h"

#include "AccessTerminalGeneratorDlg.h"
#include "InterPathGenMain.h"
#include "InterModalMainDlg.h"

#include "ZoneGroupGeneration.h"
#include "ODGroupGeneration.h"
#include "InterPathGeneratorMainDlg.h"
#include "ODGroupGenerationDlg.h"

#include "TableImpEmmeSelectFileDlg.h"
#include "TableImpEmmeNetworkDlg.h"
#include "TableExpEmmeNetworkDlg.h"
#include "TableExpEmmeMatrixDlg.h"
#include "TableImpEmmeMatrixDlg.h"

#include "DefaultAccessStationSetting.h"
#include "DefaultPathGenerateConstraint.h"
#include "DefaultMultimodalPathGeneration.h"
#include "DefaultMultimodalModeChoicePara.h"
#include "DefaultTransitRouteChoicePara.h"
#include "DefaultHighwayAssignment.h"
#include "DefaultAccessTerminalSetting.h"
#include "DefInterUrbanModeChoiceParaDlg.h"
#include "DefaultInterPathGenerator.h"
#include "DefaultRegionalModeChoiceParaDlg.h"
#include "DefaultDistributionParaSettingDlg.h"

#include "DefaultLogisticParaDlg.h"

#include "DefaultGenerationParaMainDlg.h"
#include "DefaultMDefineVariableGroupDlg.h"
#include "DefaultInterRunOptionDlg.h"
#include "DefaultCo2EmissionsInterpolationParaDlg.h"
#include "CO2EmissionInterpolationDlg.h"
#include "DefaultCo2EmissionsEquationParaDlg.h"
#include "CO2EmissionEquationDlg.h"
#include "CO2EmissionEquation1Dlg.h"
#include "GenerationModelMainDlg.h"
#include "NetworkEditLog.h"
#include "NetworkInspectionDlg.h"

#include "ShapeExportDlg.h"

#include "DlgShapeDemarcation.h"
#include "DlgShapeNode.h"
#include "DlgShapeLink.h"

#include "TripODToVehODDlg.h"
#include "AccessTripODToVehODDlg.h"
#include "FODDiv.h"

//테스트용
#include "KTestImportMatchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/******************************************************************************
* CMainFrame                                                                  *
******************************************************************************/
IMPLEMENT_DYNAMIC(CMainFrameWnd, CMDIFrameWnd)
    BEGIN_MESSAGE_MAP(CMainFrameWnd, CMDIFrameWnd)
        ON_WM_CREATE()
        ON_XTP_CREATECONTROL()
        ON_WM_CLOSE()
        ON_WM_DESTROY()
        ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
        ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
        ///// start test code
        ON_COMMAND(ID_MAP_OPENMAP, &CMainFrameWnd::OnMapOpenmap)
        ON_COMMAND(ID_TABLE_OPENTABLE, &CMainFrameWnd::OnTableOpentable)
        ON_COMMAND(ID_TABLE_CLOSETABLE, &CMainFrameWnd::OnTableClosetable)
        ON_COMMAND(ID_MAP_CLOSEMAP, &CMainFrameWnd::OnMapClosemap)
        ON_COMMAND(ID_PROJECTVIEW_CREATETREE, &CMainFrameWnd::OnProjectviewCreatetree)
        ///// end test code
        ON_COMMAND(ID_DEVMENU_RENDERPROPERTY, &CMainFrameWnd::OnDevmenuRenderproperty)
        ON_COMMAND(ID_DEVMENU_CODEMANAGER, &CMainFrameWnd::OnDevmenuCodemanager)
        ON_COMMAND(ID_MODEL_GENERATION, &CMainFrameWnd::OnMenuModelGeneration)
        ON_COMMAND(ID_MENU_DISTRIBUTION, &CMainFrameWnd::OnMenuModelDistribution)
        /*ON_COMMAND(ID_MENU_DISTRIBUTION_FREIGHT, &CMainFrameWnd::OnMenuModelDistributionFreight)*/
        ON_COMMAND(ID_MODEL_MODE, &CMainFrameWnd::OnMenuModelMode)
        ON_COMMAND(ID_MODEL_ASSIGNMENT, &CMainFrameWnd::OnMenuModelAssignment)

        ON_COMMAND(ID_CODEMANAGER_USERCODEMANAGER, &CMainFrameWnd::OnDbmanagementCodemanagement)
        ON_COMMAND(ID_CODEMANAGER_SYSTEMCODEMANAGER, &CMainFrameWnd::OnDBManagementSystemCodeManagement)
        ON_UPDATE_COMMAND_UI(ID_DBMANAGEMENT_CODEMANAGEMENT, &CMainFrameWnd::OnUpdateDbmanagementCodemanagement)
        ON_COMMAND(ID_MULITIMODAL_SETOFACCESSIBILITY, &CMainFrameWnd::OnMultiModalSetAccessibility)
        ON_COMMAND(ID_MULITIMODAL_GENERATIONOFBUSPATH, &CMainFrameWnd::OnMultiModalGenerationBusPath)
        ON_COMMAND(ID_MULITIMODAL_MULTIMODALASSIGNMENT, &CMainFrameWnd::OnMultiModalModel)
        ON_UPDATE_COMMAND_UI(ID_MULITIMODAL_MULTIMODALASSIGNMENT, &CMainFrameWnd::OnUpdateMultiModalModel)
        ON_COMMAND(ID_NETWORK_TRANSITCOSTDEFINE, &CMainFrameWnd::OnTranstiCostManagement)
        
        ON_COMMAND(ID_DATAINSPECTION_SED, &CMainFrameWnd::OnDataInspectionSed)
        ON_COMMAND(ID_OD_OD2PASUM, &CMainFrameWnd::OnOdOD2PASum)
        //ON_COMMAND(ID_TOOLS_RATIO_INPUT, &CMainFrameWnd::OnToolsIRIORatioInput)
        ON_COMMAND(ID_TOOLS_CALCULATE_TRAFFIC_VOL, &CMainFrameWnd::OnToolsCalculateTrafficVol)
        //ON_COMMAND(ID_TOOLS_RATIOREGION, &CMainFrameWnd::OnToolsRatioRegion)
        ON_COMMAND(ID_TOOLS_GRAVITY, &CMainFrameWnd::OnGravityCalibration)

        ON_COMMAND(ID_RESULTVIEW_GENERATION,   &CMainFrameWnd::OnToolsPaSum)
        ON_COMMAND(ID_RESULTVIEW_DISTRIBUTION, &CMainFrameWnd::OnResultViewDistribution)
        ON_COMMAND(ID_RESULTVIEW_MODE,         &CMainFrameWnd::OnResultViewMode)
        ON_COMMAND(ID_RESULTVIEW_ASSIGNMENT,   &CMainFrameWnd::OnResultViewAssignment)
        ON_COMMAND(ID_LOGVIEW_ASSIGNMENT,      &CMainFrameWnd::OnLogViewAssignment)
        
        ON_COMMAND(ID_Influence_Off,          &CMainFrameWnd::OnInfluenceOff)
        ON_COMMAND(ID_Influence_Statistics,   &CMainFrameWnd::OnInfluenceStatistics)

        ON_COMMAND(ID_CalculatePahFindInfo,   &CMainFrameWnd::OnFCalculatePathFindInfo)
        ON_COMMAND(ID_CalculateTimeCostInfo,  &CMainFrameWnd::OnFCalculateTimeCostInfo)
        ON_COMMAND(ID_MENU_TIMEOD_GENERATION, &CMainFrameWnd::OnMenuTimeODGeneration)
        ON_COMMAND(ID_TOOLS_PATOOD,           &CMainFrameWnd::OnMenuPATOOD)
        ON_COMMAND(ID_DEVPLAN_MANAGER,        &CMainFrameWnd::OnMenuDevPlanManager)
        ON_COMMAND(ID_DEVPLAN_MODEL,          &CMainFrameWnd::OnMenuDevPlan)

        ON_COMMAND(ID_ARCGISTEST_STRINGCHECKER, &CMainFrameWnd::OnArcgistestStringchecker)
        ON_COMMAND(ID_WINDOWS_PROJECTEXPLORER, &CMainFrameWnd::OnProjectExplorerWindowShow)
        //ON_COMMAND(ID_WINDOWS_SEDEXPLORERWINDOW, &CMainFrameWnd::OnSedExplorerWindowShow)
        ON_COMMAND(ID_WINDOWS_NAVIEXPLORERWINDOW, &CMainFrameWnd::OnNaviExplorerWindowShow)

        ON_COMMAND(ID_WINDOWS_COMMANDWINDOW, &CMainFrameWnd::OnCommandWindowShow)

        ON_COMMAND(ID_NETWORKEDIT_NETWORKONOFF, &CMainFrameWnd::OnNetworkOnOff)

        ON_COMMAND(ID_NETWORKEDIT_IMPORTTABLE, &CMainFrameWnd::OnImportTable)
        ON_UPDATE_COMMAND_UI(ID_NETWORKEDIT_IMPORTTABLE, &CMainFrameWnd::OnUpdateImportTable)
        ON_COMMAND(ID_NETWORKEDIT_EXPORTTABLE, &CMainFrameWnd::OnExportTable)
        ON_UPDATE_COMMAND_UI(ID_NETWORKEDIT_EXPORTTABLE, &CMainFrameWnd::OnUpdateExportTable)

        ON_COMMAND(ID_NETWORKEDIT_EMME_NETWORK_IMPORT, &CMainFrameWnd::OnEmmeNetworkImportTable)
        ON_COMMAND(ID_NETWORKEDIT_EMME_NETWORK_EXPORT, &CMainFrameWnd::OnEmmeNetworkExportTable)
        ON_UPDATE_COMMAND_UI(ID_NETWORKEDIT_EMME_NETWORK_EXPORT, &CMainFrameWnd::OnUpdateEmmeNetworkExportTable)

        ON_COMMAND(ID_NETWORKEDIT_EMME_MATRIX_IMPORT,  &CMainFrameWnd::OnEmmeMatrixImportTable)
        ON_COMMAND(ID_NETWORKEDIT_EMME_MATRIX_EXPORT,  &CMainFrameWnd::OnEmmeMatrixExportTable)
        ON_UPDATE_COMMAND_UI(ID_NETWORKEDIT_EMME_MATRIX_EXPORT, &CMainFrameWnd::OnUpdateEmmeMatrixExportTable)

        ON_COMMAND(ID_TABLEMANAGER_VECTOROPERATOR, &CMainFrameWnd::OnTableManagerVector)
        ON_COMMAND(ID_TABLEMANAGER_MATRIXOPEARTOR, &CMainFrameWnd::OnTableManagerMatrix)

        ON_COMMAND(ID_TOOLS_ODPARAMETER, &CMainFrameWnd::OnToolsODParameter)
        ON_COMMAND(ID_TRANSITPATH_REGIONALMODEDEFINE, &CMainFrameWnd::OnDefineRegionalMode)

        ON_COMMAND(ID_INTERMODALPATH_ACCESSTERMINALGENERATOR, &CMainFrameWnd::OnInterAccessTerminalGenerator)

        ON_COMMAND(ID_INTERMODALPATH_INTERMODALPATHGENERATOR, &CMainFrameWnd::OnInterPathGenerator)
        ON_COMMAND(ID_PASSENGER_INTERMODALASSIGNMENTMODELS, &CMainFrameWnd::OnInterModalMain)
        ON_COMMAND(ID_APPLICATION_ROADCONSTRUCTION, &CMainFrameWnd::OnApplicationRoadConstruction)
        ON_COMMAND(ID_APPLICATION_EVENT, &CMainFrameWnd::OnApplicationEvent)
        ON_COMMAND(ID_APPLICATION_TRANSITCOMPLEXCONSTRUCTION, &CMainFrameWnd::OnApplicationTransitComplexConstruction)
        ON_COMMAND(ID_APPLICATION_SKIPANDSTOPANALYSIS, &CMainFrameWnd::OnApplicationSkipandStopAnalysis)
        ON_COMMAND(ID_APPLICATION_ROADIMPROVEMENT, &CMainFrameWnd::OnApplicationRoadimprovement)
        ON_COMMAND(ID_APPLICATION_CO2INTERPOLATION, &CMainFrameWnd::OnApplicationCO2Interpolation)
        ON_COMMAND(ID_APPLICATION_CO2EMISSION, &CMainFrameWnd::OnApplicationCO2Emission)
        ON_COMMAND(ID_APPLICATION_BUSONLYLAENANALYSIS, &CMainFrameWnd::OnApplicationBusOnlyLane)

        ON_COMMAND(ID_GROUPGENERATION_ZONEGROUPGENERATION, &CMainFrameWnd::OnZoneGroupGeneration)
        ON_COMMAND(ID_GROUPGENERATION_ODGROUPGENERATION, &CMainFrameWnd::OnOdGroupGeneration)
        ON_COMMAND(ID_ODGROUPGENERATION_PART2,           &CMainFrameWnd::OnOdGroupGenerationPart2)

        //Global Parameter Setting
        ON_COMMAND(ID_PARAMETERSETTING_ACCESSSTATION, &CMainFrameWnd::OnDefaultAccessStationSetting)
        ON_COMMAND(ID_PARAMETERSETTING_PATHGENERATECONSTRAINT, &CMainFrameWnd::OnDefaultPathGenerateConstraint)
        ON_COMMAND(ID_PARAMETERSETTING_MULIMODALPATHGENERATOR, &CMainFrameWnd::OnDefaultMultimodalPathGenerator)
        ON_COMMAND(ID_PARAMETERSETTING_MODECHOICEPARAMETER, &CMainFrameWnd::OnDefaultModeChoicePara)
        ON_COMMAND(ID_GLOBALPARAMETERSETTING_TRANSIT, &CMainFrameWnd::OnDefaultTransitRouteChoicepara)
        ON_COMMAND(ID_GLOBALPARAMETERSETTING_HIGHWAYASSIGNMENT, &CMainFrameWnd::OnDefaultHighwayAssignment)
        ON_COMMAND(ID_PARAMETER_ACCESSTERMINALSETTING, &CMainFrameWnd::OnDefaultAccessTerminalSetting)
        ON_COMMAND(ID_PARAMETER_INTERMODAL_MODECHOICE, &CMainFrameWnd::OnDefaultInterModeChoicePara)
        ON_COMMAND(ID_PARAMETER_INTERMODALPATHGENERATOR, &CMainFrameWnd::OnDefaultInterPathGeneratorPara)
        ON_COMMAND(ID_PARAMETER_REGIONAL_MODECHOICE, CMainFrameWnd::OnDefaultRegionalModeChoicePara)

        ON_COMMAND(ID_PARAMETER_Distribution, &CMainFrameWnd::OnDefaultDistributionPara)

        ON_COMMAND(ID_PARAMETER_FTIMECOST, &CMainFrameWnd::OnFDefaultCalTimeCostPara)
        ON_COMMAND(ID_PARAMETER_TRIPGENERATION, &CMainFrameWnd::OnDefaultGenerationPara)
        ON_COMMAND(ID_PARAMETER_INTERMODALRUNOPTIOIN, &CMainFrameWnd::OnDefaultInterRunOption)
        ON_COMMAND(ID_PARAMETER_MODECHOICE_DEFINE_VARIABLE, &CMainFrameWnd::OnDefaultModeChoiceModelDefineVariable)
        ON_COMMAND(ID_PARAMETER_CO2_INTERPOLATION, &CMainFrameWnd::OnDefaultCO2Interpolation)
        ON_COMMAND(ID_TOOLS_INTERPOLATION, &CMainFrameWnd::OnToolsEmissionsInterpolation)
        ON_COMMAND(ID_PARAMETER_CO2_EQUATION, &CMainFrameWnd::OnDefaultCO2Equation)
        ON_COMMAND(ID_TOOLS_EQUATION, &CMainFrameWnd::OnToolsEmissionsEquation)

        //ID_CODEMANAGER_USERCODEMANAGER

        //Model Proecess
        // View Model Result Log
        ON_COMMAND(ID_MODELSRESULTLOG_HIGHWAY, &CMainFrameWnd::OnLogHighwayModel)
        ON_UPDATE_COMMAND_UI(ID_MODELSRESULTLOG_HIGHWAY, &CMainFrameWnd::OnUpdateLogHighwayModel)
        ON_COMMAND(ID_MODELSRESULTLOG_MULTIMODAL, &CMainFrameWnd::OnLogMultiModalModel)
        ON_UPDATE_COMMAND_UI(ID_MODELSRESULTLOG_MULTIMODAL, &CMainFrameWnd::OnUpdateLogMultiModalModel)
        ON_COMMAND(ID_MODELSRESULTLOG_INTERMODAL, &CMainFrameWnd::OnLogInterModalModel)
        ON_UPDATE_COMMAND_UI(ID_MODELSRESULTLOG_INTERMODAL, &CMainFrameWnd::OnUpdateLogInterModalModel)

        ON_COMMAND(ID_NETWORKSHAPEEXPORT, &CMainFrameWnd::OnNetShapeExport)
        ON_UPDATE_COMMAND_UI(ID_NETWORKSHAPEEXPORT, &CMainFrameWnd::OnUpdateNetShapeExport)
        ON_COMMAND(ID_NETWORKSHAPEIMPORTNODE, &CMainFrameWnd::OnNetShapeNodeImport)
        ON_COMMAND(ID_NETWORKSHAPEIMPORTLINK, &CMainFrameWnd::OnNetShapeLinkImport)
        ON_COMMAND(ID_NETWORKSHAPEIMPORTZONE, &CMainFrameWnd::OnNetShapeZoneImport)

        //MapView
        ON_COMMAND(ID_TOOLS_SCREENLINE, &CMainFrameWnd::OnToolsScreenline)
        ON_COMMAND(ID_TOOLS_CORDONLINE, &CMainFrameWnd::OnToolsCordonline)
        ON_COMMAND(ID_SCREENCORDON_ANALYSIS, &CMainFrameWnd::OnToolsScreenCordonLineAnalysis)
        ON_UPDATE_COMMAND_UI(ID_TOOLS_SCREENLINE, &CMainFrameWnd::OnUpdateToolsScreenline)
        ON_UPDATE_COMMAND_UI(ID_TOOLS_CORDONLINE, &CMainFrameWnd::OnUpdateToolsCordonline)
        ON_UPDATE_COMMAND_UI(ID_INFLUENCE_CIRCLE, &CMainFrameWnd::OnUpdateInfluenceCircle)
        ON_UPDATE_COMMAND_UI(ID_INFLUENCE_ENVELOPE, &CMainFrameWnd::OnUpdateInfluenceEnvelope)
        ON_UPDATE_COMMAND_UI(ID_INFLUENCE_POLYGON, &CMainFrameWnd::OnUpdateInfluencePolygon)

        ON_COMMAND(ID_SYMBOLOGY_THEME_NODE,     &CMainFrameWnd::OnSymbologyThemeNode)
        ON_UPDATE_COMMAND_UI(ID_SYMBOLOGY_THEME_NODE, &CMainFrameWnd::OnUpdateSymbologyThemeNode)

        ON_COMMAND(ID_CLEAR_THEME_NODE,         &CMainFrameWnd::OnClearThemeNode)
        ON_COMMAND(ID_SYMBOLOGY_THEME_LINK,      &CMainFrameWnd::OnSymbologyThemeLink)
        ON_UPDATE_COMMAND_UI(ID_SYMBOLOGY_THEME_LINK, &CMainFrameWnd::OnUpdateSymbologyThemeLink)

        ON_COMMAND(ID_CLEAR_THEME_LINK,          &CMainFrameWnd::OnClearThemeLink)
        ON_COMMAND(ID_SYMBOLOGY_THEME_ZONE,      &CMainFrameWnd::OnSymbologyThemeZone)
        ON_COMMAND(ID_CLEAR_THEME_ZONE,          &CMainFrameWnd::OnClearThemeZone)
        ON_COMMAND(ID_ZONE_DESIRELINE_DESIRELINE,&CMainFrameWnd::OnDesireLineZone)
        ON_COMMAND(ID_ZONE_DESIRELINE_CLEAR,     &CMainFrameWnd::OnDesireLineClearZone)

        ON_COMMAND(ID_SHORTESTPATH_HIGHWAY,      &CMainFrameWnd::OnShortestpathHighway)
        ON_COMMAND(ID_SHORTESTPATH_TRANSIT,		 &CMainFrameWnd::OnShortestpathTransit)
        ON_UPDATE_COMMAND_UI(ID_SHORTESTPATH_TRANSIT,		 &CMainFrameWnd::OnUpdateShortestpathTransit)

        ON_COMMAND(ID_PATHANALYSIS_ZONETOZONE,   &CMainFrameWnd::OnPathAnalysis)
        ON_COMMAND(ID_PATHANALYSIS_AREATOAREA,   &CMainFrameWnd::OnPathVolumeHighwayAreaToArea)
        ON_COMMAND(ID_PATHANALYSIS_TRANSIT_AREATOAREA,   &CMainFrameWnd::OnPathVolumeTransitAreaToArea)
        ON_COMMAND(ID_PATHANALYSIS_INTERMODE_AREATOAREA, &CMainFrameWnd::OnPathVolumeInterModeAreaToArea)
        ON_COMMAND(ID_PATHANALYSIS_INTER_AREATOAREA,     &CMainFrameWnd::OnPathVolumeInterAreaToArea)
        ON_UPDATE_COMMAND_UI(ID_PATHANALYSIS_INTER_AREATOAREA,		 &CMainFrameWnd::OnUpdatePathVolumeInterAreaToArea)
        ON_COMMAND(ID_PATHANALYSIS_LINK,         &CMainFrameWnd::OnLinkAnalysis)

        ON_COMMAND(ID_NETWORK_NODE,              &CMainFrameWnd::OnInspectionNetworkNode)
        ON_COMMAND(ID_NETWORK_LINK,              &CMainFrameWnd::OnInspectionNetworkLink)
        ON_COMMAND(ID_NETWORK_ZONE,              &CMainFrameWnd::OnInspectionNetworkZone)
        ON_COMMAND(ID_NETWORK_TURN,              &CMainFrameWnd::OnInspectionNetworkTurn)
        ON_COMMAND(ID_DATAINSPECTION_TRANSIT,    &CMainFrameWnd::OnDataInspectionTransit)

        ON_COMMAND(ID_NETWORKINSPECTION_RUN,     &CMainFrameWnd::OnNetworkInspectionRun)
        ON_UPDATE_COMMAND_UI(ID_NETWORKINSPECTION_RUN,		     &CMainFrameWnd::OnUpdateNetworkInspectionRun)
        ON_COMMAND(ID_NETWORKINSPECTION_HIGHWAY_ERRORLIST,       &CMainFrameWnd::OnNetworkInspectionHighwayError)
        ON_COMMAND(ID_NETWORKINSPECTION_HIGHWAY_WARNINGLIST,     &CMainFrameWnd::OnNetworkInspectionHighwayWarning)
        ON_COMMAND(ID_NETWORKINSPECTION_TRANSIT_ERRORLIST,       &CMainFrameWnd::OnNetworkInspectionTransitError)
        ON_COMMAND(ID_NETWORKINSPECTION_TRANSIT_WARNINGLIST,     &CMainFrameWnd::OnNetworkInspectionTransitWarning)
        ON_COMMAND(ID_NETWORKINSPECTION_VIEWLOG,                 &CMainFrameWnd::OnNetworkInspectionViewLog)
        ON_UPDATE_COMMAND_UI(ID_NETWORKINSPECTION_HIGHWAY_ERRORLIST,       &CMainFrameWnd::OnUpdateNetworkInspectionHighwayError)
        ON_UPDATE_COMMAND_UI(ID_NETWORKINSPECTION_HIGHWAY_WARNINGLIST,     &CMainFrameWnd::OnUpdateNetworkInspectionHighwayWarning)
        ON_UPDATE_COMMAND_UI(ID_NETWORKINSPECTION_TRANSIT_ERRORLIST,       &CMainFrameWnd::OnUpdateNetworkInspectionTransitError)
        ON_UPDATE_COMMAND_UI(ID_NETWORKINSPECTION_TRANSIT_WARNINGLIST,     &CMainFrameWnd::OnUpdateNetworkInspectionTransitWarning)
        ON_UPDATE_COMMAND_UI(ID_NETWORKINSPECTION_VIEWLOG,                 &CMainFrameWnd::OnUpdateNetworkInspectionViewLog)

        ON_COMMAND(ID_SCREENCORDON_MANAGE, &CMainFrameWnd::OnScreenCordonManage)
        ON_UPDATE_COMMAND_UI(ID_SCREENCORDON_MANAGE, &CMainFrameWnd::OnUpdateScreenCordonManage)
        ON_COMMAND(ID_ZONING_DIVISION, &CMainFrameWnd::OnZoningDivision)
        ON_COMMAND(ID_ZONING_INTEGRATION, &CMainFrameWnd::OnZoningIntegration)
        
        ON_COMMAND(ID_GRAPH_NETWORK_ZONE, &CMainFrameWnd::OnPrepareChartNetworkZone)
        ON_COMMAND(ID_GRAPH_NETWORK_NODE, &CMainFrameWnd::OnPrepareChartNetworkNode)
        ON_COMMAND(ID_GRAPH_NETWORK_LINK, &CMainFrameWnd::OnPrepareChartNetworkLink)
        
        ON_COMMAND(ID_GRAPH_PC,           &CMainFrameWnd::OnPrepareChartPC)	
        ON_UPDATE_COMMAND_UI(ID_GRAPH_PC, &CMainFrameWnd::OnUpdatePrepareChartPC)

        ON_COMMAND(ID_GRAPH_OD,           &CMainFrameWnd::OnPrepareChartOD)	
        ON_UPDATE_COMMAND_UI(ID_GRAPH_OD, &CMainFrameWnd::OnUpdatePrepareChartOD)


        ON_COMMAND(ID_MULITIMODAL_ACCESSIBILITYMODIFY, &CMainFrameWnd::OnAccessibilityModify)
        ON_COMMAND(ID_MULITIMODALASSIGNMENTMODEL_RESULTVIEW, &CMainFrameWnd::OnMultiModalAssignResult)
        ON_UPDATE_COMMAND_UI(ID_MULITIMODALASSIGNMENTMODEL_RESULTVIEW, &CMainFrameWnd::OnUpdateMultiModalAssignResult)

        ON_COMMAND(ID_MULITIMODAL_TRANSITPATHEDIT, &CMainFrameWnd::OnMultiModalTransitPathEdit)	

        ON_COMMAND(ID_PATHANALYSIS_INTER_ZONETOZONE, &CMainFrameWnd::OnInterModalAssignResult)
        ON_UPDATE_COMMAND_UI(ID_PATHANALYSIS_INTER_ZONETOZONE, &CMainFrameWnd::OnUpdateInterModalAssignResult)
        ON_COMMAND(ID_PATHANALYSIS_TERMINALACCESS, &CMainFrameWnd::OnPathVolumeTerminalAccessEgress)
        ON_UPDATE_COMMAND_UI(ID_PATHANALYSIS_TERMINALACCESS, &CMainFrameWnd::OnUpdatePathVolumeTerminalAccessEgress)

        ON_COMMAND(ID_NETWORKEDIT_NODE, &CMainFrameWnd::OnNetworkEditNode)
        ON_UPDATE_COMMAND_UI(ID_NETWORKEDIT_NODE, &CMainFrameWnd::OnUpdateNetworkeditNode)

        ON_COMMAND(ID_NETWORKEDIT_LINK, &CMainFrameWnd::OnNetworkEditLink)
        ON_COMMAND(ID_NETWORKEDIT_TURN, &CMainFrameWnd::OnNetworkEditTurn)
        ON_COMMAND(ID_NETWORKEDIT_TRANSIT, &CMainFrameWnd::OnNetworkEditTransit)

        ON_COMMAND(ID_INTERMODALPATH_TARGETREGIONSETTING,               &CMainFrameWnd::OnInterModalTargetRegionSetting)
        ON_COMMAND(ID_INTERMODALPATH_ACCESSTERMINALEDITOR,              &CMainFrameWnd::OnInterModalAccessTerminalEditor)
        ON_COMMAND(ID_INTERMODALPATH_TARGETREGIONGLOBALTERMINALSETTING, &CMainFrameWnd::OnInterModalGlobalTerminalSetting)
        ON_COMMAND(ID_INTERMODALPATH_PATHEDIT,                          &CMainFrameWnd::OnInterModalPathEdit)	
        ON_COMMAND(ID_INTERMODAL_TERMINAL_DESIRELINE,                   &CMainFrameWnd::OnInterModalTerminalDesireLine)
        ON_COMMAND(ID_TERMINAL_SCHEDULE_EDIT,                           &CMainFrameWnd::OnTerminalScheduleEdit)

        ON_COMMAND(ID_TOOLS_SUBNETWORK, &CMainFrameWnd::OnMenuToolsSubNetWork)
        ON_UPDATE_COMMAND_UI(ID_TOOLS_SUBNETWORK, &CMainFrameWnd::OnUpdateMenuToolsSubNetWork)

        ON_COMMAND(ID_NETWORKEDIT_EDITLOG, &CMainFrameWnd::OnNetworkeditEditlog)
        ON_UPDATE_COMMAND_UI(ID_NETWORKEDIT_EDITLOG, &CMainFrameWnd::OnUpdateNetworkeditEditlog)
        ON_COMMAND(ID_NOMBARA_TEST_HELP, &CMainFrameWnd::OnHelpNombara)
        ON_COMMAND(ID_NOMBARA_TEST_A, &CMainFrameWnd::OnNombaraTesterA)
        ON_COMMAND(ID_NOMBARA_TEST_B, &CMainFrameWnd::OnNombaraTesterB)

        ON_COMMAND(ID_MODELSPROCESS_MULTIMODAL, &CMainFrameWnd::OnModelsProcessmultiModal)
        ON_COMMAND(ID_MODELSPROCESS_INTERMODAL, &CMainFrameWnd::OnModelsProcessInterModal)

        ON_COMMAND(ID_GROUPGENERATION_ZONEOD, &CMainFrameWnd::OnZoneODGroupGeneration)
        ON_COMMAND(ID_TOOLS_VIEWTURNVOLUME, &CMainFrameWnd::OnViewTurnVolume)

        ON_COMMAND(ID_THEMES_TERMINAL, &CMainFrameWnd::OnThemesTerminalAEModeViewer)
        ON_UPDATE_COMMAND_UI(ID_THEMES_TERMINAL, &CMainFrameWnd::OnUpdateThemesTerminalAEModeViewer)

        ON_COMMAND(ID_TRANSIT_TRANSITLINE_THEMES, &CMainFrameWnd::OnTransitLineThemes)
        ON_UPDATE_COMMAND_UI(ID_TRANSIT_TRANSITLINE_THEMES, &CMainFrameWnd::OnUpdateTransitTransitlineThemes)

        ON_COMMAND(ID_TABLE_CHART_TRANSIT, &CMainFrameWnd::OnTransitChart)
        ON_UPDATE_COMMAND_UI(ID_TABLE_CHART_TRANSIT, &CMainFrameWnd::OnUpdateTransitChart)

        //★ Utilities
        ON_COMMAND(ID_EXE_ICBOUNDVOL_ANALYSIS,      &CMainFrameWnd::OnEXE_ICBOUNDVOL_ANALYSIS)
        ON_COMMAND(ID_EXE_ICBOUNDVOLBAT_ANALYSIS,   &CMainFrameWnd::OnEXE_ICBOUNDVOLBAT_ANALYSIS)
		ON_COMMAND(ID_EXE_TRIPOD_TO_VEHICLEOD, &CMainFrameWnd::OnEXE_TRIPOD_TO_VEHICLEOD)
		ON_COMMAND(ID_EXE_FREIGHTOD_DIVISION, &CMainFrameWnd::OnEXE_FREIGHTOD_DIVISION)


        ON_COMMAND(ID_PATH_MULTIMODAL   ,     &CMainFrameWnd::On_PATH_MULTIMODAL   )
        ON_COMMAND(ID_PATH_INTERMODAL   ,     &CMainFrameWnd::On_PATH_INTERMODAL   )
        ON_COMMAND(ID_BUS_MULTIMODAL    ,     &CMainFrameWnd::On_BUS_MULTIMODAL    )
        ON_COMMAND(ID_BUS_INTERMODAL    ,     &CMainFrameWnd::On_BUS_INTERMODAL    )
        ON_COMMAND(ID_STATION_MULTIMODAL,     &CMainFrameWnd::On_STATION_MULTIMODAL)
        ON_COMMAND(ID_STATION_INTERMODAL,     &CMainFrameWnd::On_STATION_INTERMODAL)
        ON_COMMAND(ID_URBAN_MULTIMODAL  ,     &CMainFrameWnd::On_URBAN_MULTIMODAL  )
        ON_COMMAND(ID_URBAN_INTERMODAL  ,     &CMainFrameWnd::On_URBAN_INTERMODAL  )

        ON_UPDATE_COMMAND_UI(ID_EXE_ICBOUNDVOL_ANALYSIS, &CMainFrameWnd::OnUpdateEXE_ICBOUNDVOL_ANALYSIS)
        ON_UPDATE_COMMAND_UI(ID_EXE_ICBOUNDVOLBAT_ANALYSIS, &CMainFrameWnd::OnUpdateEXE_ICBOUNDVOLBAT_ANALYSIS)
		ON_UPDATE_COMMAND_UI(ID_EXE_TRIPOD_TO_VEHICLEOD, &CMainFrameWnd::OnUpdateEXE_TRIPOD_TO_VEHICLEOD)
		ON_UPDATE_COMMAND_UI(ID_EXE_FREIGHTOD_DIVISION, &CMainFrameWnd::OnUpdateEXE_FREIGHTOD_DIVISION)
        ON_UPDATE_COMMAND_UI(ID_PATH_MULTIMODAL, &CMainFrameWnd::OnUpdate_PATH_MULTIMODAL)
        ON_UPDATE_COMMAND_UI(ID_PATH_INTERMODAL, &CMainFrameWnd::OnUpdate_PATH_INTERMODAL)
        ON_UPDATE_COMMAND_UI(ID_BUS_MULTIMODAL, &CMainFrameWnd::OnUpdate_BUS_MULTIMODAL)
        ON_UPDATE_COMMAND_UI(ID_BUS_INTERMODAL, &CMainFrameWnd::OnUpdate_BUS_INTERMODAL)
        ON_UPDATE_COMMAND_UI(ID_STATION_MULTIMODAL, &CMainFrameWnd::OnUpdate_STATION_MULTIMODAL)
        ON_UPDATE_COMMAND_UI(ID_STATION_INTERMODAL, &CMainFrameWnd::OnUpdate_STATION_INTERMODAL)
        ON_UPDATE_COMMAND_UI(ID_URBAN_MULTIMODAL, &CMainFrameWnd::OnUpdate_URBAN_MULTIMODAL)
        ON_UPDATE_COMMAND_UI(ID_URBAN_INTERMODAL, &CMainFrameWnd::OnUpdate_URBAN_INTERMODAL)
		ON_COMMAND(ID_EXE_ACCESSTRIPOD_TO_VEHICLEOD, &CMainFrameWnd::OnEXE_ACCESSTRIPOD_TO_VEHICLEOD)
		ON_UPDATE_COMMAND_UI(ID_EXE_ACCESSTRIPOD_TO_VEHICLEOD, &CMainFrameWnd::OnUpdateEXE_ACCESSTRIPOD_TO_VEHICLEOD)
	END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
	ID_WINDOW_CASCADE,
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};


/**************************************
* Constructors and Destructor         *
**************************************/
CMainFrameWnd::CMainFrameWnd()
{
    TxLogDebugStartMsg();

	TCHAR    szStylesPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		

	CString  csStylesPath(szStylesPath);

	int nIndex  = csStylesPath.ReverseFind(_T('\\'));
	if (nIndex > 0) 
    {
		csStylesPath = csStylesPath.Left(nIndex);
	}
	else 
    {
		csStylesPath.Empty();
	}       
    
    if (KmzSystem::IsPassengerSystem())
    {
        m_strStylesPath = csStylesPath + _T("\\Tova.cjstyles");
    }
    else
    {
		m_strStylesPath = csStylesPath + _T("\\Tova.cjstyles");
    }	

	XTPSkinManager()->SetApplyOptions(xtpSkinApplyFrame | xtpSkinApplyColors | xtpSkinApplyMetrics | xtpSkinApplyMenus);

    m_pPolicyTarget  = nullptr;
    m_nPolicyCommand = 0;
    TxLogDebugEndMsg();
}

CMainFrameWnd::~CMainFrameWnd()
{
    TxLogDebugVisitor();
}

int CMainFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    TxLogDebugStartMsg();

    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    /// Create status bar
    if (!m_xtpStatusBar.Create(this) ||
        !m_xtpStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      
    }
	
	m_xtpStatusBar.SetPaneText(1, _T("."));
	m_xtpStatusBar.SetPaneText(2, _T("."));
	m_xtpStatusBar.SetPaneText(3, _T("TOVA"));

    // Initialize the command bars
    if (!InitCommandBars())
        return -1;

    CXTPPaintManager::SetTheme( xtpThemeVisualStudio2008 );
    XTPSkinManager()->SetAutoApplyNewThreads( FALSE );
    XTPSkinManager()->LoadSkin( m_strStylesPath, _T("NormalBlack.ini") );

    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars == NULL)
    {
        TRACE0("Failed to create command bars object.\n");
        return -1;      
    }
    pCommandBars->SetTheme( xtpThemeVisualStudio2008 );

    // Add the menu bar

    CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
    if(pMenuBar == NULL)
    {
        TRACE0("Failed to create menu bar.\n");
        return -1;      // fail to create
    }
    pMenuBar->SetFlags(xtpFlagHideMinimizeBox | xtpFlagHideMaximizeBox);

    CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
    if (!pCommandBar ||
        !pCommandBar->LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

    XTPImageManager()->SetIcons(IDR_MAINFRAME, IDR_MAINFRAME_ICON);

    pCommandBars->GetCommandBarsOptions()->szLargeIcons = CSize( 24, 24 );
    pCommandBars->GetCommandBarsOptions()->bLargeIcons = TRUE;
	pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways = FALSE; // Add or Remove Buttons 숨기기

    // Hide array of commands
    pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

    // Set "Always Show Full Menus" option to the FALSE
    pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;
    pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);
    
    /// ★ Docking Pane 생성 : 프로젝트 탐색기, SED 탐색기, Status View
    // Initialize the docking pane manager and set the
    // initial them for the docking panes.  Do this only after all
    // control bars objects have been created and docked.
    m_xtpDockingPaneManager.InstallDockingPanes(this);
    m_xtpDockingPaneManager.SetTheme                     ( xtpPaneThemeVisualStudio2005 );
    m_xtpDockingPaneManager.SetAlphaDockingContext       ( TRUE );
    m_xtpDockingPaneManager.SetShowDockingContextStickers( TRUE );
    m_xtpDockingPaneManager.SetShowContentsWhileDragging ( TRUE );
    m_xtpDockingPaneManager.SetDefaultPaneOptions        ( xtpPaneHasMenuButton );

    /// Create docking panes.
    CXTPDockingPane* pwndProjectPane = m_xtpDockingPaneManager.CreatePane(ID_VIEW_PROJECT, CRect(0, 0, 220, 460), xtpPaneDockLeft );
	
	CXTPDockingPane* pWndNaviPane    = m_xtpDockingPaneManager.CreatePane(ID_VIEW_NAVIGATION, CRect(0, 0, 220, 100), xtpPaneDockBottom, pwndProjectPane);

    //CXTPDockingPane* pwndSEDPane     = m_xtpDockingPaneManager.CreatePane(ID_VIEW_SED,     CRect(0, 0, 220, 20), xtpPaneDockBottom, pWndNaviPane );
    //CXTPDockingPane* pwndPolicyPane  = m_xtpDockingPaneManager.CreatePane(300,  CRect(0, 0, 200, 60), xtpPaneDockBottom );
    //pwndPolicyPane->SetOptions(pwndPolicyPane->GetOptions() & xtpPaneNoDockable);
    //pwndPolicyPane->SetOptions(pwndPolicyPane->GetOptions() | xtpPaneNoDockable);    
    //m_xtpDockingPaneManager.ToggleDocking(pwndPolicyPane);
    //pwndPolicyPane->Close();

	/// ★ Tab Client Window 생성
	/// Initialize tab child window
	VERIFY(m_xtpTabClientWnd.Attach (this, TRUE));
	m_xtpTabClientWnd.SetAutoTheme  ( TRUE );
	m_xtpTabClientWnd.EnableToolTips( );

	m_xtpTabClientWnd.SetNewTabPosition( xtpWorkspaceNewTabRightMost );
	m_xtpTabClientWnd.SetFlags         ( xtpWorkspaceShowCloseTab );
	//m_xtpTabClientWnd.SetFlags( xtpWorkspaceHideClose | xtpWorkspaceHideArrowsAlways | xtpWorkspaceShowActiveFiles | xtpWorkspaceShowCloseSelectedTab );

	TxLogDebugEndMsg();
    return 0;
}


int CMainFrameWnd::OnCreateControl( LPCREATECONTROLSTRUCT lpCreateControl )
{
    if (lpCreateControl->bToolBar) {
        CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
        if (!pToolBar)
            return FALSE;
                
        if (pToolBar->GetBarID() == IDR_MAINFRAME) 
        {
            if (lpCreateControl->nID == ID_SYMBOLOGY_THEME_NODE) {
                lpCreateControl->controlType = xtpControlSplitButtonPopup;
                return TRUE;
            }

            if (lpCreateControl->nID == ID_GRAPH_PC) {
                lpCreateControl->controlType = xtpControlSplitButtonPopup;
                return TRUE;
            }

            if (lpCreateControl->nID == ID_TRANSIT_TRANSITLINE_THEMES) {
                lpCreateControl->controlType = xtpControlSplitButtonPopup;
                return TRUE;
            }
        }
    }

    return FALSE;
}

void CMainFrameWnd::OnClose()
{
    TxLogDebugStartMsg();
    CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
    KProject* pProject = pApp->GetProject();

    if (pProject != NULL)
    {
        int nSelect = -1;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nSelect = AfxMessageBox(_T("종료하시겠습니까?"), MB_YESNO);
		}
		else {
			nSelect = AfxMessageBox(_T("Are you sure want to exit?"), MB_YESNO);
		}
        if( IDYES == nSelect )
        {        
            pApp->SaveProject();
        }
        else 
        {
            return;
        }
    }

    // CKmzApp::ExitInstance() 함수를 콜하지 못하여 아래 코드 추가 함
    // CKmzApp::ExitInstance() 함수를 콜할수 있도록 수정 필요
    // 웹서핑-ExitInstance not call)에 정보 많으나. 현재는 유지함

	//jin.
    //QBicDelete(pProject);
      
    // Save the current state for toolbars and menus.
    SaveCommandBars(_T("CommandBars"));

    // Save the current state for docking panes.
    CXTPDockingPaneLayout layoutNormal(&m_xtpDockingPaneManager);
    m_xtpDockingPaneManager.GetLayout(&layoutNormal);
    layoutNormal.Save(_T("NormalLayout"));
        
    CMDIFrameWnd::OnClose();
    TxLogDebugEndMsg();
}


LRESULT CMainFrameWnd::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
    //TxLogDebugStartMsg();
    if (wParam == XTP_DPN_SHOWWINDOW)
    {
        CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

        if (!pPane->IsValid())
        {
            switch (pPane->GetID())
            {
            case ID_VIEW_PROJECT : 
                if( m_wndProjectExplorer.GetSafeHwnd() == 0 )
                {
                    m_wndProjectExplorer.Create( _T("STATIC"), NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0 );
                }
                pPane->Attach( &m_wndProjectExplorer );
                break;
			case ID_VIEW_NAVIGATION : 
				if( m_wndNaviExplorer.GetSafeHwnd() == 0 )
				{
					m_wndNaviExplorer.Create( _T("STATIC"), NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0 );
				}
				pPane->Attach( &m_wndNaviExplorer);
				break;
            }            
        }

        //TxLogDebugEndMsg();
        return TRUE;
    }
    if (wParam == XTP_DPN_CONTEXTMENU)
    {
        XTP_DOCKINGPANE_CLICK* pClick = (XTP_DOCKINGPANE_CLICK*)lParam;

        CXTPDockingPane* pPopupPane = pClick->pPane;
        if (!pPopupPane) 
        {
            //TxLogDebugEndMsg();
            return FALSE;
        }

        ShowDockingPanePopupMenu(pPopupPane, pClick->pt, pClick->rcExclude);

        //TxLogDebugEndMsg();
        return TRUE;
    }

    //TxLogDebugEndMsg();
    return FALSE;
}


void CMainFrameWnd::OnDestroy()
{
    TxLogDebugStartMsg();

    CMDIFrameWnd::OnDestroy();
    m_xtpTabClientWnd.Detach();
        
    TxLogDebugEndMsg();
}

/**************************************
* Accessors                           *
**************************************/
CChildFrame* CMainFrameWnd::GetChildFrame( unsigned int nID ) const
{
	std::map<unsigned int, CChildFrame*>::const_iterator citFrameWndMap = m_mapTargetChildFrame.find( nID );
	if( m_mapTargetChildFrame.end() != citFrameWndMap )
	{
		return citFrameWndMap->second;
	}

	return NULL;
}

void CMainFrameWnd::OnProjectExplorerWindowShow()
{
    m_xtpDockingPaneManager.ShowPane(ID_VIEW_PROJECT);
}

void CMainFrameWnd::OnNaviExplorerWindowShow()
{
	m_xtpDockingPaneManager.ShowPane(ID_VIEW_NAVIGATION);
}

void CMainFrameWnd::OnCommandWindowShow()
{
    m_xtpDockingPaneManager.ShowPane(ID_VIEW_STATUS);
}

void CMainFrameWnd::ShowNetworkView( KTarget* pTarget )
{
    TxLogDebugStartMsg();
	ASSERT( NULL != pTarget );
	
	unsigned int nID = pTarget->GetObjectID();

	CChildFrame* pChildFrame = GetCreateTargetChildFrame( pTarget );
	ASSERT( NULL != pChildFrame );
        
	pChildFrame->ShowMapView( true );
    TxLogDebugEndMsg();
}


KIOTableView* CMainFrameWnd::ShowTableView( KTarget* pTarget, CString strTableName )
{
  	try
  	{
        ASSERT(NULL != pTarget);
        CChildFrame* pChildFrame = GetCreateTargetChildFrame( pTarget );
        ASSERT( NULL != pChildFrame );

        KIOTables* pIOTables = pTarget->Tables();
        ASSERT(NULL != pIOTables);

        KIOTable*  pIOTable = pIOTables->FindTable(strTableName);
        ASSERT(NULL != pIOTable);

        pChildFrame->ShowIOView( true );
        KIOView* pIOView = pChildFrame->GetIOView();

        KIOTableView* pTableView = nullptr;
        if(NULL != pIOView)
        {
            pTableView = pIOView->FindTableView(pIOTable->Name());
            if(nullptr == pTableView)
            {
                pTableView = pIOView->CreateNewTableView(pIOTable);

                KIOTableController* pController = new KIOTableController(pTarget);  //삭제는 Target 소멸 또는 UnRegister에서..
                pController->Attach(pIOTable, pTableView);
                pTarget->RegisterController(pController);
            }

            pIOView->SetActiveTableView(pIOTable->Name());
        }

        return pTableView;   
  	}
  	catch (KExceptionPtr ex)
  	{
  	    TxExceptionPrint(ex);
  	}
  	catch (...)
  	{
  		TxLogDebugException();
  	}

    return nullptr; 
}

void CMainFrameWnd::CloseTableView( KTarget* pTarget, CString strTableName )
{
	ASSERT(NULL != pTarget);

	CChildFrame* pChildFrame = GetCreatedTargetChildFrame( pTarget );
	if (pChildFrame == NULL)
		return;

	if (!pChildFrame->isIOViewShowed())
		return;

	KIOTables* pIOTables = pTarget->Tables();
	ASSERT(NULL != pIOTables);

	KIOTable*  pIOTable = pIOTables->FindTable(strTableName);
	if (pChildFrame == NULL)
		return;

	KIOView* pIOView = pChildFrame->GetIOView();

	if(NULL != pIOView)
	{
		KIOTableView* pTableView = pIOView->FindTableView(pIOTable->Name());	
        if (pTableView != nullptr)
        {
		    pIOView->CloseTableView(strTableName);
            pTarget->UnregisterController(pIOTable->Name());
        }
	}
}

CChildFrame* CMainFrameWnd::GetCreatedTargetChildFrame( KTarget* pTarget )
{
    ASSERT( pTarget );

    unsigned int nTargetID = pTarget->GetObjectID();

    std::map<unsigned int, CChildFrame*>::iterator iter = m_mapTargetChildFrame.find( nTargetID );
    std::map<unsigned int, CChildFrame*>::iterator end  = m_mapTargetChildFrame.end();

    if (iter != end) {
        return iter->second;
    }

    return nullptr;
}


CChildFrame* CMainFrameWnd::GetCreateTargetChildFrame( KTarget* pTarget )
{
    TxLogDebugStartMsg();
    ASSERT( pTarget );

    unsigned int nTargetID = pTarget->GetObjectID();

    std::map<unsigned int, CChildFrame*>::iterator iter = m_mapTargetChildFrame.find( nTargetID );
    std::map<unsigned int, CChildFrame*>::iterator end  = m_mapTargetChildFrame.end();
    
    if (iter != end) {
        return iter->second;
    }

    CKmzApp*     pApp        = (CKmzApp*)AfxGetApp();
    CChildFrame* pChildFrame = NULL;
    
    if ( true/*KmzSystem::IsPassengerSystem() */)
    {
        pChildFrame = (CChildFrame*)
                        CreateNewChild
                        (
                            RUNTIME_CLASS( CChildFrame ), 
                            IDR_MENU_PASSENGER_TYPE, pApp->getMenuHandle(), pApp->getAcceleratorHandle() 
                        );
    }
    /*ASSERT( NULL != pChildFrame );*/
    
    if (pChildFrame == nullptr) {
        TxLogDebugException();
        return nullptr;
    }

    // 초기 로딩 설정
    pChildFrame->LoadTarget(pTarget);
    m_mapTargetChildFrame.insert( std::make_pair( nTargetID, pChildFrame ) );

    TxLogDebugEndMsg();
    return pChildFrame;
}

void CMainFrameWnd::CloseAllChildFrame()
{
	std::map<unsigned int, CChildFrame* >::iterator iter = m_mapTargetChildFrame.begin();
	std::map<unsigned int, CChildFrame* >::iterator end  = m_mapTargetChildFrame.end();

	// ChildFrame에서 
	// m_mapTargetChildFrame의 항목을 Remove 요청하기 때문에 잠시 복사
	std::vector<CChildFrame*> vecTargetChildFrame;
	while(iter != end)
	{
		vecTargetChildFrame.push_back(iter->second);
		++iter;
	}

	for(size_t i=0; i<vecTargetChildFrame.size(); i++)
	{
		vecTargetChildFrame[i]->CloseFrame();
	}

    // 차일드 프레임이 종료될때 UnRegisterChileFrame(unsigned int nID) 함수를 
    // 호출하기 때문에 불필요하지만 깨끗이 정리
	m_mapTargetChildFrame.clear();
}

void CMainFrameWnd::CloseChildFrame( KTarget* pTarget )
{
	auto nxTaretID = pTarget->GetObjectID();

	auto iter = m_mapTargetChildFrame.find(nxTaretID);
	if (iter != m_mapTargetChildFrame.end()) {
		CChildFrame* pFrame = iter->second;

		m_mapTargetChildFrame.erase(iter); //차일드 프레임이 종료될때 정리되는듯 하다.  pFrame->CloseFrame(); 보다 선행되어야 한다.
		pFrame->CloseFrame();
	}
}

void CMainFrameWnd::TargetChildFrameClosedNotify( unsigned int nID )
{
	std::map<unsigned int, CChildFrame*>::iterator iter = m_mapTargetChildFrame.find( nID );
	if( m_mapTargetChildFrame.end() != iter )
	{
		m_mapTargetChildFrame.erase( iter );
	}
}

void CMainFrameWnd::ShowDockingPanePopupMenu(CXTPDockingPane* pPopupPane, CPoint pt, LPRECT lprcExclude)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_PANES));
	CMenu* pPopup = menu.GetSubMenu(0);


	if (pPopupPane->IsHidden())
	{
		pPopup->CheckMenuItem(ID_POPUP_AUTOHIDE, MF_BYCOMMAND|MF_CHECKED);
		pPopup->EnableMenuItem(ID_POPUP_FLOATING, MF_BYCOMMAND|MF_DISABLED);
		pPopup->EnableMenuItem(ID_POPUP_DOCKABLE, MF_BYCOMMAND|MF_DISABLED);
	}
	else if (pPopupPane->GetOptions() & xtpPaneNoDockable)
	{
		pPopup->CheckMenuItem(ID_POPUP_FLOATING, MF_BYCOMMAND|MF_CHECKED);
	}
	else
	{
		pPopup->CheckMenuItem(ID_POPUP_DOCKABLE, MF_BYCOMMAND|MF_CHECKED);
	}

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = lprcExclude ? *lprcExclude : CRect(0, 0, 0, 0);

	int nCommand = GetCommandBars()->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD, pt.x, pt.y, NULL, &tpm);

	switch (nCommand)
	{
	case ID_POPUP_HIDE:
		pPopupPane->Close();
		break;		

	case ID_POPUP_AUTOHIDE:
		m_xtpDockingPaneManager.ToggleAutoHide(pPopupPane);
		break;

	case ID_POPUP_DOCKABLE:
		if (pPopupPane->GetOptions() & xtpPaneNoDockable)
		{
			pPopupPane->SetOptions(pPopupPane->GetOptions() & ~xtpPaneNoDockable);
		}
		break;

	case ID_POPUP_FLOATING:
		if ((pPopupPane->GetOptions() & xtpPaneNoDockable) == 0)
		{
			if (!pPopupPane->IsFloating())
				m_xtpDockingPaneManager.ToggleDocking(pPopupPane);

			pPopupPane->SetOptions(pPopupPane->GetOptions() | xtpPaneNoDockable);
		}

		break;
	}
}


/**************************************
* Overrides                           *
**************************************/
BOOL CMainFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = 1200;
	cs.cy = 675;
		
	// 2019.05.24 : Free 버전 관련 수정
	if (theApp.IsFreeVersion())
	{
		cs.style &= ~FWS_ADDTOTITLE;
		cs.lpszName = _T("TOVA(Free)");
	}

	return TRUE;
}

// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrameWnd::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrameWnd::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


///// start test code
/// $task : delete test code
void CMainFrameWnd::OnMapOpenmap()
{
	//// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//KProject* pProject = ((CkmzApp*)AfxGetApp())->getProject();

	//CChildFrame* pChildFrame = findTargetFrame( _T("시나리오 #1"), _T("2010") );
	//if( NULL == pChildFrame )
	//{
	//	m_pChildFrame = (CChildFrame*)CreateNewChild( 
	//		RUNTIME_CLASS(CChildFrame), IDR_MAINFRAME );
	//}
	//else 
	//{
	//	m_pChildFrame = pChildFrame;
	//}

	//m_pChildFrame->updateFrameTitle();
	//m_pChildFrame->showMapView( true );

	//KMapView* pwndMapView = m_pChildFrame->getMapView();
	//if( NULL != pwndMapView )
	//{
	//	TCHAR szPath [MAX_PATH] = {0};
	//	::GetCurrentDirectory ( MAX_PATH, szPath );
	//	CString strPath = szPath;
	//	strPath += _T("\\network.mdb");

	//	pwndMapView->openMDBFile( strPath );
	//}
}

void CMainFrameWnd::OnTableOpentable()
{
	//CChildFrame* pChildFrame = findTargetFrame( _T("시나리오 #1"), _T("2010") );
	//if( NULL == pChildFrame )
	//{
	//	m_pChildFrame = (CChildFrame*)CreateNewChild( 
	//		RUNTIME_CLASS(CChildFrame), IDR_MAINFRAME );
	//}
	//else 
	//{
	//	m_pChildFrame = pChildFrame;
	//}


	//ASSERT( NULL != m_pChildFrame);
	////m_pChildFrame->setScenarioName( _T("Test") );
	////m_pChildFrame->setTargetName( _T("2019") );
	////m_pChildFrame->updateFrameTitle();

	//m_pChildFrame->showTableView( true );
	//// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CMainFrameWnd::OnMapClosemap()
{
	//CChildFrame* pFrame = (CChildFrame*)GetActiveFrame();
	//if( NULL != pFrame )
	//{
	//	pFrame->showMapView( false );
	//}
}


void CMainFrameWnd::OnTableClosetable()
{
	//CChildFrame* pFrame = (CChildFrame*)GetActiveFrame();
	//if( NULL != pFrame )
	//{
	//	pFrame->showTableView( false );
	//}
}



void CMainFrameWnd::OnProjectviewCreatetree()
{
	//if( m_wndProjectView.GetSafeHwnd() == NULL )
	//{
	//	return;
	//}

	//m_wndProjectView.createtree();
}
///// end test code


void CMainFrameWnd::OnDevmenuRenderproperty()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

}

//#include "CodeManager.h"

void CMainFrameWnd::OnDevmenuCodemanager()
{
	
}

#include "GenerationModelDlg.h"
#include "GSelectModelDlg.h"
#include "GFunctionUnitDlg.h"
#include "GFunctionRegressionDlg.h"
void CMainFrameWnd::OnMenuModelGeneration()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

    if (KmzSystem::IsPassengerSystem())
    {
        KGenerationModelDlg oGeneratonModelDlg;
        oGeneratonModelDlg.SetTarget(pTarget);
		oGeneratonModelDlg.DoModal();	
    }
}


#include "DistributionModelDlg.h"

void CMainFrameWnd::OnMenuModelDistribution()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();
	
	if (KmzSystem::IsPassengerSystem())
	{
		bool bCheck = KDBaseCheckData::GetCheckParameterData(pTarget);
		if (bCheck == false)
		{
			AfxMessageBox(_T("통행분포 모형을 수행하기 위해 먼저 도구 > 기종점간 통행 속성 값 산출 기능을 수행해야 합니다."));
			return;
		}
		KDistributionModelDlg oDistributionModelDlg;
		oDistributionModelDlg.SetTarget(pTarget);
		oDistributionModelDlg.DoModal();
	}
}

#include "ModeChoiceModelNewDlg.h"
#include "ModeChoiceModelDlg.h"
void CMainFrameWnd::OnMenuModelMode()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

    if (KmzSystem::IsPassengerSystem())
    {
		bool bCheck = KDBaseCheckData::GetCheckParameterData(pTarget);
		if (bCheck == false)
		{
			AfxMessageBox(_T("수단선택 모형을 수행하기 위해 먼저 도구 > 기종점간 통행 속성 값 산출 기능을 수행해야 합니다."));
			return;
		}
	    //KModeChoiceModelDlg dlg;
		KModeChoiceModelNewDlg dlg;
	    dlg.SetTarget(pTarget);
	    dlg.DoModal();
    }
}

#include "AssignmentModelDlg.h"
void CMainFrameWnd::OnMenuModelAssignment()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();
	
	if (KmzSystem::IsPassengerSystem())
    {
        if( !KDBaseAssignment::ModeChoiceExist(pTarget) )
        {
            AfxMessageBox(_T("정의된 수단선택 모형 수행 관련 데이터가 존재하지 않습니다."));
            return;
        }

		if (KDBaseCheckData::GetExistModeODData(pTarget) == false)
		{
			AfxMessageBox(_T("수단 통행 테이블에 데이터가 존재하지 않습니다."));
			return;
		}

		if (KDBaseCheckData::GetCheckInspectionErrorHighway(pTarget) == false)
		{
			return;
		}
    }

	KAssignmentModelDlg dlg(pTarget);
	dlg.DoModal();
}

#include "TimeODGenerationDlg.h"
void CMainFrameWnd::OnMenuTimeODGeneration()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	if (KmzSystem::IsPassengerSystem())
	{
		KTimeODGenerationDlg dlg;
		dlg.SetTarget(pTarget);
		dlg.DoModal();
	}
	else
	{
		KTimeODGenerationDlg dlg;
		dlg.SetTarget(pTarget);
		dlg.DoModal();
	}
}

// test code start
#include "CodeManagementDlg.h"
#include "IOTransitTableView.h"


void CMainFrameWnd::OnDbmanagementCodemanagement()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		//AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("There is no any active scenario !\nPlease open the map or table in Project Explorer."));
		}
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();
	KCodeManager* pCodeManager = pTarget->CodeManager();
	KCodeManagementDlg dlg;

	dlg.SetTarget(pTarget);
	dlg.CodeManager(pCodeManager);
	dlg.DoModal();
}


void CMainFrameWnd::OnUpdateDbmanagementCodemanagement(CCmdUI *pCmdUI)
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}


void CMainFrameWnd::OnUpdateMultiModalModel(CCmdUI *pCmdUI)
{
// #if _TRANSIT_EXCEPT_VER
// 	pCmdUI->Enable(FALSE);
// #else
// 	pCmdUI->Enable(TRUE);
// #endif
	pCmdUI->Enable(TRUE);
}


void CMainFrameWnd::OnToolsPaSum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	KPASumDlg dlg;
	dlg.DoModal();
}


void CMainFrameWnd::OnDataInspectionSed()
{    
}


void CMainFrameWnd::OnOdOD2PASum()
{
    CMDIChildWnd* pChildWnd = MDIGetActive();
    if(NULL == pChildWnd)
    {
        AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
        return;
    }

    if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
    {
        AfxMessageBox(_T("Not a ChildFrame "));
        return;
    }

    CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
    KTarget* pTarget = pChildFrame->GetTarget();

    KOD2PASumDlg* pModelessDlg = new KOD2PASumDlg();
    pModelessDlg->SetTarget(pTarget);

    pModelessDlg->Create(KOD2PASumDlg::IDD, this);
    pModelessDlg->ShowWindow(SW_SHOW);
}


void CMainFrameWnd::OnToolsCalculateTrafficVol()
{
    CMDIChildWnd* pChildWnd = MDIGetActive();
    if(NULL == pChildWnd)
    {
        AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
        return;
    }

    if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
    {
        AfxMessageBox(_T("Not a ChildFrame "));
        return;
    }

    CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
    KTarget* pTarget = pChildFrame->GetTarget();

    KCalculateTrafficVolumeDlg* pModelessDlg = new KCalculateTrafficVolumeDlg();
    pModelessDlg->SetTarget(pTarget);
    pModelessDlg->Create(KCalculateTrafficVolumeDlg::IDD, this);
    pModelessDlg->ShowWindow(SW_SHOW);
}


void CMainFrameWnd::OnInfluenceOff()
{
    // 영향권 설정 해제
    //BulkLinkInfluenceOff
    CMDIChildWnd* pChildWnd = MDIGetActive();
    if(NULL == pChildWnd)
    {
        AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
        return;
    }

    if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
    {
        AfxMessageBox(_T("Not a ChildFrame "));
        return;
    }

    CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
    KTarget* pTarget = pChildFrame->GetTarget();

    bool bResult = false;
    bResult = KBulkDBase::BulkLinkInfluenceOff(pTarget->GetDBaseConnection());

    pChildFrame->ShowIOView( true );
    KIOView*  pIOView  = pChildFrame->GetIOView();
    KIOTable* pIOTable = pTarget->Tables()->FindTable(TABLE_LINK);

    if(NULL != pIOView)
    {
        KIOTableView* pTableView = pIOView->FindTableView(_T("link"));
        if(NULL == pTableView)
        {
			pTableView = pIOView->CreateNewTableView(pIOTable);
            KIOTableController* pController = new KIOTableController(pTarget);
            pController->Attach(pIOTable, pTableView);
		}
        pIOView->SetActiveTableView(pIOTable->Name());
    }

    if (bResult)
    {
        AfxMessageBox(_T("영향권 정보를 모두 Off 하였습니다."));
    }
    else
    {
        AfxMessageBox(_T("오류가 발생하였습니다."));
    }
}


void CMainFrameWnd::OnInfluenceStatistics()
{
    CMDIChildWnd* pChildWnd = MDIGetActive();
    if(NULL == pChildWnd)
    {
        AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
        return;
    }

    if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
    {
        AfxMessageBox(_T("Not a ChildFrame "));
        return;
    }

    CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
    KTarget* pTarget = pChildFrame->GetTarget();

    KInfluenceStatisticsDlg dlg;
    dlg.SetTarget(pTarget);

    dlg.DoModal();
}

void CMainFrameWnd::OnFreightLogisticModel()
{
}

void CMainFrameWnd::OnFreightVehicleConvert()
{
    
}


void CMainFrameWnd::OnCustomize()
{
    TxLogDebugStartMsg();
    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars != NULL)
    {
        // Instanciate the customize dialog object.
        CXTPCustomizeSheet dlg(pCommandBars);

        // Add the keyboard page to the customize dialog.
        CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
        dlg.AddPage(&pageKeyboard);
        pageKeyboard.AddCategories(IDR_kmzTYPE);

        // Add the options page to the customize dialog.
        CXTPCustomizeOptionsPage pageOptions(&dlg);
        dlg.AddPage(&pageOptions);

        // Add the commands page to the customize dialog.
        CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
        pCommands->AddCategories(IDR_kmzTYPE);

        // Use the command bar manager to initialize the
        // customize dialog.
        pCommands->InsertAllCommandsCategory();
        pCommands->InsertBuiltInMenus(IDR_kmzTYPE);
        pCommands->InsertNewMenuCategory();

        // Dispaly the dialog.
        dlg.DoModal();
    }

    TxLogDebugEndMsg();
}


void CMainFrameWnd::OnArcgistestStringchecker()
{
  
}

void CMainFrameWnd::OnToolsIRIORatioInput()
{
	
}

#include "CalculateRatioRegionDlg.h"

void CMainFrameWnd::OnToolsRatioRegion()
{

	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	KCalculateRatioRegionDlg RegionDlg;
	RegionDlg.SetTarget(pTarget);
	if( RegionDlg.DoModal() == IDCANCEL )
	{
		return;
	}
}

void CMainFrameWnd::OnResultViewDistribution()
{
	KResultViewDistributionDlg oDlg;	
	oDlg.DoModal();
}


void CMainFrameWnd::OnResultViewMode()
{
	KResultViewModeDlg oDlg;
	oDlg.SetTable(MODECHOICE_MODEL);
	oDlg.DoModal();
}

void CMainFrameWnd::OnResultviewModeod()
{
}

void CMainFrameWnd::OnResultviewVehicleod()
{
}

void CMainFrameWnd::OnResultViewAssignment()
{
	KResultViewAssignmentDlg oResultAssigmentDlg;
	if( oResultAssigmentDlg.DoModal() == IDCANCEL )
	{
		return;
	}
}


void CMainFrameWnd::OnLogViewAssignment()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget*     pTarget     = pChildFrame->GetTarget();
	ASSERT(NULL != pTarget);

	CString     strTargetLocation = pTarget->GetLocation();
	CString     strFileName       = strTargetLocation + _T("\\") + ASSIGNMENT_LOGFILE_NAME;

	CFileFind   cFileFinder;
	if(FALSE == cFileFinder.FindFile(strFileName) )
	{
		CString strMsg(_T(""));
		strMsg.Format(_T("%s 파일이 존재하지 않습니다."), ASSIGNMENT_LOGFILE_NAME);
		AfxMessageBox(strMsg);
		return;
	}

	ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOW);    // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
}


void CMainFrameWnd::OnGravityCalibration()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget*     pTarget     = pChildFrame->GetTarget();

	if (!KDBaseCheckData::GetCheckParameterData(pTarget))
	{
		AfxMessageBox(_T("해당 기능을 수행하기 위해 먼저 도구 > 기종점간 통행 속성 값 산출 기능을 수행해야 합니다."));
		return;
	}
	
	KGravityCalibrationDlg oGravityCalibrationDlg;
	oGravityCalibrationDlg.SetTarget(pTarget);
	oGravityCalibrationDlg.DoModal();

}

void CMainFrameWnd::OnFCalculatePathFindInfo()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();


	KCalculatePathFindInfoDlg oCalculatePathFindInfoDlg;
	oCalculatePathFindInfoDlg.SetInfo(pTarget);
	oCalculatePathFindInfoDlg.DoModal();
}

void CMainFrameWnd::OnFCalculateTimeCostInfo()
{
	
}

void CMainFrameWnd::OnMenuPATOOD()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	KPA2ODModelDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnMenuDevPlanManager()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	KDevPlanMgrDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnMenuDevPlan()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	KDevPlanModelDlg oDlg(pTarget);
	oDlg.DoModal();
}


void CMainFrameWnd::OnNetworkOnOff()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	if (pChildFrame)
	{
		if (pChildFrame->IsMapViewShowed() == false)
		{
			pChildFrame->ShowMapView(true);
		}
	}
	else
	{
		TxLogDebugWarning();
		AfxMessageBox(_T("분석 년도를 선택해 주세요."));
		return;
	}
	KTarget* pTarget = pChildFrame->GetTarget();
	ASSERT(NULL != pTarget);

	KIOTables* pIOTables = pTarget->Tables();
	ASSERT(NULL != pIOTables);

	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_LINK);
	ASSERT(NULL != pIOTable);

	KMapView* pMapView = ImChampFrameWindow::GetActiveMapView();
    if (pMapView == nullptr)
        return;

	KNetworkOnOffDlg oNetworkOnOffDlg;
	oNetworkOnOffDlg.SetTarget(pTarget);

	if(oNetworkOnOffDlg.DoModal() == IDOK)
	{

	}
}

void CMainFrameWnd::OnImportTable()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd) {
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame))) {
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	KImportTableDlg oDlg; {
        oDlg.SetInitValue(pTarget, 0);
    }
	
	oDlg.DoModal();
}

void CMainFrameWnd::OnUpdateImportTable( CCmdUI *pCmdUI )
{
    CMDIChildWnd* pChildWnd = MDIGetActive();
    if (pChildWnd == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);   
}

void CMainFrameWnd::OnExportTable()
{
    CMDIChildWnd* pChildWnd = MDIGetActive();
    if(NULL == pChildWnd) {
        AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
        return;
    }

    if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame))) {
        AfxMessageBox(_T("Not a ChildFrame "));
        return;
    }

    CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
    KTarget* pTarget = pChildFrame->GetTarget();

    KImportTableDlg oDlg; {
        oDlg.SetInitValue(pTarget, 1);
    }
    
    oDlg.DoModal();
}

void CMainFrameWnd::OnUpdateExportTable( CCmdUI *pCmdUI )
{
    CMDIChildWnd* pChildWnd = MDIGetActive();
    if (pChildWnd == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);   
}

void CMainFrameWnd::OnFImportTable()
{
	
}

void CMainFrameWnd::OnMultiModalSetAccessibility()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	if (KmzSystem::IsPassengerSystem())
	{
		if (KDBaseCheckData::GetCheckInspectionErrorTransit(pTarget) == false)
		{
			return;
		}
	}

	KSetAccessibility oSetAccessibility(pProject, pTarget);
	oSetAccessibility.DoModal();
}

void CMainFrameWnd::OnMultiModalGenerationBusPath()
{
#pragma region new
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();
	KProject* pProject  = ImChampFrameWindow::GetProject();


	if (KmzSystem::IsPassengerSystem())
	{
		if (KDBaseCheckData::GetExistModeODData(pTarget) == false)
		{
			AfxMessageBox(_T("수단 통행 테이블에 데이터가 존재하지 않습니다."));
			return;
		}

		if (KDBaseCheckData::GetCheckInspectionErrorTransit(pTarget) == false)
		{
			return;
		}
	}

	bool bBackStep = false;
	bool bContinue = true;

	int          nTimeMethod          = 0;
	KIOColumn*   pIOColumnBusOD       = nullptr;
	KIOColumn*	 pIOColumnBusSubwayOD = nullptr;
	KIOColumn*   pIOColumnUseLink     = nullptr;
	std::list<TGenerationPathInfo> lstGenerationpath;

	if (false == KDBaseCheckData::GetCheckAccessStopCount(pTarget))
	{
		AfxMessageBox(_T("이용가능 역/정류장 데이터가 존재하지 않습니다.\r\n경로 만들기 > 대중교통 > 이용가능 역/정류장 생성 기능을 먼저 실행해야 합니다."));
		/*KSetAccessibility oDlg(pProject, pTarget);
		if (oDlg.DoModal() != IDOK)
		{
			return;
		}*/
		return;
	}
	
	while( bContinue )
	{
		KGenerationBusPathStep1Dlg oGenBusPathStep1(pTarget);

		if(bBackStep)
		{
			oGenBusPathStep1.m_nTimeMethod          = nTimeMethod;
			oGenBusPathStep1.m_lstGenerationInfo    = lstGenerationpath;
			oGenBusPathStep1.m_pIOColumnUseLink     = pIOColumnUseLink;
		}
		
		if( IDCANCEL == oGenBusPathStep1.DoModal())
		{
			return;
		}

		nTimeMethod          = oGenBusPathStep1.m_nTimeMethod;
		pIOColumnUseLink     = oGenBusPathStep1.m_pIOColumnUseLink;
		lstGenerationpath	 = oGenBusPathStep1.m_lstGenerationInfo;

		KGenerationBusPathStep2Dlg oGenBusPathStep2(pProject, pTarget, nTimeMethod, lstGenerationpath, pIOColumnUseLink);

		if( IDCANCEL == oGenBusPathStep2.DoModal())
		{
			return;
		}

		if(oGenBusPathStep2.GetStatus() == 2)
		{
			bBackStep = true; // back 버튼
		}
		else
		{
			bContinue = false; // run 버튼
		}
	}
#pragma endregion new

}

void CMainFrameWnd::OnMultiModalModel()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	if (KmzSystem::IsPassengerSystem())
	{
		if (KDBaseCheckData::GetCheckInspectionErrorTransit(pTarget) == false)
		{
			return;
		}
	}

	KMultiModalMainDlg oMultiModal(pTarget);
	oMultiModal.DoModal();
}


void CMainFrameWnd::OnDBManagementSystemCodeManagement()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	KDefineSystemCodeDlg oDlg(pTarget, false);
	oDlg.DoModal();
}


void CMainFrameWnd::OnTableManagerVector()
{
	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	KTableManagerVectorDlg oTableManagerVectorDlg(pProject);
	oTableManagerVectorDlg.DoModal();


// 	const KTDScenarioMap& scenarioMap = pProject->GetScenarioMap();
// 	KTDScenarioMap::const_iterator citScenarioMap = scenarioMap.begin();
// 	while( scenarioMap.end() != citScenarioMap )
// 	{
// 		KScenario* pScenario = citScenarioMap->second;
// 
// 		pScenario->GetTargetMap();
// 
// 		++citScenarioMap;
// 	}
// 
// 
// 
// 
// 	KScenario* pBaseScenario     = a_pProject->GetBaseScenario();
// 
// 	if( NULL != pBaseScenario )
// 	{
// 		InsertTreeItemScenarioInfo( hItemProject, pBaseScenario, bKTDBBase );
// 	}
// 
// 	const KTDScenarioMap& oScenarioMap  = a_pProject->GetScenarioMap();
// 	KTDScenarioMap::const_iterator iter = oScenarioMap.begin();
// 	KTDScenarioMap::const_iterator end  = oScenarioMap.end();
// 
// 	while( iter != end )
// 	{
// 		KScenario* pScenario = iter->second;
// 		InsertTreeItemScenarioInfo( hItemProject, pScenario, bKTDBBase );
// 		++iter;
// 	}


}


void CMainFrameWnd::OnTableManagerMatrix()
{
	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	KTableManagerMatrixDlg oTableManagerMatrixDlg(pProject);
	oTableManagerMatrixDlg.DoModal();
}

void CMainFrameWnd::OnTranstiCostManagement()
{

	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	//if (false == KDBaseCheckData::GetCheckTransitInfoData(pTarget))
	//{
	//	AfxMessageBox(_T("대중교통 관련 테이블에 데이터가 존재하지 않습니다."));
	//	return;
	//}

	KDefineTransitCodeCostDlg oDlg(pTarget, false);
	oDlg.DoModal();
}

void CMainFrameWnd::OnToolsODParameter()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();


	KCalculatParameterDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnDefineRegionalMode()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	KDefineRegionMode oDlg(pTarget, false);

	oDlg.DoModal();
}

void CMainFrameWnd::OnInterAccessTerminalGenerator()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	if (KmzSystem::IsPassengerSystem())
	{
		if (KDBaseCheckData::GetCheckInspectionErrorTransit(pTarget) == false)
		{
			return;
		}
	}

	KAccessTerminalGeneratorDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnInterPathGenerator()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();
	KTarget*  pTarget  = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        return;
    }

	if (KmzSystem::IsPassengerSystem())
	{
		if (KDBaseCheckData::GetCheckInspectionErrorTransit(pTarget) == false)
		{
			return;
		}
	}

	bool bErr(false);
	if (KDBaseCheckData::GetCheckTerminaNodeData(pTarget) == false)
	{
		/************************************************************************/
		/* 명지대 요청사항(16.12.08)
		만약 도시부 지역코드 0번(Default)에 노드 개수가 0이면(전체 네트워크가 도시부 통행으로 인식됨),
		경로 만들기 -> 복합수단 하위 메뉴에 이용가능 터미널 생성 기능을 수행하지 않아도
		복합수단 경로 생성을 실행할 수 있게끔 제약을 풀어줘야 함.*/
		/************************************************************************/
// 		KIOTable* pTable = pTarget->Tables()->FindTable(TABLE_NODE);
// 		if (pTable->RecordCount() > 0) {
// 			if (KDBaseInterModal::GetCountTargetRegionZeroCode(pTarget) != 0) { // 만약 도시부 지역코드 0번(Default)에 노드 개수가 0개면 통과 아니면 실패
// 				AfxMessageBox(_T("이용가능 터미널 생성 결과가 존재하지 않습니다.\n경로 만들기 > 복합수단 > 이용가능 터미널 생성을 먼저 실행해 주세요."));
// 				return;
// 			}
// 			else {
// 
// 			}
// 		}

		bErr = true;
		if (KDBaseInterModal::GetCountTargetRegionZeroCode(pTarget) == 0) { // 만약 도시부 지역코드 0번(Default)에 노드 개수가 0개면 통과 아니면 실패
			bErr = false;
			KIOTable* pTable = pTarget->Tables()->FindTable(TABLE_NODE);
			if (pTable->RecordCount() > 0) {
				bErr = false;
			}
			else {
				bErr = true;// node 테이블의 레코드 갯수가 0이기때문에 GetCountTargetRegionZeroCode()가 0을 뱉을 수도 있다. 
			}

		}
		else {
			bErr = true;
		}
	}

	if (bErr) {
		AfxMessageBox(_T("이용가능 터미널 생성 결과가 존재하지 않습니다.\n경로 만들기 > 복합수단 > 이용가능 터미널 생성을 먼저 실행해 주세요."));
		return;
	}
	else {
		KInterPathGeneratorMainDlg oDlg(pTarget);
		oDlg.DoModal();
	}
}

void CMainFrameWnd::OnInterModalMain()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
	KProject* pProject = pApp->GetProject();

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	if (ImChampFileExist::IsExistInterModalGeneraterFileMessage(pTarget) == false)
	{
		return;
	}
	
	if (KmzSystem::IsPassengerSystem())
	{
		if (KDBaseCheckData::GetCheckInspectionErrorTransit(pTarget) == false)
		{
			return;
		}
	}

	KInterModalMainDlg oDlg(pTarget);
	oDlg.DoModal();
}


void CMainFrameWnd::OnZoneGroupGeneration()
{
    try
    {
    	KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        KZoneGroupGeneration dlg(pTarget);
        dlg.DoModal();
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


void CMainFrameWnd::OnOdGroupGeneration()
{    
    try
    {
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

        KODGroupGeneration dlg(pTarget);
		//KODGroupGenerationDlg dlg(pTarget);
        dlg.DoModal();
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

void CMainFrameWnd::OnOdGroupGenerationPart2()
{
	try
	{
		KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
		KODGroupGenerationDlg dlg(pTarget);
		dlg.DoModal();
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

void CMainFrameWnd::OnEmmeNetworkImportTable()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget* pTarget = pChildFrame->GetTarget();

	bool  bContinue = true;
	bool  bBackStep = false;

	TCHAR szCurrentDirectory[MAX_PATH] = {0};
	::GetCurrentDirectory( MAX_PATH, szCurrentDirectory );

	CString strFile = szCurrentDirectory;

	while (bContinue)
	{
		KTableImpEmmeSelectFileDlg oDlg1(pTarget);

		if (bBackStep)
		{
			oDlg1.m_strFile = strFile;
			bBackStep       = false;
		}

		if (oDlg1.DoModal() != IDOK)
			break;

		strFile = oDlg1.m_strFile;

		KTableImpEmmeNetworkDlg oDlg(pTarget);
		oDlg.SetImportFile(strFile);

		if (oDlg.DoModal() == IDCANCEL)
			break;;

		if (oDlg.GetStatus() == 2)
		{
			bBackStep = true;
		}
		else
		{
			bContinue = true;
		}

	}
}

void CMainFrameWnd::OnEmmeNetworkExportTable()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget*     pTarget     = pChildFrame->GetTarget();

	KTableExpEmmeNetworkDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnUpdateEmmeNetworkExportTable(CCmdUI *pCmdUI)
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		std::vector<CString> vecExportTable;
		vecExportTable.push_back(TABLE_NODE);
		vecExportTable.push_back(TABLE_LINK);

		int nHaveRecordTable(0);
		for (size_t i= 0; i< vecExportTable.size(); i++) {
			CString   strTableName = vecExportTable[i];
			KIOTable* pTable       = pTarget->Tables()->FindTable(strTableName);

			if (pTable->RecordCount() > 0) {
				nHaveRecordTable++;
			}
		}

		if (nHaveRecordTable < 1) {
			pCmdUI->Enable(FALSE);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void CMainFrameWnd::OnEmmeMatrixImportTable()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget*     pTarget     = pChildFrame->GetTarget();

	KTableImpEmmeMatrixDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnUpdateEmmeMatrixExportTable(CCmdUI *pCmdUI)
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		int nHaveRecordTable(0);

		KIOTables* pTables = pTarget->Tables();
		int        nTables = pTables->GetCount();
		int        nIndex(0);

		for (int i= 0; i< nTables; i++){
			KIOTable* pTable = pTables->GetTableAt(i);

			if (!(pTable->TableDisplayType() == KEMIOTableDisplayTypeMatrix || pTable->TableDisplayType() == KEMIOTableDisplayTypeParameter))
				continue;

			if (pTable->RecordCount() > 0) {
				nHaveRecordTable++;
			}
		}

		if (nHaveRecordTable < 1) {
			pCmdUI->Enable(FALSE);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void CMainFrameWnd::OnEmmeMatrixExportTable()
{
	CMDIChildWnd* pChildWnd = MDIGetActive();
	if(NULL == pChildWnd)
	{
		AfxMessageBox(_T("활성화된 정보가 없습니다!\n프로젝트 탐색기에서 지도 또는 테이블을 열어 활성화 해주세요."));
		return;
	}

	if(!pChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		AfxMessageBox(_T("Not a ChildFrame "));
		return;
	}

	CChildFrame* pChildFrame = (CChildFrame*)pChildWnd;
	KTarget*     pTarget     = pChildFrame->GetTarget();

	KTableExpEmmeMatrixDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnDefaultAccessStationSetting()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefaultAccessStationSetting oDlg(pTarget);
		oDlg.DoModal();
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

void CMainFrameWnd::OnDefaultPathGenerateConstraint()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefaultPathGenerateConstraint oDlg(pTarget);
		oDlg.DoModal();
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

void CMainFrameWnd::OnDefaultMultimodalPathGenerator()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefaultMultimodalPathGeneration oDlg(pTarget);
		oDlg.DoModal();
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

void CMainFrameWnd::OnDefaultModeChoicePara()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefaultMultimodalModeChoicePara oDlg(pTarget);
		oDlg.DoModal();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
return;
}

void CMainFrameWnd::OnDefaultTransitRouteChoicepara()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefaultTransitRouteChoicePara oDlg(pTarget);
		oDlg.DoModal();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	return;
}

void CMainFrameWnd::OnDefaultHighwayAssignment()
{
	try
	{
		CKmzApp*  pApp     = (CKmzApp*) AfxGetApp();
		KProject* pProject = pApp->GetProject();

        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

			KDefaultHighwayAssignment oDlg(pTarget, pProject);
			oDlg.DoModal();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	return;
}

void CMainFrameWnd::OnDefaultAccessTerminalSetting()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefaultAccessTerminalSetting oDlg(pTarget);
		oDlg.DoModal();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	return;
}

void CMainFrameWnd::OnDefaultInterModeChoicePara()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefInterUrbanModeChoiceParaDlg oDlg(pTarget);
		oDlg.DoModal();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void CMainFrameWnd::OnDefaultInterPathGeneratorPara()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefaultInterPathGenerator oDlg(pTarget);
		oDlg.DoModal();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void CMainFrameWnd::OnDefaultRegionalModeChoicePara( void )
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		KDefaultRegionalModeChoiceParaDlg oDlg(pTarget);
		oDlg.DoModal();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void CMainFrameWnd::OnLogHighwayModel()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		CString   strFilePath(_T(""));
		strFilePath.Format(_T("%s%s%s\\%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_HIGHWAYASSIGN_FOLDER_NAME, ASSIGN_LOGFILE_NAME);

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFilePath) == FALSE)
		{
			CString strMsg(_T(""));
			strMsg.Append(_T("Log 파일이 존재 하지 않습니다.\r\n"));
			ThrowException(strMsg);
		}
		ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void CMainFrameWnd::OnLogMultiModalModel()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		CString   strFilePath(_T(""));
		strFilePath.Format(_T("%s%s%s\\%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME,ASSIGN_LOGFILE_NAME);

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFilePath) == FALSE)
		{
			CString strMsg(_T(""));
			strMsg.Append(_T("Log 파일이 존재 하지 않습니다.\r\n"));
			ThrowException(strMsg);
		}
		ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void CMainFrameWnd::OnLogInterModalModel()
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr)
            return;

		CString   strFilePath(_T(""));
		strFilePath.Format(_T("%s%s%s%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME, _T("IntermodalTrafficAssignment_Log.txt"));

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFilePath) == FALSE)
		{
			CString strMsg(_T(""));
			strMsg.Append(_T("Log 파일이 존재 하지 않습니다.\r\n"));
			ThrowException(strMsg);
		}
		ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void CMainFrameWnd::OnUpdateLogHighwayModel( CCmdUI *pCmdUI )
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		CString   strFilePath(_T(""));
		strFilePath.Format(_T("%s%s%s\\%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_HIGHWAYASSIGN_FOLDER_NAME, ASSIGN_LOGFILE_NAME);

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFilePath) == FALSE)
		{
			throw 1;
		}

		oFileFinder.FindNextFile();
		if (oFileFinder.GetLength() <= 0)
		{
			throw 1;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrameWnd::OnUpdateLogMultiModalModel( CCmdUI *pCmdUI )
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		CString   strFilePath(_T(""));
		strFilePath.Format(_T("%s%s%s\\%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME,ASSIGN_LOGFILE_NAME);

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFilePath) == FALSE)
		{
			throw 1;
		}

		oFileFinder.FindNextFile();
		if (oFileFinder.GetLength() <= 0)
		{
			throw 1;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrameWnd::OnUpdateLogInterModalModel( CCmdUI *pCmdUI )
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }

		CString   strFilePath(_T(""));
		strFilePath.Format(_T("%s%s%s%s"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME, _T("IntermodalTrafficAssignment_Log.txt"));

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFilePath) == FALSE)
		{
			throw 1;
		}

		oFileFinder.FindNextFile();
		if (oFileFinder.GetLength() <= 0)
		{
			throw 1;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrameWnd::OnFDefaultModeChoiceModelPara( void )
{
	
}

void CMainFrameWnd::OnDefaultDistributionPara()
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KDefaultDistributionParaSettingDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnFDefaultGenerationModelPara( void )
{
	
}

void CMainFrameWnd::OnFDefaultLogisticModelPara( void )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KDefaultLogisticParaDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnFDefaultVehicleConvModelPara( void )
{
}

void CMainFrameWnd::OnFDefaultCalTimeCostPara( void )
{
}

void CMainFrameWnd::OnFModelProcess()
{
}

void CMainFrameWnd::OnDefaultGenerationPara()
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KDefaultGenerationParaMainDlg oDlg(pTarget);
	oDlg.DoModal();
}


void CMainFrameWnd::OnDefaultModeChoiceModelDefineVariable()
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KDefaultMDefineVariableGroupDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnNetShapeExport( void )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KIOTable* pTable = pTarget->Tables()->FindTable(TABLE_NODE);
	KShapeExportDlg oDlg(pTarget, pTable);
	oDlg.DoModal();
}

void CMainFrameWnd::OnUpdateNetShapeExport( CCmdUI *pCmdUI )
{
	try
	{
        KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
        if (pTarget == nullptr) {
            pCmdUI->Enable(FALSE);
            return;
        }
		
		std::vector<CString> vecShapeExportTable;
		vecShapeExportTable.push_back(TABLE_NODE);
		vecShapeExportTable.push_back(TABLE_LINK);
		vecShapeExportTable.push_back(TABLE_ZONE);

		int nHaveRecordTable(0);
		for (size_t i= 0; i< vecShapeExportTable.size(); i++) {
			CString   strTableName = vecShapeExportTable[i];
			KIOTable* pTable = pTarget->Tables()->FindTable(strTableName);

			if (pTable->RecordCount() > 0) {
				nHaveRecordTable++;
			}
		}

		if (nHaveRecordTable < 1) {
			pCmdUI->Enable(FALSE);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void CMainFrameWnd::OnDefaultInterRunOption()
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KDefaultInterRunOptionDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnDefaultCO2Interpolation( void )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KDefaultCo2EmissionsInterpolationParaDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnToolsEmissionsInterpolation( void )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KCO2EmissionInterpolationDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnDefaultCO2Equation( void )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KDefaultCo2EmissionsEquationParaDlg oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnToolsEmissionsEquation( void )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	std::map<int, std::vector<KCO2EquationData>> mapDefCO2EquationData;
	
	while (true)
	{
		KCO2EmissionEquationDlg oDlg(pTarget);
		oDlg.SetCO2EquationData(mapDefCO2EquationData);

		if (oDlg.DoModal() == IDCANCEL)
			break;

		mapDefCO2EquationData   = oDlg.GetCO2EquationData();
		std::set<int> setModeID = oDlg.GetModeSet();
		
		KCO2EmissionEquation1Dlg oDlg1(pTarget);
		oDlg1.SetCO2EquationData(mapDefCO2EquationData);
		oDlg1.SetModeSet(setModeID);

		if (oDlg1.DoModal() == IDCANCEL)
			break;

		if (!oDlg1.m_bBackStep)
		{	
			//종료
			break;
		}
	}	
}

void CMainFrameWnd::OnNetShapeNodeImport()
{
	KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KIOTable* pTableNode = pTarget->Tables()->FindTable(TABLE_NODE);
	
	KDlgShapeNode oDlg(pTarget, pTableNode);
	oDlg.DoModal();
}

void CMainFrameWnd::OnNetShapeLinkImport()
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KDlgShapeLink oDlg(pTarget);
	oDlg.DoModal();
}

void CMainFrameWnd::OnNetShapeZoneImport()
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr)
        return;

	KIOTable* pTableZone = pTarget->Tables()->FindTable(TABLE_ZONE);
	KDlgShapeDemarcation oDlg(pTarget, pTableZone);
	oDlg.DoModal();
}

void CMainFrameWnd::OnUpdateSymbologyThemeNode( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}

void CMainFrameWnd::OnUpdateSymbologyThemeLink( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}


void CMainFrameWnd::OnUpdateNetworkeditNode( CCmdUI *pCmdUI )
{
    KTarget* pTarget = ImChampFrameWindow::GetActiveFrameTarget();
    if (pTarget == nullptr) {
        pCmdUI->Enable(FALSE);
        return;
    }

    pCmdUI->Enable(TRUE);
}

void CMainFrameWnd::OnUpdateFrameTitle( BOOL bAddToTitle )
{
	//do-nothing
	//재정의 해주지 않을경우 IDR_MAINFRAME으로 App Title을 내부적으로 초기화하는듯함
}

void CMainFrameWnd::OnEXE_TRIPOD_TO_VEHICLEOD()
{
	KTripODToVehODDlg dlg2;
	dlg2.DoModal();
}

void CMainFrameWnd::OnEXE_FREIGHTOD_DIVISION()
{
	KFODDiv dlg2;
	dlg2.DoModal();
}


void CMainFrameWnd::OnEXE_ACCESSTRIPOD_TO_VEHICLEOD()
{
	KAccessTripODToVehODDlg dlg2;
	dlg2.DoModal();
}
