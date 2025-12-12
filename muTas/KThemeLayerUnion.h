#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawNodeInfo.h"
#include "KThemeDrawNode.h"
#include "TDrawLinkInfo.h"
#include "KThemeDrawLink.h"
#include "TUserGeometryInfo.h"
#include "KThemeDrawUserLine.h"
#include "KThemeDrawZoneAccessBoundary.h"
#include "TTransitPassengerInfo.h"
#include "KThemeDrawStation.h"
#include "KThemeDrawUserPoint.h"

//★ 개념 정의
// - KScreenDrawComposite는 레이어의 집합체의 역할을 한다.

class KThemeLayerUnion : public IThemeLayer
{
public:
    KThemeLayerUnion(IMapView* a_mapRequest);
    ~KThemeLayerUnion(void);

public:
    KEMThemeLayer GetType();

public:
    void SetNodeDraw       (std::vector<TDrawNodeInfo>& a_vecDrawNodeInfo, bool a_bDrawLabel = false,  double a_dDrawScale=999999999.0);
    void SetShowNodeLabel  (bool a_bDrawLabel = true);
    void SetLinkDraw       (std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo, bool a_bDrawLabel = false, bool bSort = true, bool bLabelFill = false);
    void SetShowLinkLabel  (bool a_bDrawLabel = true);
    void SetUserLineDraw   (std::vector<TUserLineInfo>& ar_vecUserLine,    bool a_bDrawLabel = false);
    void SetShowUserLineLabel(bool a_bDrawLabel = true);

    void SetUserPointDraw     (std::vector<TUserPointInfo>& ar_vecUserPoint, bool a_bDrawLabel = false);
    void SetShowUserPointLabel(bool a_bDrawLabel = true);
    void SetZoneAccessDraw (std::vector<TDrawZoneAccess>& a_vecDrawZoneAccessInfo, bool a_bDrawLabel = false);
    // Line Themes
    void SetTransitPassenger(std::vector<TTransitPassengerInfo>& a_vecDrawTransitPassengerInfo, bool a_bDrawLabel = false);
    void SetTransitPassengerLabel(bool a_bDrawLabel = true);
private:
    KThemeDrawNodePtr m_spScreenDrawNode;
    KThemeDrawLinkPtr m_spScreenDrawLink;

    KThemeDrawUserPointPtr  m_spScreenDrawUserPoint;
    KThemeDrawUserLinePtr   m_spScreenDrawUserLine;
    KScreenDrawZoneAccessBoundaryPtr m_spScreenDrawZoneAccess;
    KThemeDrawStationPtr  m_spScreenDrawOffsetPointBuffer;
};

typedef std::shared_ptr<KThemeLayerUnion> KScreenDrawCompositePtr;