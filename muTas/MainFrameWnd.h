// MainFrm.h : interface of the CMainFrame class
//


#pragma once


#include "ProjectExplorerWnd.h"
#include "NaviExplorerWnd.h"
#include "IOTableView.h"
#include "CommandWnd.h"

/* forward declarations */
class CChildFrame;	// IOTable frame window

class CMainFrameWnd : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrameWnd)

	/**************************************
	* Constructors and Destructor         *
	**************************************/
public:
	CMainFrameWnd();
	virtual ~CMainFrameWnd();


	/**************************************
	* Accessor                            *
	**************************************/
public :

	/**
	* @brief Project View를 반환한다.
	* @return KProjectView* - Project View
	*/
	inline KProjectExplorerWnd* GetProjectView( void )
	{
		return &m_wndProjectExplorer;
	}

	/**
	* @brief 상태창을 반환한다.
	* @return KStatusView* - 상태 창
	
	inline KStatusViewWnd* GetStatusView( void )
	{
		return &m_wndStatusView;
	}
    */
	
	/**
	* @brief Navigation Tree View를 반환한다.
	* @return KSEDTreeView - SED Tree View
	*/
	inline KNaviExplorerWnd* GetNaviTreeView( void )
	{
		return &m_wndNaviExplorer;
	}

	/**
	* @brief Child Frame Window를 반환한다.
	* @param[in] nID - Target year ID
	* @return CChildFrame* - Child Frame Window
	*/
	CChildFrame* GetChildFrame( unsigned int nID ) const;

	//CChildFrame* getChildFrame( CString strScenarioName, CString strTargetName ) const;


	/**************************************
	* Operations                          *
	**************************************/
public :

	/**
	* @brief Target year에 해당하는 Child Frame을 표출한다.
	* @param[in] pTarget - Target year
	* @return CChildFrmae* - Child Frame Window
	*/

    CChildFrame* GetCreatedTargetChildFrame( KTarget* pTarget );
	CChildFrame* GetCreateTargetChildFrame( KTarget* pTarget );
	void CloseAllChildFrame();
    void CloseChildFrame(KTarget* pTarget);
	/**
	* @brief Target year에 해당하는 Network View를 표출한다.
	* @param[in] pTarget - Target year
	*/
	void ShowNetworkView( KTarget* pTarget );
	
	/**
	* @brief Target year에 Table 명에 해당하는 IO Table View를 표출한다.
	* @param[in] pTarget - Target year
	* @param[in] strTableName - IO Table 명칭
	*/
	KIOTableView* ShowTableView( KTarget* pTarget, CString strTableName );
	void CloseTableView( KTarget* pTarget, CString strTableName );

	/**
	* @brief Child Frame을 닫는다.
	* @param[in] nID - Target year의 ID
	*/
	void TargetChildFrameClosedNotify( unsigned int nID );
protected :
	void ShowDockingPanePopupMenu(CXTPDockingPane* pPopupPane, CPoint pt, LPRECT lprcExclude);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:    
	CString                m_strStylesPath;  

	CXTPTabClientWnd       m_xtpTabClientWnd;	
    // Docking Pane
    CXTPDockingPaneManager m_xtpDockingPaneManager;
		KProjectExplorerWnd	   m_wndProjectExplorer;
		KNaviExplorerWnd       m_wndNaviExplorer;
    CXTPStatusBar          m_xtpStatusBar;

	/* scenario - child frame mapping table */
	std::map<unsigned int, CChildFrame*> m_mapTargetChildFrame;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg int  OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
    afx_msg void OnProjectExplorerWindowShow();
	afx_msg void OnNaviExplorerWindowShow();
    afx_msg void OnCommandWindowShow();

	///// start test code
	afx_msg void OnMapOpenmap();
	afx_msg void OnTableOpentable();
	afx_msg void OnTableClosetable();
	afx_msg void OnMapClosemap();
	afx_msg void OnProjectviewCreatetree();
	///// end test code
public:
	void SetPaneTextMapCoordinate(CString a_strMapCoordinate)
	{
		m_xtpStatusBar.SetPaneText(1, a_strMapCoordinate);
	}

	void SetPaneTextMapScale(CString a_strMapScale)
	{
		m_xtpStatusBar.SetPaneText(2, a_strMapScale);
	}

	void SetPaneTextInfo(CString a_strInformation)
	{
		m_xtpStatusBar.SetPaneText(3, a_strInformation);
	}
private:
    KTarget* m_pPolicyTarget;
    int      m_nPolicyCommand;
private:
    static unsigned __stdcall PolicyThreadCaller(void* p); 
private:
    CString PolicyInitialData(KTarget* pTarget, QBicSimpleProgressPtr spSimpleProgress);
    // 정책분석모형
    // 1)화물정책분석모형
    afx_msg void OnFPolicyRailConv();
	afx_msg void OnFPolicySOCConstruction();
	afx_msg void OnFPolicyContainer();
    public: void PolicyStart();

            // 2)여객정책분석모형
            afx_msg void OnApplicationRoadConstruction();
            afx_msg void OnApplicationEvent();
            afx_msg void OnApplicationTransitComplexConstruction();
            afx_msg void OnApplicationSkipandStopAnalysis();
			afx_msg void OnApplicationBusOnlyLane();

public:
	afx_msg void OnDevmenuRenderproperty();
	afx_msg void OnDevmenuCodemanager();
	afx_msg void OnMenuModelGeneration();
	afx_msg void OnMenuModelDistribution();
	afx_msg void OnMenuModelMode();
	afx_msg void OnMenuModelAssignment();
	afx_msg void OnDbmanagementCodemanagement();
	afx_msg void OnDBManagementSystemCodeManagement();
	afx_msg void OnTranstiCostManagement();

	afx_msg void OnUpdateDbmanagementCodemanagement(CCmdUI *pCmdUI);
    afx_msg void OnToolsPaSum();
    afx_msg void OnDataInspectionSed();
    afx_msg void OnOdOD2PASum();
    afx_msg void OnToolsCalculateTrafficVol();
    afx_msg void OnInfluenceOff();
    afx_msg void OnInfluenceStatistics();
    afx_msg void OnFreightLogisticModel();
    afx_msg void OnFreightVehicleConvert();
	
    afx_msg void OnArcgistestStringchecker();
	afx_msg void OnToolsIRIORatioInput();
	afx_msg void OnToolsRatioRegion();
	afx_msg void OnGravityCalibration();
	afx_msg void OnFCalculatePathFindInfo();
	afx_msg void OnFCalculateTimeCostInfo();

	afx_msg void OnResultViewDistribution();
	afx_msg void OnResultViewMode();
	afx_msg void OnResultViewAssignment();
	afx_msg void OnLogViewAssignment();

	afx_msg void OnMenuDevPlanManager();
	afx_msg void OnMenuDevPlan();

	afx_msg void OnMenuTimeODGeneration();
	afx_msg void OnMenuPATOOD();

	afx_msg void OnNetworkOnOff();
	afx_msg void OnResultviewModeod();
	afx_msg void OnResultviewVehicleod();

	afx_msg void OnImportTable();
    afx_msg void OnUpdateImportTable(CCmdUI *pCmdUI);
    afx_msg void OnExportTable();
    afx_msg void OnUpdateExportTable(CCmdUI *pCmdUI);

	afx_msg void OnFImportTable();

	afx_msg void OnEmmeNetworkImportTable();
	afx_msg void OnEmmeNetworkExportTable();
	afx_msg void OnUpdateEmmeNetworkExportTable(CCmdUI *pCmdUI);
	afx_msg void OnEmmeMatrixImportTable();
	afx_msg void OnEmmeMatrixExportTable();
	afx_msg void OnUpdateEmmeMatrixExportTable(CCmdUI *pCmdUI);

	afx_msg void OnMultiModalSetAccessibility();
	afx_msg void OnMultiModalGenerationBusPath();
	afx_msg void OnMultiModalModel();
	afx_msg void OnUpdateMultiModalModel(CCmdUI *pCmdUI);

	afx_msg void OnTableManagerVector();
	afx_msg void OnTableManagerMatrix();

	afx_msg void OnToolsODParameter();

	afx_msg void OnDefineRegionalMode();
	afx_msg void OnInterAccessTerminalGenerator();
    
	afx_msg void OnInterPathGenerator();
	afx_msg void OnInterModalMain();
    afx_msg void OnApplicationRoadimprovement();
	afx_msg void OnApplicationCO2Interpolation(void);
	afx_msg void OnApplicationCO2Emission(void);

	afx_msg void OnDefaultAccessStationSetting();
	afx_msg void OnDefaultPathGenerateConstraint();
	afx_msg void OnDefaultMultimodalPathGenerator();
	afx_msg void OnDefaultModeChoicePara();
	afx_msg void OnDefaultTransitRouteChoicepara();
    afx_msg void OnDefaultHighwayAssignment();
	afx_msg void OnDefaultAccessTerminalSetting();
	afx_msg void OnDefaultInterModeChoicePara();
	afx_msg void OnDefaultInterPathGeneratorPara();
	afx_msg void OnDefaultRegionalModeChoicePara(void);
	afx_msg void OnDefaultDistributionPara();
	afx_msg void OnDefaultInterRunOption();

	afx_msg void OnDefaultGenerationPara();

	afx_msg void OnFDefaultGenerationModelPara(void);
	afx_msg void OnFDefaultModeChoiceModelPara(void);
	afx_msg void OnFDefaultLogisticModelPara(void);
	afx_msg void OnFDefaultVehicleConvModelPara(void);
	afx_msg void OnFDefaultCalTimeCostPara(void);
	afx_msg void OnDefaultModeChoiceModelDefineVariable();
	afx_msg void OnDefaultCO2Interpolation(void);
	afx_msg void OnToolsEmissionsInterpolation(void);
	afx_msg void OnDefaultCO2Equation(void);
	afx_msg void OnToolsEmissionsEquation(void);

private:
    afx_msg void OnZoneGroupGeneration();
    afx_msg void OnOdGroupGeneration();
public:
	afx_msg void OnOdGroupGenerationPart2();

private:
	afx_msg void OnFModelProcess();

	afx_msg void OnNetShapeExport(void);
	afx_msg void OnUpdateNetShapeExport(CCmdUI *pCmdUI);
	afx_msg void OnNetShapeNodeImport();
	afx_msg void OnNetShapeLinkImport();
	afx_msg void OnNetShapeZoneImport();

private:
	afx_msg void OnLogHighwayModel();
	afx_msg void OnUpdateLogHighwayModel(CCmdUI *pCmdUI);
	afx_msg void OnLogMultiModalModel();
	afx_msg void OnUpdateLogMultiModalModel(CCmdUI *pCmdUI);
	afx_msg void OnLogInterModalModel();
	afx_msg void OnUpdateLogInterModalModel(CCmdUI *pCmdUI);
	
	//MapView
	afx_msg void OnToolsScreenline();
	afx_msg void OnToolsCordonline();
	afx_msg void OnToolsScreenCordonLineAnalysis();
	afx_msg void OnUpdateToolsScreenline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsCordonline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInfluenceCircle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInfluenceEnvelope(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInfluencePolygon(CCmdUI *pCmdUI);

	afx_msg void OnSymbologyThemeNode();
    afx_msg void OnUpdateSymbologyThemeNode(CCmdUI *pCmdUI);

	afx_msg void OnClearThemeNode();
	afx_msg void OnSymbologyThemeLink();
    afx_msg void OnUpdateSymbologyThemeLink(CCmdUI *pCmdUI);
    
    afx_msg void OnClearThemeLink();
	afx_msg void OnSymbologyThemeZone();
	afx_msg void OnClearThemeZone();
	afx_msg void OnDesireLineZone();
	afx_msg void OnDesireLineClearZone();

	afx_msg void OnShortestpathHighway();
	afx_msg void OnShortestpathTransit();
	afx_msg void OnUpdateShortestpathTransit(CCmdUI *pCmdUI);
	afx_msg void OnFShortestpathHighway();
	afx_msg void OnFShortestpathRail();
	afx_msg void OnFShortestpathViaFacilities();
	afx_msg void OnFShortestpathTotal();
	afx_msg void OnFLinkModeODVolume();

	afx_msg void OnPathAnalysis();
	afx_msg void OnPathVolumeHighwayAreaToArea();
	afx_msg void OnPathVolumeTransitAreaToArea();
	afx_msg void OnPathVolumeInterModeAreaToArea();
	afx_msg void OnPathVolumeInterAreaToArea();
	afx_msg void OnUpdatePathVolumeInterAreaToArea(CCmdUI *pCmdUI);
	afx_msg void OnLinkAnalysis();

	afx_msg void OnInspectionNetworkNode();
	afx_msg void OnInspectionNetworkLink();
	afx_msg void OnInspectionNetworkZone();
	afx_msg void OnInspectionNetworkTurn();
	afx_msg void OnDataInspectionTransit();

	afx_msg void OnNetworkInspectionRun();
	afx_msg void OnUpdateNetworkInspectionRun(CCmdUI *pCmdUI);
	afx_msg void OnNetworkInspectionHighwayError(); 
	afx_msg void OnNetworkInspectionHighwayWarning(); 
	afx_msg void OnNetworkInspectionTransitError();
	afx_msg void OnNetworkInspectionTransitWarning();
	afx_msg void OnNetworkInspectionViewLog();
	afx_msg void OnUpdateNetworkInspectionHighwayError(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNetworkInspectionHighwayWarning(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNetworkInspectionTransitError(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNetworkInspectionTransitWarning(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNetworkInspectionViewLog(CCmdUI *pCmdUI); 

	afx_msg void OnScreenCordonManage();
	afx_msg void OnUpdateScreenCordonManage(CCmdUI *pCmdUI);

	afx_msg void OnZoningDivision();
	afx_msg void OnZoningIntegration();
    	
	afx_msg void OnPrepareChartNetworkZone();
	afx_msg void OnPrepareChartNetworkNode();
	afx_msg void OnPrepareChartNetworkLink();

	afx_msg void OnPrepareChartPC();
    afx_msg void OnUpdatePrepareChartPC(CCmdUI *pCmdUI);

    afx_msg void OnPrepareChartOD();
    afx_msg void OnUpdatePrepareChartOD(CCmdUI *pCmdUI);

	afx_msg void OnAccessibilityModify();
	afx_msg void OnMultiModalAssignResult();
	afx_msg void OnUpdateMultiModalAssignResult(CCmdUI *pCmdUI);
	afx_msg void OnMultiModalTransitPathEdit();

	afx_msg void OnInterModalAssignResult();
	afx_msg void OnUpdateInterModalAssignResult(CCmdUI *pCmdUI);
	afx_msg void OnPathVolumeTerminalAccessEgress();
	afx_msg void OnUpdatePathVolumeTerminalAccessEgress(CCmdUI *pCmdUI);

	afx_msg void OnNetworkEditNode();
    afx_msg void OnUpdateNetworkeditNode(CCmdUI *pCmdUI);

	afx_msg void OnNetworkEditLink();
	afx_msg void OnNetworkEditTurn();
	afx_msg void OnNetworkEditTransit();

	afx_msg void OnMatrixPurposeOD();
	afx_msg void OnMatrixModeOD();
	afx_msg void OnMatrixChainOD();
	afx_msg void OnMatrixVehicleModeOD();

	afx_msg void OnInterModalTargetRegionSetting();
	afx_msg void OnInterModalAccessTerminalEditor();
	afx_msg void OnInterModalGlobalTerminalSetting();
	afx_msg void OnInterModalPathEdit();
	afx_msg void OnInterModalTerminalDesireLine();
	afx_msg void OnTerminalScheduleEdit();

	afx_msg void OnMenuToolsSubNetWork(void);
    afx_msg void OnUpdateMenuToolsSubNetWork(CCmdUI *pCmdUI);

	afx_msg void OnNetworkeditEditlog();
	afx_msg void OnUpdateNetworkeditEditlog(CCmdUI *pCmdUI);
	afx_msg void OnHelpNombara();
	afx_msg void OnNombaraTesterA();
	afx_msg void OnNombaraTesterB();

	afx_msg void OnModelsProcessmultiModal();
	afx_msg void OnModelsProcessInterModal();

	afx_msg void OnZoneODGroupGeneration();
	afx_msg void OnViewTurnVolume();
	afx_msg void OnThemesTerminalAEModeViewer();
	afx_msg void OnUpdateThemesTerminalAEModeViewer(CCmdUI *pCmdUI);

	afx_msg void OnFDefaultPolicyContainer();

	afx_msg void OnUpdateFrameTitle(BOOL bAddToTitle);

	afx_msg void OnTransitLineThemes();
    afx_msg void OnUpdateTransitTransitlineThemes(CCmdUI *pCmdUI);

    afx_msg void OnTransitChart();
    afx_msg void OnUpdateTransitChart(CCmdUI *pCmdUI);

private:
    afx_msg void On_PATH_MULTIMODAL   ();
    afx_msg void OnUpdate_PATH_MULTIMODAL(CCmdUI *pCmdUI);
    afx_msg void On_PATH_INTERMODAL   ();
    afx_msg void OnUpdate_PATH_INTERMODAL(CCmdUI *pCmdUI);
    afx_msg void On_BUS_MULTIMODAL    ();
    afx_msg void OnUpdate_BUS_MULTIMODAL(CCmdUI *pCmdUI);
    afx_msg void On_BUS_INTERMODAL    ();
    afx_msg void OnUpdate_BUS_INTERMODAL(CCmdUI *pCmdUI);
    afx_msg void On_STATION_MULTIMODAL();
    afx_msg void OnUpdate_STATION_MULTIMODAL(CCmdUI *pCmdUI);
    afx_msg void On_STATION_INTERMODAL();
    afx_msg void OnUpdate_STATION_INTERMODAL(CCmdUI *pCmdUI);
    afx_msg void On_URBAN_MULTIMODAL  ();
    afx_msg void OnUpdate_URBAN_MULTIMODAL(CCmdUI *pCmdUI);
    afx_msg void On_URBAN_INTERMODAL  ();
    afx_msg void OnUpdate_URBAN_INTERMODAL(CCmdUI *pCmdUI);

    afx_msg void OnEXE_ICBOUNDVOL_ANALYSIS();
    afx_msg void OnUpdateEXE_ICBOUNDVOL_ANALYSIS(CCmdUI *pCmdUI);
    afx_msg void OnEXE_ICBOUNDVOLBAT_ANALYSIS();
    afx_msg void OnUpdateEXE_ICBOUNDVOLBAT_ANALYSIS(CCmdUI *pCmdUI);

	afx_msg void OnEXE_TRIPOD_TO_VEHICLEOD();
	afx_msg void OnUpdateEXE_TRIPOD_TO_VEHICLEOD(CCmdUI* pCmdUI);
	afx_msg void OnEXE_FREIGHTOD_DIVISION();
	afx_msg void OnUpdateEXE_FREIGHTOD_DIVISION(CCmdUI* pCmdUI);
public:
	afx_msg void OnEXE_ACCESSTRIPOD_TO_VEHICLEOD();
	afx_msg void OnUpdateEXE_ACCESSTRIPOD_TO_VEHICLEOD(CCmdUI* pCmdUI);
};
