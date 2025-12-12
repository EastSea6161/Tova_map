/**
 * @file ExpressionElementFunction.cpp
 * @brief KExpressionElementFunction 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "ExpressionElementFunction.h"



KExpressionElementFunction::KExpressionElementFunction(void)
	:	KExpressionElement(KEMExpressionElementFunction),
		m_strFunction(_T(""))
{
}


KExpressionElementFunction::KExpressionElementFunction(LPCTSTR strFunction)
	:	KExpressionElement(KEMExpressionElementFunction),
		m_strFunction(strFunction)
{
}


KExpressionElementFunction::KExpressionElementFunction(const KExpressionElementFunction& r)
	:	KExpressionElement(r),
		m_strFunction(r.m_strFunction)
{

}


KExpressionElementFunction::~KExpressionElementFunction(void)
{
}



LPCTSTR KExpressionElementFunction::Function(void)
{
	return m_strFunction;
}


void KExpressionElementFunction::Function(LPCTSTR strFunction)
{
	m_strFunction = strFunction;
}


CString KExpressionElementFunction::ToString(void)
{
	return m_strFunction;
}


CString KExpressionElementFunction::ToCaption(void)
{
	return m_strFunction;
}


KExpressionElementFunction& KExpressionElementFunction::operator=(const KExpressionElementFunction& r)
{
	KExpressionElement::operator=(r);
	m_strFunction = r.m_strFunction;

	return *this;
}