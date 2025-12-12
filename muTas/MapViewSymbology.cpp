#include "stdafx.h"
#include "MapView.h"
#include "SymbologyThemeNodeDlg.h"
#include "SymbologyThemeLinkDlg.h"
#include "SymbologyThemeZoneDlg.h"
#include "DesireLineDlg.h"
#include "ShortestPathNTransitDlg.h"
#include "ShortestPathHighwayDlg.h"

void KMapView::OnClearThemeNode()
{
	ClearNodeThemes();
}


void KMapView::OnSymbologyThemeNode()
{
	ShowNodeThemes();
}


void KMapView::ShowNodeThemes()
{
	ShowMapView();

	if(NULL != m_spSymbologyThemeNodeDlg)
	{
		m_spSymbologyThemeNodeDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spSymbologyThemeNodeDlg = KSymbologyThemeNodeDlgPtr(new KSymbologyThemeNodeDlg(this));
		m_spSymbologyThemeNodeDlg->SetTarget(m_pTarget);
		m_spSymbologyThemeNodeDlg->SetMapView(this);
		m_spSymbologyThemeNodeDlg->Create(KSymbologyThemeNodeDlg::IDD, this);
		m_spSymbologyThemeNodeDlg->ShowWindow(SW_SHOW);
	}
}

void KMapView::OnClearThemeLink()
{
	ClearLinkThemes();
}


void KMapView::OnSymbologyThemeLink()
{
	ShowLinkThemes();
}

void KMapView::ShowLinkThemes()
{
	ShowMapView();

	if(NULL != m_spSymbologyThemeLinkDlg)
	{
		m_spSymbologyThemeLinkDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spSymbologyThemeLinkDlg = KSymbologyThemeLinkDlgPtr(new KSymbologyThemeLinkDlg(this));
		m_spSymbologyThemeLinkDlg->SetTarget(m_pTarget);
		m_spSymbologyThemeLinkDlg->SetMapView(this);
		m_spSymbologyThemeLinkDlg->Create(KSymbologyThemeLinkDlg::IDD, this);
		m_spSymbologyThemeLinkDlg->ShowWindow(SW_SHOW);
	}

    m_spSymbologyThemeLinkDlg->ModelessInitialize();
}


void KMapView::OnClearThemeZone()
{
	ClearZoneThemes();
}


void KMapView::OnSymbologyThemeZone()
{
	ShowZoneThemes();
}


void KMapView::ShowZoneThemes()
{
	ShowMapView();

	if(NULL != m_spSymbologyThemeZoneDlg)
	{
		m_spSymbologyThemeZoneDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spSymbologyThemeZoneDlg = KSymbologyThemeZoneDlgPtr(new KSymbologyThemeZoneDlg(this));
		m_spSymbologyThemeZoneDlg->SetTarget(m_pTarget);
		m_spSymbologyThemeZoneDlg->SetMapView(this);
		m_spSymbologyThemeZoneDlg->Create(KSymbologyThemeZoneDlg::IDD, this);
		m_spSymbologyThemeZoneDlg->ShowWindow(SW_SHOW);
	}
}

void KMapView::OnDesireLineZone()
{
	ShowMapView();
	if(nullptr != m_spDesireLineDlg)
	{
		// Obser 작업을 등록하지 않았기 때문에 모달처럼 띄움...
		m_spDesireLineDlg.reset();
		m_spDesireLineDlg = KDesireLineDlgPtr(new KDesireLineDlg(this));
		m_spDesireLineDlg->Create(KDesireLineDlg::IDD, this);

		m_spDesireLineDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spDesireLineDlg = KDesireLineDlgPtr(new KDesireLineDlg(this));
		m_spDesireLineDlg->Create(KDesireLineDlg::IDD, this);
		m_spDesireLineDlg->ShowWindow(SW_SHOW);
	}
}

void KMapView::ShowDesireThemes(std::vector<TDesireMapData> &a_vecDesireMapData, bool a_bShowLabel)
{
	IThemeLayerPtr   spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawDesireMap);    
	if (spScreenDraw)
	{
		KScreenDrawDesireMap* pScreenDraw = (KScreenDrawDesireMap*)spScreenDraw.get();
		pScreenDraw->Draw(a_vecDesireMapData, a_bShowLabel);
	}

	MapRefresh();
}

void KMapView::ShowDesireLabel(bool a_bShowLabel)
{
	IThemeLayerPtr   spScreenDraw    = m_spThemeControl->GetThemeLayer(ThemeLayerDrawDesireMap);    
	if (spScreenDraw)
	{
		KScreenDrawDesireMap* pScreenDraw = (KScreenDrawDesireMap*)spScreenDraw.get();
		pScreenDraw->ShowLabel(a_bShowLabel);
	}
	MapRefresh();
}

void KMapView::ClearDesireThemes()
{
	if (m_spThemeControl)
		m_spThemeControl->Remove(ThemeLayerDrawDesireMap);
	
    MapRefresh();
}

void KMapView::OnDesireLineClearZone()
{
	ClearDesireThemes();
}