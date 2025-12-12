#pragma once


// KSCChangeColumnName 대화 상자입니다.

class KSCChangeColumnName : public KDialogEx
{
	DECLARE_DYNAMIC(KSCChangeColumnName)

public:
	KSCChangeColumnName(CString a_strCurrentName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSCChangeColumnName();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5741_ChangeFieldName };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CString m_strCurrentName;
	CString m_strNewName;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
