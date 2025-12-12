#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawLinkInfo.h"

class KThemeDrawLink : public IThemeLayer
{
public:
    KThemeDrawLink(IMapView* pMapView, int nLayerID = KLayerID::KThemeDrawLink());
    ~KThemeDrawLink(void);
public:
    virtual void Clear();
public:
    virtual KEMThemeLayer GetType();

private:
    int m_nLayerID;
public:
    void Draw(std::vector<TDrawLinkInfo> &vecDrawLinkInfo, bool bShowLabel = false, bool bSort = true, bool bLabelFill = false);
    void ShowLabel(bool bShowLabel=true);

private:
    bool m_bLoadLinkGeo;
private:
    void LoadLinkGeo();
private:
    void DrawLargeSizeUserLayer(std::vector<TDrawLinkInfo> &vecDrawLinkInfo, bool bShowLabel, bool bSort = true, bool bLabelFill = false);
    void DrawSmallSizeUserLayer(std::vector<TDrawLinkInfo> &vecDrawLinkInfo, bool bShowLabel, bool bSort = true, bool bLabelFill = false);
private:
    std::map<__int64, ITxGeometryPtr> m_mapLinkGeo;
};

typedef std::shared_ptr<KThemeDrawLink> KThemeDrawLinkPtr;