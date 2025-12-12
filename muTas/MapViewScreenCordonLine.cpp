#include "stdafx.h"
#include "MapView.h"

#include "ChildFrm.h"

#include "Target.h"


#include "IncludeScreenDraw.h"
#include "IncludeScreenDrawEdit.h"

#include "SCAnalysisDlg.h"
#include "SCManageDlg.h"

void KMapView::OnToolsScreenline()
{
	try
	{
		m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditScreenLine(this));
		m_spScreenDrawEdit->StartEditing();  
	}
	catch (...)
	{
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
	}	    
}

void KMapView::OnUpdateToolsScreenline(CCmdUI *pCmdUI)
{
    CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
    CChildFrame* pChildFrame = (CChildFrame*)pFrame->GetActiveFrame();
    if(pChildFrame->IsMapViewShowed() == true)
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}


void KMapView::OnToolsCordonline()
{
	try
	{
		m_spScreenDrawEdit = KScreenDrawEditPtr(new KScreenDrawEditCordonLine(this));
		m_spScreenDrawEdit->StartEditing();  
	}
	catch (...)
	{
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
	}
}


void KMapView::OnUpdateToolsCordonline(CCmdUI *pCmdUI)
{
    CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
    CChildFrame* pChildFrame = (CChildFrame*)pFrame->GetActiveFrame();
    if(pChildFrame->IsMapViewShowed() == true)
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}

void KMapView::OnToolsScreenCordonLineAnalysis()
{
	KSCAnalysisDlg oDlg(m_pTarget);
	oDlg.DoModal();
}

void KMapView::OnScreenCordonManage()
{
	ShowMapView();

	if(NULL != m_spSCManageDlg)
	{
		m_spSCManageDlg->ShowWindow(SW_SHOW);
		m_spSCManageDlg->UpdateDrawState();
	}
	else
	{
		m_spSCManageDlg = KSCManageDlgPtr(new KSCManageDlg(this));
		m_spSCManageDlg->SetTarget(m_pTarget);
		m_spSCManageDlg->SetMapView(this);
		m_spSCManageDlg->Create(KSCManageDlg::IDD, this);
		m_spSCManageDlg->ShowWindow(SW_SHOW);
	}
}

void KMapView::UpdateScreenCordonManage( int nType, __int64 nxID )
{
    ShowMapView();

    if(NULL != m_spSCManageDlg)
    {
        m_spSCManageDlg->ShowWindow(SW_SHOW);
        
        if (nType == 1)
            m_spSCManageDlg->ScreenLineRefresh(/*nID*/);
        else
            m_spSCManageDlg->CordonLineRefresh(/*nID*/);

        m_spSCManageDlg->UpdateDrawState();
    }
    else
    {
        m_spSCManageDlg = KSCManageDlgPtr(new KSCManageDlg(this));
        m_spSCManageDlg->SetTarget(m_pTarget);
        m_spSCManageDlg->SetMapView(this);
        m_spSCManageDlg->Create(KSCManageDlg::IDD, this);
        m_spSCManageDlg->ShowWindow(SW_SHOW);
    }
}


void KMapView::OnUpdateScreenCordonManage(CCmdUI *pCmdUI)
{
	CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
	CChildFrame* pChildFrame = (CChildFrame*)pFrame->GetActiveFrame();
	if(pChildFrame->IsMapViewShowed() == true)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

// 지도에서 ScreenLine Display를 모두 감춘다.
void KMapView::ClearScreenLineDraw(bool a_bRefresh)
{
    IThemeLayerPtr          spScreenDraw           = m_spThemeControl->GetThemeLayer(ThemeLayerDrawScreenLine);    
    KThemeDrawScreenLine*  pScreenDrawScreenLine  = (KThemeDrawScreenLine*)spScreenDraw.get();

    pScreenDrawScreenLine->RemoveAllDraw();
    if (a_bRefresh)
	{
		MapRefresh();
	}
}

// 지도에 Screen Line을 추가(여러건을 순차적으로 추가할 경우, 마지막 데이터에 a_bRefresh = true로 한다)
void KMapView::AddScreenLineDraw(Integer a_nxID, COLORREF  a_rgbColor, bool a_bRefresh)
{
    IThemeLayerPtr          spScreenDraw           = m_spThemeControl->GetThemeLayer(ThemeLayerDrawScreenLine);    
    KThemeDrawScreenLine*  pScreenDrawScreenLine  = (KThemeDrawScreenLine*)spScreenDraw.get();

    pScreenDrawScreenLine->AddDraw(m_pTarget, a_nxID, a_rgbColor);
    if (a_bRefresh)
	{
        MapRefresh();
	}
}

// 지도에 Screen Line을 추가(여러건을 순차적으로 제거할 경우, 마지막 데이터에 a_bRefresh = true로 한다)
void KMapView::RemoveScreenLineDraw(Integer a_nxID, bool a_bRefresh)
{
    IThemeLayerPtr          spScreenDraw           = m_spThemeControl->GetThemeLayer(ThemeLayerDrawScreenLine);    
    KThemeDrawScreenLine*  pScreenDrawScreenLine  = (KThemeDrawScreenLine*)spScreenDraw.get();

    pScreenDrawScreenLine->RemoveDraw(a_nxID);
    if (a_bRefresh)
	{
		MapRefresh();
	}
}

// 지도에 Screen Line을 추가(여러건을 순차적으로 수정할 경우, 마지막 데이터에 a_bRefresh = true로 한다)
void KMapView::UpdateScreenLineDraw(Integer a_nxID, COLORREF  a_rgbColor, bool a_bRefresh)
{
	IThemeLayerPtr          spScreenDraw           = m_spThemeControl->GetThemeLayer(ThemeLayerDrawScreenLine);    
	KThemeDrawScreenLine*  pScreenDrawScreenLine  = (KThemeDrawScreenLine*)spScreenDraw.get();

	pScreenDrawScreenLine->RemoveDraw(a_nxID);
	pScreenDrawScreenLine->AddDraw(m_pTarget, a_nxID, a_rgbColor);

	if (a_bRefresh)
	{
		MapRefresh();
	}
}

// 지도에서 CordonLine Display를 모두 감춘다.
void KMapView::ClearCordonLineDraw(bool a_bRefresh)
{
    IThemeLayerPtr          spScreenDraw           = m_spThemeControl->GetThemeLayer(ThemeLayerDrawCordonLine);    
    KThemeDrawCordonLine*  pScreenDrawCordonLine  = (KThemeDrawCordonLine*)spScreenDraw.get();

    pScreenDrawCordonLine->RemoveAllDraw();
    if (a_bRefresh)
	{
		MapRefresh();
	}
}

// 지도에 Cordon Line을 추가(여러건을 순차적으로 추가할 경우, 마지막 데이터에 a_bRefresh = true로 한다)
void KMapView::AddCordonLineDraw(Integer a_nxID, COLORREF  a_rgbColor, bool a_bRefresh)
{
    IThemeLayerPtr          spScreenDraw           = m_spThemeControl->GetThemeLayer(ThemeLayerDrawCordonLine);    
    KThemeDrawCordonLine*  pScreenDrawCordonLine  = (KThemeDrawCordonLine*)spScreenDraw.get();

    pScreenDrawCordonLine->AddDraw(m_pTarget, a_nxID, a_rgbColor);
    if (a_bRefresh)
	{
		MapRefresh();
	}
}

// 지도에 Screen Line을 추가(여러건을 순차적으로 제거할 경우, 마지막 데이터에 a_bRefresh = true로 한다)
void KMapView::RemoveCordonLineDraw(Integer a_nxID, bool a_bRefresh)
{
    IThemeLayerPtr          spScreenDraw           = m_spThemeControl->GetThemeLayer(ThemeLayerDrawCordonLine);    
    KThemeDrawCordonLine*  pScreenDrawCordonLine  = (KThemeDrawCordonLine*)spScreenDraw.get();

    pScreenDrawCordonLine->RemoveDraw(a_nxID);
    if (a_bRefresh)
	{
		MapRefresh();
	}
}

// 지도에 Screen Line을 추가(여러건을 순차적으로 수정할 경우, 마지막 데이터에 a_bRefresh = true로 한다)
void KMapView::UpdateCordonLineDraw(Integer a_nxID, COLORREF  a_rgbColor, bool a_bRefresh)
{
	IThemeLayerPtr          spScreenDraw           = m_spThemeControl->GetThemeLayer(ThemeLayerDrawCordonLine);    
	KThemeDrawCordonLine*  pScreenDrawCordonLine  = (KThemeDrawCordonLine*)spScreenDraw.get();

	pScreenDrawCordonLine->RemoveDraw(a_nxID);
	pScreenDrawCordonLine->AddDraw(m_pTarget, a_nxID, a_rgbColor);

	if (a_bRefresh)
	{
		MapRefresh();
	}
}

