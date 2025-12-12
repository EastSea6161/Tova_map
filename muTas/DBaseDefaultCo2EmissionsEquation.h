#pragma once

class KCO2EquationData
{
public:
	KCO2EquationData::KCO2EquationData(){}
	KCO2EquationData::~KCO2EquationData(){}

public:
	int     nMasterCD;
	int     nModeID;
	CString strNameVeh;
	CString strNameFuel;
	double  dRatio;
	double  dSpdGap;
	BOOL    bIsSelcted;
	BOOL    bIsSpdGapAllSelected;
	CString strExpUnder;
	CString strExpAndOver;
	double  dPCE;
};

class KDBaseDefaultCo2EmissionsEquation
{
public:
	KDBaseDefaultCo2EmissionsEquation(void);
	~KDBaseDefaultCo2EmissionsEquation(void);

	static void GetDefCO2EquationInfo(std::map<int, std::map<int, CString>>& a_mapDefCO2EquationInfo);
	static void GetDefCO2EquationData(std::map<int, std::vector<KCO2EquationData>>& a_mapDefCO2EquationData);

	static void UpdateDefCO2Equation(
		std::map<int, std::map<int, CString>>& a_mapDefCO2EquationInfo, std::map<int, std::vector<KCO2EquationData>>& a_mapDefCO2EquationData);
};

