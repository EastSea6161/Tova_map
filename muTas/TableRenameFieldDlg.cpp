// TableRenameFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableRenameFieldDlg.h"
#include "afxdialogex.h"
#include "ExistNameChecker.h"

// KTableRenameFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableRenameFieldDlg, KDialogEx)

KTableRenameFieldDlg::KTableRenameFieldDlg(CString &a_strColumnName, std::vector<CString> a_vecDisplayName, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableRenameFieldDlg::IDD, pParent)
	, m_strColumnName (a_strColumnName)
	, m_vecDisplayName(a_vecDisplayName)
{
	m_pNameChecker = new KExistNameChecker;

	for(size_t i= 0; i< m_vecDisplayName.size(); i++ )
	{
		m_pNameChecker->addName( m_vecDisplayName[i] );
	}
}

KTableRenameFieldDlg::~KTableRenameFieldDlg()
{
	if (nullptr != m_pNameChecker)
	{
		delete m_pNameChecker;
	}
}

void KTableRenameFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KTableRenameFieldDlg, KDialogEx)
	ON_BN_CLICKED(IDCANCEL,    &KTableRenameFieldDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK,        &KTableRenameFieldDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// KTableRenameFieldDlg 메시지 처리기입니다.


BOOL KTableRenameFieldDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetDlgItemText(IDC_EDIT1, m_strColumnName);

	AddImChampTooltip(IDC_EDIT1, _T("이름에는 다음 문자를 사용할 수 없습니다.\r\n \\/:*?\"<>|[]\'"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableRenameFieldDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KTableRenameFieldDlg::OnBnClickedOk()
{
	CString strNewDisplayName(_T("")); 
	GetDlgItemText(IDC_EDIT1, strNewDisplayName);
	strNewDisplayName.Trim();
	SetDlgItemText(IDC_EDIT1, strNewDisplayName);

	try
	{
		if (m_pNameChecker->isNameExists(strNewDisplayName) == true)
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("동일한 컬럼명 '%s'이 존재합니다."), strNewDisplayName);
			AfxMessageBox(strMsg);
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		if (strNewDisplayName.IsEmpty() == true)
		{
			AfxMessageBox(_T("컬럼명을 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		if (strNewDisplayName.Find(_T(' ')) > -1)
		{
			AfxMessageBox(_T("컬럼명에 공백이 존재합니다. 다시 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		CString strErrCh(_T(""));
		if (QBicStringChecker::IsValidColumnName(strNewDisplayName, strErrCh) == false)
		{
			CString strErrMsg(_T(""));
			strErrMsg.Format(_T("명칭에 \'%s\' 문자를 사용할 수 없습니다."), strErrCh);
			AfxMessageBox(strErrMsg);

			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		/*if (AfxMessageBox(_T("변경 하시겠습니까?"), MB_YESNO) == IDNO)
		{
			return;
		}*/
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	}

	m_strColumnName = strNewDisplayName;

	KDialogEx::OnOK();
}


void KTableRenameFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}
