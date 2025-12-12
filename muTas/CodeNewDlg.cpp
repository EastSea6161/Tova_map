// CodeNewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "CodeNewDlg.h"
#include "afxdialogex.h"


// KCodeNewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCodeNewDlg, KDialogEx)

KCodeNewDlg::KCodeNewDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KCodeNewDlg::IDD, pParent)
	, m_nCode(0)
	, m_strValue(_T(""))
{

}

KCodeNewDlg::~KCodeNewDlg()
{
}

void KCodeNewDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nCode);
	DDX_Text(pDX, IDC_EDIT2, m_strValue);
}


BEGIN_MESSAGE_MAP(KCodeNewDlg, KDialogEx)
END_MESSAGE_MAP()


// KCodeNewDlg 메시지 처리기입니다.


BOOL KCodeNewDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	GetDlgItem(IDC_EDIT2)->SetFocus();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KCodeNewDlg::OnOK()
{
	UpdateData(TRUE);

	if(m_strValue.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("코드번호 값을 입력해 주세요."));
		return;
	}

	KDialogEx::OnOK();
}
