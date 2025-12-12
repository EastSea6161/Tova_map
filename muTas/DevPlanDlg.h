#pragma once

enum KEMDevPlanMigrationType
{
	KEMDevPlanMigrationData 				= 0,
	KEMDevPlanMigrationDirectInput 			= 1,
	KEMDevPlanMigrationProportionalDistance = 2
};

//forward delcare
class KTarget;
// KDevPlanDlg 대화 상자입니다.

class KDevPlanDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevPlanDlg)

public:
	KDevPlanDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5601_DevPlanDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	std::set<int> PlanList(void);

private:
	void  InitPlanReport(void);
	void  UpdatePlanReport(void);

	void  InitDirectInputReport(void);
	void  UpdateDirectInputReport(void);

private:
	std::set<int> m_setPlanID;
	KTarget*      m_pTarget;
	bool          m_bIsImport;
	
	KEMDevPlanMigrationType m_emDevPlanMigrationType;

public:
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnBnClickedImportMigration();
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndPlanReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndDirectInputReport;
};
