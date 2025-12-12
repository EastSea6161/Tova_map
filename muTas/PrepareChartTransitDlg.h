#pragma once
//^#include "ResizeDialogEx.h"
#include "TChartTransit.h"
#include "afxwin.h"

class KTarget;
class KIOColumn;
class KIOTable;
// KPrepareChartTransitDlg 대화 상자입니다.

class KPrepareChartTransitDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KPrepareChartTransitDlg)

public:
	KPrepareChartTransitDlg(KTarget* a_pTarget, Integer a_nxTransitID = 0, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPrepareChartTransitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3532_PrepareChartTransitDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void LoadAssignModelInfo(std::vector<TMultiModalInfo> &a_vecMultiModalInfo);
	void InitComboAssignModelSelect(std::vector<TMultiModalInfo> &a_vecMultiModalInfo);
	void InitFieldBoardAlight();
	void InitReportHeader();
	void InitReportAHeader();
	void InitReportBHeader();
	void ResizeComponent();
	void LoadTransitData(std::vector<TChartTransitInfo> &a_vecTransitlInfo);
	
    void LoadTransitLineData(Integer a_nxTransitID, std::vector<TChartDetailLineInfo> &a_vecTransitLineInfo);
    void LoadTransitLineByStn(__int64 nxTransitID, std::vector<TCharStnInfo>& vecTransitStnInfo);

	void UpdateReportData();
	void GetFieldCaption(int a_nKeyIndex, CString &a_strCaption);
	void InitReportAData();
	void InitComboData(std::vector<TChartTransitInfo> &a_vecTransitlInfo);
	void UpdateReportBData(Integer a_nxTransitID);

    CXTPChartContent* CreateChart(std::map<int, TChartData>& mapChartData);

protected:
	KTarget*     m_pTarget;
	KIOTable*    m_pTableLine;
	Integer      m_nxDefaultTransitID;
	std::vector<TMultiModalInfo> m_vecModelInfo;
	std::map<int, KIOColumn*>    m_mapPassengerField; 

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckAll();
	void         UpdateCheckAll();
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCbnSelchangeComboLineNo();
	afx_msg void OnCheckShowLabels();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonChart();
	afx_msg void OnCbnSelchangeComboMultiModal();

private:
	CComboBox    m_cboAssignResult;
	CComboBox    m_cboLineNo;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportACtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportBCtrl;
	CXTPChartControl           m_wndChartControl;
};


typedef std::shared_ptr<KPrepareChartTransitDlg> KPrepareChartTransitDlgPtr;