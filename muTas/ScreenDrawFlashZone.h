#pragma once

#include "IMapView.h"

class ImChampMapSelectionReceiver;

class KScreenDrawFlashZone
{
private:
    IMapView* m_pIMapView;
private:
    ImChampMapSelectionReceiver* m_pSelectionReceiver;

public:
    KScreenDrawFlashZone(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_selectionReceiver, bool a_bMinDistSingleSelect=false);
    ~KScreenDrawFlashZone(void);
public:
    void Draw() {
    }
public:
    void MouseDown (UINT nFlags, long X, long Y, double mapX, double mapY);
    void MouseMove (UINT nFlags, long X, long Y, double mapX, double mapY);
private:
private:
    ITxFeatureLayerPtr m_spLayer;    
private:
    ITxSymbolPtr m_spSymbol;  
private:
    TxFeatureDispPtr m_spFeatureDisp;
};

typedef std::shared_ptr<KScreenDrawFlashZone> KScreenDrawFlashZonePtr;