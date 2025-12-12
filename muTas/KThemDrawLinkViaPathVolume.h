#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawLinkInfo.h"

class KThemDrawLinkViaPathVolume : public IThemeLayer
{
public:
	KThemDrawLinkViaPathVolume(IMapView* a_mapRequest);
	~KThemDrawLinkViaPathVolume(void);

public:
	virtual KEMThemeLayer GetType();

public:
	void Draw(std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo, std::vector<Integer>& a_vecSelectedLink, bool a_bDrawLabel = false);
public:
	void SetDrawLabel(bool a_bDrawLabel);

private:
    ITxFeatureUserLayerPtr m_spLayer;
private:
    __int64 TxID;
};