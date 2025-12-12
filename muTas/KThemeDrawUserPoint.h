#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"

class TUserPointInfo;

class KThemeDrawUserPoint : public IThemeLayer
{
public:
    KThemeDrawUserPoint(IMapView* a_mapRequest, int nLayerID = KLayerID::KScreenDrawUserPoint());
    ~KThemeDrawUserPoint(void);
private:
    int m_nLayerID;
public:
    virtual KEMThemeLayer GetType();

public:
    void Draw(std::vector<TUserPointInfo>& vecUserPoint, bool bShowLabel = false);
    void ShowLabel(bool bShowLabel=true);
};

typedef std::shared_ptr<KThemeDrawUserPoint> KThemeDrawUserPointPtr;