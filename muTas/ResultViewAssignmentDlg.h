
#pragma once

#include "ResultViewTreeCtrl.h"

class KProject;
class KTarget;
class KScenario;
class KViewResultInfo;

// KViewResultAssignmentDlg 대화 상자입니다.

class KResultViewAssignmentDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KResultViewAssignmentDlg)

public:
	KResultViewAssignmentDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KResultViewAssignmentDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5513_ViewResultAssignmentDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


public:
	KResultViewTreeCtrl m_treeState;
	CXTPTreeCtrl* m_pTreeFocus;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	std::vector<KViewResultInfo> m_vecKViewResultInfo;

protected:
	HICON m_hICon;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	void InitReportControl( void );
	void UpdateReportControl( void );
	void ClearResultInfo();
	//void GetProject(KProject* a_pProject);

	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
public:
	
};
