#include "stdafx.h"
#include "MapView.h"
#include "ChildFrm.h"
#include "Target.h"
#include "ImChampFrameWindow.h"
#include "ZoningIntegrationDlg.h"
#include "ZoningDivisionDlg.h"

void KMapView::OnZoningDivision()
{
	ShowMapView();

// 	if(NULL != m_spZoningDivisionDlg)
// 	{
// 		m_spZoningDivisionDlg->InitializeNode();
// 		m_spZoningDivisionDlg->ShowWindow(SW_SHOW);
// 	}
// 	else
// 	{
		m_spZoningDivisionDlg = KZoningDivisionDlgPtr(new KZoningDivisionDlg(this));
		m_spZoningDivisionDlg->SetTarget(m_pTarget);
		m_spZoningDivisionDlg->SetMapView(this);
		m_spZoningDivisionDlg->Create(KZoningDivisionDlg::IDD, this);
		m_spZoningDivisionDlg->InitializeNode();
		m_spZoningDivisionDlg->ShowWindow(SW_SHOW);
//	}
}


void KMapView::OnZoningIntegration()
{
	ShowMapView();

// 	if(NULL != m_spZoningIntegrationDlg)
// 	{
// 		m_spZoningIntegrationDlg->InitializeNode();
// 		m_spZoningIntegrationDlg->ShowWindow(SW_SHOW);
// 	}
// 	else
// 	{
		m_spZoningIntegrationDlg = KZoningIntegrationDlgPtr(new KZoningIntegrationDlg(this));
		m_spZoningIntegrationDlg->SetTarget(m_pTarget);
		m_spZoningIntegrationDlg->SetMapView(this);
		m_spZoningIntegrationDlg->Create(KZoningIntegrationDlg::IDD, this);
		m_spZoningIntegrationDlg->InitializeNode();
		m_spZoningIntegrationDlg->ShowWindow(SW_SHOW);
//	}
}


void KMapView::ZoningProcessStart(IMapZoning* a_pMapZoning)
{
    m_pMapZoning = a_pMapZoning;
    SetMapMode(KEMMapModeZoning);
}

void KMapView::ZoningProcessSelectPoint( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_pMapZoning == nullptr)
        return;

    try
    {
        TxPointPtr spPoint = TxPointPtr(new TxPoint(mapX, mapY));
        std::vector<TxPoint> vecPoint; {
            vecPoint.push_back(TxPoint(mapX-50.0, mapY-50.0));
            vecPoint.push_back(TxPoint(mapX-50.0, mapY+50.0));

            vecPoint.push_back(TxPoint(mapX+50.0, mapY+50.0));

            vecPoint.push_back(TxPoint(mapX+50.0, mapY-50.0));
            vecPoint.push_back(TxPoint(mapX-50.0, mapY-50.0));
        }
        
        TxPolygonPtr spPolygon = TxPolygonPtr(new TxPolygon(vecPoint));
        m_pMapZoning->ReceivedZoneInfoFromMap(spPoint, spPolygon);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }     
    
    ZoningProcessClose();
}


void KMapView::ZoningProcessClose()
{
    /*if (m_emMapMode == KEMMapModeZoning)
    {
        SetMapMode(m_emPrevMapMode);
    }*/

    SetMapMode(KEMMapModePan);

    m_pMapZoning = NULL;
}