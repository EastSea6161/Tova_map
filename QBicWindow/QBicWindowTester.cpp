// QBicWindowTester.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QBicWindowTester.h"
#include "afxdialogex.h"


// QBicWindowTester 대화 상자입니다.

IMPLEMENT_DYNAMIC(QBicWindowTester, CDialogEx)

QBicWindowTester::QBicWindowTester(CWnd* pParent /*=NULL*/)
	: CDialogEx(QBicWindowTester::IDD, pParent)
{

}

QBicWindowTester::~QBicWindowTester()
{
}

void QBicWindowTester::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(QBicWindowTester, CDialogEx)
END_MESSAGE_MAP()


// QBicWindowTester 메시지 처리기입니다.
