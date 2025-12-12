#include "StdAfx.h"
#include "TimeODInfo.h"
//^^ #include "IOColumn.h"

KTimeODInfo::KTimeODInfo(void)
	:	m_pModeODColumn(NULL),
		m_pParameterODColumn(NULL)
{
}


KTimeODInfo::KTimeODInfo(const KTimeODInfo& r)
{
	m_pModeODColumn      = r.m_pModeODColumn;
	m_pParameterODColumn = r.m_pParameterODColumn;
}


KTimeODInfo::~KTimeODInfo(void)
{
}


KTimeODInfo& KTimeODInfo::operator=(const KTimeODInfo& r)
{
	m_pModeODColumn      = r.m_pModeODColumn;
	m_pParameterODColumn = r.m_pParameterODColumn;

	return *this;
}


KIOColumn* KTimeODInfo::ModeODColumn(void)
{
	return m_pModeODColumn;
}


void KTimeODInfo::ModeODColumn( KIOColumn* a_pModeODColumn )
{
	m_pModeODColumn = a_pModeODColumn;
}


KIOColumn* KTimeODInfo::ParameterODColumn(void)
{
	return m_pParameterODColumn;
}


void KTimeODInfo::ParameterODColumn( KIOColumn* a_pParameterODColumn )
{
	m_pParameterODColumn = a_pParameterODColumn;
}