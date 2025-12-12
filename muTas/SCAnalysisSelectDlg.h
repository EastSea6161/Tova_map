#pragma once

// KSCAnalysisSelectDlg 대화 상자입니다.
/*forward declare*/
class KTarget;

struct TScreenCordonInfo
{
	int		TLineID;
	int		TDirectionType;
	CString	TLineName;
};

const CString HEADER_ID		= _T("ID");
const CString HEADER_SELECT = _T("Select");
const CString HEADER_NAME	= _T("Name");

class KSCAnalysisSelectDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSCAnalysisSelectDlg)

public:
	KSCAnalysisSelectDlg(KTarget* a_pTarget, int a_nRadioType, std::vector<int> a_vecSelected, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSCAnalysisSelectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5720_SCAnalysisSelectDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int					LineType();
	std::vector<int>	Selected();

protected:
	void UpdateControlState(void);

	void InitScreenReportControl(void);
	void UpdateScreenReportControl(void);

	void InitCordonReportControl(void);
	void UpdateCordonReportControl(void);

	void LoadScreenCordonData(void);

	void ApplySelectedInfo(void);

private:
	KTarget*			m_pTarget;

	int					m_nLineType;
	std::vector<int>	m_vecSelected;

	std::vector<TScreenCordonInfo> m_vecScreenList;
	std::vector<TScreenCordonInfo> m_vecCordonList;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioClicked(UINT nID);

public:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndScreenReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndCordonReport;
	afx_msg void OnBnClickedOk();
};
