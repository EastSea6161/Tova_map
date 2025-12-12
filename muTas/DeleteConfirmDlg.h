#pragma once


// KDeleteConfirmDlg 대화 상자입니다.

class KDeleteConfirmDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDeleteConfirmDlg)

public:
	KDeleteConfirmDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDeleteConfirmDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_0900_KDeleteConfirmDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	bool IsDeleteFile(void);

private:
	bool m_bDelFile;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	HACCEL m_accelerator_key;

};
