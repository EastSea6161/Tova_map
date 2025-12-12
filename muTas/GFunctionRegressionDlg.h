/**
 * @file GFunctionRegressionDlg.h
 * @brief KGFunctionRegressionDlg 선언 파일
 * @author 
 * @date 2011.04.29
 * @remark
 */

#pragma once
#include "afxwin.h"


/* Forward declaratios */
class KGRegressionExpression;
class KTarget;

/**
 * @brief 발생 모델에서 Regression 모형의 수식을 입력하는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.29
 */
class KGFunctionRegressionDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KGFunctionRegressionDlg)

public:
	KGFunctionRegressionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGFunctionRegressionDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_6150_GFunctionGressionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    /// DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public :

	void SetTarget(KTarget* a_pTarget);
	/**
	* @brief Dialog에서 입력 할 Regression 모형의 수식을 설정한다.
	* @param[in] pExpression - Regression 모형 수식
	*/
	void Expression(KGRegressionExpression* pExpression);

	void SetExpressionView(LPCTSTR a_strExpression);

protected :

    /**
    * @brief SED(Vector) 필드 정보 초기 설정
    */
	void initFieldList(void);

    /**
    * @brief Operator(사칙연산) 리스트 초기 설정
    */
	void initOperatorList(void);

    /**
    * @brief 함수 리스트 초기 설정
    */
	void initFunctionList(void);

    /**
    * @brief 필드 여부 체크
    * @param[in] strElement - 수식 Element
    * @return 필드 여부
    */
	bool isField(LPCTSTR a_strElement);

    /**
    * @brief 연산자 여부 체크
    * @param[in] strElement - 수식 Element
    * @return 연산자 여부
    */
	bool isOperator(LPCTSTR a_strElement);

    /**
    * @brief 함수 여부 체크
    * @param[in] strElement - 수식 Element
    * @return 함수 여부
    */
	bool isFunction(LPCTSTR a_strElement);

    /**
    * @brief 필드명 검색
    * @param[in] strElement - 수식 Element
    * @return 필드명
    */
	std::tstring getFieldName(LPCTSTR a_strElement);

    /**
    * @brief 필드의 캡션 정보 검색
    * @param[in] strElement - 수식 Element
    * @return 필드의 캡션
    */
	std::tstring getFieldCaption(LPCTSTR a_strElement);


protected:
	KTarget*				m_pTarget;
	CString					m_strExpression;
	CListBox				m_lstField;
	CListBox				m_lstOperator;
	CListBox				m_lstFunction;	
	KGRegressionExpression* m_pExpression;


protected:
    /**
    * @brief 필드의 캡션 정보 검색
    * @param[in] strElement - 수식 Element
    * @return 필드의 캡션
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDblclkList1();
	afx_msg void OnDblclkList2();
	afx_msg void OnDblclkList3();
};
