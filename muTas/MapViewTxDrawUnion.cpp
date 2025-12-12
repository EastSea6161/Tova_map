#include "stdafx.h"
#include "MapView.h"

#include "IncludeMap.h"
#include "IncludeScreenDraw.h"
#include "Target.h"

#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"

void KMapView::TxDrawCompositeNode(std::vector<TDrawNodeInfo>& a_vecDrawNodeInfo, bool a_bDrawLabel, bool a_bRefresh, double a_dDrawScale)
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw) {
        pScreenDraw->SetNodeDraw(a_vecDrawNodeInfo, a_bDrawLabel, a_dDrawScale);
    }

    if (a_bRefresh)
        MapRefresh();
}

void KMapView::TxDrawCompositeNodeLabel(bool a_bDrawLabel/* =true */, bool a_bRefresh)
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->SetShowNodeLabel(a_bDrawLabel);
        if (a_bRefresh)
            MapRefresh();
    }
}

void KMapView::TxDrawCompositeLink(std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo, bool a_bDrawLabel, bool a_bRefresh, bool bSort, bool bLabelFil)
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->SetLinkDraw(a_vecDrawLinkInfo, a_bDrawLabel, bSort, bLabelFil);
    }

    if (a_bRefresh)
        MapRefresh();
}

void KMapView::TxDrawShowCompositeLinkLablel(bool a_bDrawLabel/* =true */, bool a_bRefresh)
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->SetShowLinkLabel(a_bDrawLabel);
        if (a_bRefresh)
            MapRefresh();
    }
}

void KMapView::TxDrawCompositeUserPoint( std::vector<TUserPointInfo>& ar_vecUserPoint, bool a_bDrawLabel/*=false*/, bool a_bRefresh/*=false*/ )
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->SetUserPointDraw(ar_vecUserPoint, a_bDrawLabel);
    }

    if (a_bRefresh)
        MapRefresh();
}

void KMapView::TxDrawShowCompositeUserPointLabel( bool a_bDrawLabel/*=true*/, bool a_bRefresh )
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->SetShowUserPointLabel(a_bDrawLabel);
        if (a_bRefresh)
            MapRefresh();
    }
}

void KMapView::TxDrawCompositeUserLine(std::vector<TUserLineInfo>& ar_vecUserLine, bool a_bDrawLabel, bool a_bRefresh)
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->SetUserLineDraw(ar_vecUserLine, a_bDrawLabel);
    }

    if (a_bRefresh)
        MapRefresh();
}

void KMapView::TxDrawShowCompositeUserLineLablel(bool a_bDrawLabel/* =true */, bool a_bRefresh)
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->SetShowUserLineLabel(a_bDrawLabel);
        if (a_bRefresh)
            MapRefresh();
    }
}

void KMapView::TxDrawCompositeZoneAccess(std::vector<TDrawZoneAccess>& ar_vecDrawZoneAccess, bool a_bRefresh/* =false */, bool a_bDrawLabel/* =false */)
{
    IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
    KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->SetZoneAccessDraw(ar_vecDrawZoneAccess, a_bDrawLabel);
    }

    if (a_bRefresh)
        MapRefresh();
}

void KMapView::TxClearCompositeThemes(bool bRefresh)
{
    if (m_spThemeControl)
        m_spThemeControl->Remove(ThemeLayerDrawComposite);

    if (bRefresh)
        MapRefresh();
}

void KMapView::TxDrawTransitBoardAlight(std::vector<TTransitPassengerInfo>& ar_vecDrawTransitPassengerInfo, bool a_bDrawLabel, bool a_bRefresh)
{
    try
    {
        IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
        KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
        if (pScreenDraw)
        {
            pScreenDraw->SetTransitPassenger(ar_vecDrawTransitPassengerInfo, a_bDrawLabel);
        }

        if (a_bRefresh)
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

void KMapView::TxDrawShowTransitPassengerLabel(bool a_bDrawLabel, bool a_bRefresh)
{
    try
    {
        IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
        KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
        if (pScreenDraw)
        {
            pScreenDraw->SetTransitPassengerLabel(a_bDrawLabel);
            if (a_bRefresh)
                MapRefresh();
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

