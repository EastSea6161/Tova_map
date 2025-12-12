#pragma once

//#include "StdioFileEx.h"

struct TTransitCost
{
	int nMode;
	int nTType;
	double dTbasicRate;
	double dTbasicDistance;
	double dTAddRate;
	double dTAddDistance;
	int nTMergeRate;
};


struct TParaOutputInfo
{
	bool bUseData;
	CString strMode;
	int nMode;
	
	int nInVehTime;
	CString strInVehTime;
	CString strInVehColumnName;
	
	int nWaitTime;
	CString strWaitTime;
	CString strWaitColumnName;

	int nTransferTime;
	CString strTransferTime;
	CString strTransferColumnName;

	int nAccessTime;
	CString strAccessTime;
	CString strAccessColumnName;
	
	int nEgressTime;
	CString strEgressTime;
	CString strEgrColumnName;
	
	int nCost;
	CString strCost;
	CString strCostColumnName;
	
	int nLength;
	CString strLength;
	CString strLengthColumnName;

	int nNoTransfer;
	CString strNoTransfer;
	CString strNoTransferColumnName;

	int nNoStop;
	CString strNoStop;
	CString strNoStopColumnName;
};

struct TAutoFuel
{
	int		nFuelType;
	CString strFuelName;
	double  dPrice;
	double  dRation;
	double  dFuelEfficiency;
};

struct TTaxiFuel
{
	int		nFuelType;
	CString strFuelName;
	double	dBaseFare;
	double	dBaseDistance;
	double	dDistBaseFare;
	double	dUnitDist;
};

struct TTruckFuel
{
	int		nFuelType;
	CString strFuelName;
	double	dPrice;
	double	dFuelEfficiency;
};

struct TAssignRule
{
	double dDWTimeBusstop;
	double dDwTimeRailStation;
	double dDWTimeTaxi;
	int nInvehicle;
	int nWating;
	int nTransfer;
	int nAETime;
	double dAlpha;
	double dBeta;
	double dFirstPathTime;
	double dSecondePathLength;
	double dInterValueofTime;
	double dHighwayVOT;
	int nUseTurn;
};

struct TParaSelectedMode
{
	bool bAuto;
	bool bTaxi;
	bool bTruck;
	bool bBus;
	bool bRail;
	bool bBus_Rail;
	bool bPark_Ride;
	bool bKiss_Ride;
	bool bTaxi_Transit;
	bool bTransit;
};


class KTarget;

class KDBaseDefineTransitCost
{
public:
	KDBaseDefineTransitCost(void);
	~KDBaseDefineTransitCost(void);

	static void GetTransitCostValue2DB(KTarget* a_pTarget, std::map<int, TTransitCost>& a_mapTransitCost);

	static void GetTransitType2DB(KTarget* a_pTarget, std::vector<int>& a_vecTransitType);

	static void UpdateTransitCost(KTarget* a_pTarget, std::vector<TTransitCost> a_vecTransitCost);

	static void GetTransitModeCostValue2DB(KTarget* a_pTarget, std::vector<TTransitCost>& a_vecTransitCost);

	static bool WriteDefineCostArgument(CStdioFileEx& of, std::vector<TTransitCost>& a_vecTransitCost);

	static void UpdateTransitInfoMode(KTarget* a_pTarget, std::vector<TTransitCost> a_vecTransitCost);

	static void GetGeneratorParameter(KTarget* a_pTarget, TAssignRule& a_oUrbanInfo, TAssignRule& a_oRegionalInfo);

};

