/**
 * @file KBulkTypeKTDB.h
 * @brief KBulkDBase, KBulkDbaseFile Entity Class
 * @author jyk@qbicware.com
 * @date 2012.02.01
 * @remark 
 */
#pragma once

#include "model_common.h"

#pragma pack(push, 4) 

/**
* @brief TKTDBNodeDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.02.01
*/
struct TKTDBNodeDB
{
	Integer node_id  ; 
	CString name	 ;
	int     node_type;
	double  x        ; 
	double  y        ; 
	Integer zone_id  ;
	int     facility_type;	/*only freight*/
	double  facility_fee;	/*only freight*/
	double  loading_fee;	/*only freight*/	
	double  unloading_fee;	/*only freight*/
	CString available_item;	/*only freight*/
	CString traffic_mean;	/*only freight*/
	int		occurrence_via;	/*only freight*/
	double  capa;			/*only freight*/

	int     target_region; /*only passenger*/
	double  parking_fee;   /*only passenger*/
};

/**
* @brief TKTDBZoneDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.02.01
*/
struct TKTDBZoneDB
{
	Integer zone_id      ; 
	CString name	     ;
	CString full_name    ; 
	Integer node_id		 ;
	Integer main_category;
	Integer sub_category ;
};

/**
* @brief TKTDBLinkDB Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2012.03.23
*/
struct TKTDBLinkDB
{
	Integer link_id;
	Integer fnode_id;
	Integer tnode_id;
	CString name;
	int     link_type;
	double  length;
	int     lane;
	double  capa;
	double  speed;
	double  survey_volume;
	int     influence;
	int     function_id;
	double  alpha;
	double  beta;
	double  a;
	double  b;
};


/**
* @brief TKTDBTurnDB Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2012.03.23
*/
struct TKTDBTurnDB
{
	Integer turn_id;
	Integer inlink_id;
	Integer node_id;
	Integer outlink_id;
	int     turn_type;
	int     turn_time;
};

/**
* @brief TKTDBIntersectionDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.06.06
*/
struct TKTDBIntersectionDB
{
	Integer node_id;
	int     intersection_type;
	Integer first_bound_node_id;
	Integer second_bound_node_id;
	Integer third_bound_node_id;
	Integer fourth_bound_node_id;
	int     cross_type;
};


/**
* @brief TKTDBDetailTypeDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.06.06
*/
struct TKTDBDetailTypeDB
{
	int        type_group_code;
	int        detail_type_id;
	CString    type_name;
};


/**
* @brief TKTDBTransitDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.03.23
*/
struct TKTDBTransitDB
{
	Integer transit_id;
	int     type;
	int     detail_type;
	CString name;
	double  peak_headway;
	double  offpeak_headway;
	double  avg_headway;
	double  peak_frequency;
	double  offpeak_frequency;
	double  total_frequency;
	double  commercial_speed;
	int     seat_count;	
	int     capacity;	
};


/**
* @brief TKTDBTransitLinkDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.03.22
*/
struct TKTDBTransitLinkDB
{
	Integer transit_id;
	Integer link_id;
	int     seq;
	int     station_yn;
	double  onPersons;
	double  offPersons;
	double  atPersons;
};

/**
* @brief TKTDBMatchingVehicleConvDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.05.31
*/
struct TKTDBMatchingVehicleConvDB
{
	Integer object_id;
	int     vehicle_type;
	int     object_group_code;
	CString average_loadage_id;
	CString goods_transported_id;
};

/**
* @brief TKTDBParameterODInfoDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.06.07
*/
struct TKTDBParameterODInfoDB
{
	CString column_name;
	int     purpose_code;
	int     mode_code;
	int     type_code;
};

/**
* @brief TKTDBParameterODDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.06.11
*/
struct TKTDBParameterODDB
{
	Integer ozone_id;
	Integer dzone_id;
	double  direct_dist;
	double  direct_time;
// 	Integer onode_id;
// 	Integer dnode_id;
// 	Integer station1_node_id;
// 	Integer station2_node_id;
// 	Integer station1_zone_id;
// 	Integer station2_zone_id;
// 	double  origin_station1_dist;
// 	double  station1_station2_dist;
// 	double  station2_destination_dist;
// 	Integer road_via_node;
// 	Integer road_via_zone;
// 	double  origin_via_dist;
// 	double  via_destination_dist;
};

/**
* @brief TKTDBParameterODDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.05.29
*/
struct TKTDBODDB
{
	Integer ozone_id;
	Integer dzone_id;
	std::vector<double> vecDoubleValue;
};

/**
* @brief TKTDBSedVectorDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.05.24
*/
struct TKTDBSedVectorDB
{
	Integer zone_id;
	CString name;
	CString full_name;
	int     main_category;	/*only freight*/
	std::vector<double> vecDoubleValue;
};

/**
* @brief TKTDBSedPopDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.05.24
*/
struct TKTDBSedPopDB
{
	Integer zone_id;
	int     sex;
	int     age;
	CString name;
	CString full_name;
	double  dValue/* value */;
};

/**
* @brief TKTDBCategoryAgeGroupDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.05.24
*/
struct TKTDBCategoryAgeGroupDB
{
	Integer category_id;
	int     seq;
	int     min;
	int     max;
};

/**
* @brief TKTDBCategoryClassDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.05.24
*/
struct TKTDBCategoryClassDB
{
	Integer category_id;
	CString name;
	int     sex_use;
	int     age_use;
};

/**
* @brief TKTDBColumnDescriptionDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.05.25
*/
struct TKTDBColumnDescriptionDB
{
	CString    table_name;
	CString    column_name;
	int        data_type;
	int        column_type;
	int        code_type;
	CString    display_yn;
	CString    freeze_yn;
	CString    display_name;
	CString    column_info;
	int        display_order;
	Integer    base_id;
	CString    pk_yn;
	int        use_flag;
};

/**
* @brief TKTDBKCodeGroupDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.04.12
*/
struct TKTDBKCodeGroupDB
{
	int       kcode_group_key;
	CString   kcode_group_name;
	CString   system_code_yn;
};

/**
* @brief TKTDBKCodeDefineDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.04.12
*/
struct TKTDBKCodeDefineDB
{
	int       kcode_group_key;
	int       kcode_key;
	CString   kcode_display;
};

/**
* @brief TKTDBTerminalScheduleDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.04.26
*/
struct TKTDBTerminalScheduleDB
{
	int       seq;
	Integer   o_terminal;
	Integer   d_terminal;
	CString   name;
	int       mode;
	double    travel_time;
	double    cost;
	int       frequency;
	double    headway;
};

/**
* @brief TKTDBDefineSystemCodeDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.05.02
*/
struct TKTDBDefineSystemCodeDB
{
	int user_code_group;
	int user_code_key;
	int system_code_group;
	int system_code_key;
};

/**
* @brief TKTDBRegionalModeDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.05.02
*/
struct TKTDBRegionalModeDB
{
	int regional_mode;
	int terminal_type;
};

/**
* @brief TKTDBTransitCostDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.05.02
*/
struct TKTDBTransitCostDB
{
	int    type;
	int    mode;
	double basic_rate;
	double basic_distance;
	double add_rate;
	double add_distance;
	int    merge_rete;
};

/**
* @brief TKTDBXBusRestDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.06.14
*/
struct TKTDBXBusRestDB
{
	CString data;
};

#pragma pack(pop)