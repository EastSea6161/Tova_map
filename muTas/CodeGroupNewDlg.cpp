// CodeGroupNewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "CodeGroupNewDlg.h"
#include "afxdialogex.h"


// KCodeGroupNewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCodeGroupNewDlg, KDialogEx)

KCodeGroupNewDlg::KCodeGroupNewDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KCodeGroupNewDlg::IDD, pParent)
	, m_strGroupName(_T(""))
	, m_nGroupKey(0)
{

}

KCodeGroupNewDlg::~KCodeGroupNewDlg()
{
}

void KCodeGroupNewDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strGroupName);
	DDX_Text(pDX, IDC_EDIT2, m_nGroupKey);
}


BEGIN_MESSAGE_MAP(KCodeGroupNewDlg, KDialogEx)
END_MESSAGE_MAP()


// KCodeGroupNewDlg 메시지 처리기입니다.


BOOL KCodeGroupNewDlg::OnInitDialog()
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


void KCodeGroupNewDlg::OnOK()
{
	UpdateData(TRUE);
	if(m_strGroupName.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("코드 그룹 명칭을 입력해 주세요."));
		return;
	}

	KDialogEx::OnOK();
}
