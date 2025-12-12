#pragma once
#include "afxwin.h"

class KTarget;
class KGenerationInfo;
class KGUnitExpression;

// KDefaultTGUnitPage 대화 상자입니다.

class KDefaultTGUnitPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KDefaultTGUnitPage)

public:
	KDefaultTGUnitPage(KTarget* a_pTarget, KGenerationInfo* a_pGInfo);   // 표준 생성자입니다.
	virtual ~KDefaultTGUnitPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8515_DefaultTripGUintPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*			m_pTarget;
	KGenerationInfo*	m_pGenerationInfo;
	KGUnitExpression*	m_pKExpression;

protected:
	KIOColumn* m_pColumn;

protected:
	CListBox m_lstField;

protected:
	void InitList();


public:
	void InitDefaultData();

protected:
	void ResizeComponent();

public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnLbnDblclkList();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
typedef std::shared_ptr<KDefaultTGUnitPage> KDefaultTGUnitPagePtr;