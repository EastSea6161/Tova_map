/**
 * @file DistributionFunctionBase.cpp
 * @brief KDistributionFunctionBase 구현 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#include "StdAfx.h"
#include "DistributionFunctionBase.h"

const LPCTSTR KDistributionFunctionBase::m_strFunctions[3] = {
	_T("EXP(-β*Rij)"), _T("α*EXP(-β*Rij)"), _T("α*Rij-β*EXP(-θ*Rij)")
};


KDistributionFunctionBase::KDistributionFunctionBase(KEMDistributionFunction emFunction)
	:	m_emFunctionType(emFunction),
		m_emVariableType(KEMImpedanceVariableTypeTime),
        m_strVariableColumnName(_T(""))
{
}


KDistributionFunctionBase::~KDistributionFunctionBase(void)
{
}


KEMImpedanceVariableType KDistributionFunctionBase::VariableType(void)
{
	return m_emVariableType;
}


void KDistributionFunctionBase::VariableType(KEMImpedanceVariableType emVariableType)
{
	m_emVariableType = emVariableType;
}


KEMDistributionFunction KDistributionFunctionBase::FunctionType(void)
{
	return m_emFunctionType;
}


void KDistributionFunctionBase::FunctionType(KEMDistributionFunction emFunctionType)
{
	m_emFunctionType = emFunctionType;
}


KDistributionFunctionBase& KDistributionFunctionBase::operator=(const KDistributionFunctionBase& r)
{
	this->m_emFunctionType = r.m_emFunctionType;
	this->m_emVariableType = r.m_emVariableType;
    this->m_strVariableColumnName = r.m_strVariableColumnName;

	return *this;
}


LPCTSTR KDistributionFunctionBase::FunctionAt(int nIndex)
{
	int nCount = sizeof(m_strFunctions) / sizeof(LPCTSTR);
	ASSERT((0 <= nIndex) && (nIndex < nCount));

	return m_strFunctions[nIndex];
}

void KDistributionFunctionBase::RijColumnName(CString variableColumnName)
{
    m_strVariableColumnName = variableColumnName;
}

CString KDistributionFunctionBase::RijColumnName()
{
    return m_strVariableColumnName;
}