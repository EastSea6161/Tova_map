#pragma once

#include "afxwin.h"
#include "Target.h"
#include "TDrawNodeInfo.h"
#include "TDrawLinkInfo.h"
#include "TUserGeometryInfo.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseNode.h"

class KTarget;

#pragma pack(push, 1) 

enum KEMInPathType
{
	KEMInPathTypeNull    = 0,
	KEMInPathTypeBus     = 1,
	KEMInPathTypeRail    = 2,
};

enum KEMInterMode
{
// 	KEMInterModeBus         = 1,
// 	KEMInterModeRail        = 2,
// 	KEMInterModeBusRail     = 3,
// 	KEMInterModeParkRide    = 4,
// 	KEMInterModeKissRide    = 5,
// 	KEMInterModeTaxiTransit = 6,
// 	KEMInterModeAutoTaxi    = 7

	KEMInterModeAutoTaxi    = 1,
	KEMInterModeBus         = 2,
	KEMInterModeRail        = 3,
	KEMInterModeBusRail     = 4,
	KEMInterModeParkRide    = 5,
	KEMInterModeKissRide    = 6,
	KEMInterModeTaxiTransit = 7
};

enum KEMInterModalPathMode
{
	KEMInterModalPathModeUrban    = 1,
	KEMInterModalPathModeRegional = 2
};

enum KEMInterAccEgrType
{
	KEMInterAccEgrTypeAccess = 1,
	KEMInterAccEgrTypeEgress = 2
};

enum KEMInterRegDetailMode
{
	KEMInterRegDetailModeAuto    = 0,
	KEMInterRegDetailModeBus     = 1,
	KEMInterRegDetailModeRail    = 2,
	KEMInterRegDetailModeBusRail = 3
};

enum KEMInterAssignCount
{
	BUSPATH_INTER_ASSIGNSET_COUNT     = 3,
	BUSRAILPATH_INTER_ASSIGNSET_COUNT = 4
};

enum KEMInterPathAreaMode
{
	KEMInterPathAreaModeAuto = 1,
	KEMInterPathAreaModeTaxi = 2,
	KEMInterPathAreaModeBus  = 3,
	KEMInterPathAreaModeRail = 4
};

enum KEMInterPathTerminalMode
{
	KEMInterPathTerminalModeAuto    = 1,
	KEMInterPathTerminalModeTaxi    = 2,
	KEMInterPathTerminalModeBus     = 3,
	KEMInterPathTerminalModeRail    = 4,
	KEMInterPathTerminalModeTransit = 5
};


const int URBAN_MODE_AUTO   = 1;
const int URBAN_MODE_TAXI   = 2;
const int URBAN_MODE_BUS    = 3;
const int URBAN_MODE_SUBWAY = 4;
const int URBAN_MODE_WALK   = 5;

const int REGION_MODE_CODE_AUTO = 0;

const int PARKRIDEPATH_ASSIGNSET_COUNT     = 2;
const int KISSRIDEPATH_ASSIGNSET_COUNT     = 4;
const int TAXITRANSITPATH_ASSIGNSET_COUNT  = 4;

const int REG_BUSPATH_ASSIGNSET_COUNT      = 2;
const int REG_BUSRAILPATH_ASSIGNSET_COUNT  = 3;

struct TInAreaComboModeInfo
{
	KEMInterModalPathMode emInterModalPathMode;
	int nModeID;
};

struct TInResultDetailActiveInfo
{
	CString strContentName;
	double  dValue;
};

struct TInResultTransitInfo 
{
	Integer       nxTransitID;
	int           nStartStation;
	int           nEndStation;
	double        dTrip;
	KEMInPathType emPathType;
	COLORREF      clrSect;
	bool		  bDuplicationRail;
};

struct TInMapResultInputValue
{
	std::map<KODKey, double>	mapWalkingTrip;
	std::map<KODKey, double>    mapAutoTrip;
	std::map<KODKey, double>	mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	mapTransferWalkingB2STrip;
	std::map<KODKey, double>	mapTransferWalkingS2STrip;
	std::map<KODKey, double>	mapTransferWalkingA2TTrip;
	std::map<KODKey, double>	mapTerminalConnetWalkingTrip;
	std::map<int/*regionalmode*/, std::map<KODKey, double>>      mapRegionalModeMainTrip;

	std::map<Integer, double>	mapBusLinkTrip;
	std::map<Integer, double>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
	std::set<Integer>			setTerminalID;
	std::set<Integer>			setTransTerminalID;
	std::set<Integer>           setAccessNodeStation;
};

struct TInMapResultValue
{
	std::vector<TDrawLinkInfo>  vecDrawLinkInfo;
	std::vector<TDrawNodeInfo>  vecDrawNodeInfo;
	std::vector<TUserLineInfo>  vecUserLineInfo;
	std::vector<TDrawNodeInfo>  vecDrawZoneNodeInfo;
	std::vector<TUserPointInfo> vecUserPointInfo;
};

struct TInEditBusSubData
{
	double dInVehTime;
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dCost;
	int	   nTransferNo;
	int	   nNSL;
	double dLength;
	double dTrip;
};

struct TInEditRailSubData
{
	double dInVehTime;
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dCost;
	int	   nTransferNo;
	double dLength;
	double dTrip;
};

struct TInEditMultiSubData
{
	double dVehTime;	// 승용차/택시 통행시간
	double dVehCost;	
	double dTravelTime;	// 차내통행시간
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dTransitCost;
	int	   nTransferNo;
	double dLength;
	int    nMode;
	double dTrip;
};

struct TInEditAutoSubData
{
	double dTravelLength;
	double dTravelTime;
	double dAutoCost;
	double dTaxiCost;
	double dTrip;
};

struct TRegAutoSubData
{
	double dAutoTime;
	double dAutoCost;
	double dTrip;
};

struct TInPathSubData
{
	double dInVehTime;
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dCost;
	double dLength;
	double dAssignTrip;
};

struct TInIndexResultTransitInfo
{
	KODKey oIndexKey;
	std::vector<TInResultTransitInfo> vecLowResultTransitInfo;
};

// Area to Area Mode Analysis
struct TInModePathResultSettingInfo
{
	bool   bUseOutRange;

	double dMaxTrip;
	double dMinTrip;
	double dMinSize;
	double dMaxSize;

	COLORREF clrAuto;
	COLORREF clrTaxi;
	COLORREF clrBus;
	COLORREF clrRail;

	COLORREF clrZone;
	COLORREF clrOutRange;
};
struct TInModePathResultInputValue
{
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::map<int, double>       mapModeSumVolume;
	
	std::vector<TMapPoint>      vecAvgSelectedPoints;
};

class TMainPathInfo
{
public:
	TMainPathInfo::TMainPathInfo(){}
	TMainPathInfo::TMainPathInfo(int a_nMode, CString a_strModeName, Integer a_nxOrgTermID, Integer a_nxTransTermID, Integer a_nxDesTermID, 
								double  a_dMainTime, double  a_dMainCost, double  a_dMainLength, double  a_dTrip, Integer a_nxMasterOZoneID, Integer a_nxMasterDZoneID, COLORREF a_clrMode)
	{
		nMode          = a_nMode; 
		strModeName    = a_strModeName;
		nxOrgTermID    = a_nxOrgTermID;
		nxTransTermID  = a_nxTransTermID;
		nxDesTermID    = a_nxDesTermID;


		dMainTime      = a_dMainTime;
		dMainCost      = a_dMainCost;
		dMainLength    = a_dMainLength;
		dTrip          = a_dTrip;

		nxMasterOZoneID = a_nxMasterOZoneID;
		nxMasterDZoneID = a_nxMasterDZoneID;

		clrMode        = a_clrMode; 
	}
	TMainPathInfo::~TMainPathInfo(){}
public:
	int     nMode; // 1: Auto // 2~ : (air,ktx....
	CString strModeName;
	Integer nxOrgTermID;
	Integer nxTransTermID;
	Integer nxDesTermID;

	double  dMainTime;
	double  dMainCost;
	double  dMainLength;
	double  dTrip;

	Integer nxMasterOZoneID;
	Integer nxMasterDZoneID;

	COLORREF    clrMode;
};

class TInterPathColor
{
public:
	TInterPathColor::TInterPathColor(){}
	TInterPathColor::TInterPathColor(CString a_strName, COLORREF a_clrPath)
	{
		strName  = a_strName;
		clrPath  = a_clrPath;
	}
	TInterPathColor::~TInterPathColor(){}
public:
	CString  strName;
	COLORREF clrPath;
};

struct TInResultSettingInfo
{
	bool   bUseOutRange;

	double dMaxTrip;
	double dMinTrip;
	double dMinSize;
	double dMaxSize;

	std::map<int, TInterPathColor> mapUrbanColor;
	std::map<int, TInterPathColor> mapRegionalColor;

	COLORREF clrOrgZone;
	COLORREF clrDestZone;
	COLORREF clrStaion;
	COLORREF clrOutRange;
};


struct TInTerminalAccEgrSettingInfo
{
	bool   bUseOutRange;

	double dMaxTrip;
	double dMinTrip;
	double dMinSize;
	double dMaxSize;

	std::map<int, TInterPathColor> mapModeColor;

	COLORREF clrZone;
	COLORREF clrOutRange;
	COLORREF clrOrgZone;
	COLORREF clrDestZone;
};

#pragma pack(pop)

class KInterPathEditInfo 
{
public:
	KInterPathEditInfo::KInterPathEditInfo(void){}
	KInterPathEditInfo::~KInterPathEditInfo(void){}

public:
	Integer nxOrginID;
	Integer nxDestinationID;
	int nPathID;

	std::vector<TInIndexResultTransitInfo> vecResulTransitInfo;

	KEMInterMode        emInterMode;

	TInEditBusSubData   oTBusSubData;
	TInEditRailSubData  oTRailSubData;
	TInEditMultiSubData oTMultiSubData;
	TInEditAutoSubData  oTAutoSubData;

	TRegAutoSubData     oTRegAutoSubData;

	Integer nxAutoStartNode;
	Integer nxAutoEndNode;

	int nUseFlag;

	size_t szUseFlagDataPoint;

	KEMInterAccEgrType    emInterAccEgrType;
	KEMInterRegDetailMode emInterRegDetailMode;

	KEMInterModalPathMode emInterModalPathMode;
};

//std::map<dzoneid, std::vector<KRegionModePathInfo>>  m_mapReagionAssign
//std::vector<KRegionModePathInfo> vecResion ---> air, ktx, exbus
class KRegionPathDetail;
class KRegionModePathInfo
{
public:
	KRegionModePathInfo::KRegionModePathInfo(void){}
	KRegionModePathInfo::~KRegionModePathInfo(void){}

public:
	Integer nxOZoneID;
	Integer nxDZoneID;
	CString strModeName;
	int     nMode; // 1: Auto // 2~ : (air,ktx....
	int nPathCount;
	std::vector<KRegionPathDetail> vecOnePathDetail;
};

class KRegionPathDetail
{
public:
	KRegionPathDetail::KRegionPathDetail(void){}
	KRegionPathDetail::~KRegionPathDetail(void){}

public:
	double dMainTime;
	double dMainCost;
	double dMainLength;
	double dTrip;

	double dMainWaitTime;
	double dMainTransferTime;
	double dMainProcessTime;
	double dMainFrequency;

	Integer nxOrgTermID;
	Integer nxTransTermID;
	Integer nxDesTermID;

	std::map<Integer/*O존터미널ID*/, std::vector<KInterPathEditInfo>> mapAccAutoAssign;	
	std::map<Integer/*O존터미널ID*/, std::vector<KInterPathEditInfo>> mapAccTaxiAssign;	
	std::map<Integer/*O존터미널ID*/, std::vector<KInterPathEditInfo>> mapAccBusAssign;
	std::map<Integer/*O존터미널ID*/, std::vector<KInterPathEditInfo>> mapAccRailAssign;
	std::map<Integer/*O존터미널ID*/, std::vector<KInterPathEditInfo>> mapAccBusRailAssign;

	std::map<Integer/*D존ID*/,      std::vector<KInterPathEditInfo>> mapEgrAutoAssign;	
	std::map<Integer/*D존ID*/,      std::vector<KInterPathEditInfo>> mapEgrTaxiAssign;	
	std::map<Integer/*D존ID*/,      std::vector<KInterPathEditInfo>> mapEgrBusAssign;
	std::map<Integer/*D존ID*/,      std::vector<KInterPathEditInfo>> mapEgrRailAssign;
	std::map<Integer/*D존ID*/,      std::vector<KInterPathEditInfo>> mapEgrBusRailAssign;
};


class KInterModalPathInfo
{
public:
	KInterModalPathInfo(void);
	~KInterModalPathInfo(void);

	static void ResultTransitPathDrawInfo3(KTarget* a_pTarget, std::map<DWORD_PTR, TInMapResultInputValue> &a_mapDestResultInputValue, std::set<DWORD_PTR> &a_setInterAssignInfo, 
											std::vector<TMainPathInfo> &a_vecMainPathInfo, TInResultSettingInfo &a_oSettingInfo, 
											OUT TInMapResultValue& a_oMapResult);
	
	static void ResultTransitTripInfo2(KTarget* pTarget, std::set<DWORD_PTR> &a_setInterAssignInfo, std::vector<TMainPathInfo> &a_vecMainPathInfo, 
										OUT std::map<DWORD_PTR, TInMapResultInputValue> &a_mapDestResultInputValue, std::map<int, std::map<KODKey, double>> &a_mapRegionalModeMainTrip);
	
	static void ResultTransitPathDrawInfo(KTarget* a_pTarget, TInMapResultInputValue &a_oInMapResultTripInfo, 
											TInResultSettingInfo &a_oTInResultSettingInfo, TInMapResultValue& a_oMapResult);

	static void ResultTransitTripInfo(KTarget* pTarget, std::vector<KInterPathEditInfo> &a_vecTransitEditInfo, std::vector<TMainPathInfo> &a_vecMainPathInfo,
										TInMapResultInputValue &a_oInMapResultInputValue);

	static void RegionalMainTrip(KODKey oODKey, double a_dTrip, int a_nRegionalMode, std::map<int/*regionalmode*/, std::map<KODKey, double>> &a_mapTestTrip);

	static void ResultTransitEditInfo(KTarget* pTarget, std::vector<KInterPathEditInfo> &a_vecTransitEditInfo, KEMInterModalPathMode a_emInterModalPathMode,
										std::map<int, std::vector<TMainPathInfo>> &a_mapMainPathInfo, TInMapResultValue& a_oMapResult, 
										std::map<int, TInterPathColor> &a_mapUrbanColor);

	static void ResultTransitEditInfo2(KTarget* pTarget, std::set<DWORD_PTR> &a_setMBusAssignInfo, KEMInterModalPathMode a_emInterModalPathMode, 
										OUT std::map<DWORD_PTR, TInMapResultInputValue> &a_mapDestResultInputValue);

	static void ResultTransitEditInfo3(KTarget* pTarget, std::map<DWORD_PTR, TInMapResultInputValue> &a_mapDestResultInputValue, std::set<DWORD_PTR> &a_setMBusAssignInfo, KEMInterModalPathMode a_emInterModalPathMode,
										std::map<int, std::vector<TMainPathInfo>> &a_mapMainPathInfo, TInMapResultValue& a_oMapResult, 
										std::map<int, TInterPathColor> &a_mapUrbanColor);

	static void SetOriginZoneNodeInfo(Integer a_nxZoneID , std::vector<TDrawNodeInfo> &a_vecZoneNodeInfo, COLORREF a_clrOrigin = RGB(255,0,0));

	static void SetDestinationZoneNodeInfo(Integer a_nxZoneID , std::vector<TDrawNodeInfo> &a_vecZoneNodeInfo, COLORREF a_clrDestiantion = RGB(0, 0, 255));

public:
	static void GetStatoinIDFromSEQ(KTarget* pTarget, int a_nSeq, Integer a_nxLineID, Integer& a_nxStationID);
    
private:
	static void NewGetStatoinIDFromSEQ(std::map<Integer, std::vector<Integer>> &a_mapLineSeq, std::map<Integer, TLinkFTNodeID> &a_mapLinkFTNode, int a_nSeq, Integer a_nxTransitID, Integer& a_nxStationID);

	static void CreateRegionalMainTrip(std::vector<TMainPathInfo> &a_vecMainPathInfo, OUT std::set<Integer> &a_setOriginZoneID, std::set<Integer> &a_setDestinationZoneID, std::set<Integer> &a_setTerminalID, std::set<Integer> &a_setTransTerminalID, OUT std::map<int, std::map<KODKey, double>> &a_mapRegionalModeMainTrip);
	//★ 2016-10-10 : nombara
    //★ 명지대 모듈의 시퀀스 정보 -> 링크 정보로 변경하는 방법을 변경함
private:
    static void DllTransitSeqToLinkTrip(std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TInResultTransitInfo oResultInfo, std::map<Integer, double>& mapLinkTrip);
	   //static void NewGetTransitInfo(std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TInResultTransitInfo a_oResultInfo, std::map<Integer, double>& a_mapLinkTrip);
	   //static void GetReverseTransitInfo(std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TInResultTransitInfo a_oResultInfo, std::map<Integer, double>& a_mapLinkTrip, std::map<Integer, TLinkFTNodeID> a_mapFTNode);

	static void Node2XYCoordinate(KTarget* pTarget, KODKey oODKey, std::vector<TMapPoint>& a_vecMapPoint);

public:
	static void CheckParentItem( CXTPReportRecord* pRecord );
	static void UnCheckParentItem( CXTPReportRecord* pRecord );
	static void CheckChildItem( CXTPReportRecord* pRecord, BOOL a_BChecked );

public:
	static void ResultAreaModeDrawInfo(KTarget* a_pTarget, TInModePathResultInputValue &a_oTInModePathResultInputValue, TInModePathResultSettingInfo &a_oTMResltSettingInfo, 
										TInMapResultValue& a_oMapResult, bool a_bRatio=false);

	static void TerminalAccEgrTripInfo(KTarget* a_pTarget, Integer a_nxTerminalID, std::map<int, std::map<KODKey, std::map<int, double>>> &a_mapTypeODModeTrip, 
										TInTerminalAccEgrSettingInfo &a_oTResltSettingInfo, TInMapResultValue& a_oMapResult);

	static void CalculateValue2Width(double a_dTrip, TInModePathResultSettingInfo a_oSettingInfo, double& dWidth );
	static void CalculateValue2Width(double a_dTrip, TInResultSettingInfo a_oSettingInfo, double& dWidth );
	static void CalculateValue2Width(double a_dTrip, TInTerminalAccEgrSettingInfo a_oSettingInfo, double& dWidth );
	static bool PathSumVolDescending( TUserLineInfo left, TUserLineInfo right );

	// 2018.08.01 명지대 요청사항 수정
	static void updateODTripMap(std::map<KODKey, double>& src, std::map<KODKey, double>& dest);
	static void updateLinkTripMap(std::map<Integer, double>& src, std::map<Integer, double>& dest);
	// 2018.08.01 명지대 요청사항 수정 끝
};

