#pragma once

#include "DBaseInterModal.h"


struct TDescriptionVariableInfo;
struct TZoneVariableInfo;


class KDBaseDefaultParaInterUrbanModeChoice
{
public:
	KDBaseDefaultParaInterUrbanModeChoice(void);
	~KDBaseDefaultParaInterUrbanModeChoice(void);

public:
	static void LoadDefaultModeChoicePara(std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultPara);
	static void DeleteInsertModeChoicePara(int a_nMasterCode, std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultPara);

	static void LoadDefaultModeChoiceParaByObject(int a_nObjectID, std::map<int, TUrbanChoiceValues> &a_mapUrbanChoiceValues);

	static void LoadDefaultModeChoiceMEU(double &a_dMeuParameter);
	static void DeleteInsertModeChoiceMEU(int a_nMasterCode, double &a_dMeuParameter);


	/**
	* @brief 설명변수(UrbanModeChoiceVariable 테이블) 정보를 읽어온다.
	* @param mapVariableInfo 설명변수 정보를 저장 할 map
	*/
	static void ReadVariableInfo(std::map<int, TDescriptionVariableInfo>& mapVariableInfo);

	/**
	* @brief 존특성 변수(UrbanModeChoiceVariableZone 테이블) 정보를 읽어온다.
	* @param mapZoneVariableInfo 존특성 변수 정보를 저장 할 map
	*/
	static void ReadZoneVariableInfo(std::map<int, TZoneVariableInfo>& mapZoneVariableInfo);

	/**
	* @brief 주어진 존 특성변수를 선택되지 않은 상태로 변경
	* @param spDBConn DB 연결정보
	* @param arrId 변경할 존특성변수 ID의 vector
	* @return bool 정상실행시 true, 실패시 false 반환
	*/
	static bool UpdateZoneVariableInfoUnselected(KDBaseConPtr spDBConn, std::vector<int>& arrId);

	/**
	* @brief 주어진 변수ID의 파라미터 값을 삭제
	* @param spDBConn DB 연결정보
	* @param arrId 삭제 할 파라미터 변수의 ID vector
	* @return bool 정상실행시 true, 실패시 false 반환
	*/
	static bool DeleteUrbanModeChoicePara(KDBaseConPtr spDBConn, std::vector<int>& arrId);

	/**
	* @brief 주어진 변수ID의 파라미터 값을 삭제
	* @param spDBConn DB 연결정보
	* @param arrZoneVar 갱신 할 Zone Variable info의 vector
	* @return bool 정상실행시 true, 실패시 false 반환
	*/
	static bool UpdateZoneVariableInfo(KDBaseConPtr spDBConn, std::vector<TZoneVariableInfo>& arrZoneVar);




	/**
	* @brief 설명변수의 기본값을 읽어온다.
	* @param mapPara Default 변수 값을 저장 할 map
	*/
	static void ReadDefaultModeChoicePara(std::map<int, std::map<int, TUrbanChoiceValues>> &mapPara);

	/**
	* @brief 설명변수의 마지막 설정값을 읽어온다.
	* @param mapPara 현재 설정된(마지막 설정) 변수 값을 저장 할 map
	*/
	static void ReadCurrentModeChoicePara(std::map<int, std::map<int, TUrbanChoiceValues>> &mapPara);

private:
	static int MaxMasterCodeModeChlcePara();
	static int MaxMasterCodeModeChlceMEU();
	
};

