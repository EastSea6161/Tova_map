// GenerationRegressionPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GenerationRegressionPage.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationModelInfo.h"

// KGenerationRegressionPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGenerationRegressionPage, CXTPPropertyPage)

KGenerationRegressionPage::KGenerationRegressionPage(KTarget* a_pTarget, KGenerationModelInfo& a_oGenerationModelInfo)
	: CXTPPropertyPage(KGenerationRegressionPage::IDD)
	, m_pTarget(a_pTarget)
	, m_oGenerationModelInfo(a_oGenerationModelInfo)
{

}

KGenerationRegressionPage::~KGenerationRegressionPage()
{
}

void KGenerationRegressionPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KGenerationRegressionPage, CXTPPropertyPage)
END_MESSAGE_MAP()


// KGenerationRegressionPage 메시지 처리기입니다.
