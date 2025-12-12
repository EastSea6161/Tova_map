/**
 * @file GGroupExpressionPage.h
 * @brief KGGroupExpressionPage 선언 파일 
 * @author teegee@zaolsoft.com
 * @date 2011.05.04
 * @remark
 */


#pragma once


#include "afxwin.h"
#include "resource.h"


/* Forward declarations */
class KCodeManager;
class KGenerationInfo;
class KIOColumn;
class KIOTable;
class KZoneGroupColumn;


/**
 * @brief Zone group 설정된 목적의 수식입력 Property page class
 * @remark
 * @warning
 * @version 1.0
 * @author teegee@zaolsoft.com
 * @date 2011.05.04
 */
class KGGroupExpressionPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KGGroupExpressionPage)

public:
	KGGroupExpressionPage();
	virtual ~KGGroupExpressionPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6100_GZoneGroupPage };


public :
	/**
	 * @brief GroupExpressionPage에서 수식을 입력 할 GenerationInfo를 설정한다.
	 * @param[in] pGInfo - GenerationInfo 정보
	 */
	void SetGenerationInfo(KGenerationInfo* pGInfo);

	/**
	 * @brief GroupExpressionPage에서 참조 할 zone table을 설정한다.
	 * @param[in] pTable - zone정보를 가지고 있는 KIOTable 정보
	 */
	void SetZoneTable(KIOTable* pTable);

	void SetCodeManager(KCodeManager* pCodeManager);


protected :
	KCodeManager* m_pCodeManager;
	KIOTable* m_pZone;
	KGenerationInfo* m_pGInfo;
	CXTPReportControl m_Report;
	CButton m_btnAdd;
	CButton m_btnDelete;
	CComboBox m_cmbField;
	std::list<KIOColumn*> m_ColumnList;
	std::list<KIOColumn*> m_CurrentColumnList;
	KZoneGroupColumn* m_pExpressionRoot;

	struct GroupColumnInfo {
		KZoneGroupColumn* pColumn;
		CXTPReportRecord* pParent;
	};

	// todo : zone group 데이터 설정(2011.05.04)
    /**
    * @brief 존 그룹정보 초기화
    */
	void initZoneGroup(void);

    /**
    * @brief ReportControl 초기화
    */
	void initReportControl(void);

    /**
    * @brief 필드 추가 
    * @param[in] pParent - Parent Record
    */
	void addField(CXTPReportRecord* pParent);
	void updateReportControl(void);

    /**
    * @brief Update Columns
    */
	void updateReportColumns(std::list<GroupColumnInfo>& remainColumns);

    /**
    * @brief Update Button State
    */
	void updateButtonState(void);

    /**
    * @brief Update Combo List
    */
	void updateComboList(void);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnExpressionEdit(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
};
