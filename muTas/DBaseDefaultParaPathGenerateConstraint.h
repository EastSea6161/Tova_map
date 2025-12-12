#pragma once
//^#include "DBaseConnector.h"
#include "Target.h"

class KPathGenerateConstraint
{
public:
	KPathGenerateConstraint::KPathGenerateConstraint()
	:dPathTime(0.0), nInvehicleTime(0), nWatingTime(0), nTransferTime(0), nAETime(0), dPathLength(0.0), dPathCost(0.0)
	{
	}
	KPathGenerateConstraint::~KPathGenerateConstraint(){}

public:
	double	dPathTime;
	int		nInvehicleTime;
	int		nWatingTime;
	int		nTransferTime;
	int		nAETime;
	double	dPathLength;
	double  dPathCost;
};

class KPathGenerateInvehicle
{
public:
	KPathGenerateInvehicle::KPathGenerateInvehicle()
	:dAlpha(0.0), dBeta(0.0)
	{
	}
	KPathGenerateInvehicle::~KPathGenerateInvehicle(){}

public:
	double	dAlpha;
	double	dBeta;
};

class KDBaseDefaultParaPathGenerateConstraint 
{

public:
	KDBaseDefaultParaPathGenerateConstraint();
	~KDBaseDefaultParaPathGenerateConstraint();

public:
	static void LoadDefaultPathGenerateConstraint(std::map<int, KPathGenerateConstraint>& a_mapDefaultPathGenerateConstraint);
    static void LoadDefaultPathGenerateInvehicle (std::map<int, KPathGenerateInvehicle >& a_mapDefaultPathGenerateInvehicle);
	static bool LoadDefaultPathGenerateCheck();
	
};


