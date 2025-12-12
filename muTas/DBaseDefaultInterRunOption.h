#pragma once
#include "DBaseInterModal.h"

class KTarget;
class KMode;

class KInterOccupancePCEMode
{
public:
	KInterOccupancePCEMode::KInterOccupancePCEMode()
		:nUse(0), nModeID(0), dOccupancy(1.0), dPCE(1.0), pMode(nullptr), pColumn(nullptr)
	{
	}
	KInterOccupancePCEMode::~KInterOccupancePCEMode(){}

public:
	int			nUse;
	int			nModeID;
	double		dOccupancy;
	double		dPCE;

	KMode*		pMode;
	KIOColumn*	pColumn;
};


class KDBaseDefaultInterRunOption
{
public:
	KDBaseDefaultInterRunOption(void);
	~KDBaseDefaultInterRunOption(void);

	static void GetModeInfo(KTarget* a_pTarget, std::map<int, KInterOccupancePCEMode>& a_mapOccupancyPCE);
	static void LoadInterOccupancyPCE(std::map<int, std::map<int, KInterOccupancePCEMode>>& a_mapOccupancyPCEmode);

	static void LoadInterUrbanOccupancyPCE(KTarget* a_pTarget, std::vector<TInterModeDefine>& ar_vecInterUrbanMode );
	static void LoadInterRegionalOccupancyPCE(KTarget* a_pTarget, std::vector<TInterModeDefine>& ar_vecInterUrRegionalMode );
	static void LoadInterODDuration(KTarget* a_pTarget, double& ar_dDuration );
};

