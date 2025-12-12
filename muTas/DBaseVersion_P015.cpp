#include "StdAfx.h"
#include "DBaseVersion_P015.h"


KDBaseVersion_P015::KDBaseVersion_P015(void)
{
}


KDBaseVersion_P015::~KDBaseVersion_P015(void)
{
}

void KDBaseVersion_P015::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableEditNetworkLog(spDBaseConnection);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}


void KDBaseVersion_P015::TableEditNetworkLog( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_NETWORKEDIT_LOG);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{

	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [NetworkEditLog] (  "));
		strSQLCreate.Append(_T( "   [LAYER] INT NOT NULL DEFAULT 0,  "));
		strSQLCreate.Append(_T( "   [KEY_VALUE] INTEGER NOT NULL DEFAULT 0,  "));
		strSQLCreate.Append(_T( "   [EDIT_TYPE] INT NOT NULL DEFAULT 0,   "));
		strSQLCreate.Append(_T( "   [EDIT_TIME] DATETIME DEFAULT (datetime('now', 'localtime')),  "));
		strSQLCreate.Append(_T( "   [CHECK_DATA] INTEGER DEFAULT 0,  "));
		strSQLCreate.Append(_T( "   CONSTRAINT [] PRIMARY KEY ([LAYER], [KEY_VALUE]));  "));

		spDBaseConnection->ExecuteUpdate(strSQLCreate);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}

}
