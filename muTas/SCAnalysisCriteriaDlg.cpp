// SCAnalysisCriteriaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCAnalysisCriteriaDlg.h"
#include "afxdialogex.h"


// KSCAnalysisCriteriaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSCAnalysisCriteriaDlg, KDialogEx)

KSCAnalysisCriteriaDlg::KSCAnalysisCriteriaDlg(int a_nIteration, double a_dToleranceError, CWnd* pParent /*=NULL*/)
	: KDialogEx(KSCAnalysisCriteriaDlg::IDD, pParent)
	, m_nIteration(a_nIteration)
	, m_dToleranceError(a_dToleranceError)
{

}

KSCAnalysisCriteriaDlg::~KSCAnalysisCriteriaDlg()
{
}

void KSCAnalysisCriteriaDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text	(pDX, IDC_EDIT1, m_nIteration);
	DDX_Text	(pDX, IDC_EDIT2, m_dToleranceError);
	DDX_Control	(pDX, IDC_EDIT2, m_edtToleranceError);
}


BEGIN_MESSAGE_MAP(KSCAnalysisCriteriaDlg, KDialogEx)
END_MESSAGE_MAP()


// KSCAnalysisCriteriaDlg 메시지 처리기입니다.
int KSCAnalysisCriteriaDlg::Iteration()
{
	return m_nIteration;
}


double KSCAnalysisCriteriaDlg::ToleranceError()
{
	return m_dToleranceError;
}


BOOL KSCAnalysisCriteriaDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_edtToleranceError.EnableRealNumber(true);
	m_edtToleranceError.EnableMinMax(0.0, 1000.0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}