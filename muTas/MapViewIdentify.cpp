#include "stdafx.h"
#include "MapView.h"
#include "ChildFrm.h"
#include "MapFrameWnd.h"


#include "MapFindDlg.h"
#include "ScreenDrawEditMapIdentify.h"

void KMapView::OnIdentifyCommand( void )
{	
	try
	{
		if (m_spScreenDrawEdit != NULL)
		{
			if (m_spScreenDrawEdit->GetType() == ThemeLayerDrawEditScreenIdentify) {
				KScreenDrawEditMapIdentify* pScreenDrawEditMapIdentify = (KScreenDrawEditMapIdentify*)m_spScreenDrawEdit.get();
				pScreenDrawEditMapIdentify->SetIdentifyLayer(m_emLayerIdentify);
				pScreenDrawEditMapIdentify->StartEditing();
				return;
			}
			m_spScreenDrawEdit.reset();
		}

		m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditMapIdentify(this));

		KScreenDrawEditMapIdentify* pScreenDrawEditMapIdentify = (KScreenDrawEditMapIdentify*)m_spScreenDrawEdit.get();
		pScreenDrawEditMapIdentify->SetIdentifyLayer(m_emLayerIdentify);
		pScreenDrawEditMapIdentify->StartEditing();
	}
	catch (...)
	{
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
	}	
}


void KMapView::OnUpdateIdentifyCommand( CCmdUI* pCmdUI )
{
    BOOL bCheck = (KEMMapModeIdentify == m_emMapMode) ? TRUE : FALSE;
    pCmdUI->SetCheck( bCheck );
}


void KMapView::CtrlFind()
{
	try
	{
		if(nullptr != m_spMapFindDlg)
		{
			m_spMapFindDlg->ShowWindow(SW_SHOW);
			m_spMapFindDlg->Initialize();
		}
		else
		{
			m_spMapFindDlg = KMapFindDlgPtr(new KMapFindDlg(this, m_pTarget, this));
			m_spMapFindDlg->Create(KMapFindDlg::IDD, this );
			m_spMapFindDlg->ShowWindow(SW_SHOW);
			m_spMapFindDlg->Initialize();
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KMapView::MapFindDlgNext()
{
	try
	{
		int     nType = 0;
		Integer nxID = 0;
		m_spMapFindDlg->GetUserSelectedInfo(nType, nxID);

        if (nType == 0) {
            PanToFeature(KLayerID::Node(), nxID);
        }
        else {
            PanToFeature(KLayerID::Link(), nxID);
        }
	}
	catch(...)
	{
		TxLogDebugException();
	}
}