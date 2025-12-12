#include "stdafx.h"
#include "TxLineDecoSymbolCircleCross.h"

//★ ----------------------------------------------------------------------------------------------------------
TxLineDecoSymbolCircleCross::TxLineDecoSymbolCircleCross( double dDrawMaxScale, double dPosRate, Gdiplus::Color clrBack, Gdiplus::Color clrIn)
    :ITxLineDecoSymbol(dDrawMaxScale, dPosRate)
{
    m_bDrawCross = true;

    m_dX = 0.f;
    m_dY = 0.f;
    m_dR = 9.f;
    m_nPosFlag = 0;

    m_dInLineSize = 1.f;
    m_pSolidBrush = new Gdiplus::SolidBrush(clrBack);
    m_pLinePen    = new Gdiplus::Pen(clrIn, m_dInLineSize);

    RecalcShape();
}

TxLineDecoSymbolCircleCross::~TxLineDecoSymbolCircleCross()
{
    delete m_pSolidBrush;
    delete m_pLinePen;
}

void TxLineDecoSymbolCircleCross::Radius( int nSize )
{
    m_dR = (float)nSize;
    RecalcShape();
}


void TxLineDecoSymbolCircleCross::InCrossLine( double dWidth, Gdiplus::Color clrIn )
{
    delete m_pLinePen;
    m_dInLineSize = (float)dWidth;
    m_pLinePen = new Gdiplus::Pen(clrIn, m_dInLineSize);
}

void TxLineDecoSymbolCircleCross::Draw( Gdiplus::Graphics& xGraphics )
{
    xGraphics.FillEllipse(m_pSolidBrush, m_gdiRect);
    if (m_bDrawCross) {
        xGraphics.DrawLines(m_pLinePen, m_pLine1, (INT)2);  
        xGraphics.DrawLines(m_pLinePen, m_pLine2, (INT)2);  
    }    
}

double TxLineDecoSymbolCircleCross::HalfWidth()
{
    return m_gdiRect.Width * 0.5;
}

void TxLineDecoSymbolCircleCross::RecalcShape()
{
    m_dX = 0.f;
    m_dY = 0.f;

    if (m_nPosFlag == 1) {
        m_dX = 0.f;
        m_dY = 0.f;
    }
    else if (m_nPosFlag == 2) {
        m_dY = m_dY + m_dR;
    }
    else if (m_nPosFlag == 3) {
        m_dY = m_dY - m_dR;
    }
    else {
        if (m_dPosRate > 0.6 ) {
            //● - 상단이 중심
            m_dY = m_dY + m_dR;
        }
        else if (m_dPosRate < 0.4 ) {
            //● - 하단이 중심
            m_dY = m_dY - m_dR;
        }
    }

    m_gdiRect = Gdiplus::RectF(m_dX - m_dR, m_dY - m_dR, 2 * m_dR, 2 * m_dR);

    float dHalf = m_dR * 0.5f;
    m_pLine1[0].X = m_dX - dHalf;
    m_pLine1[0].Y = m_dY + dHalf;
    m_pLine1[1].X = m_dX + dHalf;
    m_pLine1[1].Y = m_dY - dHalf;

    m_pLine2[0].X = m_dX - dHalf;
    m_pLine2[0].Y = m_dY - dHalf;
    m_pLine2[1].X = m_dX + dHalf;
    m_pLine2[1].Y = m_dY + dHalf;    
}

void TxLineDecoSymbolCircleCross::DrawCross( bool bShow )
{
    m_bDrawCross = bShow;
}

void TxLineDecoSymbolCircleCross::SetLinePosFlag( int nFlag )
{
    if (nFlag < 0 || nFlag > 3)
        nFlag = 0;

    m_nPosFlag = nFlag;

    RecalcShape();
}
