#include "StdAfx.h"
#include "DBaseVersion_P020.h"


KDBaseVersion_P020::KDBaseVersion_P020(void)
{
}


KDBaseVersion_P020::~KDBaseVersion_P020(void)
{
}

void KDBaseVersion_P020::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableDefaultCO2Equation(spDBaseConnection);
		TableDefalutCo2Interpolation(spDBaseConnection);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseVersion_P020::TableDefaultCO2Equation( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_CO2_Equation_Data "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [Default_CO2_Equation_Data] ( "));
		strCreateSQL.AppendFormat(_T(" [master_cd] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [mode_id] INTEGER,  "));
		strCreateSQL.AppendFormat(_T(" [vehicle_name] VARCHAR2,  "));
		strCreateSQL.AppendFormat(_T(" [pce] DOUBLE,  "));
		strCreateSQL.AppendFormat(_T(" [fuel_name] VARCHAR2, "));
		strCreateSQL.AppendFormat(_T(" [ratio] DOUBLE, "));
		strCreateSQL.AppendFormat(_T(" [spd_gap] DOUBLE, "));
		strCreateSQL.AppendFormat(_T(" [is_selected] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [is_spd_gap_all_selected] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [exp_under] VARCHAR2, "));
		strCreateSQL.AppendFormat(_T(" [exp_andover] VARCHAR2 ) "));
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

void KDBaseVersion_P020::TableDefalutCo2Interpolation( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_CO2_Interpolation_Data "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);

		CString strDel(_T(""));
		strDel.Format(_T("Delete From Default_CO2_Interpolation_Info"));
		spDBaseConnection->ExecuteUpdate(strDel);

	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [Default_CO2_Interpolation_Data] ( "));
		strCreateSQL.AppendFormat(_T(" [master_code] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [mode_seq] INTEGER,  "));
		strCreateSQL.AppendFormat(_T(" [mode_name] VARCHAR2,  "));
		strCreateSQL.AppendFormat(_T(" [pce] DOUBLE,  "));
		strCreateSQL.AppendFormat(_T(" [speed_value] VARCHAR2, "));
		strCreateSQL.AppendFormat(_T(" [is_selected] DOUBLE ) "));
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
