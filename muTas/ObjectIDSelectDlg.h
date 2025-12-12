#pragma once
//^#include "ResizeDialogEx.h"
// KLinkSelectDlg 대화 상자입니다.
class KObjectIDSelectDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KObjectIDSelectDlg)
public:
	KObjectIDSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KObjectIDSelectDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_3011_ObjectSelectDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void InitReportHeader();
	void InitReportData();

private:
	std::vector<Integer> m_vecID;
	void ClearCheck();
private:
	Integer m_nxSelectedID;
public:
	Integer GetSelectedID() {
		return m_nxSelectedID;
	}

private:
	CPoint m_windowPosition;
public:
	void SetWindowPosition(CPoint a_point)
	{
		m_windowPosition = a_point;
	}

public:
	void SetLink(std::vector<Integer>& ar_vecLinkID);
protected:	
	afx_msg void OnReportItemClick(NMHDR* pNotify, LRESULT* pResult);
	afx_msg void OnBnClickedOk();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_reportCtrl;
};
