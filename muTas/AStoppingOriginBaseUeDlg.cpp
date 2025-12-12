// AStoppingCriteriaOriginBaseDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AStoppingOriginBaseUeDlg.h"
#include "afxdialogex.h"


// KAStoppingCriteriaOriginBaseDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAStoppingOriginBaseUeDlg, KDialogEx)

KAStoppingOriginBaseUeDlg::KAStoppingOriginBaseUeDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KAStoppingOriginBaseUeDlg::IDD, pParent)
	, m_nHighwayIteration(100)
	, m_dHighwayRelativeGap(1e-012)
    , m_nHighwayRelativeGapExp(12)
	, m_nUEOBaseZoneCentroidThrought(1)
	, m_dUEOBaseScaleFactor(0.0)
{
}

KAStoppingOriginBaseUeDlg::~KAStoppingOriginBaseUeDlg()
{
}

void KAStoppingOriginBaseUeDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nHighwayIteration);

	DDX_Text(pDX, IDC_EDIT2, m_dHighwayRelativeGap);
	DDX_Text(pDX, IDC_EDIT4, m_dUEOBaseScaleFactor);
	DDX_Control(pDX, IDC_EDIT2, m_edtHighwayRelativeGap);
	DDX_Control(pDX, IDC_EDIT4, m_edtUEOBaseScaleFactor);

	DDX_Control(pDX, IDC_COMBO_ADJUST_FACTOR, m_cboAdjustFactor);
	DDX_Control(pDX, IDC_COMBO_RELATIVE_GAP, m_cboRelativeGapExp);
	DDX_Control(pDX, IDC_COMBO_SCALE_FACTOR, m_cboScaleFactor);
}


BEGIN_MESSAGE_MAP(KAStoppingOriginBaseUeDlg, KDialogEx)
    ON_BN_CLICKED(IDCANCEL, &KAStoppingOriginBaseUeDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &KAStoppingOriginBaseUeDlg::OnBnClickedOk)
    ON_CBN_SELCHANGE(IDC_COMBO_ADJUST_FACTOR, &KAStoppingOriginBaseUeDlg::OnCbnSelchangeComboAdjustFactor)
    ON_CBN_SELCHANGE(IDC_COMBO_RELATIVE_GAP, &KAStoppingOriginBaseUeDlg::OnCbnSelchangeComboRelativeGap)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALE_FACTOR, &KAStoppingOriginBaseUeDlg::OnSelchangeComboScaleFactor)
END_MESSAGE_MAP()


int KAStoppingOriginBaseUeDlg::NumberOfIteration(void)
{
	return m_nHighwayIteration;
}


void KAStoppingOriginBaseUeDlg::NumberOfIteration(int nIteration)
{
	m_nHighwayIteration = nIteration;
}


double KAStoppingOriginBaseUeDlg::RelativeGap(void)
{
	return m_dHighwayRelativeGap;
}


void KAStoppingOriginBaseUeDlg::RelativeGap(double dGap)
{
	m_dHighwayRelativeGap = dGap;
}


int KAStoppingOriginBaseUeDlg::ZoneCentroidThrought(void)
{
	return m_nUEOBaseZoneCentroidThrought;
}


void KAStoppingOriginBaseUeDlg::ZoneCentroidThrought(int nIteration)
{
	m_nUEOBaseZoneCentroidThrought = nIteration;
}


double KAStoppingOriginBaseUeDlg::ScaleFactor(void)
{
	return m_dUEOBaseScaleFactor;
}


void KAStoppingOriginBaseUeDlg::ScaleFactor(double dGap)
{
	m_dUEOBaseScaleFactor = dGap;
}

// KAStoppingCriteriaOriginBaseDlg 메시지 처리기입니다.
BOOL KAStoppingOriginBaseUeDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_edtHighwayRelativeGap.EnableRealNumber(true);
	m_edtUEOBaseScaleFactor.EnableRealNumber(true);
    m_edtUEOBaseScaleFactor.EnableMinMax(0.0, 4.0);
	UpdateData(FALSE);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_cboAdjustFactor.AddString(_T("아니오"));
		m_cboAdjustFactor.AddString(_T("예"));
	}
	else {
		m_cboAdjustFactor.AddString(_T("No"));
		m_cboAdjustFactor.AddString(_T("Yes"));
	}

	m_cboAdjustFactor.SetCurSel(m_nUEOBaseZoneCentroidThrought);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        m_cboScaleFactor.AddString(_T("수동"));
        m_cboScaleFactor.AddString(_T("자동"));
    }
    else {
        m_cboScaleFactor.AddString(_T("Manual"));
        m_cboScaleFactor.AddString(_T("Auto"));
    }	

	m_cboScaleFactor.SetCurSel(m_nScaleFactorCursor);

	OnSelchangeComboScaleFactor();

    for (int i=1; i<21; i++)
    {
        CString strTemp;
        strTemp.Format(_T("%d"), i);
        m_cboRelativeGapExp.AddString(strTemp);
    }

    m_cboRelativeGapExp.SetCurSel(m_nHighwayRelativeGapExp-1);

	OnCbnSelchangeComboRelativeGap();
        
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KAStoppingOriginBaseUeDlg::OnCbnSelchangeComboAdjustFactor()
{
    m_nUEOBaseZoneCentroidThrought = m_cboAdjustFactor.GetCurSel();
}

void KAStoppingOriginBaseUeDlg::OnCbnSelchangeComboRelativeGap()
{
    UpdateData(TRUE);
    m_nHighwayRelativeGapExp = m_cboRelativeGapExp.GetCurSel() + 1;
    //m_dHighwayRelativeGap
    CString strTemp;
    strTemp.Format(_T("1.0E-%d"), m_nHighwayRelativeGapExp);
    m_dHighwayRelativeGap = _tstof(strTemp);

    UpdateData(FALSE);
}

void KAStoppingOriginBaseUeDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KDialogEx::OnCancel();
}


void KAStoppingOriginBaseUeDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
    KDialogEx::OnOK();
}

void KAStoppingOriginBaseUeDlg::ScaleFactorCursor( int a_nCursorIndex )
{
	m_nScaleFactorCursor = a_nCursorIndex;
}

int KAStoppingOriginBaseUeDlg::ScaleFactorCursor( void )
{
	return m_nScaleFactorCursor;
}


void KAStoppingOriginBaseUeDlg::OnSelchangeComboScaleFactor()
{
	UpdateData(TRUE);

	m_nScaleFactorCursor = m_cboScaleFactor.GetCurSel();

	if (m_nScaleFactorCursor == 0)
	{
		m_edtUEOBaseScaleFactor.SetWindowText(_T("0.5"));
		UpdateData(TRUE);
		m_edtUEOBaseScaleFactor.SetReadOnly(FALSE);
	}
	else
	{
		m_edtUEOBaseScaleFactor.SetWindowText(_T("0.0"));
		UpdateData(TRUE);

		m_edtUEOBaseScaleFactor.SetReadOnly(TRUE);
	}

	UpdateData(FALSE);
}
