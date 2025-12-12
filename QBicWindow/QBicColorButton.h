#pragma once
/**
*@brief 버튼
*@details 색별로 입력 버튼
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicColorButton : public CButton
{
private:
    enum {	
        BUTTON_IN			= 0x01,
        BUTTON_OUT			= 0x02,
        BUTTON_BLACK_BORDER	= 0x04,
    };
public:
    QBicColorButton();
    QBicColorButton(COLORREF text, COLORREF bkgnd);
    QBicColorButton(COLORREF text, COLORREF bkgnd, COLORREF disabled);
    QBicColorButton(COLORREF text, COLORREF bkgnd, COLORREF disabled, COLORREF light, COLORREF highlight, COLORREF shadow, COLORREF darkShadow);
public:

private:
    COLORREF m_clrText;
    COLORREF m_clrBkgnd;
    COLORREF m_clrDisabledBkgnd;
    COLORREF m_clrLight;
    COLORREF m_clrHighlight;
    COLORREF m_clrShadow;
    COLORREF m_clrDarkShadow;
public:
	/**
	*@brief 색상 설정
	*@details 글자, 백그라운드 색상
	*@param COLORREF text 글자색
	*@param COLORREF bkgnd 백그라운드 색
	*/
    void SetColor(COLORREF text, COLORREF bkgnd);
	/**
	*@brief 색상 설정
	*@details 글자, 백그라운드 색상
	*@param COLORREF text 글자색
	*@param COLORREF bkgnd 백그라운드 색
	*@param COLORREF disabled 사용금지 색
	*/
    void SetColor(COLORREF text, COLORREF bkgnd, COLORREF disabled);
	/**
	*@brief 색상 설정
	*@details 글자, 백그라운드 색상
	*@param COLORREF text 글자색
	*@param COLORREF bkgnd 백그라운드 색
	*@param COLORREF disabled 사용금지 색
	*@param COLORREF light 밝은 색
	*@param COLORREF highlight 특정 색
	*@param COLORREF shadow 그림자 색
	*@param COLORREF darkShadow 어두운 그림자 색
	*/
    void SetColor(COLORREF text, COLORREF bkgnd, COLORREF disabled, COLORREF light, COLORREF highlight, COLORREF shadow, COLORREF darkShadow);
	/**
	*@brief 색상 설정
	*@details 윈도우 기본 색상
	*/
    void SetColorToWindowsDefault();

public:
	/**
	*@brief 글자 색 추출
	*@return 글자색 반환
	*/
    COLORREF GetTextColor() {
        return m_clrText;
    }
private:
	/**
	*@brief 틀 그리기
	*@param CDC *pDC
	*@param CRect rc
	*@param int state 틀 구별 번호
	*/
    void DrawFrame(CDC *pDC, CRect rc, int state);
		/**
	*@brief 색상 채우기
	*@param CDC *pDC
	*@param CRect rc
	*@param COLORREF color 색상
	*/
    void DrawFilledRect(CDC *pDC, CRect rc, COLORREF color);
	/**
	*@brief 선 그리기
	*@param CDC *pDC
	*@param long sx 시작점 x
	*@param long sy 시작점 y
	*@param long ex 끝점 x
	*@param long ey 끝점 y
	*@param COLORREF color 색상
	*/
    void DrawLine(CDC *pDC, long sx, long sy, long ex, long ey, COLORREF color);
		/**
	*@brief 버튼에 글자 넣기
	*@param CDC *pDC
	*@param CRect rc
	*@param CString strCaption 입력할 글자
	*@param COLORREF textcolor 글자 색상
	*/
    void DrawButtonText(CDC *pDC, CRect rc, CString strCaption, COLORREF textcolor);
public:
	/**
	*@brief 아이템 만들기
	*@param LPDRAWITEMSTRUCT lpDrawItemStruct
	*/
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
    virtual ~QBicColorButton();
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};
