#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"

class TCoordinate;

enum KEMTargetRegionEditStatus
{
    KEMTargetRegionEditNothing = 0,
    KEMTargetRegionVertexMove         
};

class KScreenDrawEditTargetRegionEdit : public IScreenDrawEdit
{
public:
    KScreenDrawEditTargetRegionEdit(IMapView* a_mapRequest, 
                                    int a_nTargetRegionID, std::vector<TCoordinate> vecCoordinate, 
                                    ImTasTargetRegionReceiver* a_pMapFeedback) ;
    virtual ~KScreenDrawEditTargetRegionEdit(void) ;

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
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
    virtual void Sleep();
private:
    void FeedbackIntersectInfo();

private:
    void FirstPointCommandMsg();
    void AddPointCommandMsg();
private:
    KEMTargetRegionEditStatus m_emEditStatus;
private:
private:
    ImTasTargetRegionReceiver*   m_pMapFeedback;
private:
    int m_nTargetRegionID;
private:
    TxFeaturePtr m_spFeature;

private:
    bool IsHittingLinkVertex (double mapX, double mapY, size_t& nxIdx);
    bool InsertVertex(double mapX, double mapY, size_t& nxIdx);
    bool DeleteVertex(double mapX, double mapY, size_t& nxIdx);
private:
private:
    ITxSymbolPtr m_spSymPoly;
    ITxSymbolPtr m_spSymVertex;
private:
    int m_nFlag;
    int m_nIdx;
protected:
    virtual void Draw();
protected:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY); //{}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY); //{}
};