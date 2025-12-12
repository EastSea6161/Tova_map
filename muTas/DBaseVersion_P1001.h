#pragma once



class KProject;


class KDBaseVersion_P1001
{
public:
	KDBaseVersion_P1001() {};
	~KDBaseVersion_P1001() {};
public:
	/**
	* 1000 버전의 프로젝트를 불러올때 변경된 DB에 따라서 업그레이드 진행
	* 업그레이드 순서
	*	1. UrbanModeChoiceVariable 생성
	*	2. Default_Intermodal_Urban_ModeChoice_Para_v1001 생성 및 데이터 추가
	*	3. UrbanModeChoiceVariable 의 선택 컬럼을 Default_Intermodal_Urban_ModeChoice_Para_v1001 마지막 값으로 설정
	*/
	static void ProjectDBUpgrade(KProject* pProject, KDBaseConPtr spDBConn);

private:

	/**
	* 
	* 테이블 존재 여부를 확인
	* @param spDBConn 프로젝트 DB connector
	* @param strTableName 확인 할 테이블 명
	* @return bool 테이블 존재시 true, 없으면 false를 반환
	*/
	static bool CheckTableExists(KDBaseConPtr spDBConn, CString& strTableName);

	/**
	* 수단선택계수리스트 설정과 관련하여 프로젝트 DB에 설명변수 설정과 관련된 UrbanModeChoiceVariable 테이블을 생성하고, 초기 데이터를 입력한다.
	*/
	static void CreateTableUrbanModeChoiceVariable(KDBaseConPtr spDBConn);

	/**
	* 수단선택계수리스트 설정과 관련하여 프로젝트 DB에 사용자 정의 존특성변수와 관련된 UrbanModeChoiceVariableZone 테이블을 생성하고, 
	* 초기 데이터를 입력한다.
	*/
	static void CreateTableUrbanModeChoiceVariableZone(KDBaseConPtr spDBConn);


	/**
	* 프로젝트 DB의 Default_Intermodal_Urban_ModeChoice_Para_v1001 테이블에서 마지막 변수 설정 값을 읽고, 
	* 테이블을 초기화한 후 새로운 변수 기본값을 입력하고, 
	* 마지막 변수 설정값을 추가한다.
	*/
	static void UpdateDefaultIntermodalUrbanModeChoicePara(KProject* pProject, KDBaseConPtr spDBConn);

	static void UpdateSelectedVariable(KDBaseConPtr spDBConn);

	/**
	* 도로통행배정 모형의 여러시나리오 반복실행중 웝 스타트 실행시 실행정보를 저장할 테이블 생성
	* 기존 multi_assign_info와 동일한 구조로 생성
	*/
	static void CreateTableMultiAssignInfoSoba(KDBaseConPtr spDBConn);

	/**
	* 도로통행배정 모형의 여러시나리오 반복실행중 웝 스타트 실행시 실행한 시나리오와 웝스타트 초기치 정보를 저장 할 테이블 생성
	* 기존 multi_assign_data에 초기치 시나리오와 대상정보를 추가하여 생성
	*/
	static void CreateTableMultiassignDataSoba(KDBaseConPtr spDBConn);

};