#include "StdAfx.h"
#include "InterFunctionBase.h"


KInterFunctionBase::KInterFunctionBase(KEMInterModalFunction a_emFunctionType)
	:	m_emFunctionType(a_emFunctionType)
{
}

KInterFunctionBase::KInterFunctionBase(const KInterFunctionBase& r)
	:	m_emFunctionType(r.m_emFunctionType)
{
}

KInterFunctionBase::~KInterFunctionBase(void)
{
}

KInterFunctionBase& KInterFunctionBase::operator=(const KInterFunctionBase& r)
{
	m_emFunctionType = r.m_emFunctionType;
	return *this;
}

KEMInterModalFunction KInterFunctionBase::FunctionType(void)
{
	return m_emFunctionType;
}