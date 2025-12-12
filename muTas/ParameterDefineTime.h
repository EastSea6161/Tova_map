#pragma once
#include "DBaseDefineTransitCost.h"
#include "DBaseDefaultParaPathGenerateConstraint.h"
#include "DBaseDefaultParaMultimodalPathGeneration.h"

class KTarget;
class KIOColumn;


// KParameterDefineTime 대화 상자입니다.

class KParameterDefineTime : public KDialogEx
{
	DECLARE_DYNAMIC(KParameterDefineTime)

public:
	KParameterDefineTime(KTarget* a_pTarget, int a_nTimeMethod, KIOColumn* a_pColumn, TAssignRule a_oAssignRule, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KParameterDefineTime();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4602_ParameterDefineTimeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	void GetDefineTimeData(int& a_nTimeMethod, KIOColumn*& a_pColumn, TAssignRule& a_oAssignInfo);

protected:
	
	void InitComboLinkField();
	void InitAssignRuleValue();
	bool InvalidateInputInfo();

	void InitSystemConstraintConditions();
	void DefaultConstraintConditions();
	void InitSystemPathInfo();
	void DefultPathInfo();

protected:

	KTarget*	 m_pTarget;
	int          m_nTimeMethod;
	KIOColumn*   m_pIOColumnUseLink;

	TAssignRule			m_oAssignInfo;
	CComboBox			m_cboUsingLinkField;

	std::map<int, KPathGenerateConstraint> mapDefaultPathGenerateConstraint;
	std::map<int, KMultimodalPathGenerationInfo> mapDefaultMultimodalPathInfo;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioTimeMethod(UINT nID);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDefault();
	
};
