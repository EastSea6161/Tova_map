/**
 * @file IOQueryBuilder.cpp
 * @brief KIOQueryBuilder 구현 파일
 * @author 
 * @date 2011.03.25
 * @remark
 */


#include "StdAfx.h"
#include "IOQueryBuilder.h"
#include "IOcolumn.h"
#include "IOColumns.h"


KIOQueryBuilder::KIOQueryBuilder(void)
{
}

KIOQueryBuilder::~KIOQueryBuilder(void)
{
}


CString KIOQueryBuilder::BuildSelectQuery(const KIOColumns* pColumns, LPCTSTR szTable)
{
	CString strQuery;
	int nColumnCount = pColumns->ColumnCount();
	if(nColumnCount <= 0)
	{
		return _T("");
	}

	CString strSelect = _T("SELECT ");
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		strSelect += pColumn->Name();
		if(i < nColumnCount-1)
		{
			strSelect += _T(",");
		}
		strSelect += _T(" ");
	}

	strQuery = strSelect;
	strQuery += _T("FROM ");
	strQuery += szTable;

	return strQuery;
}


CString KIOQueryBuilder::BuildSelectQuery(const KIOColumns* pColumns, LPCTSTR szTable, LPCTSTR szWhere, LPCTSTR szOrder)
{
	CString strQuery  = BuildSelectQuery(pColumns, szTable);
	if(strQuery.IsEmpty() == true)
	{
		return _T("");
	}

	if(_tcslen(szWhere) > 0)
	{
		strQuery += _T(" WHERE ");
		strQuery += szWhere;
	}
    	
	if(_tcslen(szOrder) > 0)
	{
		strQuery += _T(" ORDER BY ");
		strQuery += szOrder;
	}
	return strQuery;
}