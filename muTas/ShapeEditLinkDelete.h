#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"

class KIORow;

enum KEMLinkDeleteStatus
{
	KEMLinkDeleteUserMapSelectMode
};

typedef std::shared_ptr<KIORow> KIRowPtr;

class KShapeEditLinkDelete : public IShapeEdit
{
public:
	KShapeEditLinkDelete(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
	virtual ~KShapeEditLinkDelete(void) ;

public:
	virtual void CommandFeedback(CString a_strResult);
	virtual void Canceled       (                   );

	virtual KEMMapShapeEdit GetType() {
		return KEMMapShapeEditLinkUpdate;
	}

public:
public:
	virtual void StartEditing();
	virtual void SaveEditing();
	virtual void StopEditing();
	virtual void Sleep       (){};
	virtual WORD GetCursorID ();
private:
	KEMLinkDeleteStatus m_emLinkDeleteStatus;
private:
	std::vector<TxFeaturePtr> m_vecSelectedFeature;

private:
	void Initialize();
private:    
	void UserSelectLinkFeatureCommand();   
	void SelectFeature(TxEnvelope& oEnvelope);
	void UserSelectEditMode();
	void DeleteProcess();
	bool DeleteLink(KDBaseConPtr a_spDBaseConnection, TxFeatureLayerPtr spFeaLayer, TxFeaturePtr spFeature);
private:
    void RelationTransitData(KDBaseConPtr a_spDBaseConnection, std::set<Integer>& ar_setLinkViaTransitID);

private:
    TxPointPtr   m_spPtFirst;
    TxPolygonPtr m_spRect;
    ITxSymbolPtr m_spSymbolPoly;
public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
};