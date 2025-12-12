/**
 * @file ExpressionElementFunction.h
 * @brief KExpressionElementFunction 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "expressionelement.h"


/**
 * @brief 모델의 수식에서 함수를 구성하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KExpressionElementFunction : public KExpressionElement
{
public:
	KExpressionElementFunction(void);
	KExpressionElementFunction(LPCTSTR strFunction);
	KExpressionElementFunction(const KExpressionElementFunction& r);
	virtual ~KExpressionElementFunction(void);


public :

	/**
	* @brief 함수를 문자열로 반환한다.
	* @return LPCTSTR - 함수의 문자열
	*/
	LPCTSTR Function(void);

	/**
	* @brief 함수를 문자열을 설정한다.
	* @return strFunction - 함수의 문자열
	*/
	void Function(LPCTSTR strFunction);

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

	KExpressionElementFunction& operator=(const KExpressionElementFunction& r);

protected :
	CString m_strFunction;
};

