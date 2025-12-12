#include "StdAfx.h"
#include "Mode.h"


KMode::KMode() : m_emModeGroup(KEMModeGroupODBase), m_nID(0), m_strName(_T(""))
{	
}


KMode::KMode(KEMModeGroup emGroup, int nModeID, LPCTSTR strModeName)
	:	m_emModeGroup(emGroup),
		m_nID(nModeID),
		m_strName(strModeName)
{
}


KMode::~KMode(void)
{
}


KEMModeGroup KMode::ModeGroup(void)
{
	return m_emModeGroup;
}


void KMode::ModeGroup(KEMModeGroup emGroup)
{
	m_emModeGroup = emGroup;
}


int KMode::ModeID(void)
{
	return m_nID;
}


void KMode::ModeID(int nID)
{
	m_nID = nID;
}


LPCTSTR KMode::ModeName(void)
{
	return m_strName;
}


void KMode::ModeName(LPCTSTR strName)
{
	m_strName = strName;
}


bool KMode::MatchModeGroup(int a_nModeCode, KEMModeGroup &a_emModeGroup)
{
	switch(a_nModeCode)
	{
	case 11:

		a_emModeGroup = KEMModeGroupODBase;
		break;
	case 21:

		a_emModeGroup = KEMModeGroupHomeBase;
		break;
	case 22:

		a_emModeGroup = KEMModeGroupNoneHomeBase;
		break;
	case 61:

		a_emModeGroup = KEMModeGroupFreightOD;
		break;
	default:

		return false;
		break;
	}

	return true;
}