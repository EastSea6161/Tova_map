#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "ShapeEditDemarcationInsert.h"

class KIORow;

enum KEMNodeDeleteStatus
{
	KEMUserMapMultiSelectMode  
};

typedef std::shared_ptr<KIORow> KIRowPtr;

class KShapeEditNodeDelete : public IShapeEdit
{
public:
	KShapeEditNodeDelete(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
	virtual ~KShapeEditNodeDelete(void) ;

public:
	virtual void CommandFeedback(CString a_strResult);
	virtual void Canceled       (                   );

	virtual KEMMapShapeEdit GetType() {
		return KEMMapShapeEditNodeUpdate;
	}

public:
public:
	virtual void StartEditing();
	virtual void UpdateNode  ();
	virtual void StopEditing ();
	virtual void Sleep       (){};
	virtual WORD GetCursorID ();

private:
	KEMNodeDeleteStatus m_emNodeDeleteStatus;
private:
	std::vector<TxFeaturePtr> m_vecSelectedFeature;
private:    
	void UserSelectNodeFeatureCommand();    
	void SelectNodeFeature(TxEnvelope& oEnvelope);
	void UserSelectEditMode();              
	void DeleteProcess();
	bool DeleteNode(KDBaseConPtr a_spDBaseConnection, TxFeatureLayerPtr spLayer, TxFeaturePtr spFeature);
	void DeleteZone(KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID);
	void DeleteZoneFeature( Integer a_nxZoneID );
    void DeleteODData(KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID);
    void DeleteLink(KDBaseConPtr a_spDBaseConnection, std::set<Integer>& a_setLinkID, QBicDonkeyProgress& a_oDonkeyProgress);
private:
	void Initialize();
private:
    void ExistRelationData(KDBaseConPtr a_spDBaseConnection, std::set<Integer>& ar_setLinkID, std::set<Integer>& ar_setTransitID);
    CString RelationLinkInfoMsg   (std::set<Integer>& ar_setLinkID);
    CString RelationTransitInfoMsg(std::set<Integer>& ar_setTransitID);

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