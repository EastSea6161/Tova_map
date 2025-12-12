#pragma once

class KScenario;

// KCopyProject 대화 상자입니다.

class KCopyProject : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KCopyProject)

public:
	KCopyProject(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCopyProject();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_1500_CopyProject };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	
private:
	void ValidateInput(void);
	void InitReportHeader(void);
	void InitReportData(void);
	void AddTreeRecord(CXTPReportRecord* a_pParent, KScenario* a_pScenario);

	void RunCopyProject(void);

	void CopyDefaultParameter(CString a_strSource, CString a_strTarget);
	void CopyDefaultProjectDB(CString a_strSource, CString a_strTarget);
	void CreateProjectFile(void);
	void CreateScenarioFile(void);

	static unsigned __stdcall ThreadCaller(void* p);

private:
	static int CALLBACK BrowseCallBackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lPData );

private:
	CString m_strPrjName;
	CString m_strPrjPath;
	CString m_strFullPath;
	
	bool    m_bThreadOK;
	CString m_strErrorMsg;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPrjbtnLocation();
	afx_msg void OnBnClickedCheck1();

	afx_msg void OnReportCheckItem(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
};
