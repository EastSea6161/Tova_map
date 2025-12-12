#pragma once


// KNaviItemAddDlg 대화 상자입니다.

class KNaviItemAddDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KNaviItemAddDlg)

public:
	KNaviItemAddDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KNaviItemAddDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_2190_AddNaviDlg };

public:
	CString NaviName(void);

private:
	CString m_strNaviName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
