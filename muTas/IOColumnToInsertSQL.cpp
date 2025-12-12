/**
 * @file IOColumnToInsertSQL.cpp
 * @brief KIOColumnToInsertSQL 구현 파일
 * @author 
 * @date 2011.04.05
 * @remark
 */


#include "StdAfx.h"
#include "IOColumnToInsertSQL.h"
//^^ #include "IOColumn.h"




KIOColumnToInsertSQL::KIOColumnToInsertSQL(LPCTSTR strTableName, KIOColumn& column)
	:	KIOColumnToSQL(strTableName, column)
{
}

KIOColumnToInsertSQL::~KIOColumnToInsertSQL(void)
{
}


CString KIOColumnToInsertSQL::GetFieldsString(void)
{
	CString strFields = _T("");

	strFields += _T("table_name, column_name, data_type, column_type, code_type, display_yn, freeze_yn, ");
	strFields += _T("display_name, column_info, display_order, pk_yn");
	if(m_Column.BaseID() > 0)
	{
		strFields += _T(", base_id");
	}

	if(m_Column.CodeGroup() > 0)
	{
		strFields += _T(", kcode_group_key");
	}

	strFields += _T(" ");

	return strFields;
}


CString KIOColumnToInsertSQL::GetValuesString(void)
{
	CString strValues = _T("");
	CString strValue = _T("");

	// table name
	strValues += makeQuot(m_strTableName);

	// column name 
	strValues += _T(", ");
	strValues += makeQuot(m_Column.Name());

	// data type
	strValue.Format(_T("%d"), m_Column.DataType());
	strValues += _T(", ");
	strValues += makeQuot(strValue);

	// column type
	strValue.Format(_T("%d"), m_Column.ColumnType());
	strValues += _T(", ");
	strValues += makeQuot(strValue);

	// code type
	strValue.Format(_T("%d"), m_Column.CodeType());
	strValues += _T(", ");
	strValues += makeQuot(strValue);

	// display yn
	strValue = m_Column.Visible() == true ? _T("Y") : _T("N");
	strValues += _T(", ");
	strValues += makeQuot(strValue);

	// freeze yn
	strValue = m_Column.FreezeColumn() == true ? _T("Y") : _T("N");
	strValues += _T(", ");
	strValues += makeQuot(strValue);

	// display name
	strValues += _T(", ");
	strValues += makeQuot(m_Column.Caption());

	// column info
	strValues += _T(", ");
	strValues += makeQuot(m_Column.ColumnInfo());

	// display order
	strValue.Format(_T("%d"), m_Column.DisplayOrder());
	strValues += _T(", ");
	strValues += makeQuot(strValue);

	// pk yn
	strValue = m_Column.PrimaryKey() == true ? _T("Y") : _T("N");
	strValues += _T(", ");
	strValues += makeQuot(strValue);

	// base id
	if(m_Column.BaseID() > 0)
	{
		strValue.Format(_T("%d"), m_Column.BaseID());
		strValues += _T(", ");
		strValues += makeQuot(strValue);
	}

	// code group
	if(m_Column.CodeGroup() > 0)
	{
		strValue.Format(_T("%d"), m_Column.CodeGroup());
		strValues += _T(", ");
		strValues += makeQuot(strValue);
	}

	return strValues;
}


CString KIOColumnToInsertSQL::GetFieldsAndValuesString(void)
{
	CString strPair = _T("");
	CString strValue = _T("");

	// talbe name
	strPair += _T("talbe_name = ");
	strPair += makeQuot(m_strTableName);

	// column name
	strPair += _T(", column_name = ");
	strPair += makeQuot(m_Column.Name());

	// data type
	strValue.Format(_T("%d"), m_Column.DataType());
	strPair += _T(", data_type = ");
	strPair += makeQuot(strValue);

	// column type
	strValue.Format(_T("%d"), m_Column.ColumnType());
	strPair += _T(", column_type = ");
	strPair += makeQuot(strValue);

	// code type
	strValue.Format(_T("%d"), m_Column.CodeType());
	strPair += _T(", code_type = ");
	strPair += makeQuot(strValue);

	// display yn
	strValue = m_Column.Visible() == true ? _T("Y") : _T("N");
	strPair += _T(", display_yn = ");
	strPair += makeQuot(strValue);

	// freeze yn
	strValue = m_Column.FreezeColumn() == true ? _T("Y") : _T("N");
	strPair += _T(", freeze_yn = ");
	strPair += makeQuot(strValue);

	// display name
	strPair += _T(", display_name = ");
	strPair += makeQuot(m_Column.Caption());

	// column info
	strPair += _T(", column_info = ");
	strPair += makeQuot(m_Column.ColumnInfo());

	// display order
	strValue.Format(_T("%d"), m_Column.DisplayOrder());
	strPair += _T(", display_order = ");
	strPair += makeQuot(strValue);

	// pk yn
	strValue = m_Column.PrimaryKey() == true ? _T("Y") : _T("N");
	strPair += _T(", pk_yn = ");
	strPair += makeQuot(strValue);

	// base id
	if(m_Column.BaseID() > 0)
	{
		strValue.Format(_T("%d"), m_Column.BaseID());
		strPair += _T(", base_id = ");
		strPair += makeQuot(strValue);
	}

	//code group
	if(m_Column.CodeGroup() > 0)
	{
		strValue.Format(_T("%d"), m_Column.CodeGroup());
		strPair += _T(", kcode_group_key = ");
		strPair += makeQuot(strValue);
	}

	return strPair;
}