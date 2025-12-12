#include "StdAfx.h"
#include "DBaseVersion_P021.h"


KDBaseVersion_P021::KDBaseVersion_P021(void)
{
}


KDBaseVersion_P021::~KDBaseVersion_P021(void)
{
}

void KDBaseVersion_P021::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableInterTimeInfo(spDBaseConnection);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P021::TableInterTimeInfo( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE inter_time_info "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}


	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [inter_time_info] ( "));
		strCreateSQL.AppendFormat(_T(" [type] INTEGER NOT NULL, "));
		strCreateSQL.AppendFormat(_T(" [taxi_time] DOUBLE DEFAULT 0,  "));
		strCreateSQL.AppendFormat(_T(" [bus_time] DOUBLE DEFAULT 0,  "));
		strCreateSQL.AppendFormat(_T(" [subway_time] DOUBLE DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [alpha] DOUBLE DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [beta] DOUBLE DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [constaint_time] DOUBLE DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [constaint_length] DOUBLE DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [invehicle_time] INTEGER DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [waiting_time] INTEGER DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [transfer_time] INTEGER DEFAULT 0,  "));
		strCreateSQL.AppendFormat(_T(" [ae_time] INTEGER DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [use_turn] INTEGER DEFAULT 0, "));
		strCreateSQL.AppendFormat(_T(" [inter_vot] DOUBLE DEFAULT 0) "));

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

void KDBaseVersion_P021::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		DefaultIntermodalRegionalPathSetting(spDBaseConnection);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P021::DefaultIntermodalRegionalPathSetting( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_Intermodal_Regional_Path_Setting "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [Default_Intermodal_Regional_Path_Setting] ( "));
		strCreateSQL.AppendFormat(_T(" [Master_Code] Integer PRIMARY KEY, "));
		strCreateSQL.AppendFormat(_T(" [Taxi_Time] Double,   "));
		strCreateSQL.AppendFormat(_T(" [Highway_Time] Double,  "));
		strCreateSQL.AppendFormat(_T(" [Rail_Time] Double, "));
		strCreateSQL.AppendFormat(_T(" [inter_vot] Double ) "));

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
