/**
 * @file MSimpleVariable.cpp
 * @brief KMSimpleVariable 구현 파일
 * @author 
 * @date 2011.06.09
 * @remark
 */


#include "StdAfx.h"
#include "MSimpleVariable.h"
#include "ModeFunction.h"



KMSimpleVariable::KMSimpleVariable(void)
	:	KMBaseVariable(KEMModeVariableTypeSimple)
{
}


KMSimpleVariable::KMSimpleVariable(const KMSimpleVariable& r)
	:	KMBaseVariable(r)
{
	std::map<int, KModeFunction*>::const_iterator itFunction, itFEnd = r.m_FunctionMap.end();
	for(itFunction = r.m_FunctionMap.begin(); itFunction != itFEnd; ++itFunction)
	{
		KModeFunction* pFunction = new KModeFunction(*(itFunction->second));
		m_FunctionMap.insert(std::make_pair(itFunction->first, pFunction));
	}

	m_ParameterMap = r.m_ParameterMap;
}


KMSimpleVariable::~KMSimpleVariable(void)
{
	RemoveAllModeFunction();
}


KMSimpleVariable& KMSimpleVariable::operator=(const KMSimpleVariable& r)
{
	KMBaseVariable::operator=(r);
	RemoveAllModeFunction();
	std::map<int, KModeFunction*>::const_iterator itFunction, itFEnd = r.m_FunctionMap.end();
	for(itFunction = r.m_FunctionMap.begin(); itFunction != itFEnd; ++itFunction)
	{
		KModeFunction* pFunction = new KModeFunction(*(itFunction->second));
		m_FunctionMap.insert(std::make_pair(itFunction->first, pFunction));
	}

	m_ParameterMap = r.m_ParameterMap;

	return *this;
}


KModeFunction* KMSimpleVariable::ModeFunction(int nModeID)
{
	KModeFunction* pRet = NULL;
	std::map<int, KModeFunction*>::iterator itFunction = m_FunctionMap.find(nModeID);
	if(m_FunctionMap.end() != itFunction)
	{
		pRet = itFunction->second;
	}
	return pRet;
}


void KMSimpleVariable::ModeFunction(int nModeID, KModeFunction* pFunction)
{
	std::map<int, KModeFunction*>::iterator itFunction = m_FunctionMap.find(nModeID);
	if(m_FunctionMap.end() != itFunction)
	{
		KModeFunction* pOldFunction = itFunction->second;
		*pOldFunction = *pFunction;
	}
	else
	{
		KModeFunction* pNewFunction = new KModeFunction(*pFunction);
		m_FunctionMap.insert(std::make_pair(nModeID, pNewFunction));
	}
}


void KMSimpleVariable::RemoveModeFunction(int nModeID)
{
	std::map<int, KModeFunction*>::iterator itFunction = m_FunctionMap.find(nModeID);
	if(m_FunctionMap.end() != itFunction)
	{
		KModeFunction* pFunction = itFunction->second;
		delete pFunction;
		m_FunctionMap.erase(itFunction);
	}
}


void KMSimpleVariable::RemoveAllModeFunction(void)
{
	std::map<int, KModeFunction*>::iterator itFunction, itFEnd = m_FunctionMap.end();
	for(itFunction = m_FunctionMap.begin(); itFunction != itFEnd; ++itFunction)
	{
		KModeFunction* pFunction = itFunction->second;
		delete pFunction;
	}
	m_FunctionMap.clear();
}


double KMSimpleVariable::ModeParameter(int nModeID)
{
	double dRet = 0.0f;
	std::map<int, double>::iterator itParam = m_ParameterMap.find(nModeID);
	if(m_ParameterMap.end() != itParam)
	{
		dRet = itParam->second;
	}
	return dRet;
}


void KMSimpleVariable::ModeParameter(int nModeID, double dParameter)
{
	m_ParameterMap[nModeID] = dParameter;
}


void KMSimpleVariable::RemoveModeParameter(int nModeID)
{
	std::map<int, double>::iterator itParam = m_ParameterMap.find(nModeID);
	if(m_ParameterMap.end() != itParam)
	{
		m_ParameterMap.erase(itParam);
	}
}


void KMSimpleVariable::RemoveAllModeParameter(void)
{
	m_ParameterMap.clear();
}