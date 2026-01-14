/**
 * @file DefineNames.h
 * @brief Table name define 
 * @author 
 * @date 2011.08.17
 * @remark
 */


#pragma once
/*Define Talbe*/
extern const TCHAR* TABLE_SQLITE_MASTER;

/* Define Table names */
extern const TCHAR* TABLE_NODE;		               /** Node table name */
extern const TCHAR* TABLE_LINK;		               /** Link table name */
extern const TCHAR* TABLE_ZONE;		               /** Zone table name */
extern const TCHAR* TABLE_TURN;		               /** Turn table name */
extern const TCHAR* TABLE_KCODE_DEFINE;
extern const TCHAR* TABLE_KCODE_GROUP;
extern const TCHAR* TABLE_PURPOSE_OD;	           /** Purpose OD table name */
extern const TCHAR* TABLE_PARAMETER_OD;	           /** Parameter OD table name */
extern const TCHAR* TABLE_MODE_OD;		           /** Mode OD table name */
extern const TCHAR* TABLE_TRANSIT;		           /** Transit table name */
extern const TCHAR* TABLE_TRANSITLINKS;            /** Transit links table name */
extern const TCHAR* TABLE_TRANSITNODES;
extern const TCHAR* TABLE_INTERSECTION;            /** Intersection table name */
extern const TCHAR* TABLE_INTERSECTION_EXTENSION;  /** intersection_extension table name */
extern const TCHAR* TABLE_INTERSECTION_INFO;       /** intersection_info table name */
extern const TCHAR* TABLE_INTERSECTION_SIGNAL_INDICATION; /** intersection_signal_indication table name */
extern const TCHAR* TABLE_OBJECT_GROUP;            /** Object Group table name */
extern const TCHAR* TABLE_DETAIL_OBJECT;           /** Detail Object table name */
extern const TCHAR* TABLE_MODE_GROUP;	           /** Mode Group table name */
extern const TCHAR* TABLE_DETAIL_MODE;             /** Detail Mode table name */
extern const TCHAR* TABLE_INPUT_COEFFICIENT;       /** Input Coefficient table name */
extern const TCHAR* TABLE_INPUT_COEFFICIENT_MODEL; /** Input Coefficient Model table name */
extern const TCHAR* TABLE_SCREEN_CORDON;	       /** Screen_Cordon table name **/
extern const TCHAR* TABLE_SED_VECTOR;			   /** Sed_Vector table name **/
extern const TCHAR* TABLE_SED_POP;			       /** Sed_Pop table name **/
extern const TCHAR* TABLE_AVERAGE_LOADAGE_OD;      /** Average_loadage_OD table name **/
extern const TCHAR* TABLE_GOODS_TRANSPORTED_OD;    /** Goods_Transported_OD table name **/
extern const TCHAR* TABLE_VEHICLE_MODE_OD;         /** Vehicle_Mode_OD table name **/
extern const TCHAR* TABLE_EMPTY_VEHICLE_OD;        /** Empty_vehicle_OD table name **/
extern const TCHAR* TABLE_MATCHING_VEHICLECONV;    /** Matching_VehicleConv **/
extern const TCHAR* TABLE_CHAIN_OD;                /** chain_od table name */
extern const TCHAR* TABLE_INPUT_RATIO_OD;          /** Input_Ratio_OD table name */
extern const TCHAR* TABLE_RATIO_REGION;            /** Ratio_Region table name */
extern const TCHAR* TABLE_DISTRIBUTION_MODEL;      /** Distribution_Model table_name**/
extern const TCHAR* TABLE_GENERATION_MODEL;        /** Generation_Model table_name**/
extern const TCHAR* TABLE_MODECHOICE_MODEL;        /** modechoice_Model table_name**/
extern const TCHAR* TABLE_NETWORKOFF;			   /** Network_Off table_name**/
extern const TCHAR* TABLE_ACCESS_LINE_STATION;     /** access_line_station table_name**/
extern const TCHAR* TABLE_ACCESS_CLASS;            /** access_class table_name**/
extern const TCHAR* TABLE_BUS_PATH_COEFFICIENT;    /** bus_path_coefficient */
extern const TCHAR* TABLE_BUS_PATH_COST;           /** bus_path_cost */
extern const TCHAR* TABLE_BUS_PATH_INPUT;          /** bus_path_input */
extern const TCHAR* TABLE_ASSIGN_MODEL;            /** assign_model */
extern const TCHAR* TABLE_ASSIGN_PATH_INPUT;       /** assign_path_input */
extern const TCHAR* TABLE_ASSIGN_PATH_MODEL;       /** assign_path_model */
extern const TCHAR* TABLE_ASSIGN_TURN_VOLUME_INPUT;
extern const TCHAR* TABLE_DEFINE_SYSTEM_CODE;      /** define_system_code*/
extern const TCHAR* TABLE_BUS_PATH_MATRIX;
extern const TCHAR* TABLE_ACCESS_STOP_NODE;	
extern const TCHAR* TABLE_ACCESS_STOP_NODE_INFO;
extern const TCHAR* TABLE_TRANSIT_COST;
extern const TCHAR* TABLE_PARAMTER_OD_INFO;		  
extern const TCHAR* TABLE_PARAMTER_INFO_TYPE;
extern const TCHAR* TABLE_CHAIN_MODEL;
extern const TCHAR* TABLE_VEHICLECONV_MODEL;
extern const TCHAR* TABLE_REGIONAL_MODE_DEFINE;
extern const TCHAR* TABLE_ACCESS_TERMINAL_INFO;
extern const TCHAR* TABLE_ACCESS_TERMINAL_NODE;
extern const TCHAR* TABLE_TERMINAL_SCHEDULE;
extern const TCHAR* TABLE_INTER_PURPOSE_OD_INFO;
extern const TCHAR* TABLE_REGION_GLOBAL_TERMINAL;
extern const TCHAR* TABLE_INTER_URBAN_PATH_INFO;
extern const TCHAR* TABLE_INTER_REGION_PATH_INFO;
extern const TCHAR* TABLE_INTER_AUTO_COST_INFO;
extern const TCHAR* TABLE_INTER_TAXI_COST_INFO;
extern const TCHAR* TABLE_INTER_TIME_INFO;
extern const TCHAR* TABLE_INTER_TERMINAL_TIME_INFO;
extern const TCHAR* TABLE_INTER_TRANSIT_COST;
extern const TCHAR* TABLE_F_TERMINAL_PATH_INFO;
extern const TCHAR* TABLE_T_TERMINAL_PATHCOLUMN_INFO;
extern const TCHAR* TABLE_NETWORKEDIT_LOG;
extern const TCHAR* TABLE_CATEGORY_CLASS;
extern const TCHAR* TABLE_CATEGORY_AGE_GROUP;
extern const TCHAR* TABLE_CATEGORY_CLASS_VALUE;
extern const TCHAR* TABLE_SUBWAY_TRANSFER_INFO;
extern const TCHAR* TABLE_SUBWAY_TRANSFER_SECTION;
extern const TCHAR* TABLE_SUBWAY_TRANSFER_SECTION_LINE;
extern const TCHAR* TABLE_IMTASVERSION;
extern const TCHAR* TABLE_FMODE_MODEL;
extern const TCHAR* TABLE_DETAIL_FMODE;
extern const TCHAR* TABLE_ITEMLOGIT_MODEL;
extern const TCHAR* TABLE_TERMINAL_RESULT;
extern const TCHAR* TABLE_ASSIGN_INTER_MODE_INFO;

extern const TCHAR* TABLE_DISPLAY_OPTION;

extern const TCHAR* TABLE_TXSHP_LINK;
extern const TCHAR* TABLE_TXIDX_LINK;

extern const TCHAR* TABLE_TXSHP_NODE;
extern const TCHAR* TABLE_TXIDX_NODE;

extern const TCHAR* TABLE_TXSHP_ZONE;
extern const TCHAR* TABLE_TXIDX_ZONE;

/* Define column names */
extern const TCHAR* COLUMN_LINK_LANE;
extern const TCHAR* COLUMN_LINK_FN_ID;
extern const TCHAR* COLUMN_LINK_LENGTH;
extern const TCHAR* COLUMN_LINK_CAPA;
extern const TCHAR* COLUMN_LINK_SPEED;
extern const TCHAR* COLUMN_LINK_SURVEYVOLUME;
extern const TCHAR* COLUMN_LINK_ALPHA;
extern const TCHAR* COLUMN_LINK_BETA;
extern const TCHAR* COLUMN_LINK_A;
extern const TCHAR* COLUMN_LINK_B;
extern const TCHAR* COLUMN_LINK_TYPE;	/** Link type column name */
extern const TCHAR* COLUMN_NODE_ID;
extern const TCHAR* COLUMN_LINK_ID;
extern const TCHAR* COLUMN_FNODE_ID;
extern const TCHAR* COLUMN_TNODE_ID;

extern const TCHAR* COLUMN_NODE_TYPE;	/** Node type column name */
extern const TCHAR* COLUMN_NODE_X;		
extern const TCHAR* COLUMN_NODE_Y;
extern const TCHAR* COLUMN_ZONE_ID;
extern const TCHAR* COLUMN_NODE_FACILITY_TYPE;
extern const TCHAR* COLUMN_NODE_FACILITY_FEE;
extern const TCHAR* COLUMN_NODE_LOADING_FEE;
extern const TCHAR* COLUMN_NODE_UNLOADING_FEE;
extern const TCHAR* COLUMN_NODE_AVAILABLE_ITEM;
extern const TCHAR* COLUMN_NODE_TRAFFIC_MEAN;
extern const TCHAR* COLUMN_NODE_OCCURRENCE_VIA;
extern const TCHAR* COLUMN_NODE_CAPA;

extern const TCHAR* COLUMN_OZONE;		/** OZone column name */
extern const TCHAR* COLUMN_DZONE;		/** DZone column name */
extern const TCHAR* COLUMN_PZONE_ID;
extern const TCHAR* COLUMN_CZONE_ID;
extern const TCHAR* COLUMN_NODE_NAME;
extern const TCHAR* COLUMN_LINK_NAME;
extern const TCHAR* COLUMN_ZONE_NAME;
extern const TCHAR* COLUMN_ZONE_FULL_NAME;
extern const TCHAR* COLUMN_ZONE_MAIN_CATEGORY;

extern const TCHAR* COLUMN_TURN_ID;
extern const TCHAR* COLUMN_TURN_TYPE;
extern const TCHAR* COLUMN_TURN_F_LINK;
extern const TCHAR* COLUMN_TURN_T_LINK;
extern const TCHAR* COLUMN_TURN_NODE;

extern const TCHAR* COLUMN_TRANSIT_ID;
extern const TCHAR* COLUMN_TRANSIT_TYPE;
extern const TCHAR* COLUMN_TRANSIT_DETAIL_TYPE;
extern const TCHAR* COLUMN_TRANSIT_NAME      ;

extern const TCHAR* COLUMN_TRANSIT_PEAK_HEADWAY   ;
extern const TCHAR* COLUMN_TRANSIT_OFFPEAK_HEADWAY;
extern const TCHAR* COLUMN_TRANSIT_AVG_HEADWAY    ;
extern const TCHAR* COLUMN_TRANSIT_PEAK_FREQUENCY;
extern const TCHAR* COLUMN_TRANSIT_OFFPEAK_FREQUENCY;
extern const TCHAR* COLUMN_TRANSIT_TOTAL_FREQUENCY;
extern const TCHAR* COLUMN_TRANSIT_COMMERCIAL_SPEED;

extern const TCHAR* COLUMN_TRANSIT_SEAT_COUNT;
extern const TCHAR* COLUMN_TRANSIT_CAPACITY  ;

extern const TCHAR* COLUMN_TRANSIT_LINK_ID;
extern const TCHAR* COLUMN_TRANSIT_LINK_SEQ;
extern const TCHAR* COLUMN_TRANSIT_LINK_STATION_YN;

extern const TCHAR* COLUMN_TRANSIT_NODE_ID;
extern const TCHAR* COLUMN_TRANSIT_NODE_SEQ;
extern const TCHAR* COLUMN_TRANSIT_NODE_STATION_YN;

extern const TCHAR* COLUMN_OBJECT_GROUP_CODE;
extern const TCHAR* COLUMN_OBJECT_GROUP_NAME;
extern const TCHAR* COLUMN_DETAIL_OBJECT_ID;
extern const TCHAR* COLUMN_DETAIL_OBJECT_NAME;

extern const TCHAR* COLUMN_MODE_GROUP_CODE;
extern const TCHAR* COLUMN_MODE_GROUP_NAME;
extern const TCHAR* COLUMN_DETAIL_MODE_ID;
extern const TCHAR* COLUMN_DETAIL_MODE_NAME;

extern const TCHAR* COLUMN_ZONE_COLUMN_NAME;
extern const TCHAR* COLUMN_MODE_OD_COLUMN_NAME;
extern const TCHAR* COLUMN_PURPOSE_OD_COLUMN_NAME;


extern const TCHAR* COLUMN_PARAMETER_OD_DIRECT_DIST;
extern const TCHAR* COLUMN_PARAMETER_OD_DIRECT_TIME;
extern const TCHAR* COLUMN_PARAMETER_OD_ONODE_ID;					
extern const TCHAR* COLUMN_PARAMETER_OD_DNODE_ID;					
extern const TCHAR* COLUMN_PARAMETER_OD_STATION1_NODE_ID;			
extern const TCHAR* COLUMN_PARAMETER_OD_STATION2_NODE_ID;			
extern const TCHAR* COLUMN_PARAMETER_OD_STATION1_ZONE_ID;			
extern const TCHAR* COLUMN_PARAMETER_OD_STATION2_ZONE_ID;			
extern const TCHAR* COLUMN_PARAMETER_OD_ORIGIN_STATION1_DIST;		
extern const TCHAR* COLUMN_PARAMETER_OD_STATION1_STATION2_DIST;		
extern const TCHAR* COLUMN_PARAMETER_OD_STATION2_DESTINATION_DIST;
extern const TCHAR* COLUMN_PARAMETER_OD_ROAD_VIA_NODE;				
extern const TCHAR* COLUMN_PARAMETER_OD_ROAD_VIA_ZONE;				
extern const TCHAR* COLUMN_PARAMETER_OD_ORGIN_VIA_DIST;				
extern const TCHAR* COLUMN_PARAMETER_OD_VIA_DESTINATION_DIST;		

extern const TCHAR* COLUMN_INTERSECTION_FIRST_BOUND_NODE_ID;
extern const TCHAR* COLUMN_INTERSECTION_SECOND_BOUND_NODE_ID;
extern const TCHAR* COLUMN_INTERSECTION_THIRD_BOUND_NODE_ID;
extern const TCHAR* COLUMN_INTERSECTION_FOURTH_BOUND_NODE_ID;
extern const TCHAR* COLUMN_INTERSECTION_CROSS_TYPE;

extern const TCHAR* COLUMN_ASSIGN_AVERAGE_TIME;
extern const TCHAR* COLUMN_ASSIGN_VC;
extern const TCHAR* COLUMN_ASSIGN_SUM_VOLUME;
extern const TCHAR* COLUMN_ASSIGN_TRIP;
extern const TCHAR* COLUMN_ASSIGN_MODE_VOLUME;
extern const TCHAR* COLUMN_ASSIGN_VDF_TIME;
extern const TCHAR* COLUMN_ASSIGN_AVE_TIME_TOLL;
extern const TCHAR* COLUMN_ASSIGN_VDF_TOLL_TIME;
extern const TCHAR* COLUMN_ASSIGN_BUS_INITIAL_VOLUME;
extern const TCHAR* COLUMN_ASSIGN_OD_AVERAGE_TIME;
extern const TCHAR* COLUMN_ASSIGN_OD_VDF_TIME;
extern const TCHAR* COLUMN_ASSIGN_OD_AVERAGE_BUS;
extern const TCHAR* COLUMN_ASSIGN_OD_AVERAGE_SUBWAY;
extern const TCHAR* COLUMN_ASSIGN_OD_AVERAGE_BUSSUBWAY;
extern const TCHAR* COLUMN_ASSIGN_OD_BUS_DIRECT;
extern const TCHAR* COLUMN_ASSIGN_OD_BUS_ONE_TRANSFER;
extern const TCHAR* COLUMN_ASSIGN_OD_BUS_TWO_TRANSFER;
extern const TCHAR* COLUMN_ASSIGN_LINFO_NUM_PASSENGERS;
extern const TCHAR* COLUMN_ASSIGN_OD_BUS;
extern const TCHAR* COLUMN_ASSIGN_OD_RAIL;
extern const TCHAR* COLUMN_ASSIGN_OD_BUSRAIL;
extern const TCHAR* COLUMN_ASSIGN_LINE_BOARD_NET;
extern const TCHAR* COLUMN_ASSIGN_LINE_BOARD_TRANSFER;
extern const TCHAR* COLUMN_ASSIGN_LINE_BOARD_TOTAL;
extern const TCHAR* COLUMN_ASSIGN_LINE_ALIGHT_NET;
extern const TCHAR* COLUMN_ASSIGN_LINE_ALIGHT_TRANSFER;
extern const TCHAR* COLUMN_ASSIGN_LINE_ALIGHT_TOTAL;
extern const TCHAR* COLUMN_ASSIGN_LINE_PASSENGER;
extern const TCHAR* COLUMN_ASSIGN_NODE_BUS_NET_BOARD;
extern const TCHAR* COLUMN_ASSIGN_NODE_BUS_TRANSFER_BOARD;
extern const TCHAR* COLUMN_ASSIGN_NODE_BUS_SUM_BOARD;
extern const TCHAR* COLUMN_ASSIGN_NODE_BUS_NET_ALIGHT;
extern const TCHAR* COLUMN_ASSIGN_NODE_BUS_TRANSFER_ALIGHT;
extern const TCHAR* COLUMN_ASSIGN_NODE_BUS_SUM_ALIGHT;

//Define passenger od_parameter
extern const TCHAR* COLUMN_PARA_AUTO_INVEHTIME;
extern const TCHAR* COLUMN_PARA_AUTO_COST;
extern const TCHAR* COLUMN_PARA_AUTO_LENGTH;
extern const TCHAR* COLUMN_PARA_TAXI_INVEHTIME;
extern const TCHAR* COLUMN_PARA_TAXI_COST;
extern const TCHAR* COLUMN_PARA_TAXI_LENGTH;
extern const TCHAR* COLUMN_PARA_TRUCK_INVEHTIME;
extern const TCHAR* COLUMN_PARA_TRUCK_COST;
extern const TCHAR* COLUMN_PARA_TRUCK_LENGTH;
extern const TCHAR* COLUMN_PARA_BUS_INVEHTIME;
extern const TCHAR* COLUMN_PARA_BUS_WAITTIME;
extern const TCHAR* COLUMN_PARA_BUS_TRANSTIME;
extern const TCHAR* COLUMN_PARA_BUS_ACCTIME;
extern const TCHAR* COLUMN_PARA_BUS_ERGTIME;
extern const TCHAR* COLUMN_PARA_BUS_COST;
extern const TCHAR* COLUMN_PARA_BUS_LENGTH;
extern const TCHAR* COLUMN_PARA_BUS_NOTRANSFER;
extern const TCHAR* COLUMN_PARA_BUS_NOOFSTOP;
extern const TCHAR* COLUMN_PARA_RAIL_INVEHTIME;
extern const TCHAR* COLUMN_PARA_RAIL_WAITTIME;
extern const TCHAR* COLUMN_PARA_RAIL_TRANSTIME;
extern const TCHAR* COLUMN_PARA_RAIL_ACCTIME;
extern const TCHAR* COLUMN_PARA_RAIL_ERGTIME;
extern const TCHAR* COLUMN_PARA_RAIL_COST;
extern const TCHAR* COLUMN_PARA_RAIL_LENGTH;
extern const TCHAR* COLUMN_PARA_RAIL_NOTRANSFER;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_INVEHTIME;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_WAITTIME;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_TRANSTIME;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_ACCTIME;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_ERGTIME;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_COST;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_LENGTH;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_NOTRANSFER;
extern const TCHAR* COLUMN_PARA_BUS_RAIL_NOOFSTOP;
extern const TCHAR* COLUMN_PARA_PR_INVEHTIME;
extern const TCHAR* COLUMN_PARA_PR_WAITTIME;
extern const TCHAR* COLUMN_PARA_PR_TRANSTIME;
extern const TCHAR* COLUMN_PARA_PR_ACCTIME;
extern const TCHAR* COLUMN_PARA_PR_ERGTIME;
extern const TCHAR* COLUMN_PARA_PR_COST;
extern const TCHAR* COLUMN_PARA_PR_LENGTH;
extern const TCHAR* COLUMN_PARA_PR_NOTRANSFER;
extern const TCHAR* COLUMN_PARA_KR_INVEHTIME;
extern const TCHAR* COLUMN_PARA_KR_WAITTIME;
extern const TCHAR* COLUMN_PARA_KR_TRANSTIME;
extern const TCHAR* COLUMN_PARA_KR_ACCTIME;
extern const TCHAR* COLUMN_PARA_KR_ERGTIME;
extern const TCHAR* COLUMN_PARA_KR_COST;
extern const TCHAR* COLUMN_PARA_KR_LENGTH;
extern const TCHAR* COLUMN_PARA_KR_NOTRANSFER;
extern const TCHAR* COLUMN_PARA_TT_INVEHTIME;
extern const TCHAR* COLUMN_PARA_TT_WAITTIME;
extern const TCHAR* COLUMN_PARA_TT_TRANSTIME;
extern const TCHAR* COLUMN_PARA_TT_ACCTIME;
extern const TCHAR* COLUMN_PARA_TT_ERGTIME;
extern const TCHAR* COLUMN_PARA_TT_COST;
extern const TCHAR* COLUMN_PARA_TT_LENGTH;
extern const TCHAR* COLUMN_PARA_TT_NOTRANSFER;

extern const TCHAR* COLUMN_CHAINMODEL_ROAD;
extern const TCHAR* COLUMN_CHAINMODEL_RAIL;

extern const TCHAR* COLUMN_VEHICLECONV_MODEL_SMALL;
extern const TCHAR* COLUMN_VEHICLECONV_MODEL_MEDIUM;
extern const TCHAR* COLUMN_VEHICLECONV_MODEL_LARGE;
extern const TCHAR* COLUMN_VEHICLECONV_MODEL_SUM;

extern const TCHAR* COLUMN_TERMINAL_SCHEDULE_SEQ; 
extern const TCHAR* COLUMN_TERMINAL_SCHEDULE_O_TERMINAL;
extern const TCHAR* COLUMN_TERMINAL_SCHEDULE_D_TERMINAL;
extern const TCHAR* COLUMN_TERMINAL_SCHEDULE_MODE;

extern const TCHAR* COLUMN_ACCESS_STOP_NODE_ZONE_ID;
extern const TCHAR* COLUMN_ACCESS_STOP_NODE_NODE_ID;
extern const TCHAR* COLUMN_ACCESS_STOP_NODE_MODE;
extern const TCHAR* COLUMN_ACCESS_STOP_NODE_ZONE_NAME;
extern const TCHAR* COLUMN_ACCESS_STOP_NODE_NO_OF_LINE;

extern const TCHAR* COLUMN_SED_POP_ZONE_ID;
extern const TCHAR* COLUMN_SED_POP_SEX;
extern const TCHAR* COLUMN_SED_POP_AGE;

/* Define Dll Path*/
extern const TCHAR* DLL_READ_WRITE_PATH;
extern const TCHAR* DLL_MODEL_FOLDER_NAME;
extern const TCHAR* DLL_HIGHWAYASSIGN_FOLDER_NAME;
extern const TCHAR* DLL_MULTIMODAL_FOLDER_NAME;
extern const TCHAR* MULTIMODAL_GENERATOR_PATH;
extern const TCHAR* MULTIMODAL_GENERATOR_FOLDER_NAME;
extern const TCHAR* DLL_INTERMODAL_FOLDER_NAME;
extern const TCHAR* INTERMODAL_FOLDER_NAME;
extern const TCHAR* MULTIMODAL_RESULT_PATH;
extern const TCHAR* INTERMODAL_RESULT_PATH;
extern const TCHAR* TRANST_SHORTEST_RESULT_PATH;
extern const TCHAR* GENERATE_TRANSIT_RESULT_PATH;
extern const TCHAR* INTERMODAL_GENERATOR_PATH;
extern const TCHAR* INTERMODAL_GEN_RESULT_PATH;
extern const TCHAR* INTERMODAL_GENERATOR_FOLDER_NAME;
extern const TCHAR* INTERMODAL_ACCESSTERMINAL_FOLDER_NAME;
extern const TCHAR* ZONING_FOLDER_NAME;

/*Define BUSH Path*/
extern const TCHAR* BUSH_PATHVOLUMN_FOLDER_NAME;

/* Define DB File names */
extern const TCHAR* NETWORKFILE_NAME;	/** network db file name */
extern const TCHAR* IODATAFILE_NAME;	/** lolocal db file name */
extern const TCHAR* PROJECTFILE_NAME;	/** project db file name */

extern const TCHAR* DEFAULT_IODATAFILE_NAME;
extern const TCHAR* DEFAULT_IODATAFILE_NAMEENG;
extern const TCHAR* DEFAULT_PROJECTFILE_NAME;
extern const TCHAR* DEFAULT_PROJECTFILE_NAMEENG;

/* Define Log File names*/
extern const TCHAR* ASSIGNMENT_LOGFILE_NAME;
extern const TCHAR* OBUE_ASSIGNMENT_LOGFILE_NAME;
extern const TCHAR* MODECHOICE_LOGFILE_NAME;
extern const TCHAR* ASSIGN_LOGFILE_NAME;

/* Define dat file names */
extern const char* DAT_NODE;
extern const char* DAT_LINK;
extern const char* DAT_TURN;
extern const char* DAT_LINEINFO;
extern const char* DAT_LINE;
extern const char* DAT_OUT_HSHORTEST_NODE;
extern const char* DAT_OUT_HSHORTEST_LINK;
extern const char* ARG_HSHORTESTPATH;
extern const char* DAT_OUT_TSHORTEST_TRANSIT;
extern const char* ARG_TSHORTESTPATH        ;
extern const char* DAT_OUT_INTERSHORTEST_INTERMODAL;
extern const char* DAT_OUT_INTERSHORTEST_AUTOPATH  ;
extern const char* ARG_INTERSHORTESTPATH              ;

extern const TCHAR* T_DAT_NODE;
extern const TCHAR* T_DAT_LINK;
extern const TCHAR* T_DAT_TURN;
extern const TCHAR* T_DAT_LINEINFO;
extern const TCHAR* T_DAT_LINE;
extern const TCHAR* T_DAT_PURPOSEOD;
extern const TCHAR* T_DAT_OUT_HSHORTEST_NODE;
extern const TCHAR* T_DAT_OUT_HSHORTEST_LINK;
extern const TCHAR* T_ARG_HSHORTESTPATH;
extern const TCHAR* T_DAT_OUT_TSHORTEST_TRANSIT;
extern const TCHAR* T_ARG_TSHORTESTPATH;
extern const TCHAR* T_DAT_OUT_INTERSHORTEST_INTERMODAL;
extern const TCHAR* T_DAT_OUT_INTERSHORTEST_AUTOPATH;
extern const TCHAR* T_ARG_INTERSHORTESTPATH;

extern const char*    DAT_IN_STATISTICS;
extern const char*    DAT_OUT_STATISTICS;
extern const char*    ARG_STATISTICS;

extern const TCHAR* T_DAT_IN_STATISTICS;
extern const TCHAR* T_DAT_OUT_STATISTICS;
extern const TCHAR* T_ARG_STATISTICS;


extern const char*    DAT_IN_SED;
extern const char*    DAT_IN_MODEOD;
extern const char*    DAT_IN_MODEOD_2;
extern const char*    DAT_IN_ODPARAMETER;
extern const char*    DAT_IN_PERSONTRIP;
extern const char*    DAT_OUT_TIMEOD;
extern const char*    TXT_OUT_TRIPRATE;
extern const char*    ARG_TIMEOD;

extern const TCHAR*   T_DAT_IN_SED;
extern const TCHAR*   T_DAT_IN_MODEOD;
extern const TCHAR*   T_DAT_IN_MODEOD_2;
extern const TCHAR*   T_DAT_IN_ODPARAMETER;
extern const TCHAR*   T_DAT_IN_PERSONTRIP;
extern const TCHAR*   T_DAT_OUT_TIMEOD;
extern const TCHAR*   T_TXT_OUT_TRIPRATE;
extern const TCHAR*   T_ARG_TIMEOD;

extern const TCHAR*   TIMEOD_TRIPRATE_FILENAME;

extern const char*    ARG_ZONING;
extern const char*    DAT_IN_PURPOSEOD;
extern const char*    DAT_IN_ZONE;
extern const char*    DAT_OUT_ZONE;
extern const char*    DAT_OUT_SED;
extern const char*    DAT_OUT_PURPOSEOD;
extern const char*    DAT_OUT_MODEOD;
extern const char*    DAT_OUT_ODPARAMETER;
extern const char*    DAT_OUT_ZONECONNECTLINK;

extern const TCHAR*   T_ARG_ZONING;
extern const TCHAR*   T_DAT_IN_PURPOSEOD;
extern const TCHAR*   T_DAT_IN_ZONE;
extern const TCHAR*   T_DAT_OUT_ZONE;
extern const TCHAR*   T_DAT_OUT_SED;
extern const TCHAR*   T_DAT_OUT_PURPOSEOD;
extern const TCHAR*   T_DAT_OUT_MODEOD;
extern const TCHAR*   T_DAT_OUT_ODPARAMETER;
extern const TCHAR*   T_DAT_OUT_ZONECONNECTLINK;

extern const char*    ARG_PATHVOLUME;
extern const char*    DAT_OUT_PATHVOLUME;

extern const TCHAR*   T_ARG_PATHVOLUME;
extern const TCHAR*   T_DAT_OUT_PATHVOLUME;

extern const TCHAR*   T_ARG_PATHVOLUME_AREA;
extern const TCHAR*   T_DAT_OUT_PATHVOLUME_AREA;

extern const char*    ARG_LINKANALYSIS           ;
extern const char*    DAT_OUT_LINKANALYSIS_LINK  ;
extern const char*    DAT_OUT_LINKANALYSIS_OD    ;

extern const TCHAR*   T_ARG_LINKANALYSIS         ;
extern const TCHAR*   T_DAT_OUT_LINKANALYSIS_LINK;
extern const TCHAR*   T_DAT_OUT_LINKANALYSIS_OD  ;

extern const char*    ARG_ACCESSIBILITY;
extern const char*    DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE;

extern const TCHAR*   T_ARG_ACCESSIBILITY;
extern const TCHAR*   T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE;

extern const char*    ARG_GENERATEBUSPATH;
extern const char*    DAT_OUT_R_BUSPATH;

extern const TCHAR*   T_ARG_GENERATEBUSPATH;
extern const TCHAR*   T_DAT_OUT_R_BUSPATH;

extern const TCHAR*   T_ARG_PARMETER_CALCULATER;

extern const TCHAR*   T_ARG_DATA_INSPECTION;
extern const TCHAR*   T_DAT_OUT_HIGHWAY_ERROR;
extern const TCHAR*   T_DAT_OUT_HIGHWAY_WARNING;
extern const TCHAR*   T_DAT_OUT_TRANSIT_ERROR;
extern const TCHAR*   T_DAT_OUT_TRANSIT_WARNING;
extern const TCHAR*   T_TXT_OUT_DATAINSPECTION_LOG;

extern const TCHAR*   T_ARG_ACCESSTERMINALGENERATOR;
extern const TCHAR*   T_DAT_TERMINAL_SCHEDULE;
extern const TCHAR*   T_DAT_ACCESSTERMINAL_NODE;
extern const TCHAR*   T_ARG_INTERMODAL_PATH_GENERATOR;

extern const TCHAR*   T_DAT_OUT_TURNVOLUME;

extern const TCHAR*   IMPORT_FILE_NAME;
extern const TCHAR*   IMPORT_ERROR_FILE_NAME;


extern const TCHAR*   TSTAR;
extern const double MaxNodeLabelDisplayScale;
extern const double MaxLinkLabelDisplayScale;
extern const double MaxRealWidthDisplayScale;
extern const double MaxTransitRealDisplayScale;
extern const double MaxTransitLabelDisplayScale;
extern const double MaxTurnPathDisplayScale;
extern const double MaxIntersectionSymbolScale;

extern const double MaxRealPathVolumeDisplayScale;
extern const double MaxPathVolumeLabelDisplayScale;

extern double MaxCartographicLineSymbolScale;
extern double MaxLinkArrowDisplayScale;

extern double PixcelTolerance;
extern double MaxToleranceDist;

extern const int	SYSTEM_NODECODE_GROUP_KEY;
extern const int	SYSTEM_LINKCODE_GROUP_KEY;
extern const int	USER_NODECODE_GROUP_KEY;
extern const int	USER_LINKCODE_GROUP_KEY;
extern const int    ZONE_MAINCODE_GROUP_KEY;
extern const int    ZONE_SUBCODE_GROUP_KEY;
extern const int	AVAILIABLE_ITEM_GROUP_KEY;

extern const int	ZONE_CODE_KEY;
extern const int    SUBSATION_CODE_KEY;
extern const int    RAIIL_CODE_KEY;
extern const int	PARKANDRIDE;
extern const int	TRANSITCOMPLEX;
extern const int	INTER_REGIONAL_RAILSTATION;
extern const int	BUS_TRMINAL;
extern const int	INTER_REGIONAL_BUS_TRANSFER;
extern const int	AIR_TERMIANL;
extern const int	MARIN_PORT;
extern const int    EXCLUSIVE_HOV_LANE_KEY;
extern const int	HIGHWAY_NODE_CODE_KEY;
extern const int	HIGHWAY_CODE_KEY;
extern const int	TRANSIT_TYPE_CODE_KEY;
extern const int	TRANSIT_MODE_CODE_KEY;
extern const int    ZONE_CONNECTOR_KEY;
extern const int    AIR_CODE_KEY;
extern const int    SEA_CODE_KEY;
extern const int    WALK_CONNECTOR_CODE_KEY;

//intermodal
extern const int	TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY;
extern const int	TARGET_REGION_CODE_GROUP_KEY;
extern const int	TARGET_REGION_DEFAULT_CODE_KEY;

extern const int    MODEOD_GROUP_KEY;

extern const int  PARAMETER_ALL;	
extern const int  PARAMETER_LENGTH;	
extern const int  PARAMETER_TIME;	
extern const int  PARAMETER_COST;	
extern const int  PARAMETER_ZONE;	
extern const int  PARAMETER_NODE;

extern const int INTER_URBAN_CODE;
extern const int INTER_REGIONAL_CODE;

extern const int P_MODE_AUTO;		
extern const int P_MODE_TAXI;		
extern const int P_MODE_TRUCK;		
extern const int P_MODE_BUS;	
extern const int P_MODE_RAIL;		
extern const int P_MODE_BUS_RAIL;
extern const int P_MODE_PARK_RIDE;
extern const int P_MODE_KISS_RIDE;	
extern const int P_MODE_TAXI_TRANSIT;

extern const int P_FUEL_GASOLINE;
extern const int P_FUEL_DIESEL;
extern const int P_FUEL_LPG;

extern const TCHAR* P_GASOLINE_NAME;
extern const TCHAR* P_DIESEL_NAME;
extern const TCHAR* P_LPG_NAME;
extern const TCHAR* P_GASOLINE_NAME_K;
extern const TCHAR* P_DIESEL_NAME_K;

extern const int TRANSIT_MODE_BUS;
extern const int TRANSIT_MODE_RAIL;

extern const int FUNCTION_ELEMENT_NOFIELD;
extern const int FUNCTION_ELEMENT_REGISTERFIELD;
extern const int FUNCTION_ELEMENT_UNREGISTERFIELD;

#define FREIGHT_DEFUALT_DISTANCE  0.0
#define FREIGHT_MAX_DISTANCE	  999
#define FREIGHT_UNKNOWN_DISTANCE  0.0
#define COLUMN_STATION_ACCESS_SEPARATOR _T(", ")
#define MULTI_COLUMN_SEPARATOR _T(", ")

#define NODE_POINT_COLOR RGB(0,  204,  255)
extern const TCHAR* NODE_COMBO_INIT;

extern const int POLICY_CONSTRUCTION_DEMOLITION;
extern const int POLICY_IMPROVEMENT;
extern const int POLICY_EVENTGROWANALYSIS;
extern const int POLICY_COMPLEXCONSTRUCTION;
extern const int POLICY_SKIPANDSTOP;

extern const int FPOLICY_SOCCONSTRUCTION;
extern const int FPOLICY_RAILCONVERT;

extern const COLORREF NOTCHANGE_COLUMN_COLOR;

extern const TCHAR* THEME_TEXT_BELOW;
extern const TCHAR* THEME_TEXT_ABOVE;

extern CString UTIL_TRIPOD_TO_VEHOD_AREA[];
extern const int UTIL_TRIPOD_TO_VEHOD_AREA_CNT;
