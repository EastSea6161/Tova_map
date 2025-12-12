#include "StdAfx.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "ScreenDrawEditSearchPolygon.h"
#include "TDrawScreenCordonInfo.h"
#include "BulkDBaseScreenCordon.h"
#include "NameInputDlg.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"


KScreenDrawEditSearchPolygon::KScreenDrawEditSearchPolygon( IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapFeedback, KEMLayer a_emLayer, int a_nActionType, bool a_bIntersectDemarcation )
                      : IScreenDrawEdit(a_mapRequest), m_pMapFeedback(a_pMapFeedback), m_emLayer(a_emLayer), m_nActionType(a_nActionType), m_bIntersectDemarcation(a_bIntersectDemarcation)
{ 
    m_spSymbolLine = TxSymbolLine::CreateLineSymbol();
    m_spSymbolPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);
}

KScreenDrawEditSearchPolygon::~KScreenDrawEditSearchPolygon( void )
{
	StopEditing();
}

void KScreenDrawEditSearchPolygon::StartEditing()
{
    try
    {
        m_bScreenDrawEditing = true;
        m_pIMapView->SetMapMode(KEMMapModeMapSelectPoly);
        m_pIMapView->MapForegroundRefresh();
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


void KScreenDrawEditSearchPolygon::CommandFeedback( CString a_strResult )
{
}

void KScreenDrawEditSearchPolygon::Canceled()
{
	SendLogMsg(_T("선택 취소"));
    StopEditing();
}


void KScreenDrawEditSearchPolygon::SaveEditing()
{
    try
    {
        if (m_vecPt.size() < 3) {
            StartEditing();
            return;
        }
                
        ITxFeatureLayerPtr   spLayer;
        std::vector<__int64> vecID;

        if (m_emLayer == NODE) {
            spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());        
        } 
        else if(m_emLayer == LINK) {
            spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link()); 
        }

        if (spLayer == nullptr) {
            StopEditing();
            return;
        }
        
        std::vector<TxPoint> vecPt = m_vecPt; {
            vecPt.push_back(m_vecPt[0]); //★ 시점 추가
        }

        TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPt));
        if (m_nActionType == 2)
        {            
            SaveEditingIntersectLink(spPolygon);
            StopEditing();
            return;
        }

        TxEnvelope   oEnvelope = spPolygon->Envelope();        
        std::vector<TxFeaturePtr> vecFeature;
        spLayer->FindFeature(oEnvelope, vecFeature);

        for (size_t i=0; i<vecFeature.size(); i++) {
            TxFeaturePtr spFeature = vecFeature[i];
            __int64 nxID = spFeature->TxObjectID;
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            if (spGeometry->GeometryType() == GeoPointType) {
                TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
                if (spPolygon->Intersect(spPoint) == true) {
                    vecID.push_back(nxID);
                }
            } else if (spGeometry->GeometryType() == GeoPolylineType) {
                TxPolylinePtr spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
                if (spPolygon->Intersect(spPLine) == true) {
                    vecID.push_back(nxID);
                }
            }
        }

        if (m_pMapFeedback) {
            try 
            {
                m_pMapFeedback->MapSelectionFeedback(vecID);
            } catch (KExceptionPtr ex) {
                TxExceptionPrint(ex);
            } catch (...)            {
                TxLogDebugException();
            }            
        }
                    
        //★ 0 : 노드, 링크 ID Search
        StartEditing();

        //★ 1, 2 : Stop Editing -> Return  
        // SubNetwork에서 사용하고 있음 : 이부분은 나중에 수정해야 함(2016-04-26)      
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


void KScreenDrawEditSearchPolygon::StopEditing()
{
	try
	{
		m_bScreenDrawEditing = false;
        m_vecPt.clear();
        m_spGeoDraw = nullptr;   

		m_pIMapView->SetMapMode(KEMMapModePan);

        m_pMapFeedback = nullptr;

		KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
		if (pCommandWindow)
			pCommandWindow->SetDefaultCommand();
		
        m_pIMapView->MapForegroundRefresh();
	}
	catch (...)
	{
		TxLogDebugException();
	}    
}


void KScreenDrawEditSearchPolygon::Draw()
{
    if (m_bScreenDrawEditing == false)
        return;

    if (m_spGeoDraw == nullptr)
        return;

    TxFeatureDispPtr spFeatureDisp; 
    if (m_spGeoDraw->GeometryType() == GeoPolylineType) {
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolLine);
    } else {
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KScreenDrawEditSearchPolygon::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    m_vecPt.push_back(TxPoint(mapX, mapY));
}

void KScreenDrawEditSearchPolygon::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
}

void KScreenDrawEditSearchPolygon::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    SaveEditing();
    m_vecPt.clear();
    m_spGeoDraw = nullptr;    
}

void KScreenDrawEditSearchPolygon::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    if (MK_MBUTTON & nFlags)
        return;

    if (m_vecPt.size() < 1)
        return;

    std::vector<TxPoint> vecPt = m_vecPt; {
        vecPt.push_back(TxPoint(mapX, mapY));
    }

    TxFeatureDispPtr spFeatureDisp; 
    size_t nxCnt = vecPt.size();
    if (nxCnt == 2) {
        m_spGeoDraw  = TxPolylinePtr(new TxPolyline(vecPt));
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolLine);
    } else {
        vecPt.push_back(m_vecPt[0]); //★ 시점추가
        m_spGeoDraw = TxPolygonPtr(new TxPolygon(vecPt));
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spGeoDraw));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }
    
    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KScreenDrawEditSearchPolygon::OnMapRButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
}

void KScreenDrawEditSearchPolygon::OnMapRButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
}


void KScreenDrawEditSearchPolygon::SaveEditingIntersectLink( TxPolygonPtr spPoly )
{
    try
    {
        std::set<__int64> setInNode = UnionInNode(spPoly);
        if (setInNode.size() == 0)
            return;

        std::map<Integer, TLinkFTNodeID> mapLinkFTNodeID;
        KBulkDBaseLink::AllLinkFTNode(m_pIMapView->GetTarget(), mapLinkFTNodeID);
        auto iter = mapLinkFTNodeID.begin();
        auto end  = mapLinkFTNodeID.end();

        std::vector<TReceiveValue> vecReceiveValue;
        while (iter != end)
        {
            TLinkFTNodeID& oLinkFTNodeID = iter->second;

            TReceiveValue oReceiveValue;
            oReceiveValue.m_nxLinkID  = oLinkFTNodeID.TLinkID;

            oReceiveValue.m_nxFNodeID = oLinkFTNodeID.TFromNodeID;
            if (setInNode.find(oLinkFTNodeID.TFromNodeID) != setInNode.end()) {
                oReceiveValue.m_nFNodeInOut = 1;
            }
            else {
                oReceiveValue.m_nFNodeInOut = 0;
            }

            oReceiveValue.m_nxTNodeID = oLinkFTNodeID.TToNodeID;
            if (setInNode.find(oLinkFTNodeID.TToNodeID) != setInNode.end()) {
                oReceiveValue.m_nTNodeInOut = 1;
            }
            else {
                oReceiveValue.m_nTNodeInOut = 0;
            }

            vecReceiveValue.push_back(oReceiveValue);

            ++iter;
        }

        if (m_pMapFeedback)
        {
            try
            {
                m_pMapFeedback->MapAnalysisFeedback(vecReceiveValue);
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

std::set<__int64> KScreenDrawEditSearchPolygon::UnionInNode( TxPolygonPtr spPoly )
{
    std::set<__int64> setInNode;
    std::vector<TxPolygonPtr> vecPoly; 
    
    if (m_bIntersectDemarcation) {
        ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
        if (spLayer == nullptr)
            return setInNode;

        std::vector<TxFeaturePtr> vecFeature;
        spLayer->FindFeature(spPoly->Envelope(), vecFeature);

        for (size_t i=0; i<vecFeature.size(); i++)
        {
            TxFeaturePtr spFeature = vecFeature[i];

            ITxGeometryPtr spGeometry = spFeature->Geometry();
            TxPolygonPtr   spPolyB    = std::dynamic_pointer_cast<TxPolygon>(spGeometry);

            if (spPolyB == nullptr)
                continue;

            if (spPoly->Intersect(spPolyB) == false)
                continue;

            vecPoly.push_back(spPolyB);
        }
    }

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node()); {
        if (spLayer == nullptr)
            return setInNode;

        std::vector<TxFeaturePtr> vecFeature;
        spLayer->FindFeature(spPoly->Envelope(), vecFeature);
        for (size_t i=0; i<vecFeature.size(); i++)
        {
            TxFeaturePtr   spFeature  = vecFeature[i];
			ITxGeometryPtr spGeometry = spFeature->Geometry();

			TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
			bool bR = spPoly->IsInPoint(spPoint);
			if (bR)
				setInNode.insert(spFeature->TxObjectID);
        }
    }
    
    for (size_t i=0; i<vecPoly.size(); i++) {
        TxPolygonPtr spPoly = vecPoly[i];
        std::vector<TxFeaturePtr> vecFeature;
        spLayer->FindFeature(spPoly->Envelope(), vecFeature);
        for (size_t i=0; i<vecFeature.size(); i++)
        {
			TxFeaturePtr   spFeature  = vecFeature[i];
			ITxGeometryPtr spGeometry = spFeature->Geometry();

			TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);
			bool bR = spPoly->IsInPoint(spPoint);
			if (bR)
				setInNode.insert(spFeature->TxObjectID);
        }
    }
    
    return setInNode;
}
