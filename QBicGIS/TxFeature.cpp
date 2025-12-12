#include "stdafx.h"
#include "TxFeature.h"
#include "ITxLayer.h"
#include "TxFeatureLayer.h"

TxFeature::TxFeature() : ITxObject()
{
}

TxFeature::TxFeature( __int64 nxFeatureID, ITxGeometryPtr spGeometry )
{
    TxObjectID   = nxFeatureID;
    m_spGeometry = spGeometry;
}

TxFeature::TxFeature( const TxFeature& right )
{
    TxObjectID   = right.TxObjectID;
    m_spGeometry = right.m_spGeometry;
}

TxFeature::~TxFeature()
{
}

const ITxGeometryPtr TxFeature::Geometry() const
{
    return m_spGeometry;
}

void TxFeature::Geometry(ITxGeometryPtr spGeometry )
{
    m_spGeometry = spGeometry;
}

ITxGeometryPtr TxFeature::CopyGeometry()
{
    if (m_spGeometry == nullptr)
        return nullptr;

    return m_spGeometry->CopyGeometry();
}

TxFeaturePtr TxFeature::Copy()
{
    TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(TxObjectID, m_spGeometry->CopyGeometry()));
    return spFeature;
}
