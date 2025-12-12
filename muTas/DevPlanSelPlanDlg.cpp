// DevPlanSelPlanDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanSelPlanDlg.h"
#include "afxdialogex.h"


// KDevPlanSelPlanDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanSelPlanDlg, KDialogEx)

KDevPlanSelPlanDlg::KDevPlanSelPlanDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanSelPlanDlg::IDD, pParent)
{

}

KDevPlanSelPlanDlg::~KDevPlanSelPlanDlg()
{
}

void KDevPlanSelPlanDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KDevPlanSelPlanDlg, KDialogEx)
END_MESSAGE_MAP()


// KDevPlanSelPlanDlg 메시지 처리기입니다.


BOOL KDevPlanSelPlanDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
