/**
 * @file : ModelGSimpleExpressionPage.cpp
 * @brief : KModelGSimpleExpressionPage 구현 파일
 * @author : teegee@zaolsoft.com
 * @date : 2011.04.22
 * @remark :
 */


#include "stdafx.h"
#include "kmz.h"
#include "ModelGSimpleExpressionPage.h"
#include "GFunctionUnitDlg.h"
#include "GFunctionRegressionDlg.h"
#include "ModelPurpose.h"



IMPLEMENT_DYNAMIC(KModelGSimpleExpressionPage, CXTPPropertyPage)


KModelGSimpleExpressionPage::KModelGSimpleExpressionPage()
	: CXTPPropertyPage(KModelGSimpleExpressionPage::IDD)
{

}

KModelGSimpleExpressionPage::~KModelGSimpleExpressionPage()
{
}


void KModelGSimpleExpressionPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KModelGSimpleExpressionPage, CXTPPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &KModelGSimpleExpressionPage::OnBnClickedButton1)
END_MESSAGE_MAP()



void KModelGSimpleExpressionPage::SetPurpose(KModelPurpose* pPurpose)
{
	m_pPurpose = pPurpose;
}



// KModelGSimpleExpressionPage 메시지 처리기입니다.


void KModelGSimpleExpressionPage::OnBnClickedButton1()
{
	if(NULL != m_pPurpose)
	{
		CString strExpression = _T("");
		if(m_pPurpose->GenerationModel() == KEMGenerationUnit)
		{
			KGFunctionUnitDlg dlg;
			if(dlg.DoModal() == IDOK)
			{
				
			}
		}
	}
}
