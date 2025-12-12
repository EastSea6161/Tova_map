#include "stdafx.h"
#include "Project.h"
#include "MapView.h"
#include "IncludeMap.h"

#include "Target.h"

#include "ShortestPathHighwayDlg.h"
#include "ShortestPathNTransitDlg.h"

void KMapView::SelectNodePoint(IMapNodeSearch* a_pMapNodeSeach)
{
    m_pMapNodeSearch = a_pMapNodeSeach;
    SetMapMode(KEMMapModeSelectNode);

    AfxMessageBox(_T("±¸ÇöÁß"));
}

void KMapView::CloseMapNodeSearch()
{
    if (m_emMapMode == KEMMapModeSelectNode)
    {
        SetMapMode(m_emPrevMapMode);
    }

    m_pMapNodeSearch = NULL;
}


KEMMapMode KMapView::GetCurrentMapMode()
{
	return m_emMapMode;
}


void KMapView::SelectNodePoint( long shift, long x, long y, double mapX, double mapY )
{
}


void KMapView::DrawShortestPath(std::vector<TPathInfo> &a_vecPath, bool a_bAutoOffset )
{
    IThemeLayerPtr                  spScreenDraw            = m_spThemeControl->GetThemeLayer(ThemeLayerDrawHighwayShortestPath);    
    KThemeDrawHighwayShortestPath* pScreenDrawShortestPath = (KThemeDrawHighwayShortestPath*)spScreenDraw.get();

    pScreenDrawShortestPath->Draw(a_vecPath);
	MapRefresh();
}

void KMapView::ClearShortestPath()
{
	if (m_spThemeControl)
		m_spThemeControl->Remove(ThemeLayerDrawHighwayShortestPath); 
	
    MapRefresh();
}

void KMapView::DrawTransitShortestPath(std::vector<TTransitRouteInfo> &a_vecRouteInfo)
{
    IThemeLayerPtr                  spScreenDraw            = m_spThemeControl->GetThemeLayer(ThemeLayerDrawHighwayShortestPath);    
    KThemeDrawTransitShortestPath* pScreenDrawShortestPath = (KThemeDrawTransitShortestPath*)spScreenDraw.get();

    pScreenDrawShortestPath->Draw(a_vecRouteInfo);
    MapRefresh();    
}

void KMapView::ClearTransitShortestPath()
{
	if (m_spThemeControl)
		m_spThemeControl->Remove(ThemeLayerDrawTransitShortestPath); 
	
    MapRefresh();
}

void KMapView::OnShortestpathHighway()
{
	ShowMapView();

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);

	KIOTable* pIOTable = pIOTables->FindTable(TABLE_LINK);
	ASSERT(NULL != pIOTable);

	if(NULL != m_spShortestPathHighwayDlg)
	{
		m_spShortestPathHighwayDlg->InitializeNode();
		m_spShortestPathHighwayDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spShortestPathHighwayDlg = KShortestPathHighwayDlgPtr(new KShortestPathHighwayDlg(this));
		m_spShortestPathHighwayDlg->SetLinkTable(pIOTable);
		m_spShortestPathHighwayDlg->SetTarget(m_pTarget);
		m_spShortestPathHighwayDlg->SetMapView(this);
		m_spShortestPathHighwayDlg->Create(KShortestPathHighwayDlg::IDD, this);
		m_spShortestPathHighwayDlg->InitializeNode();
		m_spShortestPathHighwayDlg->ShowWindow(SW_SHOW);
	}
}


void KMapView::OnShortestpathTransit()
{
	ShowMapView();

	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(nullptr != pIOTables);

	KIOTable*  pIOTable = pIOTables->FindTable(TABLE_LINK);
	ASSERT(nullptr != pIOTable);

	if(nullptr != m_spShortestPathNTransitDlg)
	{
		m_spShortestPathNTransitDlg->InitializeNode();
		m_spShortestPathNTransitDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spShortestPathNTransitDlg = KShortestPathNTransitDlgPtr(new KShortestPathNTransitDlg(this));
		m_spShortestPathNTransitDlg->SetLinkTable(pIOTable);
		m_spShortestPathNTransitDlg->SetTarget(m_pTarget);
		m_spShortestPathNTransitDlg->SetMapView(this);
		m_spShortestPathNTransitDlg->Create(KShortestPathNTransitDlg::IDD, this);
		m_spShortestPathNTransitDlg->InitializeNode();
		m_spShortestPathNTransitDlg->ShowWindow(SW_SHOW);
	}
}


void KMapView::OnUpdateShortestpathTransit(CCmdUI *pCmdUI)
{
// #if _TRANSIT_EXCEPT_VER
// 	pCmdUI->Enable(FALSE);
// #else
// 	pCmdUI->Enable(TRUE);
// #endif
	pCmdUI->Enable(TRUE);
}


void KMapView::OnFShortestpathHighway()
{
}


void KMapView::OnFShortestpathRail()
{
	
}


void KMapView::OnFShortestpathViaFacilities()
{
}


void KMapView::OnFShortestpathTotal()
{
}

void KMapView::OnFLinkModeODVolume()
{
}
