#pragma once

#include "resource.h"

/*	Forward declare */
class KInterDefineCostPublicSector;

// KInterDefineCostPublicPage 대화 상자입니다.

class KInterDefineCostPublicPage : public CXTPPropertyPage
{
	DECLARE_DYNAMIC(KInterDefineCostPublicPage)

public:
	KInterDefineCostPublicPage(std::vector<KInterDefineCostPublicSector*> a_vecPublicRecord);
	virtual ~KInterDefineCostPublicPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6912_InterDefineCostPublicPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void   GetRecords(void);

	void   MaxNoOfZoneConnectors(int a_nMaxNoOfZoneConnectors);
	int    MaxNoOfZoneConnectors(void);

	void   MaxWalkingTimeToStation(double a_dMaxWalkingTimeToStation);
	double MaxWalkingTimeToStation(void);

	void   TimeOfBoarding(double a_dTimeOfBoarding);
	double TimeOfBoarding(void);

	inline void NoOfRoute(int a_nNoOfRoute){m_nNoOfRoute = a_nNoOfRoute;}
	inline int  NoOfRoute(void){return m_nNoOfRoute;}

	void   TaxiWaitingTime(double a_dTaxiWaitingTime);
	double TaxiWaitingTime(void);

	void   TaxiStoppingTime(double a_dTaxiStoppingTime);
	double TaxiStoppingTime(void);

private:
	void InitPublicReportControl();
	void UpdatePublicReportRecord();

private:
	int	   m_nNoOfRoute;
	int    m_nMaxNoOfZoneConnectors;
	double m_dMaxWalkingTimeToStation;
	double m_dTimeOfBoarding; 

	//대기시간
	double m_dTaxiWaitingTime;
	//정지시간
	double m_dTaxiStoppingTime;
	
	std::vector<KInterDefineCostPublicSector*> m_vecPublicRecord;

private:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit11();

private:
	CXTPReportControl m_wndPublicReport;
	CSpinButtonCtrl   m_spinMaxNoOfRoute;
	CSpinButtonCtrl   m_spinMaxNoOfZoneConnectors;

	ZNumberEdit       m_edtTaxiWaitingTime;
	ZNumberEdit       m_edtTaxiStoppingTime;

	ZNumberEdit       m_edtMaxWalkingTimeToStation;
	ZNumberEdit       m_editTimeOfBoarding;
};
