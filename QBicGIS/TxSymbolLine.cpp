#include "stdafx.h"
#include "TxSymbolLine.h"

TxSymbolLine::TxSymbolLine(double dLineWidth, Gdiplus::Color clrLine, bool bUseOffset) : 
              ITxSymbol(), m_dLineWidth(dLineWidth), m_clrLine(clrLine)
{
    m_nDashStyle = Gdiplus::DashStyleSolid;

    if (bUseOffset == true) {
        m_spOffsetInfo = TxOffsetInfoPtr(new TxOffsetInfo(0));
    }
}

TxSymbolLine::~TxSymbolLine()
{
}

Gdiplus::DashStyle TxSymbolLine::GetDashStyle()
{
    return m_nDashStyle;
}

void TxSymbolLine::SetDashStyle( int nStyle/*0:DashStyleSolid, 1:DashStyleDash, 2:DashStyleDot, 3:DashStyleDashDot, 4:DashStyleDashDotDot*/ )
{
    if (nStyle == 1) {
        m_nDashStyle = Gdiplus::DashStyleDash;
    }
    else if (nStyle == 2) {
        m_nDashStyle = Gdiplus::DashStyleDot;
    }
    else if (nStyle == 3) {
        m_nDashStyle = Gdiplus::DashStyleDashDot;
    }
    else if (nStyle == 4) {
        m_nDashStyle = Gdiplus::DashStyleDashDotDot;
    }
    else {
        m_nDashStyle = Gdiplus::DashStyleSolid;
    }
}

void TxSymbolLine::SetOffset( TxOffsetInfoPtr spOffsetInfo )
{
    m_spOffsetInfo = spOffsetInfo;
}

void TxSymbolLine::RemoveOffset()
{
    if (m_spOffsetInfo)
        m_spOffsetInfo.reset();
}

std::map<int, ITxLineDecoSymbolPtr> TxSymbolLine::GetDecoSymbol()
{
    return m_mapDeco;
}

void TxSymbolLine::SetDecoSymbol( ITxLineDecoSymbolPtr spDecoSym, int nIdx /*= 0*/ )
{    
    m_mapDeco[nIdx] = spDecoSym;
}

void TxSymbolLine::RemoveDecoSymbol()
{
    m_mapDeco.clear();
}

void TxSymbolLine::RemoveDecoSymbol( int nIdx )
{
    auto iterF = m_mapDeco.find(nIdx);
    if (iterF != m_mapDeco.end())
        m_mapDeco.erase(iterF);
}
