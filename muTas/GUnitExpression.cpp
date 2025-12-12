/**
 * @file GUnitExpression.cpp
 * @brief KGUnitExpression 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "GUnitExpression.h"
#include "ExpressionElementField.h"
#include "ExpressionElementNumber.h"



KGUnitExpression::KGUnitExpression(void)
	:	KGBaseExpression(KEMGenerationUnit)
{
	m_pNumberElement = new KExpressionElementNumber;
	m_pFieldElement = new KExpressionElementField;
}


KGUnitExpression::KGUnitExpression(double dNumber, LPCTSTR strFieldName, LPCTSTR strFieldCaption)
	:	KGBaseExpression(KEMGenerationUnit)
{
	m_pNumberElement = new KExpressionElementNumber(dNumber);
	m_pFieldElement = new KExpressionElementField(strFieldName, strFieldCaption);
}


KGUnitExpression::KGUnitExpression(const KGUnitExpression& r)
	:	KGBaseExpression(r)
{
	m_pNumberElement = new KExpressionElementNumber(*r.m_pNumberElement);
	m_pFieldElement = new KExpressionElementField(*r.m_pFieldElement);
}

KGUnitExpression::~KGUnitExpression(void)
{
	if(NULL != m_pNumberElement)
	{
		delete m_pNumberElement;
	}

	if(NULL != m_pFieldElement)
	{
		delete m_pFieldElement;
	}
}



double KGUnitExpression::Number(void)
{
	return m_pNumberElement->Number();
}


void KGUnitExpression::Number(double a_dNumber)
{
	m_pNumberElement->Number(a_dNumber);
}


LPCTSTR KGUnitExpression::FieldName(void)
{
	return m_pFieldElement->FieldName();
}


LPCTSTR KGUnitExpression::FieldCaption(void)
{
	return m_pFieldElement->FieldCaption();
}


void KGUnitExpression::Field(LPCTSTR a_strFieldName, LPCTSTR a_strFieldCaption)
{
	m_pFieldElement->FieldName(a_strFieldName);
	m_pFieldElement->FieldCaption(a_strFieldCaption);
}


CString KGUnitExpression::GetExpression(void)
{
	CString strExpression = _T("");
	strExpression += m_pNumberElement->ToString();
	strExpression += _T(" * ");
	strExpression += m_pFieldElement->ToString();
	return strExpression;
}


CString KGUnitExpression::GetCaption(void)
{
	CString strExpression = _T("");
	strExpression += m_pNumberElement->ToCaption();
	strExpression += _T(" * ");
	strExpression += m_pFieldElement->ToCaption();
	return strExpression;
}


KGUnitExpression& KGUnitExpression::operator=(const KGUnitExpression& r)
{
	KGBaseExpression::operator=(r);
	*m_pNumberElement = *r.m_pNumberElement;
	*m_pFieldElement = *r.m_pFieldElement;
	return *this;
}