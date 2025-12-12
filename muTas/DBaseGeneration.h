#pragma once

#include <sys/stat.h>
#include <fstream>
#include <iterator>
#include <iostream>
#include "Target.h"
#include "afxwin.h"
#include "ZoneGroupColumn.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinyxml.h"
#include "../QBicExtStaticCppLib/tinyXmlLib/tinystr.h"
#include "ZoneGroupValue.h"


struct TGroupColumnInfo
{
	KZoneGroupColumn* pColumn;
	CXTPReportRecord* pParent;
};


class TDefaultZoneGroupParent
{
public:
	TDefaultZoneGroupParent()
	{
		pPElement = nullptr;
		pZoneGroupValue = nullptr;
	}
	~ TDefaultZoneGroupParent(){}
public:
	TiXmlElement*		pPElement;
	KZoneGroupValue*	pZoneGroupValue;
};

class TCategoryClassInto
{
public:
	TCategoryClassInto()
	{
		nCategoryID		= 0;
		strCategoryName = _T("");
		nSexUse			= 0;
		nAgeUse			= 0;
	}
	~TCategoryClassInto() {}

public:
	int nCategoryID;
	CString strCategoryName;
	int nSexUse;
	int nAgeUse;
	std::map<Integer, KODKey> mapAgeGroup;
	std::map<KODKey, double> mapClassValue;

};


class KDBaseGeneration
{
public:
	KDBaseGeneration(void);
	~KDBaseGeneration(void);

public:
	static void GetCategoryClass(KTarget* a_pTarget, std::map<int, TCategoryClassInto>& ar_mapCategoryInfo);
	static void GetCategoryAgeGroup(KTarget* a_pTarget, int nCategory_id, std::map<Integer, KODKey>& ar_mapAgeGroup);
	static void GetCategoryClassValue(KTarget* a_pTarget, int nCategory_id, std::map<KODKey, double>& ar_mapClassValue);
	static void GetCategoryClassName(KTarget* a_pTarget, int nCategory_id, CString& ar_strClassName);
	static void GetSEDPopMaxAge(KTarget* a_pTarget, int& a_nMaxAge);

	static void DeleteCategoryClass(KTarget* a_pTarget, int nCategory_id);
	static void DeleteCategoryValue(KTarget* a_pTarget, int a_nCategroy_id);
	static void GetMaxCategoryID(KTarget* a_pTarget, int& nMaxCategory_id);

	static void SaveCategroyAgeGroup(KTarget* a_pTarget, int a_nCategory_id, std::map<int, KODKey> a_mapCategroyAge);
	static void SaveCategoryClassValue(KTarget* a_pTarget, int a_nCategroy_id, std::map<KODKey, double> a_mapCategroyValue);

	static bool IsPopTableNotEmpty(KTarget* a_pTarget);
	static void GetSubCode(KTarget* a_pTarget, CString a_strColumnName, std::map<CString, int> a_mapPCodeInfo, std::set<int>& a_setCode );

};

