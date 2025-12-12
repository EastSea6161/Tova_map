// KInformationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InformationDlg.h"
#include "afxdialogex.h"


// KInformationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInformationDlg, KDialogEx)

KInformationDlg::KInformationDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KInformationDlg::IDD, pParent)
{

}

KInformationDlg::~KInformationDlg()
{
}

void KInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KInformationDlg, KDialogEx)
END_MESSAGE_MAP()


// KInformationDlg 메시지 처리기입니다.


BOOL KInformationDlg::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    SetDlgItemText(IDC_EDIT1, m_strMsg);

    return TRUE;  
}
