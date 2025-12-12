#pragma once
//^#include "ResizeDialogEx.h"
#include "TChartPA.h"
#include "model_common.h"

class KTarget;
class KIOColumns;
// KPrepareChartPADlg 대화 상자입니다.

class KPrepareChartPADlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KPrepareChartPADlg)

public:
	KPrepareChartPADlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPrepareChartPADlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3534_PrepareChartPADlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void         InitialControl();
	void         LoadComboField();
	void         LoadReportData();
	void         ResizeComponent();
	bool         CheckPASelect();
	bool         InvalidateInputInfo();
	void         GetTotalProductionData (int a_nUnitIndex, std::map<Integer, std::vector<double>> &a_mapProduct);
	void         GetTotalConsumptionData(int a_nUnitIndex, std::map<Integer, std::vector<double>> &a_mapConsumpt);
	void         UpdateChartData();
	void         MakeChartData();

	void              CreateChart    (std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayProduct, std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayConsumpt);
	CXTPChartContent* CreateAreaChart(std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayProduct, std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayConsumpt);
	CXTPChartContent* CreateBarChart (std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayProduct, std::vector<TChartZoneDisplay> &a_vecChartZoneDisplayConsumpt);
	void              ShowChart(int nID);
	void              UpdateLabels();
	void              UpdateAxisX();
public:
	static unsigned __stdcall MakeChartDataThreadCaller(void* p);

protected:
	KTarget*     m_pTarget;
	KIOColumns*        m_pSelectedColumnP;
	KIOColumns*        m_pSelectedColumnC;
	std::vector<TChartZoneDB> m_vecZone;

	std::map<int, std::map<Integer, std::vector<double>>> m_mapProductInfo;
	std::map<int, std::map<Integer, std::vector<double>>> m_mapConsumptInfo;
	std::map<int, CXTPChartContent*>  m_mapPChartContent;

	std::vector<TChartZoneDisplay> m_vecChartZoneDisplayProduct;
	std::vector<TChartZoneDisplay> m_vecChartZoneDisplayConsumpt;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboGroupUnit();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedButtonChart();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSelColumP();
	afx_msg void OnBnClickedButtonSelColumC();
	afx_msg void OnCheckShowLabels();
	afx_msg void OnCheckShowAxisX();
	afx_msg void OnBnClickedRadioChart(UINT nID);
	
private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CXTPChartControl           m_wndChartControl;
	CComboBox    m_cboGroupUnit;
};
typedef std::shared_ptr<KPrepareChartPADlg> KPrepareChartPADlgPtr;