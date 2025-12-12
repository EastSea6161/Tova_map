#pragma once

//^#include "ResizeDialogEx.h"
#include "PreferenceLabel.h"
#include "PreferenceSymbol.h"

// KPreferenceForm 대화 상자입니다.

class KPreferenceForm : public KDialogEx
{
	DECLARE_DYNAMIC(KPreferenceForm)

public:
	KPreferenceForm(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPreferenceForm();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_0100_Preference };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:	
    virtual BOOL   OnInitDialog();

private:
    CTabCtrl     m_tabCtrl;    
    void DisplayTabControl(int a_nTabIndex);
private:
    afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
private:
    KDialogEx*	      m_pCurrentShowClientDlg;
    KPreferenceLabel  m_dlgPreferenceLabel;
	KPreferenceSymbol m_dlgPreferenceSymbol;
public:
    afx_msg void OnBnClickedBtnApply();
};
