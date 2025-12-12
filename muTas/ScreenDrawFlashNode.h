#pragma once

#include "IMapView.h"
#include "TDrawNodeInfo.h"

class ImChampMapSelectionReceiver;

class KScreenDrawFlashNode
{
private:
    bool m_bMinDistSingleSelect;
private:
	IMapView* m_pIMapView;
private:
	ImChampMapSelectionReceiver* m_pSelectionReceiver;

public:
	KScreenDrawFlashNode(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_selectionReceiver, bool a_bMinDistSingleSelect=false);
	~KScreenDrawFlashNode(void);

public:
    void Draw();
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
    std::set<__int64> m_setSelectable;
    std::vector<TxFeatureDispPtr> m_vecFeatDisp;
public:
    void SetSelectable(std::set<Integer>&    a_setSelectable);
    void SetSelectable(std::vector<Integer>& a_vecSelectable );
};

typedef std::shared_ptr<KScreenDrawFlashNode> KScreenDrawFlashNodePtr;