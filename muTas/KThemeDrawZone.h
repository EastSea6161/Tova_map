#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawZoneInfo.h"

class KThemeDrawZone : public IThemeLayer
{
public:
	KThemeDrawZone(IMapView* pMapView);
	~KThemeDrawZone(void);

public:
	virtual KEMThemeLayer GetType();

public:
	void Draw(std::vector<TDrawZoneInfo>& vecDrawZoneInfo, bool bShowLabel);
	void ShowLabel(bool bShowLabel=true);
};

typedef std::shared_ptr<KThemeDrawZone> KThemeDrawZonePtr;