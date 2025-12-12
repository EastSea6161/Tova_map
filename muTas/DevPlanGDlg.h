#pragma once

enum KEMDevPlanGenerationType
{
	KEMDevPlanAutomaticCalc = 0,
	KEMDevPlanDirectInput   = 1
};

//forward declare
class KTarget;
class KPAPurpose;
class KDevPlanGInfo;

// KDevPlanGDlg 대화 상자입니다.

class KDevPlanGDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevPlanGDlg)

public:
	KDevPlanGDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanGDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5602_DevPlanGDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void ClearPurposeInfos(void);

	void InitPurposeInfos(void);

	void InitAutoReport(void);

	void UpdateAutoReport(void);

	void UpdatePACombo(CXTPReportRecordItemEditOptions* a_pEditOptions, KPAPurpose* a_pPAPurpose);

public:
	void PlanID(int);

private:
	int	     m_nPlanID;
	KTarget* m_pTarget;
	
	std::list<KDevPlanGInfo*> m_lstPurpose;
	KEMDevPlanGenerationType  m_emType;

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndAutoReport;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult);
	
	afx_msg LRESULT OnExpressionEdit1(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg LRESULT OnExpressionEdit2(WPARAM a_wParam, LPARAM a_lParam);
};
