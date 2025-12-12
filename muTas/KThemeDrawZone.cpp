#include "StdAfx.h"
#include "KThemeDrawZone.h"
#include "LayerXmlRenderer.h"

KThemeDrawZone::KThemeDrawZone(IMapView* pMapView) : IThemeLayer(pMapView)
{
}

KThemeDrawZone::~KThemeDrawZone(void)
{
}

KEMThemeLayer KThemeDrawZone::GetType()
{
    return ThemeLayerDrawZone;
}

void KThemeDrawZone::Draw( std::vector<TDrawZoneInfo>& vecDrawZoneInfo, bool bShowLabel )
{
    if (m_pIMapView == nullptr)
        return;

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return;

    TZoneDispInfo oInfo; {
        KLayerXmlRenderer oLayerXmlRenderer(m_pIMapView->GetTarget());
        oLayerXmlRenderer.LoadZoneProperty(oInfo);
    }

    //★ 기존 레이어의 심볼 정리       
    spLayer->LayerOn(true);
    ITxSymbolPtr spLayerSymbol = spLayer->LayerDispSymbol(); {
        spLayerSymbol->Show(false);
    }
    spLayer->ClearFeatureSymbol();
    spLayer->ClearFeatureLabel();

    Gdiplus::Color clrLine; {
        clrLine.SetFromCOLORREF(oInfo.TLineColor);
    }

    //★ Draw
    for (size_t i=0; i<vecDrawZoneInfo.size(); i++) {
        TDrawZoneInfo oZoneInfo = vecDrawZoneInfo[i];

        __int64 nxID     = oZoneInfo.TZoneID;
        CString strLabel = oZoneInfo.TLabel;
        Gdiplus::Color clrFill; {
            clrFill.SetFromCOLORREF(oZoneInfo.TColorRGB);
        }

        TxSymbolPolygonPtr spSymbol = TxSymbolPolygonPtr(new TxSymbolPolygon(clrFill, clrLine, oInfo.TLineWidth));
        spSymbol->Fill(true);

        spLayer->FeatureSymbol(nxID, spSymbol);
        if (strLabel.GetLength() > 0) {
            TxLabelPtr spLabel = TxLabelPtr(new TxLabel(strLabel));
            spLayer->FeatureLabel(nxID, spLabel);
        }        
    }

    spLayer->ClearScaleDisp();
    spLayer->ClearLabelScaleDisp();
    spLayer->ShowLabel(bShowLabel);

	m_bDrawing = true;
}

void KThemeDrawZone::ShowLabel( bool bShowLabel/*=true*/ )
{
    if (m_bDrawing == false)
        return;

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());
    if (spLayer == nullptr)
        return;

    spLayer->ShowLabel(bShowLabel);
}
