#pragma once

class KTarget;
class KDBaseCheckData
{
public:
	KDBaseCheckData(void);
	~KDBaseCheckData(void);

	static bool SimpleCheckDBData(KTarget* a_pTarget, CString a_strTableName);

	static bool CheckParameterODLengthdata(KTarget* a_pTarget);

	static bool CheckParaemterODCostdata(KTarget* a_pTarget);

	static bool GetCheckAccessStopCount(KTarget* a_pTarget);

	static bool GetCheckParameterData(KTarget* a_pTarget);

	static bool GetCheckTransitCostdata(KTarget* a_pTarget);

	static bool GetCheckTransitInfoData(KTarget* a_pTarget);

	static bool GetCheckAssignReNewUserEquilibriumOB( KTarget* a_pTarget );
	
	static bool GetCheckAssignReNewMultiModal( KTarget* a_pTarget );
	
	static bool GetCheckAssignReNewInterModal( KTarget* a_pTarget );

	static bool FCheckParameterDirectZone(KTarget* a_pTarget);

	static bool GetCheckTargetRegionCode(KTarget* a_pTarget);

	static bool GetCheckTerminaNodeData(KTarget* a_pTarget);

	static bool GetCheckBusPathInputData( KTarget* a_pTarget );

	static bool GetCheckDataInterTerminalTimeInfo( KTarget* a_pTarget );

	static bool GetCheckInterModalPathFile(KTarget* a_pTarget);

	static bool GetCheckInterModalPahthGenerator(KTarget* a_pTarget);

	static void GetZoneCount(KTarget* a_pTarget, int& a_nCount);
	
	static bool GetCheckAssignInterModalResult( KTarget* a_pTarget );

	static bool GetCheckInspectionErrorHighway( KTarget* a_pTarget );

	/**
	* 네트워크 오류를 검사하고, 결과를 반환
	* GetCheckInspectionErrorHighway는 오류 발견시 메시지 박스와 검사를 위한 다이얼로그를 표출 하지만
	* GetCheckInspectionErrorHighwayNoMB는 오류가 있는지 없는지만 반환한다.
	* @param a_pTarget 검사를 수행 할 분석년도
	* @return 네트워크 데이터가 정상이면 true, 이상이 있으면 false를 반환
	*/
	static bool GetCheckInspectionErrorHighwayNoMB(KTarget* a_pTarget);

	static bool GetCheckInspectionErrorTransit( KTarget* a_pTarget);

	static bool GetExistModeODData(KTarget* a_pTarget);

	static bool GetCheckAssignResult(KTarget* a_pTarget);

	static bool CheckBuslaneCode(KTarget* a_pTarget);
};

