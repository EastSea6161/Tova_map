#include "StdAfx.h"
#include "DBaseVersion_P023.h"


KDBaseVersion_P023::KDBaseVersion_P023(void)
{
}


KDBaseVersion_P023::~KDBaseVersion_P023(void)
{
}

void KDBaseVersion_P023::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableMultiAssignInfo(spDBaseConnection);
		TableMultiAssignData(spDBaseConnection);
	}
	catch (...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P023::TableMultiAssignInfo( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE multi_assign_info"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [multi_assign_info] ( "));
		strCreateSQL.AppendFormat(_T(" [ID] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [Name] VARCHAR2, "));
		strCreateSQL.AppendFormat(_T(" [Modes] VARCHAR2) "));

		spDBaseConnection->ExecuteUpdate(strCreateSQL);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P023::TableMultiAssignData( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE multi_assign_data"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [multi_assign_data] ( "));
		strCreateSQL.AppendFormat(_T(" [ID] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [Scenario] VARCHAR2, "));
		strCreateSQL.AppendFormat(_T(" [Target] VARCHAR2, "));
		strCreateSQL.AppendFormat(_T(" [Data] VARCHAR2) "));

		spDBaseConnection->ExecuteUpdate(strCreateSQL);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}
