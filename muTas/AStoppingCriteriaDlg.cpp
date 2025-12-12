// AStoppingCriteriaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "AStoppingCriteriaDlg.h"
#include "afxdialogex.h"


// KAStoppingCriteriaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAStoppingCriteriaDlg, KDialogEx)

KAStoppingCriteriaDlg::KAStoppingCriteriaDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KAStoppingCriteriaDlg::IDD, pParent)
	, m_nIteration(0)
	, m_dGap(0.0f)
{

}

KAStoppingCriteriaDlg::~KAStoppingCriteriaDlg()
{
}

void KAStoppingCriteriaDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtIteration);
	DDX_Control(pDX, IDC_EDIT2, m_edtGap);
	DDX_Control(pDX, IDC_STCASSIGN, m_stcAssign);
	DDX_Control(pDX, IDC_STCGAP, m_stcGap);
	DDX_Text(pDX, IDC_EDIT1, m_nIteration);
	DDX_Text(pDX, IDC_EDIT2, m_dGap);
}


BEGIN_MESSAGE_MAP(KAStoppingCriteriaDlg, KDialogEx)
    ON_BN_CLICKED(IDCANCEL, &KAStoppingCriteriaDlg::OnBnClickedCancel)
END_MESSAGE_MAP()



void KAStoppingCriteriaDlg::AssignmentMode(KEMAssignmentModel emModel)
{
	m_emModel = emModel;
}


int KAStoppingCriteriaDlg::NumberOfIteration(void)
{
	return m_nIteration;
}


void KAStoppingCriteriaDlg::NumberOfIteration(int nIteration)
{
	m_nIteration = nIteration;
}


double KAStoppingCriteriaDlg::Gap(void)
{
	return m_dGap;
}


void KAStoppingCriteriaDlg::Gap(double dGap)
{
	m_dGap = dGap;
}



// KAStoppingCriteriaDlg 메시지 처리기입니다.
BOOL KAStoppingCriteriaDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CString strTitle = _T("Stopping Criteria : ");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strTitle = _T("반복수행 종료조건 설정");
	}
	if(KEMAssignmentModelUserEquilibriumOB == m_emModel || KEMAssignmentModelSegment == m_emModel)
	{
		m_stcAssign.SetWindowText(_T("Transit Assign:"));
		m_stcGap.SetWindowText(_T("Normalized Gap:"));
		strTitle += _T("Transit");
	}
	else if(KEMAssignmentModelUserEquilibriumFW == m_emModel)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_stcAssign.SetWindowText(_T("종료조건 설정 :"));
		}
		else {
			m_stcAssign.SetWindowText(_T("Highway Assign :"));
		}
		m_stcGap.SetWindowText(_T("Relative Gap :"));
		//strTitle += _T("User Equilibrium");
	}
	SetWindowText(strTitle);

	m_edtGap.EnableRealNumber(true);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAStoppingCriteriaDlg::OnOK()
{
	UpdateData(TRUE);

	KDialogEx::OnOK();
}



void KAStoppingCriteriaDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KDialogEx::OnCancel();
}
