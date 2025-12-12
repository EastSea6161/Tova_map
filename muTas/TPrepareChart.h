#pragma once

class TNetworkDBData
{
public:
	Integer nxID;
	CString strName;
	double  dValue;
};

class TNetworkChartData
{
public:
	CString strFieldName;
	std::vector<TNetworkDBData> vecTData;

};