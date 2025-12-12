#pragma once

class KTarget;
class KPurpose;
class KMode;


// KDevPlanMDlg 대화 상자입니다.

class KDevPlanMDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevPlanMDlg)

public:
	KDevPlanMDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanMDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5604_DevPlanMDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetTarget(KTarget* pTarget);

	void GetValue();

private:
	void InitReportCtrl(void);
	void UpdateReportCtrl(void);
	void GetRate();

public:
	virtual BOOL OnInitDialog();

private: 
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

protected:
	KTarget* m_pTarget;
	std::vector<double> m_vecRate;
};
