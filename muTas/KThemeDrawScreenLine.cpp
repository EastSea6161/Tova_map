#include "StdAfx.h"
#include "KThemeDrawScreenLine.h"

#include "Target.h"
#include "BulkDBaseScreenCordon.h"

KThemeDrawScreenLine::KThemeDrawScreenLine(IMapView* a_mapRequest) : IThemeLayer(a_mapRequest)
{
    Initialize();
}

void KThemeDrawScreenLine::Initialize()
{
    m_spLayer = m_pIMapView->AddUserLayer(KLayerID::KScreenDrawScreenLine(), true);
    ITxLabelSymbolPtr spLabelSymbol = ITxLabelSymbolPtr(new ITxLabelSymbol());
    spLabelSymbol->BackgoundFill(true);
    m_spLayer->LabelSymbol(spLabelSymbol);
    m_spLayer->ShowLabel(true);
}

KThemeDrawScreenLine::~KThemeDrawScreenLine(void)
{
    m_pIMapView->MapRemoveLayer(KLayerID::KScreenDrawScreenLine());
}

KEMThemeLayer KThemeDrawScreenLine::GetType()
{
    return ThemeLayerDrawScreenLine;
}

void KThemeDrawScreenLine::AddDraw( KTarget* a_pTarget, Integer nxID, COLORREF a_rgbColor )
{
    if (m_spLayer == nullptr)
        return;

    auto iterF = m_setKey.find(nxID);
    if (iterF != m_setKey.end())
        return;

    CString strName;
    std::vector<TDrawScreenCordonXY> vecPoint;
    bool bResult = KBulkDBaseScreenCordon::ScreenQuery(a_pTarget, nxID, strName, vecPoint);

    if (bResult == false)
        return;

    std::vector<TxPoint> vecPt;
    for (size_t i=0; i<vecPoint.size(); i++) {
        TDrawScreenCordonXY& oPt = vecPoint[i];
        vecPt.push_back(TxPoint(oPt.Tx, oPt.Ty));
    }
    
    Gdiplus::Color oGdiColor; {
        oGdiColor.SetFromCOLORREF(a_rgbColor);
    }
    
    TxPolylinePtr spPLine   = TxPolylinePtr(new TxPolyline(vecPt));
    TxFeaturePtr  spFeature = TxFeaturePtr(new TxFeature(nxID, spPLine)); 
    
    m_spLayer->Insert(spFeature);

    TxSymbolLinePtr spSymbol = TxSymbolLine::CreateLineSymbol(2, oGdiColor); {
        m_spLayer->FeatureSymbol(nxID, spSymbol);
    }
    
    ITxLabelPtr spLabel = ITxLabelPtr(new TxLabel(strName));
    m_spLayer->FeatureLabel (nxID, spLabel);

    m_setKey.insert(nxID);
}

void KThemeDrawScreenLine::RemoveDraw( Integer a_nxScreenID )
{
    if (m_spLayer == nullptr)
        return;

    m_spLayer->Delete(a_nxScreenID);
    auto iterF = m_setKey.find(a_nxScreenID);
    if (iterF != m_setKey.end())
        m_setKey.erase(iterF);
}

void KThemeDrawScreenLine::RemoveAllDraw()
{
    m_spLayer->Delete();
    m_setKey.clear();
}

