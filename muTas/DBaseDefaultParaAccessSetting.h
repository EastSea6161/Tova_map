#pragma once
//^#include "DBaseConnector.h"
#include "Target.h"

struct TAccessClassInfo
{
	CString    strClassName;
	int        nTimeMin;
};

class KAccessStationSetting
{
public:
	KAccessStationSetting::KAccessStationSetting():dDistance(0.0),nRegionCode(0),nClassNumber(0)
	{
	}
	KAccessStationSetting::~KAccessStationSetting(){}

public:
	double dDistance;
	int    nRegionCode;
	int    nClassNumber;
};

class KNodeTypeInfo
{
public:
	KNodeTypeInfo::KNodeTypeInfo(){}
	KNodeTypeInfo::~KNodeTypeInfo(){}

public:
	int        nNodeType;
	CString    strCodeDisplay;
};

class KAccBusSubwayStopAtZoneInfo
{
public:
	KAccBusSubwayStopAtZoneInfo::KAccBusSubwayStopAtZoneInfo(){}
	KAccBusSubwayStopAtZoneInfo::~KAccBusSubwayStopAtZoneInfo(){}

public:
	Integer    nxNodeID;
	Integer    nxLineID;
	int        nType;
	int        nStationCnt;
	CString    strStations;
};

// KDBaseDefaultParameter

class KDBaseDefaultParameter
{

public:
	KDBaseDefaultParameter();
	~KDBaseDefaultParameter();

public:
	static void LoadDefaultAccessClassInfo(std::map<int, std::vector<TAccessClassInfo>>& a_mapDefaultAccessClassInfo);
	static void LoadDefaultAccessSetting(std::map<int, std::vector<KAccessStationSetting>>& a_mapDefaultAccessSetting);
	static void LoadAccessClassInfo(std::vector<TAccessClassInfo>& a_vecDefaultAccessClassInfo);
	static void SetAccessibility(std::map<int, CString>& a_mapAccessibility);


};


