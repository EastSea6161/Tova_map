#include "StdAfx.h"
#include "Project.h"
#include "KThemDrawIntersection.h"
#include "BulkDBaseIntersection.h"

KThemDrawIntersection::KThemDrawIntersection( IMapView* a_mapRequest) : IThemeLayer(a_mapRequest)
{
}

KThemDrawIntersection::~KThemDrawIntersection(void)
{
	m_pIMapView->MapRemoveLayer(KLayerID::KThemDrawIntersection());
}


KEMThemeLayer KThemDrawIntersection::GetType()
{
	return ThemeLayerDrawIntersection;
}

void KThemDrawIntersection::AddDraw( Integer nxID)
{
    ITxFeatureLayerPtr spNodeLyr = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    if (spNodeLyr == nullptr)
        return;

    ITxFeatureUserLayerPtr spLayer = m_pIMapView->MapGetFeatureUserLayer(KLayerID::KThemDrawIntersection());
    if (spLayer == nullptr) {
        spLayer = m_pIMapView->AddUserLayer(KLayerID::KThemDrawIntersection(), true);
    }

	try
	{
		TFixedIntersectionDB oInfo;
		bool bResult  = KBulkDBaseIntersection::FindIntersectionInfo(m_pIMapView->GetTarget(), nxID, oInfo);
		if (!bResult) {
			return;
		}

        __int64 nxNodeID = oInfo.node_id;
        ITxGeometryPtr   spGeometry = spNodeLyr->GetGeometry(nxNodeID);
        TxSymbolPointPtr spSymbol   = TxSymbolPoint::CreatePointSymbol(87, 15, Gdiplus::Color::Red);

        TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxID, spGeometry));
        spLayer->Insert(spFeature);
		spLayer->FeatureSymbol(nxID, spSymbol);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KThemDrawIntersection::RemoveDraw(Integer nxID)
{
	ITxFeatureUserLayerPtr spLayer = m_pIMapView->MapGetFeatureUserLayer(KLayerID::KThemDrawIntersection());
    if (spLayer == nullptr) {
        return;
    }

    spLayer->Delete(nxID);
}