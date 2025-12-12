#pragma once
///////////////////////////////////////////////
#include "KEMMapMode.h"
class KTarget;
class KCommandWnd;
class KPersonalGeoDatabase;
///////////////////////////////////////////////

#include "../QBicGIS/IThunderMapView.h"
#include "IMapReceiver.h"
#include "LayerXmlRenderer.h"

class IMapView : public IThunderMapView
{
public:
    IMapView(void) {}
    virtual ~IMapView(void) {}
public:
    virtual void FullExtentMap() = 0;

protected:
    std::map<IMapReceiveType, IMapReceiver*> m_mapReceiver;
public:
    IMapReceiver* MapReceiver(IMapReceiveType enType) {
        auto iter = m_mapReceiver.find(enType);
        if (iter != m_mapReceiver.end())
            return iter->second;

        return nullptr;
    } 

    void SetMapReceiver(IMapReceiveType enType, IMapReceiver* pReceiver) {
        IMapReceiver*& p = m_mapReceiver[enType];
        p = pReceiver;
    }

    void MapReceiverErase(IMapReceiveType enType) {
        m_mapReceiver[enType] = nullptr;
    }


/////////////////////////////////////////////////////////////////////////
public:
    virtual double GetPixelToRealDist     (double       a_dNoOfPixel)             = 0;
    // Screen Draw Refresh    
    virtual void   TasESRICacheRefresh    ()                                      = 0;
    virtual void   RefreshAll             ()                                      = 0;
       
    virtual void AddTurnEdit(__int64 nxTurnID, bool bRefresh = true) = 0;
    virtual void AddTurnMapDraw    (Integer a_nxTurnID, bool a_bRefresh = true) = 0;
    virtual void RemoveTurnMapDraw (Integer a_nxTurnID, bool a_bRefresh = true) = 0;

    virtual void AddIntersectionMapDraw    (Integer a_nxIntersectionID, bool a_bRefresh = true) = 0;
    virtual void RemoveIntersectionMapDraw (Integer a_nxIntersectionID, bool a_bRefresh = true) = 0;

    // 현재 지도의 Scale
    virtual double GetRequestMapScale   () = 0;

    // Layer Visible
    virtual bool NodeLayerVisible       () = 0;
    virtual bool LinkLayerVisible       () = 0;
    virtual bool TransitVisible         () = 0;
    virtual bool TurnVisible            () = 0;
    virtual bool IntersectionVisible    () = 0;

    // Target, CommandView Get
    virtual KTarget*     GetTarget      () = 0;
    virtual KCommandWnd* GetCommandView () = 0;
        
    // 지도의 MapMode Get, Set
    virtual KEMMapMode        GetMapMode() = 0;
    virtual void              SetMapMode(KEMMapMode emMapMode )= 0;
    virtual void              SetMapCursor(WORD a_wCursorID) = 0;
    virtual KPersonalGeoDatabase* GetPersonalGeoDataBase() const = 0;

    virtual CWnd* GetMapWindow() const = 0;

    virtual short GetCacheID() = 0;

    virtual void ShowPopupMenuNetworkEdit( long X, long Y ) {};

    virtual void ShapeDeletedItem (CString a_strLayer, Integer a_nxID) = 0;
    virtual void ShapeEditedItem  (CString a_strLayer, Integer a_nxID) = 0;
    virtual void NotifyNetworkEdior(int nEditType/*0:노드추가(노드), 1:노드추가(링크), 2:노드선택(링크)*/, Integer a_nxID) = 0;
    virtual void NotifyNetworkEdior(int nEditType/*0:예약, 1:링크추가*/, std::vector<__int64>& vecID) = 0;
    
    // Client Stopped
    //virtual void StoppedClent(int a_nFlag) = 0;
    virtual void WriteNetworkLog(CString& ar_strLog) = 0;

    virtual void NetworkEditLog(int a_nLayer, Integer a_nxID, int a_nEditType) = 0;

public:
    virtual Integer GetStartNodeID() = 0;
    virtual Integer GetStartLinkID() = 0;
    virtual void SetStartNodeID(Integer a_nxMaxNodeID) = 0;
    virtual void SetStartLinkID(Integer a_nxMaxLinkID) = 0;

    virtual __int64 GetMaxNodeID() = 0;
    virtual __int64 GetMaxLinkID() = 0;

    double MaxNodeLabelDisplayScale;
    double MaxLinkLabelDisplayScale;
   
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ 
public:
    /*nType ->0:All, 1:Node, 2:Link, 3:Zone*/
    virtual void ReloadRender(bool bRefresh=true, int nType = 0) = 0;
public:
    virtual ITxFeatureUserLayerPtr AddUserLayer(int nLayerID, bool bMemeory = true) = 0;
public:
    virtual KNodeLayerDispInfo NodeLayerDispInfo() = 0;
    virtual KLinkLayerDispInfo LinkLayerDispInfo() = 0;
    virtual ITxSymbolPtr NodeTypeSymbol(int nType) = 0;
    virtual ITxSymbolPtr LinkTypeSymbol(int nType) = 0;

public:
    virtual void UpdateNodeLinkMapDisp(int nLayerID, __int64 TxID, int nType, CString strLblColumn, CString strLblValue) = 0;

public:
    virtual void UpdateScreenCordonManage(int nType, __int64 nxID) = 0;
};