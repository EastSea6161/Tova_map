#pragma once


// CInterBaseDlg 대화 상자입니다.

class CInterBaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInterBaseDlg)

public:
	CInterBaseDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInterBaseDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_9600_KInterModal_BaseDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
