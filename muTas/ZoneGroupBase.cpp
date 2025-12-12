/**
 * @file ZoneGroupBase.cpp
 * @brief KZoneGroupBae 구현 파일
 * @author 
 * @date 2011.05.11
 * @remark
 */


#include "StdAfx.h"
#include "ZoneGroupBase.h"


KZoneGroupBase::KZoneGroupBase(KEMZoneGroupObject emObjectType)
	:	m_emObjectType(emObjectType)
{
}


KZoneGroupBase::KZoneGroupBase(const KZoneGroupBase& r)
	:	m_emObjectType(r.m_emObjectType)
{

}


KZoneGroupBase::~KZoneGroupBase(void)
{
}


KEMZoneGroupObject KZoneGroupBase::ZoneGroupObjectType(void)
{
	return m_emObjectType;
}


KZoneGroupBase& KZoneGroupBase::operator=(const KZoneGroupBase& r)
{
	m_emObjectType = r.m_emObjectType;
	return *this;
}