/**
 * @file ZoneGroupColumn.h
 * @brief KzoneGroupColumn 선언 파일
 * @author 
 * @date 2011.04.27
 * @remark
 */


#pragma once


#include "ZoneGroupBase.h"


/* Forward declarations */
class KZoneGroupValue;


/**
 * @brief KZoneGroupExpression에서 컬럼 정보를 표출한다.
 *          ZoneGroupColumn은 ZoneGroupValue만 가질 수 있다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.27
 */
class KZoneGroupColumn : public KZoneGroupBase
{
public:
	KZoneGroupColumn(void);
	KZoneGroupColumn(const KZoneGroupColumn& r);
	virtual ~KZoneGroupColumn(void);


public :

	/**
	* @brief KZoneGroupValue list를 반환한다.
	* @return std::list<KZoneGroupValue*> - Zone Group Value's list
	*/
	std::list<KZoneGroupValue*> ZoneGroupValues(void);

	/**
	* @brief KZoneGroupValue list를 설정한다.
	* @param[in] a_ZoneGroupValues - 설정 하려는 Zone group value's list
	*/
	void ZoneGroupValues( std::list<KZoneGroupValue*> a_ZoneGroupValues);

	/**
	* @brief Column 명칭을 반환한다.
	* @return LPCTSTR - Column 명칭
	*/
	LPCTSTR ColumnName(void);

	/**
	* @brief Column 명칭을 설정한다.
	* @return strColumnName - Column 명칭
	*/
	void ColumnName(LPCTSTR strColumnName);

	/**
	* @brief Column 표출 명칭을 반환한다.
	* @return LPCTSTR - Column 표출 명칭
	*/
	LPCTSTR ColumnCaption(void);

	/**
	* @brief Column 표출 명칭을 설정한다.
	* @return strColumnCaption - Column 표출 명칭
	*/
	void ColumnCaption(LPCTSTR strColumnCaption);

	/**
	* @brief Zone group value를 추가하한다.
	* @return KZoneGroupValue* - 추가된 Zone Group Value
	*/
	KZoneGroupValue* AddZoneGroupValue(void);

	/**
	* @brief 주어진 Zone group value와 일치하는 value를 삭제한다.
	* @param[in] pValue - 삭제 하려는 Zone group value
	*/
	void RemoveZoneGroupValue(KZoneGroupValue* pValue);

	/**
	* @brief 모든 Zone group value를 삭제한다.
	*/
	void RemoveAllZoneGroupValue(void);

	/**
	* @brief 첫번째 Zone group value를 반환한다.
	* @return KZoneGroupValue* - 첫번째 Zone group value
	*/
	KZoneGroupValue* GetFirstZoneGroupValue(void);

	/**
	* @brief 다음 Zone group value를 반환한다.
	* @return KZoneGroupValue* - 다음 Zone group value
	*/
	KZoneGroupValue* GetNextZoneGroupValue(void);
	
	/**
	* @brief 이전 Zone group value를 반환한다.
	* @return KZoneGroupValue* - 이전 Zone group value
	*/	
	KZoneGroupValue* GetPrevZoneGroupValue(void);
	
	/**
	* @brief 마지막 Zone group value를 반환한다.
	* @return KZoneGroupValue* - 마지막 Zone group value
	*/
	KZoneGroupValue* GetLastZoneGroupValue(void);

	KZoneGroupColumn& operator=(const KZoneGroupColumn& r);

protected :
	std::list<KZoneGroupValue*> m_ZoneGroupValues;
	std::list<KZoneGroupValue*>::iterator m_itCursor;
	CString m_strColumnName;
	CString m_strColumnCaption;
};
