// InterStoppingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterStoppingDlg.h"
#include "afxdialogex.h"


// KInterStoppingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterStoppingDlg, KDialogEx)

KInterStoppingDlg::KInterStoppingDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KInterStoppingDlg::IDD, pParent)
{

}

KInterStoppingDlg::~KInterStoppingDlg()
{
}

void KInterStoppingDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RELATIVEGAP, m_dRelativeGap);
	DDX_Text(pDX, IDC_EDIT_SCALEFACTOR, m_dScaleFactor);
	DDX_Control(pDX, IDC_EDIT_RELATIVEGAP, m_edtRelativeGap);
	DDX_Control(pDX, IDC_EDIT_SCALEFACTOR, m_edtScaleFactor);


	DDX_Text(pDX, IDC_EDIT_OUTITERATION, m_nInterModalIteration);
	DDX_Control(pDX, IDC_EDIT_OUTL2NORM, m_edtOutL2Norm);
	DDX_Text(pDX, IDC_EDIT_OUTL2NORM, m_dInterModalL2Norm);

	DDX_Text(pDX, IDC_EDIT_INTAITERATION, m_nMultimodalIteration);
	DDX_Control(pDX, IDC_EDIT_INL2NORM, m_edtInL2Norm);
	DDX_Text(pDX, IDC_EDIT_INL2NORM, m_dMultimodalL2Norm);

	DDX_Text(pDX, IDC_EDIT_UEITERATION, m_nUEIteration);

	DDX_Control(pDX, IDC_COMBO_RELATIVE_GAP, m_cboRelativeGapExp);
	DDX_Control(pDX, IDC_COMBO_SCALE_FACTOR, m_cboScaleFactor);
}


BEGIN_MESSAGE_MAP(KInterStoppingDlg, KDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_RELATIVE_GAP, &KInterStoppingDlg::OnCbnSelchangeComboRelativeGap)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALE_FACTOR, &KInterStoppingDlg::OnSelchangeComboScaleFactor)
    ON_BN_CLICKED(IDOK, &KInterStoppingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KInterStoppingDlg 메시지 처리기입니다.


BOOL KInterStoppingDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_edtRelativeGap.EnableRealNumber(true);
	m_edtOutL2Norm.EnableRealNumber(true);
	m_edtInL2Norm.EnableRealNumber(true);

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


void KInterStoppingDlg::OnCbnSelchangeComboRelativeGap()
{
	UpdateData(TRUE);
	m_nHighwayRelativeGapExp = m_cboRelativeGapExp.GetCurSel() + 1;
	CString strTemp;
	strTemp.Format(_T("1.0E-%d"), m_nHighwayRelativeGapExp);
	m_dRelativeGap = _tstof(strTemp);
	UpdateData(FALSE);
}

void KInterStoppingDlg::OnSelchangeComboScaleFactor()
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



void KInterStoppingDlg::OnBnClickedOk()
{
    CString strOutNorm;
    GetDlgItemText(IDC_EDIT_OUTL2NORM, strOutNorm);

    if (!QBicStringChecker::IsNumeric(strOutNorm))
    {
        if (KmzSystem::GetLanguage() == KEMKorea) {
            AfxMessageBox(_T("숫자를 입력해 주세요."));
        }
        else {
            AfxMessageBox(_T("Please enter a number"));
        }
        GetDlgItem(IDC_EDIT_OUTL2NORM)->SetFocus();
        return;
    }
    else
    {
        double dOutNorm = _ttof(strOutNorm);
        m_dInterModalL2Norm = dOutNorm;
    }

    CString strInNorm;
    GetDlgItemText(IDC_EDIT_INL2NORM, strInNorm);

    if (!QBicStringChecker::IsNumeric(strInNorm))
    {
        if (KmzSystem::GetLanguage() == KEMKorea) {
            AfxMessageBox(_T("숫자를 입력해 주세요."));
        }
        else {
            AfxMessageBox(_T("Please enter a number"));
        }
        GetDlgItem(IDC_EDIT_INL2NORM)->SetFocus();
        return;
    }
    else
    {
        double dInNorm = _ttof(strInNorm);
        m_dMultimodalL2Norm = dInNorm;
    }

    KDialogEx::OnOK();
}
