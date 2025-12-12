#include "stdafx.h"
#include "MapView.h"

#include "Target.h"
#include "MapDiplayMonitor.h"


double KMapView::GetMapScale()
{
    double dScale = MapGetScale();
    return dScale;
}


void KMapView::GetMapCenterPoint(double& a_dMapCenterPointX, double& a_dMapCenterPointY)
{
    TxEnvelope oEnv;
    MapGetEnvelope(oEnv);    
    oEnv.GetCenter(a_dMapCenterPointX, a_dMapCenterPointY);
}


void KMapView::MapPositionSyncMessage(double a_dMapCenterPointX, double a_dMapCenterPointY)
{
    m_bReceivedDisplaySyncMessage = true;
    if ( m_emMapDisplaySync != KEMMapDisplaySyncNone ) {
        TxEnvelope oEnv;
        MapGetEnvelope(oEnv); {
            oEnv.Move(a_dMapCenterPointX, a_dMapCenterPointY);
        }

        MapCommandZoomExtent(oEnv);
        MapRefresh();
    }

    TxLogDebugEndMsg();
}


void KMapView::MapPositionScaleSyncMessage(TxEnvelope oEnv)
{
    m_bReceivedDisplaySyncMessage = true;
    MapCommandZoomExtent(oEnv);
    MapRefresh();    
}

// deprecated function
//void KMapView::OnMapViewSync( NMHDR* pNMHDR, LRESULT* pResult )
//{
//    *pResult = TRUE;
//    AfxMessageBox(_T("Deprecated..."));
//}
//
//
//// deprecated function
//void KMapView::OnUpdateMapViewSync( CCmdUI* pCmdUI )
//{
//    pCmdUI->Enable( TRUE );
//}


void KMapView::OnMapviewSyncCommand( UINT nID )
{
    TxLogDebugStartMsg();
    KMapDisplayMonitor* pKMapDiplayMonitor = KMapDisplayMonitor::Instance();

    switch ( nID )
    {
    case ID_MAPVIEW_SYNC_NONE :
        m_emMapDisplaySync = KEMMapDisplaySyncNone;
        break;
    case ID_MAPVIEW_SYNC_POS :
        m_emMapDisplaySync = KEMMapDisplaySyncPos;
        break;
    case ID_MAPVIEW_SYNC_POS_SCALE :
        m_emMapDisplaySync = KEMMapDisplaySyncPosScale;
        break;
    default:
        m_emMapDisplaySync = KEMMapDisplaySyncNone;
        break;
    }

    if ( KEMMapDisplaySyncNone == m_emMapDisplaySync )
    {
        pKMapDiplayMonitor->UnRegister(this);
    }
    else
    {
        pKMapDiplayMonitor->Register(this);
    }

    TxLogDebugEndMsg();
}


void KMapView::OnUpdateMapviewSyncCommand( CCmdUI* pCmdUI )
{
    BOOL bCheck = TRUE;
    //pCmdUI->Enable( TRUE );
    switch ( pCmdUI->m_nID )
    {
    case ID_MAPVIEW_SYNC_NONE :
        bCheck = m_emMapDisplaySync == KEMMapDisplaySyncNone ? TRUE : FALSE;
        break;
    case ID_MAPVIEW_SYNC_POS :
        bCheck = m_emMapDisplaySync == KEMMapDisplaySyncPos ? TRUE : FALSE;
        break;
    case ID_MAPVIEW_SYNC_POS_SCALE :
        bCheck = m_emMapDisplaySync == KEMMapDisplaySyncPosScale ? TRUE : FALSE;
        break;
    default:
        bCheck = FALSE;
        break;
    }

    pCmdUI->SetCheck(bCheck);
}