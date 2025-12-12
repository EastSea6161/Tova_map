#include "stdafx.h"
#include "TxSymbolDirectionLine.h"


TxSymbolDirectionLine::TxSymbolDirectionLine()
{
}

TxSymbolDirectionLine::TxSymbolDirectionLine(ITxSymbolPtr spForward, ITxSymbolPtr spBackward)
{
    m_spForward  = spForward;
    m_spBackward = spBackward;
}

TxSymbolDirectionLine::TxSymbolDirectionLine(EnumTxDirectionFlag emFlag, ITxSymbolPtr spSymbol)
{
    if (EnumForward == emFlag) {
        m_spForward  = spSymbol;
        m_spBackward = nullptr;
    }
    else {
        m_spForward  = nullptr;
        m_spBackward = spSymbol;
    }
}

TxSymbolDirectionLine::~TxSymbolDirectionLine()
{
}

void TxSymbolDirectionLine::ForwardSymbol( ITxSymbolPtr spSymbol )
{
    m_spForward = spSymbol;
}

ITxSymbolPtr TxSymbolDirectionLine::ForwardSymbol()
{
    return m_spForward;
}

void TxSymbolDirectionLine::BackwardSymbol( ITxSymbolPtr spSymbol )
{
    m_spBackward = spSymbol;
}

ITxSymbolPtr TxSymbolDirectionLine::BackwardSymbol()
{
    return m_spBackward;
}

double TxSymbolDirectionLine::DrawOrderOptionValue()
{
    double dDirLeft(0), dDirRight(0);
    if (m_spForward != nullptr)
        dDirLeft  = m_spForward->DrawOrderOptionValue();

    if (m_spBackward != nullptr)
        dDirRight = m_spBackward->DrawOrderOptionValue();

    if (dDirLeft < dDirRight) {
        return dDirRight;
    } else {
        return dDirLeft;
    }
}
