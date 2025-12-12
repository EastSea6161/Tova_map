// NameInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NameInputDlg.h"
#include "afxdialogex.h"


// KNameInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KNameInputDlg, KDialogEx)

KNameInputDlg::KNameInputDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KNameInputDlg::IDD, pParent)
{
	m_strCaption = _T("");
}

KNameInputDlg::~KNameInputDlg()
{
}

void KNameInputDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KNameInputDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KNameInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KNameInputDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL KNameInputDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if (!m_strCaption.IsEmpty())
		SetWindowText(m_strCaption);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KNameInputDlg::OnBnClickedOk()
{
	CString strText;
	GetDlgItemText(IDC_EDIT1, strText);

	strText = strText.Trim();
	if (strText.GetLength() < 1)
	{
		AfxMessageBox(_T("이름을 입력해주세요"));
		CWnd* pEdit = GetDlgItem(IDC_EDIT1);
		pEdit->SetFocus();
		return;
	}

	m_strName = strText;
	KDialogEx::OnOK();
}


void KNameInputDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}
