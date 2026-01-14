#include "stdafx.h"
#include "DefineNames.h"

/*Define Talbe*/
const TCHAR* TABLE_SQLITE_MASTER = _T("sqlite_master");

/* Define table names */
const TCHAR* TABLE_NODE = _T("node");
const TCHAR* TABLE_LINK = _T("link");
const TCHAR* TABLE_ZONE = _T("zone");
const TCHAR* TABLE_TURN = _T("turn");
const TCHAR* TABLE_PURPOSE_OD	 = _T("purpose_od");
const TCHAR* TABLE_PARAMETER_OD  = _T("parameter_od");
const TCHAR* TABLE_MODE_OD		 = _T("mode_od");
const TCHAR* TABLE_TRANSIT		 = _T("transit");
const TCHAR* TABLE_TRANSITLINKS  = _T("transit_links");
const TCHAR* TABLE_TRANSITNODES  = _T("transit_nodes");
const TCHAR* TABLE_KCODE_DEFINE	 = _T("kcode_define");
const TCHAR* TABLE_KCODE_GROUP	 = _T("kcode_group");
const TCHAR* TABLE_INTERSECTION                    = _T("intersection");
const TCHAR* TABLE_INTERSECTION_EXTENSION          = _T("intersection_extension");
const TCHAR* TABLE_INTERSECTION_INFO               = _T("intersection_info");
const TCHAR* TABLE_INTERSECTION_SIGNAL_INDICATION  = _T("intersection_signal_indication");
const TCHAR* TABLE_OBJECT_GROUP	           = _T("object_group");
const TCHAR* TABLE_DETAIL_OBJECT           = _T("detail_object");
const TCHAR* TABLE_MODE_GROUP	           = _T("mode_group");
const TCHAR* TABLE_DETAIL_MODE	           = _T("detail_mode");
const TCHAR* TABLE_INPUT_COEFFICIENT       = _T("Input_Coefficient"     );
const TCHAR* TABLE_INPUT_COEFFICIENT_MODEL = _T("InputCoefficient_Model");
const TCHAR* TABLE_SCREEN_CORDON           = _T("Screen_Cordon"         );
const TCHAR* TABLE_SED_VECTOR              = _T("Sed_Vector"            );
const TCHAR* TABLE_SED_POP                 = _T("Sed_Pop"               );
const TCHAR* TABLE_AVERAGE_LOADAGE_OD      = _T("Average_loadage_OD"    );
const TCHAR* TABLE_GOODS_TRANSPORTED_OD    = _T("Goods_Transported_OD"  );
const TCHAR* TABLE_VEHICLE_MODE_OD         = _T("Vehicle_Mode_OD"       );
const TCHAR* TABLE_EMPTY_VEHICLE_OD        = _T("Empty_vehicle_OD"      );
const TCHAR* TABLE_MATCHING_VEHICLECONV    = _T("Matching_VehicleConv"  );
const TCHAR* TABLE_CHAIN_OD                = _T("chain_od");
const TCHAR* TABLE_INPUT_RATIO_OD          = _T("Input_Ratio_OD");
const TCHAR* TABLE_RATIO_REGION            = _T("Ratio_Region");
const TCHAR* TABLE_DISTRIBUTION_MODEL      = _T("Distribution_Model");
const TCHAR* TABLE_GENERATION_MODEL        = _T("Generation_Model");
const TCHAR* TABLE_MODECHOICE_MODEL        = _T("modechoice_Model");
const TCHAR* TABLE_NETWORKOFF              = _T("Network_off");
const TCHAR* TABLE_ACCESS_LINE_STATION     = _T("access_line_station");
const TCHAR* TABLE_ACCESS_STOP_NODE		   = _T("access_stop_node");
const TCHAR* TABLE_ACCESS_STOP_NODE_INFO   = _T("access_stop_node_info");
const TCHAR* TABLE_ACCESS_CLASS            = _T("access_class");
const TCHAR* TABLE_BUS_PATH_COEFFICIENT    = _T("bus_path_coefficient");
const TCHAR* TABLE_BUS_PATH_COST           = _T("bus_path_cost");
const TCHAR* TABLE_BUS_PATH_INPUT          = _T("bus_path_input");
const TCHAR* TABLE_ASSIGN_MODEL            = _T("assign_model");
const TCHAR* TABLE_ASSIGN_PATH_INPUT       = _T("assign_path_input");
const TCHAR* TABLE_ASSIGN_PATH_MODEL       = _T("assign_path_model");
const TCHAR* TABLE_ASSIGN_TURN_VOLUME_INPUT= _T("assign_turn_volume_input");
const TCHAR* TABLE_DEFINE_SYSTEM_CODE	   = _T("define_system_code");
const TCHAR* TABLE_BUS_PATH_MATRIX		   = _T("bus_path_matrix");
const TCHAR* TABLE_TRANSIT_COST			   = _T("transit_cost");
const TCHAR* TABLE_PARAMTER_OD_INFO		   = _T("parameter_od_info");
const TCHAR* TABLE_PARAMTER_INFO_TYPE	   = _T("parameter_info_type");
const TCHAR* TABLE_CHAIN_MODEL             = _T("Chain_Model");
const TCHAR* TABLE_VEHICLECONV_MODEL       = _T("vehicleconv_model");
const TCHAR* TABLE_REGIONAL_MODE_DEFINE	   = _T("define_regional_mode");
const TCHAR* TABLE_ACCESS_TERMINAL_INFO	   = _T("access_terminal_generator_info");
const TCHAR* TABLE_ACCESS_TERMINAL_NODE	   = _T("access_terminal_node");
const TCHAR* TABLE_TERMINAL_SCHEDULE	   = _T("terminal_schedule");
const TCHAR* TABLE_INTER_PURPOSE_OD_INFO   = _T("inter_purpose_od_info");
const TCHAR* TABLE_REGION_GLOBAL_TERMINAL  = _T("region_global_terminal");
const TCHAR* TABLE_INTER_URBAN_PATH_INFO   = _T("inter_urban_path_info");
const TCHAR* TABLE_INTER_REGION_PATH_INFO  = _T("inter_region_path_info");
const TCHAR* TABLE_INTER_AUTO_COST_INFO    = _T("inter_auto_cost_info");
const TCHAR* TABLE_INTER_TAXI_COST_INFO    = _T("inter_taxi_cost_info");
const TCHAR* TABLE_INTER_TIME_INFO         = _T("inter_time_info");
const TCHAR* TABLE_INTER_TERMINAL_TIME_INFO = _T("inter_terminal_time_info");
const TCHAR* TABLE_INTER_TRANSIT_COST		= _T("inter_transit_cost");
const TCHAR* TABLE_F_TERMINAL_PATH_INFO     = _T("ttablepathinfo");
const TCHAR* TABLE_T_TERMINAL_PATHCOLUMN_INFO = _T("TTablePathColumnInfo");
const TCHAR* TABLE_NETWORKEDIT_LOG			= _T("networkeditlog");
const TCHAR* TABLE_CATEGORY_CLASS			= _T("category_class");
const TCHAR* TABLE_CATEGORY_AGE_GROUP		= _T("category_age_group");
const TCHAR* TABLE_CATEGORY_CLASS_VALUE		= _T("category_class_value");
const TCHAR* TABLE_SUBWAY_TRANSFER_INFO         = _T("subway_transfer_info");
const TCHAR* TABLE_SUBWAY_TRANSFER_SECTION      = _T("subway_transfer_section") ;
const TCHAR* TABLE_SUBWAY_TRANSFER_SECTION_LINE = _T("subway_transfer_section_line");
const TCHAR* TABLE_IMTASVERSION				= _T("imtasversion");
const TCHAR* TABLE_FMODE_MODEL				= _T("fmode_model");
const TCHAR* TABLE_DETAIL_FMODE				= _T("detail_fmode");
const TCHAR* TABLE_ITEMLOGIT_MODEL			= _T("itemlogit_model");
const TCHAR* TABLE_TERMINAL_RESULT			= _T("terminal_result");
const TCHAR* TABLE_ASSIGN_INTER_MODE_INFO	= _T("assign_inter_mode_info");
const TCHAR* TABLE_DISPLAY_OPTION			= _T("display_option");

const TCHAR* TABLE_TXSHP_LINK = _T("TxShp_Link");
const TCHAR* TABLE_TXIDX_LINK = _T("TxIDX_Link");
const TCHAR* TABLE_TXSHP_NODE = _T("TxShp_Node");
const TCHAR* TABLE_TXIDX_NODE = _T("TxIDX_Node");
const TCHAR* TABLE_TXSHP_ZONE = _T("TxShp_Zone");
const TCHAR* TABLE_TXIDX_ZONE = _T("TxIDX_Zone");

/* Define column names */
const TCHAR* COLUMN_LINK_LANE   = _T("lane");
const TCHAR* COLUMN_LINK_FN_ID  = _T("function_id");
const TCHAR* COLUMN_LINK_LENGTH = _T("length");
const TCHAR* COLUMN_LINK_CAPA   = _T("capa");
const TCHAR* COLUMN_LINK_SPEED  = _T("speed");
const TCHAR* COLUMN_LINK_SURVEYVOLUME = _T("survey_volume");
const TCHAR* COLUMN_LINK_ALPHA	= _T("alpha");
const TCHAR* COLUMN_LINK_BETA	= _T("beta");
const TCHAR* COLUMN_LINK_A		= _T("a");
const TCHAR* COLUMN_LINK_B		= _T("b");
const TCHAR* COLUMN_ZONE_ID   = _T("zone_id");
const TCHAR* COLUMN_LINK_ID   = _T("link_id");
const TCHAR* COLUMN_FNODE_ID  = _T("fnode_id");
const TCHAR* COLUMN_TNODE_ID  = _T("tnode_id");
const TCHAR* COLUMN_LINK_TYPE = _T("link_type");

const TCHAR* COLUMN_NODE_ID   = _T("node_id");
const TCHAR* COLUMN_NODE_TYPE = _T("node_type");
const TCHAR* COLUMN_NODE_X	  = _T("x");
const TCHAR* COLUMN_NODE_Y	  = _T("y");
const TCHAR* COLUMN_NODE_FACILITY_TYPE	= _T("facility_type");
const TCHAR* COLUMN_NODE_FACILITY_FEE	= _T("facility_fee");
const TCHAR* COLUMN_NODE_LOADING_FEE	= _T("loading_fee");
const TCHAR* COLUMN_NODE_UNLOADING_FEE	= _T("unloading_fee");
const TCHAR* COLUMN_NODE_AVAILABLE_ITEM	= _T("available_item");
const TCHAR* COLUMN_NODE_TRAFFIC_MEAN	= _T("traffic_mean");
const TCHAR* COLUMN_NODE_OCCURRENCE_VIA	= _T("occurrence_via");
const TCHAR* COLUMN_NODE_CAPA			= _T("capa");


const TCHAR* COLUMN_OZONE     = _T("ozone_id");
const TCHAR* COLUMN_DZONE     = _T("dzone_id");
const TCHAR* COLUMN_PZONE_ID  = _T("pzone_id");
const TCHAR* COLUMN_CZONE_ID  = _T("czone_id");
const TCHAR* COLUMN_NODE_NAME = _T("name");
const TCHAR* COLUMN_LINK_NAME = _T("name");
const TCHAR* COLUMN_ZONE_NAME = _T("name");
const TCHAR* COLUMN_ZONE_FULL_NAME     = _T("full_name");
const TCHAR* COLUMN_ZONE_MAIN_CATEGORY = _T("main_category");

const TCHAR* COLUMN_TURN_ID   = _T("turn_id");
const TCHAR* COLUMN_TURN_F_LINK = _T("inlink_id");
const TCHAR* COLUMN_TURN_T_LINK = _T("outlink_id");
const TCHAR* COLUMN_TURN_NODE	= _T("node_id");

const TCHAR* COLUMN_TURN_TYPE = _T("turn_type");

const TCHAR* COLUMN_TRANSIT_ID            = _T("transit_id");
const TCHAR* COLUMN_TRANSIT_TYPE          = _T("type");
const TCHAR* COLUMN_TRANSIT_DETAIL_TYPE   = _T("detail_type");
const TCHAR* COLUMN_TRANSIT_NAME          = _T("name");
const TCHAR* COLUMN_TRANSIT_PEAK_HEADWAY       = _T("peak_headway");
const TCHAR* COLUMN_TRANSIT_OFFPEAK_HEADWAY    = _T("offpeak_headway");
const TCHAR* COLUMN_TRANSIT_AVG_HEADWAY        = _T("avg_headway");
const TCHAR* COLUMN_TRANSIT_PEAK_FREQUENCY     = _T("peak_frequency");
const TCHAR* COLUMN_TRANSIT_OFFPEAK_FREQUENCY  = _T("offpeak_frequency");
const TCHAR* COLUMN_TRANSIT_TOTAL_FREQUENCY    = _T("total_frequency");
const TCHAR* COLUMN_TRANSIT_COMMERCIAL_SPEED   = _T("commercial_speed");
const TCHAR* COLUMN_TRANSIT_SEAT_COUNT         = _T("seat_count");
const TCHAR* COLUMN_TRANSIT_CAPACITY           = _T("capacity");

const TCHAR* COLUMN_TRANSIT_LINK_ID         = _T("link_id");
const TCHAR* COLUMN_TRANSIT_LINK_SEQ        = _T("seq");
const TCHAR* COLUMN_TRANSIT_LINK_STATION_YN = _T("station_yn");

const TCHAR* COLUMN_TRANSIT_NODE_ID         = _T("node_id");
const TCHAR* COLUMN_TRANSIT_NODE_SEQ        = _T("seq");
const TCHAR* COLUMN_TRANSIT_NODE_STATION_YN = _T("station_yn");

const TCHAR* COLUMN_OBJECT_GROUP_CODE = _T("object_group_code");
const TCHAR* COLUMN_OBJECT_GROUP_NAME = _T("object_group_name");
const TCHAR* COLUMN_DETAIL_OBJECT_ID  = _T("detail_object_id");
const TCHAR* COLUMN_DETAIL_OBJECT_NAME= _T("object_name");

const TCHAR* COLUMN_MODE_GROUP_CODE = _T("mode_group_code");
const TCHAR* COLUMN_MODE_GROUP_NAME = _T("mode_group_name");
const TCHAR* COLUMN_DETAIL_MODE_ID  = _T("detail_mode_id");
const TCHAR* COLUMN_DETAIL_MODE_NAME= _T("mode_name");

const TCHAR* COLUMN_ZONE_COLUMN_NAME       = _T("Zone_Column_Name");
const TCHAR* COLUMN_MODE_OD_COLUMN_NAME    = _T("mode_od_column_name");
const TCHAR* COLUMN_PURPOSE_OD_COLUMN_NAME = _T("Purpose_OD_Column_Name");

const TCHAR* COLUMN_PARAMETER_OD_DIRECT_DIST				= _T("direct_dist");
const TCHAR* COLUMN_PARAMETER_OD_DIRECT_TIME				= _T("direct_time");
const TCHAR* COLUMN_PARAMETER_OD_ONODE_ID					= _T("onode_id");
const TCHAR* COLUMN_PARAMETER_OD_DNODE_ID					= _T("dnode_id");
const TCHAR* COLUMN_PARAMETER_OD_STATION1_NODE_ID			= _T("station1_node_id");
const TCHAR* COLUMN_PARAMETER_OD_STATION2_NODE_ID			= _T("station2_node_id");
const TCHAR* COLUMN_PARAMETER_OD_STATION1_ZONE_ID			= _T("station1_zone_id");
const TCHAR* COLUMN_PARAMETER_OD_STATION2_ZONE_ID			= _T("station2_zone_id");
const TCHAR* COLUMN_PARAMETER_OD_ORIGIN_STATION1_DIST		= _T("origin_station1_dist");
const TCHAR* COLUMN_PARAMETER_OD_STATION1_STATION2_DIST		= _T("station1_station2_dist");
const TCHAR* COLUMN_PARAMETER_OD_STATION2_DESTINATION_DIST	= _T("station2_destination_dist");
const TCHAR* COLUMN_PARAMETER_OD_ROAD_VIA_NODE				= _T("road_via_node");
const TCHAR* COLUMN_PARAMETER_OD_ROAD_VIA_ZONE				= _T("road_via_zone");
const TCHAR* COLUMN_PARAMETER_OD_ORGIN_VIA_DIST				= _T("origin_via_dist");
const TCHAR* COLUMN_PARAMETER_OD_VIA_DESTINATION_DIST		= _T("via_destination_dist");

const TCHAR* COLUMN_INTERSECTION_FIRST_BOUND_NODE_ID  =_T("first_bound_node_id");
const TCHAR* COLUMN_INTERSECTION_SECOND_BOUND_NODE_ID =_T("second_bound_node_id");
const TCHAR* COLUMN_INTERSECTION_THIRD_BOUND_NODE_ID  =_T("third_bound_node_id");
const TCHAR* COLUMN_INTERSECTION_FOURTH_BOUND_NODE_ID =_T("fourth_bound_node_id");
const TCHAR* COLUMN_INTERSECTION_CROSS_TYPE           =_T("cross_type");

const TCHAR* COLUMN_ASSIGN_AVERAGE_TIME         = _T("average_time");
const TCHAR* COLUMN_ASSIGN_VC                   = _T("vc");
const TCHAR* COLUMN_ASSIGN_SUM_VOLUME           = _T("sum_volume");
const TCHAR* COLUMN_ASSIGN_TRIP                 = _T("trip");
const TCHAR* COLUMN_ASSIGN_MODE_VOLUME          = _T("mode_volume");
const TCHAR* COLUMN_ASSIGN_VDF_TIME             = _T("vdf_time");
const TCHAR* COLUMN_ASSIGN_AVE_TIME_TOLL        = _T("ave_time_toll");
const TCHAR* COLUMN_ASSIGN_VDF_TOLL_TIME        = _T("vdf_toll_time");
const TCHAR* COLUMN_ASSIGN_BUS_INITIAL_VOLUME   = _T("bus_Initial_volume");
const TCHAR* COLUMN_ASSIGN_OD_AVERAGE_TIME      = _T("od_average_time");
const TCHAR* COLUMN_ASSIGN_OD_VDF_TIME          = _T("od_vdf_time");
const TCHAR* COLUMN_ASSIGN_OD_AVERAGE_BUS       = _T("od_average_bus");
const TCHAR* COLUMN_ASSIGN_OD_AVERAGE_SUBWAY    = _T("od_average_subway");
const TCHAR* COLUMN_ASSIGN_OD_AVERAGE_BUSSUBWAY = _T("od_average_bussubway");
const TCHAR* COLUMN_ASSIGN_OD_BUS_DIRECT        = _T("od_bus_direct");
const TCHAR* COLUMN_ASSIGN_OD_BUS_ONE_TRANSFER  = _T("od_bus_one_transfer");
const TCHAR* COLUMN_ASSIGN_OD_BUS_TWO_TRANSFER  = _T("od_bus_two_transfer");
const TCHAR* COLUMN_ASSIGN_LINFO_NUM_PASSENGERS = _T("linfo_num_passengers");
const TCHAR* COLUMN_ASSIGN_OD_BUS               = _T("od_bus");
const TCHAR* COLUMN_ASSIGN_OD_RAIL              = _T("od_rail");
const TCHAR* COLUMN_ASSIGN_OD_BUSRAIL           = _T("od_busrail");
const TCHAR* COLUMN_ASSIGN_LINE_BOARD_NET       = _T("line_board_net");
const TCHAR* COLUMN_ASSIGN_LINE_BOARD_TRANSFER  = _T("line_board_Transfer");
const TCHAR* COLUMN_ASSIGN_LINE_BOARD_TOTAL     = _T("line_board_Total");
const TCHAR* COLUMN_ASSIGN_LINE_ALIGHT_NET      = _T("line_alight_net");
const TCHAR* COLUMN_ASSIGN_LINE_ALIGHT_TRANSFER = _T("line_alight_Transfer");
const TCHAR* COLUMN_ASSIGN_LINE_ALIGHT_TOTAL    = _T("line_alight_Total");
const TCHAR* COLUMN_ASSIGN_LINE_PASSENGER       = _T("line_passenger");
const TCHAR* COLUMN_ASSIGN_NODE_BUS_NET_BOARD   = _T("node_bus_net_board");
const TCHAR* COLUMN_ASSIGN_NODE_BUS_TRANSFER_BOARD = _T("node_bus_transfer_board");
const TCHAR* COLUMN_ASSIGN_NODE_BUS_SUM_BOARD   = _T("node_bus_sum_board");
const TCHAR* COLUMN_ASSIGN_NODE_BUS_NET_ALIGHT  = _T("node_bus_net_alight");
const TCHAR* COLUMN_ASSIGN_NODE_BUS_TRANSFER_ALIGHT = _T("node_bus_transfer_alight");
const TCHAR* COLUMN_ASSIGN_NODE_BUS_SUM_ALIGHT  = _T("node_bus_sum_alight");

//여객 parameter column;
const TCHAR* COLUMN_PARA_AUTO_INVEHTIME		= _T("auto_invehtime");
const TCHAR* COLUMN_PARA_AUTO_COST			= _T("auto_cost");
const TCHAR* COLUMN_PARA_AUTO_LENGTH		= _T("auto_length");

const TCHAR* COLUMN_PARA_TAXI_INVEHTIME		= _T("taxi_invehtime");
const TCHAR* COLUMN_PARA_TAXI_COST			= _T("taxi_cost");
const TCHAR* COLUMN_PARA_TAXI_LENGTH		= _T("taxi_length");

const TCHAR* COLUMN_PARA_TRUCK_INVEHTIME	= _T("truck_invehtime");
const TCHAR* COLUMN_PARA_TRUCK_COST			= _T("truck_cost");
const TCHAR* COLUMN_PARA_TRUCK_LENGTH		= _T("truck_length");

const TCHAR* COLUMN_PARA_BUS_INVEHTIME		= _T("bus_invehtime");
const TCHAR* COLUMN_PARA_BUS_WAITTIME		= _T("bus_waittime");
const TCHAR* COLUMN_PARA_BUS_TRANSTIME		= _T("bus_transtime");
const TCHAR* COLUMN_PARA_BUS_ACCTIME		= _T("bus_acctime");
const TCHAR* COLUMN_PARA_BUS_ERGTIME		= _T("bus_egrtime");
const TCHAR* COLUMN_PARA_BUS_COST			= _T("bus_cost");
const TCHAR* COLUMN_PARA_BUS_LENGTH			= _T("bus_length");
const TCHAR* COLUMN_PARA_BUS_NOTRANSFER		= _T("bus_notrans");
const TCHAR* COLUMN_PARA_BUS_NOOFSTOP		= _T("bus_nostop");

const TCHAR* COLUMN_PARA_RAIL_INVEHTIME		= _T("rail_invehtime");
const TCHAR* COLUMN_PARA_RAIL_WAITTIME		= _T("rail_waittime");
const TCHAR* COLUMN_PARA_RAIL_TRANSTIME		= _T("rail_transtime");
const TCHAR* COLUMN_PARA_RAIL_ACCTIME		= _T("rail_acctime");
const TCHAR* COLUMN_PARA_RAIL_ERGTIME		= _T("rail_egrtime");
const TCHAR* COLUMN_PARA_RAIL_COST			= _T("rail_cost");
const TCHAR* COLUMN_PARA_RAIL_LENGTH		= _T("rail_length");
const TCHAR* COLUMN_PARA_RAIL_NOTRANSFER	= _T("rail_notrans");

const TCHAR* COLUMN_PARA_BUS_RAIL_INVEHTIME		= _T("br_invehtime");
const TCHAR* COLUMN_PARA_BUS_RAIL_WAITTIME		= _T("br_waittime");
const TCHAR* COLUMN_PARA_BUS_RAIL_TRANSTIME		= _T("br_transtime");
const TCHAR* COLUMN_PARA_BUS_RAIL_ACCTIME		= _T("br_acctime");
const TCHAR* COLUMN_PARA_BUS_RAIL_ERGTIME		= _T("br_egrtime");
const TCHAR* COLUMN_PARA_BUS_RAIL_COST			= _T("br_cost");
const TCHAR* COLUMN_PARA_BUS_RAIL_LENGTH		= _T("br_length");
const TCHAR* COLUMN_PARA_BUS_RAIL_NOTRANSFER	= _T("br_notrans");
const TCHAR* COLUMN_PARA_BUS_RAIL_NOOFSTOP		= _T("br_nostop");

const TCHAR* COLUMN_PARA_PR_INVEHTIME		= _T("pr_invehtime");
const TCHAR* COLUMN_PARA_PR_WAITTIME		= _T("pr_waittime");
const TCHAR* COLUMN_PARA_PR_TRANSTIME		= _T("pr_transtime");
const TCHAR* COLUMN_PARA_PR_ACCTIME			= _T("pr_acctime");
const TCHAR* COLUMN_PARA_PR_ERGTIME			= _T("pr_egrtime");
const TCHAR* COLUMN_PARA_PR_COST			= _T("pr_cost");
const TCHAR* COLUMN_PARA_PR_LENGTH			= _T("pr_length");
const TCHAR* COLUMN_PARA_PR_NOTRANSFER		= _T("pr_notrans");

const TCHAR* COLUMN_PARA_KR_INVEHTIME		= _T("kr_invehtime");
const TCHAR* COLUMN_PARA_KR_WAITTIME		= _T("kr_waittime");
const TCHAR* COLUMN_PARA_KR_TRANSTIME		= _T("kr_transtime");
const TCHAR* COLUMN_PARA_KR_ACCTIME			= _T("kr_acctime");
const TCHAR* COLUMN_PARA_KR_ERGTIME			= _T("kr_ergtime");
const TCHAR* COLUMN_PARA_KR_COST			= _T("kr_cost");
const TCHAR* COLUMN_PARA_KR_LENGTH			= _T("kr_length");
const TCHAR* COLUMN_PARA_KR_NOTRANSFER		= _T("kr_notrans");

const TCHAR* COLUMN_PARA_TT_INVEHTIME		= _T("tt_invehtime");
const TCHAR* COLUMN_PARA_TT_WAITTIME		= _T("tt_waittime");
const TCHAR* COLUMN_PARA_TT_TRANSTIME		= _T("tt_transtime");
const TCHAR* COLUMN_PARA_TT_ACCTIME			= _T("tt_acctime");
const TCHAR* COLUMN_PARA_TT_ERGTIME			= _T("tt_egrtime");
const TCHAR* COLUMN_PARA_TT_COST			= _T("tt_cost");
const TCHAR* COLUMN_PARA_TT_LENGTH			= _T("tt_length");
const TCHAR* COLUMN_PARA_TT_NOTRANSFER		= _T("tt_notrans");

const TCHAR* COLUMN_CHAINMODEL_ROAD         = _T("road");
const TCHAR* COLUMN_CHAINMODEL_RAIL         = _T("rail");

const TCHAR* COLUMN_VEHICLECONV_MODEL_SMALL  = _T("small");
const TCHAR* COLUMN_VEHICLECONV_MODEL_MEDIUM = _T("medium");
const TCHAR* COLUMN_VEHICLECONV_MODEL_LARGE  = _T("large");
const TCHAR* COLUMN_VEHICLECONV_MODEL_SUM    = _T("sum");

const TCHAR* COLUMN_TERMINAL_SCHEDULE_SEQ         =_T("seq"); 
const TCHAR* COLUMN_TERMINAL_SCHEDULE_O_TERMINAL  =_T("o_terminal");
const TCHAR* COLUMN_TERMINAL_SCHEDULE_D_TERMINAL  =_T("d_terminal");
const TCHAR* COLUMN_TERMINAL_SCHEDULE_MODE        =_T("mode");

const TCHAR* COLUMN_ACCESS_STOP_NODE_ZONE_ID      = _T("zone_id");
const TCHAR* COLUMN_ACCESS_STOP_NODE_NODE_ID      = _T("node_id");
const TCHAR* COLUMN_ACCESS_STOP_NODE_MODE         = _T("mode");
const TCHAR* COLUMN_ACCESS_STOP_NODE_ZONE_NAME    = _T("zone_name");
const TCHAR* COLUMN_ACCESS_STOP_NODE_NO_OF_LINE   = _T("no_of_line");

const TCHAR* COLUMN_SED_POP_ZONE_ID               = _T("zone_id");
const TCHAR* COLUMN_SED_POP_SEX                   = _T("sex");
const TCHAR* COLUMN_SED_POP_AGE                   = _T("age");


/* Define Dll Path*/
const TCHAR* DLL_READ_WRITE_PATH   = _T("\\MODEL\\");
const TCHAR* DLL_MODEL_FOLDER_NAME = _T("MODEL");
const TCHAR* DLL_HIGHWAYASSIGN_FOLDER_NAME	= _T("HighwayAssign"); // Highway dll  폴더
const TCHAR* DLL_MULTIMODAL_FOLDER_NAME	= _T("MultiModalAssign");// 멀티 Assign dll  폴더
const TCHAR* DLL_INTERMODAL_FOLDER_NAME		= _T("InterModalAssign\\");// 인터 Assign dll  폴더
const TCHAR* INTERMODAL_FOLDER_NAME		    = _T("InterModalAssign");
const TCHAR* MULTIMODAL_GENERATOR_PATH		= _T("TransitGenerator\\"); // 멀티 Genen dll 폴더
const TCHAR* MULTIMODAL_GENERATOR_FOLDER_NAME= _T("TransitGenerator");
const TCHAR* MULTIMODAL_RESULT_PATH			= _T("TransitPathAssign\\");
const TCHAR* INTERMODAL_RESULT_PATH			= _T("IntermodalPathAssign\\");
const TCHAR* TRANST_SHORTEST_RESULT_PATH	= _T("TransitShortestPath\\");
const TCHAR* GENERATE_TRANSIT_RESULT_PATH	= _T("TransitPath\\");
const TCHAR* INTERMODAL_GENERATOR_PATH		= _T("Intermodalgenerator\\");// 인터 Gener dll 폴더
const TCHAR* INTERMODAL_GEN_RESULT_PATH		= _T("IntermodalPath\\");
const TCHAR* INTERMODAL_GENERATOR_FOLDER_NAME = _T("Intermodalgenerator");
const TCHAR* INTERMODAL_ACCESSTERMINAL_FOLDER_NAME = _T("IntemodalAccessTerminal");
const TCHAR* ZONING_FOLDER_NAME = _T("ZoningPath");
/* Define Bush Path*/
const TCHAR* BUSH_PATHVOLUMN_FOLDER_NAME = _T("PATHVOLUME");


/* Define DB File names */
const TCHAR* NETWORKFILE_NAME = _T("network.mdb");
const TCHAR* IODATAFILE_NAME  = _T("iolocal.db");
const TCHAR* PROJECTFILE_NAME = _T("project.db");

const TCHAR* DEFAULT_IODATAFILE_NAME     = _T("template01.blank");
const TCHAR* DEFAULT_IODATAFILE_NAMEENG  = _T("template01eng.blank");
const TCHAR* DEFAULT_PROJECTFILE_NAME    = _T("template02.deftv");
const TCHAR* DEFAULT_PROJECTFILE_NAMEENG    = _T("template02eng.deftv");

/* Define Log File names*/
const TCHAR* ASSIGNMENT_LOGFILE_NAME		= _T("TrafficAssignment_Log.txt");
const TCHAR* OBUE_ASSIGNMENT_LOGFILE_NAME	= _T("OBUE_Log.txt");
const TCHAR* MODECHOICE_LOGFILE_NAME		= _T("Elasticity_Result.txt");
const TCHAR* ASSIGN_LOGFILE_NAME			= _T("Assignment_Log.txt");

/* Define dat file names */
const char*    DAT_NODE =    "node.dat";
const TCHAR* T_DAT_NODE	= _T("node.dat");
const char*    DAT_LINK	=    "link.dat";
const TCHAR* T_DAT_LINK	= _T("link.dat");
const char*    DAT_TURN	=	 "turn.dat";
const TCHAR* T_DAT_TURN	= _T("turn.dat");

const char*    DAT_LINEINFO	=	 "line_info.dat";
const TCHAR* T_DAT_LINEINFO	= _T("line_info.dat");
const char*    DAT_LINE	    =	 "line.dat";
const TCHAR* T_DAT_LINE	    = _T("line.dat");


const char*    DAT_OUT_HSHORTEST_NODE	=    "HShortestPath_Node.dat";
const TCHAR* T_DAT_OUT_HSHORTEST_NODE	= _T("HShortestPath_Node.dat");
const char*    DAT_OUT_HSHORTEST_LINK	=    "HShortestPath_Link.dat";
const TCHAR* T_DAT_OUT_HSHORTEST_LINK	= _T("HShortestPath_Link.dat");
const char*    ARG_HSHORTESTPATH        =    "FHighwayShortestPath.arg";
const TCHAR* T_ARG_HSHORTESTPATH        = _T("FHighwayShortestPath.arg");

const char*    DAT_OUT_TSHORTEST_TRANSIT=    "TShortestPath_Transit.dat";
const TCHAR* T_DAT_OUT_TSHORTEST_TRANSIT= _T("TShortestPath_Transit.dat");
const char*    ARG_TSHORTESTPATH        =    "FTransitShortestPath.arg";
const TCHAR* T_ARG_TSHORTESTPATH        = _T("FTransitShortestPath.arg");

const char*    DAT_OUT_INTERSHORTEST_INTERMODAL =    "IShortestPath_Intermodal.dat";
const TCHAR* T_DAT_OUT_INTERSHORTEST_INTERMODAL = _T("IShortestPath_Intermodal.dat");
const char*    DAT_OUT_INTERSHORTEST_AUTOPATH   =    "IAuto_Path.dat";
const TCHAR* T_DAT_OUT_INTERSHORTEST_AUTOPATH   = _T("IAuto_Path.dat");
const char*    ARG_INTERSHORTESTPATH            =    "FIntermodalShortestPath.arg";
const TCHAR* T_ARG_INTERSHORTESTPATH            = _T("FIntermodalShortestPath.arg");

const char*    DAT_IN_STATISTICS  =     "StaData.dat";
const TCHAR* T_DAT_IN_STATISTICS  =  _T("StaData.dat");
const char*    DAT_OUT_STATISTICS =     "StaResult.dat";
const TCHAR* T_DAT_OUT_STATISTICS =  _T("StaResult.dat");
const char*    ARG_STATISTICS =         "StaAnalysis.arg";
const TCHAR* T_ARG_STATISTICS =      _T("StaAnalysis.arg");

const char*    DAT_IN_SED          = "SED.dat";
const char*    DAT_IN_MODEOD       = "MODEOD.dat";
const char*    DAT_IN_MODEOD_2     = "mode_od.dat";
const char*    DAT_IN_ODPARAMETER  = "ODParameter.dat";
const char*    DAT_IN_PERSONTRIP   = "Persontrip.dat";
const char*    DAT_OUT_TIMEOD      = "TimeOD.dat";
const char*    TXT_OUT_TRIPRATE    = "TripRate.txt";
const char*    ARG_TIMEOD          = "TimeODGeneration.arg";

const TCHAR*   T_DAT_IN_SED        =_T("SED.dat");
const TCHAR*   T_DAT_IN_MODEOD     =_T("MODEOD.dat");
const TCHAR*   T_DAT_IN_MODEOD_2   =_T("mode_od.dat");
const TCHAR*   T_DAT_PURPOSEOD	   =_T("purpose_od.dat");
const TCHAR*   T_DAT_IN_ODPARAMETER=_T("ODParameter.dat");
const TCHAR*   T_DAT_IN_PERSONTRIP =_T("Persontrip.dat");
const TCHAR*   T_DAT_OUT_TIMEOD    =_T("TimeOD.dat");
const TCHAR*   T_TXT_OUT_TRIPRATE  =_T("TripRate.txt");
const TCHAR*   T_ARG_TIMEOD        =_T("TimeODGeneration.arg");

const TCHAR*   TIMEOD_TRIPRATE_FILENAME = _T("TripRate.txt");

const char*    ARG_ZONING             = "Zoning.arg";
const char*    DAT_IN_PURPOSEOD       = "PURPOSEOD.dat";
const char*    DAT_IN_ZONE            = "ZONE.dat";
const char*    DAT_OUT_ZONE           = "ZONE_out.dat";
const char*    DAT_OUT_SED            = "SED_out.dat";
const char*    DAT_OUT_PURPOSEOD      = "PURPOSEOD_out.dat";
const char*    DAT_OUT_MODEOD         = "MODEOD_out.dat";
const char*    DAT_OUT_ODPARAMETER    = "ODParameter_out.dat";
const char*    DAT_OUT_ZONECONNECTLINK = "ZoneConnlinks_out.dat";


const TCHAR*   T_ARG_ZONING           =_T("Zoning.arg");
const TCHAR*   T_DAT_IN_PURPOSEOD     =_T("PURPOSEOD.dat");
const TCHAR*   T_DAT_IN_ZONE          =_T("ZONE.dat");
const TCHAR*   T_DAT_OUT_ZONE         =_T("ZONE_out.dat");
const TCHAR*   T_DAT_OUT_SED          =_T("SED_out.dat");
const TCHAR*   T_DAT_OUT_PURPOSEOD    =_T("PURPOSEOD_out.dat");
const TCHAR*   T_DAT_OUT_MODEOD       =_T("MODEOD_out.dat");
const TCHAR*   T_DAT_OUT_ODPARAMETER  =_T("ODParameter_out.dat");
const TCHAR*   T_DAT_OUT_ZONECONNECTLINK = _T("ZoneConnlinks_out.dat");

const char*    ARG_PATHVOLUME         = "PathVolume.arg";
const char*    DAT_OUT_PATHVOLUME     = "PathVolume.out";

const TCHAR*   T_ARG_PATHVOLUME       = _T("PathVolume.arg");
const TCHAR*   T_DAT_OUT_PATHVOLUME   = _T("PathVolume.out");

const TCHAR*   T_ARG_PATHVOLUME_AREA     = _T("AreaToAreaHighwayPathAnalysis.arg");
const TCHAR*   T_DAT_OUT_PATHVOLUME_AREA = _T("AreaToAreaHighwayPathAnalysis.out");

const char*    ARG_LINKANALYSIS              = "SelectedLinkAnalysis.arg";
const char*    DAT_OUT_LINKANALYSIS_LINK     = "SelectedLink_Link.out";
const char*    DAT_OUT_LINKANALYSIS_OD       = "SelectedLink_OD.out";

const TCHAR*   T_ARG_LINKANALYSIS            = _T("SelectedLinkAnalysis.arg");
const TCHAR*   T_DAT_OUT_LINKANALYSIS_LINK   = _T("SelectedLink_Link.out");
const TCHAR*   T_DAT_OUT_LINKANALYSIS_OD     = _T("SelectedLink_OD.out");	

const char*    ARG_ACCESSIBILITY                   = "SetAccessibility.arg";
const char*    DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE    = "AccBusSubwayStopAtZone.dat";

const TCHAR*   T_ARG_ACCESSIBILITY                 = _T("SetAccessibility.arg");
const TCHAR*   T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE  = _T("AccBusSubwayStopAtZone.dat");

const char*    ARG_GENERATEBUSPATH                 = "GenerateBusPath.arg";
const char*    DAT_OUT_R_BUSPATH                   = "R_BusPath.dat";

const TCHAR*   T_ARG_GENERATEBUSPATH               = _T("GenerateTransitPath.arg");
const TCHAR*   T_DAT_OUT_R_BUSPATH                 = _T("R_BusPath.dat");

const TCHAR*   T_ARG_PARMETER_CALCULATER		   = _T("ODParameter.arg");

const TCHAR*   T_ARG_DATA_INSPECTION               = _T("DataInspection.arg"); 
const TCHAR*   T_DAT_OUT_HIGHWAY_ERROR		       = _T("Highway_Error.txt");
const TCHAR*   T_DAT_OUT_HIGHWAY_WARNING		   = _T("Highway_Warning.txt");
const TCHAR*   T_DAT_OUT_TRANSIT_ERROR             = _T("Transit_Error.txt");
const TCHAR*   T_DAT_OUT_TRANSIT_WARNING           = _T("Transit_Warning.txt");
const TCHAR*   T_TXT_OUT_DATAINSPECTION_LOG        = _T("DataInspection_log.txt");

const TCHAR*   T_ARG_ACCESSTERMINALGENERATOR	   = _T("TerminalNodeSetting.arg");
const TCHAR*   T_DAT_TERMINAL_SCHEDULE			   = _T("TerminalSchedule.dat");
const TCHAR*   T_DAT_ACCESSTERMINAL_NODE		   = _T("AccessTerminalNodeAtZone.dat");

const TCHAR*   T_ARG_INTERMODAL_PATH_GENERATOR	   = _T("GenerateIntermodalPath.arg");

const TCHAR*   T_DAT_OUT_TURNVOLUME                = _T("turnvolume.dat");

const TCHAR*   IMPORT_FILE_NAME		               = _T("importData.dat");
const TCHAR*   IMPORT_ERROR_FILE_NAME		       = _T("importError.txt");

const TCHAR*   TSTAR = _T("*");
/*
const double MaxNodeLabelDisplayScale       =  20000.0;
const double MaxLinkLabelDisplayScale       =  12000.0;
const double MaxRealWidthDisplayScale       =  80000.0;
const double MaxTransitLabelDisplayScale    =   8000.0;
const double MaxTransitRealDisplayScale     =  50000.0;
const double MaxCartographicLineSymbolScale = 200000.0;
*/
const double MaxNodeLabelDisplayScale       =  30000.0;
const double MaxLinkLabelDisplayScale       =  20000.0;
const double MaxRealWidthDisplayScale       =  80000.0;

const double MaxTransitLabelDisplayScale    =  10000.0;
const double MaxTransitRealDisplayScale     =  50000.0;

const double MaxTurnPathDisplayScale        =  10000.0;
const double MaxIntersectionSymbolScale     =  10000.0;

const double MaxRealPathVolumeDisplayScale  =  1000000.0;
const double MaxPathVolumeLabelDisplayScale =  30000.0;

double MaxCartographicLineSymbolScale =  1000000.0;
double MaxLinkArrowDisplayScale       =  11000.0;
double PixcelTolerance                =  5.0;
double MaxToleranceDist               =  50.0;


//Code Group
const int	SYSTEM_NODECODE_GROUP_KEY	= 0;
const int	SYSTEM_LINKCODE_GROUP_KEY	= 1;
const int	USER_NODECODE_GROUP_KEY		= 2;
const int	USER_LINKCODE_GROUP_KEY		= 3;

const int   ZONE_MAINCODE_GROUP_KEY		= 112;
const int   ZONE_SUBCODE_GROUP_KEY		= 115;

//fright
const int	AVAILIABLE_ITEM_GROUP_KEY	= 112;

//System Node Code
const int	ZONE_CODE_KEY				= 0;
const int	HIGHWAY_NODE_CODE_KEY		= 1;
const int   SUBSATION_CODE_KEY			= 2;
const int	PARKANDRIDE					= 3;
const int   TRANSITCOMPLEX				= 4;
const int   INTER_REGIONAL_RAILSTATION   = 5;
const int   BUS_TRMINAL					= 6;
const int   INTER_REGIONAL_BUS_TRANSFER = 7;
const int   AIR_TERMIANL				= 8;
const int   MARIN_PORT					= 9;

//System Link Code
const int   ZONE_CONNECTOR_KEY          = 0;
const int	HIGHWAY_CODE_KEY			= 1;
const int   EXCLUSIVE_HOV_LANE_KEY		= 2;
const int   RAIIL_CODE_KEY				= 3;
const int   AIR_CODE_KEY                = 4;
const int   SEA_CODE_KEY                = 5;
const int   WALK_CONNECTOR_CODE_KEY     = 6;

//Transit 
const int   TRANSIT_TYPE_CODE_KEY		= 114;
const int   TRANSIT_MODE_CODE_KEY		= 113;

//intermodal code group
const int   TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY = 121;
const int   TARGET_REGION_CODE_GROUP_KEY		 = 122;

const int   MODEOD_GROUP_KEY    = 123;

//intermodal code key
const int TARGET_REGION_DEFAULT_CODE_KEY = 0;

const TCHAR* NODE_COMBO_INIT     = _T("--------- select ---------");

//paratmer_info_type
const int  PARAMETER_ALL		= -1;
const int  PARAMETER_LENGTH		= 1;
const int  PARAMETER_TIME		= 2;
const int  PARAMETER_COST		= 3;
const int  PARAMETER_ZONE		= 4;
const int  PARAMETER_NODE		= 5;

//Passenger Parameter Mode Type
const int P_MODE_AUTO			= 1;
const int P_MODE_TAXI			= 2;
const int P_MODE_TRUCK			= 3;
const int P_MODE_BUS			= 4;
const int P_MODE_RAIL			= 5;
const int P_MODE_BUS_RAIL		= 6;
const int P_MODE_PARK_RIDE		= 7;
const int P_MODE_KISS_RIDE		= 8;
const int P_MODE_TAXI_TRANSIT	= 9;

const int INTER_URBAN_CODE		= 1;
const int INTER_REGIONAL_CODE	= 2;

//parameter fuel
const int P_FUEL_GASOLINE		= 1;
const int P_FUEL_DIESEL			= 2;
const int P_FUEL_LPG			= 3;

const TCHAR* P_GASOLINE_NAME		= _T("Gasoline");
const TCHAR* P_DIESEL_NAME			= _T("Diesel");
const TCHAR* P_LPG_NAME				= _T("LPG");
const TCHAR* P_GASOLINE_NAME_K		= _T("휘발유");
const TCHAR* P_DIESEL_NAME_K		= _T("디젤");

//Transit Mode
const int TRANSIT_MODE_BUS	= 3;
const int TRANSIT_MODE_RAIL = 4;

// 수식 에서의 요소 type
const int FUNCTION_ELEMENT_NOFIELD         = 1;
const int FUNCTION_ELEMENT_REGISTERFIELD   = 2;
const int FUNCTION_ELEMENT_UNREGISTERFIELD = 3;

//Policy Type Define
const int POLICY_CONSTRUCTION_DEMOLITION = 100;
const int POLICY_IMPROVEMENT			 = 101;
const int POLICY_EVENTGROWANALYSIS		 = 102;
const int POLICY_COMPLEXCONSTRUCTION     = 103;
const int POLICY_SKIPANDSTOP			 = 104;

const int FPOLICY_SOCCONSTRUCTION		 = 201;
const int FPOLICY_RAILCONVERT			 = 202;

//NoneChangeColumnColor
const COLORREF NOTCHANGE_COLUMN_COLOR	 = RGB(224,238,238);

//Theme
const TCHAR* THEME_TEXT_BELOW = _T("Below");
const TCHAR* THEME_TEXT_ABOVE = _T("Above");

// Utility
CString UTIL_TRIPOD_TO_VEHOD_AREA[]			= { _T("1:수도권"), _T("2:전국권"), _T("3:부산울산권"), _T("4:대구광역권"), _T("5:대전충청권"), _T("6:광주광역권"), _T("7:제주권") };
const int UTIL_TRIPOD_TO_VEHOD_AREA_CNT		= _countof(UTIL_TRIPOD_TO_VEHOD_AREA);
