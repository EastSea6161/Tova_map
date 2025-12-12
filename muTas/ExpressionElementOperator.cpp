/**
 * @file ExpressionElementOperator.cpp
 * @brief KExpressionElementOperator 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "ExpressionElementOperator.h"



KExpressionElementOperator::KExpressionElementOperator(void)
	:	KExpressionElement(KEMExpressionElementOperator)
{
}


KExpressionElementOperator::KExpressionElementOperator(TCHAR szOperator)
	:	KExpressionElement(KEMExpressionElementOperator),
		m_szOperator(szOperator)
{
}


KExpressionElementOperator::KExpressionElementOperator(const KExpressionElementOperator& r)
	:	KExpressionElement(r),
		m_szOperator(r.m_szOperator)
{

}

KExpressionElementOperator::~KExpressionElementOperator(void)
{
}


TCHAR KExpressionElementOperator::Operator(void)
{
	return m_szOperator;
}


void KExpressionElementOperator::Operator(TCHAR szOperator)
{
	m_szOperator = szOperator;
}


CString KExpressionElementOperator::ToString(void)
{
	CString strValue;
	strValue.Format(_T("%c"), m_szOperator);
	return strValue;
}


CString KExpressionElementOperator::ToCaption(void)
{
	CString strValue;
	strValue.Format(_T("%c"), m_szOperator);
	return strValue;
}


KExpressionElementOperator& KExpressionElementOperator::operator=(const KExpressionElementOperator& r)
{
	KExpressionElement::operator=(r);
	m_szOperator = r.m_szOperator;

	return *this;
}