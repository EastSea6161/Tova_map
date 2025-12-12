#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "ScreenDrawFlashLink.h"
#include "ImChampMapSelectionReceiver.h"

class KShapeEditLinkOneWayVertex : public IShapeEdit, public ImChampMapSelectionReceiver
{
public:
    KShapeEditLinkOneWayVertex(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditLinkOneWayVertex(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType() {
        return KEMMapShapeEditOneWayLinkVertex;
    }
    
public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
    virtual void Sleep       (){};
    virtual WORD GetCursorID ();
private:
    void LinkSelectMessage();
private:
private:
    bool IsHittingLinkVertex     (double mapX, double mapY, size_t& nxIdx);
    void StartVertexMove         (double a_dMapX, double a_dMapY, long a_lPartIndex, long a_lVertexIndex);

private:
    bool InsertVertex(double mapX, double mapY, size_t& nxIdx);

private:
    void UpdateFeature();
public:
    virtual void MapSelectionFeedback(Integer a_nxID);
    virtual void MapSelectionNoneFeedback();

private:
    int m_nFlag;
    int m_nIdx;
private:
    ITxSymbolPtr m_spSymLine;
    ITxSymbolPtr m_spSymVertex;
    ITxSymbolPtr m_spSymStart;
    ITxSymbolPtr m_spSymEnd;
private:
    TxFeaturePtr m_spFeature;
    std::map<__int64, TxFeaturePtr> SelectLink(double mapX, double mapY);
private:
    void MinDistFeatureSelect(std::map<__int64, TxFeaturePtr>& mapFeature, double mapX, double mapY);
public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
private:
    TxFeatureUserLayerPtr m_spUserLayer;
private:
    KScreenDrawFlashLinkPtr m_spScreenDrawFlashLink;

private:
    double m_dMapX;
    double m_dMapY;
};