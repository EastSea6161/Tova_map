#include "StdAfx.h"
#include "ModelChoiceArgInfo.h"


KModelChoiceArgInfo::KModelChoiceArgInfo(void)
{
}


KModelChoiceArgInfo::~KModelChoiceArgInfo(void)
{
}

void KModelChoiceArgInfo::PurposeIndex( int a_nPurposeIndex )
{
	m_nPurposeINdex = a_nPurposeIndex;
}

int KModelChoiceArgInfo::PurposeIndex()
{
	return m_nPurposeINdex;
}

void KModelChoiceArgInfo::FileName( CString a_strFileName )
{
	m_strFileName = a_strFileName;
}

CString KModelChoiceArgInfo::FileName()
{
	return m_strFileName;
}

void KModelChoiceArgInfo::CountNotNested( int a_nNotNested )
{
	m_nNotNestedCount = a_nNotNested;
}

int KModelChoiceArgInfo::CountNotNested()
{
	return m_nNotNestedCount;
}

void KModelChoiceArgInfo::SetMode( std::set<int> a_setMode )
{
	m_setMode = a_setMode;
}

void KModelChoiceArgInfo::GetMode( std::set<int>& a_setMode )
{
	a_setMode = m_setMode;
}

void KModelChoiceArgInfo::CountNested( int a_Nested )
{
	m_nNestedCount = a_Nested;
}

int KModelChoiceArgInfo::CoutNestd()
{
	return m_nNestedCount;
}

void KModelChoiceArgInfo::SetNestedInfo( std::vector<TNestedInfo*> a_vecNestedInfo )
{
	m_vecNestedInfo = a_vecNestedInfo;
}

void KModelChoiceArgInfo::GetNestedInfo( std::vector<TNestedInfo*>& a_vecNestedInfo )
{
	a_vecNestedInfo = m_vecNestedInfo;
}

