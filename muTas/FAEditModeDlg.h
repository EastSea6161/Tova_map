#pragma once

#include "DBaseAssignImport.h"
class KTarget;

// KFAEditModeDlg 대화 상자입니다.

class KFAEditModeDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KFAEditModeDlg)

public:
	KFAEditModeDlg(KTarget* a_pTarget, CWnd* pParent = NULL);
	virtual ~KFAEditModeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4153_EDITFAMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

	std::map<int, KFAModeInfo> m_mapFAModeInfo;
	int m_nMaxCode;

protected:
	void InitReportHeader();
	void UpdaetReportData();

protected:
	void GetMaxCode(int& a_nMaxCode);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
