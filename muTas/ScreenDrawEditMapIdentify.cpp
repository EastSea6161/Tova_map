#include "StdAfx.h"
#include "ScreenDrawEditMapIdentify.h"

#include "CommandWnd.h"
#include "ImChampCommand.h"
//^#include "DBaseConnector.h"
#include "BulkDBaseLink.h"
#include "Target.h"

#include "IMapFeedback.h"


KScreenDrawEditMapIdentify::KScreenDrawEditMapIdentify( IMapView* pMapView) : IScreenDrawEdit(pMapView)
{
	pMapView->SetMapMode(KEMMapModeIdentify);	
	m_emLayer = LINK;
}

KScreenDrawEditMapIdentify::~KScreenDrawEditMapIdentify( void )
{
	if (m_spPropertiesDlg != NULL)
	{
		m_spPropertiesDlg.reset();
	}

	StopEditing();
}

void KScreenDrawEditMapIdentify::StartEditing()
{
	m_pIMapView->SetMapMode(KEMMapModeIdentify);	

	if (m_spScreenDrawFlashLink != NULL)
	{
		m_spScreenDrawFlashLink.reset();
	}

	if (m_spScreenDrawFlashNode != NULL)
	{
		m_spScreenDrawFlashNode.reset();
	}

    if (m_spScreenDrawFlashZone != NULL)
    {
        m_spScreenDrawFlashZone.reset();
    }

	m_bScreenDrawEditing = true;
	SelectCommandMsg();
}


void KScreenDrawEditMapIdentify::Draw()
{
}

void KScreenDrawEditMapIdentify::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spScreenDrawFlashNode != nullptr) {
        m_spScreenDrawFlashNode->MouseDown(nFlags, X, Y, mapX, mapY);
    }

    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
    }

    if (m_spScreenDrawFlashZone != nullptr) {
        m_spScreenDrawFlashZone->MouseDown(nFlags, X, Y, mapX, mapY);
    }
}

void KScreenDrawEditMapIdentify::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{

}

void KScreenDrawEditMapIdentify::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{

}

void KScreenDrawEditMapIdentify::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spScreenDrawFlashNode != nullptr) {
        m_spScreenDrawFlashNode->MouseMove(nFlags, X, Y, mapX, mapY);
    }

    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->MouseMove(nFlags, X, Y, mapX, mapY);
    }

    if (m_spScreenDrawFlashZone != nullptr) {
        m_spScreenDrawFlashZone->MouseMove(nFlags, X, Y, mapX, mapY);
    }
}

void KScreenDrawEditMapIdentify::OnMapRButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{

}

void KScreenDrawEditMapIdentify::OnMapRButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{

}

void KScreenDrawEditMapIdentify::CommandFeedback( CString a_strResult )
{	
	if (a_strResult.CompareNoCase(_T("C")) == 0)
	{
		StopEditing();
		return;
	}
}

void KScreenDrawEditMapIdentify::Canceled()
{
	StopEditing();
}

void KScreenDrawEditMapIdentify::SelectCommandMsg()
{
	if (m_emLayer == NODE)
	{
		KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
		ImChampModalCommandPtr spCommand(new ImChampCommand(_T("노드 선택:"), this));
		if (pCommandWindow)
			pCommandWindow->Command(spCommand, KEMModeless);
		
		m_spScreenDrawFlashNode = KScreenDrawFlashNodePtr(new KScreenDrawFlashNode(m_pIMapView, this));
        // Test
        if (false) {
            std::vector<__int64> vecID;
            for (int i=1; i<120; i++) {
                vecID.push_back(i);
            }
            m_spScreenDrawFlashNode->SetSelectable(vecID);
        }
	}
	else if (m_emLayer == LINK) {
		KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
		ImChampModalCommandPtr spCommand(new ImChampCommand(_T("링크 선택:"), this));
		if (pCommandWindow)
			pCommandWindow->Command(spCommand, KEMModeless);
				
		m_spScreenDrawFlashLink  = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));
        // Test
        if (false) {
            std::vector<__int64> vecID;
            for (int i=1; i<120; i++) {
                vecID.push_back(i);
            }
            m_spScreenDrawFlashLink->SetSelectable(vecID);
        }
	}
    else if (m_emLayer == ZONE) {
        KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
        ImChampModalCommandPtr spCommand(new ImChampCommand(_T("존경계 선택:"), this));
        if (pCommandWindow)
            pCommandWindow->Command(spCommand, KEMModeless);

        m_spScreenDrawFlashZone  = KScreenDrawFlashZonePtr(new KScreenDrawFlashZone(m_pIMapView, this));
    }
		
	m_pIMapView->MapRefresh();
}

void KScreenDrawEditMapIdentify::MapSelectionFeedback( Integer a_nxID )
{
	Integer nxObjectKey = a_nxID;
	CString strWhere;

	if (m_emLayer == NODE)
	{
		strWhere.Format(_T(" node_id = '%I64d'"), nxObjectKey);

		if (m_spPropertiesDlg != NULL)
		{			
			m_spPropertiesDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spPropertiesDlg = KPropertiesDlgPtr(new KPropertiesDlg());
			m_spPropertiesDlg->Create(IDD_2200_PROPERTIES);			
			m_spPropertiesDlg->ShowWindow(SW_SHOW);			
		}
        m_spPropertiesDlg->ShowInformation(m_pIMapView->GetTarget(), TABLE_NODE, strWhere);
	}
	else if (m_emLayer == LINK)
	{
		strWhere.Format(_T(" link_id = '%I64d'"), nxObjectKey);

		if (m_spPropertiesDlg != NULL)
		{			
			m_spPropertiesDlg->ShowWindow(SW_SHOW);			         
		}
		else
		{
			m_spPropertiesDlg = KPropertiesDlgPtr(new KPropertiesDlg());
			m_spPropertiesDlg->Create(IDD_2200_PROPERTIES);			
			m_spPropertiesDlg->ShowWindow(SW_SHOW);
		}

        m_spPropertiesDlg->ShowInformation(m_pIMapView->GetTarget(), TABLE_LINK, strWhere);   
        m_spPropertiesDlg->SetLinkID(nxObjectKey);
	}
    else if (m_emLayer == ZONE)
    {
        strWhere.Format(_T(" zone_id = '%I64d'"), nxObjectKey);

        if (m_spPropertiesDlg != NULL)
        {			
            m_spPropertiesDlg->ShowWindow(SW_SHOW);			         
        }
        else
        {
            m_spPropertiesDlg = KPropertiesDlgPtr(new KPropertiesDlg());
            m_spPropertiesDlg->Create(IDD_2200_PROPERTIES);			
            m_spPropertiesDlg->ShowWindow(SW_SHOW);
        }

        m_spPropertiesDlg->ShowInformation(m_pIMapView->GetTarget(), TABLE_ZONE, strWhere);   
    }

	SelectCommandMsg();
}

void KScreenDrawEditMapIdentify::SaveEditing()
{
}

void KScreenDrawEditMapIdentify::StopEditing()
{
	m_bScreenDrawEditing = false;
	m_pIMapView->SetMapMode(KEMMapModePan);

	KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
	if (pCommandWindow)
		pCommandWindow->SetDefaultCommand();
	m_pIMapView->MapRefresh();
}


void KScreenDrawEditMapIdentify::ActivatedFrameWndNotifyProcess()
{
	if (m_spPropertiesDlg != NULL)
	{			
		if (!m_spPropertiesDlg->IsUserClosed())
		{
			if (!m_spPropertiesDlg->IsWindowVisible())
				m_spPropertiesDlg->ShowWindow(SW_SHOW);
		}
	}
}

void KScreenDrawEditMapIdentify::DeActivatedFrameWndNotifyProcess()
{
	if (m_spPropertiesDlg != NULL)
	{			
		if (!m_spPropertiesDlg->IsUserClosed())
		{
			if (m_spPropertiesDlg->IsWindowVisible())
				m_spPropertiesDlg->ShowWindow(SW_HIDE);
		}	
	}
}


