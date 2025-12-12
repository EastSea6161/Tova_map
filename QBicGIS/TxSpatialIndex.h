#pragma once
#include "TxEnvelope.h"
#include "IncludeBoost.h"

namespace XTMemorySpatialIndex
{
    static BoostPoint m_oSortPoint;
}

/**
*@brief 인덱스 공간
*@details 인덱스 공간
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxSpatialIndex
{
public:
    ITxSpatialIndex()          {}
    virtual ~ITxSpatialIndex() {}

public:
	/**
	*@brief 삽입
	*@param __int64 nxID				인덱스 ID
	*@param const TxEnvelope& oEnvelope 범위
	*/
    virtual void Insert(__int64 nxID, const TxEnvelope& oEnvelope) = 0;
	/**
	*@brief 제거
	*@param __int64 nxID				인덱스 ID
	*/
    virtual void Remove(__int64 nxID) = 0;
public:
	/**
	*@brief 초기화
	*/
    virtual void Clear () =0;

public:
	/**
	*@brief 교차점
	*@param const TxEnvelope& oEnvelope	범위
	*@return 추가된 결과 반환
	*/
    virtual std::vector<__int64> Intersect(const TxEnvelope& oEnvelope) = 0;

public:
	/**
	*@brief 규모 추출
	*@return 
	*/
    virtual TxEnvelope GetExtent()   = 0;
	/**
	*@brief 엔티티 개수
	*@return 엔티티 크기 반환
	*/
    virtual __int64    EntityCount() = 0;
};

/**
*@brief 인덱스 메모리공간
*@details 인덱스 메모리공간
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxMemorySpatialIndex : public ITxSpatialIndex
{
public:
    TxMemorySpatialIndex() {};
    virtual ~TxMemorySpatialIndex(){};
private:    
    typedef std::pair<BoostEnvelope, __int64> TreeValue;
    BoostGeometry::index::rtree< TreeValue, BoostGeometry::index::quadratic<16> > m_oRTree;

public:
	/**
	*@brief 삽입
	*@param __int64 nxID				인덱스 ID
	*@param const TxEnvelope& oEnvelope 범위
	*/
    virtual void Insert(__int64 nxID, const TxEnvelope& oEnvelope);
	/**
	*@brief 제거
	*@param __int64 nxID				인덱스 ID
	*/
    virtual void Remove(__int64 nxID);
public:
	/**
	*@brief 초기화
	*/
    virtual void Clear();
public:
	/**
	*@brief 교차점
	*@param const TxEnvelope& oEnvelope	범위
	*@return 추가된 결과 반환
	*/
    virtual std::vector<__int64> Intersect(const TxEnvelope& oEnvelope);

public:
	/**
	*@brief 화면 종류
	*@param TreeValue& left
	*@param TreeValue& right
	*@return 
	*/
    static bool SortDist( TreeValue& left, TreeValue& right )
    {
        BoostPoint& oLeftMaxPt  = left.first.max_corner();
        BoostPoint& oRightMaxPt = left.first.max_corner();

        double dDistLC = BoostGeometry::distance(oLeftMaxPt,  XTMemorySpatialIndex::m_oSortPoint);
        double dDistRC = BoostGeometry::distance(oRightMaxPt, XTMemorySpatialIndex::m_oSortPoint);
        
        return dDistLC >= dDistRC;
    }

public:
	/**
	*@brief 규모 추출
	*@return 
	*/
    virtual TxEnvelope GetExtent();
	/**
	*@brief 엔티티 개수
	*@return 엔티티 크기 반환
	*/
    virtual __int64    EntityCount();
};

typedef std::shared_ptr<ITxSpatialIndex> ITxSpatialIndexPtr;
