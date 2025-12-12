#include "StdAfx.h"
#include "Project.h"
#include "KThemeDrawHighwayShortestPath.h"


KThemeDrawHighwayShortestPath::KThemeDrawHighwayShortestPath(IMapView* a_mapRequest) : IThemeLayer(a_mapRequest)
{
    m_spLayer = m_pIMapView->AddUserLayer(KLayerID::KThemeDrawHighwayShortestPath(), true);
    if (m_spLayer != nullptr) {
        ITxLabelSymbolPtr spLabelSym = m_spLayer->LabelSymbol();
        //spLabelSym->BackgoundFill(true);
    }
    TxID = 1;
}

KThemeDrawHighwayShortestPath::~KThemeDrawHighwayShortestPath(void)
{
    m_pIMapView->MapRemoveLayer(KLayerID::KThemeDrawHighwayShortestPath());
}

KEMThemeLayer KThemeDrawHighwayShortestPath::GetType()
{
    return ThemeLayerDrawHighwayShortestPath;
}

void KThemeDrawHighwayShortestPath::Draw(std::vector<TPathInfo>& vecPathInfo)
{
    if (m_spLayer == nullptr)
        return;

    m_spLayer->Delete();

    for (size_t i=0; i<vecPathInfo.size(); i++) {
        TPathInfo& oInfo = vecPathInfo[i];
        Draw(oInfo, (int) i+1);
    }
}

void KThemeDrawHighwayShortestPath::Draw( TPathInfo& oPathInfo, int nOrder )
{
    ITxFeatureLayerPtr spNodeLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    ITxFeatureLayerPtr spLinkLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());

    __int64 nxStartID = oPathInfo.nxNodeIdStart;
    __int64 nxEndID   = oPathInfo.nxNodeIdEnd;

    Gdiplus::Color clrLine; {
        clrLine.SetFromCOLORREF(oPathInfo.color);
    }

    std::vector<__int64>& vecLink = oPathInfo.vecLink;

    TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(2, clrLine, 1); {
        spLineSym->DrawOrderOption(nOrder);
    }

    for (size_t i=0; i<vecLink.size(); i++) {
        __int64 nxLinkID = vecLink[i];
        TxFeaturePtr spFeature = spLinkLyr->GetFeature(nxLinkID);
        if (spFeature != nullptr) {
            TxID++;
            m_spLayer->Insert(TxFeaturePtr(new TxFeature(TxID, spFeature->Geometry())));
            m_spLayer->FeatureSymbol(TxID, spLineSym);
        }
    }
        
    TxFeaturePtr spSFeature = spNodeLyr->GetFeature(nxStartID);
    if (spSFeature != nullptr) {
        TxSymbolPointCirclePtr spSymbol = TxSymbolPointCircle::CreatePointSymbol(12, Gdiplus::Color::Blue, Gdiplus::Color::Black, 1);
        m_spLayer->Insert(spSFeature);
        m_spLayer->FeatureSymbol(nxStartID, spSymbol);
    }

    TxFeaturePtr spEFeature = spNodeLyr->GetFeature(nxEndID);
    if (spEFeature != nullptr) {
        TxSymbolPointCirclePtr spSymbol = TxSymbolPointCircle::CreatePointSymbol(12, Gdiplus::Color::Red, Gdiplus::Color::Black, 1);
       
        m_spLayer->Insert(spEFeature);
        m_spLayer->FeatureSymbol(nxEndID, spSymbol);

        CString strLabel(_T("")); {
            strLabel.Format(_T("%I64d"), nxEndID);
        }
        ITxLabelPtr spLabel = ITxLabelPtr(new TxLabel(strLabel));
        m_spLayer->FeatureLabel(nxEndID, spLabel);
    }
}
