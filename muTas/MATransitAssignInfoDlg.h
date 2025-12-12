
#pragma once

#include "DBaseAssignment.h"
//^#include "ResizeDialogEx.h"

class KTarget;
class KMultiModalInfo;

// KMATransitAssignInfoDlg 대화 상자입니다.

class KMATransitAssignInfoDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KMATransitAssignInfoDlg)

public:
	KMATransitAssignInfoDlg(KTarget* a_pTarget,std::list<KMultiModalInfo>a_lstMultiInfo, std::vector<TMAFnCoefficient> a_vecCofficient, std::vector<TMATransitCostInfo> a_vecCost, TMATransitOptionsBaseInfo& a_oBaseInfo, int a_nMaxRoute, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMATransitAssignInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6935_MATransitAssignInfoDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void GetAssignInfoValue(std::vector<TMAFnCoefficient>& a_TCofficient, std::vector<TMATransitCostInfo>& a_CostInfo, TMATransitOptionsBaseInfo& a_oOptionBaseInfo);

protected:

	bool m_bBus;
	bool m_bSubway;
	bool m_bBusSubway;
	bool m_bExclusive;

	int m_nMaxNo;

	KTarget* m_pTarget;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;

	std::list<KMultiModalInfo> m_lstMultiInfo;

	std::vector<TMAFnCoefficient> m_vecFnCofficient;
	std::vector<TMATransitCostInfo> m_vecTransitCost;

	TMATransitOptionsBaseInfo m_oOptionBaseInfo;

	std::map<int, CString> m_mapTransitName;

protected:

	virtual BOOL OnInitDialog();

	void InitEditButtion(void);

	void InitReportCoefficientHeader(void);

	void InitReportCostHeader(void);

	void UpdateReportCoefficient(void);

	void UpdateReportCost(void);

	void ResizeComponent();

	void GetCoefficientValue(void);

	void GetTransitCostValue(void);

	bool GetOptioBaseInfo();

	//★ 
	void TransitInfoSetting();

	void WatingVDFSetting();


private:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonDefault();
};
