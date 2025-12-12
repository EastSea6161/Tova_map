
#include <sys/stat.h>
#include <fstream>
#include <iterator>
#include <iostream>
#include "Target.h"
#include "DefineNames.h"
#include "ImChampMapSelectionReceiver.h"
#include "ImChampIDFind.h"
#include "Purpose.h"


#pragma pack(push, 4) 


struct TTargetRegionNode
{
	Integer TnxZoneID;
	Integer TnxNodeID;
	CString TstrName;
	int     TTRCode;
};

struct TTargetRegionInfo
{
	int nCode;
	CString strCodeName;
	std::map<Integer, TTargetRegionNode> mapNodeTargetInfo;
	std::vector<TCoordinate> vecTargetRegionCoordinate;
};


struct TInterNodeInfo
{
	Integer node_id  ;
	int     node_type;
	double  x        ; 
	double  y        ; 
	Integer zone_id  ; 
	int	    nTRCode  ;
	double  dParkFee ;
};

struct TInterNodeInNameInfo
{
	Integer node_id  ;
	int     node_type;
	CString name;
	double  x        ; 
	double  y        ; 
	Integer zone_id  ; 
	int	    nTRCode  ;
	double  dParkFee ;

};

struct TInterTerminalNodeInfo
{
	Integer node_id  ; 
	int     node_type;
	double  x;
	double  y;
	CString strName;
};

struct TInterTerminalSCInfo
{
	Integer nxSeq;
	Integer nxOriginID;
	Integer nxDestinationID;
	int		nMode;
	double	dTravelTime;
	double	dCost;
	int		nFrequency;
	double  dHeadway;
};

struct TAccessTerminalNodeInfo
{
	Integer nxZoneID;
	int		nType;
	Integer nxTerminalID;
	double	dLength;
};

struct TGlobalTerminalInfo
{
	int nTargetRigion;
	CString strTransit_Complex;
	CString strInter_Region_Rail;
	CString strBus_Terminal;
	CString strAir_Terminal;
	CString strMarine_Port;
};

struct TUrbanPathInfo
{
	int nMode;
	CString strMode;
	int nMaxofPath;
};

struct TRegionPathInfo
{
	int nMode;
	CString strMode;
	int nMaxofPath;
};

struct TTerminalProcessTime
{
	int		nModeID;
	CString strModeName;
	double  dAccess;
	double  dEgress;
	int		nModeTransfer;
};

struct TInterModeDefine
{
	int nModeID;
	CString strModeName;
	double dOccupancy;
	double dPCE;
};

struct TUrbanModeParameter
{
	double dAutoCost;
	double dAutoTime;
	double dAutoParkingFee;
	
	double dTaxiCost;
	double dTaxiTime;
	
	double dTrInvehiTime;
	double dTrWaitingTime;
	double dTrTransferTime;
	double dTrAccessWalk;
	double dTrEgressWalk;
	int nTrTransferNo;
	double dTrCost;

	double dDumAuto;
	double dDumTaxi;
	double dDumBus;
	double dDumSubway;
	double dDumBusSubway;
	double dDumParkRide;
	double dDumKissRide;
	double dDumTaxiTransit;
};

struct TRegionMEUTimeCost
{
	CString strMode;
	int     nCode;
	double dCost;
	double dTime;
};

struct TRegionDummy
{
	CString strMode;
	int     nCode;
	double dCost;
};

struct TRegionalParameter
{
	int		TnModeCode;
	CString TstrModeName;

	double	TdCost;
	double	TdDummy;
	double  TdInvehicleTime;
	double  TdWaitingTime;
	double  TdTransferTime;
	double  TdProcessTime;
	double  TdFrequency;

};

class TMRParamter
{
public:
	TMRParamter::TMRParamter(){}
	TMRParamter::~TMRParamter()
	{}

public:
	double dMEUParameter;
	double dAEParameter;

	std::map<KIOColumn*, TUrbanModeParameter> mapUrbanParameter;
	std::vector<TRegionMEUTimeCost> vecModeCost;
	std::vector<TRegionDummy> vecDummy;
};

struct TUrbanChoiceValues
{
	int		nCofficientCode;
	CString strCofficientName;

	double	dAuto;
	double	dTaxi;
	double	dHighwayTransit;
	double	dRailTransit;
	double	dCompositMode;
	double	dParkAndRide;
	double	dKissAndRide;
	double	dTaxiAndTransit;

	CString strCoefficientDisplay;

	// 2019.04.04 추가
	CString strObjName;		// 목적 명
	CString strObjNameEng;	// 목적 영문명
};


class KUrbanChoiceParameter
{
public:
	KUrbanChoiceParameter::KUrbanChoiceParameter(){}
	KUrbanChoiceParameter::~KUrbanChoiceParameter()
	{}

	KIOColumn* pColumn;
	KPurpose* pPurpose;
	std::map<int, TUrbanChoiceValues> mapModeUrbanChoiceValue;
};

class KRegionalChoiceParameter
{
public:
	KRegionalChoiceParameter::KRegionalChoiceParameter(){}
	KRegionalChoiceParameter::~KRegionalChoiceParameter()
	{

	}
	KIOColumn* pColumn;
	KPurpose* pPurpose;
	std::map<int, TRegionalParameter> mapModeRegionalChoiceValue;
};

struct TInterAutoCost
{
	int nType;
	CString strName;
	double dPrice;
	double dRation;
	double dfule;
};

struct TInterTaxiCost
{
	int nType;
	CString strName;
	double dBasefare;
	double dBaseDist;
	double dAddFare;
	double dAddDist;
};

struct TinterDefineTime
{
	double TaxiTime;
	double BusTime;
	double SubwayTime;
	double alpha;
	double beta;
	double constaint_time;
	double constaint_length;
	int invehicle_time;
	int waiting_time;
	int transfer_time;
	int ae_time;
};


#pragma pack(pop)

#pragma once
class KDBaseInterModal
{
public:
	KDBaseInterModal(void);
	~KDBaseInterModal(void);

public:

	// Target Region Setting
	static void DeleteTargetRegionInNodeTable(KTarget* a_pTarget, int a_nCode);

	static void GetZoneFromTargetRegion(KTarget* a_pTarget, std::map<Integer, TTargetRegionNode>& a_mapTRInfo);

	static int GetCountTargetRegionZeroCode( KTarget* a_pTarget);

	static void UpdateNodeTargetRegion(KTarget* a_pTarget, std::map<Integer, TTargetRegionNode> a_mapTRInfo);
	
	static void GetTargetRegionCode(KTarget* a_pTarget, std::set<int>& a_setTargetRegion);

	static void GetTargetRegionInfo(KTarget* a_pTarget, std::map<int, CString> a_mapTargetRegionCode, std::map<int, TTargetRegionInfo>& a_mapTargetRegionInfo);


	//Terminal Node Setting
	static void GetNetworkTypePairData(KTarget* a_pTarget, std::map<int, int>& a_mapNTPair);

	static void GetUsingNodeType(KTarget* a_pTarget, std::vector<int>& a_vecNodeType);

	static void GetPurposeODData(KTarget* a_pTarget, std::map<CString, int>& a_mapPurposeOD);

	static void InsertAcessTerminalNodeSetting(KTarget* a_pTarget, std::vector<TAccessTerminalNodeInfo> a_vecAccessTNInfo);

	//Terminal Node Editor 

	static void GetZoenNodeInfoforImTasIDFind(KTarget* a_pTarget, std::vector<KIDCaption>& a_vecZoneNode, std::map<Integer, CString>& a_mapZoneNodeData);

	static void GetTerminalDataInZone(KTarget* a_pTarget, Integer a_nxzoneid, int a_nType, std::map<Integer, TAccessTerminalNodeInfo>& a_mapAccessTerminalNodeInfo);

	static void GetTermianlBoundary(KTarget* a_pTarget, std::map<int,double>& a_mapTerminalBounary);

	static void GetAllNodeInfo(KTarget* a_pTarget, std::map<Integer, TInterNodeInNameInfo>& a_mapTerminalSystemcode);

	static void SaveAccessTerminalEditor(KTarget* a_pTarget, Integer a_nxZoneId, int a_nType, std::vector<TAccessTerminalNodeInfo> a_vecTerminalNodeInfo);


	//Terminal Grolbal Terminal Setting;

	static void GetGrolblalTerminalSettingInfo(KTarget* a_pTarget, std::map<int, TGlobalTerminalInfo>& a_mapGrobalInfo);

	static void GetAllNodeTerminalInfo(KTarget* a_pTarget, std::map<Integer, TInterTerminalNodeInfo>& a_mapTerminalNodeInfo);

	static void GetRegionInZoneData(KDBaseConPtr a_spDBaseConnection, int a_nRegionCode, std::vector<Integer>& a_vecRegionInZone);

	static void UpdateGlobalRegionTR2Db(KDBaseConPtr a_spDBaseConnection, int a_nTerminalType, std::vector<Integer>& a_vecZoneID, std::vector<Integer>& a_vecTerminalID, std::map<Integer, TInterTerminalNodeInfo> a_mapTerminalNodeInfo);

	static void SaveGloblaRegionSetting(KDBaseConPtr a_spDBaseConnection, TGlobalTerminalInfo a_oInfo);

	static void DeleteGlobalRegionDB(KDBaseConPtr a_spDBaseConnection);

	static void GetTargetRegionZone(KTarget* a_pTarget, int a_nTargetRegionCode, std::set<Integer>& ar_setTRZone);

	//Inter PathGenerator
	static void GetInterPurposeODData(KTarget* a_pTarget, std::map<CString, int>& a_mapPurposeODData);

	static void GetInterRegionModeDefine(KTarget* a_pTarget, std::map<int, int>& a_mapMatcingTMode);

	static void GetpurposeODAtGrouping(KTarget* a_ptarget, std::map<int, std::vector<CString>>& a_mapPurposeODData);

	//interMainDlg
	static void GetInterUrbanPath(KTarget* a_pTarget, std::vector<TUrbanPathInfo>& a_vecUrbanPathInfo);

	static void GetInterRegionPath(KTarget* a_pTarget, std::vector<TRegionPathInfo>& a_vecRegionPathInfo);

	static void GetInterAutoCost(KTarget* a_pTarget, std::vector<TInterAutoCost>& a_vecAutoCost, int a_nURCode);

	static void GetInterTaxiCost(KTarget* a_pTarget, std::vector<TInterTaxiCost>& a_vecTaxiCost, int a_nURCode);

	static void GetInterTimeDefine(KTarget* a_pTarget, TinterDefineTime& oTimeInfo);

	static void GetTerminalProsessTime(KTarget* a_pTarget, std::vector<TTerminalProcessTime>& a_vecTerminalTime);

	static void GetParaODObject(KTarget* a_pTarget, std::map<CString, int>& a_mapObject);

	//Export DB2 File
	static bool InterExportDB2NodeFile(KDBaseConPtr a_spDBaseConnection, CString strFileName);

	static bool InterExportDB2TerminalScheulFile(KDBaseConPtr a_spDBaseConnection, CString strFileName);

	static bool InterExportDb2AccessTerminalNodeAtZone(KDBaseConPtr a_spDBaseConnection, CString strFileName);

	static bool InterExportDB2PurposeODSum(KDBaseConPtr a_spDBaseConnection, KIOColumns oColumnCollection, CString strFileName);

	static bool ExportDB2TxtXBusRest(KTarget* a_pTarget, CString a_strFileName = _T("XBus_Rest.txt"));
	
	static void GetTerminalHaveSchedule( KTarget* a_pTarget, std::set<Integer>& a_setTerminalHaveSchedule );	

    static void GetUsedRegionCode(KTarget* pTarget, std::map<int, CString>& mapRegionCode);

	// import node
	static void AddTargetRegionCode(KTarget* a_pTarget);
};
