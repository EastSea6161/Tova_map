#include "StdAfx.h"
#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "ScreenDrawEditTargetRegionInsert.h"
#include "TDrawScreenCordonInfo.h"
#include "BulkDBaseScreenCordon.h"
#include "NameInputDlg.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"

#include "BulkDBaseTargetRegion.h"
#include "Target.h"
#include "TCoordinate.h"


KScreenDrawEditTargetRegionInsert::KScreenDrawEditTargetRegionInsert( IMapView* a_mapRequest, int a_nTargetRegionID, ImTasTargetRegionReceiver* a_pMapFeedback, KEMLayer a_emLayer)
                      : IScreenDrawEdit(a_mapRequest), m_pMapFeedback(a_pMapFeedback), m_emLayer(a_emLayer)
{
    m_emEditStatus    = KEMTargetRegionInsertNothing;    
    m_nTargetRegionID = a_nTargetRegionID;

    m_spSymbolLine = TxSymbolLine::CreateLineSymbol();
    m_spSymbolPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);
}

KScreenDrawEditTargetRegionInsert::~KScreenDrawEditTargetRegionInsert( void )
{
	StopEditing();
}

void KScreenDrawEditTargetRegionInsert::StartEditing()
{
    try
    {
        m_bScreenDrawEditing = true;
        // 명칭을 받고 -> 시점 입력 상태
        m_emEditStatus = KEMTargetRegionInsertNothing;    

        FirstPointCommandMsg();
        m_pIMapView->SetMapMode(KEMMapModeScreenDrawEdit);
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

void KScreenDrawEditTargetRegionInsert::Draw()
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

void KScreenDrawEditTargetRegionInsert::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    m_vecPt.push_back(TxPoint(mapX, mapY));

    m_emEditStatus = KEMTargetRegionInserting;
    AddPointCommandMsg();
}

void KScreenDrawEditTargetRegionInsert::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
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

void KScreenDrawEditTargetRegionInsert::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    if (m_emEditStatus == KEMTargetRegionInserting)
        SaveEditing();    
}


void KScreenDrawEditTargetRegionInsert::Sleep()
{
}

void KScreenDrawEditTargetRegionInsert::CommandFeedback( CString a_strResult )
{
    if (a_strResult.CompareNoCase(_T("C")) == 0)
    {
        StopEditing();
    }
}

void KScreenDrawEditTargetRegionInsert::Canceled()
{
	SendLogMsg(_T("취소"));
    StopEditing();
}

void KScreenDrawEditTargetRegionInsert::FirstPointCommandMsg()
{
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("시점 선택(지도), 취소(C):"), vecSelectableInfo, this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KScreenDrawEditTargetRegionInsert::AddPointCommandMsg()
{    
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("절점 선택(지도), 취소(C):"), vecSelectableInfo, this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);
}

void KScreenDrawEditTargetRegionInsert::SaveEditing()
{
    try
    {
        if (m_vecPt.size() < 3) {
            StartEditing();
            return;
        }
               
        std::vector<TxPoint> vecPt = m_vecPt; {
            vecPt.push_back(m_vecPt[0]); //★ 시점 추가
        }

        m_vecPt.clear();
        m_spGeoDraw = nullptr;   

        TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPt));
        FeedbackIntersectInfo(spPolygon);
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

void KScreenDrawEditTargetRegionInsert::FeedbackIntersectInfo(TxPolygonPtr spPolygon)
{
    if (m_pMapFeedback == nullptr)
        return;

    if (spPolygon == nullptr)
        return;
    
    try 
    {
        ITxFeatureLayerPtr spLayer = nullptr;

        if (m_emLayer == NODE) {
            spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
        }
        else if (m_emLayer == LINK) {
            spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
        }

        if (spLayer == nullptr)
            return;

        TxFeatureLayerPtr  spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
        if (spFLayer == nullptr)
            return;

        TxEnvelope oEnvelope = spPolygon->Envelope();

        std::vector<TxFeaturePtr> vecEnvFeature;
        spFLayer->FindFeature(oEnvelope, vecEnvFeature);

        std::vector<__int64> vecID;
        for (auto iter = vecEnvFeature.begin(); iter != vecEnvFeature.end(); ++iter) 
        {            
            TxFeaturePtr spFeature = *iter;
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            if (spGeometry->GeometryType() == GeoPointType) {
                TxPointPtr spPoint = std::dynamic_pointer_cast<TxPoint>(spGeometry);

                if (spPolygon->IsInPoint(spPoint)) {
                    vecID.push_back(spFeature->TxObjectID);
                }
            } else if (spGeometry->GeometryType() == GeoPolylineType) {
                TxPolylinePtr spPLine = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
                if (spPolygon->Intersect(spPLine) == true) {
                    vecID.push_back(spFeature->TxObjectID);
                }
            }
        }
        
        if (m_emLayer == NODE) {
            std::vector<TxPoint> vecPt = spPolygon->OutRing();
            std::vector<TCoordinate> vecPoints;
            for (size_t i = 0; i < vecPt.size(); i++) {
                TxPoint& oPt = vecPt[i];
                vecPoints.push_back(TCoordinate(oPt.X, oPt.Y));
            }

            m_pMapFeedback->TargetRegionFeedback(vecID, vecPoints);
        }
        else if (m_emLayer == LINK) {
            std::vector<TCoordinate> vecEmpty;
            m_pMapFeedback->TargetRegionFeedback(vecID, vecEmpty);
        }
    } catch (...) {
        TxLogDebugException();
    }
}


void KScreenDrawEditTargetRegionInsert::StopEditing()
{
	try
	{
		m_emEditStatus        = KEMTargetRegionInsertNothing;
		m_bScreenDrawEditing  = false;
        m_vecPt.clear();
        m_spGeoDraw = nullptr;   

		m_pIMapView->SetMapMode(KEMMapModePan);

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
