#include "StdAfx.h"
#include "InterDefineCostPublicSector.h"


KInterDefineCostPublicSector::KInterDefineCostPublicSector(void)
{
}

KInterDefineCostPublicSector::KInterDefineCostPublicSector(int a_nIndex, CString a_strModeName)
	:	m_nIndex(a_nIndex),
		m_strModeName(a_strModeName),
		m_dBaseCharge(0.0),
		m_dBaseDistance(0.0),
		m_dAddCharge(0.0),
		m_dAddDistance(0.0)
{
}

KInterDefineCostPublicSector::~KInterDefineCostPublicSector(void)
{
}

int KInterDefineCostPublicSector::GetIndex()
{
	return m_nIndex;
}

void KInterDefineCostPublicSector::SetIndex(int a_nIndex)
{
	m_nIndex = a_nIndex;
}

CString KInterDefineCostPublicSector::GetModeName()
{
	return m_strModeName;
}

void KInterDefineCostPublicSector::SetModeName(CString a_strModeName)
{
	m_strModeName = a_strModeName;
}

double KInterDefineCostPublicSector::GetBaseCharge()
{
	return m_dBaseCharge;
}

void KInterDefineCostPublicSector::SetBaseCharge(double a_dblBaseCharge)
{
	m_dBaseCharge = a_dblBaseCharge;
}

double KInterDefineCostPublicSector::GetBaseDistance()
{
	return m_dBaseDistance;
}

void KInterDefineCostPublicSector::SetBaseDistance(double a_dblBaseDistance)
{
	m_dBaseDistance = a_dblBaseDistance;
}

double KInterDefineCostPublicSector::GetAddCharge()
{
	return m_dAddCharge;
}

void KInterDefineCostPublicSector::SetAddCharge(double a_dblAddCharge)
{
	m_dAddCharge = a_dblAddCharge;
}

double KInterDefineCostPublicSector::GetAddDistance()
{
	return m_dAddDistance;
}

void KInterDefineCostPublicSector::SetAddDistance(double a_dblAddDistance)
{
	m_dAddDistance = a_dblAddDistance;
}

