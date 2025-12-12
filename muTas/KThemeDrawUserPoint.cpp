#include "stdafx.h"
#include "KThemeDrawUserPoint.h"
#include "TUserGeometryInfo.h"


KThemeDrawUserPoint::KThemeDrawUserPoint( IMapView* a_mapRequest, int nLayerID ) : IThemeLayer(a_mapRequest)
{
    m_nLayerID = nLayerID;
}

KThemeDrawUserPoint::~KThemeDrawUserPoint( void )
{
    if (m_bDrawing == false)
        return;
    if (m_pIMapView == nullptr)
        return;

    m_pIMapView->MapRemoveLayer(m_nLayerID);
}

KEMThemeLayer KThemeDrawUserPoint::GetType()
{
    return ThemeLayerDrawUserPoint;
}

void KThemeDrawUserPoint::Draw( std::vector<TUserPointInfo>& vecUserPoint, bool bShowLabel /*= false*/ )
{
    if (m_pIMapView == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID); 
    if (spLayer == nullptr) {
        return;
    } 

    if (vecUserPoint.size() == 0)
        return;
    
    //¡Ú µå·ÎÀ×
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;

    __int64 TxID(1000);
    for (size_t i=0; i<vecUserPoint.size(); i++) {
        TxID++;

        TUserPointInfo& oInfo = vecUserPoint[i]; 
        TxPointPtr spPoint(new TxPoint(oInfo.Tx, oInfo.Ty));

        TxFeaturePtr spFeature (new TxFeature(TxID, spPoint));

        Gdiplus::Color clrMarker; {
            clrMarker.SetFromCOLORREF(oInfo.TColorRGB);
        }
        ITxSymbolPtr spSymbol = TxSymbolPoint::CreatePointSymbol(oInfo.TMarker, oInfo.TSize, clrMarker);
        ITxLabelPtr  spLabel  = TxLabelPtr(new TxLabel(oInfo.TLabel));

        vecFeature.push_back(spFeature);
        vecDrawInfo.push_back(std::make_tuple(TxID, spSymbol, spLabel));
    }

    if (vecFeature.size() == 0)
        return;

    spLayer->Insert(vecFeature);
    for (size_t i=0; i<vecDrawInfo.size(); i++) {
        auto& oInfo = vecDrawInfo[i];
        __int64      nxID     = std::get<0>(oInfo);
        ITxSymbolPtr spSymbol = std::get<1>(oInfo);
        ITxLabelPtr  spLabel  = std::get<2>(oInfo);

        spLayer->FeatureSymbol(nxID, spSymbol);
        spLayer->FeatureLabel (nxID, spLabel);
    }

    //¡Ú Label Á¤º¸
    ITxLabelSymbolPtr spLabelSymbol(new ITxLabelSymbol(9.0, Gdiplus::Color::Black)); {
        spLabelSymbol->BackgoundFill(true);
        spLayer->LabelSymbol(spLabelSymbol);        
    }

    spLayer->ShowLabel(bShowLabel);
    //spLayer->SetDrawMaxScale(dDrawScale);

    /*¡Ú*/ m_bDrawing = true;
}

void KThemeDrawUserPoint::ShowLabel( bool bShowLabel/*=true*/ )
{
    if (m_bDrawing == false)
        return;

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(m_nLayerID);
    if (spLayer == nullptr)
        return;

    spLayer->ShowLabel(bShowLabel);
}
