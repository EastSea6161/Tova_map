#pragma once
//^#include "ResizeDialogEx.h"
#include "TDrawDesireMap.h"
#include "afxwin.h"

// KViewChartDlg 대화 상자입니다.

class KViewChartDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KViewChartDlg)

public:
	KViewChartDlg(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KViewChartDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3520_ViewChartDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	CXTPChartContent*         CreateRadarChart();
	CXTPChartContent*         CreateBarChart();
	CXTPChartContent*         CreateAreaChart();

	CString      CreateTitle();
	void	     ResizeComponent();

protected:
	std::vector<TDesireMapData>      &m_vecDesireMapData;
	TDesireODInfo                    &m_oTDesireODInfo;
	std::map<int, CXTPChartContent*>  m_mapPChartContent;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioChart(UINT nID);
	afx_msg void OnBnClickedCancel();

protected:
	CXTPChartControl m_wndChartControl;
};
