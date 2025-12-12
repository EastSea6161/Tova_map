#pragma once
#include "IncludeQBicDataStructure.h"
#include "IQBicGeometry.h"

class QBicPoint;
class QBicPolyline;
class QBicEnvelope;
/**
*@brief 폴리곤
*@details 큐빅웨어 폴리곤
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class QBicPolygon : public IQBicGeometry
{
public:
	QBicPolygon();
	virtual ~QBicPolygon();

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
	*@brief 교차점인지 확인
	*@param QBicPolyline& rOther 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
	bool IsIntersect(QBicPolyline& rOther);
	/**
	*@brief 내부에 있는지 확인
	*@param QBicPolyline& rOther 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
	bool IsContain(QBicPolyline& rOther);
public:
	/**
	*@brief 내부에 있는지 확인(포인트)
	*@param QBicPoint& oPt 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
	bool IsInPoint(QBicPoint& oPt);
	void GetNearPoint( const QBicPoint& oCP, QBicPoint& oOutPt, double& dDist );

public:
	std::vector<QBicPoint> GetPoints() {return m_vecPoints;}

public:
	//! 폴리곤과 라인 Intersect : 리턴(포인트)
	double GetIntersectLineLength(QBicPolyline& _oPolyline);

	//! 폴리곤 외부에 있는 라인(버텍스)과 폴리곤간의 최단 거리의 합
	double GetNearPointAvgDist(QBicPolyline& _oPolyline);
};