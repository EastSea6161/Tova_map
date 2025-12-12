#include "StdAfx.h"
#include "KThemeDrawCordonLine.h"

#include "Target.h"
#include "BulkDBaseScreenCordon.h"

KThemeDrawCordonLine::KThemeDrawCordonLine(IMapView* a_mapRequest) : IThemeLayer(a_mapRequest)
{
    Initialize();
}


void KThemeDrawCordonLine::Initialize()
{
    m_spLayer = m_pIMapView->AddUserLayer(KLayerID::KScreenDrawCordonLine(), true);
    ITxLabelSymbolPtr spLabelSymbol = ITxLabelSymbolPtr(new ITxLabelSymbol());
    spLabelSymbol->BackgoundFill(true);
    m_spLayer->LabelSymbol(spLabelSymbol);
    m_spLayer->ShowLabel(true);
}

KThemeDrawCordonLine::~KThemeDrawCordonLine(void)
{
    m_pIMapView->MapRemoveLayer(KLayerID::KScreenDrawCordonLine());
}

KEMThemeLayer KThemeDrawCordonLine::GetType()
{
    return ThemeLayerDrawCordonLine;
}

void KThemeDrawCordonLine::AddDraw( KTarget* a_pTarget, Integer nxID, COLORREF a_rgbColor )
{
    if (m_spLayer == nullptr)
        return;

    auto iterF = m_setKey.find(nxID);
    if (iterF != m_setKey.end())
        return;

    CString strName;
    std::vector<TDrawScreenCordonXY> vecPoint;
    bool bResult = KBulkDBaseScreenCordon::CordonQuery(a_pTarget, nxID, strName, vecPoint);

    if (bResult == false)
        return;
    
    // Æú¸®°ï »ý¼º
    size_t nxVertexCount = vecPoint.size();
    if (nxVertexCount < 1) {
        return;
    }

    std::vector<TxPoint> vecPt;
    for (size_t i=0; i<nxVertexCount; i++) {
        vecPt.push_back(TxPoint(vecPoint[i].Tx, vecPoint[i].Ty));
    }
    
    TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPt));
    TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxID, spPolygon)); 

    m_spLayer->Insert(spFeature);

    Gdiplus::Color oGdiColor; {
        oGdiColor.SetFromCOLORREF(a_rgbColor);
    }

    TxSymbolPolygonPtr spSymbol = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(0, 255, 255, 255), oGdiColor, 2, true); {
        TxHatchInfoPtr spHatch  = TxHatchInfoPtr(new TxHatchInfo(/*HatchStyleBackwardDiagonal*/HatchStyle05Percent, oGdiColor));       
        spSymbol->SetHatch(spHatch);
        m_spLayer->FeatureSymbol(nxID, spSymbol);
    }

    ITxLabelPtr spLabel = ITxLabelPtr(new TxLabel(strName));
    m_spLayer->FeatureLabel (nxID, spLabel);
    m_setKey.insert(nxID);
}

void KThemeDrawCordonLine::RemoveDraw( Integer a_nxScreenID )
{
    if (m_spLayer == nullptr)
        return;

    m_spLayer->Delete(a_nxScreenID);
    auto iterF = m_setKey.find(a_nxScreenID);
    if (iterF != m_setKey.end())
        m_setKey.erase(iterF);
}

void KThemeDrawCordonLine::RemoveAllDraw()
{
    m_spLayer->Delete();
    m_setKey.clear();
}

