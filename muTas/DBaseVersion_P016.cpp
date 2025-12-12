#include "StdAfx.h"
#include "DBaseVersion_P016.h"
#include "DefineNamesDefaultPara.h"

KDBaseVersion_P016::KDBaseVersion_P016(void)
{
}


KDBaseVersion_P016::~KDBaseVersion_P016(void)
{
}

void KDBaseVersion_P016::IolocalDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableIntertermialtimeInfo(spDBaseConnection);
		TableTerminalResult(spDBaseConnection);
		TableAssignModel(spDBaseConnection);
		TableInterModeInfo(spDBaseConnection);
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

void KDBaseVersion_P016::TableIntertermialtimeInfo( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_INTER_TERMINAL_TIME_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{

	}
	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [inter_terminal_time_info] (  "));
		strSQLCreate.Append(_T( "   [id] INTEGER PRIMARY KEY  NOT NULL,  "));
		strSQLCreate.Append(_T( "   [name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [access_time] DOUBLE DEFAULT 0,   "));
		strSQLCreate.Append(_T( "   [egress_time] DOUBLE DEFAULT 0,  "));
		strSQLCreate.Append(_T( "   [mode_transfer] INTEGER DEFAULT 0 )  "));

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

void KDBaseVersion_P016::ProjectDBUpgrade( KDBaseConPtr spDBaseConnection )
{
	try
	{
		TableDefaultInterterminaltimeInfo(spDBaseConnection);
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

void KDBaseVersion_P016::TableDefaultInterterminaltimeInfo(KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_TERMINAL_TIME);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{

	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [Default_Intermodal_Regional_Terminal_Time] (  "));
		strSQLCreate.Append(_T( "   [Master_Code] Integer,   "));
		strSQLCreate.Append(_T( "   [Id] Integer,  "));
		strSQLCreate.Append(_T( "   [name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [access_time] DOUBLE DEFAULT 0,   "));
		strSQLCreate.Append(_T( "   [egress_time] DOUBLE DEFAULT 0,  "));
		strSQLCreate.Append(_T( "   [mode_transfer] INTEGER DEFAULT 0,  "));
		strSQLCreate.Append(_T( "   CONSTRAINT [] PRIMARY KEY ([Master_Code], [Id]))  "));

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

void KDBaseVersion_P016::TableTerminalResult( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_TERMINAL_RESULT);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{
	}

	try
	{
		CString strDelColumnDesciption(_T(""));
		strDelColumnDesciption.Format(_T("Delete From Column_Description where table_name = '%s'"), TABLE_TERMINAL_RESULT);
		spDBaseConnection->ExecuteUpdate(strDelColumnDesciption);

		CString strDelTableMaster(_T(""));
		strDelTableMaster.Format(_T("Delete From Table_Master where table_name = '%s'"), TABLE_TERMINAL_RESULT);
		spDBaseConnection->ExecuteUpdate(strDelTableMaster);

	}
	catch(...)
	{
	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [terminal_result] (  "));
		strSQLCreate.Append(_T( "   [terminal_id] Integer PRIMARY KEY  NOT NULL ,  "));
		strSQLCreate.Append(_T( "   [name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [IM_TM_ACC_Auto] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_ACC_Taxi] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_ACC_Bus] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_ACC_Subway] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_ACC_Transit] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_EGR_Auto] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_EGR_Taxi] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_EGR_Bus] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_EGR_Subway] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_EGR_Transit] DOUBLE,  "));
		strSQLCreate.Append(_T( "   [IM_TM_Total] DOUBLE )  "));
		spDBaseConnection->ExecuteUpdate(strSQLCreate);

		CString strInsertColumnDescription(_T(""));
		strInsertColumnDescription.Format(_T(" Insert Into Column_Description ( Table_name, Column_name, Data_type, Column_type, Code_type, display_yn, freeze_yn, Display_name, Column_Info, Display_Order, PK_YN, Use_Flag ) "));
		strInsertColumnDescription.Append(_T(" VALUES "));

		CString strInsertTerminalID = strInsertColumnDescription;
		strInsertTerminalID.Append(_T(" ('terminal_result','terminal_id','1','-1','-1','Y','Y','Terminal_ID','Terminal_ID', '1','Y','1' )"));
		spDBaseConnection->ExecuteUpdate(strInsertTerminalID);

		CString strInsertTerminalName = strInsertColumnDescription;
		strInsertTerminalName.Append(_T(" ('terminal_result','name','3','-1','-1','Y','Y','Name','Name', '2','N','1') "));
		spDBaseConnection->ExecuteUpdate(strInsertTerminalName);

		CString strInsertTableMaster(_T(""));
		strInsertTableMaster.Append(_T(" Insert Into Table_Master ( Table_name, Display_name, Table_info, Table_Type, Display_Type, Display_Order )"));
		strInsertTableMaster.Append(_T(" Values "));
		strInsertTableMaster.Append(_T(" ('terminal_result','Terminal_Info','터미널 결과값','-9',-1,-1)"));
		spDBaseConnection->ExecuteUpdate(strInsertTableMaster);

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

void KDBaseVersion_P016::TableAssignModel( KDBaseConPtr spDBaseConnection )
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
		strSQLCreate.Append(_T( "   [linfo_num_passengers] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_bus] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_rail] VARCHAR2, "));
		
		strSQLCreate.Append(_T( "   [od_busrail] VARCHAR2, "));
		strSQLCreate.Append(_T( "   [od_inter_mode] VARCHAR2, "));
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

void KDBaseVersion_P016::TableInterModeInfo( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop(_T(""));
		strSQLDrop.Format(_T("DROP TABLE %s "), TABLE_ASSIGN_INTER_MODE_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch(...)
	{

	}

	try
	{
		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE [assign_inter_mode_info] (  "));
		strSQLCreate.Append(_T( "   [Id] Integer,   "));
		strSQLCreate.Append(_T( "   [Name] VARCHAR2,  "));
		strSQLCreate.Append(_T( "   [Column] VARCHAR2 )  "));

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

