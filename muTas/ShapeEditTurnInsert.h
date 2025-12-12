#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"

#include "ScreenDrawFlashLink.h"
#include "ImChampMapSelectionReceiver.h"

class IShapeEditController;

enum KEMTurnInsertStatus
{
    KEMTurnInsertStatusNothing     = 0, 
    KEMTurnInsertStatusSelectedInLinks
};

enum KEMSelectMode
{
    KEMSelectModeInLink,
    KEMSelectModeOutLink
};

class KShapeEditTurnInsert : public IShapeEdit, public ImChampMapSelectionReceiver
{
public:
    KShapeEditTurnInsert(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditTurnInsert(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType()
    {
        return KEMMapShapeEditTurnInsert;
    }

public:
    
public:
    virtual void StartEditing();
    virtual void SaveEditing ();
    virtual void StopEditing ();
	virtual void Sleep       (){};
	virtual WORD GetCursorID ();

public:
	virtual void MapSelectionFeedback(Integer a_nxID);
	virtual void MapSelectionNoneFeedback();

private:
	WORD m_wCurSorID;
private:
    KEMTurnInsertStatus    m_emTurnInsertStatus;
private:
    void SelectInLinkCommandMessage ();
	void SelectOutLinkCommandMessage ();
private:
private:
	KScreenDrawFlashLinkPtr   m_spScreenDrawFlashLink;
private:
	void SelectInLinkProcess(Integer a_nxLinkID);
	TxFeaturePtr m_spInLinkFeature;
	/*교차로 정보*/ Integer m_nxIntersectionID;
	void SelectOutLinkProcess(Integer a_nxLinkID);
	TxFeaturePtr m_spOutLinkFeature;

private:
    ITxSymbolPtr m_spSymLine;

private:
    void EditDraw(int nFlag, bool bRefresh = false);
public:
    virtual void Draw();
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);
private:
    TxFeatureUserLayerPtr m_spBackLayer;
};

