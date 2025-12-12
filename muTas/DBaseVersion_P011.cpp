#include "StdAfx.h"
#include "DBaseVersion_P011.h"
#include "DefineNamesDefaultPara.h"


KDBaseVersion_P011::KDBaseVersion_P011(void)
{
}


KDBaseVersion_P011::~KDBaseVersion_P011(void)
{
}


void KDBaseVersion_P011::ProjectDBUpgrade( KDBaseConPtr a_spPDBaseConnection)
{
	try
	{
		if (a_spPDBaseConnection != nullptr)
		{
			TableDefaultTripDistribution(a_spPDBaseConnection);
			TableCategoryAgeGroup(a_spPDBaseConnection);
			TableCategoryClass(a_spPDBaseConnection);
			TableCategoryClassValue(a_spPDBaseConnection);
			TableDefaultModeChoiceValue(a_spPDBaseConnection);
			TableDefaultModeChoiceValueInfo(a_spPDBaseConnection);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KDBaseVersion_P011::TableDefaultTripDistribution( KDBaseConPtr spDBaseConnetion )
{

// 	CString strSQL(_T(""));
// 	strSQL.Format(_T("Select count(*) From %s Where name = '%s'"),TABLE_SQLITE_MASTER, KDefaultParaTable::TABLE_DEFAULT_TRIP_DESTRIBUTION);
// 	KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
// 
// 	int nCount(0);
// 	if (spResultset->Next())
// 	{
// 		nCount = spResultset->GetValueInt(spResultset->GetValueInt(0));
// 	}

// 	if (nCount == 0)
// 	{
// 		CString strSQLCreate(_T(""));
// 		strSQLCreate.Append(_T( " CREATE TABLE [Default_Trip_Distribution] (  "));
// 		strSQLCreate.Append(_T( "   [Master_Code] INTEGER NOT NULL,  "));
// 		strSQLCreate.Append(_T( "   [Object_ID] INTEGER NOT NULL, "));
// 		strSQLCreate.Append(_T( "   [Function_Type] INTEGER, "));
// 		strSQLCreate.Append(_T( "   [Alpha] DOUBLE , "));
// 		strSQLCreate.Append(_T( "   [Beta] DOUBLE , "));
// 		strSQLCreate.Append(_T( "   [Theta] DOUBLE , "));
// 		strSQLCreate.Append(_T( "   [Rij] VARCHAR , "));
// 		strSQLCreate.Append(_T( "   PRIMARY KEY (Master_Code, Object_id)) "));
// 		spDBaseConnetion->ExecuteUpdate(strSQLCreate);
// 	}

	try
	{
		CString strSQLDrop (_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), KDefaultParaTable::TABLE_DEFAULT_TRIP_DESTRIBUTION);
		spDBaseConnetion->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	CString strSQLCreate(_T(""));
	strSQLCreate.Append(_T( " CREATE TABLE [Default_Trip_Distribution] (  "));
	strSQLCreate.Append(_T( "   [Master_Code] INTEGER NOT NULL,  "));
	strSQLCreate.Append(_T( "   [Object_ID] INTEGER NOT NULL, "));
	strSQLCreate.Append(_T( "   [Function_Type] INTEGER, "));
	strSQLCreate.Append(_T( "   [Alpha] DOUBLE , "));
	strSQLCreate.Append(_T( "   [Beta] DOUBLE , "));
	strSQLCreate.Append(_T( "   [Theta] DOUBLE , "));
	strSQLCreate.Append(_T( "   [Rij] VARCHAR , "));
	strSQLCreate.Append(_T( "   PRIMARY KEY (Master_Code, Object_id)) "));
	spDBaseConnetion->ExecuteUpdate(strSQLCreate);
}

void KDBaseVersion_P011::TableCategoryAgeGroup( KDBaseConPtr spDBaseConnetion )
{
// 	CString strSQL(_T(""));
// 	strSQL.Format(_T("Select count(*) From %s Where name = '%s'"),TABLE_SQLITE_MASTER, TABLE_CATEGORY_AGE_GROUP);
// 	KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
// 
// 	int nCount(0);
// 	if (spResultset->Next())
// 	{
// 		nCount = spResultset->GetValueInt(spResultset->GetValueInt(0));
// 	}
// 
// 	if (nCount == 0)
// 	{
// 		CString strSQLCreate(_T(""));
// 		strSQLCreate.Append(_T( " CREATE TABLE [Category_Age_Group] (  "));
// 		strSQLCreate.Append(_T( "   [category_id] INTEGER NOT NULL,  "));
// 		strSQLCreate.Append(_T( "   [seq] INTEGER NOT NULL, "));
// 		strSQLCreate.Append(_T( "   [min] INTEGER, "));
// 		strSQLCreate.Append(_T( "   [max] INTEGER , "));
// 		strSQLCreate.Append(_T( "   PRIMARY KEY (category_id, seq)) "));
// 
// 		spDBaseConnetion->ExecuteUpdate(strSQLCreate);
// 	}

	try
	{
		CString strSQLDrop (_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_CATEGORY_AGE_GROUP);
		spDBaseConnetion->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	CString strSQLCreate(_T(""));
	strSQLCreate.Append(_T( " CREATE TABLE [Category_Age_Group] (  "));
	strSQLCreate.Append(_T( "   [category_id] INTEGER NOT NULL,  "));
	strSQLCreate.Append(_T( "   [seq] INTEGER NOT NULL, "));
	strSQLCreate.Append(_T( "   [min] INTEGER, "));
	strSQLCreate.Append(_T( "   [max] INTEGER , "));
	strSQLCreate.Append(_T( "   PRIMARY KEY (category_id, seq)) "));

	spDBaseConnetion->ExecuteUpdate(strSQLCreate);
}

void KDBaseVersion_P011::TableCategoryClass( KDBaseConPtr spDBaseConnetion )
{

// 	CString strSQL(_T(""));
// 	strSQL.Format(_T("Select count(*) From %s Where name = '%s'"),TABLE_SQLITE_MASTER, TABLE_CATEGORY_CLASS);
// 	KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
// 
// 	int nCount(0);
// 	if (spResultset->Next())
// 	{
// 		nCount = spResultset->GetValueInt(spResultset->GetValueInt(0));
// 	}
// 
// 	if (nCount == 0)
// 	{
// 		CString strSQLCreate(_T(""));
// 		strSQLCreate.Append(_T( " CREATE TABLE [Category_Class] (  "));
// 		strSQLCreate.Append(_T( "   [category_id] INTEGER NOT NULL,  "));
// 		strSQLCreate.Append(_T( "   [name] INTEGER NOT NULL, "));
// 		strSQLCreate.Append(_T( "   [sex_use] INTEGER, "));
// 		strSQLCreate.Append(_T( "   [age_use] INTEGER, "));
// 		strSQLCreate.Append(_T( "   PRIMARY KEY (category_id)) "));
// 
// 		spDBaseConnetion->ExecuteUpdate(strSQLCreate);
// 	}


	try
	{
		CString strSQLDrop (_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_CATEGORY_CLASS);
		spDBaseConnetion->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	CString strSQLCreate(_T(""));
	strSQLCreate.Append(_T( " CREATE TABLE [Category_Class] (  "));
	strSQLCreate.Append(_T( "   [category_id] INTEGER NOT NULL,  "));
	strSQLCreate.Append(_T( "   [name] INTEGER NOT NULL, "));
	strSQLCreate.Append(_T( "   [sex_use] INTEGER, "));
	strSQLCreate.Append(_T( "   [age_use] INTEGER, "));
	strSQLCreate.Append(_T( "   PRIMARY KEY (category_id)) "));

	spDBaseConnetion->ExecuteUpdate(strSQLCreate);
}

void KDBaseVersion_P011::TableCategoryClassValue( KDBaseConPtr spDBaseConnetion )
{
// 	CString strSQL(_T(""));
// 	strSQL.Format(_T("Select count(*) From %s Where name = '%s'"),TABLE_SQLITE_MASTER, TABLE_CATEGORY_CLASS_VALUE);
// 	KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
// 
// 	int nCount(0);
// 	if (spResultset->Next())
// 	{
// 		nCount = spResultset->GetValueInt(spResultset->GetValueInt(0));
// 	}
// 
// 	if (nCount == 0)
// 	{
// 		CString strSQLCreate(_T(""));
// 
// 		strSQLCreate.Append(_T( " CREATE TABLE [Category_Class_Value] (  "));
// 		strSQLCreate.Append(_T( "   [category_id] INTEGER NOT NULL,  "));
// 		strSQLCreate.Append(_T( "   [male_female] INTEGER NOT NULL, "));
// 		strSQLCreate.Append(_T( "   [age_seq] INTEGER, "));
// 		strSQLCreate.Append(_T( "   [value] DOUBLE, "));
// 		strSQLCreate.Append(_T( "   PRIMARY KEY (category_id , male_female, age_seq)) "));
// 
// 		spDBaseConnetion->ExecuteUpdate(strSQLCreate);
// 	}
	try
	{
		CString strSQLDrop (_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_CATEGORY_CLASS_VALUE);
		spDBaseConnetion->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	CString strSQLCreate(_T(""));

	strSQLCreate.Append(_T( " CREATE TABLE [Category_Class_Value] (  "));
	strSQLCreate.Append(_T( "   [category_id] INTEGER NOT NULL,  "));
	strSQLCreate.Append(_T( "   [male_female] INTEGER NOT NULL, "));
	strSQLCreate.Append(_T( "   [age_seq] INTEGER, "));
	strSQLCreate.Append(_T( "   [value] DOUBLE, "));
	strSQLCreate.Append(_T( "   PRIMARY KEY (category_id , male_female, age_seq)) "));

	spDBaseConnetion->ExecuteUpdate(strSQLCreate);

}

void KDBaseVersion_P011::TableDefaultModeChoiceValue( KDBaseConPtr spDBaseConnetion )
{
// 	CString strSQL(_T(""));
// 	strSQL.Format(_T("Select count(*) From %s Where name = '%s'"),TABLE_SQLITE_MASTER, KDefaultParaTable::TABLE_DEFAULT_MODECHOICE_DEFINE_VALUE);
// 	KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
// 
// 	int nCount(0);
// 	if (spResultset->Next())
// 	{
// 		nCount = spResultset->GetValueInt(spResultset->GetValueInt(0));
// 	}
// 
// 	if (nCount == 0)
// 	{
// 		CString strSQLCreate(_T(""));
// 
// 		strSQLCreate.Append(_T( " CREATE TABLE [Default_ModeChoice_Define_Value] (  "));
// 		strSQLCreate.Append(_T( "   [purpose_id] INTEGER NOT NULL,   "));
// 		strSQLCreate.Append(_T( "   [KCode_Group_Key] INTEGER NOT NULL, "));
// 		strSQLCreate.Append(_T( "   [KCode_Key] INTEGER NOT NULL, "));
// 		strSQLCreate.Append(_T( "   [function_id] INTEGER, "));
// 		strSQLCreate.Append(_T( "   PRIMARY KEY (purpose_id , KCode_Group_Key, KCode_Key)) "));
// 
// 		spDBaseConnetion->ExecuteUpdate(strSQLCreate);
// 	}


	try
	{
		CString strSQLDrop (_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), KDefaultParaTable::TABLE_DEFAULT_MODECHOICE_DEFINE_VALUE);
		spDBaseConnetion->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	CString strSQLCreate(_T(""));

	strSQLCreate.Append(_T( " CREATE TABLE [Default_ModeChoice_Define_Value] (  "));
	strSQLCreate.Append(_T( "   [purpose_id] INTEGER NOT NULL,   "));
	strSQLCreate.Append(_T( "   [KCode_Group_Key] INTEGER NOT NULL, "));
	strSQLCreate.Append(_T( "   [KCode_Key] INTEGER NOT NULL, "));
	strSQLCreate.Append(_T( "   [function_id] INTEGER, "));
	strSQLCreate.Append(_T( "   PRIMARY KEY (purpose_id , KCode_Group_Key, KCode_Key)) "));

	spDBaseConnetion->ExecuteUpdate(strSQLCreate);

}

void KDBaseVersion_P011::TableDefaultModeChoiceValueInfo( KDBaseConPtr spDBaseConnetion )
{
// 	CString strSQL(_T(""));
// 	strSQL.Format(_T("Select count(*) From %s Where name = '%s'"),TABLE_SQLITE_MASTER, KDefaultParaTable::TABLE_DEFAULT_MODECHOICE_DEFINEVAL_INFO);
// 	KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
// 
// 	int nCount(0);
// 	if (spResultset->Next())
// 	{
// 		nCount = spResultset->GetValueInt(spResultset->GetValueInt(0));
// 	}
// 
// 	if (nCount == 0)
// 	{
// 		CString strSQLCreate(_T(""));
// 
// 		strSQLCreate.Append(_T( " CREATE TABLE [Default_ModeChoice_DefineVal_Info] (  "));
// 		strSQLCreate.Append(_T( "   [purpose_id] INTEGER NOT NULL,   "));
// 		strSQLCreate.Append(_T( "   [use_odgroup] INTEGER NOT NULL, "));
// 		strSQLCreate.Append(_T( "   [KCode_Group_Key] INTEGER NOT NULL, "));
// 		strSQLCreate.Append(_T( "   [none_function_id] INTEGER, "));
// 
// 		spDBaseConnetion->ExecuteUpdate(strSQLCreate);
// 	}

	try
	{
		CString strSQLDrop (_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), KDefaultParaTable::TABLE_DEFAULT_MODECHOICE_DEFINEVAL_INFO);
		spDBaseConnetion->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	CString strSQLCreate(_T(""));

	strSQLCreate.Append(_T( " CREATE TABLE [Default_ModeChoice_DefineVal_Info] (  "));
	strSQLCreate.Append(_T( "   [purpose_id] INTEGER NOT NULL,   "));
	strSQLCreate.Append(_T( "   [use_odgroup] INTEGER NOT NULL, "));
	strSQLCreate.Append(_T( "   [KCode_Group_Key] INTEGER NOT NULL, "));
	strSQLCreate.Append(_T( "   [none_function_id] INTEGER )"));

	spDBaseConnetion->ExecuteUpdate(strSQLCreate);

}
