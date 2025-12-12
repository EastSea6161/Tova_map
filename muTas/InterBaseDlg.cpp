// InterBaseDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "InterBaseDlg.h"
#include "afxdialogex.h"


// CInterBaseDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInterBaseDlg, CDialogEx)

CInterBaseDlg::CInterBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInterBaseDlg::IDD, pParent)
{

}

CInterBaseDlg::~CInterBaseDlg()
{
}

void CInterBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInterBaseDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInterBaseDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInterBaseDlg 메시지 처리기입니다.


void CInterBaseDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
