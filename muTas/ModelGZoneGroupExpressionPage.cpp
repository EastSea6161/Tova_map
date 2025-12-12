/**
 * @file : ModelGZoneGroupExpressionPage.cpp
 * @brief : KModelGZoneGroupExpressionPage 구현 파일
 * @author : teegee@zaolsoft.com
 * @date : 2011.04.22
 * @remark :
 */



#include "stdafx.h"
#include "kmz.h"
#include "ModelGZoneGroupExpressionPage.h"



IMPLEMENT_DYNAMIC(KModelGZoneGroupExpressionPage, CPropertyPage)


KModelGZoneGroupExpressionPage::KModelGZoneGroupExpressionPage()
	: CPropertyPage(KModelGZoneGroupExpressionPage::IDD)
{

}


KModelGZoneGroupExpressionPage::~KModelGZoneGroupExpressionPage()
{
}


void KModelGZoneGroupExpressionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KModelGZoneGroupExpressionPage, CPropertyPage)
END_MESSAGE_MAP()



// KModelGZoneGroupExpressionPage 메시지 처리기입니다.
