/**
 * @file DistributionFunction2.cpp
 * @brief KDistributionFunction2 구현 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#include "StdAfx.h"
#include "DistributionFunction2.h"



KDistributionFunction2::KDistributionFunction2(void)
	:	KDistributionFunctionBase(KEMDistributionFunction2),
		m_dParam1(0.0f),
		m_dParam2(0.0f)
{
}


KDistributionFunction2::~KDistributionFunction2(void)
{
}


LPCTSTR KDistributionFunction2::Function(void)
{
	return m_strFunctions[1];
}


double KDistributionFunction2::Parameter1(void)
{
	return m_dParam1;
}


void KDistributionFunction2::Parameter1(double dParam)
{
	m_dParam1 = dParam;
}


double KDistributionFunction2::Parameter2(void)
{
	return m_dParam2;
}


void KDistributionFunction2::Parameter2(double dParam)
{
	m_dParam2 = dParam;
}


KDistributionFunction2& KDistributionFunction2::operator=(const KDistributionFunction2& r)
{
	KDistributionFunctionBase::operator=(r);

	this->m_dParam1 = r.m_dParam1;
	this->m_dParam2 = r.m_dParam2;

	return *this;
}