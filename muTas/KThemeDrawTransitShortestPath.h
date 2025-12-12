#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "ShortestPathInfo.h"

class KThemeDrawTransitShortestPath : public IThemeLayer
{
public:
    KThemeDrawTransitShortestPath(IMapView* a_mapRequest);
    virtual ~KThemeDrawTransitShortestPath(void);

public:
    virtual KEMThemeLayer GetType();

public:
    void Draw(std::vector<TTransitRouteInfo>& vecRouteInfo);

private:
    std::vector<TTransitRouteInfo> m_vecRouteInfo;
private:
    ITxFeatureUserLayerPtr m_spLayer;

private:
    __int64 TxID;

private:
    TxSymbolPointImagePtr m_spStartMarkerSymbol;
    TxSymbolPointImagePtr m_spEndMarkerSymbol;
    TxSymbolPointImagePtr m_spWalkMarkerSymbol;
    TxSymbolPointImagePtr m_spBusMarkerSymbol;
    TxSymbolPointImagePtr m_spRailMarkerSymbol;
    TxSymbolPointImagePtr m_spAutoMarkerSymbol;
    TxSymbolPointImagePtr m_spTaxiMarkerSymbol;

private:
    void Initialize();
private:
    struct TMarker  {
        int            TType;         // 0 : 시점,  1 : 도보,  3 : 버스,  5 : 택시,  7 : 기차, 9 : 종점
        ITxGeometryPtr TGeometry;
        Integer        TnxID;
    };
};

