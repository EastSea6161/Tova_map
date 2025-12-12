// GenerationClassificationPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GenerationClassificationPage.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationModelInfo.h"


// KGenerationClassificationPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGenerationClassificationPage, CXTPPropertyPage)

KGenerationClassificationPage::KGenerationClassificationPage(KTarget* a_pTarget, KGenerationModelInfo& a_oGenerationModelInfo)
	: CXTPPropertyPage(KGenerationClassificationPage::IDD)
	, m_pTarget(a_pTarget)
	, m_oGenerationModelInfo(a_oGenerationModelInfo)
{

}

KGenerationClassificationPage::~KGenerationClassificationPage()
{
}

void KGenerationClassificationPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(KGenerationClassificationPage, CXTPPropertyPage)
END_MESSAGE_MAP()


// KGenerationClassificationPage 메시지 처리기입니다.
