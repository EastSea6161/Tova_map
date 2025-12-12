#include "StdAfx.h"
#include "Project.h"
#include "ScreenDrawFlashLink.h"

#include "ObjectIDSelectDlg.h"
#include "ImChampMapSelectionReceiver.h"

KScreenDrawFlashLink::KScreenDrawFlashLink(IMapView* pMapView, ImChampMapSelectionReceiver* a_selectionReceiver)
	: m_pIMapView(pMapView), m_pSelectionReceiver(a_selectionReceiver)
{
    m_dPixelBuffer = 10.0;

    m_spLayer  = pMapView->MapGetFeatureLayer(KLayerID::Link());
    m_spSymbol = TxSymbolLine::CreateOffsetLineSymbol(4, Gdiplus::Color(150, 171,242, 0), 1); 
}

KScreenDrawFlashLink::~KScreenDrawFlashLink(void)
{
}

void KScreenDrawFlashLink::Draw()
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
    
    m_pIMapView->MapForegroundRefresh();        
}

void KScreenDrawFlashLink::MouseDown( UINT nFlags, long X, long Y, double mapX, double mapY )
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

    if (nxSize == 1) {
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
}

void KScreenDrawFlashLink::MouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spLayer == nullptr || m_pIMapView == nullptr)
        return;

    try 
    {
        double dFindDist = m_pIMapView->MapGetPixelToDist() * m_dPixelBuffer;
        std::vector<TxFeaturePtr> vecFindFeature;
        m_spLayer->FindFeature(TxEnvelope(mapX-dFindDist, mapY-dFindDist, mapX+dFindDist, mapY+dFindDist), vecFindFeature, false);
        if (vecFindFeature.size() == 0) {
            if (m_vecDrawFeature.size() > 0) {
                m_vecDrawFeature.clear();
                Draw();
            }

            return;
        }

        TxPoint	oPoint(mapX, mapY);
        std::vector<TxFeaturePtr> vecMouseOverFeature;
        for (size_t i = 0; i <vecFindFeature.size(); i++)
        {
            TxFeaturePtr   spFeature  = vecFindFeature[i]; 
            if (m_setSelectable.size() > 0) {
                if ( m_setSelectable.find(spFeature->TxObjectID) == m_setSelectable.end() )
                    continue;
            }

            ITxGeometryPtr spGeometry = spFeature->Geometry(); 
            TxPolylinePtr  spPolyline = std::dynamic_pointer_cast<TxPolyline>(spGeometry);

            std::vector<TxPoint>& vecPt = spPolyline->GetOffsetPoint(dFindDist); 
            TxPolylinePtr  spOffsetLine = TxPolylinePtr(new TxPolyline(vecPt));
            double dDist(99999999); /*= spOffsetLine->MinDist(oPoint);*/
            TxPoint oIP; 
            if ( spOffsetLine->LineOnPoint(oPoint, oIP) == true ) {
                dDist = TxMath::Dist(oPoint, oIP);

                if (dDist < 1.0 * dFindDist) {
                    vecMouseOverFeature.push_back(spFeature);
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
    catch (int& ex) {
    	CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    } catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    } catch (...) {
        TxLogDebugException();
    }
}


void KScreenDrawFlashLink::SetSelectable( std::set<Integer>& setSelect)
{
    m_vecFeatDisp.clear();
    m_setSelectable = setSelect;
    
    //★ 링크레이어
    ITxFeatureLayerPtr spLayer  = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
    TxSymbolLinePtr    spSymbol = TxSymbolLine::CreateOffsetLineSymbol(2, Gdiplus::Color::Blue, 0);

    for (auto iter = setSelect.begin(); iter != setSelect.end(); ++iter) {
        __int64 nxTxID = *iter;

        TxFeaturePtr spFeature = spLayer->GetFeature(nxTxID);
        if (spFeature != nullptr) {
            TxFeatureDispPtr spFeaDisp = TxFeatureDispPtr(new TxFeatureDisp(spFeature));
            spFeaDisp->Symbol(spSymbol);

            m_vecFeatDisp.push_back(spFeaDisp);
        }
    }

    Draw();
}

void KScreenDrawFlashLink::SetSelectable( std::vector<Integer>& vecSelectable)
{
    std::set<__int64> setSelectable;
    for (size_t i=0; i<vecSelectable.size(); i++) {
        setSelectable.insert(vecSelectable[i]);
    }

    SetSelectable(setSelectable);
}
