/**
 * @file GSimpleRegressionPage.h
 * @brief KGSimpleRegressionPage 선언 파일
 * @author 
 * @date 2011.04.29
 * @remark
 */
#pragma once

#include "GFormulaDefaultDlg.h"
#include "DBaseFGenerationModel.h"


/* Forward declaratios */
class KGenerationInfo;
class KGRegressionExpression;
class KTarget;

/**
 * @brief 발생 모델에서 Zone group이 설정되지 않은 Regression 모형의 수식을 입력하는 property page class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.29
 */

class KGSimpleRegressionPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGSimpleRegressionPage)

public:
	KGSimpleRegressionPage(KGFormulaDefaultDlgPtr* a_spKGFormulaDefaultDlg);
	virtual ~KGSimpleRegressionPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6120_GSimpleRegressionPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief 발생모형 정보를 설정한다.
	* @param[in] pGInfo - 발생모형 정보
	*/

	void SetTarget(KTarget* a_pTarget);

	void SetGenerationInfo(KGenerationInfo* a_pGInfo);

	void SetParentSheet(CXTPPropertySheet* a_pParentSheet)
	{
		m_pParentSheet = a_pParentSheet;
	}

	void SetDefaultFreightInfo(KFPAGenerationparaInfo a_oParaInfo)
	{
		m_oParameterInfo = a_oParaInfo;
	}

	void SetDefaultPassengerInfo(KGenerationInfo* a_pGInfo)
	{
		m_pDefaultGInfo = a_pGInfo;
	}

protected:

    /**
    * @brief 필드명 검색
    * @param[in] strElement - 수식 Element
    * @return 필드명
    */
	std::tstring GetFieldName(LPCTSTR a_strElement);

    /**
    * @brief 필드의 캡션 정보 검색
    * @param[in] strElement - 수식 Element
    * @return 필드의 캡션
    */
	std::tstring GetFieldCaption(LPCTSTR a_strElement);

	/**
    * @brief SED(Vector) 필드 정보 초기 설정
    */
	void InitFieldList(void);

    /**
    * @brief Operator(사칙연산) 리스트 초기 설정
    */
	void InitOperatorList(void);

    /**
    * @brief 함수 리스트 초기 설정
    */
	void InitFunctionList(void);
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
    * @brief 입력 정보 유효성 검사
     */
	BOOL InvalidateInputInfo();

	void SetPassengerDefault();

	void SetFreightDefault();

protected :

	KTarget*                    m_pTarget;
	KGenerationInfo*			m_pGInfo;
	KGenerationInfo*			m_pDefaultGInfo;
	KGRegressionExpression*		m_pExpression;
	CString						m_strExpression;
	CListBox					m_lstField;
	CListBox					m_lstOperator;
	CListBox					m_lstFunction;	
	CXTPPropertySheet*			m_pParentSheet;
	KGFormulaDefaultDlgPtr*     m_spKGFormulaDefaultDlg;
	KFPAGenerationparaInfo		m_oParameterInfo;

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1();
	afx_msg void OnDblclkList2();
	afx_msg void OnDblclkList3();
	afx_msg void OnLbnSelchangeList1();
	virtual BOOL OnApply();
	afx_msg void OnBnClickedButtonDefault();

public:
	virtual BOOL OnSetActive();
};
