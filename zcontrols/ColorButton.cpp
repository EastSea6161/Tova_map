// ColorButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ColorButton.h"


// ZColorButton

IMPLEMENT_DYNAMIC(ZColorButton, CButton)

ZColorButton::ZColorButton()
{
	m_color = ::GetSysColor(COLOR_WINDOW);
	m_BackBrush.CreateSolidBrush(m_color);
}


ZColorButton::ZColorButton(COLORREF color)
	:	m_color(color),
		m_BackBrush(color)
{

}


ZColorButton::~ZColorButton()
{
}


BEGIN_MESSAGE_MAP(ZColorButton, CButton)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



void ZColorButton::SetButtonColor(COLORREF color)
{
	m_color = color;
	m_BackBrush.CreateSolidBrush(color);
}


// ZColorButton 메시지 처리기입니다.
HBRUSH ZColorButton::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	//return NULL;
	return (HBRUSH)m_BackBrush;
}
