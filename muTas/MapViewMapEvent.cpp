#include "stdafx.h"
#include "MapFrameWnd.h"
#include "MapView.h"
#include "MapDiplayMonitor.h"
#include "MainFrameWnd.h"
#include "TocFormView.h"
#include "IOTransitTableView.h"
#include "LayerXmlRenderer.h"


void KMapView::OnShapeEditPopupCommand(UINT nID)
{
	if (m_spShapeEditController != NULL)
		m_spShapeEditController->OnShapeEditPopupCommand(nID);
}

void KMapView::ShowPopupMenuNetworkEdit( long X, long Y )
{
	if (m_spShapeEditController == NULL)
		return;

	UINT nMenuID;
	int  nMenuPos;
	bool bShow = m_spShapeEditController->GetShapeEditPopupInfo(nMenuID, nMenuPos);

	if (bShow == false)
		return;

	CPoint ptClick(X, Y);
	ClientToScreen(&ptClick);

	CMenu menu;
	menu.LoadMenu(nMenuID);
	CMenu* pPopup = menu.GetSubMenu(nMenuPos);

	KMapFrameWnd*    pFrameWnd    = (KMapFrameWnd*)(GetParentFrame());
	CXTPCommandBars* pCommandBars = pFrameWnd->GetCommandBars();
	pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, ptClick.x, ptClick.y, this);
}


