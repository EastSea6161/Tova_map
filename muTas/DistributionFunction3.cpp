/**
 * @file DistributionFunction3.cpp
 * @brief KDistributionFunction3 구현 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#include "StdAfx.h"
#include "DistributionFunction3.h"



KDistributionFunction3::KDistributionFunction3(void)
	:	KDistributionFunctionBase(KEMDistributionFunction3),
		m_dParam1(0.0f),
		m_dParam2(0.0f),
		m_dParam3(0.0f)
{
}


KDistributionFunction3::~KDistributionFunction3(void)
{
}


LPCTSTR KDistributionFunction3::Function(void)
{
	return m_strFunctions[2];
}


double KDistributionFunction3::Parameter1(void)
{
	return m_dParam1;
}


void KDistributionFunction3::Parameter1(double dParam)
{
	m_dParam1 = dParam;
}


double KDistributionFunction3::Parameter2(void)
{
	return m_dParam2;
}


void KDistributionFunction3::Parameter2(double dParam)
{
	m_dParam2 = dParam;
}


double KDistributionFunction3::Parameter3(void)
{
	return m_dParam3;
}


void KDistributionFunction3::Parameter3(double dParam)
{
	m_dParam3 = dParam;
}


KDistributionFunction3& KDistributionFunction3::operator=(const KDistributionFunction3& r)
{
	KDistributionFunctionBase::operator=(r);

	this->m_dParam1 = r.m_dParam1;
	this->m_dParam2 = r.m_dParam2;
	this->m_dParam3 = r.m_dParam3;

	return *this;
}