// AddModeNameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AddModeNameDlg.h"
#include "afxdialogex.h"


// KAddModeNameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAddModeNameDlg, KDialogEx)

KAddModeNameDlg::KAddModeNameDlg(std::set<CString> a_setModeName, CWnd* pParent /*=NULL*/)
	: KDialogEx(KAddModeNameDlg::IDD, pParent)
{
	m_setModeName = a_setModeName;
}

KAddModeNameDlg::~KAddModeNameDlg()
{
}

void KAddModeNameDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KAddModeNameDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KAddModeNameDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KAddModeNameDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KAddModeNameDlg 메시지 처리기입니다.
BOOL KAddModeNameDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAddModeNameDlg::OnBnClickedOk()
{
	CString strValue;

	try
	{
		GetDlgItemText(IDC_EDIT1, strValue);

		if (strValue.IsEmpty())
		{
			AfxMessageBox(_T("수단명을 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		std::set<CString>::iterator fIter = m_setModeName.find(strValue);

		if (fIter != m_setModeName.end())
		{
			AfxMessageBox(_T("입력한 이름이 이미 존재합니다."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		m_strModeName = strValue;
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}
	
	KDialogEx::OnOK();
}


void KAddModeNameDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}

CString KAddModeNameDlg::GetModeName( void )
{
	return m_strModeName;
}
