/**
 * @file IOColumnToString.cpp
 * @brief KIOcolumnToString 구현 파일
 * @author 
 * @date 2011.04.06
 * @remark
 */



#include "StdAfx.h"
#include "IOColumnToString.h"
#include "IOColumnToInsertSQL.h"
#include "IOColumnToUpdateSQL.h"


KIOColumnToString::KIOColumnToString(LPCTSTR strTableName, KIOColumn& column, KEMIOColumnToString emCS)
{
	switch(emCS)
	{
	case KEMIOColumnToStringInsert : 
		m_pToString = new KIOColumnToInsertSQL(strTableName, column);
		break;
	case KEMIOColumnToStringUpdate : 
		m_pToString = new KIOColumnToUpdateSQL(strTableName, column); 
		break;
	default : 
		m_pToString = NULL;
	}
}


KIOColumnToString::~KIOColumnToString(void)
{
	if(NULL != m_pToString)
	{
		delete m_pToString;
	}
}


CString KIOColumnToString::GetFieldsString(void)
{
	if(NULL != m_pToString)
	{
		return m_pToString->GetFieldsString();
	}

	return _T("");
}


CString KIOColumnToString::GetValuesString(void)
{
	if(NULL != m_pToString)
	{
		return m_pToString->GetValuesString();
	}
	return _T("");
}

CString KIOColumnToString::GetFieldsAndValuesString(void)
{
	if(NULL != m_pToString)
	{
		return m_pToString->GetFieldsAndValuesString();
	}
	return _T("");
}