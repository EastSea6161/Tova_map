#include "stdafx.h"
#include "MapView.h"
#include "KInspectionNodeDlg.h"
#include "KInspectionLinkDlg.h"
#include "KInspectionZoneDlg.h"
#include "KInspectionTurnDlg.h"
#include "KInspectionTransitDlg.h"


void KMapView::OnInspectionNetworkNode()
{
	ShowMapView();

	if(NULL != m_spInspectionNodeDlg)
	{
		m_spInspectionNodeDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spInspectionNodeDlg = KInspectionNodeDlgPtr(new KInspectionNodeDlg(this));
		m_spInspectionNodeDlg->SetTarget(m_pTarget);
		m_spInspectionNodeDlg->Create(KInspectionNodeDlg::IDD, this);
		m_spInspectionNodeDlg->ShowWindow(SW_SHOW);
	}
}


void KMapView::OnInspectionNetworkLink()
{
	ShowMapView();

	if(NULL != m_spInspectionLinkDlg)
	{
		m_spInspectionLinkDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spInspectionLinkDlg = KInspectionLinkDlgPtr(new KInspectionLinkDlg(this));
		m_spInspectionLinkDlg->SetTarget(m_pTarget);
		m_spInspectionLinkDlg->Create(KInspectionLinkDlg::IDD, this);
		m_spInspectionLinkDlg->ShowWindow(SW_SHOW);
	}
}


void KMapView::OnInspectionNetworkZone()
{
	ShowMapView();

	if(NULL != m_spInspectionZoneDlg)
	{
		m_spInspectionZoneDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spInspectionZoneDlg = KInspectionZoneDlgPtr(new KInspectionZoneDlg(this));
		m_spInspectionZoneDlg->SetTarget(m_pTarget);
		m_spInspectionZoneDlg->Create(KInspectionZoneDlg::IDD, this);
		m_spInspectionZoneDlg->ShowWindow(SW_SHOW);

	}
}

void KMapView::OnInspectionNetworkTurn()
{
	ShowMapView();

	if(NULL != m_spInspectionTurnDlg)
	{
		m_spInspectionTurnDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spInspectionTurnDlg = KInspectionTurnDlgPtr(new KInspectionTurnDlg(this));
		m_spInspectionTurnDlg->SetTarget(m_pTarget);
		m_spInspectionTurnDlg->Create(KInspectionTurnDlg::IDD, this);
		m_spInspectionTurnDlg->ShowWindow(SW_SHOW);
	}
}


void KMapView::OnDataInspectionTransit()
{
	ShowMapView();

	if(NULL != m_spInspectionTransitDlg)
	{
		m_spInspectionTransitDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_spInspectionTransitDlg = KInspectionTransitDlgPtr(new KInspectionTransitDlg(this));
		m_spInspectionTransitDlg->SetTarget(m_pTarget);
		m_spInspectionTransitDlg->Create(KInspectionTransitDlg::IDD, this);
		m_spInspectionTransitDlg->ShowWindow(SW_SHOW);
	}
}