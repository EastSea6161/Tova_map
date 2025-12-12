#include "stdafx.h"
#include "ScreenDrawEditSearchRect.h"

#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "ScreenDrawEditSearchPolygon.h"
#include "TDrawScreenCordonInfo.h"
#include "BulkDBaseScreenCordon.h"
#include "NameInputDlg.h"

KScreenDrawEditSearchRect::KScreenDrawEditSearchRect( IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapFeedback, KEMLayer a_emLayer )
    : IScreenDrawEdit(a_mapRequest), m_pMapFeedback(a_pMapFeedback), m_emLayer(a_emLayer)
{ 
    m_spSymbolPoly = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(100, 0,0,255), Gdiplus::Color::Black);
}

KScreenDrawEditSearchRect::~KScreenDrawEditSearchRect( void )
{
    StopEditing();
}

void KScreenDrawEditSearchRect::StartEditing()
{
    try
    {
        m_spPtFirst.reset();
        m_spRect.reset();

        m_bScreenDrawEditing = true;
        FirstPointCommandMsg();
        m_pIMapView->SetMapMode(KEMMapModeMapSelectRect);
    }
    catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    }
    catch (...) {
        TxLogDebugException();
    }    
}

void KScreenDrawEditSearchRect::CommandFeedback( CString a_strResult )
{
    if (a_strResult.CompareNoCase(_T("C")) == 0)
    {
        StopEditing();
    }
}

void KScreenDrawEditSearchRect::Canceled()
{
    SendLogMsg(_T("선택 취소"));
    StopEditing();
}

void KScreenDrawEditSearchRect::Draw()
{
    if (m_bScreenDrawEditing == false)
        return;

    if (m_spRect == nullptr)
        return;

    TxFeatureDispPtr spFeatureDisp; {        
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spRect));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KScreenDrawEditSearchRect::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    m_spPtFirst = TxPointPtr(new TxPoint(mapX, mapY));
}

void KScreenDrawEditSearchRect::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_bScreenDrawEditing == false)
        return;

    if (MK_MBUTTON & nFlags)
        return;

    if (m_spPtFirst == nullptr)
        return;
    
    TxPointPtr spPtMove = TxPointPtr(new TxPoint(mapX, mapY));
    m_spRect = TxSpatialOper::RectPoly(m_spPtFirst, spPtMove);

    TxFeatureDispPtr spFeatureDisp; {        
        spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(1, m_spRect));
        spFeatureDisp->Symbol(m_spSymbolPoly);
    }

    m_pIMapView->MapAddForegroundLayer(spFeatureDisp);
    m_pIMapView->MapForegroundRefresh();
}

void KScreenDrawEditSearchRect::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spPtFirst == nullptr) {
        return;
    }
    else {
        m_pIMapView->MapForegroundRefresh();
    }

    if (m_spRect == nullptr) {
        TxPointPtr spPtMove = TxPointPtr(new TxPoint(mapX, mapY));
        m_spRect = TxSpatialOper::RectPoly(m_spPtFirst, spPtMove);
    }

    TxEnvelope oEnvelope = m_spRect->Envelope();
    ITxFeatureLayerPtr   spLayer;
    std::vector<__int64> vecID;

    if (m_emLayer == NODE) {
        spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());        
    } 
    else if(m_emLayer == LINK) {
        spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link()); 
    }

    if (spLayer == nullptr)
        return;

    std::vector<TxFeaturePtr> vecFeature;
    spLayer->FindFeature(oEnvelope, vecFeature);

    for (size_t i=0; i<vecFeature.size(); i++) {
        TxFeaturePtr spFeature = vecFeature[i];
        vecID.push_back(spFeature->TxObjectID);
    }

    if (m_pMapFeedback) {
        try {
            m_pMapFeedback->MapSelectionFeedback(vecID);
        }
        catch (KExceptionPtr ex) {
            TxExceptionPrint(ex);
        }
        catch (...) {
            TxLogDebugException();
        }            
    }

    StartEditing();
}


void KScreenDrawEditSearchRect::FirstPointCommandMsg()
{
    std::vector<CString> vecSelectableInfo;
    vecSelectableInfo.push_back(_T("C"));

    KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
    ImChampModalCommandPtr spCommand(new ImChampCommand(_T("시점 선택(지도)후 마우스 드래그, 취소(C):"), vecSelectableInfo, this));
    if (pCommandWindow)
        pCommandWindow->Command(spCommand, KEMModeless);
}


void KScreenDrawEditSearchRect::SaveEditing()
{
}

void KScreenDrawEditSearchRect::StopEditing()
{
    try
    {
        m_bScreenDrawEditing = false;        
        m_pIMapView->SetMapMode(KEMMapModePan);

        m_spPtFirst    = nullptr;
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


