/**
 * @file MDecisionTreePage.h
 * @brief KMDecisionTreePage 선언 파일
 * @author 
 * @date 2011.06.07
 * @remark
 */


#pragma once


#include "afxwin.h"
#include "model_common.h"


/* Forward declarations */
class KMode;
class KModeTree;
class KTarget;


/**
 * @brief 수단 선택 모형에서 목적 별 수단 Tree를 설정하는 Property Page class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.07
 */
class KMDecisionTreePage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KMDecisionTreePage )

public:
	KMDecisionTreePage ();
	virtual ~KMDecisionTreePage ();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6510_MDecisionTreeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public :

	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year 정보
	*/
	void Target(KTarget* pTarget);

	/**
	* @brief 수단선택 모델을 설정한다.
	* @param[in] emMModel - 수단선택 모델
	*/
	void ModeChoiceModel(KEMModeChoiceModel emMModel);

	/**
	* @brief 수단선택 Tree를 반환한다.
	* @return KModeTree* - 수단선택 Tree
	*/
	KModeTree* ModeTree(void) const;

	/**
	* @brief 수단선택 Tree를 설정한다.
	* @param[in] pModeTree - 수단선택 Tree
	*/
	void ModeTree(const KModeTree* pModeTree);


	void SetParentSheet(CXTPPropertySheet* a_pParentSheet)
	{
		m_pParentSheet = a_pParentSheet;
	}

protected :
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CComboBox m_cmbMode;
	CButton m_btnAdd;
	CButton m_btnRemove;
	KTarget* m_pTarget;
	KEMModeChoiceModel m_emMModel;
	KModeTree* m_pModeTree;
	std::list<KMode*> m_ModeList;
	std::list<KMode*> m_RemainModeList;

	CXTPPropertySheet* m_pParentSheet;

    /**
    * @brief Report Control 초기화
    */
	void initReportControl(void);

    /**
    * @brief Report Data Update
    */
	void updateReportRecords(void);

    /**
    * @brief 수단 정보 초기화
    */
	void initModeList(void);

    /**
    * @brief 콤보 박스 초기화
    */
	void updateModeCombobox(void);

    /**
    * @brief 수단 정보 삭제(Tree)
    */
	void removeModeFromList(KMode* pMode);

    /**
    * @brief 수단 정보 추가(Tree)
    */
	void addNotNestedLogit(void);

    /**
    * @brief 수단 정보 추가(Tree)
    */
	void addNestedLogit(void);

    /**
    * @brief 수단 정보 제거
    */
	void removeNotNestedLogit(void);

    /**
    * @brief 수단 정보 제거
    */
	void removeNestedLogit(void);

    /**
    * @brief 버튼 상태 정보 변경
    */
	void updateButtonState(void);

	/**
    * @brief 입력 정보 유효성 검사
    */
	BOOL InvalidateInputInfo();

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnBnClickedButton3();
	afx_msg void OnReportSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
};
