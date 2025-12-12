#pragma once

class KTarget;

// KMARunOptionDlg 대화 상자입니다.

class KMARunOptionDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KMARunOptionDlg)

public:
	KMARunOptionDlg(KTarget* a_pTarget,int& a_nChkTurn, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMARunOptionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6934_MARunOptionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


protected:
	KTarget* m_pTarget;

	CButton m_chkTurn;
	CStatic m_stcTurn;

	int& m_nChkTurn;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
