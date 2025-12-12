#include "stdafx.h"
#include "ScreenDrawFlashZone.h"

#include "ImChampMapSelectionReceiver.h"

KScreenDrawFlashZone::KScreenDrawFlashZone( IMapView* pMapView, ImChampMapSelectionReceiver* a_selectionReceiver, bool a_bMinDistSingleSelect/*=false*/ )
                     :m_pIMapView(pMapView), m_pSelectionReceiver(a_selectionReceiver)
{
    m_spLayer  = pMapView->MapGetFeatureLayer(KLayerID::Zone());
    m_spSymbol = TxSymbolPolygon::CreatePolygonSymbol(Gdiplus::Color(150, 171,242, 0),Gdiplus::Color(250, 0, 0, 0));
}

KScreenDrawFlashZone::~KScreenDrawFlashZone( void )
{
}

void KScreenDrawFlashZone::MouseDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_pSelectionReceiver == nullptr)
        return;

    if (m_spFeatureDisp == nullptr) {
        m_pSelectionReceiver->MapSelectionNoneFeedback();
        return;
    }
    
    TxFeaturePtr spFeature = m_spFeatureDisp->Feature();
    m_pSelectionReceiver->MapSelectionFeedback(spFeature->TxObjectID);
}

void KScreenDrawFlashZone::MouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{    
    try 
    {
        double dPixelDist(0.5);
        std::vector<TxFeaturePtr> vecFindFeature;
        m_spLayer->FindFeature(TxEnvelope(mapX-dPixelDist, mapY-dPixelDist, mapX+dPixelDist, mapY+dPixelDist), vecFindFeature, false);
        if (vecFindFeature.size() == 0) {
            m_spFeatureDisp.reset();
            m_pIMapView->MapForegroundRefresh();
            return;
        }

        TxFeaturePtr spFeature = vecFindFeature[0];
        m_spFeatureDisp	= TxFeatureDispPtr(new TxFeatureDisp(spFeature));
        m_spFeatureDisp->Symbol(m_spSymbol);

        m_pIMapView->MapAddForegroundLayer(m_spFeatureDisp);
        m_pIMapView->MapForegroundRefresh();
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

