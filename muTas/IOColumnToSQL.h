/**
 * @file IOColumnToSQL.h
 * @brief KIOColumnToSQL 선언 파일
 * @author 
 * @date 2011.04.06
 * @remark
 */


#pragma once


/* Forward declarations */
class KIOColumn;


/**
 * @brief Column 정보를 SQL의 필요구문으로 변환해주는 class들의 base class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.06
 */
class KIOColumnToSQL
{
public:
	KIOColumnToSQL(LPCTSTR strTable, KIOColumn& column);
	virtual ~KIOColumnToSQL(void);

public :
	/**
	* @brief column에서 유효한 fileds의 목록을 문자열로 반환한다.
	* @return CString - fields 목록 문자열
	*/
	virtual CString GetFieldsString(void) = 0;

	/**
	* @brief column에서 유효한 fields에 대한 values 목록을 문자열로 반환한다.
	* @return CString - values 목록 문자열
	*/
	virtual CString GetValuesString(void) = 0;

	/**
	 * @brief column에서 유효한 "fields = 'vlaue'" 목록의 문자열을 반환한다.
	 * @return CString - "field = 'vlaue'"로 구성된 문자열
	 */
	virtual CString GetFieldsAndValuesString(void) = 0;

protected :
	KIOColumn& m_Column;
	CString m_strTableName;

	CString makeQuot(LPCTSTR str);
};


