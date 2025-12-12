#pragma once
//^#include "ResizeDialogEx.h"
#include "DBaseDefineTransitCost.h"

class KTarget;

// KParameterDefineCostDlg 대화 상자입니다.

class KParameterDefineCostDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KParameterDefineCostDlg)

public:
	KParameterDefineCostDlg(std::vector<TParaOutputInfo> a_vecOutputInfo, KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KParameterDefineCostDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4601_ODParaDefineCostDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;

	bool m_bUseAuto;
	bool m_bUseTaxi;
	bool m_bUseTruck;
	bool m_bUseTransit;
	bool m_bUseBus;
	bool m_bUseRail;
	bool m_bUseBusRail;


	CXTPOfficeBorder<CXTPReportControl,false> m_wmAutoReportControl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmTaxiReportControl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmTruckReportControl;
	CXTPOfficeBorder<CXTPReportControl,false> m_wmTransitReportControl;


	std::vector<TParaOutputInfo>	m_vecOutputInfo;
	std::vector<TAutoFuel>			m_vecAutoCost;
	std::vector<TTaxiFuel>			m_vecTaxiCost;
	std::vector<TTruckFuel>			m_vecTruckCost;
	std::vector<TTransitCost>		m_vecTransitCost;

public:
	void SetCostInfo(std::vector<TAutoFuel> a_vecAutoCost, std::vector<TTaxiFuel> a_vecTaxiCost, 
					std::vector<TTruckFuel> a_vecTruckCost, std::vector<TTransitCost> a_vecTransitCost);

	void GetCostInfo(std::vector<TAutoFuel>& a_vecAutoCost, std::vector<TTaxiFuel>& a_vecTaxiCost, 
					std::vector<TTruckFuel>& a_vecTruckCost, std::vector<TTransitCost>& a_vecTransitCost);


private:
	void CheckUseMode();

	void InitAutoReportHeader();
	void InitTaxiReportHeader();
	void InitTruckReportHeader();
	void InitTransitReportHeader();

	void UpdateAutoReportColumn();
	void UpdateTaxiReportHeader();
	void UpdateTruckReportHeader();
	void UpdateTransitReportHeader();

	void GetAutoCostResult();
	void GetTaxiCostResult();
	void GetTruckCostResult();
	void GetTransitCostResult();

	void ResizeComponent();

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
