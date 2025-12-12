/**
 * @file ExpressionElementOperator.h
 * @brief KExpressionElementOperator 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "expressionelement.h"


/**
 * @brief 모델의 수식에서 연산자를 구성하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KExpressionElementOperator : public KExpressionElement
{
public:
	KExpressionElementOperator(void);
	KExpressionElementOperator(TCHAR szOperator);
	KExpressionElementOperator(const KExpressionElementOperator& r);
	virtual ~KExpressionElementOperator(void);

public :

	/**
	* @brief 연산자 문자를 반환한다.
	* @return TCHAR - 연산자 문자
	*/
	TCHAR Operator(void);
	
	/**
	* @brief 연산자 문자를 설정한다.
	* @return szOperator - 연산자 문자
	*/
	void Operator(TCHAR szOperator);

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

	KExpressionElementOperator& operator=(const KExpressionElementOperator& r);

protected :
	TCHAR m_szOperator;
};

