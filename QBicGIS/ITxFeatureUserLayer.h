#pragma once

#include "ITxLayer.h"
#include "ITxFeatureLayer.h"

class ThunderMapdrawProcess;
/*
// 용도 : 개발자가 간단하게 데이터를 지도에 그리는 경우
// 메모리 High, 속도 High
// 공간데이터가 많은 경우 TxFeatureFileUserLayer를 사용할 것
*/
/**
*@brief 레이어 특성
*@details 레이어 특성(유저) / 개발자가 간단하게 데이터를 지도에 그리는 경우 / 메모리 High, 속도 High / 공간데이터가 많은 경우 TxFeatureFileUserLayer를 사용할 것
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxFeatureUserLayer : public ITxFeatureLayer
{
public:
    ITxFeatureUserLayer(TxLayerInfoPtr spLayerInfo) : ITxFeatureLayer(spLayerInfo) {}
    virtual ~ITxFeatureUserLayer() {}

protected:
	/**
	*@brief 초기화
	*/
    virtual void Clear()  = 0;
//*******************************************************************************
public:
	/**
	*@brief 제거
	*/
    virtual void Delete() = 0;
	/**
	*@brief 제거
	*@param __int64 nxID 인덱스 ID
	*/
    virtual void Delete(__int64 nxID) = 0;
	/**
	*@brief 수정
	*@param TxFeaturePtr spFeature 특성정보
	*/
    virtual void Update(TxFeaturePtr spFeature) = 0;

    virtual void Update(__int64 nxID, ITxGeometryPtr spGeometry) {
        Update(TxFeaturePtr(new TxFeature(nxID, spGeometry)));
    }

	/**
	*@brief 삽입
	*@param TxFeaturePtr spFeature 특성정보
	*/
    virtual void Insert(TxFeaturePtr spFeature) = 0;

    virtual void Insert(__int64 nxID, ITxGeometryPtr spGeometry) {
        Insert(TxFeaturePtr(new TxFeature(nxID, spGeometry)));
    }
	/**
	*@brief 삽입
	*@param std::vector<TxFeaturePtr>& vecFeature 특성정보
	*/
    virtual void Insert(std::vector<TxFeaturePtr>& vecFeature) = 0;
    virtual void Insert(TxFeatureDispPtr spFeatureDisp) = 0;
    virtual void Insert(std::vector<TxFeatureDispPtr>& vecFeatureDisp) = 0;
//*******************************************************************************
public:
	/**
	*@brief 특성 표출
	*@param std::vector<TxFeatureDispPtr>& vecFeatureDisp 특성 정보 표출
	*/
    virtual void PutFeatureDisp(std::vector<TxFeatureDispPtr>& vecFeatureDisp) = 0;

//*******************************************************************************
public:
    virtual __int64 MaxID() = 0;
};

typedef std::shared_ptr<ITxFeatureUserLayer> ITxFeatureUserLayerPtr;