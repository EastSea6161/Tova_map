#pragma once
//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "TDrawDesireMap.h"

class KTarget;
// KPrepareChartODDlg 대화 상자입니다.

class KPrepareChartODDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KPrepareChartODDlg)

public:
	KPrepareChartODDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPrepareChartODDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3531_PrepareChartOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	void         InitialControl();
	void         LoadComboField();
	void         LoadOriginReportData();
	void         LoadDestReportData();
	bool         CheckOriginDestinationSelect(bool a_bMessage = true);
	void         ResizeComponent();
	bool         InvalidateInputInfo(bool a_bMessage = true);
	void         GetDesireData();
	CXTPChartContent* CreateRadarChart(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo);
	CXTPChartContent* CreateBarChart(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo);
	CXTPChartContent* CreateAreaChart(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo);
	CString      CreateTitle(TDesireODInfo &a_oTDesireODInfo);
	void         CreateChart(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo);

	void         UpdateChartData();
public:
	static unsigned __stdcall MakeChartDataThreadCaller(void* p);

protected:
	KTarget*     m_pTarget;

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlOrigin;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrlDest;
	CXTPChartControl           m_wndChartControl;
	std::vector<TDesireOrigin> m_vecDesireOrigin;
	std::vector<TDesireDest>   m_vecDesireDest;
	std::map<KODKey, std::map<KODKey, double>> m_mapDesireInfo;
	std::map<int, CXTPChartContent*>  m_mapPChartContent;
	
	std::vector<TDesireMapData> m_vecDesireMapData;

private:
	afx_msg void OnCbnSelchangeComboTable();
	afx_msg void OnCbnSelchangeComboOriginUnit();
	afx_msg void OnCbnSelchangeComboDesUnit();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckOrigin();
	afx_msg void OnBnClickedCheckDestination();
	afx_msg void OnBnClickedButtonChart();
	afx_msg void OnCbnSelchangeComboField();
	afx_msg void OnBnClickedRadioChart(UINT nID);
			void UpdateLabels();
			void UpdateAxisX();
	afx_msg void OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCheckShowLabels();
	afx_msg void OnCheckShowAxisX();

private:
	CComboBox    m_cboTable;
	CComboBox    m_cboField;
	CComboBox    m_cboOriginUnit;
	CComboBox    m_cboDestUnit;
};

typedef std::shared_ptr<KPrepareChartODDlg> KPrepareChartODDlgPtr;