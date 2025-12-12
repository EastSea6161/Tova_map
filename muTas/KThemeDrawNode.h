#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawNodeInfo.h"

class KThemeDrawNode : public IThemeLayer
{
public:
    KThemeDrawNode(IMapView* pMapView, int nLayerID = KLayerID::KThemeDrawNode());
    ~KThemeDrawNode(void);

public:
    virtual KEMThemeLayer GetType();
private:
    int m_nLayerID;
public:
    virtual void Clear();
public:
    void Draw(std::vector<TDrawNodeInfo>& vecDrawNodeInfo, bool bShowLabel = false, double dDrawScale=999999999.0);
private:
    void DrawUserLayer(std::vector<TDrawNodeInfo>& vecDrawNodeInfo, bool bShowLabel = false, double dDrawScale=999999999.0);
    void DrawLayer(std::vector<TDrawNodeInfo>& vecDrawNodeInfo, bool bShowLabel = false, double dDrawScale=999999999.0);
public:
    void ShowLabel(bool bShowLabel=true);
private:
    void Initialize(); 
private:
    std::map<__int64, ITxGeometryPtr> m_mapNodeGeo; 
};

typedef std::shared_ptr<KThemeDrawNode> KThemeDrawNodePtr;