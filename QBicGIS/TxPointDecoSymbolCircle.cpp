#include "stdafx.h"
#include "TxPointDecoSymbolCircle.h"


TxPointDecoSymbolCircle::TxPointDecoSymbolCircle(double dRadius, Gdiplus::Color clrFill, Gdiplus::Color clrLine, double dLineWidth, double dDrawMaxScale )
    :ITxPointDecoSymbol(dDrawMaxScale)
{
    m_dRadius     = dRadius;
    m_dLineWidth  = dLineWidth;

    m_pSolidBrush = new Gdiplus::SolidBrush(clrFill);
    m_pLinePen    = new Gdiplus::Pen(clrLine, m_dLineWidth);

    Radius(dRadius);
}

TxPointDecoSymbolCircle::~TxPointDecoSymbolCircle()
{
    delete m_pSolidBrush;
    delete m_pLinePen;
}

void TxPointDecoSymbolCircle::Radius( double dR )
{
    m_dRadius = dR;
    m_gdiRect = Gdiplus::RectF(- m_dRadius, - m_dRadius, 2 * m_dRadius, 2 * m_dRadius);
}

void TxPointDecoSymbolCircle::Draw( Gdiplus::Graphics& xGraphics )
{
    xGraphics.FillEllipse(m_pSolidBrush, m_gdiRect);
    if (m_dLineWidth > 0)
        xGraphics.DrawEllipse(m_pLinePen, m_gdiRect);
}

double TxPointDecoSymbolCircle::Width()
{
    return m_dRadius;
}

double TxPointDecoSymbolCircle::Height()
{
    return m_dRadius;
}
