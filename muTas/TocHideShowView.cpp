// TocHideShowView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "TocHideShowView.h"
#include "ImChampFrameWindow.h"

// KTocHideShowView

IMPLEMENT_DYNCREATE(KTocHideShowView, CFormView)

KTocHideShowView::KTocHideShowView()
	: CFormView(KTocHideShowView::IDD)
{

}

KTocHideShowView::~KTocHideShowView()
{
}

void KTocHideShowView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(KTocHideShowView, CFormView)
    ON_BN_CLICKED(IDC_BUTTON_SHOW, &KTocHideShowView::OnBnClickedButtonShow)
END_MESSAGE_MAP()


// KTocHideShowView 진단입니다.

#ifdef _DEBUG
void KTocHideShowView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void KTocHideShowView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// KTocHideShowView 메시지 처리기입니다.
void KTocHideShowView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();    
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

    CFont& oFont = XTPAuxData().font;	
    GetDlgItem(IDC_BUTTON_SHOW)->SetFont(&oFont);
}

void KTocHideShowView::OnBnClickedButtonShow()
{
    CChildFrame* pFrame = ImChampFrameWindow::GetActiveFrame();
    if (pFrame)
        pFrame->ShowToc(true);
}
