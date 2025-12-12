#pragma once
#pragma region Include_Files
#include "resource.h"
#include "IMapView.h"

#include "KEMMapMode.h"
#include "IMapDisplayObserver.h"
#include "KMapEvents.h"
#include "ImChampMapSelectionReceiver.h"
#include "ImChampCommand.h"
#include "IMapFeedback.h"
#include "ShortestPathInfo.h"
#include "IncludeScreenDraw.h"
#include "IncludeScreenDrawEdit.h"
#include "IncludeShapeEdit.h"
#include "TDrawLinkInfo.h"
#include "TDrawNodeInfo.h"
#include "TDrawTransitInfo.h"
#include "TPathAnalysisInfo.h"
#include "ScreenDrawFlashLink.h"
#include "KEMLayer.h"
#include "TTransitPath.h"
#include "MapViewSelectionFeedback.h"
#include "MapViewRenderInfo.h"
#include "intersection_common.h"
#include "KLayerID.h"

class KShortestPathHighwayDlg;
class KShortestPathNTransitDlg;
class KPathVolumeDlg;
class KPathVolumeAreaDlg;
class KLinkAnalysisDlg;
class KZoningDivisionDlg;
class KZoningIntegrationDlg;
class KSCManageDlg;
class KInspectionNodeDlg;
class KInspectionLinkDlg;
class KInspectionZoneDlg;
class KInspectionTurnDlg;
class KInspectionTransitDlg;
class KSymbologyThemeNodeDlg;
class KSymbologyThemeLinkDlg;
class KSymbologyThemeZoneDlg;
class KDesireLineDlg;
class KAccessiblityModifyDlg;
class KPrepareChartODDlg;
class KPrepareChartNetworkDlg;
class KPrepareChartPADlg;
class KMultimodalResultDlg;
class KFShortPathViaFacilitiesDlg;

class KTransitLineThemesDlg;
class KMapFindDlg;
class KMTransitPathEditDlg;
class KLinkViaTransitSearchDlg;
class KNodeViaTransitSearchDlg;
class KDlgTransitInfluenceArea;

class KNetworkInspectionDlg;
class KTargetRegionSetting;
class KODMatrixDlg;
class KAccessTermianlEditerDlg;
class KAccessNodeEditorDlg;
class KGlobalTerminalSettingDlg;
class KInterTransitPathEditDlg;
class KTerminalScheduleDesireDlg;
class KSubNetworkDlg;
class KTerminalScheduleEditDlg;
class KIntersectionTopologyAnalysisDlg;
class KModelProcessMultiModal;
class KZoneODGroupGenerationDlg;
class KFLinkModeODVolumeDlg;
class KTurnVolumeViewDlg;
class KModelProcessInterModal;
class KRulerDlg;
class KPathVolumeAreaTransitDlg;
class KPathVolumeAreaInterModeDlg;
class KTerminalAEModeViewerDlg;
class KInterModalResultDlg;
class KPathVolumeAreaInterDlg;
class KFDefaultPolicyContainerDlg;
class KStationODCalculatorDlg;
class KPathVolumeTerminalAccEgrDlg;

class KDlgNetworkEditorMain;

#pragma endregion Include_Files

#pragma region Forward_Declaration
class KProject;
class KTarget;
class KPersonalGeoDatabase;
class KMapFrameWnd;
class KIOTable;
class KIOView;
class KIOTableView; 
class KIOColumns;
class KXmlManager;
class KTocFormView;
class TDesireMapData;
class TNodeSelectionInfo;
class TCoordinate;
#pragma endregion Forward_Declaration

//★ 
//////////////////////////////////////////////////////////////////
typedef QBicM2Key<int, CString> TypeLabel;

class KMapView : public CFormView, public IMapDisplayObserver, 
                 public IMapView,  public ImChampCommandReceiver
{	
	DECLARE_DYNCREATE(KMapView)
	DECLARE_MESSAGE_MAP()
	//DECLARE_EVENTSINK_MAP()
	enum { 
        IDD = IDD_3000_MapView   
	};
protected:
	KMapView(Gdiplus::Color m_crBackground = Gdiplus::Color::White);
	virtual ~KMapView();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
    virtual void OnInitialUpdate();    
            void InitalizeMap();
    virtual void PostNcDestroy();
    afx_msg void OnDestroy();
private:
    ThunderMapCtrl m_ThunderMapCtrl;
protected:
    virtual ThunderMapCtrl* GetThunderMapCtrl() {return &m_ThunderMapCtrl;}
public:
    virtual ThunderMapPtr GetThunderMap() {
        return m_ThunderMapCtrl.GetThunderMap();
    }

public:
    virtual void MapFeedback(int a_nBusinessFeatureType, std::vector<__int64>){};
    virtual void MapFeedback(int a_nBusinessFeatureType, __int64) {};

#ifdef _DEBUG
    virtual void AssertValid() const;
    #ifndef _WIN32_WCE
        virtual void Dump(CDumpContext& dc) const;
    #endif
#endif   

private:
	short TasESRICacheID;
private:
    void CloseModelessDialog();
public:
	void ActivatedFrameWndNotifyProcess();
	void DeActivatedFrameWndNotifyProcess();
private:
    KMapViewSelectionFeedbackPtr m_spMapViewSelectionFeedback;

#pragma region IMapDisplayObserver     /*지도의 위치, 스케일 동기화 인터페이스 구현*/
protected :
    virtual KEMMapDisplaySync GetMapSyncMode()  {
        return m_emMapDisplaySync;
    }

    virtual void   MapPositionSyncMessage     (double a_dMapCenterPointX, double a_dMapCenterPointY);
	virtual void   MapPositionScaleSyncMessage(TxEnvelope oEnv);
    
    virtual double GetMapScale();
    virtual TxEnvelope GetMapEnveolpe();
    virtual void   GetMapCenterPoint(double& a_dMapCenterPointX, double& a_dMapCenterPointY);
#pragma endregion IMapDisplayObserver

#pragma region MapView_cpp             /*공통정의*/
public:
	inline KEMMapMode GetMapMode( void )
	{
		return m_emMapMode;
	}
 	inline KTarget* GetTarget( void )
 	{
 		return m_pTarget;
 	}

 	void LoadMapView( KTarget* pTarget );
	virtual KPersonalGeoDatabase* GetPersonalGeoDataBase( void ) const
	{
		return m_pPersonalGeoDataBase;
	}

	virtual CWnd* GetMapWindow() const
	{
		return (CWnd*)this;
	}
	void SetMapMode( KEMMapMode emMapMode );
	virtual void SetMapCursor(WORD a_wCursorID);

private:
    void LoadInitialLayers(bool bFirst=true);
public:

private:
    afx_msg void OnMapSetCursor    (NMHDR* pNMHDR, LRESULT* pResult);
private:
    afx_msg void OnMapLayerDrawEnd (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMapMouseMove    (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMapLButtonDown  (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMapLButtonUp    (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMapLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnMapRButtonDown  (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMapRButtonUp    (NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnMapMButtonDown  (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMapMButtonUp    (NMHDR* pNMHDR, LRESULT* pResult);
private:
    // Mouse Point
    int    m_nMouseDownX,    m_nMouseDownY;
    double m_dMouseDownMapX, m_dMouseDownMapY;
protected :    
    
public:
    afx_msg void OnMapviewFullExtent( void );  
    // 확대, 축소, 이동
	afx_msg void OnMapviewZoomInCommand();
	afx_msg void OnUpdateMapviewZoomInCommand(CCmdUI* pCmd);

	afx_msg void OnMapviewZoomOutCommand();
	afx_msg void OnUpdateMapviewZoomOutCommand(CCmdUI* pCmd);

	afx_msg void OnMapviewPanCommand();
	afx_msg void OnUpdateMapviewPanCommand(CCmdUI* pCmd);

    afx_msg void OnMapviewCommand( UINT nID );
    afx_msg void OnUpdateMapviewCommand( CCmdUI* pCmd );

    // 이전, 이후
    afx_msg void OnMapExtentStack( UINT nID );
    afx_msg void OnUpdateMapExtentStack( CCmdUI* pCmdUI );
    
    
public:
	void FullExtentMap( void );
	void ExtentToFeautre(CString a_strTableName, std::vector<Integer> a_vecNodes, double dx=5000.0, double dy=5000.0);
	    
    void ZoomExtent  (double  a_dMinX,  double   a_dMinY, double  a_dMaxX,  double   a_dMaxY);
	
    void GetCurrentExtent(double& ar_dMinX, double& ar_dMinY, double& ar_dMaxX, double&  ar_dMaxY);
	void GetCurrentExtent(TxEnvelope &ar_oEnvelope);
    void GetFullExtent   (double& ar_dMinX, double& ar_dMinY, double& ar_dMaxX, double&  ar_dMaxY);

private:
    double    m_dLinkFlashOffset;
    void      CtrlFind   ();
    
public:
	void ShowDesireThemes(std::vector<TDesireMapData> &a_vecDesireMapData, bool a_bShowLabel = true);
	void ClearDesireThemes();
	void ShowDesireLabel(bool a_bShowLabel = true);
private:    
public:
	afx_msg void OnRuler();

#pragma region MapViewCommandReceiver
public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled    (                   );
#pragma endregion MapViewCommandReceiver

public :
	void SetIOView(KIOView* pIOView);
	void NotifyIOTableViewCreated(LPCTSTR strTableName);
	void NotifyIOTableViewClosed(LPCTSTR strTableName);
	void NotifyIOTableViewActivated(LPCTSTR strTableName);

protected:
	void AdjustViewLayout(void);  	
	void ShowPopupMenuNetworkEdit( long X, long Y );	
	KIOTableView* findRegisteredIOView(LPCTSTR strTableName);
	void ShowPopupMenuTransit(int x, int y);		
	void ShowMapView(bool bShow = true);
    
#pragma region OnInitialUpdate
protected :
    KIOView*              m_pIOView;
#pragma endregion OnInitialUpdate    

protected :
    std::map<CString, KIOTableView*> m_mapRegisteredTableView;
	HCURSOR				  m_hCursor;	
	KEMMapMode			  m_emMapMode;
	KEMMapMode			  m_emPrevMapMode;	/// 이전 Map Mode를 저장
    KEMMapDisplaySync     m_emMapDisplaySync;
    bool                  m_bReceivedDisplaySyncMessage;
	KTarget*			  m_pTarget;
	KPersonalGeoDatabase* m_pPersonalGeoDataBase;  	
	KTocFormView*         m_pTocFormView;
	TNodeLabelInfo        m_TNodeLabelInfo;
	TLinkLabelInfo        m_TLinkLabelInfo;
public:
    void SetBuddyTocFormView(KTocFormView* a_pTocFormView) {
        m_pTocFormView = a_pTocFormView;
    }

protected:	
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);       

    // 동기화(None, 위치, 위치+Scale)
    afx_msg void OnMapviewSyncCommand( UINT nID );
    afx_msg void OnUpdateMapviewSyncCommand( CCmdUI* pCmdUI );
	
	afx_msg void OnIdentifyTargetChanged( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnUpdateIdentifyTarget( CCmdUI* pCmdUI );

	afx_msg void OnMapEditOperationCommand( UINT nID );
	afx_msg void OnUpdateMapEditOperationCommand( CCmdUI* pCmd );
	afx_msg void OnEditTarget( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnUpdateEditTarget( CCmdUI* pCmdUI );
	afx_msg void OnEditTask( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnUpdateEditTask( CCmdUI* pCmdUI );	
	// 편집시작
public:
	KShapeEditControllerPtr ShapeEditStart();	
    void StopShapeEdit();
public:	
	afx_msg void OnMapShapeEditStart( void );
	afx_msg void OnUpdateOnMapShapeEditStart( CCmdUI* pCmdUI );

	// 편집종료
	afx_msg void OnMapShapeEditEnd( void );
	afx_msg void OnUpdateOnMapShapeEditEnd( CCmdUI* pCmdUI );

    afx_msg void OnMapCapture();

    // 속성보기 - MapViewIdentify.cpp
	afx_msg void OnIdentifyCommand( void );
	afx_msg void OnUpdateIdentifyCommand( CCmdUI* pCmdUI );

    // 영향권 설정 - MapViewInfluence.cpp
    afx_msg void OnInfluenceEnvelope();
    afx_msg void OnInfluenceCircle();
    afx_msg void OnInfluencePolygon();
    
	afx_msg void OnPopupMapeditEditproperties();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMapSelectionSearchPolygon( /*NMHDR* pNMHDR, LRESULT* pResult*/ );
	afx_msg void OnUpdateMapSelectionSearchPolygon( CCmdUI* pCmdUI );

    afx_msg void OnMapSelectionSearchRect();
    afx_msg void OnUpdateMapSelectionSearchRect( CCmdUI* pCmdUI );

	afx_msg void OnSelectedClearMapView();	
	afx_msg void OnUpdateSelectedClearMapView( CCmdUI* pCmdUI );

    afx_msg void OnMapViewPasteAttribute();	
    afx_msg void OnUpdateMapViewPasteAttribute( CCmdUI* pCmdUI );

	afx_msg void OnPopupMapeditInsertvertex();
    
protected:
    afx_msg void OnMapviewMenuSearchTransitByLink();
public:
    void SearchTransitByLink(bool bOption = false);
    void SearchTransitByNode(bool bOption = false);
public:
	void ShowTerminalScheduleEditView();

private:
    double m_dExpandMinX, m_dExpandMinY, m_dExpandMaxX, m_dExpandMaxY;
private:
public:
    virtual double GetPixelToRealDist( double a_dNoOfPixel );

	virtual void   TasESRICacheRefresh();
    virtual void   RefreshAll();
    
    virtual double GetRequestMapScale ();
    virtual bool   NodeLayerVisible   () ;
    virtual bool   LinkLayerVisible   () ;
    virtual bool   TransitVisible     ();
	virtual bool   TurnVisible        ();
	virtual bool   IntersectionVisible();
    virtual KCommandWnd*      GetCommandView();
    
	virtual short GetCacheID();
    virtual void ShapeDeletedItem (CString a_strLayer, Integer a_nxID);
    virtual void ShapeEditedItem  (CString a_strLayer, Integer a_nxID);

    virtual void NotifyNetworkEdior(int nEditType/*0:노드추가(노드), 1:노드추가(링크), 5:버스노선추가*/, Integer a_nxID);
    virtual void NotifyNetworkEdior(int nEditType/*0:예약, 1:링크추가*/, std::vector<__int64>& vecID);
// 경로탐색, 노드 검색 - MapViewShortestPath.cpp
private:
    IMapNodeSearch* m_pMapNodeSearch;
private:
    void SelectNodePoint( long shift, long x, long y, double mapX, double mapY );
public:
    void SelectNodePoint(IMapNodeSearch* a_pMapNodeSeach);
    void CloseMapNodeSearch();
    // Highway 최단 경로
    void DrawShortestPath( std::vector<TPathInfo> &a_vecPath, bool a_bAutoOffset = false );
    void ClearShortestPath();
    // Transit 최단 경로
    void DrawTransitShortestPath(std::vector<TTransitRouteInfo> &a_vecRouteInfo);
    void ClearTransitShortestPath();

    // Link 주제도
    void DrawLinkThemes(std::vector<TDrawLinkInfo>& a_vecLinkInfo, bool a_bLabel=false, bool a_bRefresh = true);
    void SetLinkThemesLabel(bool a_bLabel=true, bool a_bRefresh = true);
    void ClearLinkThemes(bool a_bRefresh = true);

	// Node 주제도
	void DrawNodeThemes(std::vector<TDrawNodeInfo>& a_vecNodeInfo, bool a_bLabel=false, bool a_bRefresh = true);
	void SetNodeThemesLabel(bool a_bLabel=true, bool a_bRefresh = true);
	void ClearNodeThemes(bool bRefresh = true);
	
	// Zone 주제도
	void DrawZoneThemes(std::vector<TDrawZoneInfo>& a_vecZoneInfo, bool a_bLabel=false, bool a_bRefresh = true);
	void SetZoneThemesLabel(bool a_bLabel=true, bool a_bRefresh = true);
	void ClearZoneThemes();

    // 현재 지도 모드
	KEMMapMode GetCurrentMapMode();
    
public:
    virtual void AddTurnEdit(__int64 nxTurnID, bool bRefresh=true);
    void AddTurnMapDraw    (Integer a_nxTurnID, bool a_bRefresh = true);
    void RemoveTurnMapDraw (Integer a_nxTurnID, bool a_bRefresh = true);
    void RemoveTurnMapDraw (bool a_bRefresh = true);
	void AddIntersectionMapDraw    (Integer a_nxIntersectionID, bool a_bRefresh = true);
	void RemoveIntersectionMapDraw (Integer a_nxIntersectionID, bool a_bRefresh = true);
    void ClearIntersectionMapDraw();
public:
    // ScreenDraw Transit Display
    void ZoomTransitExtent         (Integer a_nxTransitID);
    void ZoomTransitFirst          (Integer a_nxTransitID);
    void ZoomTransitLast           (Integer a_nxTransitID);
	void RefreshTransitMapDraw     ();
    void RefreshTransitMapDraw     (Integer a_nxTransitID);
	void AddTransitMapDraw         (Integer a_nxTransitID);
    void RemoveTransitMapDraw      (Integer a_nxTransitID);
private:
    // ScreenDraw Transit Display
    void DrawTransit               ();
    void AddTransitDraw            (TTransitInfo a_TransitInfo );
    void UpdateTransitDraw         (TTransitInfo a_TransitInfo);
    void RemoveTransitDraw         (Integer      a_nxTransitID);
public:
    void ClearTransit              (bool a_bRefresh = true);

public:
    void ClearScreenLineDraw       (bool a_bRefresh = true);
    void AddScreenLineDraw         (Integer      a_nxID,        COLORREF  a_rgbColor = RGB(255, 0, 0), bool a_bRefresh = true);
    void RemoveScreenLineDraw      (Integer      a_nxID,        bool a_bRefresh = true);
	void UpdateScreenLineDraw      (Integer      a_nxID,        COLORREF  a_rgbColor = RGB(255, 0, 0), bool a_bRefresh = true);

    void ClearCordonLineDraw       (bool a_bRefresh = true);
    void AddCordonLineDraw         (Integer      a_nxID,        COLORREF  a_rgbColor = RGB(255, 0, 0), bool a_bRefresh = true);
    void RemoveCordonLineDraw      (Integer      a_nxID,        bool a_bRefresh = true);
	void UpdateCordonLineDraw      (Integer      a_nxID,        COLORREF  a_rgbColor = RGB(255, 0, 0), bool a_bRefresh = true);
public:
    // 존 노드를 강조
    void ScreenDrawZoneNodeAccent();
    void ClearZoneNodeAccent();

#pragma region 경로분석
	#pragma region 존간PathVolume
	public:
		void ClearPathVolume();
		void DrawPathVolume(int a_nType/*0:EachPath, 1:Goguma*/, std::vector<TPathVolumeInfo>& a_rTPathVolumeInfo);
	#pragma endregion 존간PathVolume
	#pragma region 링크경유PathVolume
	public:
		void SelectLinkPath(ImChampMapSelectionReceiver* a_pMapFeedbackLinkSelect);		
		void DrawLinkViaPathThemes(std::vector<TDrawLinkInfo>& a_vecLinkInfo,  std::vector<Integer>& a_vecSelectedLink, bool a_bDrawLabel = true);
		void SetDrawLabelLinkViaPathThemes(bool a_bDrawLabel=true);
		void ClearLinkViaPathThemes();
	#pragma endregion 링크경유PathVolume
#pragma endregion 경로분석
    public:
        void TxDrawCompositeNode          (std::vector<TDrawNodeInfo>& a_vecDrawNodeInfo, bool a_bDrawLabe=false, bool a_bRefresh=false, double a_dDrawScale=999999999.0);
        void TxDrawCompositeNodeLabel    (bool a_bDrawLabel=true, bool a_bRefresh=true);
        void TxDrawCompositeLink          (std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo, bool a_bDrawLabe=false, bool a_bRefresh=false, bool bSort = true, bool bLabelFill = false);
        void TxDrawShowCompositeLinkLablel    (bool a_bDrawLabel=true, bool a_bRefresh=true);
        void TxDrawCompositeUserLine      (std::vector<TUserLineInfo>& ar_vecUserLine,    bool a_bDrawLabe=false, bool a_bRefresh=false);
        void TxDrawShowCompositeUserLineLablel(bool a_bDrawLabel=true, bool a_bRefresh=true);

        void TxDrawCompositeUserPoint     (std::vector<TUserPointInfo>& ar_vecUserPoint,  bool a_bDrawLabe=false, bool a_bRefresh=false);
        void TxDrawShowCompositeUserPointLabel(bool a_bDrawLabel=true, bool a_bRefresh=true);

        void TxDrawCompositeZoneAccess (std::vector<TDrawZoneAccess>& ar_vecDrawZoneAccess, bool a_bRefresh=false, bool a_bDrawLabel=false);
        void TxClearCompositeThemes    (bool a_bRefresh=true);
public:
public:
		void CacheClearCompositeThemes       (bool a_bRefresh=false);	
		void CacheDrawCompositeLink          (std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo, bool a_bDrawLabe=false, bool a_bRefresh=false);
		void CacheShowCompositeLinkLabel     (bool a_bDrawLabel=true, bool a_bRefresh=true);
		void CacheDrawCompositeZoneAccess    (std::vector<TDrawZoneAccess>& ar_vecDrawZoneAccess, bool a_bRefresh/* =false */);
		void CacheDrawCompositeUserLine      (std::vector<TUserLineInfo>& ar_vecUserLine, bool a_bDrawLabel, bool a_bRefresh = false);
		void CacheShowCompositeUserLineLabel (bool a_bDrawLabel=true, bool a_bRefresh=true);
		void CacheDrawCompositeUserPoint     (std::vector<TUserPointInfo>& ar_vecUserPoint, bool a_bDrawLabel, bool a_bRefresh = false);
		void CacheDrawCompositeUserPointLabel(bool a_bDrawLabel=true, bool a_bRefresh=true);

public:
    void CacheDrawInterRegionalTransit(std::vector<TUserLineInfo>& ar_vecUserLine, std::vector<TUserPointInfo>& ar_vecUserPoint, bool a_bShowNodeLabel, bool a_bRefresh);
    void CacheClearInterRegionalTransit(bool a_bRefresh);
private:
	KThemeDrawControlPtr    m_spThemeControl;
    KScreenDrawEditPtr      m_spScreenDrawEdit;
	KShapeEditControllerPtr m_spShapeEditController;
public:	
	void ResetScreenDrawEdit()
	{
		if (m_spScreenDrawEdit != NULL)
		{
			m_spScreenDrawEdit.reset();
		}
        SetMapMode(KEMMapModePan);
	}
public:
    bool IsScreenDrawEditing()
    {
        if (m_spScreenDrawEdit != NULL)
        {
            if (m_spScreenDrawEdit->IsEditingStatus())
            {
                return true;
            }
        }

        return false;
    }
    bool IsShapeEditing()
    {
        if (m_spShapeEditController != NULL)
        {
            return true;
        }

        return false;
    }

	KShapeEditControllerPtr GetShapeEditController()
	{
		return m_spShapeEditController;
	}

private:
    Integer m_nxStartNodeID;
public:
    virtual void SetStartNodeID(Integer a_nxMaxNodeID);
    virtual Integer GetStartNodeID();
private:
    Integer m_nxStartLinkID;
public:
    virtual void SetStartLinkID(Integer a_nxMaxLinkID);
    virtual Integer GetStartLinkID();

public:
    virtual __int64 GetMaxNodeID();
    virtual __int64 GetMaxLinkID();
private:
	afx_msg void OnShapeEditPopupCommand( UINT nID );
public:
    void ClearGraphicContainer();
    void DrawBuildLinkGeometry(bool a_bRefresh = false); // 테스트 용  
    void Drawing();
    void DrawBuildLinkGeometry(std::vector<Integer>& a_vecLink, COLORREF a_colorPath = RGB(0, 255, 0), bool a_bRefresh = false, double a_dOffset=1.0, double a_dWidth=1.0);
    
    static unsigned __stdcall MapViewThreadTester(void* p); // 테스트 용
    
private:
    bool m_bLinkCartographicRenderer;
public:
    bool IsLinkCartographicRenderering() { return m_bLinkCartographicRenderer; }
public:
    void SendCommand();

public:
    bool IntersectionTopologyAnalysis();

private:
    IMapZoning* m_pMapZoning;
    void ZoningProcessSelectPoint( UINT nFlags, long X, long Y, double mapX, double mapY );
public:
    void ZoningProcessStart(IMapZoning* a_pMapZoning);
    void ZoningProcessClose();

public:
	void MapNodeSelectAction(ImChampMapSelectionReceiver* a_pMapSelectionReceiver, std::vector<TNodeSelectionInfo>& ar_vecTNodeStation, bool a_bLable = false, bool a_bRefresh = true, double a_dScale = 100000.0);
	void ClearScreenDrawEdit(bool a_bRefresh=true/*자동ForegroundRefresh*/);
private:
	QBicLoggerPtr m_spNetworkLogger;

public:
	virtual void NetworkEditLog(int a_nLayer, Integer a_nxID, int a_nEditType);

	void WriteNetworkLog(CString& ar_strLog)
	{
        //★ Tova-지원하지 않음 -------------------------------------
        /*★*/return; 
        //★ 
		if (m_spNetworkLogger == NULL)
		{
			m_spNetworkLogger = QBicLoggerPtr(new QBicLogger());			
			if (!m_spNetworkLogger->LoggerOpen(m_pTarget->GetLocation(), _T("network_edit_log"), _T("txt"), false, true))
			{
				m_spNetworkLogger.reset();
				return;
			}
		}
		try
		{
			m_spNetworkLogger->WriteLog(ar_strLog);
		}
		catch (...)
		{
		}		
	}
private:
	double  m_dMouseMoveMapX;
	double  m_dMouseMoveMapY;
    double  m_dPreTimerMapX;
    double  m_dPreTimerMapY;
    double  m_dTooltipMapX;
    double  m_dTooltipMapY;
    long    m_lMouseMoveMapX;
    long    m_lMouseMoveMapY;

	double  m_dCurrentPixelDist;
private:
	KEMLayer  m_emLayerIdentify;
    KEMLayer  m_emLayerSearch;
private:
	std::shared_ptr<KShortestPathHighwayDlg>  m_spShortestPathHighwayDlg;
	std::shared_ptr<KShortestPathNTransitDlg>    m_spShortestPathNTransitDlg;
	
	std::shared_ptr<KPathVolumeDlg>              m_spPathVolumeDlg;
	std::shared_ptr<KPathVolumeAreaDlg>          m_spPathVolumeAreaDlg;
	std::shared_ptr<KPathVolumeAreaTransitDlg>   m_spPathVolumeAreaTransitDlg;
	std::shared_ptr<KPathVolumeAreaInterModeDlg> m_spPathVolumeAreaInterModeDlg;
	std::shared_ptr<KPathVolumeAreaInterDlg>     m_spPathVolumeAreaInterDlg;
	std::shared_ptr<KLinkAnalysisDlg>            m_spLinkAnalysisDlg;
	std::shared_ptr<KZoningDivisionDlg>          m_spZoningDivisionDlg;
	std::shared_ptr<KZoningIntegrationDlg>       m_spZoningIntegrationDlg;
	std::shared_ptr<KSCManageDlg>                m_spSCManageDlg;
	std::shared_ptr<KInspectionNodeDlg>          m_spInspectionNodeDlg;
	std::shared_ptr<KInspectionLinkDlg>          m_spInspectionLinkDlg;
	std::shared_ptr<KInspectionZoneDlg>          m_spInspectionZoneDlg;
	std::shared_ptr<KInspectionTurnDlg>          m_spInspectionTurnDlg;
	std::shared_ptr<KInspectionTransitDlg>       m_spInspectionTransitDlg;
	std::shared_ptr<KSymbologyThemeNodeDlg>      m_spSymbologyThemeNodeDlg;
	std::shared_ptr<KSymbologyThemeLinkDlg>      m_spSymbologyThemeLinkDlg;
	std::shared_ptr<KSymbologyThemeZoneDlg>      m_spSymbologyThemeZoneDlg;
	std::shared_ptr<KDesireLineDlg>              m_spDesireLineDlg;
	std::shared_ptr<KPrepareChartODDlg>          m_spPrepareChartODDlg;
	std::shared_ptr<KPrepareChartNetworkDlg>        m_spPrepareChartNetworkDlg;
	std::shared_ptr<KPrepareChartPADlg>          m_spPrepareChartPADlg;
	std::shared_ptr<KMultimodalResultDlg>        m_spMultimodalResultDlg;
	std::shared_ptr<KInterModalResultDlg>        m_spIntermodalResultDlg;
    
	std::shared_ptr<KTransitLineThemesDlg>       m_spTransitLineThemesDlg;
	std::shared_ptr<KMapFindDlg>                 m_spMapFindDlg;
	std::shared_ptr<KMTransitPathEditDlg>        m_spMTransitPathEditDlg;
	std::shared_ptr<KAccessNodeEditorDlg>	   m_spAccessStopNodeEditorDlg;
    std::shared_ptr<KLinkViaTransitSearchDlg>    m_spLinkViaTransitSearchDlg;
    std::shared_ptr<KNodeViaTransitSearchDlg>    m_spNodeViaTransitSearchDlg;

    std::shared_ptr<KDlgTransitInfluenceArea>    m_spTransitInfluenceAreaDlg;

	std::shared_ptr<KNetworkInspectionDlg>       m_spNetworkInspectionDlg;
	std::shared_ptr<KTargetRegionSetting>		   m_spTargetRegionSettingDlg;
	std::shared_ptr<KODMatrixDlg>                m_spODMatrixDlg;
	std::shared_ptr<KAccessTermianlEditerDlg>	   m_spAccessTerminalEditerDlg;	
	std::shared_ptr<KGlobalTerminalSettingDlg>      m_spGlobalTerminalSettingDlg;
	std::shared_ptr<KInterTransitPathEditDlg>    m_spInterTransitPathEditDlg;
	std::shared_ptr<KTerminalScheduleDesireDlg>  m_spTerminalScheduleDesireDlg;
	std::shared_ptr<KSubNetworkDlg>              m_spSubNetworkDlg;
	std::shared_ptr<KTerminalScheduleEditDlg>    m_spTerminalScheduleEditDlg;
	std::shared_ptr<KIntersectionTopologyAnalysisDlg> m_spIntersectionTopologyAnalysisDlg;
	std::shared_ptr<KModelProcessMultiModal>     m_spModelProcessMultiModalDlg;
	std::shared_ptr<KModelProcessInterModal>	   m_spModelProcessInterModalDlg;
	std::shared_ptr<KZoneODGroupGenerationDlg>   m_spZoneODGroupGenerationDlg;
	std::shared_ptr<KTurnVolumeViewDlg>          m_spTurnVolumeViewDlg;
	std::shared_ptr<KRulerDlg>                   m_spRulerDlg;
	std::shared_ptr<KTerminalAEModeViewerDlg>    m_spTerminalAEModeViewerDlg;
	std::shared_ptr<KFDefaultPolicyContainerDlg>	m_spFDefPolicyContainer;
	std::shared_ptr<KPathVolumeTerminalAccEgrDlg>	m_spPathVolumeTerminalAccEgrDlg;
	std::shared_ptr<KStationODCalculatorDlg>		m_spStationODCalculatorDlg;

    std::shared_ptr<KDlgNetworkEditorMain> m_spDlgNetworkEdior;
public:
    void TransitLinkSelect(Integer a_nxLinkID=-1);
    void TransitNodeSelect(Integer a_nxNodeID=-1);

//★ 현재 지도의 노드, 링크 디스플레이 정보
private:
    KNodeLayerDispInfo m_oNodeDispInfo;
    KLinkLayerDispInfo m_oLinkLayerDispInfo;
public:
    virtual KNodeLayerDispInfo NodeLayerDispInfo() {
        return m_oNodeDispInfo;
    }

    virtual KLinkLayerDispInfo LinkLayerDispInfo() {
        return m_oLinkLayerDispInfo;
    }

//★ 현재 지도이 노드, 링크의 타입 심볼 정보 저장
private:
    std::map<int, ITxSymbolPtr> m_mapNodeTypeSymbol; 
    std::map<int, ITxSymbolPtr> m_mapLinkTypeSymbol; 
public:
    virtual ITxSymbolPtr NodeTypeSymbol(int nType) {
        auto iFind = m_mapNodeTypeSymbol.find(nType);
        if (iFind != m_mapNodeTypeSymbol.end())
            return iFind->second;

        return nullptr;
    } 
    virtual ITxSymbolPtr LinkTypeSymbol(int nType) {
        auto iFind = m_mapLinkTypeSymbol.find(nType);
        if (iFind != m_mapLinkTypeSymbol.end())
            return iFind->second;

        return nullptr;
    }
private:
    void ZoneSimpleRender(TZoneDispInfo&);
    void LinkSimpleRender(KLinkLayerDispInfo&);
    void LinkTypeRender  (KLinkLayerDispInfo&);
    void NodeTypeRender  (KNodeLayerDispInfo&);
    void NodeSimpleRender(KNodeLayerDispInfo&);
public:
    /* nType | 0:All, 1:Node, 2:Link, 3:Zone*/
    void ReloadRender(bool a_bRefresh=true, int nType = 0);

public:
    void TxDrawTransitBoardAlight(std::vector<TTransitPassengerInfo>& ar_vecDrawTransitPassengerInfo, bool a_bDrawLabel, bool a_bRefresh);
    void TxDrawShowTransitPassengerLabel(bool a_bDrawLabel=true, bool a_bRefresh=true);

public:
	void ShowTransitLineThemes( Integer a_nxTransitID );
	void ShowODMatrix(CString a_strTableName);
	void ShowTransitODCalculator(Integer a_nxTransitID);

    void TransitInfluenceArea();

public:
    void NodeAccent(std::vector<TDrawNodeInfo>& ar_vecDrawNodeInfo, bool a_bDrawLabe=false, bool a_bRefresh=false);
    void ClearNodeAccent(bool a_bRefresh);

    void LinkAccent(std::vector<TDrawLinkInfo>& ar_vecDrawLinkInfo, bool a_bDrawLabe=false, bool a_bRefresh=false);
    void ClearLinkAccent(bool a_bRefresh);
    
public:
	void MapFindDlgNext();
public:
    void AddSelectionNode(std::vector<Integer> ar_vecNode, bool a_bDrawLabel);
    void AddSelectionLink(std::vector<Integer> ar_vecLink, bool a_bDrawLabel);

private:
    std::vector<Integer>       m_vecSelectionNode;
    std::vector<Integer>       m_vecSelectionLink;
    std::map<CString, CString> m_mapNodeCopyKeyValue;
    std::map<CString, CString> m_mapLinkCopyKeyValue;
private:
    void PasteNode();
    void PasteLink();
public:
    std::map<CString, CString>& GetNodeCopyKeyValue()
    {
        return m_mapNodeCopyKeyValue;
    }
    std::map<CString, CString>& GetLinkCopyKeyValue()
    {
        return m_mapLinkCopyKeyValue;
    }
private:
    afx_msg void OnNetworkeditReplaceDemarcation();
public:
    bool DrawingLinkThemeState();

public:
    void NodeLayerSettingChanged();
public:
    void MapSelectionByPolygon(ImChampMapSelectionReceiver* a_pMapFeedback, int a_nActionType = 1, bool a_bIntersectDemarcation = true);
    void StopMapSelectionByPolygon();

public:
    void TargetRegionInsert(int a_nTargetRegionID, ImTasTargetRegionReceiver* a_pMapFeedback, KEMLayer a_emLayer = NODE);
    void TargetRegionEdit  (int a_nTargetRegionID, std::vector<TCoordinate> vecCoordinate, ImTasTargetRegionReceiver* a_pMapFeedback);
private:
    bool m_bUseCacheScreenDrawRullerLinkSelect;
public:
    void RulerSelectLink(ImChampMapSelectionReceiver* a_pMapFeedbackLinkSelect);
    void RulerSelectionLink(std::vector<TDrawLinkInfo>& ar_vecDrawLinkInfo, bool a_bDrawLabel=false, bool a_bRefresh=true );
    void ClearRulerSelection(bool a_bRefresh=true);

public:
    void ZoomMap(TxEnvelope& oEnv, bool bRefresh=false);

	//MainFrame 메뉴
public:
	void OnToolsScreenline();
	void OnToolsCordonline();
	void OnToolsScreenCordonLineAnalysis();
	void OnUpdateToolsScreenline(CCmdUI *pCmdUI);
	void OnUpdateToolsCordonline(CCmdUI *pCmdUI);

	void OnUpdateInfluenceCircle(CCmdUI *pCmdUI);
	void OnUpdateInfluenceEnvelope(CCmdUI *pCmdUI);
	void OnUpdateInfluencePolygon(CCmdUI *pCmdUI);

	void OnSymbologyThemeNode();
	void OnClearThemeNode();
		void ShowNodeThemes();

	void OnSymbologyThemeLink();
	void OnClearThemeLink();
		void ShowLinkThemes();

	void OnSymbologyThemeZone();
	void OnClearThemeZone();
	void ShowZoneThemes();

	void OnDesireLineZone();
	void OnDesireLineClearZone();

	void OnShortestpathHighway();
	void OnShortestpathTransit();
	void OnUpdateShortestpathTransit(CCmdUI *pCmdUI);
	void OnFShortestpathHighway();
	void OnFShortestpathRail();
	void OnFShortestpathViaFacilities();
	void OnFShortestpathTotal();
	void OnFLinkModeODVolume();

	void OnPathAnalysis();
	void OnPathVolumeHighwayAreaToArea();
	void OnPathVolumeTransitAreaToArea();
	void OnPathVolumeInterModeAreaToArea();
	void OnPathVolumeInterAreaToArea();
	void OnUpdatePathVolumeInterAreaToArea(CCmdUI *pCmdUI);
	void OnLinkAnalysis();

	void OnInspectionNetworkNode();
	void OnInspectionNetworkLink();
	void OnInspectionNetworkZone();
	void OnInspectionNetworkTurn();
	void OnDataInspectionTransit();


	void OnNetworkInspectionRun();
	void OnUpdateNetworkInspectionRun(CCmdUI *pCmdUI);
	void OnNetworkInspectionHighwayError(); 
	void OnNetworkInspectionHighwayWarning(); 
	void OnNetworkInspectionTransitError();
	void OnNetworkInspectionTransitWarning();
	void OnNetworkInspectionViewLog();
	void OnUpdateNetworkInspectionHighwayError(CCmdUI *pCmdUI); 
	void OnUpdateNetworkInspectionHighwayWarning(CCmdUI *pCmdUI); 
	void OnUpdateNetworkInspectionTransitError(CCmdUI *pCmdUI);   
	void OnUpdateNetworkInspectionTransitWarning(CCmdUI *pCmdUI); 
	void OnUpdateNetworkInspectionViewLog(CCmdUI *pCmdUI);
		void NetworkInspectionRelationFileOpen(CString a_strFileName);
		void UpdateNetworkInspection(CString a_strFileName);

	void OnScreenCordonManage();
	void OnUpdateScreenCordonManage(CCmdUI *pCmdUI);

    virtual void UpdateScreenCordonManage(int nType, __int64 nxID);

	void OnZoningDivision();
	void OnZoningIntegration();

	void OnPrepareChartOD();
	void OnPrepareChartNetworkZone();
	void OnPrepareChartNetworkNode();
	void OnPrepareChartNetworkLink();
	void OnPrepareChartPC();

	void OnAccessibilityModify();
	void OnMultiModalAssignResult();
	void OnUpdateMultiModalAssignResult(CCmdUI *pCmdUI);
		bool IsExistMultiModalAssignResultFile();

	void OnMultiModalTransitPathEdit();
	void OnInterModalAssignResult();
	void OnUpdateInterModalAssignResult(CCmdUI *pCmdUI);
		bool IsExistInterModalAssignResultFile();

	void OnInterModalTargetRegionSetting();
	void OnInterModalAccessTerminalEditor();
	void OnInterModalGlobalTerminalSetting();
	void OnInterModalPathEdit();
	void OnInterModalTerminalDesireLine();
	void OnTerminalScheduleEdit();

	void OnModelsProcessmultiModal();
	void OnModelsProcessInterModal();

	void OnPathVolumeTerminalAccessEgress();
	void OnUpdatePathVolumeTerminalAccessEgress(CCmdUI *pCmdUI);

	void OnNetworkEditNode();
	void OnNetworkEditLink();
	void OnNetworkEditTurn();
	void OnNetworkEditTransit();

	void OnMatrixPurposeOD();
	void OnMatrixModeOD();
	void OnMatrixChainOD();
	void OnMatrixVehicleModeOD();

	void OnMenuToolsSubNetWork(void);
	void OnNetworkeditEditlog();
	void OnUpdateNetworkeditEditlog(CCmdUI *pCmdUI);

	void OnZoneODGroupGeneration();

	void OnViewTurnVolume();
	void OnThemesTerminalAEModeViewer();
	void OnUpdateThemesTerminalAEModeViewer(CCmdUI *pCmdUI);

	void OnFDefaultPolicyContainer();

	void OnNombaraTesterA();
	void OnNombaraTesterB();
	void OnHelpNombara();

    //////////////////////////////////////////////////////////////////////////////
    //★ 2016.03.08 이후
    void LoadDispLayerTypeLabel(KIOColumn* pTypeColumn, KIOColumn* pLblColumn, std::map<__int64, TypeLabel>& mapTxIDTypeLabel, int nID);

private:
    int GetLayerID(CString strName);
public:
    void ZoomToFeature(int nLayerID, __int64 nxID, bool bFlash = true);
    void PanToFeature (int nLayerID, __int64 nxID, bool bFlash = true);
	void FlashToFeature( int nLayerID, __int64 nxID);
private:
    ITxSymbolPtr FlashSymbol(EnumTxGeoType a_oGeoType);
    std::vector<TxFeatureDispPtr> m_vecFlash;
private:
    void AddFlashFeature( TxFeatureDispPtr spFeatureDisp );
    void AddFlashFeature( std::vector<TxFeatureDispPtr>& vecFeatureDisp );
public:
    virtual ITxFeatureUserLayerPtr AddUserLayer(int nLayerID, bool bMemeory = true);

public:
    void NodeGeometry(std::map<__int64, ITxGeometryPtr>& mapGeometry);

public:
    virtual void UpdateNodeLinkMapDisp(int nLayerID, __int64 TxID, int nType, CString strLblColumn, CString strLblValue);

	/*****************************************************************************
	* 2019.03.08 : 지도 배경색상 변경기능 추가
	*****************************************************************************/
private: 
	int m_nBackgroundMode;	// 0 =  배경없음(흰색), 1=단일색상
	Gdiplus::Color m_crBackground;
public:
	void SetBackgroundColor(int nBGMode, Gdiplus::Color crBackground);
	Gdiplus::Color GetBackgroundColor();
	bool IsDisplayOptionTableExists();
	bool CreateTableDisplayOption();
	void ReadDisplayOptionBackgroudColor();
	bool UpdateDisplayOptionBackgroundColor();

	afx_msg void OnSetMapBackgroundColor();
	afx_msg void OnUpdateSetMapBackgroundColor(CCmdUI* pCmd);
	afx_msg void OnBackgroundNoFill();
	afx_msg void OnBackgroundMoreColors();
	afx_msg void OnBackgroundSelector(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateBackgroundSelector(CCmdUI* pCmd);
	/****************************************************************************/
};
