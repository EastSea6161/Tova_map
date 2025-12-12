#pragma once

class KTarget;
// KFProjectInfoDlg 대화 상자입니다.

class KFProjectInfoDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KFProjectInfoDlg)

public:
	KFProjectInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KFProjectInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_1050_ProjectInfoDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void SetResizeCtrl(void);
	void InitReportHeader(void);
	void InitReportData(void);

	void AddChildRecord(CXTPReportRecord* a_pParent, KTarget* a_pTarget);

	void GetGenerationRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetDistributionRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetItemLogitRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetChainRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetVehicleConvertRunTime(KTarget* a_pTarget, CString& a_strDate);
	void GetAssignmentRunTime(KTarget* a_pTarget, CString& a_strDate);

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
};
