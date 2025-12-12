#include "stdafx.h"
#include "TxPointDecoSymbol.h"

ITxPointDecoSymbol::ITxPointDecoSymbol( double dDrawMaxScale )
{
    m_dDrawMaxScale = dDrawMaxScale;
    m_dShiftX = 0;
    m_dShiftY = 0;
}

void ITxPointDecoSymbol::DrawMaxScale( double dDrawMaxScale )
{
    m_dDrawMaxScale = dDrawMaxScale;
}

double ITxPointDecoSymbol::DrawMaxScale()
{
    return m_dDrawMaxScale;
}

double ITxPointDecoSymbol::ShiftX()
{
    return m_dShiftX;
}

void ITxPointDecoSymbol::ShiftX( double dX )
{
    m_dShiftX = dX;
}

double ITxPointDecoSymbol::ShiftY()
{
    return m_dShiftY;
}

void ITxPointDecoSymbol::ShiftY( double dY )
{
    m_dShiftY = dY;
}

void ITxPointDecoSymbol::Shift( double dX, double dY )
{
    m_dShiftX = dX;
    m_dShiftY = dY;
}
