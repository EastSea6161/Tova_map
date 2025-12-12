#include "stdafx.h"
#include "KmzApp.h"
#include "MapView.h"
#include "Project.h"
#include "Target.h"
//^^ #include "TableOwner.h"
#include "IncludeMap.h"
#include "ImChampFileExist.h"
#include "ImChampFrameWindow.h"
#include "LinkAnalysisDlg.h"

void KMapView::SelectLinkPath(ImChampMapSelectionReceiver* a_pMapFeedbackLinkSelect)
{	   
    try
    {
        m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditMultiLinkSelect(this, a_pMapFeedbackLinkSelect));
        m_spScreenDrawEdit->StartEditing();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
    }
    catch (...)
    {
    	TxLogDebugException();
        AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
    }
}

void KMapView::DrawLinkViaPathThemes(std::vector<TDrawLinkInfo>& a_vecLinkInfo,  std::vector<Integer>& a_vecSelectedLink, bool a_bDrawLabel)
{
	IThemeLayerPtr   spScreenDraw    = m_spThemeControl->GetThemeLayer(ThemeLayerDrawLinkViaPathVolume);    
	if (spScreenDraw)
	{
		KThemDrawLinkViaPathVolume* pScreenDrawLink = (KThemDrawLinkViaPathVolume*)spScreenDraw.get();
		pScreenDrawLink->Draw(a_vecLinkInfo, a_vecSelectedLink, a_bDrawLabel);
	}
	
	MapRefresh();
}

void KMapView::SetDrawLabelLinkViaPathThemes(bool a_bDrawLabel)
{
	IThemeLayerPtr   spScreenDraw    = m_spThemeControl->GetThemeLayer(ThemeLayerDrawLinkViaPathVolume);    
	if (spScreenDraw)
	{
		KThemDrawLinkViaPathVolume* pScreenDrawLink = (KThemDrawLinkViaPathVolume*)spScreenDraw.get();
		pScreenDrawLink->SetDrawLabel(a_bDrawLabel);
	}

	MapRefresh();
}

void KMapView::ClearLinkViaPathThemes()
{
	if (m_spThemeControl)
		m_spThemeControl->Remove(ThemeLayerDrawLinkViaPathVolume);

	MapRefresh();
}


void KMapView::OnLinkAnalysis()
{
	try
	{
		if ( !ImChampFileExist::BushFileCheckMessage(m_pTarget))
            return;

		ShowMapView();

		KIOTables* pIOTables = m_pTarget->Tables();
		ASSERT(NULL != pIOTables);

		KIOTable* pIOTable = pIOTables->FindTable(TABLE_LINK);
		ASSERT(NULL != pIOTable);

		KProject* pProject = ImChampFrameWindow::GetProject();

		if(NULL != m_spLinkAnalysisDlg)
		{
			m_spLinkAnalysisDlg.reset();
			m_spLinkAnalysisDlg = KLinkAnalysisDlgPtr(new KLinkAnalysisDlg(m_pTarget, pProject, this, this));
			m_spLinkAnalysisDlg->SetLinkTable(pIOTable);
			m_spLinkAnalysisDlg->Create(KLinkAnalysisDlg::IDD, this);
			m_spLinkAnalysisDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spLinkAnalysisDlg = KLinkAnalysisDlgPtr(new KLinkAnalysisDlg(m_pTarget, pProject, this, this));
			m_spLinkAnalysisDlg->SetLinkTable(pIOTable);
			m_spLinkAnalysisDlg->Create(KLinkAnalysisDlg::IDD, this);
			m_spLinkAnalysisDlg->ShowWindow(SW_SHOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}