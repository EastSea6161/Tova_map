/**
 * @file ColumnNameManager.cpp
 * @brief KColumnNameManager ±¸Çö
 * @author 
 * @date 2010.08.23
 * @remark
 */


#include "StdAfx.h"
#include "ColumnNameManager.h"


/******************************************************************************
* KColumnNameManager                                                          *
******************************************************************************/


/**************************************
* Constructors / Destructor           *
**************************************/
KColumnNameManager::KColumnNameManager(void)
	: m_strPrefix( _T("") ),
	m_nColumnNumber(1)
{
}

KColumnNameManager::~KColumnNameManager(void)
{
}



/**************************************
* Operations                          *
**************************************/
CString KColumnNameManager::generateColumnName( void )
{
	CString strColumnName;
	strColumnName.Format( _T("%s%d"), m_strPrefix, m_nColumnNumber++ );

	return strColumnName;
}


void KColumnNameManager::updateColumnNameData( const CString& strColumnName )
{
	CString strPrefix = strColumnName.Left( 1 );
	CString strNumber = strColumnName.Right( strColumnName.GetLength() - 1 );

	if( strPrefix != m_strPrefix )
	{
		return;
	}

	int nCount = strNumber.GetLength();
	for( int i = 0; i < nCount; ++i )
	{
		TCHAR tcTemp = strNumber.GetAt( i );
		if( tcTemp >= '0' && tcTemp <= '9' )
		{
			continue;
		}
		else
		{
			return;
		}
	}
	
	TCHAR *ptcStop = NULL;
	unsigned int nNumber = _tcstol( strNumber, &ptcStop, 10 );
	
	if( m_nColumnNumber < nNumber )
	{
		m_nColumnNumber = nNumber;
	}
}
