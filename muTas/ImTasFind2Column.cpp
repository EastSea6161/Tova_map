// ImTasFind2Column.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImTasFind2Column.h"
#include "afxdialogex.h"


// KImTasFind2Column 대화 상자입니다.

IMPLEMENT_DYNAMIC(KImTasFind2Column, CDialogEx)

KImTasFind2Column::KImTasFind2Column(CWnd* pParent /*=NULL*/)
	: CDialogEx(KImTasFind2Column::IDD, pParent)
{

}

KImTasFind2Column::~KImTasFind2Column()
{
}

void KImTasFind2Column::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KImTasFind2Column, CDialogEx)
END_MESSAGE_MAP()


// KImTasFind2Column 메시지 처리기입니다.
