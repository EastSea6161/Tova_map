/**
 * @file KDBaseStatistics.h
 * @brief 통계조회 
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */

#pragma once

#include "KBulkType.h"
#include "Target.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumn.h" 
//^^ #include "IOColumns.h" 



/**
* @brief 링크 영향권 통계 결과 저장 클래스
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
struct TLinkInfluence
{
    int    link_type;
    int    count;
    double length;
};

/**
* @brief 통계 정보 조회 클래스
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KDBaseStatistics
{
public:
    KDBaseStatistics(void);
    ~KDBaseStatistics(void);

public:

    /**
    * @brief 컬럼별 통계값 조회
    * @param[in] pIOTable - 테이블
    * @param[in] pColumn - 컬럼
    * @param[out] resultStrArray - 결과
    * @param[in] strFilter - 검색 조건
    * @return true / false
    */
    
    /**
    * @brief 영향권 통계 조회
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[out] results - 결과
    * @return true / false
    */
    static bool StatisticsLinkInfluence(KDBaseConPtr a_spDBaseConnection, std::vector<TLinkInfluence>& results);
};

