#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawNodeInfo.h"

class KThemeDrawNodeSelection : public IThemeLayer
{
public:
    KThemeDrawNodeSelection(IMapView* a_mapRequest);
    ~KThemeDrawNodeSelection(void);

public:
    virtual KEMThemeLayer GetType();
private:
    std::vector<TDrawNodeInfo> m_vecDrawNodeInfo;
public:
    void Draw(std::vector<TDrawNodeInfo>& a_vecDrawNodeInfo, bool a_bDrawLabel = false, double a_dDrawScale=999999999.0);
    void ShowLabel(bool a_bShowLabel=true);

private:
    int m_nLayerID;
};

typedef std::shared_ptr<KThemeDrawNodeSelection> KScreenDrawNodeSelectionPtr;