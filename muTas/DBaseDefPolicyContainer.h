#pragma once

class KTarget;

class KPolicyContainerInfo
{
public:
	KPolicyContainerInfo::KPolicyContainerInfo():
	  TnMasterCode(-1),
	  TnxZoneID(0),
	  TstrZoneName(_T("")),
	  TstrPortName(_T("")),
	  TdProduction(0.0),
	  TdConsumption(0.0)
	  {

	  }
	  KPolicyContainerInfo::~KPolicyContainerInfo(){}

	  int TnMasterCode;
	  Integer TnxZoneID;
	  CString TstrZoneName;
	  CString TstrPortName;
	  double  TdProduction;
	  double  TdConsumption;
};

class KPolicyContainerZoneInfo
{
public:
	KPolicyContainerZoneInfo::KPolicyContainerZoneInfo():
		TnxZoneID(0),
		TstrZoneName(_T(""))
	  {

	  }
	  KPolicyContainerZoneInfo::~KPolicyContainerZoneInfo(){}

	  Integer TnxZoneID;
	  CString TstrZoneName;
};

class KDBaseDefPolicyContainer
{
public:
	KDBaseDefPolicyContainer(void);
	~KDBaseDefPolicyContainer(void);

	static void GetDefaultPolicyContainerInfo(std::map<Integer, KPolicyContainerInfo>& ar_mapPContainerInfo);
	static void GetAllZoneInfo(KTarget* a_pTarget, std::map<Integer, CString>& ar_mapZoneInfo);
	static bool SaveContainerInfo(std::map<Integer, KPolicyContainerInfo> a_mapContainerInfo);
};

