#include "stdafx.h"
#include "TxUserLayer.h"

TxFeatureUserLayer::TxFeatureUserLayer( TxLayerInfoPtr spLayerInfo ) : ITxFeatureLayer(spLayerInfo)
{
}

TxFeatureUserLayer::~TxFeatureUserLayer()
{
}

void TxFeatureUserLayer::Clear()
{
    if (m_spSpatialIndex)
        m_spSpatialIndex->Clear();

    m_mapFeatureDispData.clear();
}

int TxFeatureUserLayer::Initialize()
{
    KLogDebugStartMsg();

    m_spSpatialIndex = ITxSpatialIndexPtr(new TxMemorySpatialIndex());

    for (auto iter = m_mapFeatureDispData.begin(); iter != m_mapFeatureDispData.end(); ++iter)
    {
        __int64      nxID      = iter->first;
        TxFeatureDispPtr spFeatureDisp = iter->second;

        if (spFeatureDisp == nullptr)
            continue;

        TxFeaturePtr spFeature = spFeatureDisp->Feature();
        if (spFeature == nullptr)
            continue;

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr)
            continue;

        TxEnvelope oEnvelope = spGeometry->Envelope();
        m_spSpatialIndex->Insert(nxID, oEnvelope);
    }
    
    KLogDebugEndMsg();
    return 1;
}


void TxFeatureUserLayer::PutFeatureDisp( std::vector<TxFeatureDispPtr>& _vecFeautreDisp )
{
    Clear();

    size_t nxCount = _vecFeautreDisp.size();
    for (size_t i=0; i<nxCount; i++)
    {
        TxFeatureDispPtr spFeatureDisp = _vecFeautreDisp[i];

        if (spFeatureDisp == nullptr)
            continue;

        TxFeaturePtr spFeature = spFeatureDisp->Feature();
        if (spFeature == nullptr)
            continue;

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr)
            continue;

        __int64 nxID = spFeature->TxObjectID;

        if ( m_mapFeatureDispData.find(nxID) == m_mapFeatureDispData.end() )
        {            
            m_mapFeatureDispData.insert(std::make_pair(nxID, spFeatureDisp));
        }
    }

    Initialize();
}

void TxFeatureUserLayer::FindFeature( TxEnvelope oEnvelope, std::vector<TxFeaturePtr>& vecFeature )
{
    vecFeature.clear();

    std::vector<__int64> vecKey;
    if (m_spSpatialIndex)
        vecKey = m_spSpatialIndex->Intersect(oEnvelope); 

    for (size_t i=0; i<vecKey.size(); i++)
    {
        __int64 nxKey = vecKey[i];

        TxFeaturePtr spFeautre  = GetFeature(nxKey);
        if (spFeautre != nullptr)
            vecFeature.push_back(spFeautre);       
    }
}


void TxFeatureUserLayer::FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, double dMinExtent )
{
    vecFeature.clear();

    std::vector<__int64> vecKey;
    if (m_spSpatialIndex)
        vecKey = m_spSpatialIndex->Intersect(oEnvelope); 

    for (size_t i=0; i<vecKey.size(); i++)
    {
        __int64 nxKey = vecKey[i];

        TxFeatureDispPtr spFeautre  = GetFeatureDisp(nxKey);
        if (spFeautre != nullptr)
            vecFeature.push_back(spFeautre);       
    }
}

TxFeaturePtr TxFeatureUserLayer::GetFeature( __int64 nxID )
{
    TxFeatureDispPtr spFeatureDisp = GetFeatureDisp(nxID);
    return spFeatureDisp->Feature();
}

TxFeatureDispPtr TxFeatureUserLayer::GetFeatureDisp( __int64 nxID )
{
    auto iter = m_mapFeatureDispData.find(nxID);
    auto end  = m_mapFeatureDispData.end();

    if (iter == end)
        return nullptr;

    return iter->second;
}

void TxFeatureUserLayer::LayerOnOff( bool bVisible )
{
    m_bsFlag.at(EnumFlagDisplay) = bVisible;
}

void TxFeatureUserLayer::LayerDispSymbol( ITxSymbolPtr spSymbol )
{    
    if (CheckSymbolType(spSymbol) == false) return;

    for (auto iter=m_mapFeatureDispData.begin(); iter != m_mapFeatureDispData.end(); ++iter)
    {
        TxFeatureDispPtr spFeature = iter->second;
        spFeature->Symbol(spSymbol);
    }

    m_spLayerSymbol = spSymbol;
}

void TxFeatureUserLayer::FeatureSymbol( __int64 nxID, ITxSymbolPtr spSymbol )
{
    if (CheckSymbolType(spSymbol) == false) return;

    auto iter = m_mapFeatureDispData.find(nxID);
    if (iter != m_mapFeatureDispData.end())
    {
        TxFeatureDispPtr spFeature = iter->second;
        spFeature->Symbol(spSymbol);
    }
}

void TxFeatureUserLayer::ClearFeatureSymbol()
{
    LayerDispSymbol(m_spLayerSymbol);
}

void TxFeatureUserLayer::FeatureLabel( __int64 nxID, ITxLabelPtr spLabel )
{
    auto iter = m_mapFeatureDispData.find(nxID);
    if (iter != m_mapFeatureDispData.end())
    {
        TxFeatureDispPtr spFeature = iter->second;
        spFeature->Label(spLabel);
    }
}

void TxFeatureUserLayer::ClearFeatureLabel()
{
    CString strLabel(_T(""));
    auto iter = m_mapFeatureDispData.begin();
    auto end  = m_mapFeatureDispData.end();
    for(/**/; iter != end; ++iter) {
        TxFeatureDispPtr spFeature = iter->second;
        spFeature->Label(nullptr);
    }
}

