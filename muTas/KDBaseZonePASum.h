/**
 * @file KDBaseZonePASum.h
 * @brief 존의 Production, Attraction 합 계산 
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */

#pragma once
#include "Target.h"
#include "KPurposeKey.h"


/**
* @brief 존의 Production, Attraction 합 계산 클래스
* @remark 
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KDBaseZonePASum
{
public:
    KDBaseZonePASum(void);
    ~KDBaseZonePASum(void);

private:

    /**
    * @brief 존의 특정 컬럼의 합 계산
    * @param[in] pTarget - 목표년도
    * @param[in] key - 목적키 정보
    * @return 합계산결과
    */
    static CString GetSumValue(KTarget* pTarget, KPurposeKey& key);

    /**
    * @brief 존의 특정 컬럼의 합 계산
    * @param[in] pTarget - 목표년도
    * @param[in] strField - 목적 필드 Name
    * @return 합계산결과
    */
    static CString GetSumValue(KTarget* pTarget, CString strField);
public:
    /**
    * @brief 목적별 Production, Attraction 합 계산
    * @param[in] pTarget - 목표년도
    * @param[out] results - std::map<KPurposeKey, CString> 결과
    * @return true / false
    */
    static bool PASum(KTarget* pTarget, std::map<KPurposeKey, CString>& results);    
};

