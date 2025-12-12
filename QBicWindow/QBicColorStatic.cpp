#include "stdafx.h"
#include "QBicColorStatic.h"

BEGIN_MESSAGE_MAP(QBicColorStatic, CStatic)
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

QBicColorStatic::QBicColorStatic()
{
    m_crBkColor = ::GetSysColor(COLOR_3DFACE); 
    m_crTextColor = RGB(0,0,0);
    m_brBkgnd.CreateSolidBrush(m_crBkColor); 
}

QBicColorStatic::~QBicColorStatic()
{

}

HBRUSH QBicColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    HBRUSH hbr;
    hbr = (HBRUSH)m_brBkgnd; 
    pDC->SetBkColor(m_crBkColor); 
    pDC->SetTextColor(m_crTextColor); 

    if (nCtlColor)       
        nCtlColor += 0;

    return hbr;
}

void QBicColorStatic::SetTextColor( COLORREF clrColor )
{
    m_crTextColor = clrColor; 
    RedrawWindow();
}

void QBicColorStatic::SetBkColor( COLORREF clrColor )
{
    m_crBkColor = clrColor; 
    m_brBkgnd.DeleteObject(); 
    m_brBkgnd.CreateSolidBrush(clrColor); 
    RedrawWindow();
}
