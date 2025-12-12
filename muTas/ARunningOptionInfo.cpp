/**
 * @file ARunningOptionInfo.cpp
 * @brief KARunningOptionInfo 구현 파일
 * @author 
 * @date 2011.06.21
 * @remark
 */

#include "StdAfx.h"
#include "ARunningOptionInfo.h"
#include "Scenario.h"
#include "Target.h"

KARunningOptionInfo::KARunningOptionInfo(void)
	:	m_bTurnRestriction(false),
		m_bBusInitialVolume(false),
		m_bPathAnalysis(false),
		m_bIntersectionDelay(false),
		m_bIncludeWarmStart(false),
		m_bIncludeTurnVolume(false),
		m_bIncludeBalancing(false),
		m_nIteration(0),
		m_dMaximumPathRate(0.0001),
		m_pTarget(nullptr),
		m_pScnario(nullptr)
{
}


KARunningOptionInfo::KARunningOptionInfo(const KARunningOptionInfo& r)
	:	m_bTurnRestriction(r.m_bTurnRestriction),
		m_bBusInitialVolume(r.m_bBusInitialVolume),
		m_bPathAnalysis(r.m_bPathAnalysis),
		m_bIntersectionDelay(r.m_bIntersectionDelay),
		m_bIncludeWarmStart(r.m_bIncludeWarmStart),
		m_bIncludeTurnVolume(r.m_bIncludeTurnVolume),
		m_bIncludeBalancing(r.m_bIncludeBalancing),
		m_nIteration(r.m_nIteration),
		m_dMaximumPathRate(r.m_dMaximumPathRate),
		m_pTarget(r.m_pTarget),
		m_pScnario(r.m_pScnario)
{

}


KARunningOptionInfo::~KARunningOptionInfo(void)
{

}


KARunningOptionInfo& KARunningOptionInfo::operator=(const KARunningOptionInfo& r)
{
	m_bTurnRestriction		= r.m_bTurnRestriction;
	m_bBusInitialVolume		= r.m_bBusInitialVolume;
	m_bPathAnalysis			= r.m_bPathAnalysis;
	m_bIntersectionDelay	= r.m_bIntersectionDelay;
	m_bIncludeWarmStart		= r.m_bIncludeWarmStart;
	m_bIncludeTurnVolume	= r.m_bIncludeTurnVolume;
	m_bIncludeBalancing		= r.m_bIncludeBalancing;
	m_nIteration			= r.m_nIteration;
	m_dMaximumPathRate		= r.m_dMaximumPathRate;
	m_pTarget				= r.m_pTarget;
	m_pScnario				= r.m_pScnario;

	return *this;
}


bool KARunningOptionInfo::IncludeTurnRestriction(void)
{
	return m_bTurnRestriction;
}


void KARunningOptionInfo::IncludeTurnRestriction(bool bInclude)
{
	m_bTurnRestriction = bInclude;
}


bool KARunningOptionInfo::IncludeBusInitialVolume(void)
{
	return m_bBusInitialVolume;
}


void KARunningOptionInfo::IncludeBusInitialVolume(bool bInclude)
{
	m_bBusInitialVolume = bInclude;
}

bool KARunningOptionInfo::IncludePathAnalysis(void)
{
	return m_bPathAnalysis;
}


void KARunningOptionInfo::IncludePathAnalysis(bool bInclude)
{
	m_bPathAnalysis = bInclude;
}

bool KARunningOptionInfo::IncludeIntersectionDelay(void)
{
	return m_bIntersectionDelay;
}


void KARunningOptionInfo::IncludeIntersectionDelay(bool bInclude)
{
	m_bIntersectionDelay = bInclude;
}


CString KARunningOptionInfo::GetArgumentInfo()
{
    CString strOut;
    strOut.Format(_T("%d\t%d\t%d\t%d\t%d\r\n"), 
                    m_bTurnRestriction?1:0, 
                    m_bBusInitialVolume?1:0,
                    m_bPathAnalysis?1:0,
                    m_bIntersectionDelay?1:0,
					m_bIncludeTurnVolume?1:0 );

    return strOut;
}

bool KARunningOptionInfo::IncludeWarmStart( void )
{
	return m_bIncludeWarmStart;
}

void KARunningOptionInfo::IncludeWarmStart( bool bInclude )
{
	m_bIncludeWarmStart = bInclude;
}

bool KARunningOptionInfo::IncludeTurnVolume( void )
{
	return m_bIncludeTurnVolume;
}

void KARunningOptionInfo::IncludeTurnVolume( bool bTurnVolume )
{
	m_bIncludeTurnVolume = bTurnVolume;
}

bool KARunningOptionInfo::IncludeBalancing( void )
{
	return m_bIncludeBalancing;
}

void KARunningOptionInfo::IncludeBalancing( bool bBalancing )
{
	m_bIncludeBalancing = bBalancing;
}