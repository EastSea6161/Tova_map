#pragma once

#include "TUserGeometryInfo.h"

class KTarget;

struct TAccessLineSationInfo
{
	Integer nxTransitID;
	Integer nxStationID;
	int		nTransitMode;
};

struct TAccessStopNodeInfo
{
	Integer	nxZoneID;
	Integer nxNodeID;
	int		nMode;
	CString strZoneName;
	CString strClassInZone;
	CString strClassInNetwork;
	int		nNoLine;
};


struct TClassInfo
{
	COLORREF clrClass;
	double	 dSize;
};

class KDBaseAccessibilityModify
{
public:
	KDBaseAccessibilityModify(void);
	~KDBaseAccessibilityModify(void);

public:

	static void GetSationInfo(KTarget* a_pTarget, std::set<Integer>& a_setStationInfo, std::map<KODKey, int>& a_mapLineSation );
	
	static void GetZoneInfo(KTarget* a_pTarget, std::map<Integer, CString>& a_mapZoneName, std::map<Integer, CString>& a_mapZoneFullName);

	static void GetClassInfo(KTarget* a_pTarget, std::map<CString, int>& a_mapClassInfo, std::map<CString, double>& a_mapClassTime);

	static void GetExistingAccessLineInfo(KTarget* a_pTarget, Integer a_nxZoneId, Integer a_nxlineID , std::map<CString, int> a_mapClassInfo, std::map<KODKey, int>& a_mapExistAccessInfo);

	static void UpdateAccessLineInfo(KTarget* a_pTarget, Integer a_nxZoneId, Integer a_nxStationID, Integer a_nxLineId, CString a_strClassName);

	static void TransitStationNodeViaInfo(KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& a_mapNodeLineInfo, std::map<Integer,int>& a_mapStationMode);

	static void TransitStationNodeViaInfoByAccess( KTarget* a_pTarget, std::map<Integer, std::set<Integer>>& a_mapStationNodeViaInfo, std::map<Integer,int>& a_mapStationMode );

	static void GetIncludeNodeAccessInfo(KTarget* a_pTarget, Integer a_nxZoneID, int a_nMode, std::map<Integer, TAccessStopNodeInfo>& a_mapStopNodeInfo);

	static void GetIncludeZoneAccessInfo(KTarget* a_pTarget, Integer a_nxNodeID, int a_nMode, std::map<Integer, TAccessStopNodeInfo>& a_mapStopNodeInfo );

	static void GetZoneRadius(KTarget* a_pTarget, double& a_dRadius);

	static void ApplyAccessStopNodeEditAtStation(KTarget* a_pTarget, Integer a_nxZone_ID, /*CString a_strZoneName,*/ int a_nMode, std::vector<TAccessStopNodeInfo> & a_vecStopNodeInfo);

	static void ApplyAccessStopNodeEditAtZone(KTarget* a_pTarget, Integer a_nxStationID, int a_nMode, std::vector<TAccessStopNodeInfo> & a_vecStopNodeInfo);

	static void CheckUsingAssessData(KTarget* a_pTarget, bool& a_bUsable);

	static void GetAllNodeInfo(KTarget* a_pTarget, std::map<Integer, CString>& a_mapNodeInfo);

	static void GetAccessNodeZoneCount(KTarget* a_pTarget, std::map<Integer, int>& ar_mapZoneCount, int a_nMode);
	
	static void GetAccessZoneNodeCount( KTarget* a_pTarget, std::map<Integer, int>& ar_mapNodeCount, int a_nMode );
	
};

