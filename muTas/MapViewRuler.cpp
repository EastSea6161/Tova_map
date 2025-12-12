#include "stdafx.h"
#include "MapView.h"
#include "RulerDlg.h"

void KMapView::OnRuler( void )
{
	try
	{
		ShowMapView();

		if (m_spRulerDlg != nullptr)
		{
			m_spRulerDlg.reset();
			m_spRulerDlg = KRulerDlgPtr(new KRulerDlg(m_pTarget, this));
			m_spRulerDlg->Create(KRulerDlg::IDD, this);
			m_spRulerDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			m_spRulerDlg = KRulerDlgPtr(new KRulerDlg(m_pTarget, this));
			m_spRulerDlg->Create(KRulerDlg::IDD, this);
			m_spRulerDlg->ShowWindow(SW_SHOW);
		}		
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KMapView::RulerSelectLink(ImChampMapSelectionReceiver* a_pMapFeedbackLinkSelect)
{
	try
	{
		m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditSingleLinkSelect(this, a_pMapFeedbackLinkSelect));
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

/*
   룰러 에디팅 종료
   ResetScreenDrawEdit();
*/

// 룰러에서 선택한 링크 표현
void KMapView::RulerSelectionLink( std::vector<TDrawLinkInfo>& ar_vecDrawLinkInfo, bool a_bDrawLabel, bool a_bRefresh )
{
    try
    {
        bool bUseCache(m_bUseCacheScreenDrawRullerLinkSelect);

        IThemeLayerPtr spScreenDraw = m_spThemeControl->GetThemeLayer(ThemeLayerDrawRulerLinkSelect);    
        KScreenDrawRulerLinkSelection* pScreenDraw  = (KScreenDrawRulerLinkSelection*)spScreenDraw.get();
        if (pScreenDraw)
        {
            pScreenDraw->Draw(ar_vecDrawLinkInfo, a_bDrawLabel);
        }

        if (a_bRefresh == false)
            return;

        MapRefresh();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}

// 지도에서 보이지 않게 제거
void KMapView::ClearRulerSelection(bool a_bRefresh)
{
    m_spThemeControl->Remove(ThemeLayerDrawRulerLinkSelect);

    if (a_bRefresh == false)
        return;

    bool bUseCache(m_bUseCacheScreenDrawRullerLinkSelect);
    MapRefresh();
}
