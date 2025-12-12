/**
 * @file SqliteConnector.cpp
 * @brief KSqliteConnector 구현 파일
 * @author 
 * @date 2011.07.07
 * @remark
 */


#include "StdAfx.h"
#include "SqliteConnector.h"


KSqliteConnector::KSqliteConnector(LPCTSTR strFile)
{
	int nDBStatus = sqlite3_open16(strFile, &m_pDB);
	if(SQLITE_ERROR == nDBStatus)
	{
		m_bOpen = false;
		m_pDB = NULL;
	}
	else
	{
		m_bOpen = true;
	}
}


KSqliteConnector::~KSqliteConnector(void)
{
	SqliteClose(m_pDB);
}


bool KSqliteConnector::IsOpened(void)
{
	return m_bOpen;
}


KSqliteConnector::operator sqlite3*(void)
{
	return m_pDB;
}