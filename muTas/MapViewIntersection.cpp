#include "stdafx.h"
#include "MapView.h"
#include "IntersectionTopologyAnalysis.h"
#include "BulkDBaseIntersection.h"
#include "BulkDBaseLink.h"
#include "IntersectionNodeType.h"
#include "IntersectionThreeBasicPage.h"
#include "IntersectionBasicPage.h"
#include "IntersectionTopologyAnalysisDlg.h"

bool KMapView::IntersectionTopologyAnalysis()
{
	try
	{
		ShowMapView();

		if(nullptr != m_spIntersectionTopologyAnalysisDlg)
		{
			//m_spIntersectionTopologyAnalysisDlg->InitializeSet();
			m_spIntersectionTopologyAnalysisDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spIntersectionTopologyAnalysisDlg = KIntersectionTopologyAnalysisDlgPtr(new KIntersectionTopologyAnalysisDlg(m_pTarget, this, this));
			m_spIntersectionTopologyAnalysisDlg->InitializeSet();
			m_spIntersectionTopologyAnalysisDlg->Create(KIntersectionTopologyAnalysisDlg::IDD, this);
			m_spIntersectionTopologyAnalysisDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return true;
}


void KMapView::AddIntersectionMapDraw( Integer a_nxIntersectionID, bool a_bRefresh /*= true*/ )
{
	IThemeLayerPtr            spScreenDraw            = m_spThemeControl->GetThemeLayer(ThemeLayerDrawIntersection); 
	KThemDrawIntersection*  pScreenDrawIntersection = (KThemDrawIntersection*)spScreenDraw.get();

	pScreenDrawIntersection->AddDraw(a_nxIntersectionID);
	if (a_bRefresh) {
		MapRefresh();
	}
}


void KMapView::RemoveIntersectionMapDraw( Integer a_nxIntersectionID, bool a_bRefresh /*= true*/ )
{
	IThemeLayerPtr            spScreenDraw            = m_spThemeControl->GetThemeLayer(ThemeLayerDrawIntersection); 
	KThemDrawIntersection*  pScreenDrawIntersection = (KThemDrawIntersection*)spScreenDraw.get();

	pScreenDrawIntersection->RemoveDraw(a_nxIntersectionID);
	if (a_bRefresh) {
		MapRefresh();
	}
}

void KMapView::ClearIntersectionMapDraw() {
    m_spThemeControl->Remove(ThemeLayerDrawIntersection);
}