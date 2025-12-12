#include "StdAfx.h"
#include "DBaseVersion_P018.h"


KDBaseVersion_P018::KDBaseVersion_P018(void)
{
}


KDBaseVersion_P018::~KDBaseVersion_P018(void)
{
}

void KDBaseVersion_P018::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableDefaultIntermodalPCEMode(spDBaseConnection);
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

void KDBaseVersion_P018::TableDefaultIntermodalPCEMode( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_PCE_MODE);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{

	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [Default_Intermodal_PCE_Mode] (  "));
		strSQLCreate.Append(_T( "   [Master_Code] Integer,  "));
		strSQLCreate.Append(_T( "   [Use] Integer,  "));
		strSQLCreate.Append(_T( "   [detail_mode_id] Integer,  "));
		strSQLCreate.Append(_T( "   [mode_name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [Occupancy] Double,  "));
		strSQLCreate.Append(_T( "   [PCE] Double,  "));
		strSQLCreate.Append(_T( "    CONSTRAINT [sqlite_autoindex_Default_Highway_Assign_PCE_Mode_1] PRIMARY KEY ([Master_Code], [detail_mode_id]) )  "));
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
