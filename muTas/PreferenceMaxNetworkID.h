#pragma once
//^#include "ResizeDialogEx.h"

// KPreferenceMaxNetworkID 대화 상자입니다.

class KPreferenceMaxNetworkID : public KDialogEx
{
	DECLARE_DYNAMIC(KPreferenceMaxNetworkID)

public:
	KPreferenceMaxNetworkID(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPreferenceMaxNetworkID();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_0100_NETWORK_EDIT_ID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL OnInitDialog();
protected:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
    double m_dNodeFontSize;
    double m_dLinkFontSize;

public:
    virtual void UserCommand(int a_nCommand=0);
};
