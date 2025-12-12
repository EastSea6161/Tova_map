#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "ShapeEditDemarcationInsert.h"
#include "ImChampMapSelectionReceiver.h"
#include "ScreenDrawFlashNode.h"

class KIORow;

typedef std::shared_ptr<KIORow> KIRowPtr;

class KShapeEditNodeProperties : public IShapeEdit, public ImChampMapSelectionReceiver
{
public:
    KShapeEditNodeProperties(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditNodeProperties(void) ;

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
    KIRowPtr  m_spOriginRow;
    KIRowPtr  m_spUserUpdatedRow;

private:    
    void UserSelectNodeFeatureCommand();    
    void EditPropertiesProcess();
    void EditProperties    (Integer a_nxNodeID );
    bool IsChanged    (const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow);
    bool IsNodeIDChanged   (const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow, int a_nIndex, Integer& ar_nxOriginalNodeID,  Integer& ar_nxUpdatedNodeID); 
    bool IsNodeTypeChanged (const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow, int a_nIndex, int&     ar_nOriginalNodeType, int&     ar_nUpdatedNodeType);
    void ModifyZoneFeatureID();
    void CreateDefaultDemarcation(Integer a_nxNodeID);
    void UpdateProcess();
    void UpdateNodeFeature  (Integer a_nxOriginID, Integer a_nxNewID, int nType);
    void UpdateZoneFeatureID(Integer a_nxOriginID, Integer a_nxNewID);
    void DeleteZoneFeature  (Integer a_nxZoneID);
    void UpdateAllODZoneID  (KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginID, Integer a_nxNewID);
    void UpdateNodeProperties (KDBaseConPtr a_spDBaseConnection);
private:
    void Initialize();

private:
    KScreenDrawFlashNodePtr m_spScreenDrawFlashNode;

public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}

private:
    __int64 m_nxID;
};