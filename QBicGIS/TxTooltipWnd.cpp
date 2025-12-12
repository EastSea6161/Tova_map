#include "stdafx.h"
#include "TxTooltipWnd.h"

IMPLEMENT_DYNAMIC(TxTooltipWnd, CWnd)
BEGIN_MESSAGE_MAP(TxTooltipWnd, CWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()

TxTooltipWnd::TxTooltipWnd()
{
    m_strTooltip = _T("Text"); {
        m_mouseX = 0;
        m_mouseY = 0;
    }
}

TxTooltipWnd::~TxTooltipWnd()
{
}

BOOL TxTooltipWnd::Create( CWnd* pParentWnd )
{
    m_pParentWnd = pParentWnd;
    m_font.CreatePointFont(90, _T("³ª´®°íµñÄÚµù"));	
    m_pen.CreatePen(PS_DASH,2,RGB(100,100,100));
    m_brush.CreateSolidBrush(RGB(252,252,240));

    CRect rectInitialSize(0,0,300,30);
    BOOL bR = CreateEx(NULL, NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS , rectInitialSize, pParentWnd, NULL, NULL);
    return bR;
}

void TxTooltipWnd::ShowTooltip(int x, int y, CString strTooltip )
{
    m_strTooltip = strTooltip;
    m_mouseX = x;
    m_mouseY = y;

    if (true/*IsWindowVisible() == TRUE*/) {
        //CDC* pDC = GetDC();
        //CSize size = pDC->GetTextExtent(m_strTooltip);
        //pDC->LPtoDP(&size);
        //ReleaseDC(pDC);
        
        SetWindowPos(&m_pParentWnd->wndTopMost, m_mouseX, m_mouseY + 20, 500, 50, SWP_SHOWWINDOW|SWP_NOOWNERZORDER|SWP_NOACTIVATE|SWP_NOZORDER);       
        //SetWindowPos(&wndTopMost, m_mouseX, m_mouseY + 20, size.cx + 8, size.cy + 4, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);   
        Invalidate();
        
    }
}

void TxTooltipWnd::TopWindow()
{
    SetWindowPos(&m_pParentWnd->wndTopMost, m_mouseX, m_mouseY + 20, 500, 50, SWP_SHOWWINDOW|SWP_NOOWNERZORDER|SWP_NOACTIVATE|SWP_NOZORDER); 
}

void TxTooltipWnd::OnPaint()
{
    if (m_strTooltip.GetLength() < 1)
        return;

    CPaintDC dc(this); 
        
    CPen*   pOldPen   = dc.SelectObject(&m_pen);
    CFont*  pOldFont  = dc.SelectObject(&m_font);
    CBrush* pOldBrush = dc.SelectObject(&m_brush);
    dc.SetBkMode(TRANSPARENT);
    
    CSize size = dc.GetTextExtent(m_strTooltip);
    dc.LPtoDP(&size);

    //MoveWindow(m_mouseX, m_mouseY + 20, size.cx + 8, size.cy + 4); 
    //::SetWindowPos(m_pParentWnd->GetSafeHwnd(), HWND_TOPMOST, m_mouseX, m_mouseY + 20, size.cx + 8, size.cy + 4, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
    //SetWindowPos(&m_pParentWnd->wndTopMost, m_mouseX, m_mouseY + 20, size.cx + 8, size.cy + 4, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
    CRect rectToolTip(0,0,200,20); {
        GetClientRect(&rectToolTip);
    }

    if (false){
        CRect rectWnd = rectToolTip;
        m_pParentWnd->ClientToScreen(rectWnd); //Convert from client to screen coordinates
        CPoint m_ptToolTipLeft = rectWnd.TopLeft();
        SetWindowPos(&wndTop,m_ptToolTipLeft.x+1, m_ptToolTipLeft.y+1, rectWnd.Width(), rectWnd.Height(),SWP_SHOWWINDOW|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
        //SetWindowPos(m_pParentWnd,0, 0, 400, 50,SWP_SHOWWINDOW|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
    }       
    
    int nX = 0;
    int nY = 0;
    
    dc.Rectangle(&rectToolTip);
    dc.SetTextColor(RGB(86,34,0));
    dc.TextOut(nX, nY, m_strTooltip);

    dc.SelectObject(pOldFont);
    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}

