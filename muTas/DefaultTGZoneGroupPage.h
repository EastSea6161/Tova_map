#pragma once
#include "DBaseGeneration.h"

class KCodeManager;
class KGenerationInfo;
class KIOColumn;
class KIOTable;
class KZoneGroupColumn;
class KTarget;
class KGUnitExpression;
class KGRegressionExpression;
class KGCategoryExpression;

// KDefaultTGZoneGroupPage 대화 상자입니다.

class KDefaultTGZoneGroupPage :  public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KDefaultTGZoneGroupPage)

public:
	KDefaultTGZoneGroupPage(KTarget* a_pTarget, KGenerationInfo* a_pGenerationInfo);   // 표준 생성자입니다.
	virtual ~KDefaultTGZoneGroupPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8515_DefaultTripGZoneGroupPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*				m_pTarget;
	KGenerationInfo*		m_pGenerationInfo;
	KIOTable*				m_pKZone;
	KCodeManager*			m_pKCodeManager;
	std::list<KIOColumn*>	m_lstColumnList;
	std::list<KIOColumn*>	m_lstCurrentColumnList;
	KZoneGroupColumn*		m_pKExpressionRoot;

	KGUnitExpression*		m_pUnitCopyExpression;
	KGRegressionExpression*	m_pRegressionCopyExpression;
	KGCategoryExpression*	m_pCategoryCopyExpression;
	
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CComboBox				m_cboField;

protected:

	void InitZoneGroup(void);
	void InitReportControl(void);
	void UpdateReportControl(BOOL a_bExpandAll = TRUE);

	void UpdateButtonState(void);
	void UpdateComboList(void);
	void UpdateReportColumns(std::list<TGroupColumnInfo>& a_lstRemainColumns);

	void AddField(CXTPReportRecord* a_pParent);

	bool GetParentsData(CXTPReportRecord* a_pParent, std::map<CString, int>& a_mapCode);
	void GetSubCode(CString a_strColumnName, std::map<CString, int> a_mapPCodeInfo, std::set<int>& a_setCode);

	BOOL CheckReportColumns( std::list<TGroupColumnInfo>& a_lstRemainColumns);
	void ResizeComponent();

protected:

	void ClearCopyData();

	BOOL InsertCopyData();

	BOOL InsertCopySubData(std::list<TGroupColumnInfo>& a_lstRemainColumns);

public:
	void InitDefaultData();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportSelChanged(NMHDR* a_pNMHDR, LRESULT* a_pResult);
	afx_msg LRESULT OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg void OnPaint();
	virtual BOOL OnApply();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnReportItemRClick(NMHDR * pNotifyStruct, LRESULT * result);
	
};

typedef std::shared_ptr<KDefaultTGZoneGroupPage> KDefaultTGZoneGroupPagePtr;