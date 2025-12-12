/**
 * @file DataObject.cpp
 * @brief KDataObject Á¤ÀÇ
 * @author 
 * @date 2010.08.19
 * @remark
 */


#include "StdAfx.h"
#include "Object.h"

/******************************************************************************
* KDataObject                                                                 *
******************************************************************************/
ZKeyGenerator< unsigned int > KObject::KS_UniqueKeyGenerator;


/**************************************
* Constructors / Destructor           *
**************************************/
KObject::KObject( KEMObjectType type ) 
	: m_KEMObjectType( type ), 
	  m_strName( _T("") ),
	  m_strLocation( _T("") )
{
	m_nUniqueObjectID = KS_UniqueKeyGenerator.GenerateUniqueKey();
}


KObject::~KObject(void)
{
}


bool KObject::CreateObjectDirectory( void )
{
	if( m_strLocation.IsEmpty() == true )
	{
		return false;
	}

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if( (::CreateDirectory( m_strLocation, &sa ) == FALSE) &&
		(::GetLastError() != ERROR_ALREADY_EXISTS) )
	{
		return false;
	}
	return true;
}