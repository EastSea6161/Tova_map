#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawNodeInfo.h"

class KThemeDrawZoneAccessBoundary : public IThemeLayer
{
public:
    KThemeDrawZoneAccessBoundary (IMapView* a_mapRequest);
    ~KThemeDrawZoneAccessBoundary();
public:
    KEMThemeLayer GetType() {
        return ThemeLayerDrawTransitEdit;
    }
private:
    void Initialize();
private:
    ITxFeatureUserLayerPtr m_spLayer;
private:
    __int64 TxID;

public:
    void Draw(std::vector<TDrawZoneAccess>& ar_vecZoneAccess);
};

typedef std::shared_ptr<KThemeDrawZoneAccessBoundary> KScreenDrawZoneAccessBoundaryPtr;