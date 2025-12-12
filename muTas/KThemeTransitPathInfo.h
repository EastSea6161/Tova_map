#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "TTransitPath.h"
#include "BulkDBaseLink.h"

class TTransitInfo;

class KThemeTransitPathInfo : public IThemeLayer
{
public:
    KThemeTransitPathInfo(IMapView* pIMapView, bool bForegroundDraw = false);
    ~KThemeTransitPathInfo(void);

public:
    KEMThemeLayer GetType() {
        return ThemeTransitPathInfo;
    }
public:
    void SetTransitPath(Integer nxTransitiD, std::vector<TTransitPath>& vecTransitPath, bool bShowLabel=true, COLORREF rgbColor=RGB(255, 0, 255));
    void ShowLabel(bool bShowLabel);
    void RemoveTransitPath();
private:
    __int64 TxID;
};

typedef std::shared_ptr<KThemeTransitPathInfo> KThemeTransitPathInfoPtr;