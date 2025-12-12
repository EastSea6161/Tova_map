#pragma once
#include "Target.h"
#include "DBaseAssignment.h"

class KModeChoicePara
{
public:
	KModeChoicePara::KModeChoicePara()
		:nVariableCode(0), dHighwayTransit(0.0), dRailTransit(0.0), dCompositeMode(0.0)
	{
	}
	KModeChoicePara::~KModeChoicePara(){}

public:
	int	      nVariableCode;
	double	  dHighwayTransit;
	double	  dRailTransit;
	double    dCompositeMode;
};

class KModeChoiceSetting
{
public:
	KModeChoiceSetting::KModeChoiceSetting()
		:dMEU(0.0), dBoardingTime(0.0), dDwellTime(0.0)
	{
	}
	KModeChoiceSetting::~KModeChoiceSetting(){}

public:
	double	 dMEU;
	double	 dBoardingTime;
	double	 dDwellTime;
};

class KDBaseDefaultParaMultimodalModeChoice
{
public:
	KDBaseDefaultParaMultimodalModeChoice(void);
	~KDBaseDefaultParaMultimodalModeChoice(void);

public:
	static void LoadDefaultModeChoicePara(std::map<int, std::vector<KModeChoicePara>>& a_mapDefaultModeChoicePara);
	static void LoadDefaultModeChoiceSetting(std::map<int, KModeChoiceSetting>& a_mapDefaultModeChoiceSetting);
	static void GetVariableCodeName(std::map<int, CString>& a_mapVariableCodeName);

	//Assign
	static void LoadDefaultTransitRouteChoiceParaBus(std::map<int, TMAFnCoefficient>& a_mapBusCofficient);
	static void LoadDefaultTransitRouteChoiceParaRail(std::map<int, TMAFnCoefficient>& a_mapSubwayCofficient);
	static void LoadDefaultTransitRouteChoiceParaComposite(std::map<int, TMAFnCoefficient>& a_mapCompositCofficient);

	static void SaveCofficient(int a_nVariableCode, double a_dValue, TMAFnCoefficient& a_oFnCofficient);
};
