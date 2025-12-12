#pragma once

#include "IThunderMapCommand.h"
#include "IThunderMap.h"
/**
*@brief 지도 제어 명령
*@details 지도 제어 명령(줌아웃)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class ThunderMapCommandZoomOut : public IThunderMapCommand
{
public:
    ThunderMapCommandZoomOut(IThunderMap* pThunderMap, Gdiplus::Color crBackground = Gdiplus::Color::White);
    virtual ~ThunderMapCommandZoomOut();

public:
	/**
	*@brief 마우스 왼쪽버튼 누름상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnLButtonDown   (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 왼쪽버튼 뗀 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnLButtonUp     (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 왼쪽버튼 더블 클릭
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnLButtonDblClk (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 움직일때
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnMouseMove     (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 오른쪽버튼 누름상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnRButtonDown   (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 오른쪽버튼 뗀 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnRButtonUp     (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 오른쪽버튼 더블클릭
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnRButtonDblClk (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 휠 버튼 누름상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnMButtonDown   (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 휠 버튼 뗀 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnMButtonUp     (UINT nFlags, CPoint point);
		/**
	*@brief 마우스 휠 움직일 때
	*@param UNIT nFlags
	*@param short zDelta 
	*@param CPoint point 
	*/
    virtual void OnMouseWheel    (UINT nFlags, short zDelta, CPoint point);

public:
	/**
	*@brief 명령어 작동 상태 확인
	*/
    virtual bool IsCommandRun() {return false;}

private:
    void RubberBandDraw(Gdiplus::RectF& oDestRect);
};