#include "stdafx.h"
#include "Project.h"
#include "KThemeDrawZoneAccessBoundary.h"

KThemeDrawZoneAccessBoundary::KThemeDrawZoneAccessBoundary( IMapView* a_mapRequest )
                              :IThemeLayer(a_mapRequest)
{
    Initialize();
}

void KThemeDrawZoneAccessBoundary::Initialize()
{
    m_spLayer = m_pIMapView->AddUserLayer(KLayerID::KThemeDrawZoneAccessBoundary(), true);
    if (m_spLayer != nullptr) {
        ITxLabelSymbolPtr spLabelSym = m_spLayer->LabelSymbol();
    }

    TxID = 1;
}

KThemeDrawZoneAccessBoundary::~KThemeDrawZoneAccessBoundary()
{
    m_pIMapView->MapRemoveLayer(KLayerID::KThemeDrawZoneAccessBoundary());
}


void KThemeDrawZoneAccessBoundary::Draw( std::vector<TDrawZoneAccess>& vecZoneAccess )
{
    if (m_spLayer == nullptr)
        return;

    ITxFeatureLayerPtr m_spNodeLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    ITxFeatureLayerPtr m_spZoneLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Zone());

    for (size_t i=0; i<vecZoneAccess.size(); i++)
    {
        TDrawZoneAccess& oInfo = vecZoneAccess[i];
        __int64 nxNodeId = oInfo.TNodeID;

        ITxGeometryPtr spNodeGeo = m_spNodeLyr->GetGeometry(nxNodeId);
        if (spNodeGeo == nullptr)
            continue;

        TxPointPtr   spNodePt  = std::dynamic_pointer_cast<TxPoint>(spNodeGeo);
        TxPolygonPtr spPolyRed = TxSpatialOper::Buffer(spNodePt, oInfo.TRadius);

        if (spPolyRed == nullptr)
            continue;

        ITxGeometryPtr spUnionPoly = spPolyRed;

        ITxGeometryPtr spZoneGeo = m_spZoneLyr->GetGeometry(nxNodeId);
        if (spZoneGeo != nullptr) {
            TxPolygonPtr spPolyBlue = std::dynamic_pointer_cast<TxPolygon>(spZoneGeo);
            if (spPolyBlue == nullptr)
                continue;


            std::vector<TxPolygonPtr> vecPolygon = TxSpatialOper::Union(spPolyRed, spPolyBlue);
            size_t nxCnt = vecPolygon.size();

            if (nxCnt == 0)
                continue;

            if (nxCnt == 1) {
                spUnionPoly = vecPolygon[0];
            }
            else {
                TxMPolygonPtr spMPoly = TxMPolygonPtr(new TxMPolygon());
                for (size_t i=0; i<vecPolygon.size(); i++) {
                    spMPoly->AddSubPolygon(vecPolygon[i]);
                }
                spUnionPoly = spMPoly;
            }
        }        

        TxID++; {
            Gdiplus::Color oGdiColor; {
                oGdiColor.SetFromCOLORREF(oInfo.TColorRGB);
            }

            TxSymbolPolygonPtr spSymbol = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(0, 255, 255, 255), oGdiColor, 2, true); {
                TxHatchInfoPtr spHatch  = TxHatchInfoPtr(new TxHatchInfo(HatchStyle05Percent, oGdiColor));       
                spSymbol->SetHatch(spHatch);                
            }
            
            TxFeaturePtr  spFeature = TxFeaturePtr(new TxFeature(TxID, spUnionPoly));
            m_spLayer->Insert(spFeature);
            m_spLayer->FeatureSymbol(TxID, spSymbol);
        }        
    }    
}
