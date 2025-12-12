#include "stdafx.h"
#include "TxMapLayerManager.h"

TxMapLayerManager::TxMapLayerManager()
{    
}

TxMapLayerManager::~TxMapLayerManager()
{
    m_vecDrawOrderLayers.clear();
    m_mapLayers.clear();
}

ITxLayerPtr TxMapLayerManager::GetLayer( int nLayerID )
{
    auto iter = m_mapLayers.find(nLayerID);
    if (iter != m_mapLayers.end()) {
        return iter->second;
    }

    return nullptr;
}

void TxMapLayerManager::RemoveLayer( int nLayerID )
{
    ITxLayerPtr spLayer; {
        auto iter = m_mapLayers.find(nLayerID);
        if (iter != m_mapLayers.end()) {
            spLayer = iter->second;
            m_mapLayers.erase(iter);
        } else {
            return;
        }
    }
    
    for (auto iter = m_vecDrawOrderLayers.begin(); iter != m_vecDrawOrderLayers.end(); ++iter) {
        if (spLayer == *iter){
            m_vecDrawOrderLayers.erase(iter);
            break;
        }
    }
}

int TxMapLayerManager::AddLayer( ITxLayerPtr spLayer, int nLayerID, int nLayerOrder/*=-1*/ )
{
    if (spLayer == nullptr)
        throw 1;

    if (nLayerID < 0) 
        throw 1;

    auto iter = m_mapLayers.find(nLayerID);
    if (iter != m_mapLayers.end()) {
        return -1;
    }    

    int nCount = (int)m_vecDrawOrderLayers.size();
    if (nLayerOrder < 0)
        nLayerOrder = nCount;

    //★ 추가
    m_mapLayers.insert(std::make_pair(nLayerID, spLayer));

    //★ 첫번째 또는 마지막 순서 보다 큰 경우
    if (nCount == 0 || nCount <= nLayerOrder) {
        m_vecDrawOrderLayers.push_back(spLayer);
    }
    else {
        std::vector<ITxLayerPtr> vecLayers;

        for (int i=0; i<nCount; i++) {
            if (i == nLayerOrder) {
                vecLayers.push_back(spLayer);
            }

            vecLayers.push_back(m_vecDrawOrderLayers[i]);
        }

        m_vecDrawOrderLayers = vecLayers;
    }    

    return nLayerID;
}

void TxMapLayerManager::MoveLayerOrder( int nLayerID, int nLayerOrder/*=0*/ )
{
    int nCount = (int)m_vecDrawOrderLayers.size();

    if (nLayerOrder < 0)
        nLayerOrder = 0;
    
    ITxLayerPtr spLayer = GetLayer(nLayerID);
    if (spLayer == nullptr) return;
        
    std::vector<ITxLayerPtr> vecLayers;
    
    if (nLayerOrder <= nCount -1) {
        for (int i=0; i<nCount; i++) {
            if (i == nLayerOrder) {
                vecLayers.push_back(spLayer);
            }

            if (spLayer != m_vecDrawOrderLayers[i])
                vecLayers.push_back(m_vecDrawOrderLayers[i]);
        }
    }
    else {
        for (int i=0; i<nCount; i++) {
            if (spLayer != m_vecDrawOrderLayers[i])
                vecLayers.push_back(m_vecDrawOrderLayers[i]);
        }
        vecLayers.push_back(spLayer);
    }

    m_vecDrawOrderLayers = vecLayers;
}

std::vector<int> TxMapLayerManager::LayerOrder() 
{
    std::vector<int> vecLayerID;
    for (size_t i=0; i<m_vecDrawOrderLayers.size(); i++) {
        ITxLayerPtr spLayer = m_vecDrawOrderLayers[i];

        for (auto iter=m_mapLayers.begin(); iter != m_mapLayers.end(); ++iter) {
            if (spLayer == iter->second) {
                vecLayerID.push_back(iter->first);
                break;
            }
        }
    }

    return vecLayerID;
}

void TxMapLayerManager::LayerOrderByID()
{
    m_vecDrawOrderLayers.clear();
    for(auto iter = m_mapLayers.begin(); iter != m_mapLayers.end(); ++iter) {
        ITxLayerPtr spLayer = iter->second;
        m_vecDrawOrderLayers.push_back(spLayer);
    }
}

int TxMapLayerManager::LayerCount()
{
    return (int)m_vecDrawOrderLayers.size();
}

int TxMapLayerManager::MaxLayerID()
{
    int nMaxID(0);
    for(auto iter = m_mapLayers.begin(); iter != m_mapLayers.end(); ++iter) {
        int nLayerID = iter->first;
        if (nLayerID > nMaxID) {
            nMaxID = nLayerID;
        }
    }

    return nMaxID;
}



