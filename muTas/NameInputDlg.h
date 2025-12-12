#pragma once

//^#include "ResizeDialogEx.h"
// KNameInputDlg 대화 상자입니다.

class KNameInputDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KNameInputDlg)

public:
	KNameInputDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KNameInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_9999_NAMEINPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	inline void SetCaption(CString a_strCaption)
	{
		m_strCaption = a_strCaption;
	};

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	CString m_strCaption;
	CString m_strName;
public:
	CString GetName()
	{
		return m_strName;
	}
};
