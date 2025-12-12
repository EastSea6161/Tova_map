#pragma once
#include "afxcmn.h"
#include "TabSubSample.h"

// KTabMainSample 대화 상자입니다.
#define TAB_LEFT_MARGIN    5/*5*/
#define TAB_TOP_MARGIN    25
#define TAB_WIDTH_MARGIN  TAB_LEFT_MARGIN*2
#define TAB_HEIGHT_MARGIN 30

class KTabMainSample : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTabMainSample)

public:
	KTabMainSample(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTabMainSample();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TAB_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
    CTabCtrl m_tabCtrl;
protected:
    afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
    void DisplayTabControl(int a_nTabIndex);

private:
    int  m_nTabAbovePixcel;
private:
    KDialogEx*    m_pCurrentShowClientDlg;
    KTabSubSample m_dlgTabSubSample;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();

private:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
    void InitReportHeader();
    void InitReportData();
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
};
