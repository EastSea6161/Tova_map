#pragma once

struct TPathInfo
{
    Integer nxNodeIdStart;
    Integer nxNodeIdEnd;
    COLORREF color;
    std::vector<Integer/*LinkID*/> vecLink;
};


struct TPathNodeInfo
{
	Integer nxNodeId;
	Integer nxBackLinkId;
	double  dCost;
};


struct TPathLinkInfo
{

	Integer nxLinkId;
	Integer nxBackLinkId;
	double  dCost;
};


struct TTransitPathBase
{
	Integer  nxTransitId;
	Integer  nxSeqStart;
	Integer  nxSeqEnd;
	double   dLength;
	double   dTime;
	double   dCost;
};


struct TTransitRouteCostData
{
	double  dTotal_Length;
	double  dTotal_Alltime;
	double  dTotal_TimeVeh;
	double  dTotal_TimeWait;
	double  dTotal_TimeTrans;
	double  dTotal_TimeAccessWalk;
	double  dTotal_TimeEgressWalk;
	double  dTotal_Cost;
};


/*
struct TTransitPathInfo
{
	Integer   nxTransitId;
	Integer   nxTransitType;
	double    dLength;
	double    dTime;
	double    dCost;

	Integer   nxNodeId_PathStart;
	Integer   nxNodeId_PathEnd;
	COLORREF  color;
	std::vector<Integer> vecLink;
};*/


/*
enum KEMTransitType
{
	TRANSIT_TYPE_WALK = 0,
	TRANSIT_TYPE_BUS  = 3,
	TRANSIT_TYPE_RAIL = 4
};*/

namespace ShortestPathInfo
{
    enum KEMTransitType
    {
        TRANSIT_TYPE_BUS  = 1,
        TRANSIT_TYPE_RAIL = 2,
        TRANSIT_TYPE_AUTO = 3,
        TRANSIT_TYPE_TAXI = 4,
        TRANSIT_TYPE_WALK = 5,
        TRANSIT_TYPE_TRUCK
    };
}

struct TTransitPathInfo
{
	TTransitPathBase  oTPathBase;

	ShortestPathInfo::KEMTransitType    nTransitType;
	Integer           nxNodeId_PathStart;
	Integer           nxNodeId_PathEnd;
	COLORREF          color;
	std::vector<Integer> vecLink;
};


struct TTransitRouteInfo
{
	Integer               nxNodeIdStart;
	Integer               nxNodeIdEnd;

	TTransitRouteCostData oTRouteCost;
	
	std::vector<TTransitPathInfo> vecPathInfo;
};


