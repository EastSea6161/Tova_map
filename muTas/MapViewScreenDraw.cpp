#include "stdafx.h"
#include "MapView.h"

#include "IncludeMap.h"
#include "IncludeScreenDraw.h"
#include "Target.h"

#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"

void KMapView::ScreenDrawZoneNodeAccent()
{ 
}

void KMapView::ClearZoneNodeAccent()
{
}

void KMapView::DrawNodeThemes(std::vector<TDrawNodeInfo>& a_vecNodeInfo, bool a_bLabel, bool a_bRefresh)
{
	IThemeLayerPtr spLayer = m_spThemeControl->GetThemeLayer(ThemeLayerDrawNode);    
    if (spLayer == nullptr)
        return;

    KThemeDrawNodePtr spThemeLayer = std::dynamic_pointer_cast<KThemeDrawNode>(spLayer);
    if (spThemeLayer == nullptr)
        return;

    spThemeLayer->Draw(a_vecNodeInfo, a_bLabel);
	
	if (a_bRefresh) {
        MapRefresh();
	}
}

void KMapView::SetNodeThemesLabel(bool a_bLabel, bool a_bRefresh)
{
    bool bDraw = m_spThemeControl->IsDrawing(ThemeLayerDrawNode);
    if (bDraw == false)
        return;

    IThemeLayerPtr spLayer = m_spThemeControl->GetThemeLayer(ThemeLayerDrawNode);    
    if (spLayer == nullptr)
        return;

    KThemeDrawNodePtr spThemeLayer = std::dynamic_pointer_cast<KThemeDrawNode>(spLayer);
    if (spThemeLayer == nullptr)
        return;

    spThemeLayer->ShowLabel(a_bLabel);

    if (a_bRefresh) {
        MapRefresh();
    }	
}

void KMapView::ClearNodeThemes(bool bRefresh)
{
    bool bDraw = m_spThemeControl->IsDrawing(ThemeLayerDrawNode);
    if (bDraw == false)
        return;

	if (m_spThemeControl)
		m_spThemeControl->Remove(ThemeLayerDrawNode);
	
    if (bRefresh)
        MapRefresh();

    //ReloadRender(bRefresh, 1);
}

void KMapView::DrawLinkThemes(std::vector<TDrawLinkInfo>& a_vecLinkInfo, bool a_bLabel, bool a_bRefresh)
{
    IThemeLayerPtr    spLayer      = m_spThemeControl->GetThemeLayer(ThemeLayerDrawLink);    
    KThemeDrawLinkPtr spThemeLayer = std::dynamic_pointer_cast<KThemeDrawLink>(spLayer);

    spThemeLayer->Draw(a_vecLinkInfo, a_bLabel);
    if (a_bRefresh)
        MapRefresh();
}

void KMapView::SetLinkThemesLabel(bool a_bLabel, bool a_bRefresh)
{
    bool bDraw = m_spThemeControl->IsDrawing(ThemeLayerDrawLink);
    if (bDraw == false)
        return;

    IThemeLayerPtr    spLayer      = m_spThemeControl->GetThemeLayer(ThemeLayerDrawLink);    
    KThemeDrawLinkPtr spThemeLayer = std::dynamic_pointer_cast<KThemeDrawLink>(spLayer);
    spThemeLayer->ShowLabel(a_bLabel);
    if (a_bRefresh) {
        MapRefresh();
    }
}

void KMapView::ClearLinkThemes(bool a_bRefresh)
{
    bool bDraw = m_spThemeControl->IsDrawing(ThemeLayerDrawLink);
    if (bDraw == false)
        return;

    if (m_spThemeControl)
        m_spThemeControl->Remove(ThemeLayerDrawLink);

    if (a_bRefresh)
        MapRefresh();
    //ReloadRender(a_bRefresh, 2);
}


void KMapView::DrawZoneThemes(std::vector<TDrawZoneInfo>& a_vecZoneInfo, bool a_bLabel, bool a_bRefresh)
{
    IThemeLayerPtr    spLayer      = m_spThemeControl->GetThemeLayer(ThemeLayerDrawZone);    
    KThemeDrawZonePtr spThemeLayer = std::dynamic_pointer_cast<KThemeDrawZone>(spLayer);

    spThemeLayer->Draw(a_vecZoneInfo, a_bLabel);
    if (a_bRefresh)
        MapRefresh();
}


void KMapView::SetZoneThemesLabel(bool a_bLabel, bool a_bRefresh)
{
    bool bDraw = m_spThemeControl->IsDrawing(ThemeLayerDrawZone);
    if (bDraw == false)
        return;

    IThemeLayerPtr    spLayer      = m_spThemeControl->GetThemeLayer(ThemeLayerDrawZone);    
    KThemeDrawZonePtr spThemeLayer = std::dynamic_pointer_cast<KThemeDrawZone>(spLayer);
    spThemeLayer->ShowLabel(a_bLabel);
    if (a_bRefresh) {
        MapRefresh();
    }
}

void KMapView::ClearZoneThemes()
{
    bool bDraw = m_spThemeControl->IsDrawing(ThemeLayerDrawZone);
    if (bDraw == false)
        return;

    if (m_spThemeControl)
        m_spThemeControl->Remove(ThemeLayerDrawZone);

    MapRefresh();
}


void KMapView::DrawTransit()
{
    IThemeLayerPtr       spScreenDraw        = m_spThemeControl->GetThemeLayer(ThemeLayerDrawTransit);   
    KThemeDrawTransit*  pScreenDrawTransit  = (KThemeDrawTransit*)spScreenDraw.get();
    pScreenDrawTransit->Create();
}

void KMapView::AddTransitDraw(TTransitInfo a_TransitInfo)
{
    IThemeLayerPtr       spScreenDraw        = m_spThemeControl->GetThemeLayer(ThemeLayerDrawTransit);    
    KThemeDrawTransit*  pScreenDrawTransit  = (KThemeDrawTransit*)spScreenDraw.get();

    pScreenDrawTransit->AddTransitDraw(m_pTarget, a_TransitInfo);
}

void KMapView::UpdateTransitDraw(TTransitInfo a_TransitInfo)
{
    IThemeLayerPtr       spScreenDraw        = m_spThemeControl->GetThemeLayer(ThemeLayerDrawTransit);    
    KThemeDrawTransit*  pScreenDrawTransit  = (KThemeDrawTransit*)spScreenDraw.get();

    pScreenDrawTransit->UpdateTransitDraw(m_pTarget, a_TransitInfo);
}

void KMapView::RemoveTransitDraw(Integer a_nxTransitID)
{
    IThemeLayerPtr       spScreenDraw        = m_spThemeControl->GetThemeLayer(ThemeLayerDrawTransit);    
    KThemeDrawTransit*  pScreenDrawTransit  = (KThemeDrawTransit*)spScreenDraw.get();

    pScreenDrawTransit->RemoveTransitDraw(a_nxTransitID);
}


void KMapView::ClearTransit(bool a_bRefresh)
{
	if (m_spThemeControl)
		m_spThemeControl->Remove(ThemeLayerDrawTransit);
    if (a_bRefresh) {
		MapRefresh();
	}
};


void KMapView::AddSelectionNode( std::vector<Integer> ar_vecNode, bool a_bDrawLabel )
{
    try
    {
        m_vecSelectionNode.clear();

        std::vector<TDrawNodeInfo> vecDrawNodeInfo;

        size_t nxCount = ar_vecNode.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TDrawNodeInfo oDrawNodeInfo;
            oDrawNodeInfo.TNodeID = ar_vecNode[i];
            oDrawNodeInfo.TSymbol = 48;
            oDrawNodeInfo.TColorRGB = RGB(79, 129,189);
            oDrawNodeInfo.TSize   = 15;

            CString strLabel;
            strLabel.Format(_T("%I64d"), ar_vecNode[i]);
            oDrawNodeInfo.TLabel = strLabel;

            vecDrawNodeInfo.push_back(oDrawNodeInfo);
        }

        bool bUseCache(true);

        IThemeLayerPtr            spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawNodeSelection);    
        KThemeDrawNodeSelection* pScreenDraw  = (KThemeDrawNodeSelection*)spScreenDraw.get();
        if (pScreenDraw)
        {
            pScreenDraw->Draw(vecDrawNodeInfo, a_bDrawLabel);
            m_vecSelectionNode = ar_vecNode;

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

void KMapView::AddSelectionLink( std::vector<Integer> ar_vecLink, bool a_bDrawLabel )
{
    try
    {
        m_vecSelectionLink.clear();

        std::vector<TDrawLinkInfo> vecDrawLinkInfo;

        size_t nxCount = ar_vecLink.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TDrawLinkInfo oDrawLinkInfo;
            oDrawLinkInfo.TLinkID   = ar_vecLink[i];
            oDrawLinkInfo.TColorRGB = RGB(79, 129,189);
            oDrawLinkInfo.TWidth    = 2.0;

            CString strLabel;
            strLabel.Format(_T("%I64d"), ar_vecLink[i]);
            oDrawLinkInfo.TLabel = strLabel;

            vecDrawLinkInfo.push_back(oDrawLinkInfo);
        }

        bool bUseCache(true);

        IThemeLayerPtr            spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawLinkSelection);    
        KThemeDrawLinkSelection* pScreenDraw  = (KThemeDrawLinkSelection*)spScreenDraw.get();
        if (pScreenDraw)
        {
            pScreenDraw->Draw(vecDrawLinkInfo, a_bDrawLabel);
            m_vecSelectionLink = ar_vecLink;

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

void KMapView::CacheClearCompositeThemes(bool a_bRefresh)
{
    if (m_spThemeControl)
        m_spThemeControl->Remove(ThemeLayerDrawComposite);
    if (a_bRefresh)
        TasESRICacheRefresh();
}

void KMapView::CacheDrawCompositeLink(std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo, bool a_bDrawLabel, bool a_bRefresh)
{
	IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
	KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
	if (pScreenDraw)
	{
		pScreenDraw->SetLinkDraw(a_vecDrawLinkInfo, a_bDrawLabel);
	}

	if (a_bRefresh)
		TasESRICacheRefresh();
}


void KMapView::CacheShowCompositeLinkLabel(bool a_bDrawLabel/* =true */, bool a_bRefresh)
{
	IThemeLayerPtr         spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
	KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
	if (pScreenDraw)
	{
		pScreenDraw->SetShowLinkLabel(a_bDrawLabel);
		if (a_bRefresh)
			TasESRICacheRefresh();
	}
}


void KMapView::CacheDrawCompositeUserLine(std::vector<TUserLineInfo>& ar_vecUserLine, bool a_bDrawLabel, bool a_bRefresh)
{
	IThemeLayerPtr         spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
	KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
	if (pScreenDraw)
	{
		pScreenDraw->SetUserLineDraw(ar_vecUserLine, a_bDrawLabel);
	}

	if (a_bRefresh)
		TasESRICacheRefresh();
}


void KMapView::CacheShowCompositeUserLineLabel(bool a_bDrawLabel/* =true */, bool a_bRefresh)
{
	IThemeLayerPtr         spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
	KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
	if (pScreenDraw)
	{
		pScreenDraw->SetShowUserLineLabel(a_bDrawLabel);
		if (a_bRefresh)
			TasESRICacheRefresh();
	}
}


void KMapView::CacheDrawCompositeZoneAccess(std::vector<TDrawZoneAccess>& ar_vecDrawZoneAccess, bool a_bRefresh/* =false */)
{
	IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
	KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
	if (pScreenDraw)
	{
		pScreenDraw->SetZoneAccessDraw(ar_vecDrawZoneAccess, false);
	}

	if (a_bRefresh)
		TasESRICacheRefresh();	
}


void KMapView::CacheDrawInterRegionalTransit(std::vector<TUserLineInfo>& ar_vecUserLine, std::vector<TUserPointInfo>& ar_vecUserPoint, bool a_bShowNodeLabel, bool a_bRefresh)
{
    IThemeLayerPtr             spScreenDraw              = m_spThemeControl->GetThemeLayer(ThemeLayerDrawRegionTerminal);    
    KScreenDrawRegionTerminal* pScreenDrawRegionTerminal = (KScreenDrawRegionTerminal*)spScreenDraw.get();
    // 여기에 추가 작업을 하심 
    
	if (pScreenDrawRegionTerminal)
	{
		pScreenDrawRegionTerminal->SetUserLineDraw(ar_vecUserLine, false);
		pScreenDrawRegionTerminal->SetUserPointDraw(ar_vecUserPoint, a_bShowNodeLabel);
	}

    if (a_bRefresh)
        TasESRICacheRefresh();	
}

void KMapView::CacheClearInterRegionalTransit(bool a_bRefresh)
{
	if (m_spThemeControl)
		m_spThemeControl->Remove(ThemeLayerDrawRegionTerminal);
    if (a_bRefresh)
        TasESRICacheRefresh();	
}


void KMapView::CacheDrawCompositeUserPoint(std::vector<TUserPointInfo>& ar_vecUserPoint, bool a_bDrawLabel, bool a_bRefresh)
{
	IThemeLayerPtr     spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
	KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();

	if (pScreenDraw)
	{
		pScreenDraw->SetUserPointDraw(ar_vecUserPoint, a_bDrawLabel);
	}

	if (a_bRefresh)
		TasESRICacheRefresh();
}


void KMapView::CacheDrawCompositeUserPointLabel(bool a_bDrawLabel/* =true */, bool a_bRefresh)
{
	IThemeLayerPtr         spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawComposite);    
	KThemeLayerUnion*  pScreenDraw  = (KThemeLayerUnion*)spScreenDraw.get();
	if (pScreenDraw)
	{
		pScreenDraw->SetShowUserPointLabel(a_bDrawLabel);
		if (a_bRefresh)
			TasESRICacheRefresh();
	}
}