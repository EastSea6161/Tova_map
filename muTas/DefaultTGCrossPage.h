#pragma once

#include "KGCategoryExpression.h"
#include "afxwin.h"
class KTarget;
class KGenerationInfo;

// KDefaultTGCrossPage 대화 상자입니다.

class KDefaultTGCrossPage :public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KDefaultTGCrossPage)

public:
	KDefaultTGCrossPage(KTarget* a_pTarget, KGenerationInfo* a_pGenerationInfo);   // 표준 생성자입니다.
	virtual ~KDefaultTGCrossPage();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_8515_DefaultTripGCrossClassficationPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*			m_pTarget;
	KGenerationInfo*	m_pGenerationInfo;

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CButton m_chkSex;
	CButton m_chkAge;
	CComboBox m_cboCategoryClass;

protected:
	std::vector<TCategoryClass> m_vecCategoryClass;
	std::vector<TAgeRange>	    m_vecAgeRange;
	KGCategoryExpression*		m_pGCategoryExpression;
	TCategoryClass*				m_pCategoryClass;

public:
	void InitDefaultData();


protected:
	void ResizeComponent();

protected:
	void loadCategoryClass();
	void loadCategoryAgeGroup(TCategoryClass* pCategory);
	void loadCategoryValue();

	int GetCategoryIndex(int category_id);
	void SelchangeComboAgeGroup();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboAgeGroup();
	virtual BOOL OnApply();
public:
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

typedef std::shared_ptr<KDefaultTGCrossPage> KDefaultTGCrossPagePtr;