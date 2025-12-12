#include "StdAfx.h"
#include "KmzApp.h"
#include "MapView.h"
#include "Project.h"
#include "Target.h"
#include "ImChampFrameWindow.h"
#include "ImChampFileExist.h"
#include "DBaseCheckData.h"
#include "PathVolumeDlg.h"
#include "PathVolumeAreaDlg.h"
#include "PathVolumeAreaTransitDlg.h"
#include "PathVolumeAreaInterModeDlg.h"
#include "PathVolumeAreaInterDlg.h"
#include "PathVolumeTerminalAccEgrDlg.h"

void KMapView::ClearPathVolume()
{
	if (m_spThemeControl)
		m_spThemeControl->Remove(ThemeLayerDrawZoneToZonePathVolume); 
		
	MapRefresh();
}

void KMapView::DrawPathVolume( int a_nType/*0:EachPath, 1:Goguma*/, std::vector<TPathVolumeInfo>& a_rTPathVolumeInfo )
{
	IThemeLayerPtr   spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawZoneToZonePathVolume);    
	KThemDrawZoneToZonePathVolume* pScreenDrawPathVolume   = (KThemDrawZoneToZonePathVolume*)spScreenDraw.get();

	pScreenDrawPathVolume->Draw(a_nType, a_rTPathVolumeInfo);
	MapRefresh();
}


void KMapView::OnPathAnalysis()
{
	try
	{
        if ( !ImChampFileExist::BushFileCheckMessage(m_pTarget))
            return;

		ShowMapView();

		KProject* pProject = ImChampFrameWindow::GetProject();

		m_spPathVolumeDlg = KPathVolumeDlgPtr(new KPathVolumeDlg(m_pTarget, pProject, this, this));
		m_spPathVolumeDlg->Create(KPathVolumeDlg::IDD, this);
		m_spPathVolumeDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMapView::OnPathVolumeHighwayAreaToArea()
{
	try
	{
        if ( !ImChampFileExist::BushFileCheckMessage(m_pTarget))
            return;

		ShowMapView();
        
		KProject* pProject = ImChampFrameWindow::GetProject();

		m_spPathVolumeAreaDlg = KPathVolumeAreaDlgPtr(new KPathVolumeAreaDlg(m_pTarget, pProject, this, this));
		m_spPathVolumeAreaDlg->Create(KPathVolumeAreaDlg::IDD, this);
		m_spPathVolumeAreaDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMapView::OnPathVolumeTransitAreaToArea()
{
	try
	{
		if (!IsExistMultiModalAssignResultFile())
		{
			return;
		}

		ShowMapView();

		KProject* pProject = ImChampFrameWindow::GetProject();

		m_spPathVolumeAreaTransitDlg = KPathVolumeAreaTransitDlgPtr(new KPathVolumeAreaTransitDlg(m_pTarget, pProject, this, this));
		m_spPathVolumeAreaTransitDlg->Create(KPathVolumeAreaTransitDlg::IDD, this);
		m_spPathVolumeAreaTransitDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMapView::OnPathVolumeInterModeAreaToArea()
{
	try
	{
		if (!KDBaseCheckData::GetCheckAssignInterModalResult( m_pTarget ))
		{
			return;
		}

		ShowMapView();

		KProject* pProject = ImChampFrameWindow::GetProject();

		m_spPathVolumeAreaInterModeDlg = KPathVolumeAreaInterModeDlgPtr(new KPathVolumeAreaInterModeDlg(m_pTarget, pProject, this, this));
		m_spPathVolumeAreaInterModeDlg->Create(KPathVolumeAreaInterModeDlg::IDD, this);
		m_spPathVolumeAreaInterModeDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMapView::OnPathVolumeInterAreaToArea()
{
	try
	{
		if (!KDBaseCheckData::GetCheckAssignInterModalResult( m_pTarget ))
		{
			return;
		}

		ShowMapView();

		KProject* pProject = ImChampFrameWindow::GetProject();

		m_spPathVolumeAreaInterDlg = KPathVolumeAreaInterDlgPtr(new KPathVolumeAreaInterDlg(m_pTarget, pProject, this, this));
		m_spPathVolumeAreaInterDlg->Create(KPathVolumeAreaInterDlg::IDD, this);
		m_spPathVolumeAreaInterDlg->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMapView::OnUpdatePathVolumeInterAreaToArea(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void KMapView::OnPathVolumeTerminalAccessEgress()
{
	try
	{
		if (!KDBaseCheckData::GetCheckAssignInterModalResult( m_pTarget ))
		{
			return;
		}

		ShowMapView();

 		m_spPathVolumeTerminalAccEgrDlg = KPathVolumeTerminalAccEgrDlgPtr(new KPathVolumeTerminalAccEgrDlg(m_pTarget, this, this));
 		m_spPathVolumeTerminalAccEgrDlg->Create(KPathVolumeTerminalAccEgrDlg::IDD, this);
 		m_spPathVolumeTerminalAccEgrDlg->ShowWindow(SW_SHOW);

		//KPathVolumeTerminalAccEgrDlg oDlg(m_pTarget, this, this);
		//oDlg.DoModal();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KMapView::OnUpdatePathVolumeTerminalAccessEgress(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}