
#pragma once

#include "stdafx.h"

struct TPathVolumeInfo
{
	Integer		TnxPathNo;
	Integer		TnxOriginID;
	Integer		TnxDestinationID;
	COLORREF	Tcolor;
	double		TdWidth;
	double		TdVolume;
	std::vector<Integer> TvecLink;
	double      TdDistance;
	double      TdTimeCost;
};

//std::map<double, TPathVolumeInfo> : Key °ªÀº Volume°ª

struct TPathVolInterModeInfo
{
	int     nModeID;
	CString strModeName;
	CString strResultFieldName;
};
