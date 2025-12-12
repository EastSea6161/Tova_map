#pragma once

// KThemeSetFilterDlg 대화 상자입니다.

using namespace std;

struct TFilterInfo
{
	BOOL bChecked;
	int  nCodeUser;
	int  nCodeSystem;
};

class KTarget;

class KThemeSetFilterDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KThemeSetFilterDlg)

public:
	KThemeSetFilterDlg( KTarget* a_pTarget,
		vector<TFilterInfo*> a_vecFilterInfo, 
		int a_nUserGroupKey, int a_nSysGroupKey, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeSetFilterDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_SetFilterDlg };

private:
	void InitRptCtrl(void);
	void UpdateRptCtrl(void);

	void UpdateChkAllStatus(void);

private:
	vector<TFilterInfo*> m_vecFilterInfo;

	KTarget* m_pTarget;

	int m_nUserGroupKey;
	int m_nSysGroupKey;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedChkAll();
	
private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndRptCtrl;	
};
