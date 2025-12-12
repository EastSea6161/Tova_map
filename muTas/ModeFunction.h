/**
 * @file ModeFunction.h
 * @brief KModeFunction 선언 파일
 * @author 
 * @date 2011.06.03
 * @remark
 */


#pragma once


/* Forward declarations */
class KExpressionElement;


/**
 * @brief 수단선택 모형에서 Mode의 Utility function 정보를 저장하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.03
 */
class KModeFunction
{
public:
	KModeFunction(void);
	KModeFunction(const KModeFunction& r);
	~KModeFunction(void);

public :

	/**
	* @brief 수단선택 Function에 상수를 추가한다.
	* @param[in] dElement - double형 상수
]	*/
	void AddNumber(double dElement);

	/**
	* @brief 수단선택 Function에 연산자를 추가한다.
	* @param[in] szOperator - 연산자
	*/
	void AddOperator(TCHAR szOeprator);

	/**
	* @brief 수단선택 Function에 함수를 추가한다.
	* @param[in] strFunction - 함수
	*/
	void AddFunction(LPCTSTR strFunction);

	/**
	* @brief 수단선택 Function에 Column을 추가한다.
	* @param[in] strFieldName - Column 명칭
	* @param[in] strFieldCaption - Column의 표출 명칭 
	*/
	void AddField(LPCTSTR strFieldName, LPCTSTR strFieldCaption);

	/**
	* @brief 수단선택 Function을 삭제한다.
	*/
	void ClearElements(void);

	/**
	* @brief 수단선택 Function을 문자열로 반환한다. 
	* @return CString - 수단선택 Function
	*/
	CString GetExpression(void);

	/**
	* @brief 수단선택 Function의 표출 문자열을 반환한다.
	* @return CString - 수단선택 Function의 표출 문자열
	*/
	CString GetCaption(void);

	/**
	* @brief 수단선택 Function에 입력된 요소의 list를 반환한다.
	* @return std::list<KExpressionElement*>& - 요소의 list
	*/
	const std::list<KExpressionElement*>& GetExpressionList(void);

	KModeFunction& operator=(const KModeFunction& expression);


protected :
	std::list<KExpressionElement*> m_ElementList;

    // Expression 전체를 수정해야 하나. 
    // 현재 시점(2012.05.30)에서 수정을 최소화하기 위해 아래 함수와 멤버 변수를 추가
public:
    CString GetCaptionExpression()
    {
        return m_strCaptionExpression;
    }
    void SetCaptionExpression(CString a_strCaptionExpression)
    {
        m_strCaptionExpression = a_strCaptionExpression;
    }
protected:
    CString   m_strCaptionExpression;
};

