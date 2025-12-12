#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"
#include "KEMLayer.h"

enum KEMScreenDrawEditSearchStatus
{
    KEMScreenDrawEditSearchNothing           = 0, 
    KEMScreenDrawEditSearchNameInput         = 1,
    KEMScreenDrawEditSearchNameInputed       = 2,
    KEMScreenDrawEditSearchFirstPoint        = 3,
    KEMScreenDrawEditSearchFirstPointInputed = 4, 
    KEMScreenDrawEditSearchAdd               = 6,
    KEMScreenDrawEditSearchComplete          = 9
};

class KScreenDrawEditSearchPolygon : public IScreenDrawEdit
{
public:
    KScreenDrawEditSearchPolygon(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapFeedback, KEMLayer a_emLayer, int a_nActionType = 0, bool a_bIntersectDemarcation = true) ;
    virtual ~KScreenDrawEditSearchPolygon(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );
public:
    void MapSelectionFeedback(Integer a_nxID){};
    virtual ThemeLayerDrawEdit GetType() {
        return KEMScreenDrawEditSearchPolygon;
    }

public:
public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
private:

private:
    ImChampMapSelectionReceiver* m_pMapFeedback;
    KEMLayer m_emLayer;
    int      m_nActionType;

private:
    ITxSymbolPtr   m_spSymbolLine;
    ITxSymbolPtr   m_spSymbolPoly;
private:
    ITxGeometryPtr m_spGeoDraw;    
private:
    std::vector<TxPoint> m_vecPt;
public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);

private:
    bool m_bIntersectDemarcation;
private:
    void SaveEditingIntersectLink(TxPolygonPtr spPoly);
    std::set<__int64> UnionInNode(TxPolygonPtr spPoly);
};