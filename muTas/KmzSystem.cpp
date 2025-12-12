#include "StdAfx.h"
#include "KmzSystem.h"


KmzSystem::KmzSystem(void)
{
}

KmzSystem::~KmzSystem(void)
{
}

KEMPSYSTEMTYPE KmzSystem::GetSystemType()
{
    return m_emSystemType;
}

void KmzSystem::SetSystemType(KEMPSYSTEMTYPE systemType)
{
    m_emSystemType = systemType;
}

bool KmzSystem::IsPassengerSystem()
{
    if (m_emSystemType == KEMPASSENGER_SYSTEM) 
        return true;

    return false;
}

bool KmzSystem::IsFreightSystem()
{
    if (m_emSystemType == KEMFREIGHT_SYSTEM)
        return true;

    return false;
}

KEMEsriAppType KmzSystem::GetEsriAppType()
{
	return m_emEsriAppType;
}

void KmzSystem::SetEsriAppType( KEMEsriAppType systemType )
{
	m_emEsriAppType = systemType;
}

bool KmzSystem::IsArcGISEngineSystem()
{
	if (m_emEsriAppType == KEMEsriApp_ENGINE)
		return true;

	return false;
}

KEMPSYSTEMTYPE KmzSystem::m_emSystemType  = KEMPASSENGER_SYSTEM;
KEMEsriAppType KmzSystem::m_emEsriAppType = KEMEsriApp_ENGINE;
