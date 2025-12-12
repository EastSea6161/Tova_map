#pragma once

#include "afxwin.h"
#include "resource.h"
#include "PictureEx.h"


// KInterProgressDlg 대화 상자입니다.

class KInterProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KInterProgressDlg)

public:
	KInterProgressDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterProgressDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6999_Inter_Progress_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetThreadFunction(void (*func)(void* pParam));

	void SetThreadParam(void* pParam);

	void SetStatus(LPCTSTR strMsg);

	bool IsCancled(void);

	void Stop(void);


protected :
	CPictureEx m_stcAnimate;
	CStatic m_stcStatus;
	CListBox m_lstStatus;
	CButton m_btnClose;
	bool m_bCancel;
	CString m_strCaption;
	CRITICAL_SECTION m_cs;
	void (*ThreadFunction)(void* pParam);
	void* m_pParam;
	std::queue<CString> m_StatusQueue;


public :
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
