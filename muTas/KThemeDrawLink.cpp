#include "StdAfx.h"
#include "Project.h"
#include "KThemeDrawLink.h"

#include "LayerXmlRenderer.h"

KThemeDrawLink::KThemeDrawLink(IMapView* pMapView, int nLayerId) : IThemeLayer(pMapView)
{
    m_nLayerID = nLayerId;
    m_bLoadLinkGeo = false;
}

KThemeDrawLink::~KThemeDrawLink(void)
{
    if (m_bDrawing == false)
        return;

    m_pIMapView->MapRemoveLayer(m_nLayerID);
}

void KThemeDrawLink::Clear()
{
    if (m_pIMapView == nullptr)
        return;

    m_pIMapView->MapRemoveLayer(m_nLayerID);
}

KEMThemeLayer KThemeDrawLink::GetType()
{
    return ThemeLayerDrawLink;
}


void KThemeDrawLink::Draw( std::vector<TDrawLinkInfo> &vecDrawLinkInfo, bool bShowLabel, bool bSort, bool bLabelFill )
{   
    if (m_pIMapView == nullptr)
        return;
    if (vecDrawLinkInfo.size() > 3000)
        DrawLargeSizeUserLayer(vecDrawLinkInfo, bShowLabel, bSort, bLabelFill);
    else
        DrawSmallSizeUserLayer(vecDrawLinkInfo, bShowLabel, bSort, bLabelFill);
}

void KThemeDrawLink::ShowLabel( bool bShowLabel)
{
    if (m_bDrawing == false)
        return;

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(m_nLayerID);

    if (spLayer == nullptr)
        return;

    spLayer->ShowLabel(bShowLabel);
}

void KThemeDrawLink::LoadLinkGeo()
{
    if (m_bLoadLinkGeo)
        return;

    ITxFeatureLayerPtr spLinkLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLyr == nullptr)
        return;

    spLinkLyr->GetGeometry(m_mapLinkGeo);      
    m_bLoadLinkGeo = true;
}

void KThemeDrawLink::DrawLargeSizeUserLayer( std::vector<TDrawLinkInfo> &vecDrawLinkInfo, bool bShowLabel, bool bSort, bool bLabelFill )
{
    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID); 
    if (spLayer == nullptr) {
        return;
    } 
        
    LoadLinkGeo();

    if (m_mapLinkGeo.size() == 0)
        return;
        
    TxOffsetInfoPtr spOffset = TxOffsetInfoPtr(new TxOffsetInfo(KMapConfig::Offset()));

    //★ 드로잉
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;
    __int64 TxID(1000);

    for (size_t i=0; i<vecDrawLinkInfo.size(); i++) {
        TDrawLinkInfo& oDrawInfo = vecDrawLinkInfo[i];
        __int64 nxLinkID = oDrawInfo.TLinkID;
        Gdiplus::Color clrLine; {
            COLORREF clrRgb = oDrawInfo.TColorRGB;
            clrLine = Gdiplus::Color(oDrawInfo.TAlpha, GetRValue(clrRgb), GetGValue(clrRgb), GetBValue(clrRgb));
            //clrLine.SetFromCOLORREF(oDrawInfo.TColorRGB);
        }

        auto iFind = m_mapLinkGeo.find(nxLinkID);
        if (iFind == m_mapLinkGeo.end())
            continue;

        ITxGeometryPtr spGeometry = iFind->second;
        if (spGeometry == nullptr)
            continue;

        TxSymbolLinePtr spSymbol(new TxSymbolLine(oDrawInfo.TWidth, clrLine)); {
            spSymbol->SetOffset(spOffset);
        }
        
        TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oDrawInfo.TLabel));

        TxID++;
        vecFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spGeometry)));
        vecDrawInfo.push_back(std::make_tuple(TxID, spSymbol, spLabel));
    }

    spLayer->Insert(vecFeature);
    for (size_t i=0; i<vecDrawInfo.size(); i++) {
        auto& oInfo = vecDrawInfo[i];
        __int64      nxID     = std::get<0>(oInfo);
        ITxSymbolPtr spSymbol = std::get<1>(oInfo);
        ITxLabelPtr  spLabel  = std::get<2>(oInfo);

        spLayer->FeatureSymbol(nxID, spSymbol);
        spLayer->FeatureLabel (nxID, spLabel);
    }

    //★ Label 정보
    ITxLabelSymbolPtr spLabelSymbol(new ITxLabelSymbol(9.0, Gdiplus::Color::Black)); {
        spLabelSymbol->BackgoundFill(bLabelFill);
        spLayer->LabelSymbol(spLabelSymbol);        
    }

    spLayer->ShowLabel(bShowLabel);

    //★ 라인 두께 Sorting
    spLayer->UseSizeSortDraw(bSort);

    m_bDrawing = true;
}

void KThemeDrawLink::DrawSmallSizeUserLayer( std::vector<TDrawLinkInfo> &vecDrawLinkInfo, bool bShowLabel, bool bSort, bool bLabelFill )
{
    ITxFeatureLayerPtr spLinkLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLyr == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID); 
    if (spLayer == nullptr) {
        return;
    } 
    
    TxOffsetInfoPtr spOffset = TxOffsetInfoPtr(new TxOffsetInfo(KMapConfig::Offset()));

    //★ 드로잉
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;
    __int64 TxID(1000);

    for (size_t i=0; i<vecDrawLinkInfo.size(); i++) {
        TDrawLinkInfo& oDrawInfo = vecDrawLinkInfo[i];
        __int64 nxLinkID = oDrawInfo.TLinkID;
        Gdiplus::Color clrLine; {
            COLORREF clrRgb = oDrawInfo.TColorRGB;
            clrLine = Gdiplus::Color(oDrawInfo.TAlpha, GetRValue(clrRgb), GetGValue(clrRgb), GetBValue(clrRgb));
            //clrLine.SetFromCOLORREF(oDrawInfo.TColorRGB);
        }
        
        ITxGeometryPtr spGeometry = spLinkLyr->GetGeometry(nxLinkID);
        if (spGeometry == nullptr)
            continue;

        TxSymbolLinePtr spSymbol(new TxSymbolLine(oDrawInfo.TWidth, clrLine)); {
            spSymbol->SetOffset(spOffset);
        }

        TxLabelPtr spLabel = TxLabelPtr(new TxLabel(oDrawInfo.TLabel));

        TxID++;
        vecFeature.push_back(TxFeaturePtr(new TxFeature(TxID, spGeometry)));
        vecDrawInfo.push_back(std::make_tuple(TxID, spSymbol, spLabel));
    }

    spLayer->Insert(vecFeature);
    for (size_t i=0; i<vecDrawInfo.size(); i++) {
        auto& oInfo = vecDrawInfo[i];
        __int64      nxID     = std::get<0>(oInfo);
        ITxSymbolPtr spSymbol = std::get<1>(oInfo);
        ITxLabelPtr  spLabel  = std::get<2>(oInfo);

        spLayer->FeatureSymbol(nxID, spSymbol);
        spLayer->FeatureLabel (nxID, spLabel);
    }

    //★ Label 정보
    ITxLabelSymbolPtr spLabelSymbol(new ITxLabelSymbol(9.0, Gdiplus::Color::Black)); {
        spLabelSymbol->BackgoundFill(bLabelFill);
        spLayer->LabelSymbol(spLabelSymbol);        
    }

    spLayer->ShowLabel(bShowLabel);

    //★ 라인 두께 Sorting
    spLayer->UseSizeSortDraw(bSort);

    m_bDrawing = true;
}

