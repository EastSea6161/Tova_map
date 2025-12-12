#pragma once

#include "resource.h"
#include "afxwin.h"


/*forward declare*/
class KTarget;
class KPurpose;
class KInterInputInfo;

// KInterInputDlg 대화 상자입니다.
class KInterInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KInterInputDlg)

public:
	KInterInputDlg(KTarget* a_pTarget, std::list<KInterInputInfo*>& a_lstInputInfo, 
		CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6900_InterPurposeInputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitPurposeReport(void);
	void UpdatePurposeReport(void);

	void InitInputReport(void);
	void UpdateInputReport(void);
	
	void UpdateInputRecord(CXTPReportRecord* a_pParent, KPurpose* a_pPurpose);

	void ApplyPurposeInfos(void);
	void ApplyInputInfos(void);

private:
	KTarget*                    m_pTarget;
	std::list<KInterInputInfo*> m_lstInputInfo;
	
private:
	CXTPReportControl m_wndPurposeReport;
	CXTPReportControl m_wndInputReport;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	afx_msg void OnReportCheckItem(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnInputCheckItem(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
};
