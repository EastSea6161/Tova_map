#include "StdAfx.h"
#include "ViewResultInfo.h"
#include "Scenario.h"
#include "Target.h"


KViewResultInfo::KViewResultInfo(void)
{
}


KViewResultInfo::~KViewResultInfo(void)
{
}

void KViewResultInfo::SetScenarioInfo( KScenario* a_pKSenario )
{
	m_pKScenarioInfo = a_pKSenario;
}

void KViewResultInfo::SetTargetInfo( KTarget* a_pKTarget )
{
	m_pKTargetInfo = a_pKTarget;
}

void KViewResultInfo::SetColumnInfo( KIOColumn* a_pKColumn )
{
	m_pkColumn = a_pKColumn;
}

KScenario* KViewResultInfo::GetSenarioInfo()
{
	return m_pKScenarioInfo;
}

KTarget* KViewResultInfo::GetTargetInfo()
{
	return m_pKTargetInfo;
}

KIOColumn* KViewResultInfo::GetColumnInfo()
{
	return m_pkColumn;
}
