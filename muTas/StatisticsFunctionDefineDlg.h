/**
 * @file StatisticsFunctionDefineDlg.h
 * @brief KStatisticsFunctionDefineDlg 선언 파일
 * @author jyk@qbicware.com
 * @date 2012.04.23
 * @remark
 */
#pragma once

class KIOTable;
class KGRegressionExpression;

// KStatisticsFunctionDefineDlg 대화 상자입니다.

class KStatisticsFunctionDefineDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KStatisticsFunctionDefineDlg)

public:
	KStatisticsFunctionDefineDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KStatisticsFunctionDefineDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4142_StatisticsFunctionDefineDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetTable(KIOTable* a_pTable)
	{
		m_pTable = a_pTable;
	}

	void SetExpression(KGRegressionExpression* a_pExpression)
	{
		m_pExpression = a_pExpression;
	}

protected:

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
    * @brief 입력 정보 유효성 검사
    */
	BOOL InvalidateInputInfo();

protected:
	KIOTable* m_pTable;
	KGRegressionExpression* m_pExpression;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1();
	afx_msg void OnDblclkList2();
	afx_msg void OnDblclkList3();
	
protected:
	CString  m_strExpression;
	CListBox m_lstField;
	CListBox m_lstOperator;
	CListBox m_lstFunction;	
public:
	afx_msg void OnBnClickedOk();
};
