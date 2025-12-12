/**
 * @file GZoneGroupPage.h
 * @brief KGZoneGroupPage 선언 파일 
 * @author 
 * @date 2011.05.04
 * @remark
 */

#pragma once


#include "afxwin.h"
#include "resource.h"
#include "DBaseGeneration.h"


/* Forward declarations */
class KCodeManager;
class KGenerationInfo;
class KIOColumn;
class KIOTable;
class KZoneGroupColumn;
class KGBaseExpression;
class KTarget;class KGUnitExpression;
class KGRegressionExpression;
class KGCategoryExpression;



/**
 * @brief Zone group 설정된 목적의 수식입력 Property page class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.04
 */
class KGZoneGroupPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGZoneGroupPage)

public:
	KGZoneGroupPage();
	virtual ~KGZoneGroupPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6100_GZoneGroupPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :
	/**
	 * @brief GroupExpressionPage에서 수식을 입력 할 GenerationInfo를 설정한다.
	 * @param[in] pGInfo - GenerationInfo 정보
	 */
	void SetGenerationInfo(KGenerationInfo* a_pGInfo);

	void SetParentSheet(CXTPPropertySheet* a_pParentSheet)
	{
		m_pParentSheet = a_pParentSheet;
	}

	/**
	 * @brief GroupExpressionPage에서 참조 할 zone table을 설정한다.
	 * @param[in] pTable - zone정보를 가지고 있는 KIOTable 정보
	 */
	void SetZoneTable(KIOTable* a_pTable);

	void SetTarget(KTarget* a_pTarget);

	void SetCodeManager(KCodeManager* a_pCodeManager);

	void SetDefaultPassengerInfo(KGenerationInfo* a_pGenerationInfo)
	{
		m_pDefaultGInfo = a_pGenerationInfo;
	}

protected:
	// todo : zone group 데이터 설정(2011.05.04)
    
    /**
    * @brief Update Columns
    */
	void UpdateReportColumns(std::list<TGroupColumnInfo>& a_lstRemainColumns);


	BOOL CheckReportColumns( std::list<TGroupColumnInfo>& a_lstRemainColumns);

    /**
    * @brief Update Button State
    */
	void UpdateButtonState(void);
	/**
    * @brief 존 그룹정보 초기화
    */
	void InitZoneGroup(void);
	/**
    * @brief ReportControl 초기화
    */
	void InitReportControl(void);

	void UpdateReportControl(BOOL a_bExpandAll = TRUE);
	/**
    * @brief Update Combo List
    */
	void UpdateComboList(void);
	/**
    * @brief 필드 추가 
    * @param[in] pParent - Parent Record
    */
	void AddField(CXTPReportRecord* a_pParent);

	/**
    * @brief 입력 정보 유효성 검사
    */
	BOOL InvalidateInputInfo();

	bool GetParentsData(CXTPReportRecord* a_pParent, std::map<CString, int>& a_mapCode);

	void ClearCopyData();

	BOOL InsertCopyData();

	BOOL InsertCopySubData(std::list<TGroupColumnInfo>& a_lstRemainColumns);
	

protected:
	KCodeManager*			m_pKCodeManager;
	KTarget*				m_pTarget;
	KIOTable*				m_pKZone;
	KGenerationInfo*		m_pKGInfo;
	KGenerationInfo*		m_pDefaultGInfo;
	std::list<KIOColumn*>	m_lstColumnList;
	std::list<KIOColumn*>	m_lstCurrentColumnList;
	KZoneGroupColumn*		m_pKExpressionRoot;
	
	CXTPPropertySheet*		m_pParentSheet;

	KGUnitExpression*		m_pUnitCopyExpression;
	KGRegressionExpression*	m_pRegressionCopyExpression;
	KGCategoryExpression*	m_pCategoryCopyExpression;

protected:
	/*윈도우즈 컨트롤 변수*/
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CButton m_btnAdd;
	CButton m_btnDelete;
	CComboBox m_cboField;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportSelChanged(NMHDR* a_pNMHDR, LRESULT* a_pResult);
	afx_msg LRESULT OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg void OnPaint();
	virtual BOOL OnApply();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnReportItemRClick(NMHDR * pNotifyStruct, LRESULT * result);

};
