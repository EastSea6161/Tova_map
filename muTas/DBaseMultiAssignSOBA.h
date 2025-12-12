#pragma once


struct TMultiAssignDataSOBA
{
	CString strScenario;	// 대상 시나리오
	CString strTarget;		// 대상 분석년도
	CString strData;		// 수단 문자열
	CString strWSScenario;	// 웝스타트 초기치 시나리오
	CString strWSTarget;	// 웜스타트 초기치 분석년도
};

class KDBaseMultiAssignSOBA
{
public:
	KDBaseMultiAssignSOBA(void);
	~KDBaseMultiAssignSOBA(void);

public:

	/**
	* MultiAssignSOBA와 관련된(Multi_Assign_Info_Soba와 Multi_Assign_Data_Soba) 테이블 존재 여부를 확인
	* @return bool 테이블 존재시 true, 없으면 false를 반환
	*/
	static bool CheckMultiAssignSOBATableExists();

	/**
	* MultiAssignSOBA와 관련된(Multi_Assign_Info_Soba와 Multi_Assign_Data_Soba) 테이블을 생성
	* @return bool 정상 처리시 true, 실패시 false를 반환
	*/
	static bool CreateMultiAssignSOBATable();



	static bool IsExistName(CString a_strName);
	static bool IsExistNameExcludeCurrent(int a_nID, CString a_strName);	//현재 ID를 제외하고 해당이름을 사용하는지를 체크함

	static int  NextMultiInfoID(void);
	static int  InsertMutliInfo(CString a_strName, CString a_strModes);
	static void UpdateMultiInfo(int a_nID, CString a_strName, CString a_strModes);


	static void InsertMultiData(int a_nID, int nNo, CString a_strScenario, CString a_strTarget, CString a_strData,
		CString strWSScenario, CString strWSTarget);

	static void SelectMultiInfos(CString a_strModes, std::map<int, CString>& a_mapInfo);
	static void SelectMultiDatas(int a_nID, std::vector<TMultiAssignDataSOBA>& a_vecData);

	static void DeleteMultiAssign(int a_nID);
};

