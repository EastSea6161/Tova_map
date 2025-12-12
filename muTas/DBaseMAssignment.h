#pragma once

#include "DBaseConnection.h"
#include "IOColumnCollection.h"

class KTarget;
//
//struct TAssignResultInfo
//{
//	bool	TbUseData;
//	bool	TbUseOverWrite;
//	int     TnRunSeq;
//	CString TstrStartDate;
//	CString TstrEndDate;
//	int		TnType;
//	CString TstrDescription;
//	CString TstrAverageTime;
//	CString TstrVC;
//	CString TstrSumVolumn;
//	CString TstrTrip;
//	CString TstrModeVolume;
//	CString TstrVDFTime;
//	CString TstrAVETimeToll;
//	CString TstrVDFTimeToll;
//	CString TstrBusInitVolume;
//	CString TstrODAveTime;
//	CString TstrODVDFTime;
//	CString TstrODAveBus;
//	CString TstrODAveSubway;
//	CString TstrODBusDirect;
//	CString TstrODBusOneTrans;
//	CString TstrODBusTwoTrans;
//	CString TstrLInfoPassengernum;
//	CString TstrLineBorading;
//	CString TstrLineAlighting;
//	CString TstrLineSecondRide;
//};

struct TMAFnCofficient
{
	int	   TnMode;
	double TdInVehcle;
	double TdWaitingTime;
	double TdTransferTime;
	double TdAccessWalkTime;
	double TdEqressWalkTime;
	double TdNoofTransfer;
	double TdCost;
	double TdNoofStop;
};
//
//struct TMATransitOptionsBaseInfo
//{
//	int TnMaxofNo;
//	int TnTimeBorardingAlighting;
//	double TdAlpha;
//	double TdBeta;
//	int TnSationNodeType;
//	int TnSubwayLinkType;
//	int TnSubwayConnectLinkType;
//};

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

struct TMALinkInfo
{
	Integer TnxLink_ID;
	Integer TnxFNode_ID;
	Integer TnxTNode_ID;
	int		TnLinkType;
	double	TdLength;
	double  TdAddField;
};

class KDBaseMAssignment
{
public:
	KDBaseMAssignment(void);
	~KDBaseMAssignment(void);

public:
	
	static bool ModeChoiceExist(KTarget* a_pTarget);

	static void GetModeChoiceModelinfo(KTarget* a_pTarget, std::map<CString, int>& a_mapModeChoice);

	static void GetLinkTravelTimeField(KTarget* a_pTarget, std::set<CString>& a_lstTraveTimeField);

	static bool TravelTimeExist(KTarget* a_pTarget);

	static int	GetNewAssignRunSeq(KTarget* a_pTarget);

//	static void GetAssignResultInfo(KTarget* a_pTarget, std::vector<TAssignResultInfo>& a_vecMAResultInfo);

	static void GetNoClass(KTarget* a_pTarget, std::map<CString, double>& a_mapNoClass);

	static void GetUtilitFnCoffcient(KTarget* a_pTarget, std::vector<TMAFnCofficient>& a_mapFnCoeffcient);

	static void GetOptionCost(KTarget* a_pTarget, std::vector<TMATransitCostInfo>& a_mapTransitCost);

	/*static void GetInitBusPathValue(KTarget* a_pTarget,TMATransitOptionsBaseInfo& a_oBaseInfo );*/

	static bool ExportDB2NodeFileExcludeNodeOff(sqlite3* pDB, CString strFileName=_T("node.dat"));

	static bool ExportDB2ODFile(sqlite3* pDB, CString strTableName, KIOColumnCollection& userColumnCollection, CString strFileName);

	static bool ExportDB2LinkFileWithOutHighway(sqlite3* pDB, int nUseLinkTime, KIOColumn* a_pColumn, CString strFileName = _T("link.dat"));

};

