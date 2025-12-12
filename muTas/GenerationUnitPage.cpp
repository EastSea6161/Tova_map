// GenerationUnitPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GenerationUnitPage.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationModelInfo.h"


// KGenerationUnitPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGenerationUnitPage, CXTPPropertyPage)

KGenerationUnitPage::KGenerationUnitPage(KTarget* a_pTarget, KGenerationModelInfo& a_oGenerationModeInfo)
	: CXTPPropertyPage(KGenerationUnitPage::IDD)
	, m_pTarget(a_pTarget)
	, m_oGenerationModeIinfo(a_oGenerationModeInfo)
{

}

KGenerationUnitPage::~KGenerationUnitPage()
{
}

void KGenerationUnitPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KGenerationUnitPage, CXTPPropertyPage)
END_MESSAGE_MAP()


// KGenerationUnitPage 메시지 처리기입니다.


BOOL KGenerationUnitPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
