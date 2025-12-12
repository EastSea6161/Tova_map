#pragma once
#include "IncludeMessageID.h"
#include "TxPoint.h"

/**
*@brief 맵 이벤트
*@details 맵 이벤트
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxMapEvent : public NMHDR
{
public:
    TxMapEvent () {
        hwndFrom = nullptr;
        idFrom   = 0;
        code     = 0;
    }
    ~TxMapEvent() {}
};

/**
*@brief 맵 마우스 이벤트
*@details 맵 마우스 이벤트
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxMapEventMouse : public TxMapEvent
{
public:
    TxMapEventMouse() : TxMapEvent() {
        Flags = 0;
    }
    ~TxMapEventMouse() {}
public:
    CPoint  MousePoint;
    TxPoint MapPoint;
public:
    UINT    Flags;
};

/*
MK_CONTROL - ctrl 키가 눌렸을때
MK_SHIFT -  shift 키가 눌렸을 때
MK_LBUTTON - 마우스 왼쪽 버튼이 눌렸을 때
MK_MBUTTON - 마우스 가운데 버튼이 눌렸을 때
MK_RBUTTON - 마우스 오른쪽 버튼이 눌렸을 때

if(MK_SHIFT & nFlags)
if( nFlags == MK_CONTROL + MK_LBUTTON )
    AfxMessageBox( "컨트롤 키 누름" );
else if( nFlags == MK_SHIFT + MK_LBUTTON )
    AfxMessageBox( "쉬프트 키 누름" );
else
    AfxMessageBox( "마우스 왼쪽 버튼 누름" );
*/

/*  WM_LBUTTONDBLCLK :
    WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, WM_LBUTTONUP
*/