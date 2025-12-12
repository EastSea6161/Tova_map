#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"
#include "KEMLayer.h"

enum KEMTargetRegionInsertStatus
{
    KEMTargetRegionInsertNothing = 0,
    KEMTargetRegionInserting,
    KEMKEMTargetRegionInsertComplete      
};

class KScreenDrawEditTargetRegionInsert : public IScreenDrawEdit
{
public:
    KScreenDrawEditTargetRegionInsert(IMapView* a_mapRequest, int a_nTargetRegionID, ImTasTargetRegionReceiver* a_pMapFeedback, KEMLayer a_emLayer) ;
    virtual ~KScreenDrawEditTargetRegionInsert(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );
public:
    void MapSelectionFeedback(Integer a_nxID){};
    virtual ThemeLayerDrawEdit GetType()
    {
        return KEMScreenDrawEditSearchPolygon;
    }

public:
public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
    virtual void Sleep();
private:
    void FeedbackIntersectInfo(TxPolygonPtr spPolygon);
private:
    void FirstPointCommandMsg();
    void AddPointCommandMsg();
private:
    KEMTargetRegionInsertStatus m_emEditStatus;
private:
private:
    ImTasTargetRegionReceiver* m_pMapFeedback;
    KEMLayer m_emLayer;
private:
    int m_nTargetRegionID;

private:
private:
    ITxSymbolPtr m_spSymbolLine;
    ITxSymbolPtr m_spSymbolPoly;
private:
    ITxGeometryPtr m_spGeoDraw;    
private:
    std::vector<TxPoint> m_vecPt;
protected:
    virtual void Draw();
protected:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
};