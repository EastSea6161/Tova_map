#pragma once
#include "Target.h"
#include "DBaseAssignment.h"

class KTransitRouteChoicePara
{
public:
	KTransitRouteChoicePara::KTransitRouteChoicePara()
		:nVariableCode(0), dHighwayTransit(0.0), dRailTransit(0.0), dCompositeMode(0.0)
	{
	}
	KTransitRouteChoicePara::~KTransitRouteChoicePara(){}

public:
	int	      nVariableCode;
	double	  dHighwayTransit;
	double	  dRailTransit;
	double    dCompositeMode;
};

class KTransitRouteChoiceSetting
{
public:
	KTransitRouteChoiceSetting::KTransitRouteChoiceSetting()
		:dBoardingTime(0.0), dDwellTime(0.0)
	{
	}
	KTransitRouteChoiceSetting::~KTransitRouteChoiceSetting(){}

public:
	double	 dBoardingTime;
	double	 dDwellTime;
};

class KDBaseDefaultParaTransitRouteChoice
{
public:
	KDBaseDefaultParaTransitRouteChoice(void);
	~KDBaseDefaultParaTransitRouteChoice(void);

public:
	static void LoadDefaultTransitRouteChoicePara(std::map<int, std::vector<KTransitRouteChoicePara>>& a_mapDefaultTransitRouteChoicePara);
	static void LoadDefaultTransitRouteChoiceSetting(std::map<int, KTransitRouteChoiceSetting>& a_mapDefaultTransitRouteChoiceSetting);

	//Assign
	static void LoadDefaultTransitRouteChoiceParaBus(std::map<int, TMAFnCoefficient>& a_mapBusCofficient);
	static void LoadDefaultTransitRouteChoiceParaRail(std::map<int, TMAFnCoefficient>& a_mapSubwayCofficient);
	static void LoadDefaultTransitRouteChoiceParaComposite(std::map<int, TMAFnCoefficient>& a_mapCompositCofficient);

	static void SaveCoefficient(int a_nVariableCode, double a_dValue, TMAFnCoefficient& a_oFnCofficient);
};

