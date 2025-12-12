#include "StdAfx.h"
#include "Project.h"
#include "KThemeDrawLinkSelection.h"

KThemeDrawLinkSelection::KThemeDrawLinkSelection(IMapView* pMapView) : IThemeLayer(pMapView)
{
    m_nLayerID = KLayerID::KScreenDrawLinkSelection();
}

KThemeDrawLinkSelection::~KThemeDrawLinkSelection(void)
{
    m_pIMapView->MapRemoveLayer(m_nLayerID);
}

void KThemeDrawLinkSelection::ShowLabel( bool a_bShowLabel/*=true*/ )
{
    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(m_nLayerID);
    if (spLayer != nullptr)
        spLayer->ShowLabel(a_bShowLabel);
}

KEMThemeLayer KThemeDrawLinkSelection::GetType()
{
    return ThemeLayerDrawLinkSelection;
}

void KThemeDrawLinkSelection::Draw( std::vector<TDrawLinkInfo> &vecDrawLinkInfo, bool bShowLabel )
{
    ITxFeatureLayerPtr spLinkLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLayer == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID, true);
    
    //¡Ú µå·ÎÀ×
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;

    __int64 TxID(1000);
    for (size_t i=0; i<vecDrawLinkInfo.size(); i++) {
        TxID++;
        TDrawLinkInfo& oDrawInfo = vecDrawLinkInfo[i];
        __int64 nxID = oDrawInfo.TLinkID;

        ITxGeometryPtr spGeometry = spLinkLayer->GetGeometry(nxID);

        TxPolylinePtr spPline; {            
            spPline = std::dynamic_pointer_cast<TxPolyline>(spGeometry);
        }

        if (spPline == nullptr)
            continue;

        Gdiplus::Color clrLine; {
            clrLine.SetFromCOLORREF(oDrawInfo.TColorRGB);
        }

        ITxSymbolPtr spSymbol = TxSymbolLine::CreateOffsetLineSymbol(oDrawInfo.TWidth, clrLine, 1.0);
        ITxLabelPtr  spLabel  = TxLabelPtr(new TxLabel(oDrawInfo.TLabel));

        vecFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spPline)));
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
}
