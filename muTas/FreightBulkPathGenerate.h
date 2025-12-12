#pragma once

class KTarget;

struct TRailPathInfo
{
    Integer TerminalA;
    Integer TerminalB;
    double  LengthOA;
    double  LengthAB;
    double  LenghtBD;
};

struct TRoadViaPathInfo
{
    Integer FacilityViaID;
    double  LengthOA;
    double  LengthAD;
};

struct TDistLinksInfo
{
	double	dDist;
	CString strLinks;
};

struct TViaColumnInfo
{
	int nID;
	CString strTerminalA;
	CString strTerminalB;
	CString strRoadVia;
};

struct TViaTermianlInfo
{
	Integer TermianlA;
	Integer TermianlB;
	Integer RoadVia;
};

class TItemTimeCost
{
public:
    TItemTimeCost ()
    {
        CommodityType = -1;

        RoadTime_LoadUp       = 120.0;
        RoadTime_LoadDown     = 120.0;
        RoadTime_Trip         = 1.22;

        RoadCost_LoadUp       = 10000.0;
        RoadCost_LoadDown     = 10000.0;
        RoadCost_Trip         = 78.4;

        RailTime_LoadUp       = 120.0;
        RailTime_LoadDown     = 120.0;
        RailTime_Trip         = 1.22;
        RailTime_Speed        = 60.0;

        RailCost_LoadUp       = 10000.0;
        RailCost_LoadDown     = 10000.0;
        RailCost_RoadTrip     = 78.4;
        RailCost_RailTrip     = 42.5;
        RailCost_FacilityCost = 10000.0;
    }
    ~TItemTimeCost()
    {
    }
public:
    int    CommodityType;
public:
    double RoadTime_LoadUp;
    double RoadTime_LoadDown;
    double RoadTime_Trip;

    double RoadCost_LoadUp;
    double RoadCost_LoadDown;
    double RoadCost_Trip;

    double RailTime_LoadUp;
    double RailTime_LoadDown;
    double RailTime_Trip;
    double RailTime_Speed;

    double RailCost_LoadUp;
    double RailCost_LoadDown;
    double RailCost_RoadTrip;
    double RailCost_RailTrip;
    double RailCost_FacilityCost;
};

class TFPathTimeInfo
{
public:
    TFPathTimeInfo () 
    {
        PathType           = 0;
        OriginID           = 0;
        DestinationID      = 0;
        TerminalID         = 0;
        RailTerminalFromID = 0;
        RailTerminalToID   = 0;

        TotalRoadTime      = 0.0;
        TotalRoadCost      = 0.0;
        TotalRailTime      = 0.0;
        TotalRailCost      = 0.0;

        TotalLength        = 0.0;
        TotalTime          = 0.0;
        TotalCost          = 0.0;
    }
    ~TFPathTimeInfo() {}
public:
    int     PathType;
    Integer OriginID;
    Integer DestinationID;

    Integer TerminalID;

    Integer RailTerminalFromID;
    Integer RailTerminalToID;
        
    double  TotalRoadTime;
    double  TotalRoadCost;

    double  TotalRailTime;
    double  TotalRailCost;

    double  TotalLength;
    double  TotalTime;
    double  TotalCost;
};

class KFreightBulkPathGenerate
{
public:
    KFreightBulkPathGenerate () {};
    ~KFreightBulkPathGenerate() {};
public:
    static void BulkPathGenerate(KTarget* a_pTarget, QBicSimpleProgressPtr a_spSimpleProgress);
private:
    static void ExtractResult( Integer nxOriginID, std::set<Integer>& setDestination,
                               std::map<KODKey, TDistLinksInfo>& mapKeyDist, 
                               KCostLinkCollection& rCostLinkCollection, 
                               KNodeArrivedMinLink& rNodeArrivedMinLink, 
                               QBicSimpleProgressPtr   a_spSimpleProgress
                              );
private:
    static void InsertTTempDB(int nType, KTarget* a_pTarget, std::map<KODKey, TDistLinksInfo>& mapKeyDist);

public:
    static void TimeCost(KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress);
private:
    static void DirectTimeCost(KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress,
                                                 std::map<Integer, std::set<int>>& mapAvailableItem,
                                                 std::map<Integer, std::set<int>>& mapTrafficMean);
    
    static void FDistributionTimeCost(KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress,
                                                        std::map<Integer, std::set<int>>& mapAvailableItem,
                                                        std::map<Integer, std::set<int>>& mapTrafficMean, Integer nxOriginID, Integer nxDestinationID, 
                                                        std::vector<std::map<KODKey, TRoadViaPathInfo>>& vecFindRoadViaPathItem);

    static void RailTimeCost         (KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress,
                                                        std::map<Integer, std::set<int>>& mapAvailableItem,
                                                        std::map<Integer, std::set<int>>& mapTrafficMean, Integer nxOriginID, Integer nxDestinationID);

    static void DistRailTimeCost     (KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress,
                                                        std::map<Integer, std::set<int>>& mapAvailableItem,
                                                        std::map<Integer, std::set<int>>& mapTrafficMean,
                                                        std::vector<std::map<KODKey, TRailPathInfo>>& vecFindRailPathItem);

    static bool IsAvailableMode (Integer nxNodeID, int nMode, std::map<Integer, std::set<int>>& mapTrafficMean );
    static bool IsAvailableItem (Integer nxNodeID, int nItem, std::map<Integer, std::set<int>>& mapAvailableItem);    
public:
    static void QueryTime (KTarget* pTarget, QBicSimpleProgressPtr spSimpleProgress);
    static void QueryTime2(KTarget* pTarget, QBicSimpleProgressPtr spSimpleProgress);

private:

	static void GetExistPathInfo(KTarget* pTarget, std::map<int, std::map<KODKey,TViaTermianlInfo>>& ar_mapPathInfo);

	static void GetNewPathInfo(	std::vector<TItemTimeCost>& vecItemTimeCost,
								std::vector<Integer>& vecZoneNodeID, 
								std::vector<std::map<KODKey, TRoadViaPathInfo>>& vecFindRoadViaPathItem,
								std::vector<std::map<KODKey, TRailPathInfo>>& vecFindRailPathItem, 
								std::map<int, std::map<KODKey,TViaTermianlInfo>>& ar_mapPathInfo);

    static void CreateTTablePathInfo( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, QBicSimpleProgressPtr spSimpleProgress );
	static void CreatePathInfoTable( KTarget* pTarget, std::map<int, std::map<KODKey,TViaTermianlInfo>>& ar_mapPathInfo, QBicSimpleProgressPtr spSimpleProgress );

	static void InsertTTablePathInfo( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, QBicSimpleProgressPtr spSimpleProgress, 
                                      std::vector<Integer>& vecZoneNodeID,
                                      std::vector<std::map<KODKey, TRoadViaPathInfo>>& vecFindRoadViaPathItem,
                                      std::vector<std::map<KODKey, TRailPathInfo>>& vecFindRailPathItem );

	static void InsertPathInfoTable( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, QBicSimpleProgressPtr spSimpleProgress, 
										std::vector<Integer>& vecZoneNodeID, std::map<int, std::map<KODKey,TViaTermianlInfo>>& ar_mapPathInfo );


    static void WriteTTablePathInfo( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, QBicSimpleProgressPtr spSimpleProgress, 
                                      std::vector<Integer>& vecZoneNodeID,
                                      std::vector<std::map<KODKey, TRoadViaPathInfo>>& vecFindRoadViaPathItem,
                                      std::vector<std::map<KODKey, TRailPathInfo>>& vecFindRailPathItem );

public:
    static void FreightLinkCollection   (KTarget* pTarget,   KLinkCollection& oRoadLinkCollection, KLinkCollection& oRailLinkCollection, QBicSimpleProgressPtr spSimpleProgress);
    static void BulkPathLinks(KTarget* a_pTarget, KLinkCollection& oRoadLinkCollection, 
                              Integer  nxStartID, std::set<Integer>& setDestination, 
                              std::set<Integer>& setAppendPathLinks, QBicSimpleProgressPtr a_spSimpleProgress);
	
public:
	static void GetTableO2TD(KTarget* pTarget, Integer a_nxOZoneID, Integer a_nxDZoneID, std::set<Integer>& ar_setLinkID, QBicSimpleProgressPtr a_spSimpleProgress);
	static void GetTableR2R(KTarget* pTarget, Integer a_nxOZoneID, Integer a_nxDZoneID, std::set<Integer>& ar_setLinkID, QBicSimpleProgressPtr a_spSimpleProgress);
	static void GetTableT2TD(KTarget* pTarget, Integer a_nxOZoneID, Integer a_nxDZoneID, std::set<Integer>& ar_setLinkID, QBicSimpleProgressPtr a_spSimpleProgress);

};