#include "StdAfx.h"
#include "InterChoiceParamBase.h"

#include "InterChoiceParam.h"


KInterChoiceParamBase::KInterChoiceParamBase(void)
{
	m_bSelected = false;
}


KInterChoiceParamBase::~KInterChoiceParamBase(void)
{
	ClearChoiceParamVector();
}

void KInterChoiceParamBase::ClearChoiceParamVector(void)
{
	int nCntChoiceParamVector = m_vecChoiceParam.size();
	for(int i = 0; i < nCntChoiceParamVector; ++i)
	{
		KInterChoiceParam* pChoiceParam = m_vecChoiceParam[i];
		delete pChoiceParam;
	}

	m_vecChoiceParam.clear();
}

bool KInterChoiceParamBase::Selected(void)
{
	return m_bSelected;
}

void KInterChoiceParamBase::Selected(bool a_bSelected)
{
	m_bSelected = a_bSelected;
}

void KInterChoiceParamBase::Caption(LPCTSTR a_strCaption)
{
	m_strCaption = a_strCaption;
}

LPCTSTR	KInterChoiceParamBase::Caption(void)
{
	return m_strCaption;
}

std::vector<KInterChoiceParam*> KInterChoiceParamBase::GetInterChoiceParam(void)
{
	return m_vecChoiceParam;
}

void KInterChoiceParamBase::SetInterChoiceParam(std::vector<KInterChoiceParam*> a_vecChoiceParam)
{
	m_vecChoiceParam = a_vecChoiceParam;
}

void KInterChoiceParamBase::InitChoiceParamDataVector(void)
{
	int nCntChoiceParamVector = m_vecChoiceParam.size();
	for(int i = 0; i < nCntChoiceParamVector; ++i)
	{
		KInterChoiceParam* pChoiceParam = m_vecChoiceParam[i];
		pChoiceParam->InitParamData();
	}
}