// MARunOptionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MARunOptionDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "KBulkDBaseUtil.h"

// KMARunOptionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMARunOptionDlg, KDialogEx)

KMARunOptionDlg::KMARunOptionDlg(KTarget* a_pTarget,int& a_nChkTurn, CWnd* pParent /*=NULL*/)
	: KDialogEx(KMARunOptionDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_nChkTurn(a_nChkTurn)
{

}

KMARunOptionDlg::~KMARunOptionDlg()
{
}

void KMARunOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_chkTurn);
	DDX_Control(pDX, IDC_STC_TRUN, m_stcTurn);
}


BEGIN_MESSAGE_MAP(KMARunOptionDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KMARunOptionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KMARunOptionDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KMARunOptionDlg 메시지 처리기입니다.


BOOL KMARunOptionDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CString strNotExists = _T("Data not exists");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strNotExists = _T("데이터 존재하지 않음");
	}
	CString strEmpty     = _T("");

	bool bExistTurn    = KBulkDBaseUtil::ExitstTurnData(m_pTarget->GetDBaseConnection());

	bool bChecked = false;
	if (m_nChkTurn == 1)
	{
		bChecked = true;
	}

	if ( bExistTurn )
	{
		m_chkTurn.SetCheck(bChecked);
		m_stcTurn.SetWindowText(strEmpty);
	}
	else
	{
		m_chkTurn.EnableWindow(FALSE);
		m_stcTurn.SetWindowText(strNotExists);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMARunOptionDlg::OnBnClickedOk()
{
	
	if (m_chkTurn.GetCheck() == BST_CHECKED)
	{
		m_nChkTurn = 1;
	}
	else
	{
		m_nChkTurn = 0;
	}

	KDialogEx::OnOK();
}


void KMARunOptionDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}
