#pragma once
#include <cmath>
#include "TxConst.h"
#include "TxPoint.h"
#include "TxPolyline.h"
/**
*@brief 공식
*@details 공식
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxMath
{
public:
    TxMath(void);
    ~TxMath(void);

public:
    static double Mod(double x, double y);
public:
	/**
	*@brief 도 -> 라디안
	*@param double dDegree	도
	*@return 라디안 반환
	*/
    static double Degree2Radian(double dDegree);
	/**
	*@brief 라디안 -> 도
	*@param double dRadian	라디안
	*@param bool bPlus		true = 플러스
	*@return 도 반환
	*/
    static double Radian2Degree(double dRadian, bool bPlus = true);

public:
	/**
	*@brief 길이(점<->점)
	*@param double x1 좌표 1 x
	*@param double y1 좌표 1 y
	*@param double x2 좌표 2 x
	*@param double y2 좌표 2 y
	*@return 길이 값
	*/
    static double Dist(double x1, double y1, double x2, double y2);
	/**
	*@brief 길이(버텍스<->버텍스)
	*@param const TxPoint& oPt1	버텍스 1
	*@param const TxPoint& oPt2 버텍스 2
	*@return 길이 값
	*/
    static double Dist(const TxPoint& oPt1, const TxPoint& oPt2);
public:
	/**
	*@brief 위치와 라인의 각도
	*@details 라인의 시점에서 요청거리에 해당하는 점을 Offset한 위치와 라인의 각도 정보 전달
	*@param const TxPoint& oPt1	버텍스 1
	*@param const TxPoint& oPt2	버텍스 2
	*@param double dDist		요청거리
	*@param TxPoint& oOnPt		해당 포인트
	*@param TxPoint& oOffsetPt	해당 오프셋 포인트
	*@param double& dAngle		각도
	*@param double dOffset		오프셋
	*/
    static void LineOnPoint(const TxPoint& oPt1, const TxPoint& oPt2, double dDist, TxPoint& oOnPt);
    static void LineOnPoint(const TxPoint& oPt1, const TxPoint& oPt2, double dDist, TxPoint& oOnPt, TxPoint& oOffsetPt, double& dAngle, double dOffset=0.0);
public:
	/**
	*@brief 라인 오프셋 포인트
	*@param const TxPoint& fromPt
	*@param const TxPoint& toPt
	*@param double dOffset
	*@return 입력된 벡터 반환
	*/
    static std::vector<TxPoint> GetLineOffsetPoints(const TxPoint& fromPt, const TxPoint& toPt, double dOffset=1.0);
public:
	/**
	*@brief 교차 포인트 추출
	*@details 직선(두점)과 한점이 주어진 경우 직선의 연장선과 교차하는 점 (-1:역방향교차, 0:직선위의점, 1:직선진행방향)
	*@param const TxPoint& oA1	시작 점
	*@param const TxPoint& oA2	끝 점
	*@param const TxPoint& oCP	한 점
	*@param TxPoint& oIP		겹치는 점
	*@param double &dDistanceOnLine
	*@return 라인과 교차하는 점 리턴
	*/
    static int  GetIntersectPoint(const TxPoint& oA1, const TxPoint& oA2, const TxPoint& oCP, TxPoint& oIP, double &dDistanceOnLine);
	/**
	*@brief 교차 여부
	*@details 두직선의 교차 여부를 판단, 만약 교차하면 교차점을 리턴, bExtendOption(true:양선을 연장하여 교차 체크)
	*@param const TxPoint& oA1	A시작 점
	*@param const TxPoint& oA2	A끝 점
	*@param const TxPoint& oB1	B시작 점
	*@param const TxPoint& oB2	B끝 점
	*@param TxPoint& oIP		겹치는 점
	*@param bool bExtendOption
	*@return 
	*/
    static bool GetIntersectPoint(const TxPoint& oA1, const TxPoint& oA2, const TxPoint& oB1, const TxPoint& oB2, TxPoint& oIP, bool bExtendOption = true);
	/**
	*@brief 교차 확인
	*@details 두직선의 교점 : 9(평행), 0(교차), 1(두직선 모두 진행방향에서 교차), -1(두직선 모두 역방향에서 교차), 2
	*@param const TxPoint& oAP1	A 시작 점
	*@param const TxPoint& oAP2 A 끝 점
	*@param const TxPoint& oBP1 B 시작 점
	*@param const TxPoint& oBP2 B 끝점
	*@param TxPoint& oIP
	*@return 교점 반환
	*/
    static int IntersectPoint( const TxPoint& oAP1, const TxPoint& oAP2, const TxPoint& oBP1, const TxPoint& oBP2, TxPoint& oIP );

public:
    static double Azimuth(double dX1, double dY1, double dX2, double dY2);
    static double Azimuth(const TxPoint& fromPt,  const TxPoint& toPt);
public:
    static double AzimuthDegree(double dX1, double dY1, double dX2, double dY2);
    static double AzimuthDegree(const TxPoint& fromPt,  const TxPoint& toPt);
};
