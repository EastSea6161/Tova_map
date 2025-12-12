// WStoppingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WStoppingDlg.h"
#include "afxdialogex.h"


// KWStoppingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KWStoppingDlg, KDialogEx)

KWStoppingDlg::KWStoppingDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KWStoppingDlg::IDD, pParent)
{

}

KWStoppingDlg::~KWStoppingDlg()
{
}

void KWStoppingDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RELATIVEGAP, m_dRelativeGap);
	DDX_Text(pDX, IDC_EDIT_SCALEFACTOR, m_dScaleFactor);
	DDX_Text(pDX, IDC_EDIT_L2NORM, m_dL2Norm);
	DDX_Control(pDX, IDC_EDIT_RELATIVEGAP, m_edtRelativeGap);
	DDX_Control(pDX, IDC_EDIT_SCALEFACTOR, m_edtScaleFactor);
	DDX_Control(pDX, IDC_EDIT_L2NORM, m_edtL2Norm);

	DDX_Text(pDX, IDC_EDIT_TAITERATION, m_nTAIteration);
	DDX_Text(pDX, IDC_EDIT_UEITERATION, m_nUEIteration);

	DDX_Control(pDX, IDC_COMBO_RELATIVE_GAP, m_cboRelativeGapExp);
	DDX_Control(pDX, IDC_COMBO_SCALE_FACTOR, m_cboScaleFactor);

}


BEGIN_MESSAGE_MAP(KWStoppingDlg, KDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_RELATIVE_GAP, &KWStoppingDlg::OnCbnSelchangeComboRelativeGap)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALE_FACTOR, &KWStoppingDlg::OnSelchangeComboScaleFactor)
END_MESSAGE_MAP()


// KWStoppingDlg 메시지 처리기입니다.


BOOL KWStoppingDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_edtRelativeGap.EnableRealNumber(true);
	m_edtL2Norm.EnableRealNumber(true);
	m_edtScaleFactor.EnableRealNumber(true);
	m_edtScaleFactor.EnableMinMax(0.0, 4.0);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        m_cboScaleFactor.AddString(_T("수동"));
        m_cboScaleFactor.AddString(_T("자동"));
    }
    else {
        m_cboScaleFactor.AddString(_T("Manual"));
        m_cboScaleFactor.AddString(_T("Auto"));
    }	

	m_cboScaleFactor.SetCurSel(m_nScaleFactorCursor);

	for (int i=1; i<21; i++)
	{
		CString strTemp;
		strTemp.Format(_T("%d"), i);
		m_cboRelativeGapExp.AddString(strTemp);
	}

	m_cboRelativeGapExp.SetCurSel(m_nHighwayRelativeGapExp-1);

	OnCbnSelchangeComboRelativeGap();
	OnSelchangeComboScaleFactor();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KWStoppingDlg::OnSelchangeComboScaleFactor()
{
	UpdateData(TRUE);

	m_nScaleFactorCursor = m_cboScaleFactor.GetCurSel();

	if (m_nScaleFactorCursor == 0)
	{
		CString strSCaleFactor = _T("");
		strSCaleFactor.Format(_T("%f"), m_dScaleFactor);
		m_edtScaleFactor.SetWindowText(strSCaleFactor);
		UpdateData(TRUE);
		m_edtScaleFactor.SetReadOnly(FALSE);
	}
	else
	{
		m_edtScaleFactor.SetWindowText(_T("0.0"));
		UpdateData(TRUE);

		m_edtScaleFactor.SetReadOnly(TRUE);
	}

	UpdateData(FALSE);
}

void KWStoppingDlg::OnCbnSelchangeComboRelativeGap()
{
	UpdateData(TRUE);
	m_nHighwayRelativeGapExp = m_cboRelativeGapExp.GetCurSel() + 1;
	CString strTemp;
	strTemp.Format(_T("1.0E-%d"), m_nHighwayRelativeGapExp);
	m_dRelativeGap = _tstof(strTemp);

	UpdateData(FALSE);
}
