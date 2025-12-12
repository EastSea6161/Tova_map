#pragma once
#include "afxwin.h"

class KTarget;
class KGenerationInfo;
class KGRegressionExpression;


// KDefaultTGRegressionPage 대화 상자입니다.

class KDefaultTGRegressionPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KDefaultTGRegressionPage)

public:
	KDefaultTGRegressionPage(KTarget* a_pTarget, KGenerationInfo* a_pGenertaionInfo);   // 표준 생성자입니다.
	virtual ~KDefaultTGRegressionPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8515_DefaultTripGRegressionPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*					m_pTarget;
	KGenerationInfo*			m_pGenerationInfo;
	KGRegressionExpression*		m_pExpression;

protected:
	CListBox m_lstField;
	CListBox m_lstOperator;
	CListBox m_lstFunction;


public:
	void InitDefaultData();

protected:
	void InitFieldList();
	void InitOperatorList();
	void InitFunctionList();

	void ResizeComponent();

protected:
	bool isField(LPCTSTR a_strElement);
	bool isOperator(LPCTSTR a_strElement);
	bool isFunction(LPCTSTR a_strElement);
	std::tstring GetFieldName(LPCTSTR a_strElement);
	std::tstring GetFieldCaption(LPCTSTR a_strElement);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListField();
	afx_msg void OnLbnSelchangeListOperator();
	afx_msg void OnLbnSelchangeListFunction();
	virtual BOOL OnApply();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
};
typedef std::shared_ptr<KDefaultTGRegressionPage> KDefaultTGRegressionPagePtr;