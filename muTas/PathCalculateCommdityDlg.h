#pragma once

class KTarget;
class KPurpose;

// KPathCalculateCommdityDlg 대화 상자입니다.

class KPathCalculateCommdityDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KPathCalculateCommdityDlg)

public:
	KPathCalculateCommdityDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPathCalculateCommdityDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4301_PathCalculateCommodityDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


protected:
	CXTPReportControl                      m_wndReport;
	KTarget*                               m_pTarget;
	std::map<int,bool>                     m_mapSelectCommdity;


public:
	void SetInfo(KTarget* a_pTarget, std::map<int, bool> a_mapSelectCommdity);

	virtual BOOL OnInitDialog();

	void initReportControl(void);

	void updatePurposeRecord();

	void SelectCommdityList(std::map<int, bool>& a_mapSelectCommdity, CString& a_strEditBox);

public:

	afx_msg void OnBnClickedOk();
};

