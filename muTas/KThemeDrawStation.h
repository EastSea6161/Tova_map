#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "TTransitPassengerInfo.h"

class KThemeDrawStation : public IThemeLayer
{
public:
    KThemeDrawStation (IMapView* pMapView);
    ~KThemeDrawStation();
public:
    KEMThemeLayer GetType();
public:
    void ShowLabel(bool bShowLabel=true);

public:
    void Draw(std::vector<TTransitPassengerInfo>& vecTransitPassengerInfo, bool bShowLabel = false);
private:
    void DrawOld(std::vector<TTransitPassengerInfo>& vecTransitPassengerInfo, bool bShowLabel = false);
private:
    int m_nLayerID;
};

typedef std::shared_ptr<KThemeDrawStation> KThemeDrawStationPtr;