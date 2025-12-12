#include "stdafx.h"
#include "ITxFeatureLayer.h"
#include "TxSymbolPoint.h"
#include "TxSymbolLine.h"
#include "TxSymbolPolygon.h"

ITxFeatureLayer::ITxFeatureLayer( TxLayerInfoPtr spLayerInfo ) : ITxLayer(spLayerInfo)
{
    m_bUseSortDraw = true;
    m_bLabelFilter = true;

    EnumTxGeoType enGeoType = spLayerInfo->GeoType();
    switch (enGeoType)
    {
    case GeoPointType:
        m_spLayerSymbol = TxSymbolPoint::CreatePointSymbol();
        break;
    case GeoPolylineType:
        m_spLayerSymbol = TxSymbolLine::CreateLineSymbol();
        break;
    case GeoPolygonType:
        m_spLayerSymbol = TxSymbolPolygon::CreatePolygonSymbol();
        break;
    case GeoMPolygonType:
        m_spLayerSymbol = TxSymbolPolygon::CreatePolygonSymbol();
        break;
    default:
        break;
    }

    m_spLabelSymbol  = ITxLabelSymbolPtr(new ITxLabelSymbol());
    m_dLabelMaxScale = TxConst::MaxDbl();

    m_bEnableTooltip = true; {        
        m_bUseLabelMapTooltip = true; /*라벨정보를 툴팁으로 표현*/
    } 
 
    m_bDrawExpired = true;
}

ITxFeatureLayer::~ITxFeatureLayer()
{
}

bool ITxFeatureLayer::DrawExpired(const TxEnvelope& oDrawMapEnv, const TxEnvelope& oDrawScrEnv )
{
    if (m_bDrawExpired == true)
        return true;

    //★ m_bDrawExpired는 false 상태이다.
    if (m_oDrawMapEnv.Equal(oDrawMapEnv) == false || m_oDrawScrEnv.Equal(oDrawScrEnv) == false) {
        return true;
    }

    //★ 다시 그릴 필요가 없다.
    return m_bDrawExpired;
}


void ITxFeatureLayer::SetDrawExpire()
{
    m_bDrawExpired = true;
}

TxGdiplusBitmapPtr ITxFeatureLayer::GetLayerImage()
{
    return m_spLayerBitmap;
}

void ITxFeatureLayer::SetLayerImage( TxGdiplusBitmapPtr spBitmap, const TxEnvelope& oDrawMapEnv, const TxEnvelope& oDrawScrEnv )
{
    m_spLayerBitmap = spBitmap;

    m_oDrawMapEnv = oDrawMapEnv;
    m_oDrawScrEnv = oDrawScrEnv;

    m_bDrawExpired  = false;
}

void ITxFeatureLayer::LabelSymbol( ITxLabelSymbolPtr spLabelSymbol )
{
    m_spLabelSymbol = spLabelSymbol;
    SetDrawExpire();
}

void ITxFeatureLayer::ShowLabel( bool bShow )
{
    m_bsFlag[EnumFlagLabelDisplay] = bShow;
    SetDrawExpire();
}

bool ITxFeatureLayer::ShowLabel()
{
    return m_bsFlag[EnumFlagLabelDisplay];
}

void ITxFeatureLayer::LayerDispSymbol( ITxSymbolPtr spSymbol )
{
    m_spLayerSymbol = spSymbol;
    SetDrawExpire();
}

void ITxFeatureLayer::Symbol( ITxSymbolPtr spSymbol )
{
    LayerDispSymbol(spSymbol);
}

ITxSymbolPtr ITxFeatureLayer::Symbol()
{
    return m_spLayerSymbol;
}

void ITxFeatureLayer::ClearLabelScaleDisp()
{
    m_dLabelMaxScale = TxConst::MaxDbl();
    SetDrawExpire();
}

void ITxFeatureLayer::DrawLabelMaxScale( double dScale )
{
    m_dLabelMaxScale = dScale;
    SetDrawExpire();
}

double ITxFeatureLayer::DrawLabelMaxScale()
{
    return m_dLabelMaxScale;
}

void ITxFeatureLayer::FeatureSymbol( __int64 nxID, ITxSymbolPtr _spSymbol )
{
    ITxSymbolPtr& spSymbol = m_mapFeatureSymbol[nxID];
    spSymbol = _spSymbol;

    SetDrawExpire();
}

void ITxFeatureLayer::RemoveFeatureSymbol( __int64 nxID )
{
    auto iter = m_mapFeatureSymbol.find(nxID);
    if (iter != m_mapFeatureSymbol.end())
        m_mapFeatureSymbol.erase(iter);

    SetDrawExpire();
}

void ITxFeatureLayer::ClearFeatureSymbol()
{
    m_mapFeatureSymbol.clear();
    SetDrawExpire();
}

void ITxFeatureLayer::FeatureLabel( __int64 nxID, ITxLabelPtr _spLabel )
{
    ITxLabelPtr& spLabel = m_mapLabel[nxID];
    spLabel = _spLabel;

    SetDrawExpire();
}

void ITxFeatureLayer::ClearFeatureLabel()
{
    m_mapLabel.clear();

    SetDrawExpire();
}

void ITxFeatureLayer::RemoveFeatureLabel( __int64 nxID )
{
    auto iter = m_mapLabel.find(nxID);
    if (iter != m_mapLabel.end()) {
        m_mapLabel.erase(iter);
    }

    SetDrawExpire();
}

bool ITxFeatureLayer::IsScaleLabelDispMode()
{
    if (m_dLabelMaxScale != TxConst::MaxDbl())
        return true;

    return false;
}

void ITxFeatureLayer::FeatureExTooltip( __int64 nxID, ITxExtendTooltipPtr spTooltip )
{
    m_mapExTooltip[nxID] = spTooltip;
}

ITxExtendTooltipPtr ITxFeatureLayer::FeatureExTooltip( __int64 nxID )
{
    auto iter  = m_mapExTooltip.find(nxID);
    if ( iter != m_mapExTooltip.end())
        return iter->second;

    return nullptr;
}

void ITxFeatureLayer::ClearExTooltip()
{
    m_mapExTooltip.clear();
}

void ITxFeatureLayer::RemoveExTooltip( __int64 nxID )
{
    auto iter  = m_mapExTooltip.find(nxID);
    if ( iter != m_mapExTooltip.end())
        m_mapExTooltip.erase(iter);
}

void ITxFeatureLayer::FeatureExtendInfo( __int64 nxID, ITxExtendInfoPtr spInfo )
{
    m_mapExtendInfo[nxID] = spInfo;
}

void ITxFeatureLayer::ClearExtendInfo()
{
    m_mapExtendInfo.clear();
}

ITxExtendInfoPtr ITxFeatureLayer::FeatureExtendInfo( __int64 nxID)
{    
    auto iter  = m_mapExtendInfo.find(nxID); 
    if ( iter != m_mapExtendInfo.end()) {
        return iter->second;
    }

    return nullptr;
}

void ITxFeatureLayer::RemoveExtendInfo( __int64 nxID )
{
    auto iter  = m_mapExtendInfo.find(nxID);
    if ( iter != m_mapExtendInfo.end()) {
        m_mapExtendInfo.erase(iter);
    }
}

void ITxFeatureLayer::AddKeySet( CString strKey, __int64 nxTxID )
{
    std::set<__int64>& setTxID = m_mapKeySet[strKey];
    setTxID.insert(nxTxID);
}

void ITxFeatureLayer::AddKeySet( CString strKey, std::set<__int64>& setAddTxID )
{
    std::set<__int64>& setTxID = m_mapKeySet[strKey];
    for (auto iter = setAddTxID.begin(); iter != setAddTxID.end(); ++iter) {
        setTxID.insert(*iter);
    }
}

void ITxFeatureLayer::RemoveKeySet( CString strKey )
{
    auto iter = m_mapKeySet.find(strKey);
    if (iter != m_mapKeySet.end()) {
        m_mapKeySet.erase(iter);
    }
}

void ITxFeatureLayer::ReplaceKeySet( CString strKey, std::set<__int64>& setTxID )
{
    m_mapKeySet[strKey] = setTxID;
}

void ITxFeatureLayer::ClearKeySet( CString strKey )
{
    std::set<__int64>& setTxID = m_mapKeySet[strKey]; 
    setTxID.clear();
}

void ITxFeatureLayer::ClearKeySet()
{
    m_mapKeySet.clear();
}

std::set<__int64> ITxFeatureLayer::FindKeySet( CString strKey )
{
    auto iter = m_mapKeySet.find(strKey);
    if (iter != m_mapKeySet.end())
        return iter->second;

    std::set<__int64> setBlank;
    return setBlank;
}


