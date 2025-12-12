// NaviItemAddDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NaviItemAddDlg.h"
#include "afxdialogex.h"


// KNaviItemAddDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KNaviItemAddDlg, KDialogEx)

KNaviItemAddDlg::KNaviItemAddDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KNaviItemAddDlg::IDD, pParent)
	, m_strNaviName(_T(""))
{

}

KNaviItemAddDlg::~KNaviItemAddDlg()
{
}

void KNaviItemAddDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strNaviName);
}


BEGIN_MESSAGE_MAP(KNaviItemAddDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KNaviItemAddDlg::OnBnClickedOk)
END_MESSAGE_MAP()

CString KNaviItemAddDlg::NaviName( void )
{
	return m_strNaviName;
}

// KNaviItemAddDlg 메시지 처리기입니다.
BOOL KNaviItemAddDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KNaviItemAddDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (0 == m_strNaviName.CompareNoCase(_T("")))
	{
		AfxMessageBox(_T("이름을 입력해 주세요."));
		return;
	}

	KDialogEx::OnOK();
}