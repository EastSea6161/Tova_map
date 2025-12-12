#include "StdAfx.h"
#include "MapView.h"
#include "FDefaultPolicyContainerDlg.h"

void KMapView::OnFDefaultPolicyContainer()
{
	ShowMapView();

	try
	{
		m_spFDefPolicyContainer = KFDefaultPolicyContainerDlgPtr(new KFDefaultPolicyContainerDlg(m_pTarget, this, this));
		m_spFDefPolicyContainer->Create(KFDefaultPolicyContainerDlg::IDD, this);
		m_spFDefPolicyContainer->ShowWindow(SW_SHOW);
 	}
	catch(...)
	{
		TxLogDebugException();
	}
}