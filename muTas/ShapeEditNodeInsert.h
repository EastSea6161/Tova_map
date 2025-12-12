#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "ShapeEditDemarcationInsert.h"

#include "TShapeEdit.h"

class IShapeEditController;

enum KEMNodeInsertStatus
{
    KEMNodeInsertStatusNothing             , 
    KEMNodeInsertStatusNodeInput           ,
    KEMNodeInsertStatusNodeInputed         ,
    KEMNodeInsertStatusNodeTypeInput       ,
    KEMNodeInsertStatusNodeTypeInputed     , 
    KEMNodeInsertStatusDemarcationInputYN  ,
    KEMNodeInsertStatusDemarcationInput    ,
    KEMNodeInsertStatusDemarcationInputed  ,
    KEMNodeInsertStatusLinkDevideYN        ,
    KEMNodeInsertStatusLinkDevided         ,
    KEMNodeInsertStatusCompleted          
};

class KShapeEditNodeInsert : public IShapeEdit
{
public:
    KShapeEditNodeInsert(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditNodeInsert(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       ();

    virtual KEMMapShapeEdit GetType()
    {
        return KEMMapShapeEditNodeInsert;
    }

public:
    virtual void StartEditing();    
    virtual void StopEditing ();
	virtual void Sleep       (){};
	virtual WORD GetCursorID ();
private:
    void NotifyTables();

public:
	virtual void SaveEditing ();
private:
	void InsertNode(KDBaseConPtr spDBCon, Integer nxNodeID, CString strLblColumn, std::map<__int64, CString>& mapLabel);
	void         SplitLinkInsert   (KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID, CString strLblColumn, std::map<__int64, CString>& mapLabel);
	void         DemarcationInsert (KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID);

private:
	KEMNodeInsertStatus m_emNodeInsertStatus;
private:
    void FirstNodeInputMessageWrite();
		
    void SecondNodeTypeConditionAction();
		void ThirdDemarcationConditionAction();
			// Demarcation ют╥б
			private:
				private:
				void  CreateDemarcationProcess();
									
				private: 
				void  CreateDefaultDemarcationProcess();
		private:
			void NodeInputAction();
				std::vector<TxFeaturePtr> FindIntersectLink();		
				void LinkDevideAction();
private:
    int  m_nNodeType;
	long m_lMouseX;
	long m_lMouseY;
	WORD m_wMouseCursorID;
private:
       
private:
    void Initialize();
private:
    ITxSymbolPtr m_spSymLine;
    ITxSymbolPtr m_spSymPt;
private:
    TxPointPtr  m_spNodePoint;
    std::vector<TxFeaturePtr> m_vecILink;

private:
    TxPolygonPtr m_spDemarcation;
private:
    ITxSymbolPtr   m_spSymPolygonLine;
    ITxSymbolPtr   m_spSymPoly;
private:
    ITxGeometryPtr m_spGeoDraw;  
private:
    std::vector<TxPoint> m_vecPt;
public:
    virtual void Draw();
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

private:
    void RecalcNodePosition();
 };