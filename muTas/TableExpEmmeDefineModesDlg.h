#pragma once

//^#include "ResizeDialogEx.h"

class KTarget;

using namespace std;

// KTableExpEmmeDefineModesDlg 대화 상자입니다.

class KTableExpEmmeDefineModesDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableExpEmmeDefineModesDlg)

public:
	KTableExpEmmeDefineModesDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableExpEmmeDefineModesDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4172_TableExpEmmeDefineModesDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void GetDefineModes(map<int, CString>& ar_mapDefineMode);

private:
	void GetUsedLinkType(void);
	void GetDefineSystemCode(void);

	void InitReportHeader(void);
	void UpdateReportData(void);

private:
	bool     m_bDefined;
	KTarget* m_pTarget;
	CString  m_strPreValue;
	
	map<int, CString> m_mapDefineMode;
	map<int, int>     m_mapLinkMatchingCode;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemValueChanged(NMHDR* pNotifyStruct, LRESULT* pResult);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
};

