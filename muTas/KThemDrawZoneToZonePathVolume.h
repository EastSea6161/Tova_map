#pragma once
#include "IMapView.h"
#include "IThemeLayer.h"
#include "TPathAnalysisInfo.h"


class TVineLinkInfo
{
public:
    TVineLinkInfo() {
    }
    ~TVineLinkInfo() {}
public:
	double   TVolume;
	COLORREF TColor;
	double	 TWidth;
};


class KThemDrawZoneToZonePathVolume : public IThemeLayer
{
public:
	KThemDrawZoneToZonePathVolume(IMapView* a_mapRequest);
	virtual ~KThemDrawZoneToZonePathVolume(void);

public:
	virtual KEMThemeLayer GetType();

private:
	int m_nType;
private:
	void Initialize();
private:
    ITxFeatureUserLayerPtr m_spLayer;
private:
    __int64 TxID;

private:
    void DrawRequestEachPath(std::vector<TPathVolumeInfo>& vecTPathVolumeInfo);
    void DrawRequestVinePath(std::vector<TPathVolumeInfo>& vecTPathVolumeInfo);
public:
    void Draw(int a_nType/*0:EachPath, 1:Goguma*/, std::vector<TPathVolumeInfo>& a_rVecTPathVolumeInfo);
};