#pragma once

#include "IMapView.h"
#include "IThemeLayer.h"
#include "KThemeDrawZone.h"
#include "KThemeDrawNode.h"
#include "KThemeDrawLink.h"

#include "KThemeDrawTransit.h"
#include "KEditForeDrawTransitPath.h"
#include "KThemeDrawScreenLine.h"
#include "KThemeDrawCordonLine.h"
#include "KThemeDrawTurn.h"
#include "KThemDrawIntersection.h"

#include "KThemDrawZoneToZonePathVolume.h"
#include "KThemDrawLinkViaPathVolume.h"
#include "ScreenDrawDesireMap.h"
#include "KThemeLayerUnion.h"
#include "KThemeDrawLinkSelection.h"
#include "KThemeDrawNodeSelection.h"
#include "KThemeDrawUserPoint.h"
#include "KThemeDrawHighwayShortestPath.h"
#include "KThemeDrawTransitShortestPath.h"

typedef KThemeDrawLinkSelection KScreenDrawLinkAccent;
typedef std::shared_ptr<KScreenDrawLinkAccent> KScreenDrawLinkAccentPtr;

typedef KThemeDrawNodeSelection KScreenDrawNodeAccent;
typedef std::shared_ptr<KScreenDrawNodeAccent> KScreenDrawNodeAccentPtr;

typedef KThemeDrawLinkSelection KScreenDrawRulerLinkSelection;
typedef std::shared_ptr<KScreenDrawRulerLinkSelection> KScreenDrawRulerLinkSelectionPtr;

typedef KThemeLayerUnion KScreenDrawRegionTerminal;
typedef std::shared_ptr<KScreenDrawRegionTerminal> KScreenDrawRegionTerminalPtr;

class KThemeLayerDrawFactory
{
public:
    static IThemeLayer* Create(IMapView* pMapView, KEMThemeLayer a_type)
    {        
        switch (a_type)
        {
		case ThemeLayerDrawZone:
			return new KThemeDrawZone               (pMapView);
			break;
        case ThemeLayerDrawNode:
            return new KThemeDrawNode           (pMapView);
            break;
        case ThemeLayerDrawLink:
            return new KThemeDrawLink               (pMapView);
            break;
        case ThemeLayerDrawHighwayShortestPath:
            return new KThemeDrawHighwayShortestPath(pMapView);
            break;
        case ThemeLayerDrawTransitShortestPath:
            return new KThemeDrawTransitShortestPath(pMapView);
            break;
        case ThemeLayerDrawTransit:
            return new KThemeDrawTransit            (pMapView);
            break;
		case ThemeLayerDrawZoneToZonePathVolume:
			return new KThemDrawZoneToZonePathVolume(pMapView);
			break;
		case ThemeLayerDrawLinkViaPathVolume:
			return new KThemDrawLinkViaPathVolume  (pMapView);
			break;
        case ThemeLayerDrawCordonLine:
            return new KThemeDrawCordonLine         (pMapView);
            break;
        case ThemeLayerDrawScreenLine:
            return new KThemeDrawScreenLine         (pMapView);
            break;
        case ThemeLayerDrawTurn:
            return new KThemeDrawTurn               (pMapView);
            break;
		case ThemeLayerDrawDesireMap:
			return new KScreenDrawDesireMap          (pMapView);
			break;
        case ThemeLayerDrawComposite:
            return new KThemeLayerUnion          (pMapView);
            break;
        case ThemeLayerDrawLinkSelection:
            return new KThemeDrawLinkSelection      (pMapView);
            break;
        case ThemeLayerDrawNodeSelection:
            return new KThemeDrawNodeSelection      (pMapView);
            break;
		case ThemeLayerDrawIntersection:
			return new KThemDrawIntersection       (pMapView);
			break;
        case ThemeLayerDrawLinkAccent:
            return new KScreenDrawLinkAccent         (pMapView);
            break;
        case ThemeLayerDrawNodeAccent:
            return new KScreenDrawNodeAccent        (pMapView);
            break;
        case ThemeLayerDrawRulerLinkSelect:
            return new KScreenDrawRulerLinkSelection(pMapView);
            break;
        case ThemeLayerDrawRegionTerminal:
            return new KThemeLayerUnion(pMapView);
        default:
            return NULL;
            break;
        }

        return NULL;
    }
};