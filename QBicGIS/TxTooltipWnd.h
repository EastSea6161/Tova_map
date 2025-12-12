#pragma once
// Backup
/**
*@brief 툴팁
*@details 툴팁
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxTooltipWnd : public CWnd
{
	DECLARE_DYNAMIC(TxTooltipWnd)

public:
	TxTooltipWnd();
	virtual ~TxTooltipWnd();

protected:
	DECLARE_MESSAGE_MAP()
private:
    CWnd* m_pParentWnd;
public:
	/**
	*@brief 툴팁생성
	*@param CWnd* pParentWnd
	*@return 생성된 툴팁 반환
	*/
    BOOL Create(CWnd* pParentWnd);
private:
    int m_mouseX;
    int m_mouseY;
    CString m_strTooltip;
public:
	/**
	*@brief 툴팁 보이기
	*@param int x				마우스 x 좌표
	*@param int y				마우스 y 좌표
	*@param CString strTooltip	툴팁 제목
	*/
    void ShowTooltip(int x, int y, CString strTooltip);
	/**
	*@brief 가장 위에 펼치기 설정
	*/
    void TopWindow();
protected:
	/**
	*@brief 표출하기 
	*/
    afx_msg void OnPaint();
private:
    CBrush m_brush;
    CPen   m_pen;
    CFont  m_font;
};


