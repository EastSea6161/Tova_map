#pragma once

struct TGenerationModelDB
{
	CString strColumnName;
	CString strStartDate;
	CString strEndDate;
};


struct TChartZoneDB
{
	Integer nxID;
	CString strName;
    double  dValue;
};


struct TChartZoneDisplay
{
	CString strModelName;
	std::vector<TChartZoneDB> vecTChartZoneDB;
};