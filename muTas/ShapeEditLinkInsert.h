#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
//#include "ImChampMapSelectionReceiver.h"
#include "ScreenDrawFlashNode.h"

class IShapeEditController;

enum KEMLinkInsertStatus
{
    KEMLinkInsertStatusNothing           = 0, 
    KEMLinkInsertStatusFNodeInputed      = 1,
    KEMLinkInsertStatusVertexInput       = 2
};

class TTempPoint
{
public:
    TTempPoint(double x, double y)
    {
        Tx = x;
        Ty = y;
    }

    ~TTempPoint()
    {    
    }
    double Tx;
    double Ty;
};

class KShapeEditLinkInsert : public IShapeEdit/*, public ImChampMapSelectionReceiver*/
{
private:
	bool m_isTwowayLink;
public:
    KShapeEditLinkInsert(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, bool bTwoWayLink=true, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditLinkInsert(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType()
    {
        return KEMMapShapeEditLinkInsert;
    }
    
public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
	virtual void Sleep       (){};
	virtual WORD GetCursorID ();

private:
    KEMLinkInsertStatus m_emLinkInsertStatus;

public:
	virtual void MapSelectionFeedback(Integer a_nxID);
	virtual void MapSelectionNoneFeedback();

private:
    void ExistNodeFeatureStart();
    void NewNodeFeatureStart();
    void Clear();

private:
    TxFeaturePtr  m_spFNodeFeat; // 시점 노드(신규추가 또는 기존 노드)
    TxPolylinePtr m_spTempLine;
    std::vector<TxPoint> m_vecPt;
    TxFeaturePtr  m_spTNodeFeat; // 종점 노드(신규추가 또는 기존 노드)
    TxFeaturePtr  m_spFlashNode;
private:
    ITxSymbolPtr m_spSymLine;
    ITxSymbolPtr m_spSymFromPt;
    ITxSymbolPtr m_spSymFlash;

private:
    long MouseX;
    long MouseY;
public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}

private:
    TxFeaturePtr FindExistNode(double mapX, double mapY, int nFlag=1);
    TxFeaturePtr InsertNode   (double mapX, double mapY);
};