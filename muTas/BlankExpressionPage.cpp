// KBlankExpressionPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "BlankExpressionPage.h"
#include "afxdialogex.h"


// KBlankExpressionPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KBlankExpressionPage, CXTPPropertyPage)

KBlankExpressionPage::KBlankExpressionPage()
	: CXTPPropertyPage(KBlankExpressionPage::IDD)
{

}

KBlankExpressionPage::~KBlankExpressionPage()
{
}

void KBlankExpressionPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KBlankExpressionPage, CXTPPropertyPage)
END_MESSAGE_MAP()


// KBlankExpressionPage 메시지 처리기입니다.
