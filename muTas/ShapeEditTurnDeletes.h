#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"

class KIORow;

enum KEMTurnDeleteStatus
{
	KEMTurnDeleteUserMapSelectMode
};

typedef std::shared_ptr<KIORow> KIRowPtr;

class KShapeEditTurnDeletes : public IShapeEdit
{
public:
	KShapeEditTurnDeletes(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
	virtual ~KShapeEditTurnDeletes(void) ;

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
private:
	KEMTurnDeleteStatus m_emLinkDeleteStatus;
private:
	std::vector<TxFeaturePtr> m_vecSelectedFeature;
private:
	CString  m_strName;
    
private:    
	void UserSelectTurnMessage();   
	void DeleteProcess();

private:
    TxPointPtr   m_spPtFirst;
    TxPolygonPtr m_spRect;
private:
    ITxSymbolPtr m_spSymPoly;
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
    void SelectFeature(TxEnvelope& oEnvelope);
};