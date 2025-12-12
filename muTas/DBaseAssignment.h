#pragma once
//^#include "DBaseConnector.h"
//^^ #include "IOColumns.h"
#include "Mode.h"
#include "ARunningOptionInfo.h"
#include "DBaseDefaultInterRunOption.h"
/* Forward declarations */
class KTarget;
class KSEDData;
class KAInputInfo;

/**
* @brief TAssignResultLinkColumnName Entity
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.29
*/

struct TAssignResultColumnName
{
	//link
	CString AverageTime;
	CString VC;
	CString SumVolume;
	CString Trip;
	CString AvgTimeToll;
	CString BusInitialVolumn;
	std::vector<CString> ModeVolumn;
	std::vector<CString> VDFTime;
	std::vector<CString> VDFTollTime;

	//od parameter
	CString ODAvgTime;
	CString ODAvgBusTime;
	CString ODAvgSubwayTime;
	CString ODAVgBusSubwayTime;
	CString ODBusDirect;
	CString ODBusOneTransfer;
	CString ODBustwoTransfer;
	std::vector<CString> ODVDFTime;

	//od Mode
	CString ODMBus;
	CString ODMRail;
	CString ODMBusRail;
	std::vector<CString> InterMode;

	//lineinfo
	CString LinfoPassenger;
	CString LinfoTotalLength;
	CString LinfoAvgLength;
	CString LinfoMaxPassenger;
	CString LinfoAvePassenger;
	CString LinfoMaxSecond;
	CString LinfoAvgSecond;
	CString LinfoMaxStanding;
	CString LinfoAvgStanding;
	CString LinfoMaxCongestion;
	CString LinfoAvgCongestion;
	CString LinfoKMPassenger;
	CString LinfoOncePassenger;
	CString LinfoKMOncePassenger;

	//line
	CString LineBoardNet;
	CString LineBoardTransfer;
	CString LineBoardTotal;
	CString LineAlightNet;
	CString LineAlightTransfer;
	CString LineAlightTotal;
	CString LinePassenger;

	//Node
	CString NodeBusNetBorad;
	CString NodeBusTransferBoard;
	CString NodeBusSumBoard;
	CString NodeBusNetAlight;
	CString NodeBusTransferAlight;
	CString NodeBusSumAlight;

	CString NodePRAuto;
	CString NodePRBus;
	CString NodePRRail;
	CString NodePRTransit;

	//Terminal
	CString TerminalTrip;

	//Termianl_Resutl
	CString TR_AC_Auto;
	CString TR_AC_Taxi;
	CString TR_AC_Bus;
	CString TR_AC_Sub;
	CString TR_AC_Transit;
	CString TR_EG_Auto;
	CString TR_EG_Taxi;
	CString TR_EG_Bus;
	CString TR_EG_Sub;
	CString TR_EG_Transit;
	CString TR_Total;

};

struct TAssignResultInfo
{
	//info
	bool	TbUseData;
	bool	TbUseOverWrite;
	int     TnRunSeq;
	CString TstrStartDate;
	CString TstrEndDate;
	int		TnType;
	CString TstrDescription;

	//link
	CString TstrAverageTime;
	CString TstrVC;
	CString TstrSumVolumn;
	CString TstrTrip;
	CString TstrModeVolume;//
	CString TstrVDFTime;//
	CString TstrAVETimeToll;
	CString TstrVDFTimeToll;//
	CString TstrBusInitVolume;

	//od paramter
	CString TstrODAveTime;
	CString TstrODVDFTime;//
	CString TstrODAveBus;
	CString TstrODAveSubway;
	CString TstrODAveBusSubway;
	CString TstrODBusDirect;
	CString TstrODBusOneTrans;
	CString TstrODBusTwoTrans;

	//od
	CString TstrODMBus;
	CString TstrODMRail;
	CString TstrODMBusRail;
	CString TstrODMInterMode;

	//lineInfo
	CString TstrLInfoPassengernum;
	CString TstrLinfoTotalLength;
	CString TstrLinfoAvgLength;
	CString TstrLinfoMaxPassenger;
	CString LinfoAvePassenger;
	CString TstrLinfoMaxSecond;
	CString TstrLinfoAvgSecond;
	CString TstrLinfoMaxStanding;
	CString TstrLinfoAvgStanding;
	CString TstrLinfoMaxCongestion;
	CString TstrLinfoAvgCongestion;
	CString TstrLinfoKMPassenger;
	CString TstrLinfoOncePassenger;
	CString TstrLinfoKMOncePassenger;
	
	//line
	CString TstrLineBoradNet;
	CString TstrLineBoradTransfer;
	CString TstrLineBoradTotal;
	CString TstrLineAlightNet;
	CString TstrLineAlightTransfer;
	CString TstrLineAlightTotal;
	CString TstrLinePassenger;

	//Node
	CString TstrNodeBusNetBorad;
	CString TstrNodeBusTransferBoard;
	CString TstrNodeBusSumBoard;
	CString TstrNodeBusNetAlight;
	CString TstrNodeBusTransferAlight;
	CString TstrNodeBusSumAlight;

	CString TstrNodePRAuto;
	CString TstrNodePRBus;
	CString TstrNodePRRail;
	CString TstrNodePRTransit;
	
	//Terminal
	CString TstrTerminalTrip;

	//Terminal Result
	CString TstrTR_AC_Auto;
	CString TstrTR_AC_Taxi;
	CString TstrTR_AC_Bus;
	CString TstrTR_AC_Subway;
	CString TstrTR_AC_Transit;
	CString TstrTR_ER_Auto;
	CString TstrTR_ER_Taxi;
	CString TstrTR_ER_Bus;
	CString TstrTR_ER_Subway;
	CString TstrTR_ER_Transit;
	CString TstrTR_Total;

};

struct TMAFnCoefficient
{
	int	   TnMode;

	double TdInVehcle;
	double TdWaitingTime;
	double TdTransferTime;
	double TdAccessWalkTime;
	double TdEgressWalkTime;
	double TdNoOfTransfer;
	double TdCost;
	double TdNoOfStop;
};

struct TMATransitOptionsBaseInfo
{
	int TnMaxofNo;
	double TdDWTimeInBus;
	double TdAlpha;
	double TdBeta;
	int TnUsingBusSubway;
	double TdDWTimeInRail;
	double TdPathTime;
	int	   TnInvehicleTime;
	int    TnWatingTime;
	int	   TnTransferTime;
	int	   TnAETime;
	double TdPathLength;
	double TdMEU;
};

struct TMATransitCostInfo
{
	int TnMode;
	int TnType;
	CString TstrTypeName;
	double TdBasicRate;
	double TdBasicDistance;
	double TdAddDistance;
	double TdAddRate;
	int	   TnMergeRate;

};
#pragma pack(push, 1)
struct TMALInkInfoSpeed
{
	Integer TnxLink_ID;
	Integer TnxFNode_ID;
	Integer TnxTNode_ID;
	int		TnLinkType;
	double	TdLength;
	double  TdSpeed;
	double  TdAddField;
};
#pragma pack(pop)

#pragma pack(push, 1) 
struct TMALinkInfo
{
	Integer TnxLink_ID;
	Integer TnxFNode_ID;
	Integer TnxTNode_ID;
	int		TnLinkType;
	double	TdLength;
	double  TdAddField;
};
#pragma pack(pop)

#pragma pack(push, 1) 
struct TMATransitLine
{
	Integer TnxLineID;
	int		TnSeq;
	double	TdBoardNet;
	double	TdBoardTransfer;
	double	TdBoardTotal;
	double	TdAllightNet;
	double	TdAllightTransfer;
	double	TdAllightTotal;
	double  TdPassenger;
};
#pragma pack(pop)

#pragma pack(push, 1) 
struct TIMTerminalInfo
{
	Integer TnxOZoneID;
	Integer TnxDZoneID;
	int		TnType;
	double	TdValue;
};
#pragma pack(pop)

struct TModeChoiceColumnInfo
{
	int		TnModeGroupCode;
	int		TnDetailmodeID;
	int		TnRunSeq;
	CString TstrColumnName;
	CString TstrStartDate;
	CString TstrEndDate;
	CString TstrDescription;
};


struct TGenerationPathInfo
{
	KIOColumn* pColumn;
	KMode*	   pMode;
	CString    strModeName;
	int        nBus;		// 0 : NO , 1:Yes
	int        nSubway;		// 0 : NO , 1:Yes
	int        nExclusive;  // 0 : NO , 1:Yes
};

struct TAssignPathInput
{
	int     nModeID;
	double  dOccupancy;
	double  dPCU;
	int     nNetworkType;
	int     nPreload;
	CString strOutputColumnName;
	CString strInputColumnName;
};

#pragma pack(push, 1) 
struct TInterSectionType
{
	Integer nxNodeID;
	int     nType;
	int     nTopoloy;
};

struct TInterSectionBoundTurnType
{
	Integer nxNodeID;
	int		n1Lt;
	int		n1Rt;
	int		n2Lt;
	int		n2Rt;
	int		n3Lt;
	int		n3Rt;
	int		n4Lt;
	int		n4Rt;
	int		n1SigType;
	int		n2SigType;
	int		n3SigType;
	int		n4SigType;
};

struct TInterSectionPhaseOper
{
	Integer nxNodeID;
	int		nPhaseNo;
	int		nPhaseType1;
	int		nPhaseType2;
	int		nPhaseType3;
	int		nPhaseType4;
	int		nPhaseType5;
	int		nPhaseType6;
	int		nPhaseTime1;
	int		nPhaseTime2;
	int		nPhaseTime3;
	int		nPhaseTime4;
	int		nPhaseTime5;
	int		nPhaseTime6;
};

struct TInterSectionMovement
{
	Integer nxFromNode;
	Integer nxAtNode;
	Integer nxToNode;
	int		nTurnIndex;
};

struct TInterModeODInfo
{
	int nModeID;
	CString strModeName;
	CString strModeColumn;
};

struct TInterRunOptionInfo
{
	int nUseBusIV;
	int nUseRegionalBusIV;
	int nMinLine;
	int nUseFreightIV;
	std::map<int, KInterOccupancePCEMode> mapInterFOPMode;
};


#pragma pack(pop)

class KDBaseAssignment
{
public:
	KDBaseAssignment(void);
	~KDBaseAssignment(void);

public:
	//수단 결과 정보 확인
	static bool ModeChoiceExist(KTarget* a_pTarget);

	//BPRFnName 확인
	static bool BPRFnNameExist(KDBaseConPtr spDBaseConnection, CString a_strFnName);

	//결과 저장 SEQ
	static int	GetNewAssignRunSeq(KTarget* a_pTarget);

	//AssignRunSeq 업데이트
	static void UpdateAssignRunSeq(KDBaseConPtr spDBaseConnection, int runSeq, 
			TAssignResultColumnName& resultLinkColumnName, CString strStartDate, CString strEndDate,
			CString strDescription, int nType);

	//OBsingleSeq 결과 업데이트
	static void UpdateOBSingleARunSeq(KDBaseConPtr spDBaseConnection, int runSeq, 
		TAssignResultColumnName& resultLinkColumnName, CString strStartDate, CString strEndDate,
		CString strDescription, int nType);

	//OBMultiSeq 결과 업데이트
	static void UpdateOBMultiARunSeq(KDBaseConPtr spDBaseConnection, int runSeq, 
		TAssignResultColumnName& resultLinkColumnName, CString strStartDate, CString strEndDate,
		CString strDescription, int nType);

	//MultiModal 결과 업데이트
	static void UpdateMuliModalRunSeq(KDBaseConPtr spDBaseConnection, int nRunSeq, 
		TAssignResultColumnName& resultColumnName, CString strStartDate, CString strEndDate,
		CString strDescription, int nModeType);

	static void UpdateAssignPathmodel(KDBaseConPtr spDBaseConnection, int runSeq, int nRunOption, int nZoneThrow,
		CString strAveTime, CString strStartDate, CString strEndDate);


	static void UpdateAssignpathInfo(KDBaseConPtr spDBaseConnection, std::list<KAInputInfo*> a_lstAInfo,
		TAssignResultColumnName& resultLinkColumnName);

	static void DeleteAssignPathInfo(KDBaseConPtr spDBaseConnection);

	static void UpdateAssignTurnVolumeInfo(KDBaseConPtr spDBaseConnection, std::list<KAInputInfo*> a_lstAinfo, KARunningOptionInfo a_oRuningOption);

	static void DeleteAssignTurnVolumnInfo(KDBaseConPtr spDBaseConnection);

	static bool GetSumVolumeColumns( KTarget* pTarget, CStringArray& columnsNameArray);

	//TransitData 유무 확인
	static bool TransitDataExist(KTarget* a_pTarget);
	
	// Highway Assign 결과값 정보 저장
	static void GetRunHighwayAssignInfo(KTarget* a_pTarget, std::vector<TAssignResultInfo>& ar_vecAssignResultInfo);

	//수단결과정보 저장
	static void GetModeChoiceModelinfo(KTarget* a_pTarget, std::map<CString, int>& a_mapModeChoice);

	//LinkTravelTime 가져오기
	static void GetLinkTravelTimeField(KTarget* a_pTarget, std::set<CString>& a_lstTraveTimeField);

	//TravleTime 유무 확인
	static bool TravelTimeExist(KTarget* a_pTarget);

	//기존 Multimodal Assign 정보 가져오기
	static void GetMulitimodalAssignResultInfo(KTarget* a_pTarget, std::vector<TAssignResultInfo>& a_vecMAResultInfo);

	//NoClass 가져오기
	static void GetNoClass(KTarget* a_pTarget, std::map<CString, double>& a_mapNoClass);

	//UtilitFnCoffcient 가져오기
	static void GetUtilitFnCoffcient(KTarget* a_pTarget, std::vector<TMAFnCoefficient>& a_mapFnCoeffcient);

	//OptionCost 가져오기
	static void GetOptionCost(KTarget* a_pTarget, std::vector<TMATransitCostInfo>& a_mapTransitCost);

	//Mode 별 ModeChoicd컬럼 가져오기
	static void GetModeChoiceODAtGroupingMode(KTarget* a_pTarget, std::map<int,std::vector<CString>>& ar_mapModeColumnInfo);

	static void GetDistributionODAtGroupingPurpose(KTarget* a_pTarget, std::map<int,std::vector<CString>>& ar_mapPurposeColumnInfo);

	//Mode컬럼 가져오기
	static void GetModeChoiceColumn(KTarget* a_pTarget, KMode* a_pMode, std::vector<TModeChoiceColumnInfo>& ar_vecModeColumnInfo);

	static void GetInitBusPathValue(KTarget* a_pTarget,TMATransitOptionsBaseInfo& a_oBaseInfo );

	static bool ExportDB2NodeFileExcludeNodeOff(KDBaseConPtr a_spDBaseConnection, CString strFileName=_T("node.dat"));

	static bool ExportDB2ODFile(KDBaseConPtr a_spDBaseConnection, CString strTableName, KIOColumns& userColumnCollection, CString strFileName);

	static bool ExportDB2LinkFileWithOutHighway(KDBaseConPtr a_spDBaseConnection, int nUseLinkTime, KIOColumn* a_pColumn, CString strFileName = _T("link.dat"));

	static bool GetHighwayTypeData(KTarget* a_pTarget, std::set<int>& a_setIncludeNodeType, std::set<int>& a_setIncludeLinkType);

	static bool GetAssignmentTypeData(KTarget* a_pTarget, std::set<int>& a_setIncludeNodeType, std::set<int>& a_setIncludeLinkType);

	static bool GetHighwayAssignmentTypeData( KTarget* a_pTarget, std::set<int>& a_setIncludeNodeType, std::set<int>& a_setIncludeLinkType );

	static bool GetAOBATypeData(KTarget* a_pTarget, std::set<int>& a_setIncludeNodeType, std::set<int>& a_setIncludeLinkType);

	static bool GetTransitGeneratorMode(KTarget* a_pTarget, std::list<TGenerationPathInfo>& a_lstGenerationPathInfo);

	static bool DeleteOverWriteColumn(KTarget* a_pTarget, TAssignResultInfo& a_AssignResultInfo);
	static bool DeleteOverWriteColumn(KTarget* a_pTarget, TAssignResultInfo* a_AssignResultInfo);
	
	static void DeleteUpdateInterModalRunSeq( KDBaseConPtr spDBaseConnection, int nRunSeq, TAssignResultColumnName& resultColumnName, CString strStartDate, CString strEndDate, CString strDescription, int nModeType );
	
	static void GetInterModalResultInfo( KTarget* a_pTarget, TAssignResultInfo& a_oInterResultInfo );

	static void GetAssignPathInputInfo(KTarget* a_pTarget, std::vector<TAssignPathInput>& ar_vecAssignPathInputInfo);

	//Export DB2 File InterSection

	static bool InterSectionTypeDB2File(KDBaseConPtr a_spDBaseConnection, CString strFileName);

	static bool InterSectionMovementDB2File(KDBaseConPtr a_spDBaseConnection, CString strFileName);

	static bool InterSectionBoundTurnTypeDB2File(KDBaseConPtr a_spDBaseConnection, CString strFileName);

	static bool InterSectionPhaseOperDB2File(KDBaseConPtr a_spDBaseConnection, CString strFileName);
	
	static void GetInterDefineCost( KTarget* a_pTarget, std::vector<TMATransitCostInfo>& a_mapTransitCost );

	static void GetFModeChoiceODAtGroupingMode(KTarget* a_pTarget, std::map<int,std::vector<CString>>& ar_mapModeColumnInfo);

	static void GetZoneID(KTarget* a_pTarget, std::set<Integer>& ar_setZoneID);

	static void UpdateInterModeODInfo(KDBaseConPtr a_spDBaseConnection, std::vector<TInterModeODInfo> a_vecModeInfo);
	
};

