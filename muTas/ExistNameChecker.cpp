/**
 * @file ExistNameChecker.cpp
 * @brief KExistNameChecker ±¸Çö
 * @author 
 * @date 2010.08.23
 * @remark
 */


#include "StdAfx.h"
#include "ExistNameChecker.h"



/******************************************************************************
* KExistNameChecker                                                           *
******************************************************************************/


/**************************************
* Constructors / Destructor           *
**************************************/
KExistNameChecker::KExistNameChecker(void)
{
}

KExistNameChecker::~KExistNameChecker(void)
{
}



/**************************************
* Operations                          *
**************************************/
void KExistNameChecker::addName( LPCTSTR strName )
{
	m_ExistNameSet.insert( strName );

	CString strUpperName = strName;
	strUpperName.MakeUpper();
	m_ExistNameSetNoCase.insert(strUpperName);
}


bool KExistNameChecker::isNameExists( LPCTSTR strName )
{
	std::set< CString >::iterator itNameSet = m_ExistNameSet.find( strName );
	if( m_ExistNameSet.end() == itNameSet )
	{
		return false;
	}
	return true;
}


bool KExistNameChecker::isNameExistsNoCase( LPCTSTR strName )
{
	CString strUpperName = strName;
	strUpperName.MakeUpper();

	std::set< CString >::iterator itNameSet = m_ExistNameSetNoCase.find( strUpperName );
	if( m_ExistNameSetNoCase.end() == itNameSet )
	{
		return false;
	}
	return true;
}

