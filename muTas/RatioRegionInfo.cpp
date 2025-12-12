#include "StdAfx.h"
#include "RatioRegionInfo.h"


KRatioRegionInfo::KRatioRegionInfo(void)
{
}


KRatioRegionInfo::~KRatioRegionInfo(void)
{
}


void KRatioRegionInfo::SetCommdityCode(int a_nCommdityCode)
{
	m_nCommdityCode = a_nCommdityCode;
}


void KRatioRegionInfo::SetCommdityGroupCode( int a_nCommdityCodeGroup )
{
	m_nCommdityCodeGroup = a_nCommdityCodeGroup;
}


void KRatioRegionInfo::SetSEDCode( CString a_nSEDCode )
{
	m_nSEDCode = a_nSEDCode;
}


int KRatioRegionInfo::GetCommdityCode()
{
	return m_nCommdityCode;
}


int KRatioRegionInfo::GetCommdityCodeGroup()
{
	return m_nCommdityCodeGroup;
}


CString KRatioRegionInfo::GetSEDCode()
{
	return m_nSEDCode;
}

void KRatioRegionInfo::SetCommdityName( CString a_strCommdityName )
{
	m_strCommdityName = a_strCommdityName;
}

CString KRatioRegionInfo::GetCommdityName()
{
	return m_strCommdityName;
}

void KRatioRegionInfo::SetSEDCaption( CString a_strSEDCaption )
{
	m_strSEDCaption = a_strSEDCaption;
}

CString KRatioRegionInfo::GetSEDCaption()
{
	return m_strSEDCaption;
}


void KRatioRegionInfo::SetRatioReionCode( CString a_strRatioReionCode )
{
	m_strRatioReionCode = a_strRatioReionCode;
}

CString KRatioRegionInfo::GetRatioReionCode()
{
	return m_strRatioReionCode;
}

