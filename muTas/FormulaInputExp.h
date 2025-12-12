#pragma once
#include "FormulaExpression.h"

// KFormulaInputExp 대화 상자입니다.

class KFormulaInputExp : public KDialogEx
{
	DECLARE_DYNAMIC(KFormulaInputExp)

public:
	KFormulaInputExp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KFormulaInputExp();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6525_FORMULA_INPUT_EXP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
    void InitComboCtrl(void);

private:
    int       m_nIdxUserColumn;

private:
    CString   m_strExpression;
    CListBox  m_lstField;
    CListBox  m_lstOperator;
    CListBox  m_lstFunction;

    CComboBox m_cboUserColumn;

private:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnDblclkListField();
    afx_msg void OnDblclkListOperator();
    afx_msg void OnDblclkListFunction();

private:
    std::vector<CString>       m_vecFunction;
    std::vector<CString>       m_vecOperator;
    std::vector<TFormulaField> m_vecFormulaField;
public:
    void UsableFunction(std::vector<CString> a_vecFunction);
    void UsableOperator(std::vector<CString> a_vecOperator);
    void UsableField   (std::vector<TFormulaField> a_vecFormulaField);
    inline int IndexUserColumn(void){ return m_nIdxUserColumn; }

public:
    CString GetFormulaCaption();
};
