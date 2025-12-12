// ThemeNodePropertiesMulti.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ThemeNodePropertiesMulti.h"
#include "afxdialogex.h"


// KThemeNodePropertiesMulti 대화 상자입니다.

IMPLEMENT_DYNAMIC(KThemeNodePropertiesMulti, KDialogEx)

KThemeNodePropertiesMulti::KThemeNodePropertiesMulti(CWnd* pParent /*=NULL*/)
	: KDialogEx(KThemeNodePropertiesMulti::IDD, pParent)
{

}

KThemeNodePropertiesMulti::~KThemeNodePropertiesMulti()
{
}

void KThemeNodePropertiesMulti::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KThemeNodePropertiesMulti, KDialogEx)
END_MESSAGE_MAP()


// KThemeNodePropertiesMulti 메시지 처리기입니다.
