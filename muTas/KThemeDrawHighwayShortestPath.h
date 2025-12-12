#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "ShortestPathInfo.h"

class KThemeDrawHighwayShortestPath : public IThemeLayer
{
public:
    KThemeDrawHighwayShortestPath(IMapView* a_mapRequest);
    virtual ~KThemeDrawHighwayShortestPath(void);

public:
    virtual KEMThemeLayer GetType();

public:
    void Draw(std::vector<TPathInfo>& vecPathInfo);
private:
    void Draw(TPathInfo& oPathInfo, int nOrder);
private:
    ITxFeatureUserLayerPtr m_spLayer;

private:
    __int64 TxID;
};

