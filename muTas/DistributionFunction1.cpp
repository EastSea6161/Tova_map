/**
 * @file DistributionFunction1.cpp
 * @brief KDistributionFunction1 구현 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#include "StdAfx.h"
#include "DistributionFunction1.h"



KDistributionFunction1::KDistributionFunction1(void)
	:	KDistributionFunctionBase(KEMDistributionFunction1),
		m_dParam1(0.0f)
{
}


KDistributionFunction1::~KDistributionFunction1(void)
{
}


LPCTSTR KDistributionFunction1::Function(void)
{
	return m_strFunctions[0];
}


double KDistributionFunction1::Parameter2(void)
{
	return m_dParam1;
}


void KDistributionFunction1::Parameter2(double dParam)
{
	m_dParam1 = dParam;
}


KDistributionFunction1& KDistributionFunction1::operator=(const KDistributionFunction1& r)
{
	KDistributionFunctionBase::operator=(r);
	this->m_dParam1 = r.m_dParam1;

	return *this;
}