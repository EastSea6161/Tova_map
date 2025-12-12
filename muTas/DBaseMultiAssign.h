#pragma once

struct TMultiAssignData
{
	CString strScenario;
	CString strTarget;
	CString strData;
};

class KDBaseMultiAssign
{
public:
	KDBaseMultiAssign(void);
	~KDBaseMultiAssign(void);

public:
	static bool IsExistName(CString a_strName);
	static bool IsExistNameExcludeCurrent(int a_nID, CString a_strName);	//현재 ID를 제외하고 해당이름을 사용하는지를 체크함
	
	static int  NextMultiInfoID(void);
	static int  InsertMutliInfo(CString a_strName, CString a_strModes);
	static void UpdateMultiInfo(int a_nID, CString a_strName, CString a_strModes);


	static void InsertMultiData(int a_nID, CString a_strScenario, CString a_strTarget, CString a_strData);

	static void SelectMultiInfos(CString a_strModes, std::map<int, CString>& a_mapInfo);
	static void SelectMultiDatas(int a_nID, std::vector<TMultiAssignData>& a_vecData);

	static void DeleteMultiAssign(int a_nID);
};

