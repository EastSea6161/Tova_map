/**
 * @file MGroupVariable.cpp
 * @brief KMGroupVariable 구현 파일
 * @author 
 * @date 2011.06.09
 * @remark
 */


#include "StdAfx.h"
#include "MGroupVariable.h"
#include "ModeFunction.h"



KMGroupVariable::KMGroupVariable(void)
	:	KMBaseVariable(KEMModeVariableTypeGroup)
{
}


KMGroupVariable::KMGroupVariable(const KMGroupVariable& r)
	:	KMBaseVariable(r)
{
	std::map<KMGVKey, KModeFunction*>::const_iterator itFunction, itFEnd = r.m_FunctionMap.end();
	for(itFunction = r.m_FunctionMap.begin(); itFunction != itFEnd; ++itFunction)
	{
		KModeFunction* pFunction = new KModeFunction(*(itFunction->second));
		m_FunctionMap[itFunction->first] = pFunction;
	}

	m_ParameterMap = r.m_ParameterMap;
}


KMGroupVariable::~KMGroupVariable(void)
{
	RemoveAllModeFunction();
}


KMGroupVariable& KMGroupVariable::operator=(const KMGroupVariable& r)
{
	KMBaseVariable::operator=(r);

	RemoveAllModeFunction();
	std::map<KMGVKey, KModeFunction*>::const_iterator itFunction, itFEnd = r.m_FunctionMap.end();
	for(itFunction = r.m_FunctionMap.begin(); itFunction != itFEnd; ++itFunction)
	{
		KModeFunction* pFunction = new KModeFunction(*(itFunction->second));
		m_FunctionMap[itFunction->first] = pFunction;
	}

	m_ParameterMap = r.m_ParameterMap;

	return *this;
}


KModeFunction* KMGroupVariable::ModeFunction(int nGroupCode, int nModeID)
{
	KModeFunction* pRet = NULL;
	KMGVKey key(nGroupCode, nModeID);
	std::map<KMGVKey, KModeFunction*>::iterator itFunction = m_FunctionMap.find(key);
	if(m_FunctionMap.end() != itFunction)
	{
		pRet = itFunction->second;
	}

	return pRet;
}


void KMGroupVariable::ModeFunction(int nGroupCode, int nModeID, KModeFunction* pFunction)
{
	KMGVKey key(nGroupCode, nModeID);

	std::map<KMGVKey, KModeFunction*>::iterator itFunction = m_FunctionMap.find(key);
	if(m_FunctionMap.end() != itFunction)
	{
		KModeFunction* pOldFunction = itFunction->second;
		*pOldFunction = *pFunction;
	}
	else
	{
		KModeFunction* pNewFunction = new KModeFunction(*pFunction);
		m_FunctionMap[key] = pNewFunction;
	}
}


void KMGroupVariable::RemoveModeFunction(int nGroupCode, int nModeID)
{
	KMGVKey key(nGroupCode, nModeID);
	std::map<KMGVKey, KModeFunction*>::iterator itFunction = m_FunctionMap.find(key);
	if(m_FunctionMap.end() != itFunction)
	{
		KModeFunction* pFunction = itFunction->second;
		delete pFunction;
		m_FunctionMap.erase(itFunction);
	}
}


void KMGroupVariable::RemoveAllModeFunction(void)
{
	std::map<KMGVKey, KModeFunction*>::iterator itFunction, itEnd = m_FunctionMap.end();
	for(itFunction = m_FunctionMap.begin(); itFunction != itEnd; ++itFunction)
	{
		KModeFunction* pFunction = itFunction->second;
		delete pFunction;
	}
	m_FunctionMap.clear();
}


double KMGroupVariable::ModeParameter(int nGroupCode, int nModeID)
{
	double dRet = 0.0f;
	KMGVKey key(nGroupCode, nModeID);

	std::map<KMGVKey, double>::iterator itParam = m_ParameterMap.find(key);
	if(m_ParameterMap.end() != itParam)
	{
		dRet = itParam->second;
	}
	
	return dRet;
}


void KMGroupVariable::ModeParameter(int nGroupCode, int nModeID, double dParameter)
{
	KMGVKey key(nGroupCode, nModeID);
	m_ParameterMap[key] = dParameter;
}


void KMGroupVariable::RemoveModeParameter(int nGroupCode, int nModeID)
{
	KMGVKey key(nGroupCode, nModeID);
	std::map<KMGVKey, double>::iterator itParam = m_ParameterMap.find(key);
	if(m_ParameterMap.end() != itParam)
	{
		m_ParameterMap.erase(itParam);
	}
}


void KMGroupVariable::RemoveAllModeParameter(void)
{
	m_ParameterMap.clear();
}