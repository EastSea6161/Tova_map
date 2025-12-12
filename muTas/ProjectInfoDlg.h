#pragma once

class KTarget;
// KProjectInfoDlg 대화 상자입니다.

class KProjectInfoDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KProjectInfoDlg)

public:
	KProjectInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KProjectInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_1050_ProjectInfoDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void SetResizeCtrl(void);
	void InitReportHeader(void);
	void InitReportData(void);
	
	void GetGenerationRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetDistributionRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetAssignRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetMultiModalRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetInterModalRunTime(KTarget* a_pTarget, CString& a_strDate);

	void AddChildRecord(CXTPReportRecord* a_pParent, KTarget* a_pTarget);

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
};
