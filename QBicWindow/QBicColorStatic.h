#pragma once
/**
*@brief 고정값 설정
*@details 색상의 고정값
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicColorStatic : public CStatic
{
public:
    QBicColorStatic();
    virtual ~QBicColorStatic();

protected:
    CBrush   m_brBkgnd; 
    COLORREF m_crBkColor; 
    COLORREF m_crTextColor; 
protected:
	/**
	*@brief 컨트롤 색상
	*@details 버튼들의 기본 색상
	*@param CDC* pDC
	*@param UINT nCtlColor
	*@return 
	*/
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

protected:
    DECLARE_MESSAGE_MAP()

public:
	/**
	*@brief 글자 기본 색
	*@param COLORREF clrColor
	*/
    void SetTextColor(COLORREF clrColor); 
	/**
	*@brief 배경 기본 색
	*@param COLORREF clrColor
	*/
    void SetBkColor  (COLORREF clrColor);  
};