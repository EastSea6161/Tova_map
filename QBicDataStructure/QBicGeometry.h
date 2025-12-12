#pragma once
#include "IncludeQBicDataStructure.h"
#include "QBicVector.h"
#include "QBicPoint.h"
#include "QBicPolyline.h"
#include "QBicPolygon.h"
#include "QBicConst.h"
/**
*@brief 기하학
*@details 큐빅웨어 기하학
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class QBicGeometry
{
public:
	QBicGeometry(void);
	~QBicGeometry(void);
public:
	/**
	*@brief 거리 간격
	*@details (X1, Y1) ~ (X2, Y2) 거리
	*@param  double dX1 첫번째 X의 좌표
	*@param  double dY1 첫번째 Y의 좌표
	*@param  double dX2 첫번째 X의 좌표
	*@param  double dY2 첫번째 Y의 좌표
	*@param  double dOffset 
	*@return 간격 반환
	*/
	static QBicPoint GetLineOffsetPoint(double dX1, double dY1, double dX2, double dY2, double dLength=0.0, double dOffset=1.0);
	/**
	*@brief 거리 간격
	*@details fromtT ~ toPt 거리
	*@param const QBicPoint& fromPt
	*@param const QBicPoint& toPt
	*@param double dLength
	*@param double dOffset
	*@return 간격 반환
	*/
	static QBicPoint GetLineOffsetPoint(const QBicPoint& fromPt, const QBicPoint& toPt, double dLength=0.0, double dOffset=1.0);
public:
	/**
	*@brief 거리 간격
	*@details fromtT ~ toPt 거리
	*@param const QBicPoint& fromPt
	*@param const QBicPoint& toPt
	*@param double dOffset 
	*@return 간격 반환
	*/
	static std::vector<QBicPoint> GetLineOffsetPoints(const QBicPoint& fromPt, const QBicPoint& toPt, double dOffset=1.0);
public:
	static std::vector<QBicPoint> GetCWCircleLinePoint (const QBicPoint& centerPt, const QBicPoint& firstPt, double dCWRAngle, int nPointCount = 360);
	static std::vector<QBicPoint> GetUCWCircleLinePoint(const QBicPoint& centerPt, const QBicPoint& firstPt, double dCWRAngle, int nPointCount = 360);

public:
	/**
	*@brief 길이
	*@param double dX1 x1 좌표
	*@param double dY1 y1 좌표
	*@param double dX2 x2 좌표
	*@param double dY2 y2 좌표
	*@return 길이 반환
	*/
	static double Length(double dX1, double dY1, double dX2, double dY2);
	/**
	*@brief 길이
	*@param const QBicPoint& fromPt from 노드의 좌표
	*@param const QBicPoint& toPt	To 노드의 좌표
	*@return 길이 반환
	*/
	static double Length(const QBicPoint& fromPt, const QBicPoint& toPt);
	/**
	*@brief 각도
	*@param double dX1 x1 좌표
	*@param double dY1 y1 좌표
	*@param double dX2 x2 좌표
	*@param double dY2 y2 좌표
	*@return 두 점의 각도
	*/
	static double AzimuthAngle(double dX1, double dY1, double dX2, double dY2);
		/**
	*@brief 각도
	*@param const QBicPoint& fromPt from 노드의 좌표
	*@param const QBicPoint& toPt	To 노드의 좌표
	*@return AzimuthAngle(double dX1, double dY1, double dX2, double dY2) 반환
	*/
	static double AzimuthAngle(const QBicPoint& fromPt, const QBicPoint& toPt);
public:
	/**
	*@brief 라인 선상의 점인가
	*@param const QBicPoint& oA1
	*@param const QBicPoint& oA2
	*@param const QBicPoint& oCP
	*@param double dRadius = QBicConst::Epsilon
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
	static bool   IsLineOnPoint(const QBicPoint& oA1, const QBicPoint& oA2, const QBicPoint& oCP, double dRadius = QBicConst::Epsilon);
public:
	/**
	*@brief 두 직선의 교점
	*@details 9(평행), 0(교차), 1(두직선 모두 진행방향에서 교차), -1(두직선 모두 역방향에서 교차), 2
	*@param const QBicPoint& oAP1
	*@param const QBicPoint& oAP2
	*@param const QBicPoint& oBP1
	*@param const QBicPoint& oBP2
	*@param QBicPoint& oIP
	*@return 상황에 따른 반환하는 값이 다름
	*/
	static int IntersectPoint(const QBicPoint& oAP1, const QBicPoint& oAP2, const QBicPoint& oBP1, const QBicPoint& oBP2, QBicPoint& oIP);
	/**
	*@brief 두 직선의 교차 여부
	*@param const QBicPoint& oAP1
	*@param const QBicPoint& oAP2
	*@param const QBicPoint& oBP1
	*@param const QBicPoint& oBP2
	*@param QBicPoint& oIP
	*@param bool bExtendOption
	*@return 상황에 따른 반환하는 값이 다름
	*/
	static bool   GetIntersectPoint(const QBicPoint& oA1, const QBicPoint& oA2, const QBicPoint& oB1, const QBicPoint& oB2, QBicPoint& oIP, bool bExtendOption = true);
	/**
	*@brief 가장 가까운 직선위의 점
	*@details 직선, 한점이 주어진 경우 가장 가까운 직선위의 점
	*@param const QBicPoint& oAP1
	*@param const QBicPoint& oAP2
	*@param QBicPoint& oCP
	*@param QBicPoint& oIP
	*@param double &dDistanceOnLine
	*@param double &dDistanceShortestLine
	*/
	static void   GetShortestPoint (const QBicPoint& oA1, const QBicPoint& oA2, const QBicPoint& oCP, QBicPoint& oIP, double &dDistanceOnLine, double &dDistanceShortestLine);
	/**
	*@brief 가장 가까운 직선위의 점
	*@details 직선, 한점이 주어진 경우 직선의 연장선과 교차하는 점 
	*@param const QBicPoint& oAP1
	*@param const QBicPoint& oAP2
	*@param QBicPoint& oCP
	*@param QBicPoint& oIP
	*@param double &dDistanceOnLine
	*@param double &dDistanceShortestLine
	*@return (-1:역방향교차, 0:직선위의점, 1:직선진행방향)
	*/
	static int    GetIntersectPoint(const QBicPoint& oA1, const QBicPoint& oA2, const QBicPoint& oCP, QBicPoint& oIP, double &dDistanceOnLine, double &dDistanceShortestLine) ;
};

