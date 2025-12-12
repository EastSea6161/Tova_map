#include "stdafx.h"
#include "TxFeatureFileUserLayer.h"
#include "ThunderMapdrawProcess.h"
#include "TxConst.h"

TxFeatureFileUserLayer::TxFeatureFileUserLayer( TxLayerInfoPtr spLayerInfo ) : ITxFeatureUserLayer(spLayerInfo)
{
    m_spFileMapServer = TxTempMapServerPtr(new TxTempMapServer());
}

TxFeatureFileUserLayer::~TxFeatureFileUserLayer()
{
    Clear();
}


void TxFeatureFileUserLayer::Clear()
{
    if (m_spFileMapServer)
        m_spFileMapServer->Clear();

    ClearExtendInfo();
    ClearFeatureSymbol();
    ClearFeatureLabel();

    SetDrawExpire();
}

TxFeaturePtr TxFeatureFileUserLayer::GetFeature( __int64 nxID )
{
    ITxGeometryPtr spGeometry = m_spFileMapServer->LoadGeometry(nxID);
    if (spGeometry == nullptr) 
        return nullptr;
    
    return TxFeaturePtr(new TxFeature(nxID, spGeometry));
}

void TxFeatureFileUserLayer::FindFeature( TxEnvelope oEnvelope, std::vector<TxFeaturePtr>& vecFeature, bool bIncludeHide )
{
    vecFeature.clear();

    if (m_spFileMapServer == nullptr) 
        return;

    if ( bIncludeHide == false && LayerOn() == false )
        return;

    std::map<__int64, ITxGeometryPtr> mapGeometry;
    m_spFileMapServer->LoadGeometry(oEnvelope, mapGeometry);

    for (auto iter = mapGeometry.begin(); iter != mapGeometry.end(); ++iter) {
        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;

        if ( bIncludeHide == true) {
            TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxID, spGeometry));
            if (spFeature != nullptr) {
                ITxGeometryPtr spGeometry = spFeature->Geometry();
                if( spGeometry->Intersect(oEnvelope)) 
                    vecFeature.push_back(spFeature);  
            }
        } else {
            TxFeatureDispPtr spFeatureDisp = GetFeatureDisp(nxID, spGeometry);
            if (spFeatureDisp == nullptr)
                continue;

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

void TxFeatureFileUserLayer::FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeatureDisp, double dMinExtent, ThunderMapdrawProcess* pDrawProcess)
{
    vecFeatureDisp.clear();

    if (m_spFileMapServer == nullptr) 
        return;

    std::map<__int64, ITxGeometryPtr> mapGeometry;
    m_spFileMapServer->LoadGeometry(oEnvelope, mapGeometry, pDrawProcess);

    for (auto iter = mapGeometry.begin(); iter != mapGeometry.end(); ++iter) {
        if (pDrawProcess->GetMapDrawStatus() == 0)
            break;

        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;

        TxFeatureDispPtr spFeature = GetFeatureDisp(nxID, spGeometry);
        if (spFeature == nullptr)
            continue;

        vecFeatureDisp.push_back(spFeature);
    }
}

void TxFeatureFileUserLayer::FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, bool bIncludeHide/*=true*/ )
{
    vecFeature.clear();

    if (m_spFileMapServer == nullptr) 
        return;

    std::map<__int64, ITxGeometryPtr> mapGeometry;
    m_spFileMapServer->LoadGeometry(oEnvelope, mapGeometry);

    for (auto iter = mapGeometry.begin(); iter != mapGeometry.end(); ++iter) {
        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;

        TxFeatureDispPtr spFeature = GetFeatureDisp(nxID, spGeometry);
        if (spFeature == nullptr)
            continue;

        if (bIncludeHide == true) {
            if (spFeature->Symbol()->Show() == false)
                continue;
        }

        vecFeature.push_back(spFeature);
    }
}

void TxFeatureFileUserLayer::GetGeometry( std::map<__int64, ITxGeometryPtr>& mapGeometry )
{
    TxEnvelope oEnvelope(TxConst::MinDbl(), TxConst::MinDbl(), TxConst::MaxDbl(), TxConst::MaxDbl());
    m_spFileMapServer->LoadGeometry(oEnvelope, mapGeometry);
}


ITxGeometryPtr TxFeatureFileUserLayer::GetGeometry( __int64 nxID )
{
    ITxGeometryPtr spGeometry = m_spFileMapServer->LoadGeometry(nxID);
    if (spGeometry == nullptr) 
        return nullptr;

    return spGeometry;
}

TxFeatureDispPtr TxFeatureFileUserLayer::GetFeatureDisp( __int64 nxID, ITxGeometryPtr spGeometry )
{   
    TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(nxID, spGeometry));

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

void TxFeatureFileUserLayer::Delete()
{
    Clear();    
    SetDrawExpire();
}

void TxFeatureFileUserLayer::Delete( __int64 nxID )
{
    if (m_spFileMapServer == nullptr) 
        return;

    m_spFileMapServer->Delete(nxID);
    RemoveExtendInfo(nxID);
    RemoveFeatureSymbol(nxID);
    RemoveFeatureLabel(nxID);

    SetDrawExpire();
}

void TxFeatureFileUserLayer::Update( TxFeaturePtr spFeature )
{
    if (m_spFileMapServer == nullptr) 
        return;

    m_spFileMapServer->Update(spFeature);
    SetDrawExpire();
}

void TxFeatureFileUserLayer::Insert( TxFeaturePtr spFeature )
{
    if (m_spFileMapServer == nullptr) 
        return;

    m_spFileMapServer->Insert(spFeature);
    SetDrawExpire();
}

void TxFeatureFileUserLayer::Insert( std::vector<TxFeaturePtr>& vecFeature )
{
    if (m_spFileMapServer == nullptr) 
        return;

    std::map<__int64, ITxGeometryPtr> mapGeometry;
    for (size_t i=0; i<vecFeature.size(); i++) {
        TxFeaturePtr spFeature = vecFeature[i];
        mapGeometry.insert(std::make_pair(spFeature->TxObjectID, spFeature->Geometry()));
    }

    m_spFileMapServer->Insert(mapGeometry);
    SetDrawExpire();
}

void TxFeatureFileUserLayer::Insert( TxFeatureDispPtr spFeatureDisp )
{
    TxFeaturePtr spFeature = spFeatureDisp->Feature();
    if (spFeature == nullptr)
        return;

    Insert(spFeature);
    ITxSymbolPtr spSymbol = spFeatureDisp->Symbol();
    if (spSymbol) {
        FeatureSymbol(spFeature->TxObjectID, spSymbol);
    }

    ITxLabelPtr spLabel  = spFeatureDisp->Label();
    if (spLabel) {
        FeatureLabel(spFeature->TxObjectID, spLabel);
    }   
}

void TxFeatureFileUserLayer::Insert( std::vector<TxFeatureDispPtr>& vecFeatureDisp )
{
    for (size_t i=0; i<vecFeatureDisp.size(); i++) {
        Insert(vecFeatureDisp[i]);
    }
}

void TxFeatureFileUserLayer::PutFeatureDisp( std::vector<TxFeatureDispPtr>& vecFeatureDisp )
{
    Clear();
    
    //★ Geometry
    std::map<__int64, ITxGeometryPtr> mapGeometry;
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

        mapGeometry.insert(std::make_pair(nxID, spGeometry));
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

TxEnvelope TxFeatureFileUserLayer::Extent()
{
    TxEnvelope oEnv; 
    if (m_spFileMapServer != nullptr) {
        oEnv = m_spFileMapServer->Extent();
    }
    return oEnv;
}

__int64 TxFeatureFileUserLayer::EntityCount()
{
    __int64 nxCnt(0); 
    if (m_spFileMapServer != nullptr) {
        nxCnt = m_spFileMapServer->EntityCount();
    }

    return nxCnt;
}

__int64 TxFeatureFileUserLayer::MaxID()
{
    __int64 nxMaxID(0); 
    if (m_spFileMapServer != nullptr) {
        nxMaxID = m_spFileMapServer->MaxID();
    }

    return nxMaxID;
}
