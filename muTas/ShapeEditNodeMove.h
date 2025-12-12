#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "ShapeEditDemarcationInsert.h"
#include "ImChampMapSelectionReceiver.h"
#include "ScreenDrawFlashNode.h"


class KIORow;

typedef std::shared_ptr<KIORow> KIRowPtr;

class KShapeEditNodeMove : public IShapeEdit, public ImChampMapSelectionReceiver
{
public:
    KShapeEditNodeMove(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditNodeMove(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType()
    {
        return KEMMapShapeEditNodeUpdate;
    }

public:
    virtual void StartEditing();
    virtual void StopEditing ();
    virtual void Sleep       (){};
    virtual WORD GetCursorID ();
    virtual void OnShapeEditPopupCommand(UINT nID);
    virtual bool GetShapeEditPopupInfo(UINT&  nMenuID, int& nPos);
  
public:
    virtual void MapSelectionFeedback(Integer a_nxID);
    virtual void MapSelectionNoneFeedback();

private:    
    void UserSelectNodeFeatureCommand();    
    void UserSelectEditMode(); 

private:
    void Initialize();
private:
    void SelectNodeFeature(double a_dMapX, double a_dMapY);
    KScreenDrawFlashNodePtr m_spScreenDrawFlashNode;
    void CleareMovedFeedback();

private:
    int m_nFlag;
    ITxSymbolPtr m_spSymLine;
    ITxSymbolPtr m_spSymPt;
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
    std::vector<TxFeaturePtr> m_vecFLink;
    std::vector<TxFeaturePtr> m_vecTLink;
    TxFeaturePtr m_spNodeFeature;

private:
    bool SelectFeature(double mapX, double mapY);
    void Save();
};