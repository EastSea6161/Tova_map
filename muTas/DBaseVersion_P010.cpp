#include "StdAfx.h"
#include "DBaseVersion_P010.h"


KDBaseVersion_P010::KDBaseVersion_P010(void)
{
}


KDBaseVersion_P010::~KDBaseVersion_P010(void)
{
}

void KDBaseVersion_P010::Upgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		GroupGeneration(spDBaseConnection);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 1;
	}    
}

void KDBaseVersion_P010::GroupGeneration( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop (_T(" DROP TABLE Group_Generation_OD "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{

	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [Group_Generation_OD] (  "));
		strSQLCreate.Append(_T( "   [seq] INTEGER NOT NULL PRIMARY KEY,  "));
		strSQLCreate.Append(_T( "   [OD_Group_Key] INTEGER, "));
		strSQLCreate.Append(_T( "   [Output_Column] VARCHAR2)   "));   
		spDBaseConnection->ExecuteUpdate(strSQLCreate);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 41;
	}
	
}

