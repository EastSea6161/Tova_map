#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "TTransitPath.h"
#include "BulkDBaseLink.h"

class TTransitInfo;

class KEditForeDrawTransitPath : public IThemeLayer
{
public:
    KEditForeDrawTransitPath(IMapView* pIMapView);
    ~KEditForeDrawTransitPath(void);
private:
public:
    KEMThemeLayer GetType() {
        return ThemeLayerDrawTransitEdit;
    }
public:
    void SetTransitPath(bool bAppendFlag, std::vector<TTransitPath>& vecTransitPath, bool bShowLabel=true, COLORREF rgbColor=RGB(255, 0, 255));
    void RemoveTransitPath();

private:
    __int64 TxID;
private:
    TxFeatureUserLayerPtr m_spUserLayer;
private:
    bool m_bShowLabel;
public:
    void ForegroundDraw();
};

typedef std::shared_ptr<KEditForeDrawTransitPath> KForeDrawSingleTransitPathPtr;