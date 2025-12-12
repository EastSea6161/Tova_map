// GenerationZoneGroupPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GenerationZoneGroupPage.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationModelInfo.h"


// KGenerationZoneGroupPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGenerationZoneGroupPage, CXTPPropertyPage)

KGenerationZoneGroupPage::KGenerationZoneGroupPage(KTarget* a_pTarget, KGenerationModelInfo& a_oGenerationModelInfo)
	: CXTPPropertyPage(KGenerationZoneGroupPage::IDD)
	, m_pTarget(a_pTarget)
	, m_oGenerationModelInfo(a_oGenerationModelInfo)
{

}

KGenerationZoneGroupPage::~KGenerationZoneGroupPage()
{
}

void KGenerationZoneGroupPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KGenerationZoneGroupPage, CXTPPropertyPage)
END_MESSAGE_MAP()


// KGenerationZoneGroupPage 메시지 처리기입니다.
