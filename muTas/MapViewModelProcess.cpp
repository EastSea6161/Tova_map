#include "stdafx.h"
#include "MapView.h"
#include "ModelProcessMultiModal.h"
#include "ModelProcessInterModal.h"

void KMapView::OnModelsProcessmultiModal()
{
	ShowMapView();
	try
	{
		if (m_spModelProcessMultiModalDlg == nullptr)
		{
			m_spModelProcessMultiModalDlg = KModelProcessMultiModalPtr(new KModelProcessMultiModal(m_pTarget, this, this));
			m_spModelProcessMultiModalDlg->Create(KModelProcessMultiModal::IDD, this);
			m_spModelProcessMultiModalDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spModelProcessMultiModalDlg->Initialize();
			m_spModelProcessMultiModalDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KMapView::OnModelsProcessInterModal()
{
	ShowMapView();
	try
	{
		if (m_spModelProcessInterModalDlg == nullptr)
		{
			m_spModelProcessInterModalDlg = KModelProcessInterModalPtr(new KModelProcessInterModal(m_pTarget, this, this));
			m_spModelProcessInterModalDlg->Create(KModelProcessInterModal::IDD, this);
			m_spModelProcessInterModalDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spModelProcessInterModalDlg->Initialize();
			m_spModelProcessInterModalDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}
