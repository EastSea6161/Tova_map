/**
 * @file ExpressionElement.h
 * @brief KEMExpressionElement 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "model_common.h"


/**
 * @brief 모델의 수식을 구성하는 기본 요소의 base class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KExpressionElement
{
public:
	KExpressionElement(enum KEMExpressionElementType emElementType);
	KExpressionElement(const KExpressionElement& r);
	virtual ~KExpressionElement(void) = 0;


public :

	/**
	* @brief Expression Element의 종류를 반환한다.
	* @return KEMExpressionElementType - Expression Element의 종류
	*/
	KEMExpressionElementType ElementType(void);

	/**
	* @brief Element를 문자열로 반환한다.
	* @return CString - 요소의 문자열
	*/
	virtual CString ToString(void) = 0;

	/**
	* @brief 요소를 표출 문자열로 반환한다.
	* @return CString - 요소의 표출 문자열
	*/
	virtual CString ToCaption(void) = 0;

	KExpressionElement& operator=(const KExpressionElement& r);

protected :
	KEMExpressionElementType m_emElementType;
};

