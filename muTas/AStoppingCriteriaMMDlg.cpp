// AStoppingCriteriaMMDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "AStoppingCriteriaMMDlg.h"
#include "afxdialogex.h"


// KAStoppingCriteriaMMDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAStoppingCriteriaMMDlg, KDialogEx)

KAStoppingCriteriaMMDlg::KAStoppingCriteriaMMDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KAStoppingCriteriaMMDlg::IDD, pParent)
	, m_nHIteration(0)
	, m_dHGap(0)
	, m_nTIteration(0)
	, m_dTGap(0)
{

}

KAStoppingCriteriaMMDlg::~KAStoppingCriteriaMMDlg()
{
}

void KAStoppingCriteriaMMDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nHIteration);
	DDX_Text(pDX, IDC_EDIT2, m_dHGap);
	DDX_Text(pDX, IDC_EDIT3, m_nTIteration);
	DDX_Text(pDX, IDC_EDIT4, m_dTGap);
	DDX_Control(pDX, IDC_EDIT2, m_edtHGap);
	DDX_Control(pDX, IDC_EDIT4, m_edtTGap);
}


BEGIN_MESSAGE_MAP(KAStoppingCriteriaMMDlg, KDialogEx)
END_MESSAGE_MAP()



int KAStoppingCriteriaMMDlg::HighwayIteration(void)
{
	return m_nHIteration;
}


void KAStoppingCriteriaMMDlg::HighwayIteration(int nIteration)
{
	m_nHIteration = nIteration;
}


double KAStoppingCriteriaMMDlg::RelativeGap(void)
{
	return m_dHGap;
}


void KAStoppingCriteriaMMDlg::RelativeGap(double dGap)
{
	m_dHGap = dGap;
}


int KAStoppingCriteriaMMDlg::TransitIteration(void)
{
	return m_nTIteration;
}


void KAStoppingCriteriaMMDlg::TransitIteration(int nIteration)
{
	m_nTIteration = nIteration;
}


double KAStoppingCriteriaMMDlg::NormalizedGap(void)
{
	return m_dTGap;
}


void KAStoppingCriteriaMMDlg::NormalizedGap(double dGap)
{
	m_dTGap = dGap;
}


// KAStoppingCriteriaMMDlg 메시지 처리기입니다.
BOOL KAStoppingCriteriaMMDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_edtHGap.EnableRealNumber(true);
	m_edtTGap.EnableRealNumber(true);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAStoppingCriteriaMMDlg::OnOK()
{
	UpdateData(TRUE);

	KDialogEx::OnOK();
}
