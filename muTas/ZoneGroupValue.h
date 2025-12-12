/**
 * @file ZoneGroupValue.h
 * @brief KZoneGroupValue 선언 파일
 * @author 
 * @date 2011.04.27
 * @remark
 */


#pragma once


#include "model_common.h"
#include "ZoneGroupBase.h"


/* Forward declarations */
class KGBaseExpression;
class KZoneGroupColumn;


/**
 * @brief KZoneGroupExpression에서 column의 값 정보를 가지는 class
 *          ZoneGroupValue는 수식 또는 하위 ZoneGroupColumn 을 가진다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.27
 */
class KZoneGroupValue : public KZoneGroupBase
{
public:
	KZoneGroupValue(void);
	KZoneGroupValue(const KZoneGroupValue& r);
	virtual ~KZoneGroupValue(void);


public :
	
	/**
	* @brief Zone group의 Value를 반환한다.
	* @return int - Zone group value
	*/
	int Value(void);
	
	/**
	* @brief Zone group의 Value를 설정한다.
	* @return nValue - Zone group value
	*/
	void Value(int nValue);

	/**
	* @brief Zone group value의 명칭을 반환한다.
	* @return LPCTSTR - Zone group value의 명칭
	*/
	LPCTSTR ValueName(void);

	/**
	* @brief Zone group value의 명칭을 설정한다.
	* @return strName - Zone group value의 명칭
	*/
	void ValueName(LPCTSTR strName);

	/**
	* @brief 하위 Zone group column을 추가한다.
	* @param[in] strColumnName - Column 명칭
	* @param[in] strColumnCaption - Column의 표출 명칭 
	* @return KZoneGroupColumn* - 추가된 Zone group column 정보
	*/
	KZoneGroupColumn* AddSubColumn(LPCTSTR strColumnName, LPCTSTR strColumnCaption);

	/**
	* @brief 하위 Zone group column을 삭제한다.
	*/
	void RemoveSubColumn(void);

	/**
	* @brief Zone group column을 반환한다.
	* @return KZoneGroupColumn* - Zone group column
	*/
	KZoneGroupColumn* GetSubColumn(void);

	/**
	* @brief 하위 Zone group column을 가지고 있는지 여부를 반환한다.
	* @return bool - Zone group column 존재 여부
	* @remark 
	* @warning 
	*/
	bool HasSubColumn(void);


	/**
	* @brief 수식 정보를 반환한다.
	* @return KGBaseExpression* - 수식 정보
	*/
	KGBaseExpression* ExpressionObject(void);

	/**
	* @brief 수식 정보를 설정한다.
	* @return pExpressionObject - 수식 정보
	*/
	void ExpressionObject(KGBaseExpression* pExpressioinObject);

	/**
	* @brief 새로운 수식 정보를 생성한다.
	* @return KGBaseExpression* - 생성된 수식 정보
	* @remark 
	* @warning 
	*/
	KGBaseExpression* CreateExpressionObject(KEMGenerationModel emGModel);

	KZoneGroupValue& operator=(const KZoneGroupValue& r);


	/**
	* @brief 수식 정보를 설정 여부를 저장 
	* @param[in] a_bDoneExpression - 설정 여부
	*/
	void DoneExpression(bool a_bDoneExpression);

    /**
	* @brief 수식 정보를 설정 여부를 반환
	* @return true/false 수식 정보 설정 여부 
	*/
	bool DoneExpression(void);

protected :
	int     m_nValue;	    //  ZoneGroup의 value는 code값이다.
	CString m_strValueName;	// Zone group value의 code값이 갖는 의미
	KZoneGroupColumn* m_pSubColumn;
	KGBaseExpression* m_pExpressionObject;
	bool    m_bDoneExpression;	// 수식 정보 설정 여부 
};
