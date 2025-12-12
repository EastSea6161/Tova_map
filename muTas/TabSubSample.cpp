// KTabSubSample.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TabSubSample.h"
#include "afxdialogex.h"


// KTabSubSample 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTabSubSample, KDialogEx)

KTabSubSample::KTabSubSample(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTabSubSample::IDD, pParent)
{

}

KTabSubSample::~KTabSubSample()
{
}

void KTabSubSample::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KTabSubSample, KDialogEx)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// KTabSubSample 메시지 처리기입니다.

BOOL KTabSubSample::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    SetBackgroundColor(RGB(255,255,255));
    SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDOK,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTabSubSample::OnSize(UINT nType, int cx, int cy)
{
    KDialogEx::OnSize(nType, cx, cy);    
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


