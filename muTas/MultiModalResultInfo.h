#pragma once
#include "afxwin.h"
#include "Target.h"
#include "TDrawNodeInfo.h"
#include "TDrawLinkInfo.h"
#include "TUserGeometryInfo.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseTransit.h"

class KTarget;

#pragma pack(push, 1) 

enum KEMPathType
{
	KEMPathTypeNull    = 0,
	KEMPathTypeBus     = 1,
	KEMPathTypeRail    = 2,
};

enum KEMTransitAssignMode
{
	KEMTransitAssignBus     = 1,
	KEMTransitAssignRail    = 2,
	KEMTransitAssignBusRail = 3
};


enum KEMTransitAssignCount
{
	BUSPATH_ASSIGNSET_COUNT     = 3,
	BUSRAILPATH_ASSIGNSET_COUNT = 4
};


struct TMResultBusInfo 
{
	Integer     nxTransitID;
	int         nStartStation;
	int         nEndStation;
	double      dTrip;
	KEMPathType emPathType;
	COLORREF    clrSect;
	bool		bDuplicationRail;
};

struct TMResultSettingInfo
{
	bool   bUseOutRange;

	double dMaxTrip;
	double dMinTrip;
	double dMinSize;
	double dMaxSize;

	COLORREF clrBus;
	COLORREF clrRail;
	COLORREF clrWalk;

	COLORREF clrZone;
	COLORREF clrStaion;
	COLORREF clrOutRange;

};

struct TPathColor
{
	COLORREF clrSect;
};

struct TMMapResultInputValue
{
	std::map<KODKey, double>	mapWalkingTrip;
	std::map<KODKey, double>	mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	mapTransferWalkingB2STrip;
	std::map<KODKey, double>	mapTransferWalkingS2STrip;
	std::map<Integer, double>	mapBusLinkTrip;
	std::map<Integer, double>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
	std::set<Integer>           setAccessNodeStation;
};

struct TTransitResultInputValue
{
	std::map<KODKey, TMResultBusInfo*>	mapWalkingTrip;
	std::map<KODKey, TMResultBusInfo*>	mapTransferWalkingB2BTrip;
	std::map<KODKey, TMResultBusInfo*>	mapTransferWalkingB2STrip;
	std::map<KODKey, TMResultBusInfo*>	mapTransferWalkingS2STrip;
	std::map<Integer, TMResultBusInfo*>	mapBusLinkTrip;
	std::map<Integer, TMResultBusInfo*>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginNodeID;
	std::set<Integer>			setDestinationNodeID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
};

struct TMMapResultValue
{
	std::vector<TDrawLinkInfo> vecDrawLinkInfo;
	std::vector<TDrawNodeInfo> vecDrawNodeInfo;
	std::vector<TUserLineInfo> vecUserLineInfo;
	std::vector<TDrawNodeInfo> vecDrawZoneNodeInfo;
};

struct TBusSubData
{
	double dInVehTime;
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dCost;
	int	   nTransferNo;
	int	   nNSL;
	double dAssignTrip;
	double dLength;
};

struct TRailSubData
{
	double dInVehTime;
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dCost;
	int	   nTransferNo;
	double dAssignTrip;
	double dLength;
};

struct TShortPathBusSubData
{
	double dInVehTime;
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dCost;
	int	   nTransferNo;
	double dLength;
};

struct TShortPathRailSubData
{
	double dInVehTime;
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dCost;
	int	   nTransferNo;
	double dLength;
};

struct TEditBusSubData
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
};

struct TEditRailSubData
{
	double dInVehTime;
	double dWaitTime;
	double dTransferTime;
	double dAccessWalkTime;
	double dEgressWalkTime;
	double dCost;
	int	   nTransferNo;
	double dLength;
};

struct TPathSubData
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

class TStationODInfo
{
public:
    TStationODInfo() {
        TNodeID    = 0;
        TNodeName = _T("");
        TStationYN  = 1;
    }
    ~TStationODInfo(){}
public:
	Integer TNodeID;
	CString TNodeName;
	int     TStationYN;
};

struct TIndexResultTransitInfo
{
	KODKey oIndexKey;
	std::vector<TMResultBusInfo> vecLowResultTransitInfo;
};

#pragma pack(pop)


class KControlMessage
{
public:
	KControlMessage::KControlMessage(void){};
	KControlMessage::KControlMessage(UINT a_nID, CString a_strMsg){nID = a_nID;  strMsg = a_strMsg;};
	KControlMessage::~KControlMessage(void){};
public:	
	UINT    nID;
	CString strMsg;
};

class KMBusAssignInfo 
{
public:
	KMBusAssignInfo::KMBusAssignInfo(void){}
	KMBusAssignInfo::~KMBusAssignInfo(void){}

public:
	Integer nxOrginID;
	Integer nxDestinationID;
	int nPathID;
	std::vector<TIndexResultTransitInfo> vecResultBusInfo;
	
	KEMTransitAssignMode emTransitAssignMode;
	TBusSubData  oTBusSubData;
	TRailSubData oTRailSubData;

	TShortPathBusSubData  oTPathBusSubData;
	TShortPathRailSubData oTPathRailSubData;
};


class KMTransitEditInfo 
{
public:
	KMTransitEditInfo::KMTransitEditInfo(void){}
	KMTransitEditInfo::~KMTransitEditInfo(void){}

public:
	Integer nxOrginID;
	Integer nxDestinationID;
	int nPathID;
	
	std::vector<TIndexResultTransitInfo> vecResultBusInfo;

	KEMTransitAssignMode emTransitAssignMode;
	TEditBusSubData  oTBusSubData;
	TEditRailSubData oTRailSubData;

	int nUseFlag;
	size_t szUseFlagDataPoint;
};

class KMultiModalResultInfo
{
public:
	KMultiModalResultInfo(void);
	~KMultiModalResultInfo(void);

	static void ResultBusPathTripInfo(KTarget* pTarget, std::vector<KMBusAssignInfo> &a_vecBusAssinInfo, TMMapResultInputValue& a_oMapResultInput);
	static void ResultBusPathDrawInfo(KTarget* pTarget, TMMapResultInputValue& a_oMMapResultInput, TMResultSettingInfo &a_oTMResltSettingInfo, TMMapResultValue& a_oMapResult, bool a_bLinkRatio=false);
	
	static void ResultBusPathTripInfo2(KTarget* pTarget, std::map<Integer, std::map<int, std::vector<KMBusAssignInfo> *>> &a_mapDestMAssignInfo, std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> &a_mapDestResultInputValue);
	static void ResultBusPathDrawInfo3(KTarget* pTarget, std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> &a_mapDestResultInputValue, std::map<Integer, std::map<int, std::set<int>>> &a_mapZoneModePIndex, TMResultSettingInfo &a_oSettingInfo, TMMapResultValue& a_oMapResult, bool a_bLinkRatio=false);
	
	static void ResultTransitEditInfo(KTarget* pTarget, std::vector<KMTransitEditInfo> &a_vecTransitEditInfo, TMMapResultValue& a_oMapResult);
	static void ResultTransitEditInfo2(KTarget* pTarget, std::map<Integer, std::map<int, std::vector<KMTransitEditInfo>*>> &a_mapDestMAssignInfo, std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> &a_mapDestResultInputValue);
	static void ResultTransitEditInfo3(KTarget* pTarget, std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> &a_mapDestResultInputValue, std::map<Integer, std::map<int, std::set<int>>> &a_mapZoneModePIndex, OUT TMMapResultValue& a_oMapResult);

	static void ResultTransitShortestInfo(KTarget* pTarget, TMResultSettingInfo &a_oSettingInfo, std::vector<KMBusAssignInfo> &a_vecBusAssignInfo, TMMapResultValue& a_oMapResult, COLORREF a_clrWalk);
	static void ResultTransitShortestInfo2(KTarget* pTarget, std::map<int, std::vector<KMBusAssignInfo>*> &a_mapModeMAssignInfo, std::map<int, std::vector<TTransitResultInputValue>> &a_mapModeResultInputValue);
	static void ResultTransitShortestInfo3(KTarget* pTarget, std::map<int, std::vector<TTransitResultInputValue>> &a_mapModeResultInputValue, std::map<int, std::set<int>> &a_mapModePIndex, TMResultSettingInfo &a_oSettingInfo, TMMapResultValue& a_oMapResult, COLORREF a_clrWalk);
	
	static void GetStatoinIDFromSEQ(KTarget* pTarget, int a_nSeq, Integer a_nxLineID, Integer& a_nxStationID);

	static void TransitPathFind(KTarget* pTarget, std::vector<KMBusAssignInfo> & a_vecTransitInfo, TMMapResultValue& a_oMapResult);
	
	static void CalculateValue2Width(double a_dTrip, TMResultSettingInfo a_oSettingInfo, double& dWidth);

	static void GetStationODSQLData(KTarget* a_pTarget, Integer a_nxTransitID, std::map<int, TStationODInfo>& a_mapStationODData);

public:
    static void Node2XYCoordinate(KTarget* pTarget, KODKey oODKey, std::vector<TMapPoint>& a_vecMapPoint);
private:
	static void NewGetStatoinIDFromSEQ(std::map<Integer, std::vector<Integer>> &a_mapLineSeq, std::map<Integer, TLinkFTNodeID> &a_mapLinkFTNode, int a_nSeq, Integer a_nxTransitID, Integer& a_nxStationID);

//★ 2016-10-10 : nombara
//★ 명지대 모듈의 시퀀스 정보 -> 링크 정보로 변경하는 방법을 변경함
private:
    static void DllTransitSeqToLinkTrip(std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TMResultBusInfo oResultInfo, std::map<Integer, double>& mapLinkTrip);
    //     아래 함수를 통합
    //     -> static void NewGetTransitInfo(std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TMResultBusInfo a_oResultInfo, std::map<Integer, double>& a_mapLinkTrip);
    //     -> static void GetReverseTransitInfo(std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TMResultBusInfo a_oResultInfo, std::map<Integer, double>& a_mapLinkTrip, std::map<Integer, TLinkFTNodeID> a_mapFTNode);    

    static void DllTransitSeqToLinkClr(std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TMResultBusInfo oResultInfo, std::map<Integer, TPathColor>& mapLinkPathClr);
    //     아래 함수를 통합
    //     -> static void GetTransitShortestInfo(std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TMResultBusInfo oResultInfo, std::map<Integer, TPathColor>& mapLinkPathClr);
    //     -> static void GetReverseTransitShortestInfo( std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TMResultBusInfo a_oResultInfo, std::map<Integer, TPathColor>& mapLinkPathClr, std::map<Integer, TLinkFTNodeID> a_mapFTNode);

	static void DllTransitSeqToLinkBusInfoPtr(std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TMResultBusInfo &oResultInfo, std::map<Integer, TMResultBusInfo*>& mapLinkPathClr);
	// 색상을 TMResultBusInfo 내부에서 받아오기 위한, 주소정보를 저장한다.
};
