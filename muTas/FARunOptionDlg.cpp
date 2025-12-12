// FARunOptionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FARunOptionDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "KBulkDBaseUtil.h"

// KFARunOptionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KFARunOptionDlg, KDialogEx)

KFARunOptionDlg::KFARunOptionDlg(KTarget* a_pTarget, KEMAssignmentModel a_emAModel, int a_nVDFCount, CWnd* pParent /*=NULL*/)
	: KDialogEx(KFARunOptionDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_emAModel(a_emAModel)
	, m_nVDFCount(a_nVDFCount)
{

}

KFARunOptionDlg::~KFARunOptionDlg()
{
}

void KFARunOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_chkTurn);
	DDX_Control(pDX, IDC_STC_TRUN, m_stcTurn);
}


BEGIN_MESSAGE_MAP(KFARunOptionDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KFARunOptionDlg::OnOK)
END_MESSAGE_MAP()


// KFARunOptionDlg 메시지 처리기입니다.


BOOL KFARunOptionDlg::OnInitDialog()
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

	if ( bExistTurn )
	{
		m_chkTurn.SetCheck(m_RunningOption.IncludeTurnRestriction() == true ? TRUE : FALSE);
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


void KFARunOptionDlg::RunningOption( KARunningOptionInfo& info )
{
	m_RunningOption = info;
}

KARunningOptionInfo& KFARunOptionDlg::RunningOption( void )
{
	return m_RunningOption;
}

void KFARunOptionDlg::OnOK()
{
	m_RunningOption.IncludeTurnRestriction(m_chkTurn.GetCheck() == BST_CHECKED ? true : false);

	KDialogEx::OnOK();
}
