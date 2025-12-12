#include "StdAfx.h"
#include "InterDefineCostPrivateSector.h"


KInterDefineCostPrivateSector::KInterDefineCostPrivateSector(void)
{
}


KInterDefineCostPrivateSector::KInterDefineCostPrivateSector(int a_nIndex, CString a_strName)
	:	m_nIndex(a_nIndex),
		m_strName(a_strName),
		m_dblPrice(0.0),
		m_dblRatio(0.0),
		m_dblFuelEfficiency(0.0)
{
}

KInterDefineCostPrivateSector::~KInterDefineCostPrivateSector(void)
{
}


int KInterDefineCostPrivateSector::GetIndex()
{
	return m_nIndex;
}

void KInterDefineCostPrivateSector::SetIndex(int a_nIndex)
{
	m_nIndex = a_nIndex;
}


CString KInterDefineCostPrivateSector::GetName()
{
	return m_strName;
}

void KInterDefineCostPrivateSector::SetName(CString a_strName)
{
	m_strName = a_strName;
}


double KInterDefineCostPrivateSector::GetPrice()
{
	return m_dblPrice;
}

void KInterDefineCostPrivateSector::SetPrice(double a_dblPrice)
{
	m_dblPrice = a_dblPrice;
}


double KInterDefineCostPrivateSector::GetRatio()
{
	return m_dblRatio;
}

void KInterDefineCostPrivateSector::SetRatio(double a_dblRatio)
{
	m_dblRatio = a_dblRatio;
}


double KInterDefineCostPrivateSector::GetFuelEfficiency()
{
	return m_dblFuelEfficiency;
}

void KInterDefineCostPrivateSector::SetFuelEfficiency(double a_dblFuelEfficiency)
{
	m_dblFuelEfficiency = a_dblFuelEfficiency;
}