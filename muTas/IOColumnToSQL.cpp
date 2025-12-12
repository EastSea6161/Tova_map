/**
 * @file IOColumnToSQL.cpp
 * @brief KIOColumnToSQL 구현파일
 * @author 
 * @date 2011.04.06
 * @remark
 */



#include "StdAfx.h"
#include "IOColumnToSQL.h"



KIOColumnToSQL::KIOColumnToSQL(LPCTSTR strTableName, KIOColumn& column)
	:	m_strTableName(strTableName),
		m_Column(column)
{

}

KIOColumnToSQL::~KIOColumnToSQL(void)
{
}


CString KIOColumnToSQL::makeQuot(LPCTSTR str)
{
	CString strQuot = _T("'");
	strQuot += str;
	strQuot += _T("'");

	return strQuot;	
}