
#pragma once

#include "stdafx.h"

struct TLinkInfo
{
	Integer nxLinkID;
	Integer nxFNodeID;
	Integer nxTNodeID;
};

struct TLinkVolume
{
	Integer nxLinkID;
	double  dVolume;
};

struct TLinkODVolume
{
	Integer nxOZoneID;
	Integer nxDZoneID;
	double  dVolume;
};

struct TMinMaxInfo
{
	double dMaxValue;
	CXTPReportRecord* pRecord;
};