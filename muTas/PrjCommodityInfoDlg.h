#pragma once

class KPurpose;

// KPrjCommodityInfoDlg 대화 상자입니다.

class KPrjCommodityInfoDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KPrjCommodityInfoDlg)

public:
	KPrjCommodityInfoDlg(std::vector<KPurpose*> &vecPurposeInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPrjCommodityInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_1100_PrjCommodityInfoDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	std::vector<KPurpose*> m_vecPurposeInfo;

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_reportPurpose;

protected:
	void IniteReportHeader();
	void IniteReportData();
	
protected:
	void ResizeComponet();

protected:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedCancel();
};
