/**
 * @file MDefineVariableDlg.h
 * @brief KMDefineVariableDlg 선언 파일
 * @author 
 * @date 2011.06.02
 * @remark
 */


#pragma once


#include "afxwin.h"
#include "ModeChoiceInfo.h"

/* Forward declarations */
class KCodeGroup;
class KIOColumn;
class KMBaseVariable;
class KModeFunction;
class KTarget;
class KExpressionChecker;


/**
 * @brief 수단 선택 모형에서 Define variable 설정 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.02
 */
class KMDefineVariableDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KMDefineVariableDlg)

public:
	KMDefineVariableDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMDefineVariableDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6520_MDefineVariableDlg };

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
	* @brief Group 여부를 반환한다.
	* @return bool - Group 여부
	*/
	bool Group(void);

	/**
	* @brief Group 여부를 설정한다.
	* @param[in] bGroup - Group 여부
	*/
	void Group(bool bGroup);

	/**
	* @brief Group Column을 반환한다.
	* @return KIOColumn* - Column 정보
	*/
	KIOColumn* GroupColumn(void);

	/**
	* @brief Group Column을 설정한다.
	* @param[in] pColumn - Column 정보
	*/
	void GroupColumn(KIOColumn* pColumn);
	//int CodeGroup(void);
	//void CodeGroup(int nCodeGroup);

	/**
	* @brief 수단선택 정보를 추가한다.
	* @param[in] pMCInfo - 수단선택 정보
	*/
	void AddModeChoiceInfo(KModeChoiceInfo* pMCInfo);

protected :
	CButton m_btnGroup;
	CStatic m_stcGroupLabel;
	CComboBox m_cmbGroupColumn;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	KTarget* m_pTarget;
	bool m_bGroup;
	KIOColumn* m_pGroupColumn;
	//int m_nCodeGroup;
	std::list<KIOColumn*> m_ColumnList;
	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator> m_VariableMap;
	KExpressionChecker* m_pExpressionChecker;
	std::map<int, std::map<int, CString>> m_mapDefaultFunction;

    /**
    * @brief 수식 정보 검사
    */
	void initExpressionChecker(void);

    /**
    * @brief Group Colum List 초기화
    */
	void initGroupColumnList(void);

    /**
    * @brief Group Colum 콤보 박스 초기화
    */
	void updateGroupColumnCombobox(void);

    /**
    * @brief 변수 정보 초기화
    */
	void updateVariableMap(void);

    /**
    * @brief Report Control 초기화
    */
	void initReportControl(void);

    /**
    * @brief Report Data Update
    */
	void updateReportRecord(void);

    /**
    * @brief 수식 정보 Update
    */
	void updateReportRecordSimple(CXTPReportRecord* pParent, KMBaseVariable* pVariable);

    /**
    * @brief 수식 정보 Update
    */
	void updateReportRecordSimpleMode(CXTPReportRecord* pParent, KMBaseVariable* pVariable);

    /**
    * @brief 수식 정보 Update
    */
	void updateReportRecordGroup(CXTPReportRecord* pParent, int nGroupCode, KMBaseVariable* pVariable);

    /**
    * @brief 수식 정보 Update
    */
	void updateReportRecordGroupMode(CXTPReportRecord* pParent, KMBaseVariable* pVariable);

    /**
    * @brief 수식 정보(사용자 입력 정보) 적용
    */
	bool applySimpleVariable(CXTPReportRecord* pRoot);

    /**
    * @brief 수식 정보(사용자 입력 정보) 적용
    */
	bool applyGroupVariable(CXTPReportRecord* pRoot);

    /**
    * @brief 함수 수식 정보 생성
    */
	void generateModeFunction(LPCTSTR strFunction, KModeFunction& function);

    /**
    * @brief Field Caption Return
    * @param[in] strName - Column Name
    * @param[out] CString - Field Caption
    */
	CString getFieldName(LPCTSTR strCaption);

	/**
    * @brief Default 수식 DB Load
    */
	void LoadDefaultFunction(std::map<int, std::map<int, CString>> &a_mapDefaultFunction);

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult);
	afx_msg LRESULT OnFunctionEdit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonDefault();
};
