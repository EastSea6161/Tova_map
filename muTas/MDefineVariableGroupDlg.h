#pragma once

#include "afxwin.h"
#include "ModeChoiceInfo.h"
#include "MDefVarSubPage.h"
#include "ModeChoiceModelInfo.h"

#define TAB_LEFT_MARGIN    5/*5*/
#define TAB_TOP_MARGIN    25
#define TAB_WIDTH_MARGIN  TAB_LEFT_MARGIN*2
#define TAB_HEIGHT_MARGIN 30

class KMode;
class KCodeGroup;
class KIOColumn;
class KMBaseVariable;
class KMSimpleVariable;
class KModeFunction;
class KTarget;
class KExpressionChecker;

// KMDefineVariableGroupDlg 대화 상자입니다.

class KMDefineVariableGroupDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KMDefineVariableGroupDlg)

public:
	KMDefineVariableGroupDlg(std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMDefineVariableGroupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6521_MDefineVariableGroupDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year 정보
	*/
	void Target(KTarget* pTarget);

	/**
	* @brief 수단선택 정보를 추가한다.
	* @param[in] pMCInfo - 수단선택 정보
	*/
	void AddModeChoiceInfo(KModeChoiceInfo* pMCInfo);

	void UsedFunctionIDSet(std::set<int> &a_setUsedFunctionID);

protected:
	void GetResultInfo (std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup);

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
	KTarget*     m_pTarget;

	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator> m_VariableMap;

	KExpressionChecker* m_pExpressionChecker;

	std::map<int, KMSimpleVariable*> m_mapTempSaveData;

	std::map<int, CString> m_mapFunctionInfo;  

private:
	CTabCtrl     m_ctrlTab;
	std::map<int, KMDefVarSubPagePtr> m_mapSubPage;
	KMDefVarSubPagePtr  m_spCurShowDefVarSubPage;
	std::map<int, TDefVarDefaultODGroup> &m_mapDefaultODGroup;

	CComboBox m_cboFunction;

	void         DeleteTempSaveData();
	void         LoadXmlDataModeChoiceVarDefault();
	KMode*       FindMode(int a_nModeID);
	void         GenerateModeFunction(LPCTSTR strFunction, KModeFunction& function);
	CString      GetFieldName(LPCTSTR strCaption);

	void         InitExpressionChecker(void);

	void         LoadInitialFunction(int a_nPreVariableID=-1);

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
	void UpdateReportRecord(void);

	void         DisplayTabControl(int a_nTabIndex);
	void         CreateChildPage();
	void         SubPageRedrawWindow();

	bool         InvalidateInputInfo();

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelchangeTabObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnCbnSelchangeComboFunc();
	

};
