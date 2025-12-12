/**
 * @file ExpressionChecker.h
 * @brief KExpressionChecker 선언 파일
 * @author 
 * @date 2011.06.10
 * @remark
 */


#pragma once


/**
 * @brief 주어진 문자열이 연산자, 함수, 필드 여부를 확인하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.10
 */
class KExpressionChecker
{
public:
	KExpressionChecker(void);
	~KExpressionChecker(void);

public :

	/**
	* @brief Column 명칭을 추가한다.
	* @param[in] strField - Column 명칭
	*/
	void AddField(LPCTSTR strField);

	/**
	* @brief 함수를 추가한다.
	* @param[in] strFunction - 함수
	*/
	void AddFunction(LPCTSTR strFunction);

	/**
	* @brief 연산자를 추가한다.
	* @param[in] strOperatior - 연산자
	*/
	void AddOperator(LPCTSTR strOperator);

	/**
	* @brief 입력된 요소가 Column 요소 여부를 반환한다.
	* @return bool - Column 요소 여부
	*/
	bool IsField(LPCTSTR strElement);
	
	/**
	* @brief 입력된 요소가 Column 요소 여부를 반환한다.
	*/
	void IsField(LPCTSTR strElement, int &a_nFlag);

	/**
	* @brief 입력된 요소가 함수 요소 여부를 반환한다.
	* @return bool - 함수 요소 여부
	*/
	bool IsFunction(LPCTSTR strElement);
	
	/**
	* @brief 입력된 요소가 연산자 요소 여부를 반환한다.
	* @return bool - 연산자 요소 여부
	*/
	bool IsOperator(LPCTSTR strElement);

	/**
	* @brief Column 요소 목록을 반환한다.
	* @return std::list<std::tstring>& - Column 요소 list
	*/
	const std::list<std::tstring>& FieldList(void);

	/**
	* @brief 함수 요소 목록을 반환한다.
	* @return std::list<std::tstring>& - 함수 요소 list
	*/
	const std::list<std::tstring>& FunctionList(void);

	/**
	* @brief Column 연산자 목록을 반환한다.
	* @return std::list<std::tstring>& - 연산자 요소 list
	*/
	const std::list<std::tstring>& OperatorList(void);

protected :
	std::list<std::tstring> m_FieldList;
	std::list<std::tstring> m_FunctionList;
	std::list<std::tstring> m_OperatorList;
};

