#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"

class IShapeEditController;

enum KEMDemarcationInsertStatus
{
    KEMDemarcationInsertStatusNothing       = 0, 
    KEMDemarcationInsertStatusInput         = 1,
    KEMDemarcationInsertStatusInputed       = 2,
    KEMDemarcationInsertStatusFirstPoint    = 3,
    KEMDemarcationInsertStatusPointInputed  = 4, 
    KEMDemarcationInsertStatusAdd           = 6,
    KEMDemarcationInsertStatusComplete      = 9
};

class KShapeEditDemarcationInsert : public IShapeEdit
{
public:
    KShapeEditDemarcationInsert(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditDemarcationInsert(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType()
    {
        return KEMMapShapeEditDemarcationInsert;
    }

public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
	virtual void Sleep       (){};
	virtual WORD GetCursorID ();
private:
    void FirstPointCommandMsg();
    void AddPointCommandMsg();

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
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
};

typedef std::shared_ptr<KShapeEditDemarcationInsert> KShapeEditDemarcationInsertPtr;