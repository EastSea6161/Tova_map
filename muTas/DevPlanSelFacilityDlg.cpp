// DevPlanSelFacilityDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanSelFacilityDlg.h"
#include "afxdialogex.h"


// KDevPlanSelFacilityDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanSelFacilityDlg, KDialogEx)

KDevPlanSelFacilityDlg::KDevPlanSelFacilityDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanSelFacilityDlg::IDD, pParent)
{

}

KDevPlanSelFacilityDlg::~KDevPlanSelFacilityDlg()
{
}

void KDevPlanSelFacilityDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KDevPlanSelFacilityDlg, KDialogEx)
END_MESSAGE_MAP()


// KDevPlanSelFacilityDlg 메시지 처리기입니다.


BOOL KDevPlanSelFacilityDlg::OnInitDialog()
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
