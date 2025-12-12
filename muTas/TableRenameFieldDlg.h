#pragma once
//^#include "ResizeDialogEx.h"

class KExistNameChecker;
// KTableRenameFieldDlg 대화 상자입니다.

class KTableRenameFieldDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTableRenameFieldDlg)

public:
	KTableRenameFieldDlg(CString &a_strColumnName, std::vector<CString> a_vecDisplayName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableRenameFieldDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4155_RenameFieldDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	std::vector<CString>   m_vecDisplayName;
	CString				  &m_strColumnName;
	KExistNameChecker*	   m_pNameChecker;

protected:
	virtual BOOL OnInitDialog();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
protected:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();	
};
