#pragma once

#include "Target.h"

class KMultimodalPathGenerationInfo
{
public:
	KMultimodalPathGenerationInfo::KMultimodalPathGenerationInfo()
		:nMax_Routes(0), dDwellTimeBus(0.0), dDwellTimeRail(0.0), nWalkTime(0)
	{
	}
	KMultimodalPathGenerationInfo::~KMultimodalPathGenerationInfo(){}

public:
	int		nMax_Routes;
	double  dDwellTimeBus;
	double  dDwellTimeRail;
	int   	nWalkTime;
};

class KDBaseDefaultParaMultimodalPathGeneration
{

public:
	KDBaseDefaultParaMultimodalPathGeneration();
	~KDBaseDefaultParaMultimodalPathGeneration();

public:
	static void LoadDefaultMultimodalPathInfo(std::map<int, KMultimodalPathGenerationInfo>& a_mapDefaultMultimodalPathInfo);

};
