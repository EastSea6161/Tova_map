#pragma once

class KTarget;


// KSeatCapaInfoDlg 대화 상자입니다.

class KSeatCapaInfoDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KSeatCapaInfoDlg)

public:
	KSeatCapaInfoDlg(KTarget* a_pTarget, int a_nType, std::vector<Integer> a_vecLineID, double a_dCapaRatio, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSeatCapaInfoDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_4002_SEATCAPAINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;

	std::vector<Integer> m_vecTransitLine;
	int	m_nSeatCapaType;
	double m_dCapaRatio;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

protected:
	void ResizeComponet();

protected:
	void IniteReportHeader();
	void UpdateReportData();

	void GetSeatValue(Integer a_nxLineID, double& a_dSetValue);
	void GetCapacityValue(Integer a_nxLineID, double& a_dCapaValue);
	void GetTransitName(Integer a_nxLineID, CString& a_strTransitName);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};

typedef std::shared_ptr<KSeatCapaInfoDlg> KSeatCapaInfoDlgPtr;