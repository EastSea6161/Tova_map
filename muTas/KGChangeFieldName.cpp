// KGChangeFieldName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGChangeFieldName.h"
#include "afxdialogex.h"


// KGChangeFieldName 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGChangeFieldName, KDialogEx)

KGChangeFieldName::KGChangeFieldName(CString a_strCurrentName, CWnd* pParent /*=NULL*/)
	: KDialogEx(KGChangeFieldName::IDD, pParent)
	, m_strCurrentName(a_strCurrentName)
	, m_strNewName(_T(""))
{

}

KGChangeFieldName::~KGChangeFieldName()
{
}

void KGChangeFieldName::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strCurrentName);
	DDX_Text(pDX, IDC_EDIT2, m_strNewName);
}


BEGIN_MESSAGE_MAP(KGChangeFieldName, KDialogEx)
	ON_BN_CLICKED(IDOK, &KGChangeFieldName::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KGChangeFieldName::OnBnClickedCancel)
END_MESSAGE_MAP()


// KGChangeFieldName 메시지 처리기입니다.

BOOL KGChangeFieldName::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	m_strNewName = m_strCurrentName;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void KGChangeFieldName::OnBnClickedOk()
{
	UpdateData(TRUE);

	if(m_strNewName.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("새 이름을 입력해 주세요."));
		return;
	}

	KDialogEx::OnOK();
}


void KGChangeFieldName::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}
