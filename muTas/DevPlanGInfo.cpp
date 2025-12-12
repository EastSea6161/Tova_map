#include "StdAfx.h"
#include "DevPlanGInfo.h"


KDevPlanGInfo::KDevPlanGInfo(void)
	: m_pPurpose(NULL)
{
}

KDevPlanGInfo::~KDevPlanGInfo(void)
{
}

KPAPurpose* KDevPlanGInfo::Purpose( void )
{
	return m_pPurpose;
}

void KDevPlanGInfo::Purpose( KPAPurpose* a_pPurpose )
{
	m_pPurpose = a_pPurpose;
}