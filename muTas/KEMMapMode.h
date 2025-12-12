#pragma once

/* map mode */
enum KEMMapMode 
{
    // 확대, 축소, 이동
	KEMMapModeNone              = 0,
    KEMMapModeZoomIn            = 1,
    KEMMapModeZoomOut           = 2,
    KEMMapModePan               = 3,    
    KEMMapModeIdentify          = 4,  // 정보 확인
    KEMMapModeNetworkEdit       = 11, //★ 편집 모드     
    KEMMapModeZoneDivide        = 12,
    KEMMapModeRouteCreate       = 21,
    KEMMapModeRouteEdit         = 22,
    KEMMapModeSelectTransitByLink = 31,
    KEMMapModeSelectRouteByZone = 32,
    KEMMapModeSelectInfluence   = 41,
    KEMMapModeScreenLine        = 51,
    KEMMapModeCordonLine        = 52, 
    KEMMapModeSubNetwork        = 61,
    KEMMapModeSelectNode        = 70,
    KEMMapModeZoning            = 71,
    KEMMapModeScreenDrawEdit    = 90, //★ 
	KEMMapModeSelectLink        = 91,
    KEMMapModeMapSelectRect,
    KEMMapModeMapSelectPoly
};