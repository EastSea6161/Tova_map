/**
 * @file GSimpleExpressionPage.cpp
 * @brief KGSimpleExpressionPage 구현 파일
 * @author 
 * @date 2011.04.29
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "GSimpleExpressionPage.h"
#include "afxdialogex.h"
#include "GFunctionUnitDlg.h"
#include "GFunctionRegressionDlg.h"
#include "GRegressionExpression.h"
#include "GSimpleExpressionContainer.h"
#include "GUnitExpression.h"
#include "Purpose.h"



IMPLEMENT_DYNAMIC(KGSimpleExpressionPage, CXTPPropertyPage)


KGSimpleExpressionPage::KGSimpleExpressionPage()
	:	CXTPPropertyPage(KGSimpleExpressionPage::IDD), 
		m_pPurpose(NULL)
{

}

KGSimpleExpressionPage::~KGSimpleExpressionPage()
{
}

void KGSimpleExpressionPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtExpression);
}




BEGIN_MESSAGE_MAP(KGSimpleExpressionPage, CXTPPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &KGSimpleExpressionPage::OnBnClickedButton1)
END_MESSAGE_MAP()


void KGSimpleExpressionPage::SetPurpose(KPurpose* pPurpose)
{
	//m_pPurpose = pPurpose;
	//KGBaseExpressionContainer* pContainer = m_pPurpose->GetExpressionContainer();
	//KGSimpleExpressionContainer* pSimpleContainer = (KGSimpleExpressionContainer*)pContainer;
	//KGBaseExpression* pBase = pSimpleContainer->ExpressionObject();
	//if(NULL != pBase)
	//{
	//	m_edtExpression.SetWindowText(pBase->GetCaption());
	//}
	//else
	//{
	//	m_edtExpression.SetWindowText(_T(""));
	//}
}


// KGSimpleExpressionPage 메시지 처리기입니다.
void KGSimpleExpressionPage::OnBnClickedButton1()
{
	//ASSERT(NULL != m_pPurpose);

	//KGBaseExpressionContainer* pContainer = m_pPurpose->GetExpressionContainer();
	//ASSERT(NULL != pContainer);
	//ASSERT(KEMGExpressionContainerSimple == pContainer->ContainerType());

	//KGSimpleExpressionContainer* pSimpleContainer = (KGSimpleExpressionContainer*)pContainer;
	//KGBaseExpression* pBase = pSimpleContainer->ExpressionObject();
	//if(NULL == pBase)
	//{
	//	pBase = pSimpleContainer->CreateExpressionObject();
	//}

	//if(KEMGenerationUnit == pContainer->GenerationModel())
	//{
	//	KGUnitExpression* pUnit = (KGUnitExpression*)pBase;
	//	KGFunctionUnitDlg dlg;
	//	dlg.Number(pUnit->Number());
	//	dlg.FieldName(pUnit->FieldName());
	//	dlg.FieldCaption(pUnit->FieldCaption());
	//	if(dlg.DoModal() == IDOK)
	//	{
	//		pUnit->Number(dlg.Number());
	//		pUnit->Field(dlg.FieldName(), dlg.FieldCaption());
	//	}
	//}
	//else if(KEMGenerationRegression == pContainer->GenerationModel())
	//{
	//	KGRegressionExpression* pRegression = (KGRegressionExpression*)pBase;
	//	KGFunctionRegressionDlg dlg;
	//	dlg.Expression(pRegression);
	//	if(dlg.DoModal() == IDOK)
	//	{
	//	}
	//}
	//else if(KEMGenerationCrossClassification == pContainer->GenerationModel())
	//{
	//	// todo : Cross-Classfication 구현
	//}
	//m_edtExpression.SetWindowText(pBase->GetCaption());
}
