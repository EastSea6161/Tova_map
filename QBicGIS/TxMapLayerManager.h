#pragma once
#include "ITxLayer.h"
#include "TxMapEnumType.h"

/**
*@brief 맵 레이어 관리
*@details 맵 레이어 관리
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxMapLayerManager
{
public:
    TxMapLayerManager() ;
    virtual ~TxMapLayerManager();

public:
	/**
	*@brief 레이어 추출
	*@param int nLayerID	레이어 ID
	*@return 레이어 ID  / 실패시 nullptr
	*/
    ITxLayerPtr GetLayer(int nLayerID);
public:    
	/**
	*@brief 레이어 제거
	*@param int nLayerID	레이어 ID
	*/
    void RemoveLayer   (int nLayerID);
	/**
	*@brief 레이어 이동
	*@param int nLayerID	레이어 ID
	*@param int nLayerOrder	레이어 순서
	*/
    void MoveLayerOrder(int nLayerID, int nLayerOrder=0);
public:
	/**
	*@brief 레이어 추가
	*@param ITxLayerPtr spLayer	레이어 정보
	*@param int nLayerID		레이어 ID
	*@param int nLayerOrder		레이어 순서
	*@return 
	*/
    int AddLayer(ITxLayerPtr spLayer, int nLayerID, int nLayerOrder = -1/*-1:마지막*/);
private:
    std::vector<ITxLayerPtr>   m_vecDrawOrderLayers; // Drawing Order와 일치 시킨다.
    std::map<int, ITxLayerPtr> m_mapLayers;          // 레이어 ID, Layer
public:
    //★ 현재 레이어의 Ordering -> <레이어ID, 레이어ID, ...>
    std::vector<int> LayerOrder();

public:
    void LayerOrderByID();
public:
    // Drawing Order
	/**
	*@brief 레이어(벡터)
	*@return 레이어(벡터) 반환
	*/
    std::vector<ITxLayerPtr> Layers() {
        return m_vecDrawOrderLayers;
    }
public:
    // LayerID, Layer
	/**
	*@brief 레이어(맵)
	*@return 레이어(맵) 반환
	*/
    std::map<int, ITxLayerPtr> MapLayers() {
        return m_mapLayers;
    }
public:
	/**
	*@brief 레이어 수
	*@return 레이어 수
	*/
    int LayerCount();
public:
	/**
	*@brief 최대 레이어 
	*@return 최대 레이어 ID
	*/
    int MaxLayerID();

friend class ThunderDxDraw;
friend class ThunderMapdrawProcess;
};

typedef std::shared_ptr<TxMapLayerManager> TxMapLayerManagerPtr;