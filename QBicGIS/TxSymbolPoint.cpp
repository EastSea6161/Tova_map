#include "stdafx.h"
#include "TxSymbolPoint.h"

TxSymbolPoint::TxSymbolPoint(char cSymbol, double dSize, Gdiplus::Color color) : ITxSymbol()
{
    m_cSymbol  = cSymbol;
    m_dSize    = dSize;
    m_clrPoint = color;
	m_bFixedSize = false;
}

TxSymbolPoint::~TxSymbolPoint()
{
}

TxSymbolType TxSymbolPoint::GetType()
{
    return enSymbolPointType;
}

int TxSymbolPoint::Symbol()
{
    return m_cSymbol;
}

void TxSymbolPoint::Symbol( char nSymbol )
{
    m_cSymbol = nSymbol;
}

double TxSymbolPoint::Size()
{
    return m_dSize;
}

void TxSymbolPoint::Size( double dSize )
{
    m_dSize = dSize;
}

Gdiplus::Color TxSymbolPoint::Color()
{
    return m_clrPoint;
}

void TxSymbolPoint::Color( Gdiplus::Color color )
{
    m_clrPoint = color;
}

double TxSymbolPoint::DrawOrderOptionValue()
{
    return m_nDrawOrder * m_dSize;
}

bool TxSymbolPoint::operator < (const TxSymbolPoint& right) const
{
    if (m_cSymbol == right.m_cSymbol) {
        if (m_dSize == right.m_dSize) {
            return m_clrPoint.GetValue() < right.m_clrPoint.GetValue();
        }
        
        int nMeSize = (int) (m_dSize * 10.0);
        int nYuSize = (int) (right.m_dSize * 10.0);
        return nMeSize < nYuSize;
    }

    return m_cSymbol < right.m_cSymbol;
}

std::map<int, ITxPointDecoSymbolPtr> TxSymbolPoint::GetDecoSymbol()
{
    return m_mapDeco;
}

void TxSymbolPoint::SetDecoSymbol( ITxPointDecoSymbolPtr spDecoSym, int nIdx /*= 0*/ )
{
    m_mapDeco[nIdx] = spDecoSym;
}

void TxSymbolPoint::RemoveDecoSymbol()
{
    m_mapDeco.clear();
}

void TxSymbolPoint::RemoveDecoSymbol( int nIdx )
{
    auto iterF = m_mapDeco.find(nIdx);
    if (iterF != m_mapDeco.end())
        m_mapDeco.erase(iterF);
}
