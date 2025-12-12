#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "ImChampMapSelectionReceiver.h"

class KShapeEditLinkTwoWayVertex  : public IShapeEdit, public ImChampMapSelectionReceiver
{
public:
    KShapeEditLinkTwoWayVertex(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditLinkTwoWayVertex(void);
public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType() {
        return KEMMapShapeEditTwoWayLinkVertex;
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
    virtual void MapSelectionFeedback(Integer a_nxID);
    virtual void MapSelectionNoneFeedback();
private:
private:
    TxFeatureUserLayerPtr m_spUserLayer;
private:
    ITxSymbolPtr m_spSymLine;
    ITxSymbolPtr m_spSymVertex;
    ITxSymbolPtr m_spSymStart;
    ITxSymbolPtr m_spSymEnd;

private:
    TxFeaturePtr m_spFeature;
    std::map<__int64, TxFeaturePtr> m_mapFeature;
private:
    int m_nFlag; //★ 0 : 선택인 안된경우, 1 : 최초선택(마우스클릭), 2 : 선택 이후 절점 편집 가능 모드 
    int m_nIdx;
private:
    virtual void Draw();
private:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}

private:
    std::map<__int64, TxFeaturePtr> SelectLink(double mapX, double mapY);
private:
    bool IsHittingLinkVertex     (double mapX, double mapY, size_t& nxIdx);
private:
    bool InsertVertex(double mapX, double mapY, size_t& nxIdx);
private:
    void UpdateFeature();
};