// WoStoppingTransitDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WoStoppingTransitDlg.h"
#include "afxdialogex.h"
//#include "StringChecker.h"

// KWoStoppingTransitDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KWoStoppingTransitDlg, KDialogEx)

KWoStoppingTransitDlg::KWoStoppingTransitDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KWoStoppingTransitDlg::IDD, pParent)
	, m_nOuterIteration(0)
	, m_nInnerIteration(0)
	, m_dOuterL2Norm(0.0)
	, m_dInnerL2Norm(0.0)
{

}

KWoStoppingTransitDlg::~KWoStoppingTransitDlg()
{
}

void KWoStoppingTransitDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KWoStoppingTransitDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KWoStoppingTransitDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KWoStoppingTransitDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

int KWoStoppingTransitDlg::OutterNumberOfIteration( void )
{
	return m_nOuterIteration;
}

void KWoStoppingTransitDlg::OutterNumberOfIteration( int nIteration )
{
	m_nOuterIteration = nIteration;
}

int KWoStoppingTransitDlg::InnerNumberOfIteration( void )
{
	return m_nInnerIteration;
}

void KWoStoppingTransitDlg::InnerNumberOfIteration( int nIteration )
{
	m_nInnerIteration = nIteration;
}

double KWoStoppingTransitDlg::OutterL2Norm( void )
{
	return m_dOuterL2Norm;
}

void KWoStoppingTransitDlg::OutterL2Norm( double dL2Norm )
{
	m_dOuterL2Norm = dL2Norm;
}

double KWoStoppingTransitDlg::InnerL2Norm( void )
{
	return m_dInnerL2Norm;
}

void KWoStoppingTransitDlg::InnerL2Norm( double dL2Norm )
{
	m_dInnerL2Norm = dL2Norm;
}


// KWoStoppingTransitDlg 메시지 처리기입니다.


BOOL KWoStoppingTransitDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CString strValue(_T(""));
	strValue.Format(_T("%d"), m_nOuterIteration);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OUTITER);
	SetDlgItemText(IDC_EDIT_OUTITER, strValue);

	strValue.Format(_T("%d"), m_nInnerIteration);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_INITER);
	SetDlgItemText(IDC_EDIT_INITER, strValue);

	strValue.Format(_T("%.1f"), m_dOuterL2Norm);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OUTNORM);
	SetDlgItemText(IDC_EDIT_OUTNORM, strValue);

	strValue.Format(_T("%.1f"), m_dInnerL2Norm);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_INNORM);
	SetDlgItemText(IDC_EDIT_INNORM, strValue);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KWoStoppingTransitDlg::OnBnClickedOk()
{
	
	CString strOuterIter;
	GetDlgItemText(IDC_EDIT_OUTITER, strOuterIter);

	if (!QBicStringChecker::IsNumeric(strOuterIter))
	{
		AfxMessageBox(_T("숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_OUTITER)->SetFocus();
		return;
	}
	else
	{
		int nOuterIter = _ttoi(strOuterIter);
		m_nOuterIteration	= nOuterIter;
	}

	CString strInnerIter;
	GetDlgItemText(IDC_EDIT_INITER, strInnerIter);

	if (!QBicStringChecker::IsNumeric(strInnerIter))
	{
		AfxMessageBox(_T("숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_INITER)->SetFocus();
		return;
	}
	else
	{
		int nInnerIter = _ttoi(strInnerIter);
		m_nInnerIteration	= nInnerIter;
	}


	CString strOutNorm;
	GetDlgItemText(IDC_EDIT_OUTNORM, strOutNorm);

	if (!QBicStringChecker::IsNumeric(strOutNorm))
	{
        if (KmzSystem::GetLanguage() == KEMKorea) {
            AfxMessageBox(_T("숫자를 입력해 주세요."));
        }
        else {
            AfxMessageBox(_T("Please enter a number"));
        }
		
		GetDlgItem(IDC_EDIT_OUTNORM)->SetFocus();
		return;
	}
	else
	{
		double dOutNorm = _ttof(strOutNorm);
		m_dOuterL2Norm	= dOutNorm;
	}

	CString strInNorm;
	GetDlgItemText(IDC_EDIT_INNORM, strInNorm);

	if (!QBicStringChecker::IsNumeric(strInNorm))
	{
        if (KmzSystem::GetLanguage() == KEMKorea) {
            AfxMessageBox(_T("숫자를 입력해 주세요."));
        }
        else {
            AfxMessageBox(_T("Please enter a number"));
        }

		GetDlgItem(IDC_EDIT_INNORM)->SetFocus();
		return;
	}
	else
	{
		double dInNorm = _ttof(strInNorm);
		m_dInnerL2Norm	= dInNorm;
	}

	KDialogEx::OnOK();
}


void KWoStoppingTransitDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}
