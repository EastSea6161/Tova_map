#pragma once

class KTarget;
#include "DBaseDefaultInterRunOption.h"

// KDefaultInterRunOptionDlg 대화 상자입니다.

class KDefaultInterRunOptionDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultInterRunOptionDlg)

public:
	KDefaultInterRunOptionDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultInterRunOptionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8513_Default_InterRunOptionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndUrbanReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndRegionalReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndFreightReport;

protected:

	void InitDuationEditContol(void);

	void InitReportUrban(void);
	void InitRoportRegional(void);
	void InitReportFreight(void);
	
	void UpdateUrbanData(void);
	void UpdateRegionalData(void);
	void UpdateFreightData(void);
	

	void InvalidData(std::map<int, KInterOccupancePCEMode>& a_mapModeOPInfo);
	void SaveFreightData(std::map<int, KInterOccupancePCEMode> a_mapModeOPInfo);
	void SaveUrbanData();
	void SaveRegionalData();
	void SaveDuation();

	void ResizeComponet(void);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedDefault();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
