#pragma once

#include "IMapView.h"
#include "TDrawLinkInfo.h"

class ImChampMapSelectionReceiver;

class KScreenDrawFlashLink 
{
private:
	IMapView* m_pIMapView;
private:
	ImChampMapSelectionReceiver* m_pSelectionReceiver;

public:
	KScreenDrawFlashLink(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_selectionReceiver);
	~KScreenDrawFlashLink(void);

public:
    void Draw();
public:
    void MouseDown (UINT nFlags, long X, long Y, double mapX, double mapY);
    void MouseMove (UINT nFlags, long X, long Y, double mapX, double mapY);
private:
    double m_dPixelBuffer;
private:
    ITxFeatureLayerPtr m_spLayer;    
private:
    ITxSymbolPtr m_spSymbol;    
private:
    std::vector<TxFeatureDispPtr> m_vecDrawFeature;

private:
    std::vector<TxFeatureDispPtr> m_vecFeatDisp;
	std::set<__int64> m_setSelectable;
public:
	void SetSelectable(std::set<__int64>&    a_setSelectable);
	void SetSelectable(std::vector<__int64>& a_vecSelectable);
};

typedef std::shared_ptr<KScreenDrawFlashLink> KScreenDrawFlashLinkPtr;
