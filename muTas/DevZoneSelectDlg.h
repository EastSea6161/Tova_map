#pragma once

class KTarget;
// KDevZoneSelectDlg 대화 상자입니다.

class KDevZoneSelectDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevZoneSelectDlg)

public:
	KDevZoneSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevZoneSelectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5698_DEVZONESELECTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
	void         InitReportHeader();
	void         UpdateReportData();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

private:
	std::set<Integer>   m_setInitialSelectedZone;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndZoneReportCtrl;
private:
	KTarget* m_pTarget;
public:
	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}
public:
	void SetInitialSelectedZone(std::vector<Integer> a_vecInitialSelectedZone);
	std::vector<Integer> GetSelectedZone();

};
