#include "stdafx.h"
#include "TxLineDecoSymbolCircle.h"

//★ ----------------------------------------------------------------------------------------------------------
TxLineDecoSymbolCircle::TxLineDecoSymbolCircle( double dDrawMaxScale, double dPosRate, Gdiplus::Color clrFill, Gdiplus::Color clrLine, double dLineWidth)
    :ITxLineDecoSymbol(dDrawMaxScale, dPosRate)
{
    m_dX = 0.f;
    m_dY = 0.f;
    m_dR = 9.f;

    m_nPosFlag   = 0;
    m_dLineWidth = dLineWidth;

    m_pSolidBrush = new Gdiplus::SolidBrush(clrFill);
    m_pLinePen    = new Gdiplus::Pen(clrLine, m_dLineWidth);

    RecalcShape();
}

TxLineDecoSymbolCircle::~TxLineDecoSymbolCircle()
{
    delete m_pSolidBrush;
    delete m_pLinePen;
}

void TxLineDecoSymbolCircle::Radius( int nSize, int nPosFlag )
{
    m_dR = (float)nSize;

    if (nPosFlag < 0 || nPosFlag > 3)
        nPosFlag = 0;

    m_nPosFlag = nPosFlag;

    RecalcShape();
}

void TxLineDecoSymbolCircle::Draw( Gdiplus::Graphics& xGraphics )
{
    xGraphics.FillEllipse(m_pSolidBrush, m_gdiRect);
    if (m_dLineWidth > 0)
        xGraphics.DrawEllipse(m_pLinePen, m_gdiRect);
}

double TxLineDecoSymbolCircle::HalfWidth()
{
    return m_gdiRect.Width * 0.5;
}

void TxLineDecoSymbolCircle::RecalcShape()
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
}

void TxLineDecoSymbolCircle::SetLinePosFlag( int nPosFlag )
{
    if (nPosFlag < 0 || nPosFlag > 3)
        nPosFlag = 0;

    m_nPosFlag = nPosFlag;

    RecalcShape();
}
