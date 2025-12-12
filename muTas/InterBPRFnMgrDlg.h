#pragma once

struct TBPRInfo
{
	__int64 nxNum;
	double	alpha;
	double	beta;
	double	a;
	double	b;
};

class KTarget;
// KInterBPRFnMgrDlg 대화 상자입니다.

class KInterBPRFnMgrDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KInterBPRFnMgrDlg)

public:
	KInterBPRFnMgrDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterBPRFnMgrDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6877_InterBPRFnMgrDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int     FnGroupID(void);

private:
	void	InitReportCtrl(void);
	void	UpdateReportCtrl(void);
	void	UpdateControlState(void);
	void	ReadBPRTextFile(void);
	void	AddBPRFn(void);
	void	EditBPRFn(void);
	void	InitComboBoxFn(void);
	bool	GetLinkFunctionID(void);
	void	UpdateInitReportCtrl(void);

private:
	int     m_nFnGroupID;
	CString m_strImportFileName;
	CString m_strFnName;

	std::vector<std::vector<CString>>	m_vecRows;
	std::vector<CString>				m_vecLinkFunction;

	KTarget* m_pTarget;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnRadioClicked(UINT nID);

private:
	CComboBox 		  m_comboFn;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
};
