#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"
#include "KEMLayer.h"


class KScreenDrawEditSearchRect : public IScreenDrawEdit
{
public:
    KScreenDrawEditSearchRect(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapFeedback, KEMLayer a_emLayer) ;
    virtual ~KScreenDrawEditSearchRect(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );
public:
    void MapSelectionFeedback(Integer a_nxID){};
    virtual ThemeLayerDrawEdit GetType() {
        return KEMScreenDrawEditSearchRect;
    }
public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}

public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
    virtual void Sleep() {}
private:
    void FirstPointCommandMsg();
private:
    TxPointPtr   m_spPtFirst;
    TxPolygonPtr m_spRect;
    ITxSymbolPtr m_spSymbolPoly;
private:
    ImChampMapSelectionReceiver* m_pMapFeedback;
    KEMLayer m_emLayer;
};