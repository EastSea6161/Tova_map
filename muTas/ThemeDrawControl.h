#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"

typedef std::shared_ptr<IThemeLayer> IThemeLayerPtr;

class KThemeDrawControl
{
public:
    KThemeDrawControl(IMapView* pIMapView);
    ~KThemeDrawControl(void);
public:
    IThemeLayerPtr GetThemeLayer(KEMThemeLayer enLayer);
public:
    void Remove(KEMThemeLayer enLayer);
    void RemoveAll();

private:
    IMapView* m_pIMapView;
private:
    std::map<KEMThemeLayer, IThemeLayerPtr> m_mapTheme;

public:
    bool IsDrawing(KEMThemeLayer enLayer);
};

typedef std::shared_ptr<KThemeDrawControl> KThemeDrawControlPtr;