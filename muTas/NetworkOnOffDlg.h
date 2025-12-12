#pragma once

class KTarget;

struct TNodeGroupOnOff
{
	int     nGroupType;
	CString strGroupName;
	BOOL    bOnOff;
	int     nUsedCount;
};

struct TLinkGroupOnOff
{
	int     nGroupType;
	CString strGroupName;
	BOOL    bOnOff;
	int     nUsedCount;
};

// KNetworkOnOffDlg 대화 상자입니다.

class KNetworkOnOffDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KNetworkOnOffDlg)

public:
	KNetworkOnOffDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KNetworkOnOffDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3910_NetworkOnOffDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	void SetTarget(KTarget *a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

protected:

	CString DisplayScenarioTitle( CString a_strTitle );

	void    InitReportHeader();

	void    InitNodeReportHeader();

	void    IntiLinkReportHeader();

	void    SetResizeReportControl();
	
	void    InitReportData();
	
	void    InitNodeReportData();
	
	void    InitLinkReportData();

	void    GetNodeTypeInfo(std::map<int, TNodeGroupOnOff> &a_mapNodeGroupOnOff);

	void    GetLinkTypeInfo(std::map<int, TLinkGroupOnOff> &a_mapLinkGroupOnOff);

	void    GetNodeOffInfo(std::set<int> &a_setNodeOffType);

	void    GetLinkOffInfo(std::set<int> &a_setLinkOffType);

	BOOL    IsNetworkOnOff(std::set<int> &a_setNetworkOnOff, int &a_nGroupType);

	void    SaveNetworkOff(CString &a_strNodeOffTypes, CString &a_strLinkOffTypes);

	void    UpdateNodeOffTypes(CString &a_strNodeOffTypes);

	void    UpdateLinkOffTypes(CString &a_strLinkOffTypes);
	
protected:
	KTarget      *m_pTarget;

	std::map<int, TNodeGroupOnOff> m_mapNodeGroupOnOff;
	std::map<int, TLinkGroupOnOff> m_mapLinkGroupOnOff;

protected:
	virtual BOOL OnInitDialog();

	afx_msg void OnNodeReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLinkReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNodeSelectCancel();
	afx_msg void OnNodeSelectAll();
	afx_msg void OnLinkSelectCancel();
	afx_msg void OnLinkSelectAll();
	afx_msg void OnBnClickedOk();

	void CheckboxControl(CXTPReportControl *pReport, int a_nCheckFieldIdx, BOOL a_bCheck);

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportNode;
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportLink;
};
