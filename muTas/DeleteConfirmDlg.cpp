// DeleteConfirmDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DeleteConfirmDlg.h"
#include "afxdialogex.h"


// KDeleteConfirmDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDeleteConfirmDlg, KDialogEx)

KDeleteConfirmDlg::KDeleteConfirmDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDeleteConfirmDlg::IDD, pParent)
	, m_bDelFile(false)
{
	m_accelerator_key = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DeleteConfirmDlg_ACCELERATOR));
}

KDeleteConfirmDlg::~KDeleteConfirmDlg()
{
}

void KDeleteConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KDeleteConfirmDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KDeleteConfirmDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDeleteConfirmDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KDeleteConfirmDlg 메시지 처리기입니다.


BOOL KDeleteConfirmDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	//Add Icon
	HICON h_icon = LoadIcon(NULL, IDI_QUESTION);
	((CStatic*)GetDlgItem(IDC_STATIC1))->SetIcon(h_icon);


	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);

	SetWindowText(strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDeleteConfirmDlg::OnBnClickedOk()
{
	m_bDelFile = false;

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1);

	if (pButton->GetCheck() == BST_CHECKED)
		m_bDelFile = true;

	KDialogEx::OnOK();
}

bool KDeleteConfirmDlg::IsDeleteFile( void )
{
	return m_bDelFile;
}


BOOL KDeleteConfirmDlg::PreTranslateMessage(MSG* pMsg)
{
	if (!(m_accelerator_key && ::TranslateAccelerator(this->m_hWnd, m_accelerator_key, pMsg)))
	{
		return KDialogEx::PreTranslateMessage(pMsg);
	}
	else
	{
		return TRUE;
	}
}


void KDeleteConfirmDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}
