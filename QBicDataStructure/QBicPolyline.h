#pragma once
#include "IncludeQBicDataStructure.h"
#include "QBicConst.h"
#include "IQBicGeometry.h"

class QBicPoint;
class QBicEnvelope;
class QBicPolygon;
/**
*@brief 폴리라인
*@details 큐빅웨어 폴리라인
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class QBicPolyline : public IQBicGeometry
{
public:
    QBicPolyline ();
    QBicPolyline (const QBicPolyline& right);
    virtual ~QBicPolyline();
public:
    QBicPolyline& operator = (const QBicPolyline& right);
private:
    std::vector<QBicPoint> m_vecPoints;
public:
	/**
	*@brief 초기화
	*/
    void Clear();
	/**
	*@brief 포인트 위치 추가(1개)
	*@param double dX X좌표
	*@param double dY Y좌표
	*/
    void AddPoint(double dX, double dY);
	/**
	*@brief 포인트 위치 추가(다중)
	*@param std::vector<QBicPoint>& vecPoint 포인트 벡터
	*/
    void AddPoint(std::vector<QBicPoint>& vecPoint);

public:
    QBicEnvelope m_oEnvelope;
public:
	/**
	*@brief 범위지정
	*@return 초반 범위 확인
	*/
    virtual QBicEnvelope GetEnvelope();

public:
	/**
	*@brief 정상적인 폴리라인 확인
	*@param QBicPolyline& rOther 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
    bool Intersect(QBicPolyline& rOther);
	/**
	*@brief 라인 선상의 점인가
	*@param QBicPoint& oCP 가지고 있는 좌표
	*@param double dEpsilon = QBicConst::Epsilon 
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
    bool IsPolyLineOnPoint ( const QBicPoint& oCP,                double dEpsilon = QBicConst::Epsilon );
	/**
	*@brief 점에 위치한 선 추출
	*@param QBicPoint& oCP 가지고 있는 좌표
	*@param double& dDist
	*@param double dEpsilon = QBicConst::Epsilon 
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
    bool GetPolyLineOnPoint( const QBicPoint& oCP, double& dDist, double dEpsilon = QBicConst::Epsilon );

public:
	/**
	*@brief 정확한 간격
	*@param double dOffset
	*@return oPolygon 반환
	*/
    QBicPolygon RightOffset(double dOffset); 
	/**
	*@brief 
	*@param double dOffset
	*/
    void Offset_Back(double dOffset);
	/**
	*@brief 차이
	*@param double dOffset
	*/
    void Offset(double dOffset);
	/**
	*@brief 역방향 확인
	*/
    void Reverse();
public:
    void GetNearPoint(const QBicPoint& oCP, QBicPoint& oOutPt, double& dDist);
public:
    bool GetVerticalPoint(const QBicPoint& oCP, QBicPoint& oOutPt, double& dDist);

public:
    std::vector<QBicPoint> GetPoints() const {return m_vecPoints;}

friend class QBicPolygon;
};