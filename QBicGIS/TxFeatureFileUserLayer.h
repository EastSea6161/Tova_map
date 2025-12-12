#pragma once

#include "ITxLayer.h"
#include "ITxFeatureUserLayer.h"
#include "TxTempMapServer.h"

class ThunderMapdrawProcess;
/*
// 용도 : 개발자가 간단하게 데이터를 지도에 그리는 경우
// 메모리 Low, 속도 High Under
// 공간데이터가 많은 경우
*/
/**
*@brief 레이어 특성
*@details 레이어 특성(유저) / 개발자가 간단하게 데이터를 지도에 그리는 경우 / 메모리 Low, 속도 High Under / 공간데이터가 많은 경우
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxFeatureFileUserLayer : public ITxFeatureUserLayer
{
public:
    TxFeatureFileUserLayer(TxLayerInfoPtr spLayerInfo);
    virtual ~TxFeatureFileUserLayer();

protected:
	/**
	*@brief 초기화
	*/
    virtual void Clear();
protected:
    TxTempMapServerPtr m_spFileMapServer;

protected:
	/**
	*@brief 표시된 특성 추출
	*@param __int64 nxID 인덱스 ID
	*@param ITxGeometryPtr spGeometry 기하학 정보
	*@return 0
	*/
    TxFeatureDispPtr GetFeatureDisp( __int64 nxID, ITxGeometryPtr spGeometry );
public:
	/**
	*@brief 특성 추출
	*@param __int64 nxID 인덱스 ID
	*@return 0
	*/
    virtual TxFeaturePtr   GetFeature(__int64 nxID);
    virtual ITxGeometryPtr GetGeometry(__int64 nxID);
	/**
	*@brief 특성 찾기
	*@param TxEnvelope oEnvelope					정도
	*@param std::vector<TxFeaturePtr>& vecFeature	특성 벡터
	*@param bool bIncludeHide						정의 숨김
	*/
    virtual void FindFeature    (TxEnvelope oEnvelope, std::vector<TxFeaturePtr>&     vecFeature, bool bIncludeHide=true );
	/**
	*@brief 특성 찾기 표출
	*@param TxEnvelope oEnvelope					정도
	*@param std::vector<TxFeatureDispPtr>& vecFeature	특성 벡터
	*@param bool bIncludeHide						정의 숨김
	*/
protected:
    virtual void FindFeatureDisp(TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, bool bIncludeHide=true);
protected:
	/**
	*@brief 특성 찾기 표출
	*@param TxEnvelope oEnvelope					정도
	*@param std::vector<TxFeatureDispPtr>& vecFeature	특성 벡터
	*@param double dMinExtent						최소 규모
	*@param ThunderMapdrawProcess*					맵 프로세스
	*/
    virtual void FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, double dMinExtent, ThunderMapdrawProcess*);

public:
	/**
	*@brief 기하학 추출
	*@param std::map<__int64, ITxGeometryPtr>& mapGeometry 
	*/
    virtual void GetGeometry(std::map<__int64, ITxGeometryPtr>& mapGeometry);

//*******************************************************************************
public:
	/**
	*@brief 제거
	*/
    virtual void Delete();
	/**
	*@brief 제거
	*@param __int64 nxID 인덱스 ID
	*/
    virtual void Delete(__int64 nxID);
	/**
	*@brief 수정
	*@param TxFeaturePtr spFeature 특성정보
	*/
    virtual void Update(TxFeaturePtr spFeature);
	/**
	*@brief 삽입
	*@param TxFeaturePtr spFeature 특성정보
	*/
    virtual void Insert(TxFeaturePtr spFeature);
	/**
	*@brief 삽입
	*@param std::vector<TxFeaturePtr>& vecFeature 특성정보
	*/
    virtual void Insert(std::vector<TxFeaturePtr>& vecFeature);
    virtual void Insert(TxFeatureDispPtr spFeatureDisp);
    virtual void Insert(std::vector<TxFeatureDispPtr>& vecFeatureDisp);
//*******************************************************************************
public:
	/**
	*@brief 특성 표출
	*@param std::vector<TxFeatureDispPtr>& vecFeatureDisp 표출 할 특성 정보
	*/
    virtual void PutFeatureDisp(std::vector<TxFeatureDispPtr>& vecFeatureDisp);
public:
	/**
	*@brief 규모
	*/
    virtual TxEnvelope Extent();
	/**
	*@brief 엔티티 개수
	*/
    virtual __int64    EntityCount();
	/**
	*@brief 최대값 ID
	*/
    virtual __int64    MaxID();
};

typedef std::shared_ptr<TxFeatureFileUserLayer> TxFeatureFileUserLayerPtr;