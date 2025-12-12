#include "stdafx.h"
#include "KThemeLayerUnion.h"

KThemeLayerUnion::KThemeLayerUnion( IMapView* a_mapRequest ):IThemeLayer(a_mapRequest)
{
    m_bDrawing = true;
}

KThemeLayerUnion::~KThemeLayerUnion( void )
{
}

KEMThemeLayer KThemeLayerUnion::GetType()
{
    return ThemeLayerDrawComposite;
}

void KThemeLayerUnion::SetNodeDraw(std::vector<TDrawNodeInfo>& a_vecDrawNodeInfo, bool a_bDrawLabel, double a_dDrawScale )
{
    m_spScreenDrawNode = KThemeDrawNodePtr(new KThemeDrawNode(m_pIMapView, KLayerID::KThemeLayerDrawNode_Union()));
    m_spScreenDrawNode->Draw(a_vecDrawNodeInfo, a_bDrawLabel, a_dDrawScale);
}

void KThemeLayerUnion::SetShowNodeLabel( bool a_bDrawLabel /*= true*/ )
{
    if (m_spScreenDrawNode != nullptr) {
        m_spScreenDrawNode->ShowLabel(a_bDrawLabel);
    }
}

void KThemeLayerUnion::SetLinkDraw( std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo, bool a_bDrawLabel, bool bSort, bool bLabelFill )
{
    m_spScreenDrawLink = KThemeDrawLinkPtr(new KThemeDrawLink(m_pIMapView, KLayerID::KThemeDrawLink_Union()));
    m_spScreenDrawLink->Draw(a_vecDrawLinkInfo, a_bDrawLabel, bSort, bLabelFill);
}

void KThemeLayerUnion::SetShowLinkLabel( bool a_bDrawLabel /*= true*/ )
{
    if (m_spScreenDrawLink != nullptr)
    {
        m_spScreenDrawLink->ShowLabel(a_bDrawLabel);
    }
}

void KThemeLayerUnion::SetUserPointDraw( std::vector<TUserPointInfo>& ar_vecUserPoint, bool a_bDrawLabel /*= false*/ )
{
    m_spScreenDrawUserPoint = KThemeDrawUserPointPtr(new KThemeDrawUserPoint(m_pIMapView));
    m_spScreenDrawUserPoint->Draw(ar_vecUserPoint, a_bDrawLabel);
}

void KThemeLayerUnion::SetShowUserPointLabel( bool a_bDrawLabel /*= true*/ )
{
    if (m_spScreenDrawUserPoint != nullptr)
        m_spScreenDrawUserPoint->ShowLabel(a_bDrawLabel);
}

void KThemeLayerUnion::SetUserLineDraw( std::vector<TUserLineInfo>& ar_vecUserLine, bool a_bDrawLabel /*= false*/ )
{
    m_spScreenDrawUserLine = KThemeDrawUserLinePtr(new KThemeDrawUserLine(m_pIMapView));
    m_spScreenDrawUserLine->Draw(ar_vecUserLine, a_bDrawLabel);
}

void KThemeLayerUnion::SetShowUserLineLabel( bool a_bDrawLabel /*= true*/ )
{
    if (m_spScreenDrawUserLine != nullptr)
    {
        m_spScreenDrawUserLine->ShowLabel(a_bDrawLabel);
    }
}

void KThemeLayerUnion::SetZoneAccessDraw( std::vector<TDrawZoneAccess>& a_vecDrawZoneAccessInfo, bool a_bDrawLabel /*= false*/ )
{
    m_spScreenDrawZoneAccess = KScreenDrawZoneAccessBoundaryPtr(new KThemeDrawZoneAccessBoundary(m_pIMapView));
    m_spScreenDrawZoneAccess->Draw(a_vecDrawZoneAccessInfo);
}

void KThemeLayerUnion::SetTransitPassenger( std::vector<TTransitPassengerInfo>& a_vecDrawTransitPassengerInfo, bool a_bDrawLabel /*= false*/ )
{
    m_spScreenDrawOffsetPointBuffer = KThemeDrawStationPtr(new KThemeDrawStation(m_pIMapView));
    m_spScreenDrawOffsetPointBuffer->Draw(a_vecDrawTransitPassengerInfo, a_bDrawLabel);
}

void KThemeLayerUnion::SetTransitPassengerLabel( bool a_bDrawLabel /*= true*/ )
{
    if (m_spScreenDrawOffsetPointBuffer != nullptr)
    {
        m_spScreenDrawOffsetPointBuffer->ShowLabel(a_bDrawLabel);
    }
}

