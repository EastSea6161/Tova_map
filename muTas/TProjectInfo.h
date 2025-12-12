#pragma once

struct TProjectInfo
{
	CString strProjectName;
	CString strInvestigator;
	CString strOrganization;
	CString strDescription;
	CString strLocation;

	int     nBaseYear;
	CString strTargetYear;
	
	//Purpose
	bool    bEditedPurpose;
	CString strPurposeName;
	CString strPurposeID;
	CString strPurposeGroup;
	std::vector<KPurpose*> vecPurpose;

	//Mode
	bool    bEditedMode;
	CString strModeName;
	CString strModeID;
	CString strModeGroup;
	std::vector<KMode*> vecMode;
};