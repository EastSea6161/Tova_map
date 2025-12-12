#include "stdafx.h"
#include "MapView.h"

void KMapView::NodeAccent(std::vector<TDrawNodeInfo>& ar_vecDrawNodeInfo, bool a_bDrawLabel, bool a_bRefresh)
{
    IThemeLayerPtr            spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawNodeAccent);    
    KScreenDrawNodeAccent*    pScreenDraw  = (KScreenDrawNodeAccent*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->Draw(ar_vecDrawNodeInfo, a_bDrawLabel);

        if (a_bRefresh)
            TasESRICacheRefresh();
    }
}

void KMapView::ClearNodeAccent(bool a_bRefresh)
{
    m_spThemeControl->Remove(ThemeLayerDrawNodeAccent);

    if (a_bRefresh)
        TasESRICacheRefresh();
}

void KMapView::LinkAccent(std::vector<TDrawLinkInfo>& ar_vecDrawLinkInfo, bool a_bDrawLabel, bool a_bRefresh)
{
    IThemeLayerPtr            spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawLinkAccent);    
    KScreenDrawLinkAccent*    pScreenDraw  = (KScreenDrawLinkAccent*)spScreenDraw.get();
    if (pScreenDraw)
    {
        pScreenDraw->Draw(ar_vecDrawLinkInfo, a_bDrawLabel);

        if (a_bRefresh)
            TasESRICacheRefresh();
    }
}

void KMapView::ClearLinkAccent(bool a_bRefresh)
{
    m_spThemeControl->Remove(ThemeLayerDrawLinkAccent);

    if (a_bRefresh)
        TasESRICacheRefresh();
}