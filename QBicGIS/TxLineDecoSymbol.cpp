#include "stdafx.h"
#include "TxLineDecoSymbol.h"


ITxLineDecoSymbol::ITxLineDecoSymbol( double dDrawMaxScale, double dPosRate)
{
    if (dPosRate <= 0) {
        m_dPosRate = 0;
    } else if (dPosRate > 1) {
        m_dPosRate = 1;
    } else {
        m_dPosRate  = (float)dPosRate;
    }
    
    dDrawMaxScale  += 0.5;
    m_nDrawMaxScale = (int)dDrawMaxScale;

    m_bRightShift = true;
}

void ITxLineDecoSymbol::DrawMaxScale( double dDrawMaxScale )
{
    dDrawMaxScale  += 0.5;
    m_nDrawMaxScale = (int)dDrawMaxScale;
}

double ITxLineDecoSymbol::DrawMaxScale()
{
    return m_nDrawMaxScale;
}

bool ITxLineDecoSymbol::RightShift()
{
    return m_bRightShift;
}

void ITxLineDecoSymbol::RightShift( bool bRightShift )
{
    m_bRightShift = bRightShift;
}

double ITxLineDecoSymbol::PosRate()
{
    return m_dPosRate;
}

void ITxLineDecoSymbol::PosRate( double dPosRate )
{
    if (dPosRate <= 0) {
        m_dPosRate = 0;
    } else if (dPosRate > 1) {
        m_dPosRate = 1;
    } else {
        m_dPosRate  = (float)dPosRate;
    }

    RecalcShape();
}



