#pragma once

class KIOColumns;
/**
*@brief 쿼리문 생성
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KIOQueryBuilder
{
public:
	KIOQueryBuilder(void);
	~KIOQueryBuilder(void);

public :
	/**
	 * @brief 주어진 테이블명에서 columns에 해당하는 열을 조회하는 SQL문 생성
	 * @param[in] pColumns - 조회 할 테이블의 컬럼정보
	 * @param[in] szTable - DB 테이블 명
	 * @return CString - SQL문
	 */
	CString BuildSelectQuery(const KIOColumns* pColumns, LPCTSTR szTable);

	/**
	* @brief 주어진 테이블명에서 columns에 해당하는 열을 조회하는 SQL문 생성
	* @param[in] pColumns - 조회 할 테이블의 컬럼정보
	* @param[in] szTable - DB 테이블 명
	* @param[in] szWhere - 조회 조건
	* @param[in] szGroup- 그룹 조건
	* @param[in] szOrder - 정렬 조건
	* @return CString - SQL문
	*/
	CString BuildSelectQuery(const KIOColumns* pColumns, LPCTSTR szTable, LPCTSTR szWhere, LPCTSTR szOrder);

};
