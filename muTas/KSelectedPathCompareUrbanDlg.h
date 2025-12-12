#pragma once

#include "KSelectedPathCompareTypes.h"



// KSelectedPathCompareUrbanDlg 대화 상자입니다.

class KSelectedPathCompareUrbanDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KSelectedPathCompareUrbanDlg)

public:
	KSelectedPathCompareUrbanDlg(bool bPathEdit, EMPathCompareMode emMode, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSelectedPathCompareUrbanDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_6929_KSelectedPathCompareUrban };
#endif


public:
	void SetTransitEditInfos(std::vector<KMTransitEditInfoWrapper>& arr);
	void SetMultimodalResultInfos(std::vector<KMBusAssignInfoWrapper>& arr);
	void SetInterPathEditInfos(std::vector<KInterPathEditInfoWrapper>& arr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	bool m_bPathEdit;	// 경로편집창에서 실행여부, true=경로편집 창에서 실행됨, false=경로분석 창에서 실행됨
	EMPathCompareMode m_emCompareMode;

	void InitReportHeader();
	void UpdateReportData();
	void UpdateReportMultimodalPathEditInfo();
	void UpdateReportMultimodalResultInfo();
	void UpdateReportIntermodalUrbanPathInfo();

	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;
	std::vector<KMTransitEditInfoWrapper> m_arrTransitEditInfo;
	std::vector<KMBusAssignInfoWrapper> m_arrMultimodalResultInfo;
	std::vector<KInterPathEditInfoWrapper> m_arrInterPathEditInfo;

	double GetTotalTime(KInterPathEditInfo* pInfo);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

};


typedef std::shared_ptr<KSelectedPathCompareUrbanDlg> KSelectedPathCompareUrbanDlgPtr;