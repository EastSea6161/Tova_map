#include "stdafx.h"
#include "IncludeMap.h"
#include "MapView.h"

#include "IOView.h"
#include "IOTransitTableView.h"
#include "IOTurnTableView.h"
#include "IOIntersectionTableView.h"
#include "MapFrameWnd.h"
#include "CommandWnd.h"

#include "KDlgNetworkEditorMain.h"

double KMapView::GetPixelToRealDist( double a_dNoOfPixel )
{   
    return MapGetPixelToDist() * a_dNoOfPixel;
}

void KMapView::TasESRICacheRefresh()
{	
    MapRefresh();
}

void KMapView::RefreshAll()
{
    MapRefresh();
}

double KMapView::GetRequestMapScale()
{
    return GetMapScale();
}

bool KMapView::NodeLayerVisible() 
{
    ITxFeatureLayerPtr spLayer = MapGetFeatureLayer(KLayerID::Node());
    if (spLayer == nullptr)
        return false;

    return spLayer->LayerOn();
}

bool KMapView::LinkLayerVisible()
{
    ITxFeatureLayerPtr spLayer = MapGetFeatureLayer(KLayerID::Link());
    if (spLayer == nullptr)
        return false;

    return spLayer->LayerOn();
}

bool KMapView::TransitVisible()
{
    KIOTransitTableView* pTransitView = (KIOTransitTableView*)(m_pIOView->FindTableView(_T("transit")));
    if(NULL == pTransitView) {
        return false;
    }

    return true;
}

bool KMapView::TurnVisible()
{
	KIOTurnTableView* pTurnView = (KIOTurnTableView*)(m_pIOView->FindTableView(_T("turn")));
	if(NULL == pTurnView) {
		return false;
	}

	return true;
}

bool KMapView::IntersectionVisible()
{
	KIOIntersectionTableView* pIntersectionView = (KIOIntersectionTableView*)m_pIOView->FindTableView(TABLE_INTERSECTION);
	if (nullptr == pIntersectionView) {
		return false;
	}

	return true;
}

KCommandWnd* KMapView::GetCommandView()
{
    KMapFrameWnd* pMapFrame      = (KMapFrameWnd*)GetParentFrame();
    KCommandWnd*  pCommandWindow = pMapFrame->GetCommandView();

    return pCommandWindow;
}


short KMapView::GetCacheID()
{
	return TasESRICacheID;
}

void KMapView::ShapeDeletedItem( CString strLayer, Integer a_nxID )
{
    //★ 향후, 역활을 달리하는 함수 필요
    //LoadInitialLayers(false);
}

void KMapView::ShapeEditedItem( CString strLayer, Integer a_nxID )
{
    //★ 향후, 역활을 달리하는 함수 필요
    //LoadInitialLayers(false);
}

void KMapView::NotifyNetworkEdior( int nEditType/*0:노드추가(노드), 1:노드추가(링크), 2:노드선택(링크), 5:버스노선추가*/, Integer a_nxID )
{
    if (m_spDlgNetworkEdior == nullptr)
        return;

    m_spDlgNetworkEdior->NotifyNetworkEdior(nEditType, a_nxID);
}

void KMapView::NotifyNetworkEdior( int nEditType/*0:예약, 1:링크추가 3:회전정보추가*/, std::vector<__int64>& vecID )
{
    //★ 회전정보(turn_id, f-link, at-node, t-link

    if (m_spDlgNetworkEdior == nullptr)
        return;

    m_spDlgNetworkEdior->NotifyNetworkEdior(nEditType, vecID);
}
