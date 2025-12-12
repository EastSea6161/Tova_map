/**
 * @file ModeFunction.cpp
 * @brief KModeFunction 구현 파일
 * @author 
 * @date 2011.06.03
 * @remark
 */


#include "StdAfx.h"
#include "ModeFunction.h"
#include "ExpressionElementField.h"
#include "ExpressionElementFunction.h"
#include "ExpressionElementNumber.h"
#include "ExpressionElementOperator.h"



KModeFunction::KModeFunction(void)
{
}


KModeFunction::KModeFunction(const KModeFunction& r)
{
	std::list<KExpressionElement*>::const_iterator itElement, itEnd = r.m_ElementList.end();
	for(itElement = r.m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		KEMExpressionElementType elementType = pElement->ElementType();
		if(KEMExpressionElementField == elementType)
		{
			KExpressionElementField* pField = (KExpressionElementField*)pElement;
			AddField(pField->FieldName(), pField->FieldCaption());
		}
		else if(KEMExpressionElementFunction == elementType)
		{
			AddFunction(((KExpressionElementFunction*)pElement)->Function());
		}
		else if(KEMExpressionElementNumber == elementType)
		{
			AddNumber(((KExpressionElementNumber*)pElement)->Number());
		}
		else if(KEMExpressionElementOperator == elementType)
		{
			AddOperator(((KExpressionElementOperator*)pElement)->Operator());
		}
	}

    m_strCaptionExpression = r.m_strCaptionExpression;
}


KModeFunction::~KModeFunction(void)
{
	ClearElements();	
}

void KModeFunction::AddNumber(double dNumber)
{
	KExpressionElement* pElement = new KExpressionElementNumber(dNumber);
	m_ElementList.push_back(pElement);
}


void KModeFunction::AddOperator(TCHAR szOperator)
{
	KExpressionElement* pElement = new KExpressionElementOperator(szOperator);
	m_ElementList.push_back(pElement);
}


void KModeFunction::AddFunction(LPCTSTR strFunction)
{
	KExpressionElement* pElement = new KExpressionElementFunction(strFunction);
	m_ElementList.push_back(pElement);
}


void KModeFunction::AddField(LPCTSTR strFieldName, LPCTSTR strFieldCaption)
{
	KExpressionElement* pElement = new KExpressionElementField(strFieldName, strFieldCaption);
	m_ElementList.push_back(pElement);
}

void KModeFunction::ClearElements(void)
{
	std::list<KExpressionElement*>::iterator itElement, itEnd = m_ElementList.end();
	for(itElement = m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		delete pElement;
	}

	m_ElementList.clear();
}

CString KModeFunction::GetExpression(void)
{
	CString strValue = _T("");
	bool bBegin = true;
	std::list<KExpressionElement*>::iterator itElement, itEnd = m_ElementList.end();
	for(itElement = m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		if(true == bBegin)
		{
			bBegin = false;
		}
		else
		{
			strValue += _T(" ");
		}

		strValue += (*itElement)->ToString();
	}

	return strValue;
}


CString KModeFunction::GetCaption(void)
{
	CString strValue = _T("");
	bool bBegin = true;
	std::list<KExpressionElement*>::iterator itElement, itEnd = m_ElementList.end();
	for(itElement = m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		if(true == bBegin)
		{
			bBegin = false;
		}
		else
		{
			strValue += _T(" ");
		}

		strValue += (*itElement)->ToCaption();
	}

	return strValue;
}


const std::list<KExpressionElement*>& KModeFunction::GetExpressionList(void)
{
	return m_ElementList;
}

KModeFunction& KModeFunction::operator=(const KModeFunction& expression)
{
	ClearElements();
	std::list<KExpressionElement*>::const_iterator itElement, itEnd = expression.m_ElementList.end();
	for(itElement = expression.m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		KEMExpressionElementType elementType = pElement->ElementType();
		if(KEMExpressionElementField == elementType)
		{
			KExpressionElementField* pField = (KExpressionElementField*)pElement;
			AddField(pField->FieldName(), pField->FieldCaption());
		}
		else if(KEMExpressionElementFunction == elementType)
		{
			AddFunction(((KExpressionElementFunction*)pElement)->Function());
		}
		else if(KEMExpressionElementNumber == elementType)
		{
			AddNumber(((KExpressionElementNumber*)pElement)->Number());
		}
		else if(KEMExpressionElementOperator == elementType)
		{
			AddOperator(((KExpressionElementOperator*)pElement)->Operator());
		}
	}

    m_strCaptionExpression = expression.m_strCaptionExpression;

	return *this;
}