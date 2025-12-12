#include "StdAfx.h"
#include "KThemeDrawNode.h"


KThemeDrawNode::KThemeDrawNode(IMapView* pMapView, int nLayerID) : IThemeLayer(pMapView)
{
    m_nLayerID = nLayerID;
    Initialize();
}

void KThemeDrawNode::Initialize()
{
    //★ 노드레이어-> Geometry 정보 조회
    {
        ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
        if (spLayer == nullptr)
            return;

        spLayer->GetGeometry(m_mapNodeGeo);        
    }
}

KThemeDrawNode::~KThemeDrawNode(void)
{    
    Clear();
}

KEMThemeLayer KThemeDrawNode::GetType()
{
    return ThemeLayerDrawNode;
}

void KThemeDrawNode::Clear()
{
    m_pIMapView->MapRemoveLayer(m_nLayerID);
}

void KThemeDrawNode::Draw(std::vector<TDrawNodeInfo>& vecDrawNodeInfo, bool bShowLabel /*= false*/, double dDrawScale/*=999999999.0*/ )
{
    DrawUserLayer(vecDrawNodeInfo, bShowLabel, dDrawScale);
}

void KThemeDrawNode::DrawUserLayer(std::vector<TDrawNodeInfo>& vecDrawNodeInfo, bool bShowLabel, double dDrawScale)
{   
    if (m_pIMapView == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->AddUserLayer(m_nLayerID, true); 
    if (spLayer == nullptr) {
        return;
    } 

    if (vecDrawNodeInfo.size() == 0)
        return;
        
    if (m_mapNodeGeo.size() == 0)
        return;

    //★ 레이어 디폴트 심볼 세팅
    spLayer->LayerDispSymbol(TxSymbolHide::CreateHideSymbol());

    //★ 드로잉
    std::vector<TxFeaturePtr> vecFeature;
    std::vector<std::tuple<__int64, ITxSymbolPtr, ITxLabelPtr>> vecDrawInfo;

    for (size_t i=0; i<vecDrawNodeInfo.size(); i++) {
        TDrawNodeInfo& oInfo = vecDrawNodeInfo[i]; 
        
        __int64 nxID = oInfo.TNodeID;
        auto iFind = m_mapNodeGeo.find(nxID);
        if (iFind == m_mapNodeGeo.end()) {
            continue;
        }

        ITxGeometryPtr spGeometry = iFind->second;
        if (spGeometry == nullptr)
            continue;

        TxFeaturePtr spFeature (new TxFeature(nxID, spGeometry));

        Gdiplus::Color clrMarker(oInfo.TColorAlpha, (BYTE)GetRValue(oInfo.TColorRGB), (BYTE)GetGValue(oInfo.TColorRGB), (BYTE)GetBValue(oInfo.TColorRGB)); 
        ITxSymbolPtr spSymbol = TxSymbolPoint::CreatePointSymbol(oInfo.TSymbol, oInfo.TSize, clrMarker);

        ITxLabelPtr   spLabel = TxLabelPtr(new TxLabel(oInfo.TLabel));

        vecFeature.push_back(spFeature);
        vecDrawInfo.push_back(std::make_tuple(nxID, spSymbol, spLabel));
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

    //★ Label 정보
    ITxLabelSymbolPtr spLabelSymbol(new ITxLabelSymbol(9.0, Gdiplus::Color::Black)); {
        spLayer->LabelSymbol(spLabelSymbol);        
    }
    
    spLayer->ShowLabel(bShowLabel);
    spLayer->SetDrawMaxScale(dDrawScale);

    /*★*/ m_bDrawing = true;
}

void KThemeDrawNode::ShowLabel( bool bShowLabel/*=true*/ )
{
    if (m_bDrawing == false)
        return;

    ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(m_nLayerID);
    if (spLayer == nullptr)
        return;

    spLayer->ShowLabel(bShowLabel);
}
