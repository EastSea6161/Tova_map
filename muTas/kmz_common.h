/**
 * @file kmz_common.h
 * @brief kmz 공통 모듈 정의
 * @author 
 * @date 2010.10.05
 * @remark
 */


#pragma once

#include "KEMProject.h"
#include "KEMEncoding.h"
#include "KeyGenerator.h"
#include "Singleton.h"


/// Project/Scenario/Target의 key 생성 클래스
typedef ZSingleton< ZKeyGenerator< unsigned int > > TDKeyGenerator;


/**
 * @brief ", "로 구분된 연도를 vector<int>로 parsing 한다.
 * @param[in] a_strYears : ", "로 구분된 연도목록의 문자열
 * @param[out] a_vecTargetYears : year 배열
 * @return int : 연도의 갯수
 * @remark ", "외의 구분자는 정상적으로 수행되지 않음
 * @warning 
 */
int ParseTargetYear(CString a_strYears, std::vector<int>& a_vecTargetYears);


/**
 * @brief ", "로 구분된 목적을 vector<CStirng>로 parsing 한다.
 * @param[in] a_strPurposes : ", "로 구분된 목적 목록의 문자열
 * @param[out] a_vecPurposes : purpose 배열
 * @return int : 선택한 목적의 갯수
 * @remark ", "외의 구분자는 정상적으로 수행되지 않음
 * @warning 
 */
int ParsePurposeName(CString a_strPurposes, std::vector<CString>& a_vecPurposes);


/**
 * @brief ", "로 구분된 목적 ID를 vector<CStirng>로 parsing 한다.
 * @param[in] a_strPurposeId : ", "로 구분된 목적 ID 목록의 문자열
 * @param[out] a_vecPurposeId : purpose ID 배열
 * @return int : 선택한 목적의 갯수
 * @remark ", "외의 구분자는 정상적으로 수행되지 않음
 * @warning 
 */
int ParsePurposeId(CString a_strPurposeId, std::vector<int>& a_vecPurposeId);


/**
 * @brief ", "로 구분된 mode을 vector<CStirng>로 parsing 한다.
 * @param[in] a_strModes : ", "로 구분된 mode 목록의 문자열
 * @param[out] a_vecModes : mode 배열
 * @return int : 선택한 mode의 갯수
 * @remark ", "외의 구분자는 정상적으로 수행되지 않음
 * @warning 
 */
int ParseModeName(CString a_strModes, std::vector<CString>& a_vecModes);


/**
 * @brief ", "로 구분된 mode ID를 vector<CStirng>로 parsing 한다.
 * @param[in] a_strModeId : ", "로 구분된 mode ID 목록의 문자열
 * @param[out] a_vecModeId : Mode ID 배열
 * @return int : 선택한 Mode의 갯수
 * @remark ", "외의 구분자는 정상적으로 수행되지 않음
 * @warning 
 */
int ParseModeId(CString a_strModeId, std::vector<int>& a_vecModeId);


/**
 * @brief ", "로 구분된 CString 변수 값을 vector<int>로 parsing 한다.
 * @param[in] a_strIn : ", "로 구분된 연도목록의 문자열
 * @param[out] a_vecOut : int 배열
 * @return int : 선택한 목록의 갯수
 * @remark ", "외의 구분자는 정상적으로 수행되지 않음
 * @warning 
 */
int ParseIntegerArr(CString a_strIn, std::vector<int>& a_vecOut);


/**
 * @brief ", "로 구분된 CString 변수 값을 vector<CStirng>로 parsing 한다.
 * @param[in] a_strIn : ", "로 구분된 String 목록의 문자열
 * @param[out] a_vecOut : String 배열
 * @return int : 선택한 목록의 갯수
 * @remark ", "외의 구분자는 정상적으로 수행되지 않음
 * @warning 
 */
int ParseStringArr(CString a_strIn, std::vector<CString>& a_vecOut);
