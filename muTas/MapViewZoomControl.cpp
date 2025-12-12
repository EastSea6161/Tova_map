#include "stdafx.h"
#include "Project.h"
#include "MapView.h"

#include "Target.h"
#include "BulkDBaseTransit.h"
#include "BulkDBaseTurn.h"
#include "BulkDBaseNode.h"

void KMapView::OnMapviewFullExtent( void )
{
    FullExtentMap();
}

void KMapView::OnMapExtentStack( UINT nID )
{
    switch( nID )
    {
    case ID_MAPVIEW_BACKWARD : 
        MapCommandDispUndo();
        break;
    case ID_MAPVIEW_FORWARD : 
        MapCommandDispRedo();
        break;
    }
}

void KMapView::OnUpdateMapExtentStack( CCmdUI* pCmdUI )
{
    bool bCanMove(true);

    switch( pCmdUI->m_nID )
    {
    case ID_MAPVIEW_BACKWARD : 
        bCanMove = MapCanDispUndo();
        break;
    case ID_MAPVIEW_FORWARD : 
        bCanMove = MapCanDispRedo();
        break;
    }

    if( true == bCanMove )
        pCmdUI->Enable( TRUE );
    else
        pCmdUI->Enable( FALSE );
}

void KMapView::FullExtentMap( void )
{
    try
    {
		ITxFeatureLayerPtr spFLayer  = MapGetFeatureLayer(KLayerID::Node());
		TxEnvelope         oEnvelope = spFLayer->Extent();

		double dMinX(300000.0), dMinY(400000.0), dMaxX(303000.0), dMaxY(401500.0);

        if (oEnvelope.Area() == 0) {
        }
        else if (oEnvelope.GetMinX() > 0.0 &&
			oEnvelope.GetMaxX() > 0.0 &&
			oEnvelope.GetMinY() > 0.0 &&
			oEnvelope.GetMaxY() > 0.0) {

			dMinX = oEnvelope.GetMinX();
			dMinY = oEnvelope.GetMinY();
			dMaxX = oEnvelope.GetMaxX();
			dMaxY = oEnvelope.GetMaxY();

			double d10Width  = (dMaxX - dMinX) * 0.1;
			double d10Height = (dMaxY - dMinY) * 0.1;

			dMinX -= d10Width;
			dMaxX += d10Width;

			dMinY -= d10Height;
			dMaxY += d10Height;
		}
        else {
            dMinX = oEnvelope.GetMinX();
            dMinY = oEnvelope.GetMinY();

            dMaxX = oEnvelope.GetMaxX();
            dMaxY = oEnvelope.GetMaxY();
        }

        MapCommandZoomExtent(TxEnvelope(dMinX, dMinY, dMaxX, dMaxY));
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


void KMapView::ZoomMap(TxEnvelope& oEnv, bool bRefresh)
{
    MapCommandZoomExtent(oEnv);
    if (bRefresh == true)
        MapRefresh();
}


void KMapView::ExtentToFeautre( CString strTableName, std::vector<Integer> vecNodes, double dx/*=5000.0*/, double dy/*=5000.0*/ )
{
    int nLayerID(-1);

	if (strTableName.CompareNoCase(TABLE_TERMINAL_SCHEDULE) == 0) {
        nLayerID = KLayerID::Node();
	} 
    else {
        AfxMessageBox(_T("Layer 정보를 찾을 수 없습니다."));
        return;
    }

    ITxFeatureLayerPtr spFLayer = MapGetFeatureLayer(nLayerID);
    if (spFLayer == nullptr)
        return;

    TxEnvelope oEnv;
    bool bFind(false);
    for (size_t i=0; i<vecNodes.size(); i++) {
        TxFeaturePtr spFeature = spFLayer->GetFeature(vecNodes[i]);
        if (spFeature == nullptr)
            continue;

        ITxGeometryPtr spGeometry = spFeature->Geometry();
        if (spGeometry == nullptr)
            continue;

        if (bFind == false) {
            oEnv  = spGeometry->Envelope();
            bFind = true;
        } else {
            oEnv.Union(spGeometry->Envelope());
        }
    }

    if (bFind == false) {
        AfxMessageBox(_T("검색 결과가 없습니다."));
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


void KMapView::ZoomExtent(double  a_dMinX,  double   a_dMinY, double  a_dMaxX,  double   a_dMaxY)
{
	MapCommandZoomExtent(TxEnvelope(a_dMinX, a_dMinY, a_dMaxX, a_dMaxY));
    MapRefresh();
}

void KMapView::GetCurrentExtent(double& ar_dMinX, double& ar_dMinY, double& ar_dMaxX, double&  ar_dMaxY)
{
    TxEnvelope oEnv; {
	    MapGetEnvelope(oEnv);
    }

    ar_dMinX = oEnv.GetMinX();
    ar_dMinY = oEnv.GetMinY();

    ar_dMaxX = oEnv.GetMaxX();
    ar_dMaxY = oEnv.GetMaxY();
}

void KMapView:: GetCurrentExtent(TxEnvelope &ar_oEnvelope)
{
	MapGetEnvelope(ar_oEnvelope);
}

void KMapView::GetFullExtent(double& ar_dMinX, double& ar_dMinY, double& ar_dMaxX, double&  ar_dMaxY)
{	
    try
    {   
        KBulkDBaseNode::GetMinMaxXY(m_pTarget, ar_dMinX, ar_dMinY, ar_dMaxX, ar_dMaxY);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        ThrowException(_T("Error - KMapView::GetFullExtent"));
    }    
}