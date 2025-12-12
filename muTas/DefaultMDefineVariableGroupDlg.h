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

// KDefaultMDefineVariableGroupDlg 대화 상자입니다.

class KDefaultMDefineVariableGroupDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDefaultMDefineVariableGroupDlg)

public:
	KDefaultMDefineVariableGroupDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultMDefineVariableGroupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8516_DefaultMDefineVariableGroupDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*     m_pTarget;

	std::list<KPurpose*>                 m_lstPurpose;
	std::vector<KModeChoiceInfo*>        m_MInfoVector;
	std::map<KModeChoiceInfo*, KMBaseVariable*, TModeChoiceInfoComparator> m_VariableMap;

	KExpressionChecker*                  m_pExpressionChecker;
	std::map<int, KMSimpleVariable*>     m_mapTempSaveData;
	std::map<int, CString>               m_mapFunctionInfo;  

	std::map<int, TDefVarDefaultODGroup> m_mapDefaultODGroup;

protected:
	void         PrepareData();
	void         InitPurposeSet(void);
	void         LoadDefaultPurpoeGroupODInfo();
	void         LoadODGroupDefineVariableID(int a_nGroupKey, int a_nPurposeID, std::map<int, int> &a_mapODGroupSavedFunctionID);
	void         ClearPurposeSet(void);
	void         ClearMInfoList(void);
	
	void         GetResultInfo (std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup);
	bool         SaveDefaultPurpoeGroupODInfo(std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup);
	bool         SaveODGroupDefineVariableID(std::map<int, TDefVarDefaultODGroup> &a_mapDefaultODGroup);


private:
	void         DeleteTempSaveData();
	void         LoadXmlDataModeChoiceVarDefault();
	KMode*       FindMode(int a_nModeID);
	void         GenerateModeFunction(LPCTSTR strFunction, KModeFunction& function);
	CString      GetFieldName(LPCTSTR strCaption);

	void         InitExpressionChecker(void);

	void         LoadInitialFunction(int a_nPreVariableID=-1);

	void         updateVariableMap(void);
	void         initReportControl(void);
	void         UpdateReportRecord(void);

	void         DisplayTabControl(int a_nTabIndex);
	void         CreateChildPage();
	void         SubPageRedrawWindow();

	bool         InvalidateInputInfo();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
	CComboBox    m_cboFunction;
	CTabCtrl     m_ctrlTab;
	std::map<int, KMDefVarSubPagePtr> m_mapSubPage;
	KMDefVarSubPagePtr  m_spCurShowDefVarSubPage;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelchangeTabObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnCbnSelchangeComboFunc();
	afx_msg void OnBnClickedBtnInit();
};
