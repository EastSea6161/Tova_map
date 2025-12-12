#pragma once
#include "Target.h"

class KTarget;

class KOccupancyPCEMode
{
public:
	KOccupancyPCEMode::KOccupancyPCEMode()
		:nModeID(0), strModeName(_T("")), dOccupancy(0.0), dPCE(0.0)
	{
	}
	KOccupancyPCEMode::~KOccupancyPCEMode(){}

public:
	int			nModeID;
	CString		strModeName;
	double		dOccupancy;
	double		dPCE;
};

class KCheckMode
{
public:
	KCheckMode::KCheckMode()
		:nModeCode(0), strModeName(_T("")), dOccupancy(0.0), dPCE(0.0)
	{
	}
	KCheckMode::~KCheckMode(){}

public:
	int			nModeCode;
	CString		strModeName;
	double		dOccupancy;
	double		dPCE;
};

class KModeCheckInfo
{
public:
	KModeCheckInfo::KModeCheckInfo()
		:nCheckMode(0), nCheckPreLoad(0)
	{
	}
	KModeCheckInfo::~KModeCheckInfo(){}

public:
	int	nCheckMode;
	int nCheckPreLoad;
};

class KODTime
{
public:
	KODTime::KODTime()
		:dTimeDuration(0.0), dTravelTime(0.0)
	{
	}
	KODTime::~KODTime(){}

public:
	double		dTimeDuration;
	double		dTravelTime;
};

class KVDFPara
{
public:
	KVDFPara::KVDFPara()
		:dAllLinkAlpha(0.0), dAllLinkBeta(0.0), strUseFunctionID(_T("")), strLinkColumnAlpha(_T("")), strLinkColumnBeta(_T(""))
		,strLinkColumnA(_T("")), strLinkColumnB(_T("")), nVDFChoice(0)	
	{
	}
	KVDFPara::~KVDFPara(){}

public:
	double	dAllLinkAlpha;
	double	dAllLinkBeta;
	CString strUseFunctionID;
	CString strLinkColumnAlpha;
	CString strLinkColumnBeta;
	CString strLinkColumnA;
	CString strLinkColumnB;
	int		nVDFChoice;
};

class KVDFFunction
{
public:
	KVDFFunction::KVDFFunction()
		:nFn(0), dAlpha(0.0), dBeta(0.0), dA(0.0), dB(0.0)
	{
	}
	KVDFFunction::~KVDFFunction(){}

public:
	int			nFn;
	double		dAlpha;
	double		dBeta;
	double		dA;
	double		dB;
};

class KDBaseDefaultParaHighwayAssign
{
public:
	KDBaseDefaultParaHighwayAssign(void);
	~KDBaseDefaultParaHighwayAssign(void);

protected:
	std::map<CString, int>		   m_mapModeChoiceInfo;
	std::vector<KOccupancyPCEMode> vecOccupancyPCEMode;

		
public:
	static void GetModeInfo(KTarget* a_pTarget, std::map<int, CString>& a_mapGetModeinfo);
	static void GetFModeInfo(KTarget* a_pTarget, std::map<int, CString>& a_mapGetModeInfo);

	static void LoadDefaultOccupancyPCEMode(std::map<int, std::vector<KOccupancyPCEMode>>& a_mapDefaultOccupancyPCEMode);
	static void LoadDefaultODTime(std::map<int, KODTime>& a_mapDefaultODTime);
	static void LoadDefaultVDFPara(std::map<int, KVDFPara>& a_mapDefaultVDFPara);
	static void LoadDefaultVDFFuntion(std::map<int, std::vector<KVDFFunction>>& a_mapDefaultVDFFuntion);
	static void LoadDefaultVDFFuntionGroup(std::map<CString, int>& a_mapDefaultVDFFuntionGroup);
};

