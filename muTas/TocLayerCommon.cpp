// TocLayerCommon.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocLayerCommon.h"
#include "afxdialogex.h"


// KTocLayerCommon 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTocLayerCommon, KDialogEx)

KTocLayerCommon::KTocLayerCommon(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTocLayerCommon::IDD, pParent)
{

}

KTocLayerCommon::~KTocLayerCommon()
{
}

void KTocLayerCommon::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KTocLayerCommon, KDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// KTocLayerCommon 메시지 처리기입니다.
BOOL KTocLayerCommon::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetBackgroundColor(RGB(255,255,255));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH KTocLayerCommon::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}