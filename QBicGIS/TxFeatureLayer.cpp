#include "stdafx.h"
#include "TxFeatureLayer.h"
#include "ITxMapServer.h"
#include "ThunderMapdrawProcess.h"
#include "TxTables.h"
#include "TxConst.h"

TxFeatureLayer::TxFeatureLayer( ITxMapServerPtr spMapServer, TxLayerInfoPtr spLayerInfo ) : ITxFeatureLayer(spLayerInfo), m_spMapServer(spMapServer)
{
}

TxFeatureLayer::~TxFeatureLayer()
{
    Clear();
}

void TxFeatureLayer::Clear()
{
}

void TxFeatureLayer::FindFeature( TxEnvelope oEnvelope, std::vector<TxFeaturePtr>& vecFeature, bool bIncludeHide )
{
    vecFeature.clear();

    if ( bIncludeHide == false && LayerOn() == false )
        return;

    std::map<__int64, ITxGeometryPtr> mapGeometry;
    m_spMapServer->LoadServerData(m_spLayerInfo, oEnvelope, mapGeometry);
    
    for (auto iter = mapGeometry.begin(); iter != mapGeometry.end(); ++iter) {
        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;

        if( spGeometry->Intersect(oEnvelope) == false) 
            continue;

        if (bIncludeHide == true) {
            TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxID, spGeometry));
            vecFeature.push_back(spFeature);
        } else {
            TxFeatureDispPtr spFeatureDisp = GetFeatureDisp(nxID, spGeometry);
            if ( spFeatureDisp->Symbol()->Show() ) {
                TxFeaturePtr spFeature = spFeatureDisp->Feature();
                if (spFeature != nullptr)
                    vecFeature.push_back(spFeature);   
            }
        }        
    }    
}


void TxFeatureLayer::GetGeometry( std::map<__int64, ITxGeometryPtr>& mapGeometry )
{
	//TxEnvelope oEnvelope(TxConst::MinI64ToDbl(), TxConst::MinI64ToDbl(), TxConst::MaxI64ToDbl(), TxConst::MaxI64ToDbl());
	//TxEnvelope oEnvelope(-999999999, -999999999, 999999999, 999999999);
    //m_spMapServer->LoadServerData(m_spLayerInfo, oEnvelope, mapGeometry);
    m_spMapServer->LoadServerData(m_spLayerInfo, mapGeometry);
}


ITxGeometryPtr TxFeatureLayer::GetGeometry( __int64 nxID )
{
    ITxGeometryPtr spGeometry = m_spMapServer->LoadGeometry(m_spLayerInfo, nxID);
    if (spGeometry == nullptr)
        return nullptr;

    return spGeometry;
}

void TxFeatureLayer::FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, double dMinExtent, ThunderMapdrawProcess* pDrawProcess)
{
    vecFeature.clear();

    std::map<__int64, ITxGeometryPtr> mapGeometry;
    m_spMapServer->LoadServerData(m_spLayerInfo, oEnvelope, dMinExtent, mapGeometry, pDrawProcess);

    for (auto iter = mapGeometry.begin(); iter != mapGeometry.end(); ++iter) {

        if (pDrawProcess->GetMapDrawStatus() == 0)
            break;

        __int64        nxID       = iter->first;
        ITxGeometryPtr spGeometry = iter->second;
        
        TxFeatureDispPtr spFeature = GetFeatureDisp(nxID, spGeometry);
        if (spFeature == nullptr)
            continue;

        vecFeature.push_back(spFeature);
    }
}

void TxFeatureLayer::FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, bool bIncludeHide/*=true*/ )
{
    vecFeature.clear();

    std::map<__int64, ITxGeometryPtr> mapGeometry;
    m_spMapServer->LoadServerData(m_spLayerInfo, oEnvelope, mapGeometry);

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

TxFeaturePtr TxFeatureLayer::GetFeature( __int64 nxID )
{
    ITxGeometryPtr spGeometry = m_spMapServer->LoadGeometry(m_spLayerInfo, nxID);
    if (spGeometry == nullptr)
        return nullptr;

    TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxID, spGeometry));
    return spFeature;
}

TxFeatureDispPtr TxFeatureLayer::GetFeatureDisp( __int64 nxID, ITxGeometryPtr spGeometry )
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

void TxFeatureLayer::ProhibitGeoInsert( __int64 nxID, ITxGeometryPtr spGeometry )
{
    Insert(nxID, spGeometry);
}

void TxFeatureLayer::Insert(__int64 nxID, ITxGeometryPtr spGeometry )
{
    m_spMapServer->Insert(m_spLayerInfo, nxID, spGeometry);
    SetDrawExpire();
}

void TxFeatureLayer::Insert( __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord )
{    
    m_spMapServer->Insert(m_spLayerInfo, nxID, spGeometry, oRecord);
    SetDrawExpire();
}

void TxFeatureLayer::Delete( __int64 nxID )
{
    m_spMapServer->Delete(m_spLayerInfo, nxID);
    
    RemoveExtendInfo(nxID);
    RemoveFeatureSymbol(nxID);
    RemoveFeatureLabel(nxID);

    SetDrawExpire();
}

void TxFeatureLayer::Delete()
{
    m_spMapServer->RemoveAll(m_spLayerInfo);
    m_mapExtendInfo.clear();
    m_mapFeatureSymbol.clear();
    m_mapLabel.clear();

    SetDrawExpire();
}

void TxFeatureLayer::Update( __int64 nxID, ITxGeometryPtr spGeometry )
{
    m_spMapServer->Update(m_spLayerInfo, nxID, spGeometry);
    SetDrawExpire();
}

void TxFeatureLayer::Update( __int64 nxID, TxRecord& oRecord )
{
    m_spMapServer->Update(m_spLayerInfo, nxID, oRecord);
    SetDrawExpire();
}

void TxFeatureLayer::Update(  __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord )
{
    m_spMapServer->Update(m_spLayerInfo, nxID, spGeometry, oRecord);
    SetDrawExpire();
}

TxEnvelope TxFeatureLayer::Extent()
{
    TxEnvelope oEnv; 
    oEnv = m_spMapServer->GetExtent(m_spLayerInfo);
    return oEnv;
}

__int64 TxFeatureLayer::EntityCount()
{
    __int64 nxCnt(0); {
        nxCnt = m_spMapServer->EntityCount(m_spLayerInfo);
    }

    return nxCnt;
}

void TxFeatureLayer::RebuildIndex()
{//내부에서 트랜잭션이 발생
    m_spMapServer->RebuildIndex(m_spLayerInfo);
    SetDrawExpire();
}

void TxFeatureLayer::RebuildIndexGridSize(double a_dGSize)
{
	m_spMapServer->RebuildIndex(m_spLayerInfo, a_dGSize);
	SetDrawExpire();
}

