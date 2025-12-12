#pragma once
#include "afxwin.h"

class KMode;
class KModeTree;
class KModeFunction;
class KTarget;
class KExpressionChecker;
class KMSimpleVariable;

// KMDefineVariableEdit 대화 상자입니다.

class KMDefineVariableEdit : public KDialogEx
{
	DECLARE_DYNAMIC(KMDefineVariableEdit)

public:
	KMDefineVariableEdit(int a_nVariableID, KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMDefineVariableEdit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6522_MDefineVariableEdit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	KTarget*          m_pTarget;
	int               m_nPreSelectedVariableID;

	KModeTree*        m_pModeTree;
	std::list<KMode*> m_RemainModeList;

	KExpressionChecker* m_pExpressionChecker;

	int          m_nFunctionID;
	std::map<int, KMSimpleVariable*> m_mapTempSaveData;

private:
	void         DeleteTempSaveData();
	void         LoadXmlData();
	KMode*       FindMode(int a_nModeID);
	void         SaveXmlData();

	void         InitExpressionChecker(void);
	void         InitReportHeader(void);
	void         InitModeList();
	void         UpdateComboMode();
	void         UpdateReportData();
	void         RemoveModeFromList(KMode* pMode);

	void         LoadInitialFunction();
	void         FunctionSelectChangeAction();

	void         GenerateModeFunction(LPCTSTR strFunction, KModeFunction& function);
	CString      GetFieldName(LPCTSTR strCaption);

	bool         InvalidateInput();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
	CComboBox    m_cboFunction;
	CComboBox    m_cboMode;

private:
	virtual BOOL OnInitDialog();
	
private:
	afx_msg void OnBnClickedRadioFunction(UINT nID);
	void         ControlRadioFunction();

	afx_msg void OnBnClickedBtnModeAdd();
	void         AddNestedLogit(void);
	afx_msg void OnBnClickedBtnModeDel();
	void         RemoveNestedLogit(void);
	afx_msg void OnBnClickedBtnRemoveall();
	
	afx_msg void OnReportSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	void         UpdateModeButtonState(void);
	afx_msg void OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult);
	afx_msg LRESULT OnFunctionEdit(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedBtnApply();
	bool         ApplySimpleVariable(CXTPReportRecord* a_pRecordRoot, KMSimpleVariable* a_pSaveSimpleVariable);
	afx_msg void OnBnClickedButtonFuncDelete();
	afx_msg void OnCbnSelchangeComboFunc();
};
