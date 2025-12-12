#pragma once

class KCO2InterpolationSetting
{
public:
	KCO2InterpolationSetting::KCO2InterpolationSetting()
		: nMaxSpeed(0), nInterval(0), nColumns(0)
	{
	}
	KCO2InterpolationSetting::~KCO2InterpolationSetting(){}
public:
	int nMaxSpeed;
	int nInterval;
	int nColumns;
};

class KCO2InterpolationData
{
public:
	KCO2InterpolationData::KCO2InterpolationData()
	{
	}
	KCO2InterpolationData::~KCO2InterpolationData(){}

public:
	int     nMasterCode;
	int     nModeSeq;
	CString strModeName;
	BOOL    bIsSelected;
	double  dPCE;
	std::vector<double> vecData;
};

class KDBaseDefaultCo2EmissionsInterpolation
{
public:
	KDBaseDefaultCo2EmissionsInterpolation(void);
	~KDBaseDefaultCo2EmissionsInterpolation(void);

	static void SeperateBySemicolon(CString a_strValue, std::vector<double>& a_vecData);
	static void VectorConvertString(std::vector<double> a_vecData, CString& a_strValue);

	static void GetDefaultCo2InterpolationInfo(std::map<int, KCO2InterpolationSetting>& a_mapCO2InterpolationInfo);
	static void GetDefaultCo2InterpolationData(std::map<int, std::map<int, KCO2InterpolationData>>& a_mapCO2InterpolationData);
	static void SaveDefaultCo2Interpolation(
		std::map<int, KCO2InterpolationSetting> a_mapCO2InterpolationInfo, std::map<int, std::map<int, KCO2InterpolationData>> a_mapCO2InterpolationData);
};

