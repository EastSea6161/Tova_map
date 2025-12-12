#pragma once
#include "afxdialogex.h"


// KFODDiv 대화 상자

class KFODDiv : public CDialogEx
{
	DECLARE_DYNAMIC(KFODDiv)

public:
	KFODDiv(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~KFODDiv();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_7002_UTIL_FODDiv };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_cboAreaNumber;
	CComboBox m_cboDistributionYear;
	CListCtrl m_listODFilePath;
protected:
	bool CreateArgument();
	void LoadDistributionList();
	void UpdateListBoxHorizontalScroll();

public:
	afx_msg void OnBnClickedBtnAddod();
	afx_msg void OnBnClickedBtnDelod();
	afx_msg void OnBnClickedRun();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDelete();
};
