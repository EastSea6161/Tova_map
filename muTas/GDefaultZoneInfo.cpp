#include "StdAfx.h"
#include "ZoneGroupValue.h"
#include "GDefaultZoneInfo.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "XmlManager.h"

KGDefaultZoneInfo::KGDefaultZoneInfo(void)
:	m_pZoneGroupValue(NULL)
{

}


KGDefaultZoneInfo::~KGDefaultZoneInfo(void)
{
}

void KGDefaultZoneInfo::SetElement(TiXmlElement* a_pElement)
{
	m_pElement = a_pElement;
}

void KGDefaultZoneInfo::SetZoneGroupValue(KZoneGroupValue* a_pKZoneGroupValue)
{
	m_pZoneGroupValue = a_pKZoneGroupValue;
}

TiXmlElement* KGDefaultZoneInfo::GetElement()
{
	return m_pElement;
}

KZoneGroupValue* KGDefaultZoneInfo::GetZoneGroupValue()
{
	return m_pZoneGroupValue;
}