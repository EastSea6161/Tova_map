/**
 * @file GRegressionExpression.cpp
 * @brief KGRegressionExpression 구현 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#include "StdAfx.h"
#include "GRegressionExpression.h"
#include "ExpressionElementField.h"
#include "ExpressionElementFunction.h"
#include "ExpressionElementNumber.h"
#include "ExpressionElementOperator.h"



KGRegressionExpression::KGRegressionExpression(void)
	:	KGBaseExpression(KEMGenerationRegression)
{
}


KGRegressionExpression::KGRegressionExpression(const KGRegressionExpression& r)
	:	KGBaseExpression(r)
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


KGRegressionExpression::~KGRegressionExpression(void)
{
	ClearElements();	
}


void KGRegressionExpression::AddNumber(double a_dNumber)
{
	KExpressionElement* pElement = new KExpressionElementNumber(a_dNumber);
	m_ElementList.push_back(pElement);
}


void KGRegressionExpression::AddOperator(TCHAR a_szOperator)
{
	KExpressionElement* pElement = new KExpressionElementOperator(a_szOperator);
	m_ElementList.push_back(pElement);
}


void KGRegressionExpression::AddFunction(LPCTSTR a_strFunction)
{
	KExpressionElement* pElement = new KExpressionElementFunction(a_strFunction);
	m_ElementList.push_back(pElement);
}


void KGRegressionExpression::AddField(LPCTSTR a_strFieldName, LPCTSTR a_strFieldCaption)
{
	KExpressionElement* pElement = new KExpressionElementField(a_strFieldName, a_strFieldCaption);
	m_ElementList.push_back(pElement);
}

void KGRegressionExpression::ClearElements(void)
{
	std::list<KExpressionElement*>::iterator itElement, itEnd = m_ElementList.end();
	for(itElement = m_ElementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		delete pElement;
	}

	m_ElementList.clear();
}

CString KGRegressionExpression::GetExpression(void)
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


CString KGRegressionExpression::GetCaption(void)
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


const std::list<KExpressionElement*>& KGRegressionExpression::GetExpressionList(void)
{
	return m_ElementList;
}

KGRegressionExpression& KGRegressionExpression::operator=(const KGRegressionExpression& expression)
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