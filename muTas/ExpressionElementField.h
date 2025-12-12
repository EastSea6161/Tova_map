/**
 * @file ExpressionElementField.h
 * @brief KExpressionElementField 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "expressionelement.h"


/**
 * @brief 모델의 수식에서 수식의 Variable에 해당 하는 field를 구성하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KExpressionElementField : public KExpressionElement
{
public:
	KExpressionElementField(void);
	KExpressionElementField(LPCTSTR strFieldName, LPCTSTR strFielCaption);
	KExpressionElementField(const KExpressionElementField& r);
	virtual ~KExpressionElementField(void);


public :

	/**
	* @brief Column 명칭을 반환한다.
	* @return LPCTSTR - Column 명칭
	*/
	LPCTSTR FieldName(void);
	
	/**
	* @brief Column 명칭을 설정한다.
	* @return strFieldName - Column 명칭
	*/
	void FieldName(LPCTSTR strFieldName);

	/**
	* @brief Column 표출 명칭을 반환한다.
	* @return LPCTSTR - Column 표출 명칭
	*/
	LPCTSTR FieldCaption(void);
	
	/**
	* @brief Column 표출 명칭을 설정한다.
	* @return strFieldCaption - Column 표출 명칭
	*/
	void FieldCaption(LPCTSTR strFieldCaption);

	/**
	* @brief Element를 문자열로 반환한다.
	* @return CString - 요소의 문자열
	*/
	virtual CString ToString(void);

	/**
	* @brief 요소를 표출 문자열로 반환한다.
	* @return CString - 요소의 표출 문자열
	*/
	virtual CString ToCaption(void);

	KExpressionElementField& operator=(const KExpressionElementField& r);

protected :
	CString m_strFieldName;
	CString m_strFieldCaption;
};

