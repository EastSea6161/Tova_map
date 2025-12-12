// MapView.cpp : implementation file
//

#include "stdafx.h"
#include "MapView.h"

#include "ChildFrm.h"
#include "MapFrameWnd.h"

#include "Project.h"
#include "Target.h"
#include "ImChampFrameWindow.h"
#include "IOView.h"
#include "IOTableView.h"
#include "TocFormView.h"
#include "MapDiplayMonitor.h"
#include "IncludeMap.h"

#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"

#include "SCManageDlg.h"
#include "DBaseMap.h"
#include "LayerXmlRenderer.h"
#include "ScreenDrawEditMapIdentify.h"
#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "TCoordinate.h"
#include "DBaseCheckData.h"
#include "PopupMessenger.h"
#include "DBaseTerminalAEModeViewer.h"

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
#include "ImChampFileExist.h"

#include "DefineNames.h"
#include "KMapBackgroundColorDlg.h"


IMPLEMENT_DYNCREATE(KMapView, CFormView)
BEGIN_MESSAGE_MAP(KMapView, CFormView)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_SETCURSOR()    
    //ON_WM_TIMER()
    ON_WM_DESTROY()

    ON_NOTIFY (WM_MAP_BACKGROUND_DRAW_END, IDC_THUNDER_MAP, OnMapLayerDrawEnd)
    ON_NOTIFY (WM_SETCURSOR,     IDC_THUNDER_MAP, OnMapSetCursor)
    ON_NOTIFY (WM_MOUSEMOVE,     IDC_THUNDER_MAP, OnMapMouseMove)
    ON_NOTIFY (WM_LBUTTONDOWN,   IDC_THUNDER_MAP, OnMapLButtonDown)
    ON_NOTIFY (WM_LBUTTONUP,     IDC_THUNDER_MAP, OnMapLButtonUp)
    ON_NOTIFY (WM_LBUTTONDBLCLK, IDC_THUNDER_MAP, OnMapLButtonDblClk)

    ON_NOTIFY (WM_RBUTTONDOWN,   IDC_THUNDER_MAP, OnMapRButtonDown)
    ON_NOTIFY (WM_RBUTTONUP,     IDC_THUNDER_MAP, OnMapRButtonUp)

    ON_NOTIFY (WM_MBUTTONDOWN,   IDC_THUNDER_MAP, OnMapMButtonDown)
    ON_NOTIFY (WM_MBUTTONUP,     IDC_THUNDER_MAP, OnMapMButtonUp)

    // 전체 화면
    ON_COMMAND(ID_MAPVIEW_FULLEXTENT, OnMapviewFullExtent)

    // 확대, 축소, 이동, 편집 모드
	ON_COMMAND(ID_MAPVIEW_ZOOMIN, OnMapviewZoomInCommand)
	ON_UPDATE_COMMAND_UI( ID_MAPVIEW_ZOOMIN, OnUpdateMapviewZoomInCommand )

	ON_COMMAND(ID_MAPVIEW_ZOOMOUT, OnMapviewZoomOutCommand)
	ON_UPDATE_COMMAND_UI( ID_MAPVIEW_ZOOMOUT, OnUpdateMapviewZoomOutCommand )
	
	ON_COMMAND(ID_MAPVIEW_PAN, OnMapviewPanCommand)
	ON_UPDATE_COMMAND_UI( ID_MAPVIEW_PAN, OnUpdateMapviewPanCommand )

	ON_COMMAND_RANGE( ID_BUSLINE_CREATEBUSLINE, ID_BUSLINE_CREATEBUSLINE, OnMapviewCommand )
	ON_UPDATE_COMMAND_UI_RANGE( ID_BUSLINE_CREATEBUSLINE, ID_BUSLINE_CREATEBUSLINE, OnUpdateMapviewCommand )
	
    // 지도 이전/이후 보기
    ON_COMMAND_RANGE( ID_MAPVIEW_BACKWARD, ID_MAPVIEW_FORWARD, OnMapExtentStack )
    ON_UPDATE_COMMAND_UI_RANGE( ID_MAPVIEW_BACKWARD, ID_MAPVIEW_FORWARD, OnUpdateMapExtentStack )

    //지도 동기화(None, Position, Position+Scale
    ON_COMMAND_RANGE( ID_MAPVIEW_SYNC_NONE, ID_MAPVIEW_SYNC_POS_SCALE, OnMapviewSyncCommand )
    ON_UPDATE_COMMAND_UI_RANGE(ID_MAPVIEW_SYNC_NONE, ID_MAPVIEW_SYNC_POS_SCALE, OnUpdateMapviewSyncCommand )
	
	// IDentify
	ON_XTP_EXECUTE( ID_MAPVIEW_IDENTIFY_LAYER, OnIdentifyTargetChanged)
	ON_UPDATE_COMMAND_UI( ID_MAPVIEW_IDENTIFY_LAYER, OnUpdateIdentifyTarget )

    // 속성보기(Identify)
    ON_COMMAND( ID_MAPVIEW_IDENTIFY, OnIdentifyCommand )
    ON_UPDATE_COMMAND_UI( ID_MAPVIEW_IDENTIFY, OnUpdateIdentifyCommand )

	// 네트워크 편집 팝업(33836-33841)
	ON_COMMAND_RANGE( ID_MAP_NODE_PROPERTIES, ID_MAP_LINK_DELETE, &KMapView::OnShapeEditPopupCommand )

	// 편집시작
	ON_COMMAND( ID_SHAPE_EDIT_START, &KMapView::OnMapShapeEditStart )
	ON_UPDATE_COMMAND_UI( ID_SHAPE_EDIT_START, &KMapView::OnUpdateOnMapShapeEditStart )
	// 편집종료
	ON_COMMAND( ID_SHAPE_EDIT_END, &KMapView::OnMapShapeEditEnd )
	ON_UPDATE_COMMAND_UI( ID_SHAPE_EDIT_END, &KMapView::OnUpdateOnMapShapeEditEnd )
    // Edit Operation(Start, Save)
    ON_COMMAND_RANGE( ID_MAPEDIT_START, ID_MAPEDIT_SAVE, OnMapEditOperationCommand )
    ON_UPDATE_COMMAND_UI_RANGE( ID_MAPEDIT_START, ID_MAPEDIT_SAVE, OnUpdateMapEditOperationCommand)

    ON_COMMAND( ID_SCREEN_CAPTURE, &KMapView::OnMapCapture )

    // Edit 커서(펜)

    // MapSearch Layer 
    ON_XTP_EXECUTE( ID_MAPVIEW_EDITTARGET, OnEditTarget)
    ON_UPDATE_COMMAND_UI( ID_MAPVIEW_EDITTARGET, OnUpdateEditTarget )

    // EditTask Method
    ON_XTP_EXECUTE( ID_MAPVIEW_EDITTASK, OnEditTask )
    ON_UPDATE_COMMAND_UI( ID_MAPVIEW_EDITTASK, OnUpdateEditTask )
    
    ON_COMMAND(ID_FIND_NETWORK, &KMapView::CtrlFind)
    ON_COMMAND(ID_MAPVIEW_FIND, &KMapView::CtrlFind)

    // MapSearch Execute
    ON_COMMAND( ID_MAPVIEW_SELECTION_SEARCH, &KMapView::OnMapSelectionSearchPolygon )
    ON_UPDATE_COMMAND_UI( ID_MAPVIEW_SELECTION_SEARCH, &KMapView::OnUpdateMapSelectionSearchPolygon )

    ON_COMMAND( ID_MAPVIEW_SELECTION_RECT, &KMapView::OnMapSelectionSearchRect )
    ON_UPDATE_COMMAND_UI( ID_MAPVIEW_SELECTION_RECT, &KMapView::OnUpdateMapSelectionSearchRect )
        
    ON_COMMAND( ID_MAPVIEW_CLEARSELECTION, &KMapView::OnSelectedClearMapView )
    ON_UPDATE_COMMAND_UI( ID_MAPVIEW_CLEARSELECTION, &KMapView::OnUpdateSelectedClearMapView )

    ON_COMMAND( ID_MAPVIEW_PASTE, &KMapView::OnMapViewPasteAttribute )
    ON_UPDATE_COMMAND_UI( ID_MAPVIEW_PASTE, &KMapView::OnUpdateMapViewPasteAttribute )

    // 속성 정보 편집
    ON_COMMAND(ID_POPUP_MAPEDIT_EDITPROPERTIES, &KMapView::OnPopupMapeditEditproperties)
    ON_COMMAND(ID_MAPVIEWMENU_SEARCHROUTE_BY_LINK, &KMapView::OnMapviewMenuSearchTransitByLink)
    ON_COMMAND(ID_INFLUENCE_ENVELOPE, &KMapView::OnInfluenceEnvelope)
    ON_COMMAND(ID_INFLUENCE_CIRCLE, &KMapView::OnInfluenceCircle)
    ON_COMMAND(ID_INFLUENCE_POLYGON, &KMapView::OnInfluencePolygon)
	ON_COMMAND(ID_POPUP_MAPEDIT_INSERTVERTEX, &KMapView::OnPopupMapeditInsertvertex)
	
	ON_COMMAND(ID_HELP_FELIX,                &KMapView::OnRuler)
    ON_COMMAND(ID_MAPVIEW_RULER,             &KMapView::OnRuler)

	
	ON_COMMAND(ID_MAP_BACKGROUND_COLOR, &KMapView::OnSetMapBackgroundColor)
	ON_UPDATE_COMMAND_UI(ID_MAP_BACKGROUND_COLOR, &KMapView::OnUpdateSetMapBackgroundColor)
	ON_COMMAND(XTP_IDS_COLOR_CUST, &KMapView::OnBackgroundNoFill)
	ON_COMMAND(XTP_IDS_MORE_COLORS, &KMapView::OnBackgroundMoreColors)
	ON_XTP_EXECUTE(ID_MAP_BACKGROUND_SELECTOR, &KMapView::OnBackgroundSelector)
	ON_UPDATE_COMMAND_UI(ID_MAP_BACKGROUND_SELECTOR, &KMapView::OnUpdateBackgroundSelector)

	END_MESSAGE_MAP()

KMapView::KMapView(Gdiplus::Color crBackground)
		: CFormView(KMapView::IDD),
		m_emMapMode(KEMMapModePan),
		m_emMapDisplaySync(KEMMapDisplaySyncNone),
		m_bReceivedDisplaySyncMessage(false),
		m_hCursor(NULL),
		m_pTarget(NULL),
		m_nBackgroundMode(0),
		m_crBackground(crBackground)
{
    m_bLinkCartographicRenderer = false;
	m_pTocFormView  = NULL;
    m_pMapNodeSearch = NULL;
    m_pMapZoning    = NULL;
	
	m_TNodeLabelInfo.emLabelDisplay = KEMLabelDisplayStatusNone;
	m_TLinkLabelInfo.emLabelDisplay = KEMLabelDisplayStatusNone;

	TasESRICacheID    = 1010;
	m_emLayerIdentify = LINK;
    m_emLayerSearch   = LINK;

    m_nxStartNodeID = 1;
    m_nxStartLinkID = 1;

    m_dLinkFlashOffset = 10.0;

    m_bUseCacheScreenDrawRullerLinkSelect = true;

    m_dMouseMoveMapX = 0.0;
    m_dMouseMoveMapY = 0.0;

    m_dPreTimerMapX  = -999999.0;
    m_dPreTimerMapY  = -999999.0;

    m_dTooltipMapX   = 0.0;
    m_dTooltipMapY   = 0.0;

    m_lMouseMoveMapX = 0L;
    m_lMouseMoveMapY = 0L;

    TxLogDebugVisitor();
}

KMapView::~KMapView()
{   
    //CloseModelessDialog();
    TxLogDebugVisitor();	
}

#ifdef _DEBUG
void KMapView::AssertValid() const
{
    CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void KMapView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void KMapView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_THUNDER_MAP, m_ThunderMapCtrl);
}

BOOL KMapView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void KMapView::OnInitialUpdate()
{
    TxLogDebugStartMsg();
    CFormView::OnInitialUpdate();
    InitalizeMap();
    TxLogDebugEndMsg();
}


void KMapView::InitalizeMap() 
{
    TxLogDebugStartMsg();
    // Step 1. 지도 컨트롤 로드    
    // 이미지 맵 로드
    // Step 2. Window 정보 - CChildFrame [ KMapFrameWnd + KIOFrameWnd(KIOView) ]
    KMapFrameWnd* pFrameWnd   = (KMapFrameWnd*)(this->GetParentFrame());

    CChildFrame*  pChildFrame = (CChildFrame*) (pFrameWnd->GetParentFrame());	
    KIOView*      pIOView     = pChildFrame->GetIOView();

    if(NULL != pIOView) {
        m_pIOView = pIOView;
        int nTableViewCount = pIOView->GetTableViewCount();

        for(int i=0; i<nTableViewCount; ++i) {
            KIOTableView* pTableView = pIOView->GetTableView(i);
            pTableView->RegisterMapView(this);

            m_mapRegisteredTableView.insert(std::make_pair(pTableView->GetTableName(), pTableView));
        }
    }    

    m_spThemeControl = KThemeDrawControlPtr(new KThemeDrawControl(this));

	ThunderMapPtr spMap = m_ThunderMapCtrl.GetThunderMap();
	if (nullptr != spMap)
	{
		spMap->SetBackgroundColor(m_crBackground);
		spMap->Refresh();
	}

    TxLogDebugEndMsg();
}


void KMapView::OnPaint()
{  
    CPaintDC dc(this); // device context for painting
    
    /*
    CRect rectControl;
    CWnd *pWnd = GetDlgItem( IDC_MAPCONTROL );
    if( NULL != pWnd )
    {
        pWnd->GetWindowRect(rectControl);
        ScreenToClient(rectControl);
        rectControl.InflateRect(1, 1);
        dc.Draw3dRect( rectControl, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DDKSHADOW) );
    }
    }*/
    // Do not call CFormView::OnPaint() for painting messages
}

void KMapView::OnSize(UINT nType, int cx, int cy)
{
    //CFormView::OnSize(nType, cx, cy);
    AdjustViewLayout();
}


void KMapView::AdjustViewLayout(void)
{
    CWnd* pWnd = GetDlgItem(IDC_THUNDER_MAP);

    if( (NULL != pWnd) && (pWnd->GetSafeHwnd() != NULL) )
    {
        CRect rectClient;
        GetClientRect(rectClient);

        pWnd->SetWindowPos( NULL, 
            rectClient.left    + 1, rectClient.top      + 1, 
            rectClient.Width() - 2, rectClient.Height() - 2, 
            SWP_NOZORDER | SWP_NOACTIVATE
            );
    }
}

void KMapView::OnDestroy()
{
    CFormView::OnDestroy();
    try {
        
        KMapDisplayMonitor::Instance()->UnRegister(this);  

        CloseModelessDialog();

        m_spThemeControl   = nullptr;
        m_spScreenDrawEdit = nullptr;
        m_spShapeEditController = nullptr;        
    }
    catch(...) {
        TxLogDebugException();
    }    

    TxLogDebugVisitor();
}

void KMapView::PostNcDestroy()
{
    //CFormView::PostNcDestroy();
    /// View window class는 동적으로 생성된 후 스스로 삭제 된다.
    TxLogDebugVisitor();
    //KMapDiplayMonitor::Instance()->UnRegister(this); 
    delete this;
    //CFormView::PostNcDestroy();
}

void KMapView::OnMapviewZoomInCommand()
{
	MapCommandZoomIn();
}

void KMapView::OnUpdateMapviewZoomInCommand(CCmdUI *pCmdUI)
{
    try {
        BOOL bCheck = ZoomInCommand == ThunderMapCommand() ? TRUE : FALSE;
        pCmdUI->SetCheck(bCheck);
    }
    catch (...) {
        TxLogDebugException();
    }
}

void KMapView::OnMapviewZoomOutCommand()
{
	MapCommandZoomOut();
}

void KMapView::OnUpdateMapviewZoomOutCommand(CCmdUI *pCmdUI)
{
    try {
        BOOL bCheck = ZoomOutCommand == ThunderMapCommand() ? TRUE : FALSE;
        pCmdUI->SetCheck(bCheck);
    }
    catch (...) {
        TxLogDebugException();
    }
}

void KMapView::OnMapviewPanCommand()
{
	MapCommandPan();
}

void KMapView::OnUpdateMapviewPanCommand(CCmdUI *pCmdUI)
{
    try {
        BOOL bCheck = PanCommand == ThunderMapCommand() ? TRUE : FALSE;
        pCmdUI->SetCheck(bCheck);
    }
    catch (...) {
        TxLogDebugException();
    }
}


void KMapView::OnMapviewCommand( UINT nID )
{
	if (m_spScreenDrawEdit != NULL)
	{
		m_spScreenDrawEdit->Sleep();
	}

    KEMMapMode emMapMode = KEMMapModePan;

    switch( nID )
    {
    case ID_BUSLINE_CREATEBUSLINE :
        emMapMode = KEMMapModeRouteCreate;
        break;
    }

    SetMapMode( emMapMode );
}

void KMapView::OnUpdateMapviewCommand(CCmdUI *pCmdUI)
{
	BOOL bCheck = FALSE;	
	pCmdUI->SetCheck( bCheck );
}

void KMapView::SetMapMode( KEMMapMode emMapMode )
{
    m_emMapMode = emMapMode;

    if (emMapMode < 4) {
        if (emMapMode == KEMMapModeZoomIn) {
            MapCommandZoomIn();
        } else if (emMapMode == KEMMapModeZoomOut) {
            MapCommandZoomOut();
        } else {
            MapCommandPan();
        }
    }
    else {        
	    MapSetBusinessMode();
    }
}

void KMapView::SetMapCursor(WORD a_wCursorID)
{
	m_hCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(a_wCursorID));
}

void KMapView::SetIOView(KIOView* pIOView)
{
	m_pIOView = pIOView;
}


void KMapView::NotifyIOTableViewCreated(LPCTSTR strTableName)
{
	ASSERT(NULL != m_pIOView);
	if(NULL == m_pIOView)
	{
		return;
	}

	KIOTableView* pIOTableView = m_pIOView->FindTableView(strTableName);
	ASSERT(NULL != pIOTableView);
	if(NULL == pIOTableView)
	{
		return ;
	}

	pIOTableView->RegisterMapView(this);
	m_mapRegisteredTableView.insert(std::make_pair(strTableName, pIOTableView));
}


void KMapView::NotifyIOTableViewClosed(LPCTSTR strTableName)
{
	std::map<CString, KIOTableView*>::iterator itRegister = m_mapRegisteredTableView.find(strTableName);
	if(m_mapRegisteredTableView.end() != itRegister)
	{
		m_mapRegisteredTableView.erase(itRegister);
	}
	
	MapRefresh();
}


void KMapView::NotifyIOTableViewActivated(LPCTSTR strTableName)
{	
}


TxEnvelope KMapView::GetMapEnveolpe()
{
    ThunderMapPtr spThunderMap = GetThunderMap();

    TxEnvelope oEnvelope = spThunderMap->GetMapDisplayEnvelope();
    return oEnvelope;
}

void KMapView::LoadMapView( KTarget* pTarget )
{
    TxLogDebugStartMsg();

    try
    {
        m_pTarget = pTarget;
        m_pPersonalGeoDataBase = nullptr;

        // 지도 Handle
        LoadInitialLayers();
        FullExtentMap();

        m_nxStartNodeID = KBulkDBaseNode::MaxNodeID(m_pTarget) + 1;
        m_nxStartLinkID = KBulkDBaseLink::MaxLinkID(m_pTarget) + 1;


		/*****************************************************************************
		* 2019.03.08 : 지도 배경색상 변경기능 추가에 따른 수정
		*****************************************************************************/
		ReadDisplayOptionBackgroudColor();
		ThunderMapPtr spMap = m_ThunderMapCtrl.GetThunderMap();
		if (nullptr != spMap)
		{
			spMap->SetBackgroundColor(m_crBackground);
			spMap->Refresh();
		}

		/****************************************************************************/
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }   
    
    TxLogDebugEndMsg();
}

void KMapView::LoadInitialLayers(bool bFirst)
{
    TxLogDebugStartMsg();

    if (m_pTarget == nullptr)
        return;

    CString strMapSvrPath(_T("")); {
        strMapSvrPath = m_pTarget->GetIODataFilePath();
    }
    
    KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();    
    ITxMapServerPtr spMapServer(new TxSQLMapServer(TxMapDataServerSqlite, strMapSvrPath)); {
        if ( spMapServer->Connect(spDBase) != 1) //★ Target의 커넥션을 활용
            return; 
    }

    std::map<int, TxLayerInfoPtr> mapOrderLayer; {
        std::vector<TxLayerInfoPtr>   vecLayerInfo = spMapServer->LayerInfo();
        for (size_t i=0; i<vecLayerInfo.size(); i++) {
            TxLayerInfoPtr spLayerInfo = vecLayerInfo[i];

            CString strLayerName = spLayerInfo->Name();
            if (_tcsicmp(strLayerName, _T("Node")) == 0) {
                mapOrderLayer[KLayerID::Node()] = spLayerInfo;
            } else if (_tcsicmp(strLayerName, _T("Link")) == 0) {
                mapOrderLayer[KLayerID::Link()] = spLayerInfo;
            } else if (_tcsicmp(strLayerName, _T("Zone")) == 0) {
                mapOrderLayer[KLayerID::Zone()] = spLayerInfo;
            }
        }
    }

    int nLayerOrder(0);
    for (auto iter = mapOrderLayer.begin(); iter != mapOrderLayer.end(); ++iter) {
        TxLayerInfoPtr spLayerInfo = iter->second;

        ITxFeatureLayerPtr spLayer = nullptr;
        if (spLayerInfo->GeoType() == GeoPointType) // GeoPointType
        {
            spLayer  = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
            TxSymbolPointPtr  spSymbol = TxSymbolPointPtr(new TxSymbolPoint('0', 5.0, Gdiplus::Color(128,128,128)));
            spLayer->LayerDispSymbol(spSymbol);
        }
        else if (spLayerInfo->GeoType() == GeoPolylineType) // GeoPolylineType
        {
            spLayer  = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));                    
            TxSymbolLinePtr   spSymbol = TxSymbolLinePtr(new TxSymbolLine(1.0, Gdiplus::Color::Black, false));     
            spLayer->LayerDispSymbol(spSymbol);
        }
        else if (spLayerInfo->GeoType() == 5) // 5 (TxMapLayer.tbl 에는 5 가 저장되어있음)
        {
            spLayer = ITxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
            TxSymbolPolygonPtr spSymbol = TxSymbolPolygonPtr(new TxSymbolPolygon(Gdiplus::Color(100, 0,255,0), Gdiplus::Color(100, 0,0,0), 1.0));
            spSymbol->Fill(true);
            spLayer->LayerDispSymbol(spSymbol);
        }

        if (spLayer != nullptr) {
            MapAddLayer(spLayer, iter->first/*LayerID*/, ++nLayerOrder);
        }
    }

    ReloadRender(false);

    //★ 지도 중간에 깜박이는 옵션
    MapDrawOptionFlicker(false);
}

void KMapView::OnMapSetCursor( NMHDR* pNMHDR, LRESULT* pResult )
{    
    EnumTxMapCommand enMapCommand = ThunderMapCommand(); /*현재 지도의 명령 상태 조회*/

    // 1. 지도의 명령 상태 정보 조회
    // 2. 현재 ThunderMap의 명령 상태가 
    //     확대, 축소, 이동 - 커서 설정
    //     Nothing 상태인 경우 - 현재 비지니스 정보를 통해 커서를 설정

    HCURSOR hMapCursor;
    // 현재 지도의 모드
    switch(enMapCommand)
    {
    case ZoomInCommand:
        hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_QBicMap_ZoomIn ) );
        break;
    case ZoomOutCommand:
        hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_QBicMap_ZoomOut ) );
        break;
    case PanCommand :
        hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_QBicMap_Pan ) );
        break;
    case MPanCommand :
        hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_QBicMap_Pan ) );
        break;
    default: 
        {
            switch( m_emMapMode )
            {
            case KEMMapModeNone:
                hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_ARROW ) );
                break;
            case KEMMapModeZoomIn :
                hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_ZOOMIN ) );
                break;
            case KEMMapModeZoomOut :
                hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_ZOOMOUT ) );
                break;
            case KEMMapModePan : 
                hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_PAN ) );
                break;		
            case KEMMapModeIdentify : 
                hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_IDENTIFY ) );
                break;
            case KEMMapModeNetworkEdit:
                if (m_spShapeEditController)
                {
                    hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE(m_spShapeEditController->GetCursorID()) );
                }
                else
                {
                    hMapCursor = LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_EditingEditTool ) );
                }			
                break;
            case KEMMapModeRouteCreate : 
            case KEMMapModeRouteEdit : 
            case KEMMapModeSelectInfluence :
            case KEMMapModeScreenLine :
            case KEMMapModeCordonLine :
                hMapCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CROSS));
                break;
            case KEMMapModeSelectTransitByLink : 
                hMapCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ARROW));
                break;
            case KEMMapModeSelectNode:
                hMapCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SELECT_POINT));
                break;
            case KEMMapModeMapSelectRect :
                hMapCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SELECT_ELEMENT));
                break;
            case KEMMapModeMapSelectPoly :
                hMapCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CREATESHAPE));
                break;
            case KEMMapModeZoning:                    
            case KEMMapModeSelectLink:
            default:
                hMapCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_AddEditingEditTool/*IDC_CREATESHAPE*//*IDC_EditingEditTool*/));
                break;
            }
        }        
        break;
    }
    
    ::SetCursor( hMapCursor );
    *pResult = 0;
}

void KMapView::OnMapLayerDrawEnd( NMHDR* pNMHDR, LRESULT* pResult )
{
    double dMapScale     = MapGetScale();
    double dCurPixelDist = MapGetPixelToDist();
    
    TxEnvelope oTxEnvelope;
    MapGetEnvelope(oTxEnvelope); {
        m_dExpandMinX = oTxEnvelope.GetMinX();
        m_dExpandMinY = oTxEnvelope.GetMinY();
        m_dExpandMaxX = oTxEnvelope.GetMaxX();
        m_dExpandMaxY = oTxEnvelope.GetMaxY();
    }

    if (m_pTocFormView != NULL) {
        m_pTocFormView->MapChangedLayerState();
    }

    if (m_vecFlash.size() > 0) {   
        Flash(m_vecFlash);
        m_vecFlash.clear();
    }

    /*
    if (m_spMapBusiness)
        m_spMapBusiness->Draw();

    // Flash Geometry : 지도 드로잉이 종료된 후 호출한다.
    if (m_vecFlash.size() > 0) {   
        Flash(m_vecFlash);
        m_vecFlash.clear();
    }
   
    if ( KEMMapDisplaySyncNone != m_emMapDisplaySync )
    {
        if ( m_bReceivedDisplaySyncMessage == false ) {
            KMapDisplayMonitor::Instance()->ChangeMapDisplay(this);
        } else {
            m_bReceivedDisplaySyncMessage = false;
        }      
    }*/

    if ( KEMMapDisplaySyncNone != m_emMapDisplaySync )
    {
        if ( m_bReceivedDisplaySyncMessage == false ) {
            KMapDisplayMonitor::Instance()->ChangeMapDisplay(this);
        } else {
            m_bReceivedDisplaySyncMessage = false;
        }      
    }

    if (m_spScreenDrawEdit /*&& ThunderMapCommand() == MapUserCommand*/) {
        m_spScreenDrawEdit->Draw();
    }

    if (m_spShapeEditController /*&& ThunderMapCommand() == MapUserCommand*/) {
        m_spShapeEditController->Draw();
    }

#pragma region 스케일 표시
    try	
    {
        CMainFrameWnd* pFrame = (CMainFrameWnd*)AfxGetMainWnd();
        CString strTemp(_T(""));
        strTemp.Format(_T("1:%.0f"), dMapScale);
        pFrame->SetPaneTextMapScale(strTemp);

        strTemp.Format(_T("1px:%.2fm"), dCurPixelDist);
        pFrame->SetPaneTextInfo(strTemp);
    } catch (...) {
        TxLogDebugException();	
    }	
#pragma endregion 스케일 표시

    *pResult = 0;
}

void KMapView::OnMapMouseMove( NMHDR* pNMHDR, LRESULT* pResult )
{
    TxMapEventMouse* pMapEventMouse = (TxMapEventMouse*)pNMHDR;

#pragma region 좌표표시
    try
    {
        CMainFrameWnd* pFrame = (CMainFrameWnd*)AfxGetMainWnd();
        if (pFrame) {
            CString strMapCoordinate;
            strMapCoordinate.Format(_T("%.2f, %.2f"), pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
            pFrame->SetPaneTextMapCoordinate(strMapCoordinate);
        }
    }
    catch (...)
    {
        TxLogDebugException();	
    }
#pragma endregion 좌표표시

    //★--------------------------------------------★
    //★ 가운데 버튼 클릭 시 전송하지 않는다.
    UINT nFlags = pMapEventMouse->Flags;    
    if (MK_MBUTTON & nFlags) 
        return;
    //★--------------------------------------------★

    if (ThunderMapCommand() == MapUserCommand) 
    {
        switch( m_emMapMode ) 
        {
        case KEMMapModeIdentify:
        case KEMMapModeSelectLink:
        case KEMMapModeScreenDrawEdit:
        case KEMMapModeMapSelectRect:
        case KEMMapModeMapSelectPoly:
            if (IsScreenDrawEditing()) {                
                m_spScreenDrawEdit->OnMapMouseMove(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
            }
            break;
        case KEMMapModeNetworkEdit:
            if (IsShapeEditing()) {
                m_spShapeEditController->OnMapMouseMove(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
            }
            break;
        }
    }

    *pResult = 0;
}

void KMapView::OnMapLButtonDown( NMHDR* pNMHDR, LRESULT* pResult )
{
    if (ThunderMapCommand() != MapUserCommand)
        return;

    TxMapEventMouse* pMapEventMouse = (TxMapEventMouse*)pNMHDR;

    switch( m_emMapMode ) 
    {
    case KEMMapModeZoning:
        ZoningProcessSelectPoint(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        break;	
    case KEMMapModeIdentify:
    case KEMMapModeSelectLink:
    case KEMMapModeScreenDrawEdit:
    case KEMMapModeMapSelectRect:
    case KEMMapModeMapSelectPoly:
    if (IsScreenDrawEditing()) {
        UINT nFlags = pMapEventMouse->Flags;
        m_spScreenDrawEdit->OnMapLButtonDown(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
    }
    break;
    case KEMMapModeNetworkEdit:
        if (IsShapeEditing()) {
            UINT nFlags = pMapEventMouse->Flags;
            m_spShapeEditController->OnMapLButtonDown(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    }
    
    *pResult = 0;
}

void KMapView::OnMapLButtonUp( NMHDR* pNMHDR, LRESULT* pResult )
{
    if (ThunderMapCommand() != MapUserCommand)
        return;

    TxMapEventMouse* pMapEventMouse = (TxMapEventMouse*)pNMHDR;
    switch( m_emMapMode ) 
    {
    case KEMMapModeIdentify:
    case KEMMapModeSelectLink:
    case KEMMapModeScreenDrawEdit:
    case KEMMapModeMapSelectRect:
    case KEMMapModeMapSelectPoly:
        if (IsScreenDrawEditing()) {
            UINT nFlags = pMapEventMouse->Flags;
            m_spScreenDrawEdit->OnMapLButtonUp(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    case KEMMapModeNetworkEdit:
        if (IsShapeEditing()) {
            UINT nFlags = pMapEventMouse->Flags;
            m_spShapeEditController->OnMapLButtonUp(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    }
    
    *pResult = 0;
}

void KMapView::OnMapLButtonDblClk( NMHDR* pNMHDR, LRESULT* pResult )
{
    if (ThunderMapCommand() != MapUserCommand)
        return;

    TxMapEventMouse* pMapEventMouse = (TxMapEventMouse*)pNMHDR;
    switch( m_emMapMode ) 
    {
    case KEMMapModeIdentify:
    case KEMMapModeSelectLink:
    case KEMMapModeScreenDrawEdit:
    case KEMMapModeMapSelectRect:
    case KEMMapModeMapSelectPoly:
        if (IsScreenDrawEditing()) {
            UINT nFlags = pMapEventMouse->Flags;
            m_spScreenDrawEdit->OnMapLButtonDblClk(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    case KEMMapModeNetworkEdit:
        if (IsShapeEditing()) {
            UINT nFlags = pMapEventMouse->Flags;
            m_spShapeEditController->OnMapLButtonDblClk(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    }

    *pResult = 0;
}

void KMapView::OnMapRButtonDown( NMHDR* pNMHDR, LRESULT* pResult )
{
    if (ThunderMapCommand() != MapUserCommand)
        return;

    TxMapEventMouse* pMapEventMouse = (TxMapEventMouse*)pNMHDR;
    switch( m_emMapMode ) 
    {
    case KEMMapModeIdentify:
    case KEMMapModeSelectLink:
    case KEMMapModeScreenDrawEdit:
    case KEMMapModeMapSelectRect:
    case KEMMapModeMapSelectPoly:
        if (IsScreenDrawEditing()) {
            UINT nFlags = pMapEventMouse->Flags;        
            m_spScreenDrawEdit->OnMapRButtonDown(nFlags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    case KEMMapModeNetworkEdit:
        if (IsShapeEditing()) {
            UINT nFlags = pMapEventMouse->Flags;        
            m_spShapeEditController->OnMapRButtonDown(nFlags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    }
    
    *pResult = 0;
}

void KMapView::OnMapRButtonUp( NMHDR* pNMHDR, LRESULT* pResult )
{
    if (ThunderMapCommand() != MapUserCommand)
        return;

    TxMapEventMouse* pMapEventMouse = (TxMapEventMouse*)pNMHDR;
    switch( m_emMapMode ) 
    {
    case KEMMapModeIdentify:
    case KEMMapModeSelectLink:
    case KEMMapModeScreenDrawEdit:
    case KEMMapModeMapSelectRect:
    case KEMMapModeMapSelectPoly:
        if (IsScreenDrawEditing()) { 
            UINT nFlags = pMapEventMouse->Flags;
            m_spScreenDrawEdit->OnMapRButtonUp(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    case KEMMapModeNetworkEdit:
        if (IsShapeEditing()) {
            UINT nFlags = pMapEventMouse->Flags;
            m_spShapeEditController->OnMapRButtonUp(pMapEventMouse->Flags, pMapEventMouse->MousePoint.x, pMapEventMouse->MousePoint.y, pMapEventMouse->MapPoint.X, pMapEventMouse->MapPoint.Y);
        }
        break;
    }

    *pResult = 0;
}

void KMapView::OnMapMButtonDown( NMHDR* pNMHDR, LRESULT* pResult )
{
    *pResult = 0;
}

void KMapView::OnMapMButtonUp( NMHDR* pNMHDR, LRESULT* pResult )
{
    *pResult = 0;
}

void KMapView::ShowMapView(bool bShow /*=true*/)
{	
    CChildFrame* pChildFrame = ImChampFrameWindow::GetChildFrame(this);
	if (pChildFrame)
	{
		pChildFrame->ShowMapView(bShow);
	}
	else
	{
		TxLogDebugWarning();
		AfxMessageBox(_T("분석 년도를 선택해 주세요."));
		return;
	}
}


KIOTableView* KMapView::findRegisteredIOView(LPCTSTR strTableName)
{
	std::map<CString, KIOTableView*>::iterator itTableView = m_mapRegisteredTableView.find(strTableName);
	if(m_mapRegisteredTableView.end() != itTableView)
	{
		return itTableView->second;
	}

	return NULL;
}


void KMapView::ShowPopupMenuTransit(int x, int y)
{
	// todo : Show Context menu
    if (!TransitVisible())
        return;

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_MAPVIEW);
	CMenu* pPopup = menu.GetSubMenu(1);

	CPoint ptClick(x, y);
	ClientToScreen(&ptClick);

	KMapFrameWnd* pFrameWnd = (KMapFrameWnd*)(GetParentFrame());
	CXTPCommandBars* pCommandBars = pFrameWnd->GetCommandBars();
	pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, ptClick.x, ptClick.y, this);

}

/**************************************
* Overrides                           *
**************************************/
/* KMapView diagnostics */

KShapeEditControllerPtr KMapView::ShapeEditStart()
{
    if (m_spShapeEditController == NULL)
	{
    	m_spShapeEditController = KShapeEditControllerPtr(new KShapeEditController(this));		
	}

    m_spShapeEditController->StartMapEditing();

	SetMapMode( KEMMapModeNetworkEdit );
	return m_spShapeEditController;
}

void KMapView::OnMapShapeEditStart( void )
{	
	if (IsScreenDrawEditing())
	{
		if (m_emMapMode == KEMMapModeIdentify)
		{
			SetMapMode( KEMMapModeIdentify );
		}
		else
		{
			SetMapMode( KEMMapModeScreenDrawEdit );
		}
	}
	else
	{
		SetMapMode( KEMMapModeNetworkEdit );
	}	
}

void KMapView::OnUpdateOnMapShapeEditStart( CCmdUI* pCmdUI )
{	
    if (IsScreenDrawEditing() || IsShapeEditing()) {
        if (m_emMapMode == KEMMapModeScreenDrawEdit || m_emMapMode == KEMMapModeNetworkEdit) {
            pCmdUI->Enable(TRUE);
        }
        else {
            pCmdUI->Enable(FALSE);
        }
    }
    else {
        pCmdUI->Enable(FALSE);
    }
}

void KMapView::StopShapeEdit()
{
    if (m_spShapeEditController)
    {
        m_spShapeEditController->StopMapEditing();
        m_spShapeEditController.reset();
    }

    SetMapMode  (KEMMapModePan);
    SetMapCursor(IDC_PAN);
}

void KMapView::OnMapShapeEditEnd( void )
{	
    if (m_spScreenDrawEdit)
        m_spScreenDrawEdit.reset();

    if (m_spShapeEditController)
    {
        m_spShapeEditController->StopMapEditing();
        m_spShapeEditController.reset();
    }
    	
    SetMapMode  (KEMMapModePan);
    SetMapCursor(IDC_PAN);
}

void KMapView::OnUpdateOnMapShapeEditEnd( CCmdUI* pCmdUI )
{   
    if (IsScreenDrawEditing() || IsShapeEditing()) {
        if (m_emMapMode == KEMMapModeScreenDrawEdit || m_emMapMode == KEMMapModeNetworkEdit) {
            pCmdUI->Enable(TRUE);
        }
        else {
            pCmdUI->Enable(FALSE);
        }
    }
	else {
		pCmdUI->Enable(FALSE);
	}
}

void KMapView::OnMapCapture()
{
	try
	{
        //이미지 저장 경로를 이미지 생성전에 넣으면 결과가 이상함
        CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.bmp)|*bmp|"), NULL);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			fileDlg.m_ofn.lpstrTitle = _T("캡쳐 화면 저장");
		}
		else
		{
			fileDlg.m_ofn.lpstrTitle = _T("Save Map Image");
		}
        //fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

        if (fileDlg.DoModal() != IDOK)
            return;

        CString strExportFile = fileDlg.GetPathName();
        CString strFile;
        strExportFile.Replace(_T(".bmp"), _T(""));
        strFile.Format(_T("%s.bmp"), strExportFile);

        if (MapWriteImage(strFile) == false) {
            AfxMessageBox(_T("이미지 생성 작업 오류가 발생하였습니다."));
        }
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("이미지 저장에 실패했습니다."));
	}
}


void KMapView::OnMapEditOperationCommand( UINT nID )
{
    // 편집시작
    TxLogDebugVisitor();
	switch( nID )
	{
	case ID_MAPEDIT_START : 
        m_spShapeEditController = KShapeEditControllerPtr(new KShapeEditController(this));
        m_spShapeEditController->StartMapEditing();
		SetMapMode( KEMMapModeNetworkEdit );
		break;
	case ID_MAPEDIT_STOP : 
		if (m_spShapeEditController)
        {
            m_spShapeEditController->StopMapEditing();
            m_spShapeEditController.reset();
        }
		if( KEMMapModeNetworkEdit == m_emMapMode )
		{
			SetMapMode( m_emPrevMapMode );
		}
		break;
	case ID_MAPEDIT_SAVE : 
		//m_pMapEditor->SaveMapEditing();
		break;
	}    
}


void KMapView::OnUpdateMapEditOperationCommand( CCmdUI* pCmd )
{
	switch( pCmd->m_nID )
	{
	case ID_MAPEDIT_START : 
        if (m_spShapeEditController)
        {
            pCmd->Enable(FALSE);
        }
        else
        {
            pCmd->Enable(TRUE);
        }
		break;
	case ID_MAPEDIT_STOP :
        if (m_spShapeEditController)
        {
            pCmd->Enable(TRUE);
        }
        else
        {
            pCmd->Enable(FALSE);
        }
		break;
	case ID_MAPEDIT_SAVE : 
		pCmd->Enable( FALSE );
		break;
	}
}

void KMapView::OnIdentifyTargetChanged( NMHDR* pNMHDR, LRESULT* pResult )
{    
	NMXTPCONTROL* pNMControl = (NMXTPCONTROL*)pNMHDR;
	CXTPControlComboBox* pCombo = (CXTPControlComboBox*)pNMControl->pControl;
	if( xtpControlComboBox == pCombo->GetType() )
	{
		int nSelect = pCombo->GetCurSel();
		switch(nSelect)
		{
		case 0:			
			m_emLayerIdentify = NODE;
			OnIdentifyCommand();
			break;
		case 1:
			m_emLayerIdentify = LINK;
			OnIdentifyCommand();
			break;
        case 2:
            m_emLayerIdentify = ZONE;
            OnIdentifyCommand();
            break;
		default:
			break;
		}
		*pResult = TRUE;
	}
}

void KMapView::OnUpdateIdentifyTarget( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( TRUE );
	return;
}


void KMapView::OnEditTarget( NMHDR* pNMHDR, LRESULT* pResult )
{ 
	NMXTPCONTROL* pNMControl = (NMXTPCONTROL*)pNMHDR;
	CXTPControlComboBox* pCombo = (CXTPControlComboBox*)pNMControl->pControl;
	if( xtpControlComboBox == pCombo->GetType() )
	{
        int nSelect = pCombo->GetCurSel();
        switch(nSelect)
        {
        case 0:
            m_emLayerSearch = NODE;            
            break;
        case 1:
            m_emLayerSearch = LINK;
            break;
        default:
            break;
        }
	}

    if (m_spScreenDrawEdit != nullptr)
    {
        if (m_spScreenDrawEdit->GetType() == KEMScreenDrawEditSearchRect)
        {
            OnMapSelectionSearchRect();
        }
        else if (m_spScreenDrawEdit->GetType() == KEMScreenDrawEditSearchPolygon)
        {
            OnMapSelectionSearchPolygon();
        }
    }    

    *pResult = TRUE;
}


void KMapView::OnUpdateEditTarget( CCmdUI* pCmdUI )
{
    pCmdUI->Enable( TRUE );
}


void KMapView::OnEditTask( NMHDR* pNMHDR, LRESULT* pResult )
{        
}

void KMapView::OnUpdateEditTask( CCmdUI* pCmdUI )
{
    pCmdUI->Enable( TRUE );
}

void KMapView::OnMapSelectionSearchPolygon( /*NMHDR* pNMHDR, LRESULT* pResult*/ )
{    
    try
    {
        OnSelectedClearMapView();

        if (m_spMapViewSelectionFeedback == nullptr)
        {
            m_spMapViewSelectionFeedback = KMapViewSelectionFeedbackPtr(new KMapViewSelectionFeedback(this));
        }        

        switch(m_emLayerSearch)
        {
        case NODE:
            m_spMapViewSelectionFeedback->SetFeedbackType(MapViewFeedbackTypeNodeSelect);
            m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditSearchPolygon(this, m_spMapViewSelectionFeedback.get(), NODE)); 
            break;
        default:
            m_spMapViewSelectionFeedback->SetFeedbackType(MapViewFeedbackTypeLinkSelect);
            m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditSearchPolygon(this, m_spMapViewSelectionFeedback.get(), LINK)); 
            break;
        }
               
        m_spScreenDrawEdit->StartEditing();
    }
    catch (...)
    {
        AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
    }	
}

void KMapView::OnUpdateMapSelectionSearchPolygon( CCmdUI* pCmdUI )
{
    pCmdUI->Enable( TRUE );
}

void KMapView::OnMapSelectionSearchRect()
{    
    try
    {
        OnSelectedClearMapView();

        if (m_spMapViewSelectionFeedback == nullptr)
        {
            m_spMapViewSelectionFeedback = KMapViewSelectionFeedbackPtr(new KMapViewSelectionFeedback(this));
        }        

        switch(m_emLayerSearch)
        {
        case NODE:
            m_spMapViewSelectionFeedback->SetFeedbackType(MapViewFeedbackTypeNodeSelect);
            m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditSearchRect(this, m_spMapViewSelectionFeedback.get(), NODE)); 
            break;
        default:
            m_spMapViewSelectionFeedback->SetFeedbackType(MapViewFeedbackTypeLinkSelect);
            m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditSearchRect(this, m_spMapViewSelectionFeedback.get(), LINK)); 
            break;
        }

        m_spScreenDrawEdit->StartEditing();
    }
    catch (...)
    {
        AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
    }	
}

void KMapView::OnUpdateMapSelectionSearchRect( CCmdUI* pCmdUI )
{
    pCmdUI->Enable( TRUE );
}

void KMapView::OnSelectedClearMapView( /*NMHDR* pNMHDR, LRESULT* pResult*/ )
{
    try
    {
        m_vecSelectionNode.clear();
        m_vecSelectionLink.clear();

        if (m_spThemeControl)
            m_spThemeControl->Remove(ThemeLayerDrawLinkSelection);
        if (m_spThemeControl)
            m_spThemeControl->Remove(ThemeLayerDrawNodeSelection);
        
        MapRefresh();
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

void KMapView::OnUpdateSelectedClearMapView( CCmdUI* pCmdUI )
{
    if (m_vecSelectionNode.size() != 0 || m_vecSelectionLink.size() != 0)
    {
        pCmdUI->Enable( TRUE );
    }
    else
    {
        pCmdUI->Enable( FALSE );
    }    
}

void KMapView::OnMapViewPasteAttribute()
{
    switch(m_emLayerSearch)
    {
    case NODE:
        PasteNode();
        return;
        break;
    default:
        PasteLink();
        return;
        break;
    }
}

void KMapView::PasteNode()
{
    if (m_mapNodeCopyKeyValue.size() == 0)
    {
        AfxMessageBox(_T("노드 복사 정보가 없습니다."));
        return;
    }

    if (m_vecSelectionNode.size() == 0)
    {
        AfxMessageBox(_T("노드 선택 정보가 없습니다."));
        return;
    }

    try
    {
        int         nColumnCount(0);
        KIOTable*   pIOTable = m_pTarget->Tables()->FindTable(TABLE_NODE);	

        const 
        KIOColumns* pIOColumns   = pIOTable->Columns();
                    nColumnCount = pIOColumns->ColumnCount();
                
        CString strSQL(_T(""));
        strSQL.AppendFormat(_T(" Update %s Set "), TABLE_NODE );

        std::map<CString, CString>::iterator iter = m_mapNodeCopyKeyValue.begin();
        std::map<CString, CString>::iterator end  = m_mapNodeCopyKeyValue.end();

        bool bBlank = true;
        for(int i = 0; i < nColumnCount; i++)
        {
            KIOColumn* pColumn = pIOColumns->GetColumn( i );     
            CString    strColumnName(pColumn->Name());

            iter = m_mapNodeCopyKeyValue.find(strColumnName);
            if (iter != end)
            {
                if (bBlank)
                {
                    strSQL.AppendFormat(_T(" %s = '%s' "), strColumnName, iter->second);
                    bBlank = false;
                }
                else
                {
                    strSQL.AppendFormat(_T(", %s = '%s' "), strColumnName, iter->second);
                }                
            }
        }

        if (bBlank)
        {
            AfxMessageBox(_T("붙여넣기 실행에 필요한 항목이 없습니다."));
            return;
        }
                
        KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
        try
        {
            spDBaseConnection->BeginTransaction();

            size_t nxCount = m_vecSelectionNode.size();
            for(size_t i=0; i<nxCount; i++)
            {
                CString strSQLWhere(strSQL);
                strSQLWhere.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_NODE_ID, m_vecSelectionNode[i]);
                spDBaseConnection->ExecuteUpdate(strSQLWhere);
            }

            spDBaseConnection->Commit();
        }
        catch (KExceptionPtr ex)
        {
            spDBaseConnection->RollBack();
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            spDBaseConnection->RollBack();
        	TxLogDebugException();
        }   

        pIOTable->Notify();
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

void KMapView::PasteLink()
{
    if (m_mapLinkCopyKeyValue.size() == 0)
    {
        AfxMessageBox(_T("링크 복사 정보가 없습니다."));
        return;
    }

    if (m_vecSelectionLink.size() == 0)
    {
        AfxMessageBox(_T("링크 선택 정보가 없습니다."));
        return;
    }

    try
    {
        KIOTable*   pIOTable = m_pTarget->Tables()->FindTable(TABLE_LINK);	
        const 
        KIOColumns* pIOColumns = pIOTable->Columns();

        int nColumnCount = pIOColumns->ColumnCount();

        CString strSQL(_T(""));
        strSQL.AppendFormat(_T(" Update %s Set "), TABLE_LINK );

        std::map<CString, CString>::iterator iter = m_mapLinkCopyKeyValue.begin();
        std::map<CString, CString>::iterator end  = m_mapLinkCopyKeyValue.end();

        bool bBlank = true;
        for(int i = 0; i < nColumnCount; i++)
        {
            KIOColumn* pColumn = pIOColumns->GetColumn( i );     
            CString    strColumnName(pColumn->Name());

            iter = m_mapLinkCopyKeyValue.find(strColumnName);
            if (iter != end)
            {
                if (bBlank)
                {
                    strSQL.AppendFormat(_T(" %s = '%s' "), strColumnName, iter->second);
                    bBlank = false;
                }
                else
                {
                    strSQL.AppendFormat(_T(", %s = '%s' "), strColumnName, iter->second);
                }                
            }
        }

        if (bBlank)
        {
            AfxMessageBox(_T("붙여넣기 실행에 필요한 항목이 없습니다."));
            return;
        }

        KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
        try
        {
            spDBaseConnection->BeginTransaction();

            size_t nxCount = m_vecSelectionLink.size();
            for(size_t i=0; i<nxCount; i++)
            {
                CString strSQLWhere(strSQL);
                strSQLWhere.AppendFormat(_T(" Where %s = '%I64d' "), COLUMN_LINK_ID, m_vecSelectionLink[i]);
                spDBaseConnection->ExecuteUpdate(strSQLWhere);
            }

            spDBaseConnection->Commit();
        }
        catch (KExceptionPtr ex)
        {
            spDBaseConnection->RollBack();
            TxExceptionPrint(ex);
        }
        catch (...)
        {
            spDBaseConnection->RollBack();
            TxLogDebugException();
        }   

        pIOTable->Notify();
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

void KMapView::OnUpdateMapViewPasteAttribute( CCmdUI* pCmdUI )
{
    switch(m_emLayerSearch)
    {
    case NODE:
        if (m_mapNodeCopyKeyValue.size() != 0 && m_vecSelectionNode.size() != 0)
        {
            pCmdUI->Enable( TRUE );
            return;
        }
        break;
    default:
        if (m_mapLinkCopyKeyValue.size() != 0 && m_vecSelectionLink.size() != 0)
        {
            pCmdUI->Enable( TRUE );
            return;
        }
        break;
    }

    pCmdUI->Enable( FALSE );
}


void KMapView::OnPopupMapeditEditproperties()
{
    /*
	if( NULL != m_pMapEditor )
	{
		m_pMapEditor->editFeatureProperties();
	}
	}*/
}


BOOL KMapView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( NULL != m_hCursor )
	{
		::SetCursor( m_hCursor );
		return TRUE;
	}

	return CFormView::OnSetCursor(pWnd, nHitTest, message);
}

void KMapView::OnPopupMapeditInsertvertex()
{
    /*
	if(NULL != m_pMapEditor)
	{
		m_pMapEditor->insertVertex(m_dMouseDownMapX, m_dMouseDownMapY);
	}
	*/
}

#include "NetworkShapeInsert.h"

void KMapView::SendCommand()
{
}

//^^ #include "TableOwner.h"


void KMapView::OnPrepareChartOD()
{
	ShowMapView();

	if(nullptr != m_spPrepareChartODDlg)
	{
		m_spPrepareChartODDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spPrepareChartODDlg = KPrepareChartODDlgPtr(new KPrepareChartODDlg(m_pTarget, this));
		m_spPrepareChartODDlg->Create(KPrepareChartODDlg::IDD, this);
		m_spPrepareChartODDlg->ShowWindow(SW_SHOW);
	}
}

void KMapView::OnPrepareChartPC()
{
	ShowMapView();

	if(nullptr != m_spPrepareChartPADlg)
	{
		m_spPrepareChartPADlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spPrepareChartPADlg = KPrepareChartPADlgPtr(new KPrepareChartPADlg(m_pTarget, this));
		m_spPrepareChartPADlg->Create(KPrepareChartPADlg::IDD, this);
		m_spPrepareChartPADlg->ShowWindow(SW_SHOW);
	}
}

void KMapView::OnPrepareChartNetworkZone()
{
	ShowMapView();

	m_spPrepareChartNetworkDlg = KPrepareChartNetworkPtr(new KPrepareChartNetworkDlg(m_pTarget, TABLE_ZONE, this));
	m_spPrepareChartNetworkDlg->Create(KPrepareChartNetworkDlg::IDD, this);
	m_spPrepareChartNetworkDlg->ShowWindow(SW_SHOW);
	m_spPrepareChartNetworkDlg->ShowInformation();
}

void KMapView::OnPrepareChartNetworkNode()
{
	ShowMapView();

	m_spPrepareChartNetworkDlg = KPrepareChartNetworkPtr(new KPrepareChartNetworkDlg(m_pTarget, TABLE_NODE, this));
	m_spPrepareChartNetworkDlg->Create(KPrepareChartNetworkDlg::IDD, this);
	m_spPrepareChartNetworkDlg->ShowWindow(SW_SHOW);
	m_spPrepareChartNetworkDlg->ShowInformation();
}

void KMapView::OnPrepareChartNetworkLink()
{
	ShowMapView();

	m_spPrepareChartNetworkDlg = KPrepareChartNetworkPtr(new KPrepareChartNetworkDlg(m_pTarget, TABLE_LINK, this));
	m_spPrepareChartNetworkDlg->Create(KPrepareChartNetworkDlg::IDD, this);
	m_spPrepareChartNetworkDlg->ShowWindow(SW_SHOW);
	m_spPrepareChartNetworkDlg->ShowInformation();
}

void KMapView::OnMultiModalAssignResult()
{
	try
	{
		if (!IsExistMultiModalAssignResultFile())
		{
			return;
		}

		ShowMapView();

		if(nullptr != m_spMultimodalResultDlg)
		{
			m_spMultimodalResultDlg->InitializeNode();
			m_spMultimodalResultDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spMultimodalResultDlg = KMultimodalResultDlgPtr(new KMultimodalResultDlg(m_pTarget, this, this));
			m_spMultimodalResultDlg->Create(KMultimodalResultDlg::IDD, this);
			m_spMultimodalResultDlg->InitializeNode();
			m_spMultimodalResultDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

bool KMapView::IsExistMultiModalAssignResultFile()
{
	return ImChampFileExist::MultiModalFileCheckMessage(m_pTarget);
}

void KMapView::OnUpdateMultiModalAssignResult(CCmdUI *pCmdUI)
{
// #if _TRANSIT_EXCEPT_VER
// 	pCmdUI->Enable(FALSE);
// #else
// 	pCmdUI->Enable(TRUE);
// #endif
	pCmdUI->Enable(TRUE);
}

void KMapView::OnInterModalAssignResult()
{
	try
	{
		if (!IsExistInterModalAssignResultFile())
		{
			return;
		}

		ShowMapView();

		if(nullptr != m_spIntermodalResultDlg)
		{
			m_spIntermodalResultDlg.reset();
			m_spIntermodalResultDlg = nullptr;
		}

		m_spIntermodalResultDlg = KInterModalResultDlgPtr(new KInterModalResultDlg(m_pTarget, this, this));
		m_spIntermodalResultDlg->Create(KInterModalResultDlg::IDD, this);
		m_spIntermodalResultDlg->InitializeNode();
		m_spIntermodalResultDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

bool KMapView::IsExistInterModalAssignResultFile()
{
	return ImChampFileExist::InterModalFileCheckMessage(m_pTarget);
}

void KMapView::OnUpdateInterModalAssignResult(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void KMapView::OnNetworkEditNode()
{
    try
    {
        ShowMapView();

        m_spDlgNetworkEdior = KDlgNetworkEditorMainPtr();
        m_spDlgNetworkEdior = KDlgNetworkEditorMainPtr(new KDlgNetworkEditorMain(this, this));
        m_spDlgNetworkEdior->Create(KDlgNetworkEditorMain::IDD, this);
        m_spDlgNetworkEdior->ShowWindow(SW_SHOW);
        m_spDlgNetworkEdior->SetMapView(this);
        m_spDlgNetworkEdior->MapEditCommand(0);
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

void KMapView::OnNetworkEditLink()
{
    try
    {
        ShowMapView();

        m_spDlgNetworkEdior = KDlgNetworkEditorMainPtr();
        m_spDlgNetworkEdior = KDlgNetworkEditorMainPtr(new KDlgNetworkEditorMain(this, this));
        m_spDlgNetworkEdior->Create(KDlgNetworkEditorMain::IDD, this);
        m_spDlgNetworkEdior->ShowWindow(SW_SHOW);
        m_spDlgNetworkEdior->SetMapView(this);
        m_spDlgNetworkEdior->MapEditCommand(1);
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

void KMapView::OnNetworkEditTurn()
{
    try
    {
        ShowMapView();

        m_spDlgNetworkEdior = KDlgNetworkEditorMainPtr();
        m_spDlgNetworkEdior = KDlgNetworkEditorMainPtr(new KDlgNetworkEditorMain(this, this));
        m_spDlgNetworkEdior->Create(KDlgNetworkEditorMain::IDD, this);
        m_spDlgNetworkEdior->ShowWindow(SW_SHOW);
        m_spDlgNetworkEdior->SetMapView(this);
        m_spDlgNetworkEdior->MapEditCommand(3);
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

void KMapView::OnNetworkEditTransit()
{
    try
    {
        ShowMapView();

        m_spDlgNetworkEdior = KDlgNetworkEditorMainPtr();
        m_spDlgNetworkEdior = KDlgNetworkEditorMainPtr(new KDlgNetworkEditorMain(this, this));
        m_spDlgNetworkEdior->Create(KDlgNetworkEditorMain::IDD, this);
        m_spDlgNetworkEdior->ShowWindow(SW_SHOW);
        m_spDlgNetworkEdior->SetMapView(this);
        m_spDlgNetworkEdior->MapEditCommand(4);
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

void KMapView::OnMultiModalTransitPathEdit()
{
	try
	{
		if ( !KDBaseCheckData::GetCheckBusPathInputData(m_pTarget))
		{
			AfxMessageBox(_T("경로 만들기 > 대중교통 > 대중교통 경로 생성을 먼저 실행해 주세요."));
			return;
		}

		ShowMapView();

		if(nullptr != m_spMTransitPathEditDlg)
		{
			m_spMTransitPathEditDlg->InitializeNode();
			m_spMTransitPathEditDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spMTransitPathEditDlg = KMTransitPathEditDlgPtr(new KMTransitPathEditDlg(m_pTarget, this, this));
			m_spMTransitPathEditDlg->Create(KMTransitPathEditDlg::IDD, this);
			m_spMTransitPathEditDlg->InitializeNode();
			m_spMTransitPathEditDlg->ShowWindow(SW_SHOW);
		}
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

bool KMapView::DrawingLinkThemeState()
{
    if (m_spThemeControl == nullptr)
        return false;

    bool bDraw = m_spThemeControl->IsDrawing(ThemeLayerDrawLink);
    return false;
}

void KMapView::NodeLayerSettingChanged()
{
    if (m_spSymbologyThemeLinkDlg != nullptr) {
        m_spSymbologyThemeLinkDlg->NodeLayerSettingChanged();
    }
}

void KMapView::MapSelectionByPolygon(ImChampMapSelectionReceiver* a_pMapFeedback, int a_nActionType, bool a_bIntersectDemarcation)
{
    try
    {      
        m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditSearchPolygon(this, a_pMapFeedback, NODE, a_nActionType, a_bIntersectDemarcation)); 
		m_spScreenDrawEdit->StartEditing();
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

void KMapView::StopMapSelectionByPolygon()
{
    try
    {
    	if (m_spScreenDrawEdit != nullptr)
        {
            m_spScreenDrawEdit.reset();
        }
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

void KMapView::TargetRegionInsert(int a_nTargetRegionID, ImTasTargetRegionReceiver* a_pMapFeedback, KEMLayer a_emLayer)
{
    try
    {      
        m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditTargetRegionInsert(this, a_nTargetRegionID, a_pMapFeedback, a_emLayer));
        m_spScreenDrawEdit->StartEditing();
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

void KMapView::TargetRegionEdit(int a_nTargetRegionID, std::vector<TCoordinate> vecCoordinate, ImTasTargetRegionReceiver* a_pMapFeedback)
{
    try
    {   
        m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditTargetRegionEdit(this, a_nTargetRegionID, vecCoordinate, a_pMapFeedback)); 
        m_spScreenDrawEdit->StartEditing();
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

void KMapView::OnNetworkeditEditlog()
{
    CString strFileName(_T(""));
    strFileName.Format(_T("%s\\network_edit_log.txt"), m_pTarget->GetLocation());

    CFileFind oFileFinder;
    if (oFileFinder.FindFile(strFileName) == FALSE)
    {
        CString strMsg(_T("현재 네트워크에 관련 로그 파일이 존재하지 않습니다.\n네트워크 편집 작업을 진행하면 자동으로 편집 로그 파일이 생성됩니다."));
        AfxMessageBox(strMsg);
        return;
    }

    ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
}


void KMapView::OnUpdateNetworkeditEditlog(CCmdUI *pCmdUI)
{    
    CString strFileName(_T(""));
    strFileName.Format(_T("%s\\network_edit_log.txt"), m_pTarget->GetLocation());

    CFileFind oFileFinder;
    if (oFileFinder.FindFile(strFileName) == FALSE)
    {
        pCmdUI->Enable(FALSE);
    }
    else
    {
        pCmdUI->Enable(TRUE);
    }
}

void KMapView::OnZoneODGroupGeneration()
{
	ShowMapView();

	try
	{
		if (nullptr != m_spZoneODGroupGenerationDlg)
		{
			m_spZoneODGroupGenerationDlg->InitializeStatus();
			m_spZoneODGroupGenerationDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spZoneODGroupGenerationDlg = KZoneODGroupGenerationDlgPtr(new KZoneODGroupGenerationDlg(m_pTarget, this, this));
			m_spZoneODGroupGenerationDlg->Create(KZoneODGroupGenerationDlg::IDD, this);
			m_spZoneODGroupGenerationDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMapView::OnViewTurnVolume()
{
	ShowMapView();

	try
	{
		m_spTurnVolumeViewDlg = KTurnVolumeViewDlgPtr(new KTurnVolumeViewDlg(m_pTarget, this, this));
		m_spTurnVolumeViewDlg->Create(KTurnVolumeViewDlg::IDD, this);
		m_spTurnVolumeViewDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMapView::SetStartLinkID( Integer a_nxMaxLinkID )
{
    m_nxStartLinkID = a_nxMaxLinkID;
}

Integer KMapView::GetStartLinkID()
{   
    if (m_nxStartLinkID <= 0)
        m_nxStartLinkID = 1;

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Link_ID as NxID "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  Link "));
        strSQL.AppendFormat(_T(" Where Link_ID >= %I64d "), m_nxStartLinkID);
        strSQL.Append(_T(" Order By Link_ID "));
    }

    KDBaseConPtr spDBCon  = m_pTarget->GetDBaseConnection();
    KResultSetPtr       spResult = spDBCon->ExecuteQuery(strSQL);
    
    std::set<__int64> setID;
    while (spResult->Next()) {
        __int64 nxID = spResult->GetValueInt64(0);        
        setID.insert(nxID);

        if (setID.find(m_nxStartLinkID) == setID.end()) {
            break;
        }
        else {
            m_nxStartLinkID++;
        }
    }
    
    return m_nxStartLinkID;
}

__int64 KMapView::GetMaxNodeID()
{
    __int64 nxMaxID(0);

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Max(Node_ID) "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  Node "));
    }

    KDBaseConPtr spDBCon  = m_pTarget->GetDBaseConnection();
    KResultSetPtr       spResult = spDBCon->ExecuteQuery(strSQL);

    if (spResult->Next()) {
        nxMaxID = spResult->GetValueI64(0);
    }

    return nxMaxID;
}

__int64 KMapView::GetMaxLinkID()
{
    __int64 nxMaxID(0);

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Max(Link_ID) "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  Link "));
    }

    KDBaseConPtr spDBCon  = m_pTarget->GetDBaseConnection();
    KResultSetPtr       spResult = spDBCon->ExecuteQuery(strSQL);

    if (spResult->Next()) {
        nxMaxID = spResult->GetValueI64(0);
    }

    return nxMaxID;
}

void KMapView::SetStartNodeID( Integer a_nxMaxNodeID )
{
    m_nxStartNodeID = a_nxMaxNodeID;
}


Integer KMapView::GetStartNodeID()
{
    if (m_nxStartNodeID <= 0)
        m_nxStartNodeID = 1;

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  Node_ID as NxID "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  Node "));
        strSQL.AppendFormat(_T(" Where Node_ID >= %I64d "), m_nxStartNodeID);
        strSQL.Append(_T(" Order By Node_ID "));
    }

    KDBaseConPtr spDBCon  = m_pTarget->GetDBaseConnection();
    KResultSetPtr       spResult = spDBCon->ExecuteQuery(strSQL);

    std::set<__int64> setID;
    while (spResult->Next()) {
        __int64 nxID = spResult->GetValueInt64(0);        
        setID.insert(nxID);

        if (setID.find(m_nxStartNodeID) == setID.end()) {
            break;
        }
        else {
            m_nxStartNodeID++;
        }
    }

    return m_nxStartNodeID;
}

void KMapView::OnThemesTerminalAEModeViewer()
{
	ShowMapView();

	try
	{
		m_spTerminalAEModeViewerDlg = KTerminalAEModeViewerDlgPtr(new KTerminalAEModeViewerDlg(m_pTarget, this, this));
		m_spTerminalAEModeViewerDlg->Create(KTerminalAEModeViewerDlg::IDD, this);
		m_spTerminalAEModeViewerDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KMapView::OnUpdateThemesTerminalAEModeViewer(CCmdUI *pCmdUI)
{
	if (KDBaseTerminalAEModeViewer::IsEmptryTerminalResult(m_pTarget))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

int KMapView::GetLayerID( CString strName )
{
    if (strName.CompareNoCase(_T("node")) == 0) {
        return KLayerID::Node();
    }

    if (strName.CompareNoCase(_T("link")) == 0) {
        return KLayerID::Link();
    }

    if (strName.CompareNoCase(_T("zone")) == 0) {
        return KLayerID::Zone();
    }

    return -1;
}

void KMapView::ZoomToFeature( int nLayerID, __int64 nxID, bool bFlash )
{
    ITxFeatureLayerPtr spFLayer = MapGetFeatureLayer(nLayerID);
    if (spFLayer == nullptr)
        return;

    TxFeaturePtr spFeature = spFLayer->GetFeature(nxID);
    if(nullptr == spFeature) 
        return;

    ITxGeometryPtr spGeometry = spFeature->Geometry();
    if (spGeometry == nullptr)
        return;

    if (bFlash == true) {
        ITxSymbolPtr       spSymbol      = FlashSymbol(spGeometry->GeometryType());				
        TxFeatureDispPtr   spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature->TxObjectID, spGeometry)); {
            spFeatureDisp->Symbol(spSymbol);
        }    
        AddFlashFeature( spFeatureDisp );
    }    

    double dDist(0);
    TxEnvelope oEnvelope = spGeometry->Envelope();
    if (spGeometry->GeometryType() != GeoPointType) {
        dDist = QBicGeometry::Length(oEnvelope.GetMinX(), oEnvelope.GetMinY(), oEnvelope.GetMaxX(), oEnvelope.GetMaxY()); 
    }

    if (dDist < 1000) {
        dDist = 1000;
    } else {
        dDist *= 1.3;
    }
    oEnvelope.Offset(dDist);

    MapCommandZoomExtent(oEnvelope);
    MapRefresh();
}

void KMapView::PanToFeature( int nLayerID, __int64 nxID, bool bFlash )
{
    ITxFeatureLayerPtr spFLayer = MapGetFeatureLayer(nLayerID);
    if (spFLayer == nullptr)
        return;

    TxFeaturePtr spFeature = spFLayer->GetFeature(nxID);
    if(nullptr == spFeature) 
        return;

    ITxGeometryPtr spGeometry = spFeature->Geometry();
    if (spGeometry == nullptr)
        return;

    if (bFlash == true) {
        ITxSymbolPtr       spSymbol      = FlashSymbol(spGeometry->GeometryType());				
        TxFeatureDispPtr   spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature->TxObjectID, spGeometry)); {
            spFeatureDisp->Symbol(spSymbol);
        }

        AddFlashFeature( spFeatureDisp );
    }

    double dCx, dCy(0.0); {
        TxEnvelope oEnvelope = spGeometry->Envelope();
        oEnvelope.GetCenter(dCx, dCy);
    }
    
    TxEnvelope oEnvelope; {
        MapGetEnvelope(oEnvelope);
        oEnvelope.Move(dCx, dCy);
    }
    
    MapCommandZoomExtent(oEnvelope);
    MapRefresh();
}

void KMapView::FlashToFeature( int nLayerID, __int64 nxID)
{
	ITxFeatureLayerPtr spFLayer = MapGetFeatureLayer(nLayerID);
	if (spFLayer == nullptr)
		return;

	TxFeaturePtr spFeature = spFLayer->GetFeature(nxID);
	if(nullptr == spFeature) 
		return;

	ITxGeometryPtr spGeometry = spFeature->Geometry();
	if (spGeometry == nullptr)
		return;

	ITxSymbolPtr       spSymbol      = FlashSymbol(spGeometry->GeometryType());				
	TxFeatureDispPtr   spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature->TxObjectID, spGeometry)); {
		spFeatureDisp->Symbol(spSymbol);
	}

	AddFlashFeature( spFeatureDisp );

	MapRefresh();
}

ITxSymbolPtr KMapView::FlashSymbol( EnumTxGeoType enGeoType )
{
    ITxSymbolPtr spSymbol = nullptr;				

    if (GeoPointType == enGeoType || GeoMPointType == enGeoType)
    {
        spSymbol = TxSymbolPointPtr(new TxSymbolPoint(73, 20.0, Gdiplus::Color(255, 255, 0, 128)));
    }
    else if (GeoPolylineType == enGeoType || GeoMPolylineType == enGeoType)
    {
        spSymbol = TxSymbolLinePtr(new TxSymbolLine(3, Gdiplus::Color(255, 0, 128, 0), false));
    }
    else if (GeoPolygonType == enGeoType || GeoMPolygonType == enGeoType)
    {
        spSymbol = TxSymbolPolygonPtr(new TxSymbolPolygon(Gdiplus::Color(255, 255, 255, 128), Gdiplus::Color(255, 128, 255, 255), 2));
    }

    return spSymbol;
}

void KMapView::AddFlashFeature( TxFeatureDispPtr spFeatureDisp )
{
    m_vecFlash.push_back(spFeatureDisp);
}

void KMapView::AddFlashFeature( std::vector<TxFeatureDispPtr>& vecFeatureDisp )
{
    for (size_t i = 0; i < vecFeatureDisp.size(); i++) {
        m_vecFlash.push_back(vecFeatureDisp[i]);
    }
}


ITxFeatureUserLayerPtr KMapView::AddUserLayer( int nLayerID, bool bMemeory /*= true*/ )
{
    //★ 사용자 레이어는 1번 이상을 사용한다.
    if (nLayerID < 1)
        throw 1;

    MapRemoveLayer(nLayerID);
    CString strName(_T("")); {
        strName.Format(_T("UserLayer_%d"), nLayerID);
    }

    CString strCaption(_T("")); {
        strCaption.Format(_T("UserLayer_%d"), nLayerID);
    }

    TxLayerInfoPtr         spLayerInfo(new TxLayerInfo(GeoShapeType, strName, strCaption));
    ITxFeatureUserLayerPtr spLayer = nullptr;

    if (bMemeory == true) {
        spLayer = TxFeatureUserLayerPtr(new TxFeatureUserLayer(spLayerInfo));
    } 
    else {
        spLayer = TxFeatureFileUserLayerPtr(new TxFeatureFileUserLayer(spLayerInfo));
    }
    
    if ( MapAddLayer(spLayer, nLayerID, nLayerID) < 0) {
        return nullptr;
    }

    //★ 레이어 ID 순으로 드로잉 오더 변경
    MapLayerOrderByID();
    return spLayer;
}

void KMapView::NodeGeometry( std::map<__int64, ITxGeometryPtr>& mapGeometry )
{
    ITxFeatureLayerPtr spLayer = MapGetFeatureLayer(KLayerID::Node());
    spLayer->GetGeometry(mapGeometry);
}

void KMapView::UpdateNodeLinkMapDisp(int nLayerID, __int64 TxID, int nType, CString strLblColumn, CString strLblValue)
{
    if (nLayerID == KLayerID::Node() || nLayerID == KLayerID::Link()) {
    }
    else {
        return;
    }
        
    ITxFeatureLayerPtr spLayer = MapGetFeatureLayer(nLayerID);

    if (nType == -1) {
    }
    else {
        if (nLayerID == KLayerID::Node()) {
            KNodeLayerDispInfo oInfo = NodeLayerDispInfo();
            if ( oInfo.TUseDrawType != 0 ) {
                ITxSymbolPtr spSymbol = NodeTypeSymbol(nType);
                if (spSymbol != nullptr)
                    spLayer->FeatureSymbol(TxID, spSymbol);
            }
            else {
                //★ 레이어 심볼
            }

            if (oInfo.TLabelShow != 1)
                return;
        }
        else {
            KLinkLayerDispInfo oInfo = LinkLayerDispInfo();
            if ( oInfo.TUseDrawType != 0 ) {
                ITxSymbolPtr spSymbol = LinkTypeSymbol(nType);
                if (spSymbol != nullptr)
                    spLayer->FeatureSymbol(TxID, spSymbol);
            }
            else {
                //★ 레이어 심볼
            }

            if (oInfo.TLabelShow != 1)
                return;
        }
    }    

    //★ 여기까지 오면 라벨을 드로잉 해야한다.
    //★ 라벨에 대한 처리    
    //-- CString strLblColumn = oInfo.TLabelField;
    if (strLblColumn.GetLength() == 0) {
        TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLblValue));
        spLayer->FeatureLabel(TxID, spLabel);
        return;
    }

    if (strLblValue.GetLength() == 0) {
        TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLblValue));
        spLayer->FeatureLabel(TxID, spLabel);
        return;
    }

    KIOTables*  pIOTables = m_pTarget->Tables();
    KIOTable*   pIOTable  = nullptr;
    
    if (nLayerID == KLayerID::Node()) 
        pIOTable = pIOTables->FindTable(TABLE_NODE);
    else 
        pIOTable = pIOTables->FindTable(TABLE_LINK);

    KIOColumns* pColumns   = pIOTable->Columns();
    KIOColumn*  pLblColumn = pColumns->GetColumn(strLblColumn);

    if (pLblColumn == nullptr) {
        TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLblValue));
        spLayer->FeatureLabel(TxID, spLabel);
        return;
    }

    CString strLbl(_T(""));    
    std::map<int, CString> mapCode;
    bool bLabelDbl(false);
    bool bLabelCode(false);

    if (pLblColumn != nullptr) {
        if (pLblColumn->DataType() == KEMIODataTypeDouble) {
            bLabelDbl = true;
        }
        else {
            if (pLblColumn->CodeType() == KEMIOCodeTypeIsSingleCode) {
                int nCodeGrp = pLblColumn->CodeGroup();
                KCodeManager* pCodeMgr = m_pTarget->CodeManager();
                KCodeGroup*   pCodeGrp = pCodeMgr->FindCodeGroup(nCodeGrp);
                if (pCodeGrp != nullptr) {
                    pCodeGrp->GetCodes(mapCode);
                    bLabelCode = true;
                }
            }
        }
    }

    if (bLabelDbl) {
        double  dValue = _ttof(strLblValue);
        int nPrecision = QBicFormat::Precision(dValue); 
        CString strFormat(_T("")); {
            if (0 <= nPrecision && nPrecision <= 2) {
                strFormat.Format(_T("%%.%df"), 2);
            } else if (2 < nPrecision) {
                strFormat.Format(_T("%%.%df"), 1);
            } else {
                strFormat.Format(_T("%%.%df"), abs(nPrecision)+2);
            }
        }

        strLbl.Format(strFormat, dValue);
        TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLbl));
        spLayer->FeatureLabel(TxID, spLabel);
        return; //★//
    }

    if (bLabelCode == true) {
        int nCode = _ttoi(strLblValue);
        
        auto iter = mapCode.find(nCode);
        if (iter != mapCode.end()) {
            strLbl.Format(_T("%d:%s"), nCode, iter->second);
        } else {
            strLbl.Format(_T("%d"), nCode);
        }      
        
        TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLbl));
        spLayer->FeatureLabel(TxID, spLabel);
        return; //★//
    }

    //★ 문자로 보낸다.
    TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLblValue));
    spLayer->FeatureLabel(TxID, spLabel);
}



/*****************************************************************************
* 2019.03.08 : 지도 배경색상 변경기능 추가
*****************************************************************************/
void KMapView::SetBackgroundColor(int nBGMode, Gdiplus::Color crBackground)
{
	int nPrevMode = m_nBackgroundMode;
	Gdiplus::Color crPrev = m_crBackground;
	m_nBackgroundMode = nBGMode;
	m_crBackground = crBackground;
	if (!UpdateDisplayOptionBackgroundColor())
	{
		m_nBackgroundMode = nPrevMode;
		m_crBackground = crPrev;
		AfxMessageBox(_T("지도 배경색 설정 저장을 실패했습니다."));
		return;
	}

	ThunderMapPtr spMap = m_ThunderMapCtrl.GetThunderMap();
	if (nullptr != spMap)
	{
		spMap->SetBackgroundColor(m_crBackground);
		spMap->Refresh();
	}
}


Gdiplus::Color KMapView::GetBackgroundColor()
{
	return m_crBackground;
}


bool KMapView::IsDisplayOptionTableExists()
{
	if (nullptr == m_pTarget)
	{
		return false;
	}

	KDBaseConPtr spConn = m_pTarget->GetDBaseConnection();
	CString strSQL;

	try
	{
		strSQL.Format(_T("SELECT COUNT(*) FROM %s WHERE type = 'table' AND name = '%s'"),
			TABLE_SQLITE_MASTER, TABLE_DISPLAY_OPTION);

		KResultSetPtr spResultSet = spConn->ExecuteQuery(strSQL);
		if (!spResultSet->Next() || spResultSet->GetValueInt(0) == 0)
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		return false;
	}
	return true;
}


bool KMapView::CreateTableDisplayOption()
{
	if (nullptr == m_pTarget)
	{
		return false;
	}

	KDBaseConPtr spConn = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQLCreate;
		strSQLCreate.Format(_T(" CREATE TABLE %s ("
			"	id INTEGER NOT NULL,  "
			"   value VARCHAR2 NOT NULL,   "
			"   mode INTEGER, "
			"   desc VARCHAR2, "
			"   PRIMARY KEY (id))"), TABLE_DISPLAY_OPTION);

		spConn->ExecuteUpdate(strSQLCreate);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		return false;
	}
	return true;
}


void KMapView::ReadDisplayOptionBackgroudColor()
{
	if (nullptr == m_pTarget)
	{
		return;
	}

	if (!IsDisplayOptionTableExists())
	{
		return;
	}

	KDBaseConPtr spConn = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL;
		strSQL.Format(_T(" SELECT id, value, mode FROM %s WHERE id = 1"), TABLE_DISPLAY_OPTION);

		KResultSetPtr pResult = spConn->ExecuteQuery(strSQL);
		if (pResult == NULL)
		{
			return;
		}

		while (pResult->Next())
		{
			int id = pResult->GetValueInt(0);
			CString strColor = pResult->GetValueString(1);
			m_nBackgroundMode = pResult->GetValueInt(2);

			std::vector<CString> arrARGB = QBicSplit::Split(strColor, _T(','));
			if (arrARGB.size() == 4) {
				int nA = _ttoi(arrARGB[0]);
				int nR = _ttoi(arrARGB[1]);
				int nG = _ttoi(arrARGB[2]);
				int nB = _ttoi(arrARGB[3]);
				m_crBackground = Gdiplus::Color(nA, nR, nG, nB);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


bool KMapView::UpdateDisplayOptionBackgroundColor()
{
	if (nullptr == m_pTarget)
	{
		return false;
	}

	// 테이블 존재 여부 체크 후 테이블이 없으면 생성
	if (!IsDisplayOptionTableExists() && !CreateTableDisplayOption())
	{
		return false;
	}

	KDBaseConPtr spConn = m_pTarget->GetDBaseConnection();
	CString strSQL;

	try
	{
		CString strValue;
		strValue.Format(_T("%d,%d,%d,%d"), m_crBackground.GetAlpha(), m_crBackground.GetRed(),
			m_crBackground.GetGreen(), m_crBackground.GetBlue());

		strSQL.Format(_T("INSERT OR REPLACE INTO %s(id, value, mode, desc) VALUES(1, '%s', %d, 'map background color')"),
			TABLE_DISPLAY_OPTION, strValue, m_nBackgroundMode);

		spConn->ExecuteUpdate(strSQL);

	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		return false;
	}
	return true;
}



void KMapView::OnSetMapBackgroundColor()
{
	//KMapBackgroundColorDlg dlg;
	//dlg.SetMapBackgroundColorMode(m_nBackgroundMode);
	//dlg.SetMapBackgroundColor(m_crBackground);
	//if (dlg.DoModal() == IDOK)
	//{
	//	int nBGMode = dlg.GetMapBackgroundColorMode();
	//	Gdiplus::Color cr = dlg.GetMapBackgroundColor();

	//	SetBackgroundColor(nBGMode, cr);
	//}

	CXTPColorDialog dlg(m_crBackground.ToCOLORREF(), m_crBackground.ToCOLORREF());

	if (dlg.DoModal() == IDOK)
	{
		Gdiplus::Color cr;
		cr.SetFromCOLORREF(dlg.GetColor());
		SetBackgroundColor(1, cr);
	}
}


afx_msg void KMapView::OnUpdateSetMapBackgroundColor(CCmdUI* pCmd)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CXTPControlPopupColor* pPopup = (CXTPControlPopupColor*)pToolBar->GetControls()->GetAt(pCmd->m_nIndex);
		pPopup->SetColor(m_crBackground.ToCOLORREF());
	}

	pCmd->Enable(TRUE);
}


void KMapView::OnBackgroundNoFill()
{
	Gdiplus::Color cr = Gdiplus::Color::White;
	SetBackgroundColor(1, cr);
}


void KMapView::OnBackgroundMoreColors()
{
	CXTPColorDialog dlg(m_crBackground.ToCOLORREF(), m_crBackground.ToCOLORREF());
	
	if (dlg.DoModal() == IDOK)
	{
		Gdiplus::Color cr;
		cr.SetFromCOLORREF(dlg.GetColor());
		SetBackgroundColor(1, cr);
	}
}


void KMapView::OnBackgroundSelector(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
	Gdiplus::Color cr;
	cr.SetFromCOLORREF(pControl->GetColor());

	SetBackgroundColor(1, cr);
}


void KMapView::OnUpdateBackgroundSelector(CCmdUI* pCmd)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CXTPControlColorSelector* pSelector = (CXTPControlColorSelector*)pToolBar->GetControls()->GetAt(pCmd->m_nIndex);
		pSelector->SetColor(m_crBackground.ToCOLORREF());
	}

	pCmd->Enable(TRUE);
}


/****************************************************************************/