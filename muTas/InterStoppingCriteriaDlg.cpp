// InterStoppingCriteriaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterStoppingCriteriaDlg.h"
#include "afxdialogex.h"


// KInterStoppingCriteriaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterStoppingCriteriaDlg, CDialogEx)

KInterStoppingCriteriaDlg::KInterStoppingCriteriaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(KInterStoppingCriteriaDlg::IDD, pParent)
	, m_nHighwayIteration(100)
	, m_dHighwayRelativeGap(1e-012)
	, m_nHighwayRelativeGapExp(12)
	, m_dUEOBaseScaleFactor(0.0)
{
}

KInterStoppingCriteriaDlg::~KInterStoppingCriteriaDlg()
{
}

void KInterStoppingCriteriaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nHighwayIteration);

	DDX_Text(pDX, IDC_EDIT2, m_dHighwayRelativeGap);
	DDX_Text(pDX, IDC_EDIT4, m_dUEOBaseScaleFactor);
	DDX_Control(pDX, IDC_EDIT2, m_edtHighwayRelativeGap);
	DDX_Control(pDX, IDC_EDIT4, m_edtUEOBaseScaleFactor);

	DDX_Text(pDX, IDC_EDIT3, m_nIntermodalIteration);
	DDX_Text(pDX, IDC_EDIT5, m_dL2Norm);

	DDX_Control(pDX, IDC_COMBO_RELATIVE_GAP, m_cboRelativeGapExp);
	DDX_Control(pDX, IDC_COMBO_SCALE_FACTOR, m_cboScaleFactor);
}


BEGIN_MESSAGE_MAP(KInterStoppingCriteriaDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &KInterStoppingCriteriaDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_RELATIVE_GAP, &KInterStoppingCriteriaDlg::OnCbnSelchangeComboRelativeGap)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALE_FACTOR, &KInterStoppingCriteriaDlg::OnSelchangeComboScaleFactor)
END_MESSAGE_MAP()


int KInterStoppingCriteriaDlg::NumberOfIteration(void)
{
	return m_nHighwayIteration;
}


void KInterStoppingCriteriaDlg::NumberOfIteration(int nIteration)
{
	m_nHighwayIteration = nIteration;
}


double KInterStoppingCriteriaDlg::RelativeGap(void)
{
	return m_dHighwayRelativeGap;
}


void KInterStoppingCriteriaDlg::RelativeGap(double dGap)
{
	m_dHighwayRelativeGap = dGap;
}

double KInterStoppingCriteriaDlg::ScaleFactor(void)
{
	return m_dUEOBaseScaleFactor;
}

void KInterStoppingCriteriaDlg::ScaleFactor(double dGap)
{
	m_dUEOBaseScaleFactor = dGap;
}

void KInterStoppingCriteriaDlg::IntermodalIteration( int a_nIntermodalIteration )
{
	m_nIntermodalIteration = a_nIntermodalIteration;
}

int KInterStoppingCriteriaDlg::IntermodalIteration( void )
{
	return m_nIntermodalIteration;
}

void KInterStoppingCriteriaDlg::IntermodalL2Norm( double a_dL2Norm )
{
	m_dL2Norm = a_dL2Norm;
}

double KInterStoppingCriteriaDlg::IntermodalL2Norm( void )
{
	return m_dL2Norm;
}

// KInterStoppingCriteriaDlg 메시지 처리기입니다.
BOOL KInterStoppingCriteriaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_edtHighwayRelativeGap.EnableRealNumber(true);
	m_edtUEOBaseScaleFactor.EnableRealNumber(true);
	m_edtUEOBaseScaleFactor.EnableMinMax(0.0, 4.0);
	
	UpdateData(FALSE);

	m_cboScaleFactor.AddString(_T("Auto"));
	m_cboScaleFactor.AddString(_T("Manual"));

	m_cboScaleFactor.SetCurSel(m_nScaleFactorCursor);

	OnSelchangeComboScaleFactor();

	for (int i=1; i<21; i++)
	{
		CString strTemp;
		strTemp.Format(_T("%d"), i);
		m_cboRelativeGapExp.AddString(strTemp);
	}

	m_cboRelativeGapExp.SetCurSel(m_nHighwayRelativeGapExp-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterStoppingCriteriaDlg::OnCbnSelchangeComboRelativeGap()
{
	UpdateData(TRUE);
	m_nHighwayRelativeGapExp = m_cboRelativeGapExp.GetCurSel() + 1;
	//m_dHighwayRelativeGap
	CString strTemp;
	strTemp.Format(_T("1e-%d"), m_nHighwayRelativeGapExp);
	m_dHighwayRelativeGap = _tstof(strTemp);

	UpdateData(FALSE);
}

void KInterStoppingCriteriaDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

void KInterStoppingCriteriaDlg::ScaleFactorCursor( int a_nCursorIndex )
{
	m_nScaleFactorCursor = a_nCursorIndex;
}

int KInterStoppingCriteriaDlg::ScaleFactorCursor( void )
{
	return m_nScaleFactorCursor;
}


void KInterStoppingCriteriaDlg::OnSelchangeComboScaleFactor()
{
	m_nScaleFactorCursor = m_cboScaleFactor.GetCurSel();

	if (m_nScaleFactorCursor == 0)
	{
		m_edtUEOBaseScaleFactor.SetWindowText(_T("0.0"));
		UpdateData(TRUE);

		m_edtUEOBaseScaleFactor.SetReadOnly(TRUE);
	}
	else
	{
		m_edtUEOBaseScaleFactor.SetReadOnly(FALSE);
	}

	UpdateData(FALSE);
}
