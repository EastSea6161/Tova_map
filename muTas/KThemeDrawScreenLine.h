#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawScreenCordonInfo.h"

class KTarget;

class KThemeDrawScreenLine : public IThemeLayer
{
public:
    KThemeDrawScreenLine (IMapView* a_mapRequest);
    ~KThemeDrawScreenLine(void);

public:
    virtual KEMThemeLayer GetType();

public:
    void AddDraw       (KTarget* a_pTarget, Integer a_nxScreenID, COLORREF a_rgbColor = RGB(255, 0, 0));
    void RemoveDraw    (Integer a_nxScreenID);
    void RemoveAllDraw ();

private:
    std::set<__int64> m_setKey;
private:
    ITxFeatureUserLayerPtr m_spLayer;
private:
    void Initialize();
};
