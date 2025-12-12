// ThemeStyleSave.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ThemeStyleSave.h"
#include "afxdialogex.h"


// KThemeStyleSave 대화 상자입니다.

IMPLEMENT_DYNAMIC(KThemeStyleSave, KResizeDialogEx)

KThemeStyleSave::KThemeStyleSave(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KThemeStyleSave::IDD, pParent)
{
	m_bOverwrite = true;
}

KThemeStyleSave::~KThemeStyleSave()
{
}

void KThemeStyleSave::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KThemeStyleSave, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KThemeStyleSave::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT_STYLE_NAME, &KThemeStyleSave::OnEnSetfocusEditStyleName)
END_MESSAGE_MAP()


// KThemeStyleSave 메시지 처리기입니다.


BOOL KThemeStyleSave::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CButton* pButton;
	if (m_strExistStyleName.GetLength() == 0)
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
		pButton->SetCheck(BST_UNCHECKED);
		pButton->EnableWindow(FALSE);
		pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(BST_CHECKED);
	}
	else
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
		pButton->SetCheck(BST_CHECKED);
		pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(BST_UNCHECKED);
	}
	
	SetDlgItemText(IDC_STATIC_STYLE_NAME, m_strExistStyleName);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KThemeStyleSave::OnBnClickedOk()
{
	int nCheckedRadioBtn = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
	if (nCheckedRadioBtn == IDC_RADIO1)
	{
		m_bOverwrite = true;
	}
	else
	{
		m_bOverwrite = false;			
		GetDlgItemText(IDC_EDIT_STYLE_NAME, m_strNewStyleName);
		m_strNewStyleName = m_strNewStyleName.Trim();

		if (m_strNewStyleName.GetLength() < 1)
		{
			AfxMessageBox(_T("명칭을 입력해 주세요."));
			GetDlgItem(IDC_EDIT_STYLE_NAME)->SetFocus();			
			return;
		}
	}
	KResizeDialogEx::OnOK();
}

void KThemeStyleSave::SetExistStyleName( CString a_strExistStyleName )
{
	m_strExistStyleName = a_strExistStyleName;
}


void KThemeStyleSave::OnEnSetfocusEditStyleName()
{
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);	
	pButton->SetCheck(BST_UNCHECKED);
	pButton = (CButton*)GetDlgItem(IDC_RADIO2);
	pButton->SetCheck(BST_CHECKED);
}
