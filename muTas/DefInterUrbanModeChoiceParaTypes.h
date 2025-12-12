#pragma once



struct TDescriptionVariableInfo
{
	int nId;
	int nType;
	CString strTypeName;
	CString strVariableName;
	CString strVariableUnit;
	CString strDescript;
	bool bSelected;
	bool bDefault;
};

struct TZoneVariableInfo
{
	int nId;
	int nType;
	CString strTypeName;
	CString strVariableName;
	CString strZoneColName;
	CString strZoneColDispName;
	int nZoneColDataType;	// 1=Integer, 2=Double, 3=String
	int nZoneColCodeType;	// -1=Code พฦดิ, 0=Code
	CString strZoneColDesc;
	bool bSelected;
};


struct TZoneUserColumnInfo
{
	int nId;
	CString strColumnName;
	int nDataType;	// 1=Integer, 2=Double, 3=String
	int nCodeType;	// -1=Code พฦดิ, 0=Code
	CString strDisplayName;
	CString strDescription;
};