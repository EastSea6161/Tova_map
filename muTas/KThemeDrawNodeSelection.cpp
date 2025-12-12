#include "StdAfx.h"
#include "KThemeDrawNodeSelection.h"


KThemeDrawNodeSelection::KThemeDrawNodeSelection(IMapView* a_mapRequest) : IThemeLayer(a_mapRequest)
{
    m_nLayerID = KLayerID::KThemeDrawNodeSelection();
}

KThemeDrawNodeSelection::~KThemeDrawNodeSelection(void)
{
    m_pIMapView->MapRemoveLayer(m_nLayerID);
}

KEMThemeLayer KThemeDrawNodeSelection::GetType()
{
    return ThemeLayerDrawNodeSelection;
}

void KThemeDrawNodeSelection::Draw(std::vector<TDrawNodeInfo>& vecDrawNodeInfo, bool bShowLabel, double dDrawScale)
{
    ITxFeatureLayerPtr spNodeLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    if (spNodeLayer == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID, true);    
    
    //¡Ú µå·ÎÀ×
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;

    __int64 TxID(1000);
    for (size_t i=0; i<vecDrawNodeInfo.size(); i++) {
        TxID++;
        TDrawNodeInfo& oDrawInfo = vecDrawNodeInfo[i];
        __int64 nxID = oDrawInfo.TNodeID;
        
        ITxGeometryPtr spGeometry = spNodeLayer->GetGeometry(nxID);
        if (spGeometry == nullptr)
            continue;

        TxPointPtr spPt; {            
            spPt = std::dynamic_pointer_cast<TxPoint>(spGeometry);
            if (spPt == nullptr)
                continue;
        }

        Gdiplus::Color clrPoint; {
            clrPoint.SetFromCOLORREF(oDrawInfo.TColorRGB);
        }

        ITxSymbolPtr spSymbol = TxSymbolPoint::CreatePointSymbol(oDrawInfo.TSymbol, oDrawInfo.TSize, clrPoint);
        ITxLabelPtr  spLabel  = TxLabelPtr(new TxLabel(oDrawInfo.TLabel));

        vecFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spPt)));
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
        spLayer->LabelSymbol(spLabelSymbol);        
    }

    spLayer->ShowLabel(bShowLabel);
    spLayer->SetDrawMaxScale(dDrawScale);
}

void KThemeDrawNodeSelection::ShowLabel( bool a_bShowLabel/*=true*/ )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(m_nLayerID);
    if (spLayer != nullptr)
        spLayer->ShowLabel(a_bShowLabel);
}
