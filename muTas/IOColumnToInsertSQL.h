/**
 * @file IOColumnToInsertSQL.h
 * @brief KIOColumnToInsertSQL 선언 파일
 * @author 
 * @date 2011.04.05
 * @remark
 */


#pragma once


#include "IOColumnToSQL.h"


/**
 * @brief Column 정보를 Insert SQL문의 Field목록과 Values 목록 문자열로 변환하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.05
 */
class KIOColumnToInsertSQL : public KIOColumnToSQL
{
public:
	KIOColumnToInsertSQL(LPCTSTR strTableName, KIOColumn& column);
	~KIOColumnToInsertSQL(void);

public :
	/**
	 * @brief column에서 유효한 fileds의 목록을 문자열로 반환한다.
	 * @return CString - fields 목록 문자열
	 */
	virtual CString GetFieldsString(void);

	/**
	 * @brief column에서 유효한 fields에 대한 values 목록을 문자열로 반환한다.
	 * @return CString - values 목록 문자열
	 */
	virtual CString GetValuesString(void);

	/**
	* @brief column에서 유효한 "fields = 'vlaue'" 목록의 문자열을 반환한다.
	* @return CString - "field = 'vlaue'"로 구성된 문자열
	*/
	virtual CString GetFieldsAndValuesString(void);
};
