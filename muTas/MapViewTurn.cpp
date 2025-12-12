#include "StdAfx.h"
#include "MapView.h"

#include "IOView.h"
#include "IOTurnTableView.h"

void KMapView::AddTurnEdit( __int64 nxTurnID, bool bRefresh )
{
    KIOTurnTableView* pTurnView = (KIOTurnTableView*)(m_pIOView->FindTableView(_T("turn")));
    if(NULL == pTurnView) {
        return ;
    }

    pTurnView->NotifyTurnCreatedFromMap(nxTurnID);
    if (bRefresh) {
        MapRefresh();
    }
}

void KMapView::AddTurnMapDraw( Integer a_nxTurnID, bool a_bRefresh /*= true*/ )
{
    IThemeLayerPtr   spScreenDraw    = m_spThemeControl->GetThemeLayer(ThemeLayerDrawTurn); 
    KThemeDrawTurn*  pScreenDrawTurn = (KThemeDrawTurn*)spScreenDraw.get();

    pScreenDrawTurn->AddDraw(a_nxTurnID);
    if (a_bRefresh) {
        MapRefresh();
    }
}

void KMapView::RemoveTurnMapDraw( Integer a_nxTurnID, bool a_bRefresh /*= true*/ )
{   
    IThemeLayerPtr    spScreenDraw    = m_spThemeControl->GetThemeLayer(ThemeLayerDrawTurn); 
    KThemeDrawTurn*  pScreenDrawTurn = (KThemeDrawTurn*)spScreenDraw.get();

    pScreenDrawTurn->RemoveDraw(a_nxTurnID);
    if (a_bRefresh) {
		MapRefresh();
    }
}


void KMapView::RemoveTurnMapDraw(bool a_bRefresh)
{
    m_spThemeControl->Remove(ThemeLayerDrawTurn);
    if (a_bRefresh) {
        MapRefresh();
    }
}