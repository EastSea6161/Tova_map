#pragma once

enum KEMThemeLayer
{
    // Display Order의 의미도 갖고 있음.   
	ThemeLayerDrawZone   = 1000       , 
    ThemeLayerDrawLink                ,
    ThemeLayerDrawNode                ,    
    ThemeLayerDrawTransit             ,  // 트랜짓 테이블에서 복수 선택 -> 노선도 보기
    ThemeLayerDrawTransitEdit         ,  // 트랜짓 편집 레이어
    ThemeTransitPathInfo              ,  // 트랜짓 상세보기
    ThemeLayerDrawComposite           ,        
    ThemeLayerDrawHighwayShortestPath ,
    ThemeLayerDrawTransitShortestPath ,
	ThemeLayerDrawZoneToZonePathVolume,
	ThemeLayerDrawLinkViaPathVolume   ,
    ThemeLayerDrawCordonLine          ,
    ThemeLayerDrawScreenLine          ,
    ThemeLayerDrawTurn                ,
	ThemeLayerDrawDesireMap           ,
	ThemeLayerDrawUserLink            ,
    ThemeLayerDrawOffsetPointBuffer   ,
	ThemeLayerDrawUserPoint           ,
    ThemeLayerDrawLinkSelection       ,
    ThemeLayerDrawNodeSelection       ,
	ThemeLayerDrawIntersection        ,    
    ThemeLayerDrawNodeAccent          ,
    ThemeLayerDrawLinkAccent          ,
    ThemeLayerDrawRulerLinkSelect /*Ruler-선택된 링크 보여 주기*/,
    ThemeLayerDrawRegionTerminal
};

class IThemeLayer
{
protected:
    IMapView* m_pIMapView;
    bool      m_bDrawing;
public:
    IThemeLayer(IMapView* pMapView) { 
        m_pIMapView = pMapView;
        m_bDrawing  = false;
    }

    virtual ~IThemeLayer(void) {
        m_pIMapView = nullptr;
    }

public:
    virtual KEMThemeLayer GetType() = 0;
    virtual bool IsDrawing() {
        return m_bDrawing;
    }

public:
    virtual void Clear() {}
};
