#include "stdafx.h"
#include "TxLineDecoSymbolArrow.h"

//★ ----------------------------------------------------------------------------------------------------------
TxLineDecoSymbolArrow::TxLineDecoSymbolArrow(double dDrawMaxScale, double dPosRate, Gdiplus::Color clrArrow) 
    :ITxLineDecoSymbol(dDrawMaxScale, dPosRate)
{    
    m_dX = 0.f;
    m_dY = 0.f;

    m_dWidth  = 6.f;
    m_dHeight = 12.f;

    m_nPosFlag = 0;

    m_pSolidBrush = new Gdiplus::SolidBrush(clrArrow);
    m_pLinePen    = new Gdiplus::Pen(clrArrow);

    m_clrArrow = clrArrow;
    m_bFilled  = true;

    RecalcShape();
}

TxLineDecoSymbolArrow::~TxLineDecoSymbolArrow()
{
    delete m_pSolidBrush;
    delete m_pLinePen;
}

void TxLineDecoSymbolArrow::Draw(Gdiplus::Graphics& xGraphics )
{
    if (m_bFilled) {
        xGraphics.FillPolygon(m_pSolidBrush, m_pArrowPts, 4);
    }
    else {
        xGraphics.DrawPolygon(m_pLinePen, m_pArrowPts, 4);
    }
}

double TxLineDecoSymbolArrow::HalfWidth()
{
    return m_pArrowPts[2].X;
}

void TxLineDecoSymbolArrow::RecalcShape()
{
    m_dX = 0.f;
    m_dY = 0.f;

    float dHalfWidth  = m_dWidth  * 0.5f;
    float dHalfHeight = m_dHeight * 0.5f;

    if (m_nPosFlag == 1) {
        m_dX = 0.f;
        m_dY = 0.f;
    }
    else if (m_nPosFlag == 2) {
        m_dY = m_dY + dHalfHeight;
    }
    else if (m_nPosFlag == 3) {
        m_dY = m_dY - dHalfHeight;
    }
    else {
        if (m_dPosRate > 0.6 ) {
            //● - 상단이 중심
            m_dY = m_dY + dHalfHeight;
        }
        else if (m_dPosRate < 0.4 ) {
            //● - 하단이 중심
            m_dY = m_dY - dHalfHeight;
        }
    }

    m_pArrowPts[0].X = m_dX - dHalfWidth ;
    m_pArrowPts[0].Y = m_dY + dHalfHeight ;

    m_pArrowPts[1].X = m_dX + 0;
    m_pArrowPts[1].Y = m_dY - dHalfHeight;                            

    m_pArrowPts[2].X = m_dX + dHalfWidth;
    m_pArrowPts[2].Y = m_dY + dHalfHeight;

    m_pArrowPts[3].X = m_dX - dHalfWidth;
    m_pArrowPts[3].Y = m_dY + dHalfHeight;
}

void TxLineDecoSymbolArrow::SetLinePosFlag( int nFlag )
{
    if (nFlag < 0 || nFlag > 3)
        nFlag = 0;

    m_nPosFlag = nFlag;

    RecalcShape();
}

Gdiplus::Color TxLineDecoSymbolArrow::Color()
{
    return m_clrArrow;
}

void TxLineDecoSymbolArrow::Filled( bool bFill )
{
    m_bFilled = bFill;
}

bool TxLineDecoSymbolArrow::Filled()
{
    return m_bFilled;
}
