#pragma once
#include "ITxObject.h"
#include "ITxGeometry.h"
/**
*@brief 특성
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxFeature : public ITxObject
{
public:
    TxFeature();
    TxFeature(__int64 nxFeatureID, ITxGeometryPtr spGeometry);
    TxFeature(const TxFeature& right);

    virtual ~TxFeature();
protected:
    ITxGeometryPtr m_spGeometry;
public:
	/**
	*@brief 기하학
	*@return 기하학 정보 반환
	*/
    const ITxGeometryPtr Geometry() const;    
public:
	/**
	*@brief 기하학 정보 복사
	*@return 복사 정보 반환
	*/
    ITxGeometryPtr CopyGeometry();
public:
	/**
	*@brief 복사
	*@return 데이터 복사
	*/
    std::shared_ptr<TxFeature> Copy();
protected:
	/**
	*@brief 기하학
	*@param ITxGeometryPtr spGeometry	기하학 정보
	*/
    void  Geometry (ITxGeometryPtr spGeometry);
};

typedef std::shared_ptr<TxFeature> TxFeaturePtr;

