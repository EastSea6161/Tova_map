#include "stdafx.h"
#include "QBicColorButton.h"

namespace clr
{
    const COLORREF CLR_BTN_WHITE  = RGB(255, 255, 255);
    const COLORREF CLR_BTN_BLACK  = RGB(0, 0, 0);
    const COLORREF CLR_BTN_DGREY  = RGB(128, 128, 128);
    const COLORREF CLR_BTN_GREY   = RGB(192, 192, 192);
    const COLORREF CLR_BTN_LLGREY = RGB(223, 223, 223);
}

QBicColorButton::QBicColorButton()
{
    SetColorToWindowsDefault();
}

QBicColorButton::QBicColorButton(COLORREF text, COLORREF bkgnd)
{
    m_clrText			= text;
    m_clrBkgnd			= bkgnd; 
    m_clrDisabledBkgnd	= GetSysColor(COLOR_BTNFACE);
    m_clrLight			= GetSysColor(COLOR_3DLIGHT);
    m_clrHighlight		= GetSysColor(COLOR_BTNHIGHLIGHT);
    m_clrShadow			= GetSysColor(COLOR_BTNSHADOW);
    m_clrDarkShadow		= GetSysColor(COLOR_3DDKSHADOW);	
}

QBicColorButton::QBicColorButton(COLORREF text, COLORREF bkgnd, COLORREF disabled)
{
    m_clrText			= text;
    m_clrBkgnd			= bkgnd; 
    m_clrDisabledBkgnd	= disabled;
    m_clrLight			= GetSysColor(COLOR_3DLIGHT);
    m_clrHighlight		= GetSysColor(COLOR_BTNHIGHLIGHT);
    m_clrShadow			= GetSysColor(COLOR_BTNSHADOW);
    m_clrDarkShadow		= GetSysColor(COLOR_3DDKSHADOW);	
}

QBicColorButton::QBicColorButton(COLORREF text, COLORREF bkgnd, COLORREF disabled, COLORREF light, COLORREF highlight, COLORREF shadow, COLORREF darkShadow)
{
    m_clrText			= text;
    m_clrBkgnd			= bkgnd; 
    m_clrDisabledBkgnd	= disabled;
    m_clrLight			= light;
    m_clrHighlight		= highlight;
    m_clrShadow			= shadow;
    m_clrDarkShadow		= darkShadow;
}

QBicColorButton::~QBicColorButton()
{
}


BEGIN_MESSAGE_MAP(QBicColorButton, CButton)
    ON_WM_CREATE()
END_MESSAGE_MAP()

void QBicColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CDC		*pDC;
    CRect	rcFocus, rcButton, rcText, rcOffsetText;
    UINT	state;

    pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
    state = lpDrawItemStruct->itemState;

    rcFocus.CopyRect(&lpDrawItemStruct->rcItem); 
    rcButton.CopyRect(&lpDrawItemStruct->rcItem); 

    rcText = rcButton;
    rcText.OffsetRect(-1, -1);

    rcOffsetText = rcText;
    rcOffsetText.OffsetRect(1, 1);

    // Set the focus rectangle to just past the border decoration
    rcFocus.left   += 4;
    rcFocus.right  -= 4;
    rcFocus.top    += 4;
    rcFocus.bottom -= 4;

    // Retrieve the button's caption
    CString strCaption;
    GetWindowText(strCaption);

    if (state & ODS_DISABLED)
    {
        DrawFilledRect(pDC, rcButton, m_clrDisabledBkgnd);
    }
    else
    {
        DrawFilledRect(pDC, rcButton, m_clrBkgnd);
    }

    if (state & ODS_SELECTED)
    { 
        DrawFrame(pDC, rcButton, BUTTON_IN);
    }
    else
    {
        if ((state & ODS_DEFAULT) || (state & ODS_FOCUS))
        {
            DrawFrame(pDC, rcButton, BUTTON_OUT | BUTTON_BLACK_BORDER);			
        }
        else
        {
            DrawFrame(pDC, rcButton, BUTTON_OUT);
        }
    }

    if (state & ODS_DISABLED)
    {
        DrawButtonText(pDC, rcOffsetText, strCaption, clr::CLR_BTN_WHITE);
        DrawButtonText(pDC, rcText,	strCaption, clr::CLR_BTN_DGREY);
    }
    else
    {
        if (state & ODS_SELECTED)
        {
            DrawButtonText(pDC, rcOffsetText, strCaption, m_clrText);
        }
        else
        {
            DrawButtonText(pDC, rcText, strCaption, m_clrText);
        }
    }

    if (state & ODS_FOCUS)
    {
        DrawFocusRect(lpDrawItemStruct->hDC, (LPRECT)&rcFocus);
    }	
}

void QBicColorButton::DrawFrame(CDC *pDC, CRect rc, int state)
{
    COLORREF color;

    if (state & BUTTON_BLACK_BORDER)
    {
        color = clr::CLR_BTN_BLACK;

        DrawLine(pDC, rc.left, rc.top, rc.right, rc.top,    color); // Across top
        DrawLine(pDC, rc.left, rc.top, rc.left,  rc.bottom, color); // Down left

        DrawLine(pDC, rc.left,      rc.bottom - 1, rc.right,     rc.bottom - 1, color); // Across bottom
        DrawLine(pDC, rc.right - 1, rc.top,        rc.right - 1, rc.bottom,     color); // Down right

        rc.InflateRect(-1, -1);
    }

    if (true && state & BUTTON_OUT)
    {
        color = m_clrShadow;

        DrawLine(pDC, rc.left, rc.top, rc.right, rc.top,    color); // Across top
        DrawLine(pDC, rc.left, rc.top, rc.left,  rc.bottom, color); // Down left
        DrawLine(pDC, rc.left,      rc.bottom - 1, rc.right,     rc.bottom - 1, color); // Across bottom
        DrawLine(pDC, rc.right - 1, rc.top,        rc.right - 1, rc.bottom,     color); // Down right

        /*
        color = m_Highlight;

        DrawLine(pDC, rc.left, rc.top, rc.right, rc.top,    color); // Across top
        DrawLine(pDC, rc.left, rc.top, rc.left,  rc.bottom, color); // Down left

        color = m_Light; //m_DarkShadow;

        DrawLine(pDC, rc.left,      rc.bottom - 1, rc.right,     rc.bottom - 1, color); // Across bottom
        DrawLine(pDC, rc.right - 1, rc.top,        rc.right - 1, rc.bottom,     color); // Down right

        rc.InflateRect(-1, -1);
        color = m_Light;

        DrawLine(pDC, rc.left, rc.top, rc.right, rc.top,    color); // Across top
        DrawLine(pDC, rc.left, rc.top, rc.left,  rc.bottom, color); // Down left
        DrawLine(pDC, rc.left,      rc.bottom - 1, rc.right,     rc.bottom - 1, color); // Across bottom
        DrawLine(pDC, rc.right - 1, rc.top,        rc.right - 1, rc.bottom,     color); // Down right

        
        color = m_Shadow;
        DrawLine(pDC, rc.left,      rc.bottom - 1, rc.right,     rc.bottom - 1, color); // Across bottom
        DrawLine(pDC, rc.right - 1, rc.top,        rc.right - 1, rc.bottom,     color); // Down right
        */
    }

    if (true && state & BUTTON_IN)
    {
        color = m_clrShadow;

        DrawLine(pDC, rc.left, rc.top, rc.right, rc.top,    color); // Across top
        DrawLine(pDC, rc.left, rc.top, rc.left,  rc.bottom, color); // Down left
        DrawLine(pDC, rc.left,      rc.bottom - 1, rc.right,     rc.bottom - 1, color); // Across bottom
        DrawLine(pDC, rc.right - 1, rc.top,        rc.right - 1, rc.bottom,     color); // Down right
        /*
        color = m_clrDarkShadow;

        DrawLine(pDC, rc.left, rc.top, rc.right, rc.top,    color); // Across top
        DrawLine(pDC, rc.left, rc.top, rc.left,  rc.bottom, color); // Down left
        DrawLine(pDC, rc.left,      rc.bottom - 1, rc.right,     rc.bottom - 1, color); // Across bottom
        DrawLine(pDC, rc.right - 1, rc.top,        rc.right - 1, rc.bottom,     color); // Down right

        rc.InflateRect(-1, -1);

        color = m_clrShadow;

        DrawLine(pDC, rc.left, rc.top, rc.right, rc.top,    color); // Across top
        DrawLine(pDC, rc.left, rc.top, rc.left,  rc.bottom, color); // Down left
        DrawLine(pDC, rc.left,      rc.bottom - 1, rc.right,     rc.bottom - 1, color); // Across bottom
        DrawLine(pDC, rc.right - 1, rc.top,        rc.right - 1, rc.bottom,     color); // Down right
        */
    }
}

void QBicColorButton::DrawFilledRect(CDC *pDC, CRect rc, COLORREF color)
{
    CBrush brSolid;

    brSolid.CreateSolidBrush(color);
    pDC->FillRect(rc, &brSolid);
}

void QBicColorButton::DrawLine(CDC *pDC, long sx, long sy, long ex, long ey, COLORREF color)
{
    CPen newPen;
    CPen *oldPen;

    newPen.CreatePen(PS_SOLID, 1, color);
    oldPen = pDC->SelectObject(&newPen);

    pDC->MoveTo(sx, sy);
    pDC->LineTo(ex, ey);
    pDC->SelectObject(oldPen);

    newPen.DeleteObject();	
}

void QBicColorButton::DrawButtonText(CDC *pDC, CRect rc, CString strCaption, COLORREF textcolor)
{
    DWORD uStyle = GetWindowLong(this->m_hWnd,GWL_STYLE);

    CArray<CString, CString> arLines;

    if((uStyle & BS_MULTILINE) == BS_MULTILINE)
    {
        int nIndex = 0;
        while(nIndex != -1)
        {
            nIndex = strCaption.Find('\n');
            if(nIndex>-1)
            {
                CString line = strCaption.Left(nIndex);
                arLines.Add(line);
                strCaption.Delete(0,nIndex+1);
            }
            else
                arLines.Add(strCaption);
        }
    }
    else
    {
        arLines.Add(strCaption);
    }

    CSize sizeText = pDC->GetOutputTextExtent( strCaption );

    COLORREF oldColour;

    oldColour = pDC->SetTextColor(textcolor);
    pDC->SetBkMode(TRANSPARENT);

    int nStartPos = (int)((rc.Height() - arLines.GetSize()*sizeText.cy)/2-1);
    if((uStyle & BS_TOP) == BS_TOP)
        nStartPos = rc.top+2;
    if((uStyle & BS_BOTTOM) == BS_BOTTOM)
        nStartPos = (int)(rc.bottom- arLines.GetSize()*sizeText.cy-2);
    if((uStyle & BS_VCENTER) == BS_VCENTER)
        nStartPos = (int)((rc.Height() - arLines.GetSize()*sizeText.cy)/2-1);

    UINT uDrawStyles = 0;
    if((uStyle & BS_CENTER) == BS_CENTER)
        uDrawStyles |= DT_CENTER;
    else
    {
        if((uStyle & BS_LEFT) == BS_LEFT)
            uDrawStyles |= DT_LEFT;
        else
            if((uStyle & BS_RIGHT) == BS_RIGHT)
                uDrawStyles |= DT_RIGHT;
            else
                if(uDrawStyles == 0)
                    uDrawStyles = DT_CENTER|DT_VCENTER | DT_SINGLELINE;
    }

    for(int i=0; i<arLines.GetSize(); i++)
    {
        CRect textrc = rc;
        textrc.DeflateRect(3,0,3,0);
        textrc.top = nStartPos + sizeText.cy*i;
        textrc.bottom = nStartPos + sizeText.cy*(i+1);
        CString line = arLines.GetAt(i);
        pDC->DrawText(line, line.GetLength(), textrc, uDrawStyles);
    }

    pDC->SetTextColor(oldColour);
}

void QBicColorButton::SetColor(COLORREF text, COLORREF bkgnd)
{
    m_clrText  = text;
    m_clrBkgnd = bkgnd; 

    if(m_hWnd != NULL)
        Invalidate();
}

void QBicColorButton::SetColor(COLORREF text, COLORREF bkgnd, COLORREF disabled)
{
    m_clrText	    	= text;
    m_clrBkgnd			= bkgnd; 
    m_clrDisabledBkgnd	= disabled;

    if(m_hWnd != NULL)
        Invalidate();
}

void QBicColorButton::SetColor(COLORREF text, COLORREF bkgnd, COLORREF disabled, COLORREF light, COLORREF highlight, COLORREF shadow, COLORREF darkShadow)
{
    m_clrText			= text;
    m_clrBkgnd			= bkgnd; 
    m_clrDisabledBkgnd	= disabled;
    m_clrLight			= light;
    m_clrHighlight		= highlight;
    m_clrShadow			= shadow;
    m_clrDarkShadow		= darkShadow;

    if(m_hWnd != NULL)
        Invalidate();
}

void QBicColorButton::SetColorToWindowsDefault()
{
    m_clrText			= GetSysColor(COLOR_BTNTEXT);
    m_clrBkgnd			= GetSysColor(COLOR_BTNFACE); 
    m_clrDisabledBkgnd	= GetSysColor(COLOR_BTNFACE);
    m_clrLight			= GetSysColor(COLOR_3DLIGHT);
    m_clrHighlight		= GetSysColor(COLOR_BTNHIGHLIGHT);
    m_clrShadow			= GetSysColor(COLOR_BTNSHADOW);
    m_clrDarkShadow		= GetSysColor(COLOR_3DDKSHADOW);	
}

int QBicColorButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    lpCreateStruct->dwExStyle |= BS_OWNERDRAW;

    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}
