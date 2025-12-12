// InterStoppingCriteriaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "OldInterStoppingCriteriaDlg.h"
#include "afxdialogex.h"


// KInterStoppingCriteriaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KOldInterStoppingCriteriaDlg, CDialogEx)

KOldInterStoppingCriteriaDlg::KOldInterStoppingCriteriaDlg(CWnd* pParent /*=NULL*/)
	:	CDialogEx(KOldInterStoppingCriteriaDlg::IDD, pParent),
		m_nHighwayIteration(0),
		m_dblHighwayGap(0.0),
		m_nOutterIteration(0),
		m_dblOutterL2Norm(0.0)
{

}

KOldInterStoppingCriteriaDlg::~KOldInterStoppingCriteriaDlg()
{
}

void KOldInterStoppingCriteriaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nHighwayIteration);
	DDX_Text(pDX, IDC_EDIT2, m_dblHighwayGap);
	DDX_Text(pDX, IDC_EDIT3, m_nOutterIteration);
	DDX_Text(pDX, IDC_EDIT4, m_dblOutterL2Norm);
	DDX_Control(pDX, IDC_EDIT2, m_edtHighwayGap);
	DDX_Control(pDX, IDC_EDIT4, m_edtOutterGap);
}


BEGIN_MESSAGE_MAP(KOldInterStoppingCriteriaDlg, CDialogEx)
END_MESSAGE_MAP()

int KOldInterStoppingCriteriaDlg::GetHighwayIteration(void)
{
	return m_nHighwayIteration;
}

void KOldInterStoppingCriteriaDlg::SetHighwayIteration(int a_nHighwayIteration)
{
	m_nHighwayIteration = a_nHighwayIteration;
}

double KOldInterStoppingCriteriaDlg::GetHighwayGap(void)
{
	return m_dblHighwayGap;
}

void KOldInterStoppingCriteriaDlg::SetHighwayGap(double a_dblHighwayGap)
{
	m_dblHighwayGap = a_dblHighwayGap;
}

int KOldInterStoppingCriteriaDlg::GetOutterIteration(void)
{
	return m_nOutterIteration;
}

void KOldInterStoppingCriteriaDlg::SetOutterIteration(int a_nOutterIteration)
{
	m_nOutterIteration = a_nOutterIteration;
}

double KOldInterStoppingCriteriaDlg::GetOutterL2Norm(void)
{
	return m_dblOutterL2Norm;
}

void KOldInterStoppingCriteriaDlg::SetOutterL2Norm(double a_dblOutterL2Norm)
{
	m_dblOutterL2Norm = a_dblOutterL2Norm;
}

// KInterStoppingCriteriaDlg 메시지 처리기입니다.

BOOL KOldInterStoppingCriteriaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_edtHighwayGap.EnableRealNumber(true);
	m_edtOutterGap.EnableRealNumber(true);

	UpdateData(FALSE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KOldInterStoppingCriteriaDlg::OnOK()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
