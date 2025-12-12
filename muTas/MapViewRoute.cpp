#include "stdafx.h"
#include "MapView.h"
#include "Target.h"

#include "IOView.h"
#include "IOTransitTableView.h"

#include "BulkDBaseTransit.h"
#include "NodeSelectionInfo.h"
#include "ScreenDrawEditNodeSelect.h"
#include "KDBaseAccessibilityModify.h"
#include "DlgTransitStyle.h"

void KMapView::OnMapviewMenuSearchTransitByLink()
{
    SearchTransitByLink();
    
}

void KMapView::SearchTransitByLink(bool bOption)
{
    if (bOption == false) {
        TransitLinkSelect();
        return;
    }

    SetMapMode(KEMMapModeSelectTransitByLink);
    try
    {
        if (m_spMapViewSelectionFeedback == nullptr)
        {
            m_spMapViewSelectionFeedback = KMapViewSelectionFeedbackPtr(new KMapViewSelectionFeedback(this));
        }

        m_spMapViewSelectionFeedback->SetFeedbackType(MapViewFeedbackTypeTransitLinkSelect);
        m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditSingleLinkSelect(this, m_spMapViewSelectionFeedback.get()));
        m_spScreenDrawEdit->StartEditing();

        TransitLinkSelect();
    }
    catch (...)
    {
        AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
    }
}

void KMapView::SearchTransitByNode(bool bOption)
{
    if (bOption == false) {
        TransitNodeSelect();
        return;
    }

    SetMapMode(KEMMapModeSelectNode);
    try
    {
        if (m_spMapViewSelectionFeedback == nullptr) {
            m_spMapViewSelectionFeedback = KMapViewSelectionFeedbackPtr(new KMapViewSelectionFeedback(this));
        }

        std::map<Integer, std::set<Integer>>	mapStationLineInfo;
        std::map<Integer, int>					mapStationModeInfo;
        
        TxLogDebugStartMsg(); //★ 시간이 걸리는 부분 Start
        KDBaseAccessibilityModify::TransitStationNodeViaInfo(m_pTarget, mapStationLineInfo, mapStationModeInfo);
        
        std::vector<TNodeSelectionInfo> vecTNodeStation;

        auto iter = mapStationModeInfo.begin();
        auto end  = mapStationModeInfo.end();
        while (iter != end)
        {
            TNodeSelectionInfo oTNodeStation;
            oTNodeStation.TNodeID = iter->first;
            oTNodeStation.TType   = 1;
            oTNodeStation.TCharacterIndex = 57;
            oTNodeStation.TSize   = 7;
            oTNodeStation.TColorRGB = RGB(255, 0, 0);
            CString strLabel;
            strLabel.Format(_T("%I64d"), iter->first);
            oTNodeStation.TLabel  = strLabel;

            vecTNodeStation.push_back(oTNodeStation);
            ++iter;
        }
        
        m_spMapViewSelectionFeedback->SetFeedbackType(MapViewFeedbackTypeTransitNodeSelect);
        m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditNodeSelect(this, m_spMapViewSelectionFeedback.get(), vecTNodeStation, false));
        m_spScreenDrawEdit->StartEditing();
        
        TransitNodeSelect();
    }
    catch (...)
    {
        AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
    }
}

void KMapView::RefreshTransitMapDraw()
{
    ClearTransit(false);

    KIOTransitTableView* pTransitView = (KIOTransitTableView*)(m_pIOView->FindTableView(_T("transit")));
    if(NULL == pTransitView) {
        return;
    }

    std::set<Integer>  setSelectedTransit = pTransitView->GetSelectedTransit();

    std::set<Integer>::iterator iter = setSelectedTransit.begin();
    std::set<Integer>::iterator end  = setSelectedTransit.end  ();

    for( ;iter != end; ++iter) {
        TTransitInfo oTransitInfo;
        if (KBulkDBaseTransit::TransitInfoQuery(m_pTarget, *iter, oTransitInfo)) {   
            /*정의된 스타일이 존재하면 적용*/
            if (nullptr != pTransitView) {
                pTransitView->IsTransitStyle(oTransitInfo.TTransitID, oTransitInfo.TRGBColor, oTransitInfo.dWidth, oTransitInfo.dOffset);
            }

            AddTransitDraw(oTransitInfo);
        }
    }

    if (true) {
        DrawTransit();
    }
	
	MapRefresh();
}


void KMapView::RefreshTransitMapDraw(Integer nRouteID)
{
    KIOTransitTableView* pTransitView = (KIOTransitTableView*)(m_pIOView->FindTableView(_T("transit")));

    TTransitInfo oTransitInfo;
    if (KBulkDBaseTransit::TransitInfoQuery(m_pTarget, nRouteID, oTransitInfo)) {        

        /*정의된 스타일이 존재하면 적용*/
        if (nullptr != pTransitView) {
            pTransitView->IsTransitStyle(oTransitInfo.TTransitID, oTransitInfo.TRGBColor, oTransitInfo.dWidth, oTransitInfo.dOffset);
        }

        UpdateTransitDraw(oTransitInfo);
    }

    if (true) {
        DrawTransit();
    }

	MapRefresh();
}

void KMapView::AddTransitMapDraw(Integer nRouteID)
{
    KIOTransitTableView* pTransitView = (KIOTransitTableView*)(m_pIOView->FindTableView(_T("transit")));

    TTransitInfo oTransitInfo;
    if (KBulkDBaseTransit::TransitInfoQuery(m_pTarget, nRouteID, oTransitInfo)) {   

        /*정의된 스타일이 존재하면 적용*/
        if (nullptr != pTransitView) {
            pTransitView->IsTransitStyle(oTransitInfo.TTransitID, oTransitInfo.TRGBColor, oTransitInfo.dWidth, oTransitInfo.dOffset);
        }

        AddTransitDraw(oTransitInfo);
    }

    if (true) {
        DrawTransit();
    }

	MapRefresh();
}

void KMapView::RemoveTransitMapDraw  (Integer a_nxTransitID)
{   
    RemoveTransitDraw(a_nxTransitID);

    if (true) {
        DrawTransit();
    }

	MapRefresh();
}

void KMapView::ZoomTransitExtent (Integer a_nxTransitID)
{
    try
    {
        TxEnvelope oEnv;
        if (KBulkDBaseTransit::TransitExtent( m_pTarget, a_nxTransitID, oEnv) <= 0) {
            AfxMessageBox(_T("대중교통 경로 정보가 존재하지 않습니다."));
            return;
        }

        double dDistance = QBicGeometry::Length(oEnv.GetMinX(), oEnv.GetMinY(), oEnv.GetMaxX(), oEnv.GetMaxY()); 
        if (dDistance < 1000) {
            dDistance = 1000;
        } else {
            dDistance *= 1.3;
        }
        oEnv.Offset(dDistance);

        MapCommandZoomExtent(oEnv);
        MapRefresh();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}

void KMapView::ZoomTransitFirst  (Integer a_nxTransitID)
{
	try
	{  
        ITxFeatureLayerPtr spLayer = MapGetFeatureLayer(KLayerID::Link());
        if (spLayer == nullptr)
            return;

		std::vector<TDrawTransitLinkInfo> vecDrawTransitLinkInfo;
		KBulkDBaseTransit::TransitLinkPathQuery(m_pTarget, a_nxTransitID, vecDrawTransitLinkInfo);

		size_t nxCount = vecDrawTransitLinkInfo.size();
		if (nxCount == 0) {
			AfxMessageBox(_T("대중교통 경로 정보가 존재하지 않습니다."));
			return;
		}

		TDrawTransitLinkInfo& oDrawTransitLinkInfo = vecDrawTransitLinkInfo[0];
        __int64 nxID = oDrawTransitLinkInfo.TLinkID;

        TxFeaturePtr spFeature = spLayer->GetFeature(nxID);
        if (spFeature == nullptr) {
            AfxMessageBox(_T("대중교통 경로 정보가 존재하지 않습니다."));
            return;
        }

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr) {
            AfxMessageBox(_T("대중교통 경로 정보가 존재하지 않습니다."));
            return;
        }

        TxEnvelope oEnv = spGeometry->Envelope();
        double dDistance = QBicGeometry::Length(oEnv.GetMinX(), oEnv.GetMinY(), oEnv.GetMaxX(), oEnv.GetMaxY()); 
        if (dDistance < 1000) {
            dDistance = 1000;
        } else {
            dDistance *= 1.3;
        }
        oEnv.Offset(dDistance);

        MapCommandZoomExtent(oEnv);
        MapRefresh();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}

void KMapView::ZoomTransitLast   (Integer a_nxTransitID)
{
	try
	{        
        ITxFeatureLayerPtr spLayer = MapGetFeatureLayer(KLayerID::Link());
        if (spLayer == nullptr)
            return;

		std::vector<TDrawTransitLinkInfo> vecDrawTransitLinkInfo;
		KBulkDBaseTransit::TransitLinkPathQuery(m_pTarget, a_nxTransitID, vecDrawTransitLinkInfo);

		size_t nxCount = vecDrawTransitLinkInfo.size();
		if (nxCount == 0){
			AfxMessageBox(_T("대중교통 경로 정보가 존재하지 않습니다."));
			return;
		}

        TDrawTransitLinkInfo& oDrawTransitLinkInfo = vecDrawTransitLinkInfo[nxCount-1];
        __int64 nxID = oDrawTransitLinkInfo.TLinkID;

        TxFeaturePtr spFeature = spLayer->GetFeature(nxID);
        if (spFeature == nullptr) {
            AfxMessageBox(_T("대중교통 경로 정보가 존재하지 않습니다."));
            return;
        }

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr) {
            AfxMessageBox(_T("대중교통 경로 정보가 존재하지 않습니다."));
            return;
        }

        TxEnvelope oEnv = spGeometry->Envelope();
        double dDistance = QBicGeometry::Length(oEnv.GetMinX(), oEnv.GetMinY(), oEnv.GetMaxX(), oEnv.GetMaxY()); 
        if (dDistance < 1000) {
            dDistance = 1000;
        } else {
            dDistance *= 1.3;
        }
        oEnv.Offset(dDistance);

        MapCommandZoomExtent(oEnv);
        MapRefresh();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}

