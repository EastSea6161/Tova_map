/**
 * @file KDBaseOD2PASum.h
 * @brief OD자료로 부터 발생/도착량 집계 클래스
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */

#pragma once
#include "Target.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"


/**
* @brief OD자료로 부터 발생/도착량 집계 클래스
* @remark 
* @warning 
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KDBaseOD2PASum
{
public:
    KDBaseOD2PASum(void);
    ~KDBaseOD2PASum(void);
public:

    /**
    * @brief OD자료에서 Production 양 집계
    * @param[in] pTarget - Target(목표년도) Pointer
    * @param[in] pTable  - OD Table Pointer
    * @param[in] columnCollection  - 목적 컬럼 Collection
    * @param[out] records  - 집계 결과
    * @return true, false
    */
    static bool OD2Production (KTarget* pTarget, KIOTable* pTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records);
    
    /**
    * @brief OD자료에서 Attraction 양 집계
    * @param[in] pTarget - Target(목표년도) Pointer
    * @param[in] pTable  - OD Table Pointer
    * @param[in] columnCollection  - 목적 컬럼 Collection
    * @param[out] records  - 집계 결과
    * @return true, false
    */
    static bool OD2Attraction (KTarget* pTarget, KIOTable* pTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records);
public:
    
};

