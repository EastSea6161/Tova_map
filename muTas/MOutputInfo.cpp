/**
 * @file MOutputInfo.cpp
 * @brief KMOutputInfo 구현 파일
 * @author 
 * @date 2011.06.14
 * @remark
 */


#include "StdAfx.h"
#include "MOutputInfo.h"



KMOutputInfo::KMOutputInfo(void)
	:	m_bNewField(false),
		m_strName(_T("")),
		m_strCaption(_T(""))
{
}


KMOutputInfo::KMOutputInfo(const KMOutputInfo& r)
	:	m_bNewField(r.m_bNewField),
		m_strName(r.m_strName),
		m_strCaption(r.m_strCaption)
{

}


KMOutputInfo::~KMOutputInfo(void)
{
}


KMOutputInfo& KMOutputInfo::operator=(const KMOutputInfo& r)
{
	m_bNewField = r.m_bNewField;
	m_strName = r.m_strName;
	m_strCaption = r.m_strCaption;

	return *this;
}


bool KMOutputInfo::NewField(void)
{
	return m_bNewField;
}


void KMOutputInfo::NewField(bool bNew)
{
	m_bNewField = bNew;
}


LPCTSTR KMOutputInfo::FieldName(void)
{
	return m_strName;
}


void KMOutputInfo::FieldName(LPCTSTR strName)
{
	m_strName = strName;
}


LPCTSTR KMOutputInfo::FieldCaption(void)
{
	return m_strCaption;
}


void KMOutputInfo::FieldCaption(LPCTSTR strCaption)
{
	m_strCaption = strCaption;
}