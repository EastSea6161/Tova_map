#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TUserGeometryInfo.h"

class KThemeDrawUserLine : public IThemeLayer
{
public:
    KThemeDrawUserLine(IMapView* pMapView, int nLayerID = KLayerID::KThemeDrawUserLine_Union());
    ~KThemeDrawUserLine(void);
private:
    int m_nLayerID;
public:
    virtual KEMThemeLayer GetType();
public:
    void Draw(std::vector<TUserLineInfo>& vecUserLine, bool bShowLabel = false);
    void ShowLabel(bool bShowLabel=true);
};

typedef std::shared_ptr<KThemeDrawUserLine> KThemeDrawUserLinePtr;