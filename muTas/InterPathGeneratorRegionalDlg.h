#pragma once

#include "DBaseInterModal.h"
#include "DBaseDefineTransitCost.h"

class KTarget;

// KInterPathGeneratorRegionalDlg 대화 상자입니다.

class KInterPathGeneratorRegionalDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterPathGeneratorRegionalDlg)

public:
	KInterPathGeneratorRegionalDlg(KTarget* a_pTarget, TAssignRule a_oParameter, std::vector<TInterAutoCost> a_vecAutoCost,
		std::vector<TInterTaxiCost> a_vecTaxiCost,std::map<int, TTerminalProcessTime> a_mapTerminal, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterPathGeneratorRegionalDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6840_IntermodalPathGeneratorRegionalDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void GetParameterValue(TAssignRule& a_oParameter, std::vector<TInterAutoCost>& a_vecAutoCost,
		std::vector<TInterTaxiCost>& a_vecTaxiCost,std::map<int, TTerminalProcessTime>& a_mapTerminal);

protected:

	KTarget* m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportAuto;
	//CXTPOfficeBorder<CXTPReportControl,false> m_wndReportTaxi;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportTermianl;

	//Data 관련
	TAssignRule	m_oRegionalParameter;
	std::vector<TInterAutoCost>			m_vecRegionalAutoCost;
	std::vector<TInterTaxiCost>			m_vecRegionalTaxiCost;
	std::map<int, TTerminalProcessTime>	m_mapRegionalProcessTime;

protected:
	void InitAuotoHeader(void);
	//void InitTaxiHeader(void);
	void InitTerminalHeader(void);

	void UpdateAutoData(void);
	//void UpdateTaxiData(void);
	void UpdateTerminalData(void);

	void InitParameterControl(void);

protected:
	bool SaveAutoValue(void);
	//void SaveTaxiValue(void);
	void SaveTermianlValue(void);
	bool SaveParameterValue(void);


protected:
	void ReSizeComponent(void);

protected:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
