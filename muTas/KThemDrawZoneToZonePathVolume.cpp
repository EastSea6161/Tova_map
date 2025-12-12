#include "StdAfx.h"
#include "KThemDrawZoneToZonePathVolume.h"
#include "Project.h"

KThemDrawZoneToZonePathVolume::KThemDrawZoneToZonePathVolume( IMapView* a_mapRequest ) : IThemeLayer(a_mapRequest)
{   
    Initialize();
}

void KThemDrawZoneToZonePathVolume::Initialize()
{
    m_spLayer = m_pIMapView->AddUserLayer(KLayerID::KThemDrawZoneToZonePathVolume(), true);
    if (m_spLayer != nullptr) {
        ITxLabelSymbolPtr spLabelSym = m_spLayer->LabelSymbol();
    }

    TxID = 1;
}

KThemDrawZoneToZonePathVolume::~KThemDrawZoneToZonePathVolume( void )
{
    m_pIMapView->MapRemoveLayer(KLayerID::KThemDrawZoneToZonePathVolume());
}


void KThemDrawZoneToZonePathVolume::Draw(int a_nType/*0:EachPath, 1:Goguma*/, std::vector<TPathVolumeInfo>& a_rVecTPathVolumeInfo)
{	
	m_nType = a_nType;

	if (m_nType == 0) {
		DrawRequestEachPath(a_rVecTPathVolumeInfo);
	}

	if (m_nType == 1) {
		DrawRequestVinePath(a_rVecTPathVolumeInfo);
	}
}

void KThemDrawZoneToZonePathVolume::DrawRequestEachPath( std::vector<TPathVolumeInfo>& vecTPathVolumeInfo )
{
    ITxFeatureLayerPtr spLinkLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (m_spLayer == nullptr || spLinkLyr == nullptr)
        return;

    m_spLayer->Delete();

    for (size_t i=0; i<vecTPathVolumeInfo.size(); i++) {
        TPathVolumeInfo& oInfo = vecTPathVolumeInfo[i];

        std::vector<TxPoint> vecPLinePt;

        std::vector<Integer>& vecPathLink = oInfo.TvecLink;
        for (size_t j=0; j <vecPathLink.size(); j++) {
            __int64 nxLinkID = vecPathLink[j];

            TxFeaturePtr  spFeature = spLinkLyr->GetFeature(nxLinkID);
            if (spFeature == nullptr)
                continue;

            TxPolylinePtr spPLine = std::dynamic_pointer_cast<TxPolyline>(spFeature->Geometry());
            if (spPLine == nullptr)
                continue;

            std::vector<TxPoint> vecPt = spPLine->GetPoints();
            vecPLinePt.insert(vecPLinePt.end(), vecPt.begin(), vecPt.end());
        }

        if (vecPLinePt.size() > 1) {
            TxPolylinePtr spPLine = TxPolylinePtr(new TxPolyline(vecPLinePt));
            
            double dWidth = oInfo.TdWidth;
            Gdiplus::Color clrLine; {
                clrLine.SetFromCOLORREF(oInfo.Tcolor);
            }

            CString strLabel(_T("")); {
                strLabel.Format(_T("%I64d(%.1f) ¢º"), oInfo.TnxPathNo, oInfo.TdVolume);
            }

            ITxLabelPtr spLabel = ITxLabelPtr(new TxLabel(strLabel));
            TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(dWidth, clrLine, 1);

            TxID++;
            TxFeaturePtr  spFeature = TxFeaturePtr(new TxFeature(TxID, spPLine));
            m_spLayer->Insert(spFeature);
            m_spLayer->FeatureSymbol(TxID, spLineSym);
            m_spLayer->FeatureLabel(TxID, spLabel);
        }
    }
}

void KThemDrawZoneToZonePathVolume::DrawRequestVinePath( std::vector<TPathVolumeInfo>& vecTPathVolumeInfo )
{
    ITxFeatureLayerPtr spLinkLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (m_spLayer == nullptr || spLinkLyr == nullptr)
        return;

    m_spLayer->Delete();

    std::map<__int64, TVineLinkInfo> mapLinkInfo;

    for (size_t i=0; i<vecTPathVolumeInfo.size(); i++) 
    {
        TPathVolumeInfo& oInfo = vecTPathVolumeInfo[i];
        std::vector<Integer>& vecPathLink = oInfo.TvecLink;

        for (size_t i=0; i<vecPathLink.size(); i++) {
            __int64 nxLinkID = vecPathLink[i];

            auto iter = mapLinkInfo.find(nxLinkID);
            auto end  = mapLinkInfo.end();

            if (iter != end) {
                TVineLinkInfo& oLinkInfo = iter->second;
                oLinkInfo.TColor = RGB(146, 208, 80);
                oLinkInfo.TVolume  +=  oInfo.TdVolume;
                oLinkInfo.TWidth   +=  oInfo.TdWidth;
            }
            else {
                TVineLinkInfo oLinkInfo;
                oLinkInfo.TColor  = oInfo.Tcolor;
                oLinkInfo.TVolume = oInfo.TdVolume;
                oLinkInfo.TWidth  = oInfo.TdWidth;

                mapLinkInfo.insert(std::make_pair(nxLinkID, oLinkInfo));
            }
        }
    }

    for (auto iter = mapLinkInfo.begin(); iter != mapLinkInfo.end(); ++iter) 
    {
        __int64        nxLinkID = iter->first;
        TVineLinkInfo& oInfo    = iter->second;

        TxFeaturePtr  spFeature = spLinkLyr->GetFeature(nxLinkID);
        if (spFeature == nullptr)
            continue;

        TxPolylinePtr spPLine = std::dynamic_pointer_cast<TxPolyline>(spFeature->Geometry());
        if (spPLine == nullptr)
            continue;

        double dWidth = oInfo.TWidth;
        Gdiplus::Color clrLine; {
            clrLine.SetFromCOLORREF(oInfo.TColor);
        }

        CString strLabel(_T("")); {
            strLabel.Format(_T("%.1f ¢º"), oInfo.TVolume);
        }

        ITxLabelPtr spLabel = ITxLabelPtr(new TxLabel(strLabel));
        TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(dWidth, clrLine, 1);

        TxID++; {
            TxFeaturePtr  spFeature = TxFeaturePtr(new TxFeature(TxID, spPLine));
            m_spLayer->Insert(spFeature);
            m_spLayer->FeatureSymbol(TxID, spLineSym);
            m_spLayer->FeatureLabel(TxID, spLabel);
        }        
    }
}

KEMThemeLayer KThemDrawZoneToZonePathVolume::GetType()
{
    return ThemeLayerDrawZoneToZonePathVolume;
}
