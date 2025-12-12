#pragma once
#include "TPathAnalysisInfo.h"

class KTarget;

class KDBasePathVolume
{
public:
	KDBasePathVolume(void);
	~KDBasePathVolume(void);

public:
	static void   GetDBModeInfo  (KTarget* a_pTarget, std::map<int, CString> &a_mapModeInfo);
	static void   GetZoneNodeInfo(KTarget* a_pTarget, std::map<Integer, CString> &a_mapZoneNodeData);
	static void   GetInterModeInfo( KTarget* a_pTarget, std::map<int, TPathVolInterModeInfo> &a_mapInterModeFieldInfo);
	static void   GetInterModeVolumeInfo( KTarget* a_pTarget, std::map<int, TPathVolInterModeInfo> &a_mapInterModeFieldInfo, 
										  std::set<Integer> &a_setSelectedZoneOrg, std::set<Integer> &m_setSelectedZoneDest, 
										  std::map<int, double> &a_mapModeSumVolume);
	static void   GetSelectedZoneAvgPoint( KTarget* a_pTarget, std::set<Integer> &a_setSelectedZone, double &a_dAvgX, double &a_dAvgY);
};

