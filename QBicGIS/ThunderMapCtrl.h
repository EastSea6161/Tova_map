#pragma once

#define THUNDER_MAPCTRL_CLASSNAME _T("ThunderMapCtrl")  // Window class name

#include "ThunderMap.h"
/**
*@brief 맵 컨트롤
*@details 맵 컨트롤
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ThunderMapCtrl : public CWnd
{
    DECLARE_DYNAMIC(ThunderMapCtrl);

public:
    ThunderMapCtrl();
    virtual ~ThunderMapCtrl();

protected:
    DECLARE_MESSAGE_MAP()
protected:
	/**
	*@brief 윈도우 클래스 생성?
	*@return 정상적으로 생성이 되면 ture / 생성실패시 false 반환
	*/
    BOOL RegisterWindowClass();
public:
	/**
	*@brief 윈도우 생성
	*@param CWnd* pParentWnd
	*@param const RECT& rect
	*@param UINT nID
	*@param DWORD dwStyle
	*@return 윈도우 창 
	*/
    virtual BOOL Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle = WS_VISIBLE);
protected:
	/**
	*@brief 윈도우 새창
	*/
    virtual void PreSubclassWindow();
protected:
	/**
	*@brief 윈도우창 생성
	*@param LPCREATESTRUCT lpCreateStruct
	*@return 
	*/
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	/**
	*@brief 그리기 함수 생성
	*/
    afx_msg void OnPaint();
	/**
	*@brief 백그라운드 제거
	*@param CDC* pDC
	*@return 백그라운드 FALSE 반환
	*/
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	/**
	*@brief 사이즈 크기 조절
	*@param UINT nType	타입
	*@param int cx		X값
	*@param int cy		Y값
	*/
    afx_msg void OnSize(UINT nType, int cx, int cy);
protected:        
	/**
	*@brief 마우스 왼쪽버튼 누름상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/**
	*@brief 마우스 왼쪽버튼 뗀 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    afx_msg void OnLButtonUp  (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 오른쪽버튼 누름상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	/**
	*@brief 마우스 오른쪽버튼 뗀 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    afx_msg void OnRButtonUp  (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 왼쪽버튼 더블 클릭
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/**
	*@brief 마우스 휠버튼 내림 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	/**
	*@brief 마우스 휠버튼 올림 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    afx_msg void OnMButtonUp  (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 움직일때
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    afx_msg void OnMouseMove  (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 휠 움직일 때
	*@param UNIT nFlags
	*@param short zDelta 
	*@param CPoint pt 
	*/
    afx_msg BOOL OnMouseWheel (UINT nFlags, short zDelta, CPoint pt);
protected:
	/**
	*@brief 커서 셋팅
	*@param CWnd* pWnd		커서
	*@param UINT nHitTest	?
	*@param UINT message	메세지
	*@return 커서 셋팅 정보
	*/
    afx_msg BOOL OnSetCursor (CWnd* pWnd,  UINT nHitTest, UINT message);
private:
    bool   m_bTimer;
    bool   m_bSendOverMsg;
    CPoint m_preMouseOverPoint; //★ Tooltip 계산용도
private:
    CPoint m_preMousePoint;
protected:    
    UINT_PTR TimerID;
	/**
	*@brief 타이머 설정
	*@param UINT_PTR nIDEvent 이벤트 ID
	*/
    afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	/**
	*@brief 메세지 알림
	*@param UINT nMessage	전달 메세지
	*@param NMHDR* pNMHDR	
	*@return 정상적으로 실행 시 메세지 전달 / 실패시 0 반환
	*/
    LRESULT SendNotifyMessage(UINT nMessage, NMHDR* pNMHDR = NULL) const;
private:
	/**
	*@brief 맵 레이어 그리기의 끝???
	*@param WPARAM wParam
	*@param LPARAM lParam
	*@return 메세지 알림
	*/
    afx_msg LRESULT OnMapLayerDrawEnd(WPARAM wParam, LPARAM lParam);
private:
    ThunderMapPtr m_spThunderMap;

//*************************************************************************
public:
	/**
	*@brief 맵 추출
	*@return 맵 정보 추출
	*/
    ThunderMapPtr GetThunderMap() {
        return m_spThunderMap;
    }
//*************************************************************************
};