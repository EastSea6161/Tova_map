/**
 * @file ExpressionElementField.cpp
 * @brief KExpressionElementField 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "ExpressionElementField.h"



KExpressionElementField::KExpressionElementField(void)
	:	KExpressionElement(KEMExpressionElementField),
		m_strFieldName(_T("")),
		m_strFieldCaption(_T(""))
{
}

KExpressionElementField::KExpressionElementField(const KExpressionElementField& r)
	:	KExpressionElement(r),
		m_strFieldName(r.m_strFieldName),
		m_strFieldCaption(r.m_strFieldCaption)
{

}


KExpressionElementField::KExpressionElementField(LPCTSTR strFieldName, LPCTSTR strFieldCaption)
	:	KExpressionElement(KEMExpressionElementField),
		m_strFieldName(strFieldName),
		m_strFieldCaption(strFieldCaption)
{
}


KExpressionElementField::~KExpressionElementField(void)
{
}


LPCTSTR KExpressionElementField::FieldName(void)
{
	return m_strFieldName;
}


void KExpressionElementField::FieldName(LPCTSTR strFieldName)
{
	m_strFieldName = strFieldName;
}

LPCTSTR KExpressionElementField::FieldCaption(void)
{
	return m_strFieldCaption;
}


void KExpressionElementField::FieldCaption(LPCTSTR strFieldCaption)
{
	m_strFieldCaption = strFieldCaption;
}

CString KExpressionElementField::ToString(void)
{
	return m_strFieldName;
}


CString KExpressionElementField::ToCaption(void)
{
	CString strCaption = _T("[");
	strCaption += m_strFieldCaption;
	strCaption += _T("]");
	return strCaption;
}


KExpressionElementField& KExpressionElementField::operator=(const KExpressionElementField& r)
{
	KExpressionElement::operator=(r);
	m_strFieldName = r.m_strFieldName;
	m_strFieldCaption = r.m_strFieldCaption;
	
	return *this;
}