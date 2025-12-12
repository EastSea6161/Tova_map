#pragma once
#include "TxPoint.h"
#include "TxPolygon.h"
/**
*@brief Oper 공간
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxSpatialOper
{
public:
	/**
	*@brief 버퍼
	*@details 주어진 포인트에 대한 버퍼
	*@param TxPoint oPt		포인트
	*@param double dBuffer	버퍼
	*@return 퍼버된 값 반환
	*/
    static TxPolygonPtr Buffer  (TxPoint oPt, double dBuffer) {
        TxPointPtr spPt = TxPointPtr(new TxPoint(oPt));
        return Buffer(spPt, dBuffer);
    }
	/**
	*@brief 버퍼
	*@details 주어진 포인트에 대한 버퍼
	*@param TxPointPtr spPt		포인트
	*@param double dBuffer		버퍼
	*@return 퍼버된 값 반환
	*/
    static TxPolygonPtr Buffer  (TxPointPtr spPt,  double dBuffer);
	/**
	*@brief 폴리곤 생성작업
	*@param TxPointPtr spPt1	첫번째 포인트
	*@param TxPointPtr spPt2	두번째 포인트
	*@return 폴리곤으로 작업된 것 반환
	*/
    static TxPolygonPtr RectPoly(TxPointPtr spPt1, TxPointPtr spPt2);
public:
	/**
	*@brief 회전시 좌표
	*@details 주어진 oPt1-oPt2 선을 oP1을 중심으로 회전했을 때의 좌표 리스트
	*@param TxPoint& oPt1	라인 1
	*@param TxPoint& oPt2	라인 2
	*@param double dDegree	회전 도
	*@return	좌표 반환
	*/
    static std::vector<TxPoint> LineRotate(TxPoint& oPt1, TxPoint& oPt2, double dDegree, bool bClockwise = true);
public:
	/**
	*@brief 폴리곤
	*@details Union 폴리곤
	*@param TxPolygonPtr spRed	
	*@param TxPolygonPtr spBlue	
	*@return 
	*/
    static std::vector<TxPolygonPtr> Union(TxPolygonPtr spRed, TxPolygonPtr spBlue);
};