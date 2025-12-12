#include "stdafx.h"
#include "TxFeatureUserLayer.h"
#include "ThunderMapdrawProcess.h"
#include "TxConst.h"

TxFeatureUserLayer::TxFeatureUserLayer( TxLayerInfoPtr spLayerInfo ) : ITxFeatureUserLayer(spLayerInfo)
{
    m_spSpatialIndex = ITxSpatialIndexPtr(new TxMemorySpatialIndex());
}

TxFeatureUserLayer::~TxFeatureUserLayer()
{
    Clear();
}

void TxFeatureUserLayer::Clear()
{
    if (m_spSpatialIndex)
        m_spSpatialIndex->Clear();

    m_mapIdGeometry.clear();

    ClearExtendInfo();
    ClearFeatureSymbol();
    ClearFeatureLabel();

    SetDrawExpire();
}

void TxFeatureUserLayer::Delete()
{
    Clear();
}

void TxFeatureUserLayer::Delete( __int64 nxID )
{
    m_spSpatialIndex->Remove(nxID);
    auto iFind = m_mapIdGeometry.find(nxID);
    if (iFind != m_mapIdGeometry.end()) {
        m_mapIdGeometry.erase(iFind);
    }

    RemoveExtendInfo(nxID);
    RemoveFeatureSymbol(nxID);
    RemoveFeatureLabel(nxID);

    SetDrawExpire();
}

void TxFeatureUserLayer::Update( TxFeaturePtr spFeature )
{
    if (spFeature == nullptr)
        return;

    __int64 nxID  = spFeature->TxObjectID;
    auto    iFind = m_mapIdGeometry.find(nxID);

    if (iFind == m_mapIdGeometry.end()) {
        return;
    }
    
    if (true /* 삭제 */) {
        m_mapIdGeometry.erase(iFind);
        m_spSpatialIndex->Remove(nxID);
    }    

    Insert(spFeature);

    SetDrawExpire();
}

void TxFeatureUserLayer::Insert( TxFeaturePtr spFeature )
{
    if (spFeature == nullptr)
        return;

    __int64 nxID  = spFeature->TxObjectID;
    auto    iFind = m_mapIdGeometry.find(nxID);

    if (iFind != m_mapIdGeometry.end()) {
        return;
    }

    ITxGeometryPtr spGeometry = spFeature->Geometry();
    if (spGeometry == nullptr)
        return;

    TxEnvelope oEnvelope = spGeometry->Envelope();

    m_spSpatialIndex->Insert(nxID, oEnvelope);
    m_mapIdGeometry.insert(std::make_pair(nxID, spGeometry));

    SetDrawExpire();
}

void TxFeatureUserLayer::Insert( std::vector<TxFeaturePtr>& vecFeature )
{
    for (size_t i = 0; i < vecFeature.size(); i++) {
        Insert(vecFeature[i]);
    }    

    SetDrawExpire();
}

void TxFeatureUserLayer::Insert( TxFeatureDispPtr spFeatureDisp )
{
    TxFeaturePtr spFeature = spFeatureDisp->Feature();
    if (spFeature == nullptr)
        return;

    __int64 nxID  = spFeature->TxObjectID;
    auto    iFind = m_mapIdGeometry.find(nxID);

    if (iFind != m_mapIdGeometry.end()) {
        return;
    }

    Insert(spFeature); 

    ITxSymbolPtr spSymbol = spFeatureDisp->Symbol();
    if (spSymbol) {
        FeatureSymbol(spFeature->TxObjectID, spSymbol);
    }

    ITxLabelPtr spLabel  = spFeatureDisp->Label();
    if (spLabel) {
        FeatureLabel(spFeature->TxObjectID, spLabel);
    }

    //★ 불필요 : SetDrawExpire();
}

void TxFeatureUserLayer::Insert( std::vector<TxFeatureDispPtr>& vecFeatureDisp )
{
    for (size_t i=0; i<vecFeatureDisp.size(); i++) {
        Insert(vecFeatureDisp[i]);
    }

    //★ 불필요 : SetDrawExpire();
}

void TxFeatureUserLayer::PutFeatureDisp( std::vector<TxFeatureDispPtr>& vecFeatureDisp )
{
    Clear();

    //★ Geometry
    size_t nxCount = vecFeatureDisp.size();
    for (size_t i=0; i<nxCount; i++) {
        TxFeatureDispPtr spFeatureDisp = vecFeatureDisp[i];
        if (spFeatureDisp == nullptr)
            continue;

        TxFeaturePtr spFeature = spFeatureDisp->Feature();
        if (spFeature == nullptr)
            continue;

        __int64 nxID = spFeature->TxObjectID;
        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr)
            continue;
        
        m_mapIdGeometry.insert(std::make_pair(nxID, spGeometry));
    }

    //★ 인덱스 생성
    for (auto iter = m_mapIdGeometry.begin(); iter != m_mapIdGeometry.end(); ++iter) {
        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;
        if (spGeometry == nullptr)
            continue;

        TxEnvelope oEnvelope = spGeometry->Envelope();
        m_spSpatialIndex->Insert(nxID, oEnvelope);
    }

    //★ 심볼, 라벨
    for (size_t i=0; i<nxCount; i++) {
        TxFeatureDispPtr spFeatureDisp = vecFeatureDisp[i];
        if (spFeatureDisp == nullptr)
            continue;

        TxFeaturePtr spFeature = spFeatureDisp->Feature();
        if (spFeature == nullptr)
            continue;

        __int64 nxID = spFeature->TxObjectID;
        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr)
            continue;

        ITxSymbolPtr spSymbol = spFeatureDisp->Symbol();
        ITxLabelPtr  spLabel  = spFeatureDisp->Label();
        
        if (spSymbol != nullptr) {
            FeatureSymbol(nxID, spSymbol);
        }

        if (spLabel != nullptr) {
            FeatureLabel(nxID, spLabel);
        }
    }

    SetDrawExpire();
}

void TxFeatureUserLayer::FindFeature( TxEnvelope oEnvelope, std::vector<TxFeaturePtr>& vecFeature, bool bIncludeHide )
{
    vecFeature.clear();

    if ( bIncludeHide == false && LayerOn() == false )
        return;

    std::vector<__int64> vecKey;
    if (m_spSpatialIndex)
        vecKey = m_spSpatialIndex->Intersect(oEnvelope); 

    for (size_t i=0; i<vecKey.size(); i++)
    {
        __int64 nxKey = vecKey[i];
        TxFeatureDispPtr spFeatureDisp = GetFeatureDisp(nxKey);
        if (spFeatureDisp == nullptr)
            continue;

        if ( bIncludeHide == true) {
            TxFeaturePtr spFeature  = spFeatureDisp->Feature();
            if (spFeature != nullptr) {
                ITxGeometryPtr spGeometry = spFeature->Geometry();
                if( spGeometry->Intersect(oEnvelope)) 
                    vecFeature.push_back(spFeature);   
            }
        } 
        else {            
            if ( spFeatureDisp->Symbol()->Show() ) {
                TxFeaturePtr spFeature = spFeatureDisp->Feature();
                if (spFeature != nullptr) {
                    ITxGeometryPtr spGeometry = spFeature->Geometry();
                    if( spGeometry->Intersect(oEnvelope)) 
                        vecFeature.push_back(spFeature);  
                }
            }
        }   
    }
}

void TxFeatureUserLayer::GetGeometry( std::map<__int64, ITxGeometryPtr>& mapGeometry )
{
    for (auto iter = m_mapIdGeometry.begin(); iter != m_mapIdGeometry.end(); ++iter) {
        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;

        if (spGeometry == nullptr) {
            mapGeometry.insert(std::make_pair(nxID, spGeometry));
        } else {
            mapGeometry.insert(std::make_pair(nxID, spGeometry->CopyGeometry()));
        }        
    }
}


ITxGeometryPtr TxFeatureUserLayer::GetGeometry( __int64 nxID )
{
    auto iter = m_mapIdGeometry.find(nxID);
    auto end  = m_mapIdGeometry.end();

    if (iter == end)
        return nullptr;

    return iter->second;
}

void TxFeatureUserLayer::FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, double dMinExtent, ThunderMapdrawProcess* pDrawProcess)
{
    vecFeature.clear();

    std::vector<__int64> vecKey;
    if (m_spSpatialIndex)
        vecKey = m_spSpatialIndex->Intersect(oEnvelope); 

    for (size_t i=0; i<vecKey.size(); i++)
    {
        if (pDrawProcess->GetMapDrawStatus() == 0)
            break;

        __int64 nxKey = vecKey[i];

        TxFeatureDispPtr spFeatureDisp  = GetFeatureDisp(nxKey);
        if (spFeatureDisp != nullptr)
            vecFeature.push_back(spFeatureDisp);       
    }
}

void TxFeatureUserLayer::FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, bool bIncludeHide/*=true*/ )
{
    vecFeature.clear();

    std::vector<__int64> vecKey;
    if (m_spSpatialIndex)
        vecKey = m_spSpatialIndex->Intersect(oEnvelope); 

    for (size_t i=0; i<vecKey.size(); i++)
    {        
        __int64 nxKey = vecKey[i];

        TxFeatureDispPtr spFeatureDisp = GetFeatureDisp(nxKey);
        if (spFeatureDisp == nullptr)
            continue;

        if (bIncludeHide == true) {
            if (spFeatureDisp->Symbol()->Show() == false) {
                continue;
            }
        }

        vecFeature.push_back(spFeatureDisp);       
    }
}

TxFeaturePtr TxFeatureUserLayer::GetFeature( __int64 nxID )
{
    auto iter = m_mapIdGeometry.find(nxID);
    auto end  = m_mapIdGeometry.end();

    if (iter == end)
        return nullptr;

    TxFeaturePtr spFeature(new TxFeature(nxID, iter->second));
    return spFeature;
}

TxFeatureDispPtr TxFeatureUserLayer::GetFeatureDisp( __int64 nxID )
{
    auto iter = m_mapIdGeometry.find(nxID);
    auto end  = m_mapIdGeometry.end();

    if (iter == end)
        return nullptr;

    TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(nxID, iter->second));

    if (m_mapFeatureSymbol.size() > 0) {
        auto iter = m_mapFeatureSymbol.find(nxID);
        if (iter != m_mapFeatureSymbol.end()) {
            ITxSymbolPtr spSymbol = iter->second;
            spFeatureDisp->Symbol(spSymbol);
        }
        else {
            spFeatureDisp->Symbol(m_spLayerSymbol);
        }
    }
    else {
        spFeatureDisp->Symbol(m_spLayerSymbol);
    }

    if (m_mapLabel.size() > 0){
        auto iter = m_mapLabel.find(nxID);
        if (iter != m_mapLabel.end()) {
            spFeatureDisp->Label(iter->second);
        }
    }    

    return spFeatureDisp;
}

TxEnvelope TxFeatureUserLayer::Extent()
{
    return m_spSpatialIndex->GetExtent();
}

__int64 TxFeatureUserLayer::EntityCount()
{
    return m_spSpatialIndex->EntityCount();
}

__int64 TxFeatureUserLayer::MaxID()
{
    __int64 nxMaxID(0);
    auto iter = m_mapIdGeometry.rbegin();
    if (iter != m_mapIdGeometry.rend()) {
        nxMaxID = iter->first;
    }

    return nxMaxID;
}

