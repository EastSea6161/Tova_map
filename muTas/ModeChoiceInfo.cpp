/**
 * @file ModeChoiceInfo.cpp
 * @brief KModeChoiceInfo 구현 파일
 * @author 
 * @date 2011.06.01
 * @remark
 */


#include "StdAfx.h"
#include "ModeChoiceInfo.h"
#include "MAdjustmentDummy.h"
#include "MGroupVariable.h"
#include "MSimpleVariable.h"
#include "ModeTree.h"



KModeChoiceInfo::KModeChoiceInfo(void)
	:	m_pPurpose(NULL),
		m_pInputColumn(NULL)
{
	m_pModeTree = new KModeTree;
	m_pVariable = NULL;
	m_pDummy = NULL;
}


KModeChoiceInfo::KModeChoiceInfo(const KModeChoiceInfo& r)
	:	m_pPurpose(r.m_pPurpose)
{
	m_pModeTree = new KModeTree(*(r.m_pModeTree));
	m_pInputColumn = r.m_pInputColumn;

	m_pVariable = NULL;
	if(NULL != r.m_pVariable)
	{
		if(r.m_pVariable->ModeVariableType() == KEMModeVariableTypeSimple)
		{
			KMSimpleVariable* pSimpleVariable = new KMSimpleVariable(*(KMSimpleVariable*)(r.m_pVariable));
			m_pVariable = pSimpleVariable;
		}
		else
		{
			KMGroupVariable* pGroupVariable = new KMGroupVariable(*(KMGroupVariable*)(r.m_pVariable));
			m_pVariable = pGroupVariable;
		}
	}

	if(NULL != r.m_pDummy)
	{
		m_pDummy = new KMAdjustmentDummy(*(r.m_pDummy));
	}
}


KModeChoiceInfo::~KModeChoiceInfo(void)
{
	if(NULL != m_pVariable)
	{
		delete m_pVariable;
	}

	if(NULL != m_pDummy)
	{
		delete m_pDummy;
	}
}


KModeChoiceInfo& KModeChoiceInfo::operator=(const KModeChoiceInfo& r)
{
	m_pPurpose = r.m_pPurpose;
	m_pInputColumn = r.m_pInputColumn;

	*m_pModeTree = *(r.m_pModeTree);
	if(m_pVariable != NULL)
	{
		delete m_pVariable;
		m_pVariable = NULL;
	}
	
	if(NULL != r.m_pVariable)
	{
		if(r.m_pVariable->ModeVariableType() == KEMModeVariableTypeSimple)
		{
			KMSimpleVariable* pSimpleVariable = new KMSimpleVariable(*(KMSimpleVariable*)(r.m_pVariable));
			m_pVariable = pSimpleVariable;
		}
		else
		{
			KMGroupVariable* pGroupVariable = new KMGroupVariable(*(KMGroupVariable*)(r.m_pVariable));
			m_pVariable = pGroupVariable;
		}
	}

	if(NULL != m_pDummy)
	{
		delete m_pDummy;
		m_pDummy = NULL;
	}
	if(NULL != r.m_pDummy)
	{
		m_pDummy = new KMAdjustmentDummy(*(r.m_pDummy));
	}
	return *this;
}


KPurpose* KModeChoiceInfo::Purpose(void)
{
	return m_pPurpose;
}


void KModeChoiceInfo::Purpose(KPurpose* pPurpose)
{
	m_pPurpose = pPurpose;
}


KIOColumn* KModeChoiceInfo::InputColumn(void)
{
	return m_pInputColumn;
}


void KModeChoiceInfo::InputColumn(KIOColumn* pColumn)
{
	m_pInputColumn = pColumn;
}


KModeTree* KModeChoiceInfo::ModeTree(void)
{
	return m_pModeTree;
}


void KModeChoiceInfo::ModeTree(KModeTree* pModeTree)
{
	if(NULL != pModeTree)
	{
		*m_pModeTree = *pModeTree;
	}
}


KMBaseVariable* KModeChoiceInfo::Variable(void)
{
	return m_pVariable;
}


void KModeChoiceInfo::Variable(KMBaseVariable* pVariable)
{
	if(NULL != m_pVariable)
	{
		delete m_pVariable;
		m_pVariable = NULL;
	}
	if(NULL != pVariable)
	{
		if(pVariable->ModeVariableType() == KEMModeVariableTypeSimple)
		{
			KMSimpleVariable* pSimpleVariable = new KMSimpleVariable(*(KMSimpleVariable*)(pVariable));
			m_pVariable = pSimpleVariable;
		}
		else
		{
			KMGroupVariable* pGroupVariable = new KMGroupVariable(*(KMGroupVariable*)(pVariable));
			m_pVariable = pGroupVariable;
		}
	}
}


KMAdjustmentDummy* KModeChoiceInfo::AdjustmentDummy(void)
{
	return m_pDummy;
}


void KModeChoiceInfo::AdjustmentDummy(const KMAdjustmentDummy* pDummy)
{
	if(NULL != m_pDummy)
	{
		delete m_pDummy;
		m_pDummy = NULL;
	}

	if(NULL != pDummy)
	{
		m_pDummy = new KMAdjustmentDummy(*pDummy);
	}
}