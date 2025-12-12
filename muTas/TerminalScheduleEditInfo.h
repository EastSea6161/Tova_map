#pragma once

#include "stdafx.h"

struct TTerminalScheduleInfo
{
	Integer  nxDestTerminalID;
	CString  strName;
	int      nMode;
	double   dTravelTime;
	double   dCost;
	int		 nFrequency;
	double	 dHeadway;
};