#pragma once


// KAddModeNameDlg 대화 상자입니다.

class KAddModeNameDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAddModeNameDlg)

public:
	KAddModeNameDlg(std::set<CString> a_setModeName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAddModeNameDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8531_KAddModeNameDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString GetModeName(void);


private:
	std::set<CString> m_setModeName;
	CString           m_strModeName;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
