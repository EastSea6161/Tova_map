#include "StdAfx.h"
#include "DBaseVersion_P012.h"


KDBaseVersion_P012::KDBaseVersion_P012(void)
{
}


KDBaseVersion_P012::~KDBaseVersion_P012(void)
{
}

void KDBaseVersion_P012::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableFindHistory(spDBaseConnection);
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

void KDBaseVersion_P012::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableAssignTurnVolumeinput(spDBaseConnection);
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

void KDBaseVersion_P012::TableAssignTurnVolumeinput( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strSQLDrop (_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_ASSIGN_TURN_VOLUME_INPUT);
		spDBaseConnetion->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [assign_turn_volume_input] (  "));
		strSQLCreate.Append(_T( "   [mode] VARCHAR NOT NULL,  "));
		strSQLCreate.Append(_T( "   [busvolume] INTEGER ) "));
		spDBaseConnetion->ExecuteUpdate(strSQLCreate);
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

void KDBaseVersion_P012::TableFindHistory( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop (_T("DROP TABLE Find_History "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [Find_History] (  "));
		strSQLCreate.Append(_T( "   [Seq] INTEGER NOT NULL,  "));
		strSQLCreate.Append(_T( "   [Type] INTEGER NOT NULL, "));
		strSQLCreate.Append(_T( "   [Obj_ID] INTEGER ) "));
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
