#include "stdafx.h"
#include "QBicStaticEx.h"
#include <GdiPlus.h>
using namespace Gdiplus;

IMPLEMENT_DYNAMIC(QBicStaticEx, CStatic)

QBicStaticEx::QBicStaticEx()
{
    m_TextColor = RGB(0, 0, 0);
    m_BackColor = TRANS_BACK;    
}

QBicStaticEx::~QBicStaticEx()
{
    
}

BEGIN_MESSAGE_MAP(QBicStaticEx, CStatic)
    //ON_WM_PAINT()
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
// void QBicStaticLabel::OnPaint()
// {
//     CString strLabel;
//     GetWindowText(strLabel);
//     
//     CRect oWindowRect;
//     GetWindowRect(oWindowRect);
//     
//     CPaintDC dc(this); // device context for painting
//     Graphics graphics(dc.GetSafeHdc());
//     graphics.SetSmoothingMode(SmoothingModeAntiAlias);
//     graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
// 
//     /*SolidBrush oSolidBrush(Color(255, 255, 0, 0));
//     Rect oRect(0, 0, oWindowRect.Width(), oWindowRect.Height());
//     graphics.FillRectangle(&oSolidBrush, oRect);*/
// 
//     // Label Ãâ·Â
//     Gdiplus::FontFamily oFontFamily(L"Tahoma");
//     Gdiplus::Font oFont(&oFontFamily, 10, FontStyleBold, UnitPixel);
// 
//     SolidBrush oFontBrush(Color(200, 0, 0, 0));
//     StringFormat stringFormat;
//     stringFormat.SetAlignment(StringAlignmentNear);
//     stringFormat.SetLineAlignment(StringAlignmentCenter);
// 
//     PointF pointF((Gdiplus::REAL)0, (Gdiplus::REAL)(oWindowRect.Height()*0.5));
//     graphics.DrawString(strLabel, (INT)wcslen(strLabel), &oFont, pointF, &stringFormat, &oFontBrush);
// }

HBRUSH QBicStaticEx::CtlColor(CDC* pDC, UINT nCtlColor)
{
    m_Brush.DeleteObject();

    if (m_BackColor == TRANS_BACK) {
        m_Brush.CreateStockObject(HOLLOW_BRUSH);
        pDC->SetBkMode(TRANSPARENT);
    }
    else {
        m_Brush.CreateSolidBrush(m_BackColor);
        pDC->SetBkColor(m_BackColor);
    }

    pDC->SetTextColor(m_TextColor);

    return (HBRUSH)m_Brush;
}


void QBicStaticEx::UpdateCtrl()
{
    CWnd* pParent = GetParent();
    CRect rect;

    GetWindowRect(rect);
    pParent->ScreenToClient(rect);
    rect.DeflateRect(2, 2);

    pParent->InvalidateRect(rect, FALSE);    
}

void QBicStaticEx::PreSubclassWindow()
{
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY /*| WS_EX_TOPMOST*/);

    CStatic::PreSubclassWindow();
}
