#pragma once
#include "Target.h"
#include "DBaseInterModal.h"

class KTarget;

class KPathTimeSetting
{
public:
	KPathTimeSetting::KPathTimeSetting()
		:dTaxiWaitingTime(0.0), dHighwayTransitTime(0.0), dRailTransitTime(0.0), dInterValueofTime(1.0)
	{
	}
	KPathTimeSetting::~KPathTimeSetting(){}

public:
	double	dTaxiWaitingTime;
	double	dHighwayTransitTime;
	double	dRailTransitTime;
	double  dInterValueofTime;
};

class KDBaseDefaultParaInterPathGenerator
{
public:
	KDBaseDefaultParaInterPathGenerator(void);
	~KDBaseDefaultParaInterPathGenerator(void);

public:
	static void LoadDefaultInterUrbanPathInfo(std::map<int, std::vector<TUrbanPathInfo>>& a_mapUrbanPathInfo);
	static void LoadDefaultInterUrbanAutoCost(std::map<int, std::vector<TInterAutoCost>>& a_mapUrbanAutoCost);
	static void LoadDefaultInterUrbanTransitCost(std::map<int, std::vector<TInterTaxiCost>>& a_mapUrbanTransitCost);
	static void LoadDefaultInterRegionalPathInfo(std::map<int, std::vector<TRegionPathInfo>>& a_mapRegionalPathInfo);
	static void LoadDefaultInterRegionalAutoCost(std::map<int, std::vector<TInterAutoCost>>& a_mapRegionalAutoCost);
	static void LoadDefaultInterRegionalTerminalTime(std::map<int, std::vector<TTerminalProcessTime>>& a_mapRegionalTerminalTime);
	static void LoadDefaultInterRegionalPathSetting(std::map<int, KPathTimeSetting>& a_mapRegionalPathTimeSetting);

	static void SetUrbanModeData(std::map<int, TUrbanPathInfo>& a_mapUrbanPathInfo);
	static void SetAutoCost(std::map<int, TInterAutoCost>& a_mapAutoCost);
	static void SetRegionalModeInfo(KTarget* a_pTarget, std::map<int, CString>& a_mapRegionalModeInfo);
	static void SetRegionalTerminalTime(KTarget* a_pTarget, std::map<int, TTerminalProcessTime>& a_mapRegionalTerminalTime);
};
