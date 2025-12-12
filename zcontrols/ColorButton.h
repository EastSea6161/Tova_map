#pragma once


// ZColorButton

class AFX_EXT_CLASS ZColorButton : public CButton
{
	DECLARE_DYNAMIC(ZColorButton)

public:
	ZColorButton();
	ZColorButton(COLORREF color);
	virtual ~ZColorButton();


public :
	void SetButtonColor(COLORREF color);


protected : 
	COLORREF m_color;
	CBrush m_BackBrush;


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


