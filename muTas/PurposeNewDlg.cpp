// PurposeNewDlg.cpp : 구현 파일입니다.
//

#include "StdAfx.h"
#include "KmzApp.h"
#include "PurposeNewDlg.h"
#include "afxdialogex.h"


// KPurposeNewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPurposeNewDlg, KDialogEx)

KPurposeNewDlg::KPurposeNewDlg(CWnd* pParent /*=NULL*/)
: KDialogEx(KPurposeNewDlg::IDD, pParent)
, m_strPurposeName(_T(""))
{

}

KPurposeNewDlg::~KPurposeNewDlg()
{
}

void KPurposeNewDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPurposeName);
}


BEGIN_MESSAGE_MAP(KPurposeNewDlg, KDialogEx)
END_MESSAGE_MAP()


// KPurposeNewDlg 메시지 처리기입니다.


BOOL KPurposeNewDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPurposeNewDlg::OnOK()
{
	UpdateData(TRUE);
	if(m_strPurposeName.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("통행목적 명칭을 입력해 주세요."));
		return;
	}

	KDialogEx::OnOK();
}
