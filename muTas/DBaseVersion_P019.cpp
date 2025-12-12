#include "StdAfx.h"
#include "DBaseVersion_P019.h"


KDBaseVersion_P019::KDBaseVersion_P019(void)
{
}


KDBaseVersion_P019::~KDBaseVersion_P019(void)
{

}

void KDBaseVersion_P019::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableDefaultCO2Equation(spDBaseConnection);
		TableDefalutCo2Interpolation(spDBaseConnection);
		TableDefaultInterHighwayPara(spDBaseConnection);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}


void KDBaseVersion_P019::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableAssignModel(spDBaseConnection);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}



void KDBaseVersion_P019::TableAssignModel( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_ASSIGN_MODEL);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [assign_model] (  "));
		strSQLCreate.Append(_T( "   [run_seq] Integer PRIMARY KEY  NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [start_date] DATETIME, "));
		strSQLCreate.Append(_T( "   [end_date] DATETIME, "));
		strSQLCreate.Append(_T( "   [type] Integer, "));
		strSQLCreate.Append(_T( "   [Description] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [average_time] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [vc] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [sum_volume] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [trip] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [mode_volume] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [vdf_time] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [ave_time_toll] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [vdf_toll_time] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [bus_Initial_volume] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_average_time] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [od_vdf_time] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_average_bus] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_average_subway] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_average_bussubway] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_bus_direct] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [od_bus_one_transfer] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_bus_two_transfer] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_bus] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_rail] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_busrail] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [od_inter_mode] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_num_passengers] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_total_length] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_avg_length] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_max_passengers] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [linfo_avg_passengers] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_max_second] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_avg_second] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_max_standing] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_avg_standing] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [linfo_max_congestion] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_avg_congestion] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_km_passengers] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_once_passengers] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [linfo_km_once_passengers] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [line_board_net] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [line_board_Transfer] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [line_board_Total] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [line_alight_net] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [line_alight_Transfer] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [line_alight_Total] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [line_passenger] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [node_bus_net_board] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [node_bus_transfer_board] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [node_bus_sum_board] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [node_bus_net_alight] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [node_bus_transfer_alight] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [node_bus_sum_alight] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [pr_auto] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [pr_bus] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [pr_rail] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [pr_transit] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [terminal_trip] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_ac_auto] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_ac_taxi] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [tr_ac_bus] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_ac_rail] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_ac_transit] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_er_auto] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_er_taxi] VARCHAR2, "));

		strSQLCreate.Append(_T( "   [tr_er_bus] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_er_rail] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_er_transit] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [tr_total] VARCHAR2 ) "));

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

void KDBaseVersion_P019::TableDefaultCO2Equation( KDBaseConPtr spDBaseConnection )
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
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_CO2_Equation_Info "));
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

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [Default_CO2_Equation_Info] ( "));
		strCreateSQL.AppendFormat(_T(" [master_cd] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [mode_id] INTEGER,  "));
		strCreateSQL.AppendFormat(_T(" [mode_name] VARCHAR2 )  "));
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

void KDBaseVersion_P019::TableDefalutCo2Interpolation( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_CO2_Interpolation_Data "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_CO2_Interpolation_Info "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
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

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [Default_CO2_Interpolation_Info] ( "));
		strCreateSQL.AppendFormat(_T(" [master_code] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [max_speed] INTEGER,  "));
		strCreateSQL.AppendFormat(_T(" [class_interval] VARCHAR2 )  "));
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

void KDBaseVersion_P019::TableDefaultInterHighwayPara( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_Intermodal_Highway_Para_Urban "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_Intermodal_Highway_Para_Regional "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}

	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE Default_Intermodal_Highway_Para_Duration "));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
		TxLogDebugException();
	}


	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [Default_Intermodal_Highway_Para_Urban] ( "));
		strCreateSQL.AppendFormat(_T(" [Master_Code] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [detail_mode_id] INTEGER,  "));
		strCreateSQL.AppendFormat(_T(" [mode_name] VARCHAR2,  "));
		strCreateSQL.AppendFormat(_T(" [Occupancy] DOUBLE, "));
		strCreateSQL.AppendFormat(_T(" [PCE] DOUBLE,  "));
		strCreateSQL.AppendFormat(_T(" CONSTRAINT [sqlite_autoindex_Default_Inter_Highway_Para_Urban] PRIMARY KEY ([Master_Code], [detail_mode_id]));  "));
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

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [Default_Intermodal_Highway_Para_Regional] ( "));
		strCreateSQL.AppendFormat(_T(" [Master_Code] INTEGER, "));
		strCreateSQL.AppendFormat(_T(" [detail_mode_id] INTEGER,  "));
		strCreateSQL.AppendFormat(_T(" [mode_name] VARCHAR2,  "));
		strCreateSQL.AppendFormat(_T(" [Occupancy] DOUBLE, "));
		strCreateSQL.AppendFormat(_T(" [PCE] DOUBLE,  "));
		strCreateSQL.AppendFormat(_T(" CONSTRAINT [sqlite_autoindex_Default_Inter_Highway_Para_Regional] PRIMARY KEY ([Master_Code], [detail_mode_id]));  "));
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

	try
	{
		CString strCreateSQL(_T(""));
		strCreateSQL.AppendFormat(_T(" CREATE TABLE [Default_Intermodal_Highway_Para_Duration] ( "));
		strCreateSQL.AppendFormat(_T(" [Master_Code] INTEGER PRIMARY KEY, "));
		strCreateSQL.AppendFormat(_T(" [Time_Duration] DOUBLE )  "));
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
