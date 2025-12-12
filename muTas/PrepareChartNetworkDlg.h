#pragma once
//^#include "ResizeDialogEx.h"
#include "TDrawDesireMap.h"
#include "TPrepareChart.h"
//^^ #include "Observer.h"

class KTarget;
class KIOTable;
class KIOColumns;
class KIOColumn;
// KPrepareChartNetwork 대화 상자입니다.

class KPrepareChartNetworkDlg : public KResizeDialogEx, public KTableObserver
{
	DECLARE_DYNAMIC(KPrepareChartNetworkDlg)

public:
	KPrepareChartNetworkDlg(KTarget* a_pTarget, CString a_strTableName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPrepareChartNetworkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3530_PrepareChartNetwork };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         ShowInformation();
private:
	void         InitReportHeader();
	void         UpdateReportData();
	void         ResizeComponent();
	void         SelectData(KIOColumns &oSelectedColumns, std::vector<TNetworkChartData> &a_vecNetworkChartData);
	CXTPChartContent* CreateBarChart(std::vector<TNetworkChartData> &a_vecNetworkChartData);
	CString      GetIDColumnName();
private:
	virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);

protected:
	KTarget*     m_pTarget;
	KIOTable*    m_pTable;
	KIOColumn*   m_pIDColumn;
	KIOColumn*   m_pNameColumn;
	KIOTable*    m_pPurposeODTable;
	KIOTable*    m_pModeODTable;
	KIOTable*    m_pParaODTable;    
	CString      m_strTableName;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonChart();
	afx_msg void OnBnClickedCancel();

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CXTPChartControl  m_wndChartControl;

};

typedef std::shared_ptr<KPrepareChartNetworkDlg> KPrepareChartNetworkPtr;