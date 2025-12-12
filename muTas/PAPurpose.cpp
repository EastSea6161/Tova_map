#include "StdAfx.h"
#include "PAPurpose.h"
#include "Purpose.h"


KPAPurpose::KPAPurpose(KEMPurposeGroup emPurposeGroup, int nPurposeID, LPCTSTR strPurposeName, KEMPAType emPAType)
{
	m_pPurpose = new KPurpose(emPurposeGroup, nPurposeID, strPurposeName);
	m_emPAType = emPAType;
}


KPAPurpose::KPAPurpose(const KPurpose& p, KEMPAType emPAType)
{
	m_pPurpose = new KPurpose(p);
	m_emPAType = emPAType;
}


KPAPurpose::KPAPurpose(const KPAPurpose& r)
{
	m_pPurpose = new KPurpose(*(r.m_pPurpose));
	m_emPAType = r.m_emPAType;
}


KPAPurpose::~KPAPurpose(void)
{
	if(NULL != m_pPurpose)
	{
		delete m_pPurpose;
	}
}


KPAPurpose& KPAPurpose::operator=(const KPAPurpose& r)
{
	*m_pPurpose = *(r.m_pPurpose);
	m_emPAType = r.m_emPAType;

	return *this;
}


KEMPurposeGroup KPAPurpose::PurposeGroup(void)
{
	return m_pPurpose->PurposeGroup();
}

void KPAPurpose::PurposeGroup(KEMPurposeGroup emGroup)
{
	m_pPurpose->PurposeGroup(emGroup);
}


int KPAPurpose::PurposeID(void)
{
	return m_pPurpose->PurposeID();
}


void KPAPurpose::PurposeID(int nPurposeID)
{
	m_pPurpose->PurposeID(nPurposeID);
}


LPCTSTR KPAPurpose::PurposeName(void)
{
	return m_pPurpose->PurposeName();
}


void KPAPurpose::PurposeName(LPCTSTR strPurposeName)
{
	m_pPurpose->PurposeName(strPurposeName);
}


KEMPAType KPAPurpose::PAType(void)
{
	return m_emPAType;
}


void KPAPurpose::PAType(KEMPAType emPAType)
{
	m_emPAType = emPAType;
}