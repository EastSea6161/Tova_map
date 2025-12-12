#pragma once


// KDevPlanODlg 대화 상자입니다.

class KDevPlanODlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevPlanODlg)

public:
	KDevPlanODlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanODlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5605_DevPlanODlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitPAReport(void);
	void UpdatePAReport(void);

	void InitPurposeReport(void);
	void UpdatePurposeReport(void);

	void InitModeReport(void);
	void UpdateModeReport(void);

public:
	virtual BOOL OnInitDialog();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndPAReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndPurposeReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndModeReport;
};
