#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "TCoordinate.h"

class KThemDrawIntersection : public IThemeLayer
{
public:
	KThemDrawIntersection(IMapView* a_mapRequest);
	~KThemDrawIntersection(void);

public:
	virtual KEMThemeLayer GetType();

public:
	void AddDraw    (Integer a_nxTurnID);
	void RemoveDraw (Integer a_nxTurnID);
};

