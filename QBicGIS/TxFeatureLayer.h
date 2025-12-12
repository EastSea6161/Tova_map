#pragma once

#include <bitset>

#include "ITxObject.h"
#include "TxLayerInfo.h"
#include "ITxGeometry.h"
#include "TxSpatialIndex.h"

#include "ITxSymbol.h"
#include "TxFeature.h"
#include "ITxLabelSymbol.h"
#include "ITxLayer.h"
#include "ITxFeatureLayer.h"

class ITxMapServer;
class ThunderMapdrawProcess;
class TxItem;
/**
*@brief 레이어 특성
*@details 레이어 특성
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxFeatureLayer : public ITxFeatureLayer
{
public:
    TxFeatureLayer (/*ITxMapServerPtr*/std::shared_ptr<ITxMapServer> spMapServer, TxLayerInfoPtr spLayerInfo); 
    virtual ~TxFeatureLayer();

private:
    std::shared_ptr<ITxMapServer>/*ITxMapServerPtr*/ m_spMapServer;
public:
	/**
	*@brief 서버명 
	*@return 맵 서버 
	*/
    ITxMapServerPtr GetServer() {
        return m_spMapServer;
    }

public:
	/**
	*@brief 초기화
	*/
    virtual void Clear();
private:
	/**
	*@brief 표시된 특성 추출
	*@param __int64 nxID 인덱스 ID
	*@param ITxGeometryPtr spGeometry 기하학 정보
	*@return 0
	*/
    TxFeatureDispPtr GetFeatureDisp(__int64 nxID, ITxGeometryPtr spGeometry);
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
    virtual void FindFeature(TxEnvelope oEnvelope, std::vector<TxFeaturePtr>& vecFeature, bool bIncludeHide=true);
	/**
	*@brief 특성 찾기 표출
	*@param TxEnvelope oEnvelope					정도
	*@param std::vector<TxFeatureDispPtr>& vecFeature	특성 벡터
	*@param bool bIncludeHide						정의 숨김
	*/
protected:
    virtual void FindFeatureDisp(TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, bool bIncludeHide=true);
public:
	/**
	*@brief 기하학 추출
	*@param std::map<__int64, ITxGeometryPtr>& mapGeometry 
	*/
    virtual void GetGeometry(std::map<__int64, ITxGeometryPtr>& mapGeometry);
protected:
		/**
	*@brief 특성 찾기 표출
	*@param TxEnvelope oEnvelope					정도
	*@param std::vector<TxFeatureDispPtr>& vecFeature	특성 벡터
	*@param double dMinExtent						최소 규모
	*@param ThunderMapdrawProcess*					맵 프로세스
	*/
    virtual void FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, double dMinExtent, ThunderMapdrawProcess*);

//Feature 단위로 조작 : 특별하게 Feature의 모양을 다르게 표현해야 할 경우
//*******************************************************************************
public:
	/**
	*@brief 레이어 특성 확인
	*@return 참 반환
	*/
    virtual bool IsFeatureLayer() {return true;}

private:
	/**
	*@brief 삽입
	*@param __int64 nxID				인덱스 ID
	*@param ITxGeometryPtr spGeometry	기하학 정보
	*/
    virtual void Insert(__int64 nxID, ITxGeometryPtr spGeometry);
public:
    //★ TxFeatureLayer는 지오메트리정보와 속성정보가 쌍으로 이루어 져야 함.
    //★ ProhibitGeoInsert 함수는 속성 정보는 입력하지 않고 지오메트리 정보만 추가하는 함수
    //★ 개발자가 지오메트리와 속성 정보를 분리해서 입력 처리할 경우만 사용 됨
    void ProhibitGeoInsert(__int64 nxID, ITxGeometryPtr spGeometry);
public:
	/**
	*@brief 삽입
	*@param __int64 nxID				인덱스 ID
	*@param ITxGeometryPtr spGeometry	기하학 정보
	*@param TxRecord& oRecord			레코드 정보
	*/
    virtual void Insert(__int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord);
	/**
	*@brief 삭제
	*@param __int64 nxID				인덱스 ID
	*/

    virtual void Delete();
    virtual void Delete(__int64 nxID);
	/**
	*@brief 수정
	*@param __int64 nxID				인덱스 ID
	*@param ITxGeometryPtr spGeometry	기하학 정보
	*/
    virtual void Update(__int64 nxID, ITxGeometryPtr spGeometry);
	/**
	*@brief 수정
	*@param __int64 nxID				인덱스 ID
	*@param TxRecord& oRecord			레코드 정보
	*/
    virtual void Update(__int64 nxID, TxRecord& oRecord);
	/**
	*@brief 수정
	*@param __int64 nxID				인덱스 ID
	*@param ITxGeometryPtr spGeometry	기하학 정보
	*@param TxRecord& oRecord			레코드 정보
	*/
    virtual void Update(__int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord);    

public:
	/**
	*@brief 규모
	*@return 규모 반환
	*/
    virtual TxEnvelope Extent();
	/**
	*@brief 엔티티 개수
	*@return 엔티티 개수 반환
	*/
    virtual __int64    EntityCount();

public:
    virtual void RebuildIndex();
	virtual void RebuildIndexGridSize(double a_dGSize);
};


typedef std::shared_ptr<TxFeatureLayer> TxFeatureLayerPtr;