/**
 * @file Purpose.cpp
 * @brief KPurpose 구현 파일
 * @author 
 * @date 2011.04.22
 * @remark
 */



#include "StdAfx.h"
#include "Purpose.h"

KPurpose::KPurpose() : m_emPurposeGroup(KEMPurposeGroupODBase), m_nPurposeID(0), m_strPurposeName(_T(""))
{	
}


KPurpose::KPurpose(KEMPurposeGroup emPurposeGroup, int nPurposeID, LPCTSTR strPurposeName)
	:	m_emPurposeGroup(emPurposeGroup),
		m_nPurposeID(nPurposeID),
		m_strPurposeName(strPurposeName)
{
}


KPurpose::~KPurpose(void)
{

}


KEMPurposeGroup KPurpose::PurposeGroup(void)
{
	return m_emPurposeGroup;
}


void KPurpose::PurposeGroup(KEMPurposeGroup emPurposeGroup)
{
	m_emPurposeGroup = emPurposeGroup;
}


int KPurpose::PurposeID(void)
{
	return m_nPurposeID;
}


void KPurpose::PurposeID(int nPurposeID)
{
	m_nPurposeID = nPurposeID;
}


LPCTSTR KPurpose::PurposeName(void)
{
	return m_strPurposeName;
}


void KPurpose::PurposeName(LPCTSTR strPurposeName)
{
	m_strPurposeName = strPurposeName;
}


bool KPurpose::MatchPurposeGroup(int a_nGroupCode, KEMPurposeGroup &a_emPurposeGroup)
{
	switch(a_nGroupCode)
	{
	case 11:

		a_emPurposeGroup = KEMPurposeGroupODBase;
		break;
	case 21:

		a_emPurposeGroup = KEMPurposeGroupHomeBase;
		break;
	case 22:

		a_emPurposeGroup = KEMPurposeGroupNoneHomeBase;
		break;
	case 61:

		a_emPurposeGroup = KEMPurposeGroupFreightOD;
		break;
	default:
		
		return false;
		break;
	}

	return true;
}