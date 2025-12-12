/**
 * @file KBulkType.h
 * @brief KBulkDBase, KBulkDbaseFile Entity Class
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */


#pragma once

#include "model_common.h"

#pragma pack(push, 4) 



/**
* @brief TFixedNode Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedNode
{
    Integer node_id  ; 
    int     node_type;
    double  x        ; 
    double  y        ; 
    Integer zone_id  ; 
};

struct TFixedNodeInfo
{
	Integer node_id  ;
	CString name;
	int     node_type;
	double  x        ; 
	double  y        ; 
	Integer zone_id  ;
	int     target_region;
	double  parking_fee;

};

/**
* @brief TFixedLink Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedLink
{
    Integer link_id;
    Integer fnode_id;
    Integer tnode_id;
    int     link_type;
    double  length;
    int     lane;
    double	capa;
    double  speed;
};

/**
* @brief TFixedLink Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2012.10.10
*/
struct TFixedLink_B
{
	Integer link_id;
	Integer fnode_id;
	Integer tnode_id;
	int     link_type;
	double  length;
	int     lane;
	double	capa;
	double  speed;
	int     function_id;
};

/**
* @brief TFixedLink Entity
* @version 1.0
* @author jyk@qubicware.com
* @date 2013.02.04
*/
struct TFixedLink_C
{
	Integer link_id;
	Integer fnode_id;
	Integer tnode_id;
	int     link_type;
	double  length;
};

struct TFixedLink_Generator
{
	Integer link_id;
	Integer fnode_id;
	Integer tnode_id;
	int     link_type;
	double  length;
	double  speed;
};

/**
* @brief TFixedLink Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.04.23
*/
struct TFixedLink_D
{
	Integer link_id;
	Integer fnode_id;
	Integer tnode_id;
	int     link_type;
	double  length;
	int     lane;
	double	capa;
	double  speed;
	int     function_id;
	double  alpha;
	double  beta;
	double  a;
	double  b;
};

/**
* @brief TFixedLinkDB Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedLinkDB
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
};

/**
* @brief TFixedOD Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedOD
{
    Integer ozone_id;
    Integer dzone_id;
};

/**
* @brief TFTNode Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFTNode
{
    Integer fnode_id;
    Integer tnode_id;
};

/**
* @brief TFixedTurn Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedTurn
{
    Integer turn_id;
    Integer snode_id;
    Integer vnode_id;
    Integer enode_id;
    //double  tMaxValue;
    int     turn_type;
    //int     turn_time;
};

/**
* @brief TFixedTurnDB Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedTurnDB
{
    //Integer turn_id;
    Integer inlink_id;
    Integer node_id;
    Integer outlink_id;
    int     turn_type;
    int     turn_time;
};

/**
* @brief TFixedTransitFile Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedTransitFile
{
    Integer transit_id;
    int     type;
    int     detail_type;
    double  headway;
	double  frequency;
	double  commercial_speed;
    int     seat_count;
    int     capacity;
    int     lineCount;
};

/**
* @brief TFixedTransitDB Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedTransitDB
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
* @brief TFixedTransitLink Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedTransitLink
{
    Integer link_id;
    int     seq;
    int     station_yn;
};

/**
* @brief TFixedTransitFileRead Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedTransitFileRead
{
    Integer transit_id;
    double  trip;
};

/**
* @brief TFixedTransitLinkFileRead Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedTransitLinkFileRead
{
    Integer transit_id;
    int     seq;
    double  onPersons;
    double  offPersons;
    double  atPersons;
};

/**
* @brief TFixedSEDPop Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TFixedSEDPop
{
    Integer zone_id;
    int     sex;
    int     age;
    double  value;
};

/**
* @brief TFixedSEDVector Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/

struct TFixedSEDVector
{
	Integer nZone_id;
	int     nYear;
	CString strName;
	CString strFull_Name;
	int     nMCategory_id;
};

/**
* @brief TFixedIntersectionDB Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.06.20
*/
struct TFixedIntersectionDB
{
	Integer node_id;
	int     intersection_type;
	Integer first_bound_node_id;
	Integer second_bound_node_id;
	Integer third_bound_node_id;
	Integer fourth_bound_node_id;
	int     cross_type;
};

#pragma pack(pop)

#pragma pack(push, 1) 
/**
* @brief TFixedAccessLineStation Entity
* @version 1.0
* @author jyk@qbicware.com
* @date 2013.02.05
*/

struct TFixedAccessLineStation
{
	Integer zone_id; 
	Integer line_id;
	int     type; 
	int     no_station; 
};
#pragma pack(pop)