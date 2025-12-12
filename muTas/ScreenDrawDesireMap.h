#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TDrawDesireMap.h"

class KScreenDrawDesireMap : public IThemeLayer
{
public:
	KScreenDrawDesireMap(IMapView* pMapView);
	~KScreenDrawDesireMap(void);

public:
	virtual KEMThemeLayer GetType();

public:
	void Draw(std::vector<TDesireMapData>& vecData, bool bShowLabel = true);
	void ShowLabel(bool bShowLabel=true);
};

