#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawLinkInfo.h"

class KThemeDrawLinkSelection : public IThemeLayer
{
public:
    KThemeDrawLinkSelection(IMapView* pMapView);
    ~KThemeDrawLinkSelection(void);

public:
    virtual KEMThemeLayer GetType();

public:
    void Draw(std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo, bool a_bDrawLabel = false);
    void ShowLabel(bool a_bShowLabel=true);

private:
    int m_nLayerID;
};

typedef std::shared_ptr<KThemeDrawLinkSelection> KScreenDrawLinkSelectionPtr;

