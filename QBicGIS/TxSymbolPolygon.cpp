#include "stdafx.h"
#include "TxSymbolPolygon.h"

TxSymbolPolygon::TxSymbolPolygon()
{
    m_bsFlag[EnumFlagDrawFilled]  = false;
    m_bsFlag[EnumFlagDrawOutLine] = true;
    
    m_clrFill    = Gdiplus::Color::White;
    m_clrOutLine = Gdiplus::Color::Black; {
        m_dOutLineWidth = 1.0;
    }    
}

TxSymbolPolygon::TxSymbolPolygon( Gdiplus::Color clrFill )
{
    m_bsFlag[EnumFlagDrawFilled]  = true;
    m_bsFlag[EnumFlagDrawOutLine] = false;

    m_clrFill    = clrFill;
    m_clrOutLine = Gdiplus::Color::Black; {
        m_dOutLineWidth = 0.0;
    }    
}

TxSymbolPolygon::TxSymbolPolygon( Gdiplus::Color clrFill, Gdiplus::Color clrOutLine )
{
    m_bsFlag[EnumFlagDrawFilled]  = true;
    m_bsFlag[EnumFlagDrawOutLine] = true;
 
    m_clrFill    = clrFill;
    m_clrOutLine = clrOutLine; {
        m_dOutLineWidth = 1.0;
    }    
}

TxSymbolPolygon::TxSymbolPolygon( Gdiplus::Color clrFill, Gdiplus::Color clrOutLine, double dOutLineWidth )
{
    m_bsFlag[EnumFlagDrawFilled]  = true;
    m_bsFlag[EnumFlagDrawOutLine] = true;

    m_clrFill    = clrFill;
    m_clrOutLine = clrOutLine; {
        m_dOutLineWidth = dOutLineWidth;
    }   
}

TxSymbolPolygon::~TxSymbolPolygon()
{

}

TxSymbolType TxSymbolPolygon::GetType()
{
    return enSymbolPolygonType;
}
