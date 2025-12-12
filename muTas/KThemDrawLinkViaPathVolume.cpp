#include "StdAfx.h"
#include "Project.h"
#include "KThemDrawLinkViaPathVolume.h"


KThemDrawLinkViaPathVolume::KThemDrawLinkViaPathVolume(IMapView* a_mapRequest) : IThemeLayer(a_mapRequest)
{
    m_spLayer = m_pIMapView->AddUserLayer(KLayerID::KThemDrawLinkViaPathVolume(), true);
    if (m_spLayer != nullptr) {
        ITxLabelSymbolPtr spLabelSym = m_spLayer->LabelSymbol();
        //spLabelSym->BackgoundFill(true);
    }

    TxID = 1;
}

KThemDrawLinkViaPathVolume::~KThemDrawLinkViaPathVolume(void)
{
	m_pIMapView->MapRemoveLayer(KLayerID::KThemDrawLinkViaPathVolume());
}

KEMThemeLayer KThemDrawLinkViaPathVolume::GetType()
{
	return ThemeLayerDrawLinkViaPathVolume;
}

void KThemDrawLinkViaPathVolume::Draw( std::vector<TDrawLinkInfo>& vecDrawLinkInfo,  std::vector<Integer>& vecSelectedLink, bool a_bDrawLabel )
{
    ITxFeatureLayerPtr spLinkLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLinkLyr == nullptr)
        return;

    if (m_spLayer == nullptr)
        return;

    m_spLayer->Delete();

    for (size_t i=0; i<vecDrawLinkInfo.size(); i++) {
        TDrawLinkInfo& oInfo = vecDrawLinkInfo[i];
        __int64 nxLinkID = oInfo.TLinkID;
        double  dWidth   = oInfo.TWidth;

        TxFeaturePtr spFeature = spLinkLyr->GetFeature(nxLinkID);
        if (spFeature == nullptr)
            continue;
                
        Gdiplus::Color clrLine; {
            clrLine.SetFromCOLORREF(oInfo.TColorRGB);
        }

        TxID++;

        TxSymbolLinePtr spLineSym = TxSymbolLine::CreateOffsetLineSymbol(dWidth, clrLine, 1);
        m_spLayer->Insert(TxFeaturePtr(new TxFeature(TxID, spFeature->Geometry())));
        m_spLayer->FeatureSymbol(TxID, spLineSym);

        ITxLabelPtr spLabel = ITxLabelPtr(new TxLabel(oInfo.TLabel));
        m_spLayer->FeatureLabel(TxID, spLabel);
    }

    TxLineDecoSymbolArrowPtr spDeco = TxLineDecoSymbolArrowPtr(new TxLineDecoSymbolArrow(TxConst::MaxInt(), 0.5, Gdiplus::Color::Blue));

    for (size_t i=0; i<vecSelectedLink.size(); i++) {
        __int64 nxLinkID = vecSelectedLink[i];
        double  dWidth(1);

        TxFeaturePtr spFeature = spLinkLyr->GetFeature(nxLinkID);
        if (spFeature == nullptr)
            continue;

        TxID++;

        TxSymbolLinePtr spLineSym = TxSymbolLine::CreateLineSymbol(2, Gdiplus::Color::Black); {            
            spLineSym->SetDecoSymbol(spDeco);
        }
        m_spLayer->Insert(TxFeaturePtr(new TxFeature(TxID, spFeature->Geometry())));
        m_spLayer->FeatureSymbol(TxID, spLineSym);
    }

	m_spLayer->ShowLabel(a_bDrawLabel);
}

void KThemDrawLinkViaPathVolume::SetDrawLabel( bool a_bDrawLabel )
{
    m_spLayer->ShowLabel(a_bDrawLabel);
}
