#pragma once

#include "DBaseDefaultParaInterPathGenerator.h"
#include "DBaseInterModal.h"

class KTarget;

// KDefaultInterPathGenerator 대화 상자입니다.

class KDefaultInterPathGenerator : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefaultInterPathGenerator)

public:
	KDefaultInterPathGenerator(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefaultInterPathGenerator();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8512_Default_Inter_PathGenerationPara };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndUrbanPathReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndUrbanAutoReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndUrbanTransitReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndRegionalPathReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndRegionalAutoReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndRegionalTerminalReport;

	CString m_strAddAccegr;
	CString m_steTooltipAceeEgr;

protected:
	std::map<int, std::vector<TUrbanPathInfo>>       m_mapUrbanPathInfo;
	std::map<int, std::vector<TInterAutoCost>>       m_mapUrbanAutoCost;
	std::map<int, std::vector<TInterTaxiCost>>       m_mapUrbanTransitCost;
	std::map<int, std::vector<TRegionPathInfo>>      m_mapRegionalPathInfo;
	std::map<int, std::vector<TInterAutoCost>>       m_mapRegionalAutoCost;
	std::map<int, std::vector<TTerminalProcessTime>> m_mapRegionalTerminalTime;
	std::map<int, KPathTimeSetting>                  m_mapRegionalPathTimeSetting;
	std::map<int, TUrbanPathInfo>                    m_mapSetUrbanPathInfo;
	std::map<int, TInterAutoCost>                    m_mapSetAutoCost;
	std::map<int, CString>							 m_mapRegionalModeInfo;
	std::map<int, TTerminalProcessTime>              m_mapSetRegionalTerminalTime;

protected:
	void ResizeComponent();

	void InitUrbanPathReportHeader();
	void InitUrbanAutoReportHeader();
	void InitUrbanTransitReportHeader();
	void InitRegionalPathReportHeader();
	void InitRegionalAutoReportHeader();
	void InitRegionalTerminalReportHeader();

	void InitRegionalTimeSetting(int a_nMasterCode);
	void InitUrbanPathReportRecord(int a_nMasterCode);
	void InitUrbanAutoReportRecord(int a_nMasterCode);
	void InitUrbanTransitReportRecord(int a_nMasterCode);
	void InitRegionalPathReportRecord(int a_nMasterCode);
	void InitRegionalAutoReportRecord(int a_nMasterCode);
	void InitRegionalTerminalReportRecord(int a_nMasterCode);

	void SetRegionalTimeSetting();
	void SetUrbanPathReportRecord();
	void SetUrbanAutoReportRecord();
	void SetUrbanTransitReportRecord();
	void SetRegionalPathReportRecord();
	void SetRegionalAutoReportRecord();
	void SetRegionalTerminalReportRecord();

	bool InvalidateUrbanAutoInputInfo();
	bool InvalidateRegionalAutoInputInfo();

	void SaveRegionalTimeSetting(int a_nMasterCode);
	void SaveUrbanPathReportRecord(int a_nMasterCode);
	void SaveUrbanAutoReportRecord(int a_nMasterCode);
	void SaveUrbanTransiReportRecord(int a_nMasterCode);
	void SaveRegionalPathReportRecord(int a_nMasterCode);
	void SaveRegionalAutoReportRecord(int a_nMasterCode);
	void SaveRegionalTerminalReportRecord(int a_nMasterCode);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDefault();
};
