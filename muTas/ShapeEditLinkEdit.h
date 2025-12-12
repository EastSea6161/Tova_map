#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "ScreenDrawFlashLink.h"
#include "ImChampMapSelectionReceiver.h"

class KIORow;

enum KEMLinkUpdateStatus
{
    KEMLinkUpdateMapSelectMode     , 
    KEMLinkUpdateLinkSelectedMode  ,
    KEMLinkVertexEditingMode
};

typedef std::shared_ptr<KIORow> KIRowPtr;

class KShapeEditLinkEdit : public IShapeEdit, public ImChampMapSelectionReceiver
{
public:
    KShapeEditLinkEdit(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, int nEditType, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditLinkEdit(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType()
    {
        return KEMMapShapeEditLinkUpdate;
    }

public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
	virtual void Sleep       (){};
	virtual WORD GetCursorID ();
	virtual void OnShapeEditPopupCommand(UINT nID);
	virtual bool GetShapeEditPopupInfo(UINT&  nMenuID, int& nPos);
private:
    KEMLinkUpdateStatus m_emLinkUpdateStatus;
private:
    __int64 m_nxID;
private:
    int m_nEditType;

public:
	virtual void MapSelectionFeedback(Integer a_nxID);
	virtual void MapSelectionNoneFeedback();

private:    
    void UserSelectLinkFeatureCommand();   
        void DeleteProcess();
            void DeleteLink(KDBaseConPtr a_spDBaseConnection, __int64 nxLinkID);
    
        void EditPropertiesProcess();
            void EditProperties  ( Integer a_nxLinkID );
            bool IsChanged       (const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow);
            bool IsLinkIDChanged (const KIORow* a_pOriginalRow, const KIORow* a_pUpdatedRow, int a_nIndex, Integer& ar_nxOriginalLinkID,  Integer& ar_nxUpdatedLinkID);
            
            void UpdateProcess();
            void UpdateLinkFeature    (Integer a_nxOriginID, Integer a_nxNewID, int a_nType);
            void UpdateLinkProperties (KDBaseConPtr a_spDBaseConnection, int nLabelIdx, std::map<__int64, CString>& mapLabel);
private:
private:
    KIRowPtr  m_spOriginRow;
    KIRowPtr  m_spUserUpdatedRow;
    bool      m_bApplyOpposite;

private:
	KScreenDrawFlashLinkPtr m_spScreenDrawFlashLink;
private:
    long m_lMouseDownX, m_lMouseDownY;

public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
};