#pragma once


// KThemeFunctionDlg 대화 상자입니다.

class KThemeFunctionDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KThemeFunctionDlg)

public:
	KThemeFunctionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeFunctionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_CalculateFunctionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	public :

	/**
	* @brief 수식을 문자열로 반환한다.
	* @return LPCTSTR - 수식 문자열
	*/
	LPCTSTR Expression(void);

	/**
	* @brief 수식 문자열을 설정한다.
	* @param[in] strExpression - 수식 문자열
	*/
	void Expression(LPCTSTR strExpression);

	/**
	* @brief 수식에 사용되는 Column list 설정한다.
	* @param[in] fieldList - Column list
	*/
	void FieldList(const std::list<std::tstring>& fieldList);

	/**
	* @brief 수식에 사용하는 Function list를 설정한다.
	* @param[in] functionList - Function list
	*/
	void FunctionList(const std::list<std::tstring>& functionList);

	/**
	* @brief 수식에 사용하는 연산자 list를 설정한다.
	* @param[in] operatorList - 연산자 list
	*/
	void OperatorList(const std::list<std::tstring>& operatorList);

protected:
	CString m_strExpression;
	CListBox m_lstField;
	CListBox m_lstOperator;
	CListBox m_lstFunction;
	std::list<std::tstring> m_FieldList;
	std::list<std::tstring> m_FunctionList;
	std::list<std::tstring> m_OperatorList;

    /**
    * @brief Field List Box 
    */
	void updateFieldListBox(void);

    /**
    * @brief Operator List Box 
    */
	void updateOperatorListBox(void);

    /**
    * @brief Function List Box 
    */
	void updateFunctionListBox(void);


protected:
	void ResizeComponent();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkList1();
	afx_msg void OnDblclkList2();
	afx_msg void OnDblclkList3();
};
