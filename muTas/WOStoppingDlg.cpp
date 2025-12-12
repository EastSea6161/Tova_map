// WOStoppingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOStoppingDlg.h"
#include "afxdialogex.h"


// KWOStoppingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KWOStoppingDlg, KDialogEx)

KWOStoppingDlg::KWOStoppingDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KWOStoppingDlg::IDD, pParent)
{

}

KWOStoppingDlg::~KWOStoppingDlg()
{
}

void KWOStoppingDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtIteration);
	DDX_Control(pDX, IDC_EDIT2, m_edtGap);
	DDX_Text(pDX, IDC_EDIT1, m_nIteration);
	DDX_Text(pDX, IDC_EDIT2, m_dL2Norm);
}


BEGIN_MESSAGE_MAP(KWOStoppingDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KWOStoppingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KWOStoppingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KWOStoppingDlg 메시지 처리기입니다.


BOOL KWOStoppingDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_edtGap.EnableRealNumber(true);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KWOStoppingDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	KDialogEx::OnOK();
}


void KWOStoppingDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}

int KWOStoppingDlg::NumberOfIteration( void )
{
	return m_nIteration;
}

void KWOStoppingDlg::NumberOfIteration( int nIteration )
{
	m_nIteration = nIteration;
}

double KWOStoppingDlg::L2Norm( void )
{
	return m_dL2Norm;
}

void KWOStoppingDlg::L2Norm( double dL2Norm )
{
	m_dL2Norm = dL2Norm;
}
