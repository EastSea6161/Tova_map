#pragma once

class KTarget;
class KPurpose;

struct STPathAvailableCommdityInfo
{
	bool         bSelected;
	CString      strCommdityName;
	int			 nCommdityCode;            
};


// KAvailableCommodity 대화 상자입니다.

class KAvailableCommodity : public KDialogEx
{
	DECLARE_DYNAMIC(KAvailableCommodity)

public:
	KAvailableCommodity(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAvailableCommodity();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5051_AvailableCommodityDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()


protected:
	CXTPReportControl                      m_wndReport;
	KTarget*                               m_pTarget;
	std::list<STPathAvailableCommdityInfo> m_lstAvailiablePurpose;

public:

	void SetTarget(KTarget* a_pTarget);

	void AvailableCommdityList(std::list<STPathAvailableCommdityInfo>& a_AvailableCommodityInfo);

	void GetUseCommodity(std::list<STPathAvailableCommdityInfo>& a_AvailableCommodityInfo);

protected:

	virtual BOOL OnInitDialog();

	void initReportControl(void);

	void updatePurposeRecord();

public:
	afx_msg void OnBnClickedOk();
};
