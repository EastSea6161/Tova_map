#pragma once
#include "afxwin.h"

// KDialogExSplash 대화 상자입니다.

class KStartSplashDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KStartSplashDlg)

public:
	KStartSplashDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KStartSplashDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_0000_StartSplashDlg };

protected:
    CStatic m_systemPicture;   
    CBrush  m_pbkBrush;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
    virtual BOOL OnInitDialog();    
    afx_msg void OnBnClickedOk();
public:
	int m_nSystemTypeInfo;
    int GetSystemType() const;
	void SetSystemType(int a_nSystemType)
	{
		m_nSystemTypeInfo = a_nSystemType;
	}

    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //CXTPButton m_btnOptionPassenger;
    //CXTPButton m_btnOptionFreight;
    afx_msg void OnStnClickedSplash();
};
