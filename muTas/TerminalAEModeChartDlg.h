#pragma once

#include "DBaseTerminalAEModeViewer.h"

// KTerminalAEModeChartDlg 대화 상자입니다.

struct TTerminalChartDataInfo 
{
	CString strModeName;
	double dVolume;
	double dRatio;
};

struct TTerminalBarChartData 
{
	CString strModeName;
	double  dData;
};


struct TMainTerminal
{
	CString strMainName;
	std::vector<TTerminalBarChartData> vecTerminalChartData;
};

class KTerminalAEModeChartDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTerminalAEModeChartDlg)

public:
	KTerminalAEModeChartDlg(TTerminalResultInfo &a_oTTerminalResultInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTerminalAEModeChartDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5098_TerminalAEModeChartDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	TTerminalResultInfo &m_oTTerminalResultInfo;
	std::map<int, TTerminalChartDataInfo> m_mapModeDataAcc;
	std::map<int, TTerminalChartDataInfo> m_mapModeDataEgr;

	std::map<int, CXTPChartContent*>  m_mapChartContent;

private:
	void         ConvertChartData(TTerminalResultInfo &a_oTTerminalResultInfo);
	void         InitReportHeaderAcc();
	void         InitReportHeaderEgr();
	void         UpdateReportDataAcc();
	void         UpdateReportDataEgr();
	void         ResizeComponent();

private:         
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportAcc;
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReportEgr;
	CWnd*                      m_pParentWnd;
	CXTPChartControl           m_ctrlChart;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioDataType(UINT nID);

		void     DrawChart();
		void     CreateChart(std::vector<TMainTerminal> &a_vecBarChartData, CString a_strUnit = _T(""));
		CXTPChartContent* CreateBarChart(std::vector<TMainTerminal> &a_vecBarChartData, BOOL a_bRotate, BOOL a_bColorEach, BOOL a_bScaleNumeric);
		CXTPChartContent* CreatePieChart(std::vector<TMainTerminal> &a_vecBarChartData, CString a_strUnit = _T(""));
		CXTPChartSeriesPoint* CreatPiePoint(LPCTSTR lpszLegendText, double nYear, double nValue, double dWidth, BOOL bPie);
	afx_msg void OnBnClickedRadioChart(UINT nID);

	afx_msg void OnBnClickedCheckLabel();
		void     UpdateLabels();

	afx_msg void OnBnClickedCancel();	
	
};
