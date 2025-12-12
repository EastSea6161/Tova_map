#include "StdAfx.h"
#include "TableOwner.h"

KIOTableOwner::KIOTableOwner(KEMTableOwnerType emOwnerType)
	:	m_emOwnerType(emOwnerType)
{
}

KIOTableOwner::~KIOTableOwner(void)
{
}

KEMTableOwnerType KIOTableOwner::GetOwnerType(void)
{
	return m_emOwnerType;
}
