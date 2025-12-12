#pragma once

class KTarget;
// KZoneSelectDlg 대화 상자입니다.

class KZoneSelectDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KZoneSelectDlg)

public:
	KZoneSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KZoneSelectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4410_ZONESELECTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
	void    InitReportHeader();
	void    UpdateReportData();

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
