#pragma once
#include "afxwin.h"
//^#include "DBaseConnector.h"

// KABPRFnManagerDlg 대화 상자입니다.

struct TBPRInfo
{
	__int64 nxNum;
	double	alpha;
	double	beta;
	double	a;
	double	b;
};

class KTarget;

class KABPRFnManagerDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KABPRFnManagerDlg)

public:
	KABPRFnManagerDlg(KTarget* a_pTarget, int a_nParentFnGroupID, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KABPRFnManagerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6725_ABPRFnManagerDlg };

public:
	bool IsExistFunctionList(void);

	bool GetLinkFunctionID();
	void UpdateControlState(void);
	void InitReportCtrl(void);
	void UpdateInitReportCtrl(void);
	void UpdateInsertReportCtrl(void);
	void ReadBPRTextFile(void);
	void AddBPRFn(KDBaseConPtr a_spDBaseConnection);
	void EditBPRFn(void);
	void InitComboBoxFn(void);
	int	 FnGroupID(void);

	void InitCboReportCtrl(void);
	void InitCboReportData(void);
	void UpdateEachLinkReport(void);

	void GetComboValue(std::vector<CString>& a_vecColumn);	

public:
	int m_nFnGroupID;
	int m_nParentFnGroupID;

private:
	CString m_strImportFileName;
	std::vector<std::vector<CString>> m_vecRows;

	std::vector<CString> m_vecLinkFunction;

	CString m_strFnName;

	KTarget* m_pTarget;

	std::vector<CString> m_vecFnName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioClicked(UINT nID);
	
private:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;
	CXTPOfficeBorder<CXTPReportControl, false> m_wndCboReport;

public:
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedOk();
	afx_msg void OnReportCheckItem(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	
	CComboBox m_comboFn;
	afx_msg void OnSelchangeCombo();
};
