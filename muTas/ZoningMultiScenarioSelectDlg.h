#pragma once
#include "afxwin.h"



class KTarget;
class KScenario;



enum KEMZoningOperation
{
	KEMZoningOperationDivision,
	KEMZoningOperationIntegration
};


// KZoningMultiScenarioSelectDlg 대화 상자입니다.

class KZoningMultiScenarioSelectDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KZoningMultiScenarioSelectDlg)

public:
	KZoningMultiScenarioSelectDlg(KEMZoningOperation emOperation, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KZoningMultiScenarioSelectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_4410_MultiScenarioSelect };
#endif

public:
	void SetTarget(KTarget* pTarget);
	void SetSelectedZoneNodeSet(std::set<__int64>& setSelectedNode);
	std::vector<KTarget*> GetSelectedTarget();

protected:
	KEMZoningOperation m_emOperation;
	KTarget* m_pActiveTarget;
	std::set<__int64> m_setSelectedNode;
	std::vector<KTarget*> m_arrSelectedTarget;

	CString m_strSelectedNode;

	void initTreeControlHeader();
	void initTreeControlRecord();
	void addTreeRecord(CXTPReportRecord* pRecord, KScenario* pScenario);
	bool checkSelectable(KTarget* pTarget);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtNodeList;
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReportCtrl;
	virtual BOOL OnInitDialog();
	CString m_strOperation;
	afx_msg void OnBnClickedOk();
};
