#pragma once

class KTarget;
// KABOMulitiFnManagerDlg 대화 상자입니다.

class KABOMultiFnManagerDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KABOMultiFnManagerDlg)

public:
	KABOMultiFnManagerDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KABOMultiFnManagerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6723_ABOMulitiFnManagerDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetColumnInfo(std::vector<CString> vecColumnInfo);

	void GetImportData(std::vector<std::vector<CString>>& vecRows);

	void SetFnData(std::vector<std::vector<CString>>& vecRows);

protected:

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

	std::vector<CString> m_vecColumnInfo;

	std::vector<CString> m_vecLinkFunction;

	std::vector<std::vector<CString>> m_vecRows;

	CString m_strImportFileName;

	KTarget*     m_pTarget;

protected:

	virtual BOOL OnInitDialog();
	
	void InitReportCtrl();

	void UpdateReportImportCtrl();

	void ReadBPRTextFile(void);

	void UPdateInitReportCtrl();

	bool GetLinkFunctionID();

protected:
	afx_msg void OnBnClickedBtnImport();
};
