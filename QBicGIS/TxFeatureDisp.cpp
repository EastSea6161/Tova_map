#include "stdafx.h"
#include "TxFeatureDisp.h"

TxFeatureDisp::TxFeatureDisp()
{
}

TxFeatureDisp::TxFeatureDisp( TxFeaturePtr spFeature ) : m_spFeature(spFeature)
{
}

TxFeatureDisp::TxFeatureDisp(__int64 nxFeatureID, ITxGeometryPtr spGeometry )
{
    m_spFeature = TxFeaturePtr(new TxFeature(nxFeatureID, spGeometry));
}

TxFeatureDisp::TxFeatureDisp( const TxFeatureDisp& right )
{    
    m_spFeature = right.m_spFeature;
    m_spSymbol  = right.m_spSymbol;
    m_spLabel   = right.m_spLabel;
}

void TxFeatureDisp::Label( ITxLabelPtr spLabel )
{
    m_spLabel = spLabel;
}

ITxLabelPtr TxFeatureDisp::Label() const
{
    return m_spLabel;
}

ITxSymbolPtr TxFeatureDisp::Symbol() const
{
    return m_spSymbol;
}

void TxFeatureDisp::Symbol( ITxSymbolPtr spSymbol )
{
    m_spSymbol = spSymbol;
}