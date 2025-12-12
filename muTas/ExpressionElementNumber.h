/**
 * @file ExpressionElementNumber.h
 * @brief KExpressionElementNumber 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "expressionelement.h"


/**
 * @brief 모델의 수식에서 숫자값을 구성하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KExpressionElementNumber : public KExpressionElement
{
public:
	KExpressionElementNumber(void);
	KExpressionElementNumber(double dValue);
	KExpressionElementNumber(const KExpressionElementNumber& r);
	virtual ~KExpressionElementNumber(void);


public :

	/**
	* @brief double형의 숫자값을 반환한다.
	* @return double - 숫자값
	*/
	double Number(void);

	/**
	* @brief double형의 숫자값을 설정한다.
	* @return dValue - 숫자값
	*/
	void Number(double dValue);

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

	KExpressionElementNumber& operator=(const KExpressionElementNumber& r);

protected :
	double m_dValue;
};

