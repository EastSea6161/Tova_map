#include "stdafx.h"
#include "KThemeDrawUserLine.h"
#include "Project.h"


KThemeDrawUserLine::KThemeDrawUserLine(IMapView* a_mapRequest, int nLayerID) : IThemeLayer(a_mapRequest)
{
    m_nLayerID = nLayerID;
}

KThemeDrawUserLine::~KThemeDrawUserLine(void)
{
    if (m_bDrawing == false)
        return;
    if (m_pIMapView == nullptr)
        return;

    m_pIMapView->MapRemoveLayer(m_nLayerID);
}


KEMThemeLayer KThemeDrawUserLine::GetType()
{
    return ThemeLayerDrawUserLink;
}


void KThemeDrawUserLine::Draw( std::vector<TUserLineInfo>& vecUserLine, bool bShowLabel )
{
    if (m_pIMapView == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID);
    if (spLayer == nullptr)
        return;

    //¡Ú Draw
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;

    __int64 TxID(1000);
    for (size_t i=0; i<vecUserLine.size(); i++) {
        TxID++;

        TUserLineInfo& oDrawInfo = vecUserLine[i];
        TxPolylinePtr  spPline   = nullptr; {
            std::vector<TMapPoint>& vecPoint = oDrawInfo.TPoints;
            std::vector<TxPoint>    vecTxPt; 
            for (size_t i=0; i<vecPoint.size(); i++) {
                TMapPoint& oMapPt = vecPoint[i];
                vecTxPt.push_back(TxPoint(oMapPt.Tx, oMapPt.Ty));
            }

            if (vecTxPt.size() >= 2) {
                spPline = TxPolylinePtr(new TxPolyline(vecTxPt));
            }
        }
        
        if (spPline == nullptr)
            continue;

        TxFeaturePtr spFeature (new TxFeature(TxID, spPline));

        Gdiplus::Color clrLine; {
            COLORREF clrRgb = oDrawInfo.TColorRGB;
            clrLine = Gdiplus::Color(oDrawInfo.TAlpha, GetRValue(clrRgb), GetGValue(clrRgb), GetBValue(clrRgb));
            //clrLine.SetFromCOLORREF(oDrawInfo.TColorRGB);
        }

        TxSymbolLinePtr spSymbol(new TxSymbolLine(oDrawInfo.TWidth, clrLine, true));
        ITxLabelPtr     spLabel  = TxLabelPtr(new TxLabel(oDrawInfo.TLabel));

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
        spLayer->LabelSymbol(spLabelSymbol);        
    }

    spLayer->ShowLabel(bShowLabel);
    //spLayer->SetDrawMaxScale(dDrawScale);

    /*¡Ú*/ m_bDrawing = true;
}

void KThemeDrawUserLine::ShowLabel( bool bShowLabel/*=true*/ )
{
    if (m_bDrawing == false)
        return;

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(m_nLayerID);
    if (spLayer == nullptr)
        return;

    spLayer->ShowLabel(bShowLabel);
}


