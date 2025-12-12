// ArrowStatic.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ArrowStatic.h"
#include <GdiPlus.h>
using namespace Gdiplus;


// ZArrowStatic

IMPLEMENT_DYNAMIC(ZArrowStatic, CStatic)

ZArrowStatic::ZArrowStatic()
	:	m_nArrowLineWidth(1.0),
        m_crArrowColor(RGB(255, 255, 255)/*RGB(112, 112, 112)*/),
        m_arrowLineDashStype(DashStyleSolid),
		m_nArrowCapWidthSize(8),
        m_nArrowCapHeightSize(8),
        m_isCapFilled(FALSE),
		m_bUseMiddlePosition(false)		
{
	m_emStartPosition = m_emEndPosition = ZEMArrowPositionCC;
}


ZArrowStatic::~ZArrowStatic()
{
}


BEGIN_MESSAGE_MAP(ZArrowStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void ZArrowStatic::SetPosition(ZEMArrowPosition emStartPosition, ZEMArrowPosition emEndPosition)
{
	m_emStartPosition = emStartPosition;
    m_emEndPosition   = emEndPosition;
}


void ZArrowStatic::SetPosition(ZEMArrowPosition emStartPosition, ZEMArrowPosition emMidPosition, ZEMArrowPosition emEndPosition)
{
    m_emStartPosition = emStartPosition;
    m_emEndPosition   = emEndPosition;
    m_emMidPosition   = emMidPosition;

    m_bUseMiddlePosition = true;
}


void ZArrowStatic::UseMiddlePosition(bool bUse)
{
	m_bUseMiddlePosition = bUse;
}


void ZArrowStatic::ArrowLineWidth(float nWidth)
{
	m_nArrowLineWidth = nWidth;
}


void ZArrowStatic::SetArrowCap(float nHeight, float nWidth, BOOL isCapFilled)
{
	m_nArrowCapWidthSize  = nWidth;
    m_nArrowCapHeightSize = nHeight;
    m_isCapFilled = isCapFilled;
}

void ZArrowStatic::DashStype(Gdiplus::DashStyle arrowLineDashStype)
{
    m_arrowLineDashStype = arrowLineDashStype;
}

void ZArrowStatic::ArrowLineColor(COLORREF crColor)
{
	m_crArrowColor = crColor;
}


CPoint ZArrowStatic::getPositionPoint(CRect& rt, ZEMArrowPosition emPosition)
{
	CPoint pt;

	switch(emPosition)
	{
	case ZEMArrowPositionLT : pt = rt.TopLeft(); break;
	case ZEMArrowPositionCT : pt.x = rt.CenterPoint().x; pt.y = rt.top; break;
	case ZEMArrowPositionRT : pt.x = rt.right; pt.y = rt.top; break;
	case ZEMArrowPositionLC : pt.x = rt.left; pt.y = rt.CenterPoint().y; break;
	case ZEMArrowPositionCC : pt = rt.CenterPoint(); break;
	case ZEMArrowPositionRC : pt.x = rt.right; pt.y = rt.CenterPoint().y; break;
	case ZEMArrowPositionLB : pt.x = rt.left; pt.y = rt.bottom; break;
	case ZEMArrowPositionCB : pt.x = rt.CenterPoint().x; pt.y = rt.bottom; break;
	case ZEMArrowPositionRB : pt = rt.BottomRight(); break;
	default : pt = rt.CenterPoint(); break;
	}

	return pt;
}


// ZArrowStatic 메시지 처리기입니다.
void ZArrowStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    int nGap = 3;

	CRect clientRect;
	    GetClientRect(&clientRect);
        clientRect.left = clientRect.left + nGap;
        clientRect.top  = clientRect.top  + nGap;
        clientRect.right  = clientRect.right - nGap;
        clientRect.bottom = clientRect.bottom -nGap;

	CPoint ptStart = getPositionPoint(clientRect, m_emStartPosition);
	CPoint ptEnd   = getPositionPoint(clientRect, m_emEndPosition);

	Graphics gp(dc.GetSafeHdc());
	gp.SetSmoothingMode(SmoothingModeAntiAlias);

	Color lineColor;
	lineColor.SetFromCOLORREF(m_crArrowColor);

	Pen pen(lineColor, m_nArrowLineWidth);
	pen.SetLineJoin(LineJoinRound);
    pen.SetDashStyle(m_arrowLineDashStype);

    AdjustableArrowCap myArrow(m_nArrowCapHeightSize, m_nArrowCapWidthSize, m_isCapFilled);

	if(false == m_bUseMiddlePosition)
	{	
        pen.SetCustomEndCap(&myArrow);
		gp.DrawLine(&pen, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}
	else
	{
		CPoint ptCenter = getPositionPoint(clientRect, m_emMidPosition);
        
		gp.DrawLine(&pen, ptStart.x, ptStart.y, ptCenter.x, ptCenter.y);		
        pen.SetCustomEndCap(&myArrow);
		gp.DrawLine(&pen, ptCenter.x, ptCenter.y, ptEnd.x, ptEnd.y);
	}

	//CPen pen(PS_SOLID, m_nArrowLineWidth, m_crArrowColor);
	//CPen* pOldPen = dc.SelectObject(&pen);

	//int x1, y1, x2, y2;
	//

	//if(true == m_bUseCenterPosition)
	//{
	//	CPoint ptCenter = rect.CenterPoint();
	//	dc.MoveTo(ptStart);
	//	dc.LineTo(ptCenter);
	//	dc.LineTo(ptEnd);
	//	x1 = ptCenter.x;
	//	y1 = ptCenter.y;
	//}
	//else
	//{
	//	dc.MoveTo(ptStart);
	//	dc.LineTo(ptEnd);
	//	x1 = ptStart.x;
	//	y1 = ptStart.y;
	//}

	//x2 = ptEnd.x;
	//y2 = ptEnd.y;

	//// draw Arrow
	//double dWidth, dHeight, theta, cx, cy, sin_th, cos_th;
	//dWidth = m_nArrowCapSize;
	//dHeight = m_nArrowCapSize;

	//theta = atan2((double)(y2 - y1), (double)(x2 - x1));

	//cx = x2 - dHeight * cos(theta);
	//cy = y2 - dHeight * sin(theta);

	//sin_th = dWidth * sin(theta) / 2.0;
	//cos_th = dWidth * cos(theta) / 2.0; 
	//
	//dc.MoveTo(int(cx - sin_th + 0.5), int(cy + cos_th + 0.5));
	//dc.LineTo(x2, y2);

	//dc.MoveTo(int(cx + sin_th + 0.5), int(cy - cos_th + 0.5));
	//dc.LineTo(x2, y2);

	//dc.SelectObject(pOldPen);
	//pen.DeleteObject();

}

