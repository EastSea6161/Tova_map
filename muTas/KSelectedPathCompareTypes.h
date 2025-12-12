#pragma once


#include "MultiModalResultInfo.h"
#include "InterModalPathInfo.h"



enum EMPathCompareMode {
	EMPathCompareMode_Multimodal = 1,
	EMPathCompareMode_Intermodal,
};


struct KMTransitEditInfoWrapper
{
	CString strDestNode;
	CString strPathName;
	KMTransitEditInfo* pInfo;

	KMTransitEditInfoWrapper() : pInfo(NULL) {}
};


struct KMBusAssignInfoWrapper
{
	CString strDestNode;
	CString strPathName;
	KMBusAssignInfo* pInfo;

	KMBusAssignInfoWrapper() : pInfo(NULL) {}
};



struct KInterPathEditInfoWrapper
{
	CString strDestNode;
	CString strPathName;
	KInterPathEditInfo* pInfo;		// 지역간 통행의 주요경로일 경우 NULL임

	KInterPathEditInfoWrapper() : pInfo(NULL) {}
};



enum EMInterRegionPathKey
{
	EMInterRegionPathKey_Unknown = 0,
	EMInterRegionPathKey_PathMain,		// 주요경로
	EMInterRegionPathKey_AccessAuto,		// 출발접근-승용차
	EMInterRegionPathKey_AccessBus,		// 출발접근-버스
	EMInterRegionPathKey_AccessRail,		// 출발접근-도시철도
	EMInterRegionPathKey_AccessBusRail,	// 출발접근-버스+도시철도
	EMInterRegionPathKey_EgressAuto,		// 도착접근-승용차
	EMInterRegionPathKey_EgressBus,		// 도착접근-버스
	EMInterRegionPathKey_EgressRail,		// 도착접근-도시철도
	EMInterRegionPathKey_EgressBusRail	// 도착접근-버스+도시철도
};


struct KInterPathRegionInfoWrapper
{
	CString strDestNode;
	CString strFromTerminal;
	CString strToTerminal;
	CString strPathName;
	KRegionModePathInfo* pRegionPathInfo;
	KRegionPathDetail* pRegionPathDetail;

	KInterPathRegionInfoWrapper() : pRegionPathInfo(NULL), pRegionPathDetail(NULL) {}
};
