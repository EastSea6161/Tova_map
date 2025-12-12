/**
 * @file ExpressionChecker.cpp
 * @brief KExpressionChecker 구현 파일
 * @author 
 * @date 2011.06.10
 * @remark
 */


#include "StdAfx.h"
#include "ExpressionChecker.h"


KExpressionChecker::KExpressionChecker(void)
{
}


KExpressionChecker::~KExpressionChecker(void)
{
}


void KExpressionChecker::AddField(LPCTSTR strField)
{
	m_FieldList.push_back(strField);
}


void KExpressionChecker::AddFunction(LPCTSTR strFunction)
{
	m_FunctionList.push_back(strFunction);
}


void KExpressionChecker::AddOperator(LPCTSTR strOperator)
{
	m_OperatorList.push_back(strOperator);
}


bool KExpressionChecker::IsField(LPCTSTR strElement)
{
	std::tstring str = strElement;
	if((str.find(_T("[")) != std::tstring::npos) || (str.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type n = str.find_first_not_of(_T("["));
		str = (n == std::tstring::npos) ? str : str.substr(n, str.length());
		n = str.find_last_not_of(_T("]"));
		str = (n == std::tstring::npos) ? str : str.substr(0, n + 1);
	}

	std::list<std::tstring>::iterator itField, itEnd = m_FieldList.end();
	for(itField = m_FieldList.begin(); itField != itEnd; ++itField)
	{
		std::tstring strField = *itField;
		if(_tcsicmp(str.c_str(), strField.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}


bool KExpressionChecker::IsFunction(LPCTSTR strElement)
{
	std::tstring str = strElement;
	std::list<std::tstring>::iterator itFunction, itEnd = m_FunctionList.end();
	for(itFunction = m_FunctionList.begin(); itFunction != itEnd; ++itFunction)
	{
		std::tstring strFunction = *itFunction;
		if(_tcsicmp(str.c_str(), strFunction.c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}


bool KExpressionChecker::IsOperator(LPCTSTR strElement)
{
	std::tstring str = strElement;

	std::list<std::tstring>::iterator itOperator, itEnd = m_OperatorList.end();
	for(itOperator = m_OperatorList.begin(); itOperator != itEnd; ++itOperator)
	{
		std::tstring strOperator = *itOperator;
		if(_tcsicmp(str.c_str(), strOperator.c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}


void KExpressionChecker::IsField(LPCTSTR strElement, int &a_nFlag)
{
	a_nFlag = FUNCTION_ELEMENT_NOFIELD;

	std::tstring str = strElement;
	if((str.find(_T("[")) != std::tstring::npos) || (str.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type n = str.find_first_not_of(_T("["));
		str = (n == std::tstring::npos) ? str : str.substr(n, str.length());
		n = str.find_last_not_of(_T("]"));
		str = (n == std::tstring::npos) ? str : str.substr(0, n + 1);

		a_nFlag = FUNCTION_ELEMENT_UNREGISTERFIELD;
	}

	std::list<std::tstring>::iterator itField, itEnd = m_FieldList.end();
	for(itField = m_FieldList.begin(); itField != itEnd; ++itField)
	{
		std::tstring strField = *itField;
		if(_tcsicmp(str.c_str(), strField.c_str()) == 0)
		{
			a_nFlag = FUNCTION_ELEMENT_REGISTERFIELD;
			return;
		}
	}

	return;
}


const std::list<std::tstring>& KExpressionChecker::FieldList(void)
{
	return m_FieldList;
}


const std::list<std::tstring>& KExpressionChecker::FunctionList(void)
{
	return m_FunctionList;
}


const std::list<std::tstring>& KExpressionChecker::OperatorList(void)
{
	return m_OperatorList;
}