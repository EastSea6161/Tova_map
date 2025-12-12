#pragma once
#include <cmath>
#include "QBicGeometry.h"
/**
*@brief 공식
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class QBicMath
{
public:
    QBicMath(void);
    ~QBicMath(void);

public:
	/**
	*@brief 모드
	*@param double x X값
	*@param double y Y값
	*@return 결과값 반환
	*/
    static double Mod(double x, double y);
public:
	/**
	*@brief 도 -> 라디안 변경
	*@param double dDegree 도
	*@return 변경된 값 반환
	*/
    static double Degree2Radian(double dDegree);
	/**
	*@brief 라디안 -> 도 변경
	*@param double dRadian 라디안
	*@param bool bPlus
	*@return 변경된 값 반환
	*/
    static double Radian2Degree(double dRadian, bool bPlus = true);

public:
	/**
	*@brief 길이 측정
	*@param double x1 x1좌표
	*@param double y1 y1좌표
	*@param double x2 x2좌표
	*@param double y2 y2좌표
	*@return 길이값 반환
	*/
    static double Dist(double x1, double y1, double x2, double y2);
	/**
	*@brief 길이 측정
	*@param QBicPoint oPt1 첫번째 포인트 좌표
	*@param QBicPoint oPt2 두번째 포인트 좌표
	*@return 길이값 반환
	*/
    static double Dist(QBicPoint oPt1, QBicPoint oPt2);
};

