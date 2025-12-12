/**
 * @file IOColumnToString.h
 * @brief KIOcolumnToString 선언 파일
 * @author 
 * @date 2011.04.06
 * @remark
 */


#pragma once


/* Forward declarations */
class KIOColumn;
class KIOColumnToSQL;


/**
* @brief Column을 SQL문으로 변환 하는 방법
*/
enum KEMIOColumnToString
{
	KEMIOColumnToStringInsert = 1,	/** Insert 문으로 변환 */
	KEMIOColumnToStringUpdate = 2,	/** Update 문으로 변환 */
	KEMIOColumnToStringDelete = 3	/** Delete 문으로 변환 */
};


/**
 * @brief Column 정보를 문자열로 변환하는 class.
 *          insert, update, delete문의 field 목록과 값으로 변환한다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.06
 */
class KIOColumnToString
{
public:
	KIOColumnToString(LPCTSTR strTableName, KIOColumn& column, KEMIOColumnToString emCS);
	~KIOColumnToString(void);

public :
	/**
	* @brief column에서 유효한 fileds의 목록을 문자열로 반환한다.
	* @return CString - fields 목록 문자열
	*/
	CString GetFieldsString(void);

	/**
	* @brief column에서 유효한 fields에 대한 values 목록을 문자열로 반환한다.
	* @return CString - values 목록 문자열
	*/
	CString GetValuesString(void);

	/**
	* @brief column에서 유효한 "fields = 'vlaue'" 목록의 문자열을 반환한다.
	* @return CString - "field = 'vlaue'"로 구성된 문자열
	*/
	CString GetFieldsAndValuesString(void);


private :
	KIOColumnToSQL* m_pToString;
};
