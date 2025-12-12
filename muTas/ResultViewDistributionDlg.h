#pragma once


/*forward declare*/


#include "ResultViewTreeCtrl.h"

class KTarget;
class KProject;
class KScenario;
class KViewResultInfo;
class KODResultViewVRecord;

// KResultViewDistributionDlg 대화 상자입니다.

class KResultViewDistributionDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KResultViewDistributionDlg)

public:
	KResultViewDistributionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KResultViewDistributionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5511_ResultViewDistributionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	void InitReportControl(void);
	void UpdateReportControl(void);
	
protected:
	KResultViewTreeCtrl m_treeState;
	CXTPTreeCtrl* m_pTreeFocus;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	std::vector<KViewResultInfo> m_vecKViewResultInfo;

private:
	KODResultViewVRecord* m_pVirtualRecord;
	HICON m_hICon;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	

public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};
