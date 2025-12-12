#include "stdafx.h"
#include "DBaseVersion_P001.h"
//^#include "DBaseConnector.h"

void KDBaseVersion_P001::Upgrade( KDBaseConPtr spDBaseConnection )
{
    try
    {
        TableAccessStopNode    (spDBaseConnection);
        TableAccessStopNodeInfo(spDBaseConnection);
        TableAssignModel       (spDBaseConnection);
        TableAssignPathInput   (spDBaseConnection);
        TableAssignPathModel   (spDBaseConnection);

        TableBusPathInput      (spDBaseConnection);
        TableParameterOD       (spDBaseConnection);

        TableTransitCost       (spDBaseConnection);

        TasbleMaster           (spDBaseConnection);
        ColumnDescription      (spDBaseConnection);
        KCodeGroup             (spDBaseConnection);
        KCodeDefine            (spDBaseConnection);
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

void KDBaseVersion_P001::TableAccessStopNode( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE access_stop_node "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));

        strSQLCreate.Append(_T( " CREATE TABLE access_stop_node "));
        strSQLCreate.Append(_T( "   (zone_id INTEGER NOT NULL,  "));
        strSQLCreate.Append(_T( "   node_id INTEGER NOT NULL,   "));
        strSQLCreate.Append(_T( "   mode INTEGER,               "));
        strSQLCreate.Append(_T( "   zone_name VARCHAR2,         "));
        strSQLCreate.Append(_T( "   class_in_zone VARCHAR2,     "));
        strSQLCreate.Append(_T( "   class_in_network VARCHAR2,  "));
        strSQLCreate.Append(_T( "   no_of_line int, PRIMARY KEY (zone_id,node_id, mode)) "));

        spDBaseConnection->ExecuteUpdate(strSQLCreate);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 2;
    }    
}

void KDBaseVersion_P001::TableAccessStopNodeInfo( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE access_stop_node_info "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));
        strSQLCreate.Append(_T( " CREATE TABLE access_stop_node_info  "));
        strSQLCreate.Append(_T( "     (boundary DOUBLE,    "));
        strSQLCreate.Append(_T( "     using_code INTEGER); "));	

        spDBaseConnection->ExecuteUpdate(strSQLCreate);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 3;
    }    
}

void KDBaseVersion_P001::TableAssignModel(KDBaseConPtr spDBaseConnection)
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE assign_model "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));

        strSQLCreate.Append(_T( " CREATE TABLE assign_model "));
        strSQLCreate.Append(_T( "    (run_seq Integer PRIMARY KEY  NOT NULL , "));
        strSQLCreate.Append(_T( "    start_date DATETIME, "));
        strSQLCreate.Append(_T( "    end_date DATETIME, type INTEGER, "));
        strSQLCreate.Append(_T( "    Description VARCHAR2, "));
        strSQLCreate.Append(_T( "    average_time VARCHAR2, "));
        strSQLCreate.Append(_T( "    vc VARCHAR2, "));
        strSQLCreate.Append(_T( "    sum_volume VARCHAR2, "));
        strSQLCreate.Append(_T( "    trip VARCHAR2, "));
        strSQLCreate.Append(_T( "    mode_volume VARCHAR2, "));
        strSQLCreate.Append(_T( "    vdf_time VARCHAR2, "));
        strSQLCreate.Append(_T( "    ave_time_toll VARCHAR2, "));
        strSQLCreate.Append(_T( "    vdf_toll_time VARCHAR2, "));
        strSQLCreate.Append(_T( "    bus_Initial_volume VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_average_time VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_vdf_time VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_average_bus VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_average_subway VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_average_bussubway VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_bus_direct VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_bus_one_transfer VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_bus_two_transfer VARCHAR2, "));
        strSQLCreate.Append(_T( "    linfo_num_passengers VARCHAR2,  "));
        strSQLCreate.Append(_T( "    od_bus VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_rail VARCHAR2, "));
        strSQLCreate.Append(_T( "    od_busrail VARCHAR2, "));
        strSQLCreate.Append(_T( "    line_board_net VARCHAR2,  "));
        strSQLCreate.Append(_T( "    line_board_Transfer VARCHAR2,  "));
        strSQLCreate.Append(_T( "    line_board_Total VARCHAR2,  "));
        strSQLCreate.Append(_T( "    line_alight_net VARCHAR2,  "));
        strSQLCreate.Append(_T( "    line_alight_Transfer VARCHAR2,  "));
        strSQLCreate.Append(_T( "    line_alight_Total VARCHAR2, "));
        strSQLCreate.Append(_T( "    line_passenger VARCHAR2,  "));
        strSQLCreate.Append(_T( "    node_bus_net_board VARCHAR2,  "));
        strSQLCreate.Append(_T( "    node_bus_transfer_board VARCHAR2,  "));
        strSQLCreate.Append(_T( "    node_bus_sum_board VARCHAR2, "));
        strSQLCreate.Append(_T( "    node_bus_net_alight VARCHAR2,  "));
        strSQLCreate.Append(_T( "    node_bus_transfer_alight VARCHAR2,  "));
        strSQLCreate.Append(_T( "    node_bus_sum_alight VARCHAR2) "));

        spDBaseConnection->ExecuteUpdate(strSQLCreate);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 4;
    }    
}

void KDBaseVersion_P001::TableAssignPathInput( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE assign_path_input "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }
        
        CString strSQLCreate(_T(""));
        strSQLCreate.Append(_T( " CREATE TABLE [assign_path_input] (          "));
        strSQLCreate.Append(_T( "     [mode_id] INTEGER NOT NULL PRIMARY KEY, "));
        strSQLCreate.Append(_T( "     [occupancy] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [pcu] DOUBLE,                "));
        strSQLCreate.Append(_T( "     [network_type] INTEGER,      "));
        strSQLCreate.Append(_T( "     [preload] INTEGER,           "));
        strSQLCreate.Append(_T( "     [Column_name] VARCHAR2,      "));
        strSQLCreate.Append(_T( "     [InputColumnName] VARCHAR2)  "));

        spDBaseConnection->ExecuteUpdate(strSQLCreate);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 5;
    }    
}

void KDBaseVersion_P001::TableAssignPathModel( KDBaseConPtr spDBaseConnection )
{
    try
    {
        CString strSQLDelete(_T(" Delete From assign_path_model "));
        spDBaseConnection->ExecuteUpdate(strSQLDelete);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KDBaseVersion_P001::TableBusPathInput( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE bus_path_input "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));
        strSQLCreate.Append(_T( " CREATE TABLE [bus_path_input] ( "));
        strSQLCreate.Append(_T( "     [Max_Routes] INTEGER,       "));
        strSQLCreate.Append(_T( "     [Time_Board] INTEGER,       "));
        strSQLCreate.Append(_T( "     [Wait_Vdf_Alpha] DOUBLE,    "));
        strSQLCreate.Append(_T( "     [Wait_Vdf_Beta] DOUBLE,     "));
        strSQLCreate.Append(_T( "     [UseBusSubway] INTEGER,     "));
        strSQLCreate.Append(_T( "     [UseBus] INTEGER,           "));
        strSQLCreate.Append(_T( "     [UseSubway] INTEGER,        "));
        strSQLCreate.Append(_T( "     [DwellTime] DOUBLE ,        "));
        strSQLCreate.Append(_T( "     [Path_Time] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [Invehicle_time] INTEGER,   "));
        strSQLCreate.Append(_T( "     [waiting_time] INTEGER,     "));
        strSQLCreate.Append(_T( "     [transfer_time] INTEGER,    "));
        strSQLCreate.Append(_T( "     [ae_time] INTEGER,          "));
        strSQLCreate.Append(_T( "     [path_length] DOUBLE)       "));	

        spDBaseConnection->ExecuteUpdate(strSQLCreate);

    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 6;
    }    
}

void KDBaseVersion_P001::TableParameterOD( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE Parameter_OD "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));
        strSQLCreate.Append(_T( " CREATE TABLE [Parameter_OD] (     "));	
        strSQLCreate.Append(_T( "     [OZone_ID] INTEGER NOT NULL,  "));	
        strSQLCreate.Append(_T( "     [DZone_ID] INTEGER NOT NULL,  "));	
        strSQLCreate.Append(_T( "     [auto_invehtime] DOUBLE,      "));
        strSQLCreate.Append(_T( "     [auto_cost] DOUBLE,           "));
        strSQLCreate.Append(_T( "     [auto_length] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [taxi_invehtime] DOUBLE,      "));
        strSQLCreate.Append(_T( "     [taxi_cost] DOUBLE,           "));
        strSQLCreate.Append(_T( "     [taxi_length] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [truck_invehtime] DOUBLE,     "));
        strSQLCreate.Append(_T( "     [truck_cost] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [truck_length] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [bus_invehtime] DOUBLE,       "));
        strSQLCreate.Append(_T( "     [bus_waittime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [bus_transtime] DOUBLE,       "));
        strSQLCreate.Append(_T( "     [bus_acctime] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [bus_egrtime] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [bus_cost] DOUBLE,            "));
        strSQLCreate.Append(_T( "     [bus_length] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [rail_invehtime] DOUBLE,      "));
        strSQLCreate.Append(_T( "     [rail_waittime] DOUBLE,       "));
        strSQLCreate.Append(_T( "     [rail_transtime] DOUBLE,      "));
        strSQLCreate.Append(_T( "     [rail_acctime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [rail_egrtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [rail_cost] DOUBLE,           "));
        strSQLCreate.Append(_T( "     [rail_length] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [br_invehtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [br_waittime] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [br_transtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [br_acctime] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [br_egrtime] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [br_cost] DOUBLE,             "));
        strSQLCreate.Append(_T( "     [br_length] DOUBLE,           "));
        strSQLCreate.Append(_T( "     [pr_invehtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [pr_waittime] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [pr_transtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [pr_acctime] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [pr_egrtime] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [pr_cost] DOUBLE,             "));
        strSQLCreate.Append(_T( "     [pr_length] DOUBLE,           "));
        strSQLCreate.Append(_T( "     [kr_invehtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [kr_waittime] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [kr_transtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [kr_acctime] douBLE,          "));
        strSQLCreate.Append(_T( "     [kr_egrtime] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [kr_cost] DOUBLE,             "));
        strSQLCreate.Append(_T( "     [kr_length] DOUBLE,           "));
        strSQLCreate.Append(_T( "     [tt_invehtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [tt_waittime] DOUBLE,         "));
        strSQLCreate.Append(_T( "     [tt_transtime] DOUBLE,        "));
        strSQLCreate.Append(_T( "     [tt_acctime] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [tt_egrtime] DOUBLE,          "));
        strSQLCreate.Append(_T( "     [tt_cost] DOUBLE,             "));
        strSQLCreate.Append(_T( "     [tt_length] DOUBLE,           "));
        strSQLCreate.Append(_T( "     CONSTRAINT [sqlite_autoindex_Param eter_OD_1] PRIMARY KEY ([OZone_ID], [DZone_ID])) "));

        spDBaseConnection->ExecuteUpdate(strSQLCreate);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 7;
    }    
}

void KDBaseVersion_P001::TableTransitCost( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE transit_cost "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));
        strSQLCreate.Append(_T( " CREATE TABLE transit_cost   "));	
        strSQLCreate.Append(_T( "     (type INTEGER PRIMARY KEY  NOT NULL, "));	 
        strSQLCreate.Append(_T( "     basic_rate DOUBLE,     "));	
        strSQLCreate.Append(_T( "     basic_distance DOUBLE, "));	
        strSQLCreate.Append(_T( "     add_rate DOUBLE,       "));	
        strSQLCreate.Append(_T( "     add_distance DOUBLE,   "));	
        strSQLCreate.Append(_T( "     merge_rete INTEGER)    "));	

        spDBaseConnection->ExecuteUpdate(strSQLCreate);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 8;
    }    
}

void KDBaseVersion_P001::TasbleMaster( KDBaseConPtr spDBaseConnection )
{
    try
    {
    	CString strSQLDelete(_T(" Delete From Table_Master Where Table_Name = 'access_line_station' "));
        spDBaseConnection->ExecuteUpdate(strSQLDelete);

        CString strSQLInsert(_T(" INSERT OR REPLACE INTO Table_Master(Table_name, Display_Name, Table_Info, Create_Date, Modify_Date, Table_Type, Display_Type, Value_Type, Display_Order) VALUES ('access_stop_node','Access Stop Node','Access Stop Node 정보','','','-1','0','','8') "));
        spDBaseConnection->ExecuteUpdate(strSQLInsert);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 9;
    }    
}

void KDBaseVersion_P001::ColumnDescription( KDBaseConPtr spDBaseConnection )
{
    try
    {
        CString strSQLDelete(_T(" Delete From Column_Description Where Table_Name = 'parameter_od' "));
        spDBaseConnection->ExecuteUpdate(strSQLDelete);

    	CString strSQLInsert(_T(""));

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('access_stop_node','zone_id','1','-1','-1','Y','Y','Zone_ID','존ID','1','','Y','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('access_stop_node','node_id','1','-1','-1','Y','Y','Node_ID','노드ID','2','','Y','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('access_stop_node','mode','1','-1','0','Y','Y','Mode','Mode 타입','3','','Y','113','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('access_stop_node','class_in_zone','3','-1','-1','Y','Y','Class_In_Zone','Zone Class','5','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('access_stop_node','class_in_network','3','-1','-1','Y','Y','Class_In_Netwok','Network Class','6','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('access_stop_node','no_of_line','1','-1','-1','Y','Y','No.of_Line','라인의 갯수','7','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('access_stop_node','zone_name','3','-1','-1','Y','Y','Zone_Name','존명칭','4','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        // Parameter OD
        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','ozone_id','1','-1','-1','Y','Y','Origin','Origin','1','','Y','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','dzone_id','1','-1','-1','Y','Y','Destination','Destination','2','','Y','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','auto_invehtime','2','-1','-1','Y','Y','Auto_InVehTime','Auto_InVehTime','3','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','auto_cost','2','-1','-2','Y','Y','Auto_Cost','Auto_Cost','4','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','auto_length','2','-1','-3','Y','Y','Auto_Length','Auto_Length','5','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','truck_invehtime','2','-1','-4','Y','Y','Truck_InVehTime','Truck_InVehTime','9','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','truck_cost','2','-1','-5','Y','Y','Truck_Cost','Truck_Cost','10','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','bus_invehtime','2','-1','-6','Y','Y','Bus_InVehTime','Bus_InVehTime','12','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','bus_waittime','2','-1','-7','Y','Y','Bus_WaitTime','Bus_WaitTime','13','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','bus_transtime','2','-1','-8','Y','Y','Bus_TransTime','Bus_TransTime','14','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','bus_acctime','2','-1','-9','Y','Y','Bus_AccTime','Bus_AccTime','15','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','bus_egrtime','2','-1','-10','Y','Y','Bus_EgrTime','Bus_EgrTime','16','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','bus_cost','2','-1','-11','Y','Y','Bus_Cost','Bus_Cost','17','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','bus_length','2','-1','-12','Y','Y','Bus_Length','Bus_Length','18','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','rail_invehtime','2','-1','-13','Y','Y','Rail_InVehTime','Rail_InVehTime','19','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','rail_waittime','2','-1','-14','Y','Y','Rail_WaitTime','Rail_WaitTime','20','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','rail_acctime','2','-1','-15','Y','Y','Rail_AccTime','Rail_AccTime','22','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','rail_egrtime','2','-1','-16','Y','Y','Rail_EgrTime','Rail_EgrTime','23','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','rail_cost','2','-1','-17','Y','Y','Rail_Cost','Rail_Cost','24','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','rail_length','2','-1','-18','Y','Y','Rail_Lengh','Rail_Lengh','25','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','br_invehtime','2','-1','-19','Y','Y','BusRail_InVehTime','BusRail_InVehTime','26','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','br_waittime','2','-1','-20','Y','Y','BusRail_WaitTime','BusRail_WaitTime','27','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','br_transtime','2','-1','-21','Y','Y','BusRail_TransTime','BusRail_TransTime','28','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','br_acctime','2','-1','-22','Y','Y','BusRail_AccTime','BusRail_AccTime','29','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','br_egrtime','2','-1','-23','Y','Y','BusRail_EgrTime','BusRail_EgrTime','30','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','br_cost','2','-1','-24','Y','Y','BusRail_Cost','BusRail_Cost','31','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','br_length','2','-1','-25','Y','Y','BusRail_Length','BusRail_Length','32','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','pr_invehtime','2','-1','-26','Y','Y','ParkRide_InVehTime','ParkRide_InVehTime','33','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','pr_waittime','2','-1','-27','Y','Y','ParkRide_WaitTime','ParkRide_WaitTime','34','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','pr_transtime','2','-1','-28','Y','Y','ParkRide_TransTime','ParkRide_TransTime','35','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','pr_acctime','2','-1','-29','Y','Y','ParkRide_AccTime','ParkRide_AccTime','36','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','pr_egrtime','2','-1','-30','Y','Y','ParkRide_EgrTime','ParkRide_EgrTime','37','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','pr_cost','2','-1','-31','Y','Y','ParkRide_Cost','ParkRide_Cost','38','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','pr_length','2','-1','-32','Y','Y','ParkRide_Length','ParkRide_Length','39','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','kr_invehtime','2','-1','-33','Y','Y','KissRide_InVehTime','KissRide_InVehTime','40','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','kr_waittime','2','-1','-34','Y','Y','KissRide_WaitTime','KissRide_WaitTime','41','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','kr_transtime','2','-1','-35','Y','Y','KissRide_TransTime','KissRide_TransTime','42','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','kr_acctime','2','-1','-36','Y','Y','KissRide_AccTime','KissRide_AccTime','43','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','kr_egrtime','2','-1','-37','Y','Y','KissRide_EgrTime','KissRide_EgrTime','44','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','kr_cost','2','-1','-38','Y','Y','KissRide_Cost','KissRide_Cost','45','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','kr_length','2','-1','-39','Y','Y','KissRide_Length','KissRide_Length','46','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','tt_invehtime','2','-1','-40','Y','Y','TaxiTransit_InVehTime','TaxiTransit_InVehTime','47','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','tt_waittime','2','-1','-41','Y','Y','TaxiTransit_WaitTime','TaxiTransit_WaitTime','48','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','tt_transtime','2','-1','-42','Y','Y','TaxiTransit_TransTime','TaxiTransit_TransTime','49','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','tt_acctime','2','-1','-43','Y','Y','TaxiTransit_AccTime','TaxiTransit_AccTime','50','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','tt_egrtime','2','-1','-44','Y','Y','TaxiTransit_EgrTime','TaxiTransit_EgrTime','51','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','tt_cost','2','-1','-45','Y','Y','TaxiTransit_Cost','TaxiTransit_Cost','52','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','tt_length','2','-1','-46','Y','Y','TaxiTransit_Length','TaxiTransit_Length','53','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','rail_transtime','2','-1','-47','Y','Y','Rail_TransTime','Rail_TransTime','21','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','truck_length','2','-1','-48','Y','Y','Truck_Length','Truck_Length','11','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','taxi_invehtime','2','-1','-49','Y','Y','Taxi_InVehTime','Taxi_InVehTime','6','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','taxi_cost','2','-1','-50','Y','Y','Taxi_Cost','Taxi_Cost','7','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT OR REPLACE INTO Column_Description(Table_name, Column_Name, Data_Type, Column_Type, Code_Type, Display_YN, Freeze_YN, Display_Name, Column_Info, Display_Order, Base_ID, PK_YN, KCode_Group_Key, Use_Flag) VALUES ('parameter_od','taxi_length','2','-1','-51','Y','Y','Taxi_Length','Taxi_Length','8','','N','','1') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 10;
    }    
}

void KDBaseVersion_P001::KCodeGroup( KDBaseConPtr spDBaseConnection )
{
    try
    {
    	
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 11;
    }    
}

void KDBaseVersion_P001::KCodeDefine( KDBaseConPtr spDBaseConnection )
{
    try
    {
    	
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 12;
    }    
}

