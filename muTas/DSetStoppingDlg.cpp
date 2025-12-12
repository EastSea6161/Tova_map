/**
 * @file DSetStoppingDlg.cpp
 * @brief KDSetStoppingDlg 구현 파일
 * @author 
 * @date 2011.05.23
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "DSetStoppingDlg.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(KDSetStoppingDlg, KDialogEx)

KDSetStoppingDlg::KDSetStoppingDlg(CWnd* pParent /*=NULL*/)
	:	KDialogEx(KDSetStoppingDlg::IDD, pParent),
		m_nFratarIteration(0),
		m_dFratarGap(0.0f),
		m_nGravityIteration(0),
		m_dGravityError(0)
{

}

KDSetStoppingDlg::~KDSetStoppingDlg()
{
}

void KDSetStoppingDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtFratarIteration);
	DDX_Control(pDX, IDC_EDIT2, m_edtFratarGap);
	DDX_Control(pDX, IDC_EDIT3, m_edtGravityIteration);
	DDX_Control(pDX, IDC_EDIT4, m_edtGravityError);
}



BEGIN_MESSAGE_MAP(KDSetStoppingDlg, KDialogEx)
    ON_BN_CLICKED(IDCANCEL, &KDSetStoppingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()



int KDSetStoppingDlg::FratarIteration(void)
{
	return m_nFratarIteration;
}


void KDSetStoppingDlg::FratarIteration(int nIteration)
{
	m_nFratarIteration = nIteration;
}


double KDSetStoppingDlg::FratarGap(void)
{
	return m_dFratarGap;
}


void KDSetStoppingDlg::FratarGap(double dGap)
{
	m_dFratarGap = dGap;
}


int KDSetStoppingDlg::GravityIteration(void)
{
	return m_nGravityIteration;
}


void KDSetStoppingDlg::GravityIteration(int nIteration)
{
	m_nGravityIteration = nIteration;
}


double KDSetStoppingDlg::GravityError(void)
{
	return m_dGravityError;
}


void KDSetStoppingDlg::GravityError(double nError)
{
	m_dGravityError = nError;
}



// KDSetStoppingDlg 메시지 처리기입니다.
BOOL KDSetStoppingDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CString strText;
	strText.Format(_T("%d"), m_nFratarIteration);
	m_edtFratarIteration.SetWindowText(strText);

	m_edtFratarGap.EnableRealNumber(true);
	strText.Format(_T("%f"), m_dFratarGap);
	m_edtFratarGap.SetWindowText(strText);

	strText.Format(_T("%d"), m_nGravityIteration);
	m_edtGravityIteration.SetWindowText(strText);

	strText.Format(_T("%f"), m_dGravityError);
	m_edtGravityError.SetWindowText(strText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDSetStoppingDlg::OnOK()
{
	CString strText;
	m_edtFratarIteration.GetWindowText(strText);
	m_nFratarIteration = _ttoi(strText);

	m_edtFratarGap.GetWindowText(strText);
	m_dFratarGap = _ttof(strText);

	m_edtGravityIteration.GetWindowText(strText);
	m_nGravityIteration = _ttoi(strText);

	m_edtGravityError.GetWindowText(strText);
	m_dGravityError = _ttof(strText);

	KDialogEx::OnOK();
}


void KDSetStoppingDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KDialogEx::OnCancel();
}
