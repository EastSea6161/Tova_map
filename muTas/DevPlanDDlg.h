#pragma once

enum KEMDevplanDistributionModel
{
	KEMDevplanDistributionModelFratar  = 0,
	KEMDevplanDistributionModelGravity = 1
};

//forward declare
class KTarget;
class KScenario;
// KDevPlanDDlg 대화 상자입니다.

class KDevPlanDDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevPlanDDlg)

public:
	KDevPlanDDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanDDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5603_DevPlanDDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitTargetYearReport(void);
	void UpdateTargetYearReport(void);

	void AddTreeRecord(CXTPReportRecord* a_pParnet, KScenario* a_pScenario);

	void InitTargetODReport(void);
	void UpdateTargetODReport(void);

	void UpdateFieldConstraint(KTarget* a_pTarget);

	void InitBaseODReport(void);
	void UpdateBaseODReport(void);

	void UpdateBaseFieldConstraint();

private:
	KTarget* m_pTarget;

	KEMDevplanDistributionModel m_emDevplanDistributionMode;

public:
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndTargetYearReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndTargetODReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndBaseODReport;
};
