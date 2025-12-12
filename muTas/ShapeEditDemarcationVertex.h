#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"

enum KEMDemarcationVertexStatus
{
    KEMDemarcationVertexNothing  = 0, 
    KEMDemarcationSelected       = 1,
    KEMDemarcationVertexMove     = 2
};

class KShapeEditDemarcationVertex : public IShapeEdit
{
public:
    KShapeEditDemarcationVertex(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditDemarcationVertex(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType() {
        return KEMMapShapeEditDemarcationVertex;
    }

public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
	virtual void Sleep       (){};
	virtual WORD GetCursorID ();
private:
    void ZoneSelectMessage();
private:
    KEMDemarcationVertexStatus   m_emDemarcationVertexStatus;
private:
    ITxSymbolPtr m_spSymPoly;
    ITxSymbolPtr m_spSymVertex;
private:
    int m_nFlag;
    int m_nIdx;
private:
    TxFeaturePtr m_spFeature;
private:
    bool IsHittingLinkVertex (double mapX, double mapY, size_t& nxIdx);
    bool InsertVertex(double mapX, double mapY, size_t& nxIdx);
    bool DeleteVertex(double mapX, double mapY, size_t& nxIdx);
private:
    void UpdateFeature();
private:
    std::map<__int64, TxFeaturePtr> FindZoneFeature(double mapX, double mapY);
    void MinDistFeatureSelect(std::map<__int64, TxFeaturePtr>& mapFeature, double mapX, double mapY);
public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
};
