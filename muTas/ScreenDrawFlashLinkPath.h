#pragma once

#include "IMapView.h"
#include "TDrawLinkInfo.h"

#include "BulkDBaseLink.h"

class ImChampMapSelectionReceiver;

class TLinkOffsetDist {
public:
    TLinkOffsetDist() {}
    ~TLinkOffsetDist() {}
public:
    TxFeaturePtr TFeature;
    double  TDist;
    __int64 TLinkID;
};

class KScreenDrawFlashLinkPath 
{    
private:
	IMapView* m_pIMapView;
private:
	ImChampMapSelectionReceiver* m_pSelectionReceiver;

public:
	KScreenDrawFlashLinkPath(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_selectionReceiver, Integer a_nxStartLinkID);
	~KScreenDrawFlashLinkPath(void);

public:
private:
	void MouseMovePaintDraw();

private:
	std::vector<TLinkOffsetDist> FindLinks(double mapX, double mapY);

private:
	double  m_dMouseMoveX, m_dMouseMoveY;

public:
    void Draw();
public:
    void MouseDown (UINT nFlags, long X, long Y, double mapX, double mapY);
    void MouseMove (UINT nFlags, long X, long Y, double mapX, double mapY);
private:
	std::vector<Integer> m_vecCurMouseMoveSelect;
	std::vector<Integer> m_vecShortestLinkPath;
    std::vector<Integer> m_vecUserSelectedLink;
private:
    void ExtractPath(double mapX, double mapY);

private:
	void Initialize();
private:
	Integer                    m_nxStartLinkID;
    Integer                    m_nxStartTNodeID;
	KLinkCollection            m_oLinkCollection;
	KCostLinkCollection        m_oCostLinkCollection;
	KNodeArrivedMinLink        m_oNodeArrivedMinLink;
    bool m_bUserSelectLink;


private:
    std::map<Integer, TLinkFTNodeID> m_mapLinkFTNode;
    std::map<KODKey,  Integer>       m_mapFTLink;

private:
    std::vector<TLinkOffsetDist> SearchOffsetLinkOrderByDist( ITxFeatureLayerPtr spFeatureLayer, double dx, double dy, double dTol, double a_dFilterDist=0.5);

private:
    static bool Ascennding( TLinkOffsetDist left, TLinkOffsetDist right )
    {
        return left.TDist < right.TDist;
    }
};

typedef std::shared_ptr<KScreenDrawFlashLinkPath> KScreenDrawFlashLinkPathPtr;
