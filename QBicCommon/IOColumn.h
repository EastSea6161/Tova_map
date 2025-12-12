/**
 * @file IOColumn.h
 * @brief KIOColumn 선천 파일
 * @author 
 * @date 2011.03.24
 * @remark
 */


#pragma once


#include "IOCommons.h"


/**
 * @brief Column 정보 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.03.24
 */
class AFX_EXT_CLASS KIOColumn
{
public:
	KIOColumn(void);
	KIOColumn(const KIOColumn& column);
	~KIOColumn(void);


public :
	KIOColumn& operator=(const KIOColumn& column);


	/**
	* @brief Code Column 여부를 반환한다.
	* @return bool - Code Column 여부(true:Code column, false:Code Column이 아님)
	*/
	KEMIOCodeType CodeType(void) const;

	/**
	* @brief Code Column 여부를 설정한다.
	* @param[in] bCode - Code column 여부(true:Code Column, false:Code Column이 아님)
	*/
	void CodeType(KEMIOCodeType a_nCodeType);

	/**
	* @brief Column의 표출 여부를 반환한다.
	* @return bool - Column의 표출 여부(true:표출, false:숨김)
	*/
	bool Visible(void) const;

	/**
	* @brief Column의 표출 여부를 설정한다.
	* @param[in] bVisible - Column의 표출 여부(true:표출, false:숨김)
	*/
	void Visible(bool bVisible);

	/**
	* @brief Column의 고정 여부를 반환한다.
	* @return bool - Column의 고정 여부(true:고정, false:고정 아님)
	*/
	bool FreezeColumn(void) const;

	/**
	* @brief Column의 고정 여부를 설정한다.
	* @param[in] bFreez - Column의 고정 여부
	*/
	void FreezeColumn(bool bFreez);


	/**
	* @brief PK column 여부를 반환한다.
	* @return bool - PK column 여부
	*/
	bool PrimaryKey(void) const;

	/**
	* @brief PK column 여부를 설정한다.
	* @param[in] bPK - PK column 여부
	*/
	void PrimaryKey(bool bPK);

	/**
	* @brief Column의 표출 순서를 반환한다.
	* @return int - Column의 표출 순서
	*/
	int DisplayOrder(void) const;

	/**
	* @brief Column의 표출 순서를 설정한다.
	* @param[in] nOrder - Column의 표출 순서
	*/
	void DisplayOrder(int nOrder);

	/**
	* @brief Column의  default ID를 반환한다.
	* @return int - Column의 default ID
	*/
	int BaseID(void);

	/**
	* @brief Column의 default ID를 설정한다.
	* @param[in] nBaseID - default ID
	*/
	void BaseID(int nBaseID);

	/**
	* @brief Column의 default ID를 반환하고 default ID를 증가시킨다
	* @return int - Column의 default ID
	*/
	int IncreseBaseID(void);

	/**
	* @brief Code column인 경우 Code group의 값을 반환한다.
	* @return int - Code group의 값
	*/
	int CodeGroup(void) const;

	/**
	* @brief Column의 Code group 값을 설정한다.
	* @param[in] nCodeGroup - Code group 값
	*/
	void CodeGroup(int nCodeGroup);

	/**
	* @brief Group 정보가 있는 column인 경우 Column group의 값을 반환한다.
	* @return int - Column group의 값
	*/
	int ColumnGroup(void) const;

	/**
	* @brief Column의 Column group 값을 설정한다.
	* @param[in] nColumnGroup - Column group 값
	*/
	void ColumnGroup(int nColumnGroup);

	/**
	* @brief Column type을 반환한다.
	* @return KEMIOColumnType - Column type
	*/
	KEMIOColumnType ColumnType(void) const;

	/**
	* @brief Column type을 설정한다.
	* @param[in] emColumnType - Column type
	*/
	void ColumnType(KEMIOColumnType emColumnType);

	/**
	* @brief Column의 data type을 반환한다.
	* @return KEMIODataType - Column의 data type
	*/
	KEMIODataType DataType(void) const;

	/**
	* @brief Column의 data type을 설정한다.
	* @param[in] emDataType - Column의 data type
	*/
	void DataType(KEMIODataType emDataType);

	/**
	* @brief Column의 명칭을 반환한다.
	* @return LPCTSTR - Column의 명칭
	*/
	LPCTSTR Name(void) const;

	/**
	* @brief Column의 명칭을 설정한다.
	* @param[in] strName - Column의 명칭
	*/
	void Name(LPCTSTR strName);

	/**
	* @brief Column이 표출 될 이름을 반환한다.
	* @return LPCTSTR - Column이 표출 될 이름
	*/
	LPCTSTR Caption(void) const;

	/**
	* @brief Column이 표출 될 이름을 설정한다.
	* @param[in] strCaption - Column의 표출 명칭
	*/
	void Caption(LPCTSTR strCaption);

	/**
	* @brief Column의 정보를 반환한다.
	* @return LPCTSTR - Column의 정보
	*/
	LPCTSTR ColumnInfo(void) const;

	/**
	* @brief Column의 정보를 설정한다.
	* @param[in] strColumnInfo - Column의 정보
	*/
	void ColumnInfo(LPCTSTR strColumnInfo);


private :
	KEMIOCodeType m_enumCodeType;
	bool m_bDisplay;
	bool m_bFreeze;
	bool m_bPK;

	int m_nDisplayOrder;
	int m_nBaseID;
	int m_nCodeGroup;
	int m_nColumnGroup;

	KEMIOColumnType m_emColumnType;
	KEMIODataType	m_emDataType;

	CString m_strName;
	CString m_strCaption;
	CString m_strColumnInfo;
};
