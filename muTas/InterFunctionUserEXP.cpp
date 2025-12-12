#include "StdAfx.h"
#include "InterFunctionUserEXP.h"

#include "ExpressionElement.h"
#include "ExpressionElementField.h"
#include "ExpressionElementFunction.h"
#include "ExpressionElementNumber.h"
#include "ExpressionElementOperator.h"

KInterFunctionUserEXP::KInterFunctionUserEXP(void)
{
}

KInterFunctionUserEXP::KInterFunctionUserEXP(const KInterFunctionUserEXP& r)
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
}

KInterFunctionUserEXP::~KInterFunctionUserEXP(void)
{
	ClearElements();
}

KInterFunctionUserEXP& KInterFunctionUserEXP::operator=(const KInterFunctionUserEXP& r)
{
	ClearElements();
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

	return *this;
}

void KInterFunctionUserEXP::AddNumber(double dNumber)
{
	KExpressionElement* pElement = new KExpressionElementNumber(dNumber);
	m_ElementList.push_back(pElement);
}


void KInterFunctionUserEXP::AddOperator(TCHAR szOperator)
{
	KExpressionElement* pElement = new KExpressionElementOperator(szOperator);
	m_ElementList.push_back(pElement);
}


void KInterFunctionUserEXP::AddFunction(LPCTSTR strFunction)
{
	KExpressionElement* pElement = new KExpressionElementFunction(strFunction);
	m_ElementList.push_back(pElement);
}


void KInterFunctionUserEXP::AddField(LPCTSTR strFieldName, LPCTSTR strFieldCaption)
{
	KExpressionElement* pElement = new KExpressionElementField(strFieldName, strFieldCaption);
	m_ElementList.push_back(pElement);
}

void KInterFunctionUserEXP::ClearElements(void)
{
	std::list<KExpressionElement*>::iterator itElement, itEnd = m_ElementList.end();
	for(itElement = m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		delete pElement;
	}

	m_ElementList.clear();
}

CString KInterFunctionUserEXP::GetExpression(void)
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


CString KInterFunctionUserEXP::GetCaption(void)
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


const std::list<KExpressionElement*>& KInterFunctionUserEXP::GetExpressionList(void)
{
	return m_ElementList;
}