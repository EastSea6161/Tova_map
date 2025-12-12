#pragma once
//^#include "ResizeDialogEx.h"

// KLinkDevideSelectDlg 대화 상자입니다.

class KLinkDevideSelectDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KLinkDevideSelectDlg)

public:
	KLinkDevideSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KLinkDevideSelectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3012_LinkDevideSelectDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	void InitReportHeader();
	void InitReportData();
private:
	std::vector<Integer> m_vecID;
	std::vector<Integer> m_vecSelectedID;
private:
	CPoint m_windowPosition;
public:
	void SetWindowPosition(CPoint a_point)
	{
		m_windowPosition = a_point;
	}

public:
	void SetID(std::vector<Integer>& ar_vecID);
	std::vector<Integer> GetSelectedID();
protected:	
	afx_msg void OnReportItemClick(NMHDR* pNotify, LRESULT* pResult);
	afx_msg void OnBnClickedOk();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
public:
    afx_msg void OnBnClickedCancel();
};
