/**
 * @file GRegressionExpression.h
 * @brief KGRegressionExpression 선언 파일
 * @author 
 * @date 2011.05.02
 * @remark
 */


#pragma once


#include "GBaseExpression.h"


/* Forward delcarations */
class KExpressionElement;


/**
 * @brief 발생모형에서 Regression 모델의 수식을 저장 하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.02
 */
class KGRegressionExpression :	public KGBaseExpression
{
public:
	KGRegressionExpression(void);
	KGRegressionExpression(const KGRegressionExpression& r);
	virtual ~KGRegressionExpression(void);

public :

	KGRegressionExpression& operator=(const KGRegressionExpression& expression);

	/**
	* @brief Regression 모형 수식에 상수를 추가한다.
	* @param[in] double dElement - 상수
	*/
	void AddNumber(double a_dElement);

	/**
	* @brief Regression 모형 수식에 연산자를 추가한다.
	* @param[in] szOperator - 연산자
	*/
	void AddOperator(TCHAR a_szOeprator);

	/**
	* @brief Regression 모형 수식에 function을 추가한다.
	* @param[in] strFunction - Function 명칭
	*/
	void AddFunction(LPCTSTR a_strFunction);

	/**
	* @brief Regression 모형 수식에 Column을 추가한다.
	* @param[in] strFieldName - Column의 명칭
	* @param[in] strFieldCaption - Column의 표출 명칭 
	*/
	void AddField(LPCTSTR a_strFieldName, LPCTSTR a_strFieldCaption);

	/**
	* @brief 현재 수식의 내용을 초기화 한다.
	*/
	void ClearElements(void);

	/**
	* @brief 현재의 수식을 문자열로 반환한다.
	* @return CString - 수식 문자열
	*/
	virtual CString GetExpression(void);

	/**
	* @brief 현재의 수식을 표출하기 위한 문자열로 반환한다.
	* @return CString - 수식 표출 문자열
	*/
	virtual CString GetCaption(void);

	/**
	* @brief 수식의 요소 목록의 참조를 반환한다.
	* @return std::list<KExpressionElement*>& - 수식의 요소 목록
	*/
	const std::list<KExpressionElement*>& GetExpressionList(void);
	
    
protected :
	std::list<KExpressionElement*> m_ElementList;
};

