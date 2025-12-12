#include "StdAfx.h"

#include "ScreenDrawFlashNode.h"

#include "ObjectIDSelectDlg.h"
#include "ImChampMapSelectionReceiver.h"

KScreenDrawFlashNode::KScreenDrawFlashNode(IMapView* pMapView, ImChampMapSelectionReceiver* a_selectionReceiver, bool a_bMinDistSingleSelect)
	: m_pIMapView(pMapView), m_pSelectionReceiver(a_selectionReceiver), m_bMinDistSingleSelect(a_bMinDistSingleSelect)
{
	TxLogDebugVisitor();
    m_spLayer      = pMapView->MapGetFeatureLayer(KLayerID::Node());
    m_spSymbol	   = TxSymbolPoint::CreatePointSymbol(48, 20, Gdiplus::Color(150, 171,242, 0));
    m_dPixelBuffer = 5.0;
}

KScreenDrawFlashNode::~KScreenDrawFlashNode(void)
{
}

void KScreenDrawFlashNode::Draw()
{
    bool bDraw(false);
    if (m_vecFeatDisp.size() > 0) {
        m_pIMapView->MapAddForegroundLayer(m_vecFeatDisp);
        bDraw = true;
    }

    if (m_vecDrawFeature.size() > 0) {
        m_pIMapView->MapAddForegroundLayer(m_vecDrawFeature);
        bDraw = true;
    }

    if (true)
        m_pIMapView->MapForegroundRefresh();
}

void KScreenDrawFlashNode::MouseDown(UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_pSelectionReceiver == nullptr)
        return;

	size_t nxSize = m_vecDrawFeature.size();
	if (nxSize == 0) {
		m_pSelectionReceiver->MapSelectionNoneFeedback();
		return;
	}

    std::vector<__int64> vecTxID; 
    for (size_t i = 0; i < m_vecDrawFeature.size(); i++) {
        TxFeatureDispPtr spFeatureDisp = m_vecDrawFeature[i]; {
            TxFeaturePtr spFeature = spFeatureDisp->Feature();
            vecTxID.push_back(spFeature->TxObjectID);
        }
    }

    if (m_bMinDistSingleSelect == true || nxSize == 1) {
        m_pIMapView->MapForegroundRefresh();
        try {
            m_pSelectionReceiver->MapSelectionFeedback(vecTxID[0]);
            return;
        }
        catch(...) {
            TxLogDebugException();
        }	
    }
    else        
	{
		CPoint ptMouse(X, Y);
		CWnd* pMapWindow = m_pIMapView->GetMapWindow(); {
            pMapWindow->ClientToScreen(&ptMouse);
        }		

		KObjectIDSelectDlg dlg; {
            dlg.SetWindowPosition(ptMouse);
            dlg.SetLink(vecTxID);
        }
		
		if (dlg.DoModal() == IDOK) {
			if (m_pSelectionReceiver) {				
				m_pIMapView->MapForegroundRefresh();
				try {
					m_pSelectionReceiver->MapSelectionFeedback(dlg.GetSelectedID());
                    return;
				}
				catch(...) {
					TxLogDebugException();
				}				
			}
		}
	}
	// nombara message :
	// SelectionFeedback 이후에는 절대 다른 액션을 정의하지 말것..
}

void KScreenDrawFlashNode::MouseMove(UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spLayer == nullptr || m_pIMapView == nullptr)
        return;

    try
    {        
        double dPixelDist = m_pIMapView->MapGetPixelToDist() * m_dPixelBuffer;

        std::vector<TxFeaturePtr> vecFindFeature;
        m_spLayer->FindFeature(TxEnvelope(mapX-dPixelDist, mapY-dPixelDist, mapX+dPixelDist, mapY+dPixelDist), vecFindFeature, true);
        if (vecFindFeature.size() == 0) {
            if (m_vecDrawFeature.size() > 0) {
                m_vecDrawFeature.clear();
                Draw();
            }
            
            return;
        }
                
        std::vector<TxFeaturePtr> vecMouseOverFeature;
        double  dPointDist(0);
        TxPoint oPoint(mapX, mapY);

        for (size_t i=0; i<vecFindFeature.size(); i++)
        {
            TxFeaturePtr   spFeature  = vecFindFeature[i];
            ITxGeometryPtr spGeometry = spFeature->Geometry();
            TxPointPtr	   spPoint	  = std::dynamic_pointer_cast<TxPoint>(spGeometry);            
            double		   dNextDist  = spPoint->MinDist(oPoint);

            // 선택 가능한 노드가 정의 된 경우
            if (m_setSelectable.size() > 0) {
                if ( m_setSelectable.find(spFeature->TxObjectID) == m_setSelectable.end() )
                    continue;
            }

            if (i == 0) {
                vecMouseOverFeature.push_back(spFeature);
                dPointDist = dNextDist;
            }
            else {
                if ((abs(dNextDist -dPointDist)) < (dPixelDist / 2) ) {
                    vecMouseOverFeature.push_back(spFeature);
                }
                else if (dNextDist < dPointDist) {
                    vecMouseOverFeature.clear();
                    vecMouseOverFeature.push_back(spFeature);
                    dPointDist = dNextDist;
                }
            }
        }

        m_vecDrawFeature.clear();
        for (size_t i = 0; i <vecMouseOverFeature.size(); i++) {
            TxFeaturePtr     spFeature		= vecMouseOverFeature[i];
            TxFeatureDispPtr spFeatureDisp	= TxFeatureDispPtr(new TxFeatureDisp(spFeature->TxObjectID, spFeature->Geometry()));
            spFeatureDisp->Symbol(m_spSymbol);
            m_vecDrawFeature.push_back(spFeatureDisp);
        }

        Draw();
    }
    catch (...)
    {
    }
}

void KScreenDrawFlashNode::SetSelectable( std::set<__int64>& setSelect )
{
    m_vecFeatDisp.clear();
    m_setSelectable = setSelect;

    ITxFeatureLayerPtr spLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Node());
    TxSymbolPointPtr   spSymbol = TxSymbolPoint::CreatePointSymbol(48, 12, Gdiplus::Color(128, 0,0,255));
    
    //★ 2018-04-11 : 조회 속도 개선
    if (setSelect.size() < 1000) {
        for (auto iter = setSelect.begin(); iter != setSelect.end(); ++iter) {
            __int64 nxTxID = *iter;

            TxFeaturePtr spFeature = spLayer->GetFeature(nxTxID); //★ 1건을 연속적 호출
            if (spFeature != nullptr) {
                TxFeatureDispPtr spFeaDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature));
                spFeaDisp->Symbol(spSymbol);

                m_vecFeatDisp.push_back(spFeaDisp);
            }
        }
    }
    else {
        std::map<__int64, ITxGeometryPtr> mapGeo;
        spLayer->GetGeometry(mapGeo);
        for (auto iter = setSelect.begin(); iter != setSelect.end(); ++iter) {
            __int64 nxTxID = *iter;
            ITxGeometryPtr spGeo = mapGeo[nxTxID];
            
            if (spGeo != nullptr) {
                TxFeatureDispPtr spFeaDisp = TxFeatureDispPtr(new TxFeatureDisp(nxTxID, spGeo));
                spFeaDisp->Symbol(spSymbol);

                m_vecFeatDisp.push_back(spFeaDisp);
            }
        }
    }

    Draw();
}

void KScreenDrawFlashNode::SetSelectable( std::vector<__int64>& vecSelectable )
{
    std::set<__int64> setSelectable;
    for (size_t i=0; i<vecSelectable.size(); i++) {
        setSelectable.insert(vecSelectable[i]);
    }

    SetSelectable(setSelectable);
}