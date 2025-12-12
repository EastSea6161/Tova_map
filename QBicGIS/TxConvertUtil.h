#pragma once
/**
*@brief 전환 유틸
*@details 전환 유틸
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxConvertUtil
{
public:
    TxConvertUtil() {}
    ~TxConvertUtil(){}
public:
	/**
	*@brief 직사각형 범위 설정
	*@param const CPoint& ptFrom	From 노드
	*@param const CPoint& ptTo		To 노드
	*@return 
	*/
static Gdiplus::RectF ToRectF(const CPoint& ptFrom, const CPoint& ptTo);
/**
	*@brief 직사각형 범위
	*@param const CPoint& ptFrom	From 노드
	*@param const CPoint& ptTo		To 노드
	*@return 
	*/
static CRect ToCRect(const CPoint& ptFrom, const CPoint& ptTo);
};