/**
 * @file ExpressionElementNumber.cpp
 * @brief KExpressionElementNumber 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "ExpressionElementNumber.h"



KExpressionElementNumber::KExpressionElementNumber(void)
	:	KExpressionElement(KEMExpressionElementNumber), 
		m_dValue(1.0f)
{
}


KExpressionElementNumber::KExpressionElementNumber(double dValue)
	:	KExpressionElement(KEMExpressionElementNumber),
		m_dValue(dValue)
{
}


KExpressionElementNumber::KExpressionElementNumber(const KExpressionElementNumber& r)
	:	KExpressionElement(r),
		m_dValue(r.m_dValue)
{

}


KExpressionElementNumber::~KExpressionElementNumber(void)
{
}


double KExpressionElementNumber::Number(void)
{
	return m_dValue;
}


void KExpressionElementNumber::Number(double dValue)
{
	m_dValue = dValue;
}


CString KExpressionElementNumber::ToString(void)
{
	CString strValue;
	strValue.Format(_T("%f"), m_dValue);
	return strValue;
}


CString KExpressionElementNumber::ToCaption(void)
{
	CString strValue;
	strValue.Format(_T("%f"), m_dValue);
	return strValue;
}


KExpressionElementNumber& KExpressionElementNumber::operator=(const KExpressionElementNumber& r)
{
	KExpressionElement::operator=(r);
	m_dValue = r.m_dValue;

	return *this;
}