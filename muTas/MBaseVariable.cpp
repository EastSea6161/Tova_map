/**
 * @file MBaseVariable.cpp
 * @brief KMBaseVariable 구현 파일
 * @author 
 * @date 2011.06.08
 * @remark
 */


#include "StdAfx.h"
#include "MBaseVariable.h"


KMBaseVariable::KMBaseVariable(KEMModeVariableType emVariableType)
	:	m_emModeVariableType(emVariableType),
		m_pModeTree(NULL)
{
}


KMBaseVariable::KMBaseVariable(const KMBaseVariable& r)
	:	m_emModeVariableType(r.m_emModeVariableType),
		m_pModeTree(r.m_pModeTree)
{

}


KMBaseVariable::~KMBaseVariable(void)
{
}


KEMModeVariableType KMBaseVariable::ModeVariableType(void)
{
	return m_emModeVariableType;
}


KMBaseVariable& KMBaseVariable::operator=(const KMBaseVariable& r)
{
	m_emModeVariableType = r.m_emModeVariableType;
	m_pModeTree = r.m_pModeTree;
	return *this;
}


KModeTree* KMBaseVariable::ModeTree(void)
{
	return m_pModeTree;
}


void KMBaseVariable::ModeTree(KModeTree* pModeTree)
{
	m_pModeTree = pModeTree;
}


CString KMBaseVariable::VariableName()
{
	return m_strName;
}


void KMBaseVariable::VariableName(CString a_strName)
{
	m_strName = a_strName;
}


int KMBaseVariable::VariableID()
{
	return m_nID;
}


void KMBaseVariable::VariableID(int a_nID)
{
	m_nID = a_nID;
}