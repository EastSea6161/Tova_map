/**
 * @file GUnitExpression.h
 * @brief KGUnitExpression 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "GBaseExpression.h"


/* Forward declarations */
class KExpressionElementNumber;
class KExpressionElementField;


/**
 * @brief 발생모형에서 Unit 모델의 수식을 저장 하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KGUnitExpression : public KGBaseExpression
{
public:
	KGUnitExpression(void);
	KGUnitExpression(double dNumber, LPCTSTR strFieldName, LPCTSTR strFieldCaption);
	KGUnitExpression(const KGUnitExpression& r);
	
	virtual ~KGUnitExpression(void);

public :

	/**
	* @brief Unit 모형 수식의 상수를 반환한다.
	* @return double - 상수
	*/
	double Number(void);

	/**
	* @brief Unit 모형 수식의 상수를 설정한다.
	* @param[in] dNumber - 상수
	*/
	void Number(double a_dNumber);

	/**
	* @brief Unit 모형 수식의 입력 Column을 설정한다.
	* @param[in] strFieldName - Column 명칭
	* @param[in] strFieldCaption - Column의 표출 명칭 
	*/
	void Field(LPCTSTR a_strFieldName, LPCTSTR a_strFieldCaption);

	/**
	* @brief Unit 모형 수식의 Column 명칭을 반환한다.
	* @return LPCTSTR - Column 명칭
	*/
	LPCTSTR FieldName(void);

	/**
	* @brief Unit 모형 수식의 Column 표출 명칭을 반환한다.
	* @return LPCTSTR - Column의 표출 명칭
	*/
	LPCTSTR FieldCaption(void);

	/**
	* @brief Unit 모형의 수식을 문자열로 반환한다.
	* @return CString - Unit 모형의 수식 문자열
	*/
	virtual CString GetExpression(void);

	/**
	* @brief Unit 모형의 수식을 표출 문자열로 반환한다.
	* @return CString - Unit 모형의 수식 표출 문자열
	*/
	virtual CString GetCaption(void);

	KGUnitExpression& operator=(const KGUnitExpression& r);


protected :
	KExpressionElementNumber* m_pNumberElement;
	KExpressionElementField* m_pFieldElement;
};

