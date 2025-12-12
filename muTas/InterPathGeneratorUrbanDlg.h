#pragma once

#include "DBaseInterModal.h"
#include "DBaseDefineTransitCost.h"

class KTarget;

// KInterPathGeneratorUrbanDlg 대화 상자입니다.

class KInterPathGeneratorUrbanDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterPathGeneratorUrbanDlg)

public:
	KInterPathGeneratorUrbanDlg(KTarget* a_pTarget, std::vector<TInterAutoCost> a_vecUrbanAuto, std::vector<TInterTaxiCost> a_vecUrbanTaxi,
		std::vector<TTransitCost> a_vecTransit, TAssignRule a_oUrbanPara, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterPathGeneratorUrbanDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6840_IntermodalPathGeneratorUrbanDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void GetParameterValue(std::vector<TInterAutoCost>& a_vecUrbanAuto, std::vector<TInterTaxiCost>& a_vecUrbanTaxi, 
		std::vector<TTransitCost>& a_vecTransit, TAssignRule& a_oUrbanPara);


protected:
	KTarget* m_pTarget;

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportAuto;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportTaxi;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportTransit;

	//Data 관련
	TAssignRule	m_oUrbanParameter;
	std::vector<TInterAutoCost> m_vecUrbanAutoCost;
	std::vector<TInterTaxiCost> m_vecUrbanTaxiCost;
	std::vector<TTransitCost>	m_vecTransitCost;

protected:
	void InitReportHeaderAuto(void);
	void InitReportHeaderTaxi(void);
	void InitReportHeaderTransit(void);

	void UpdateReportDataAuto(void);
	void UpdateReportDataTaxi(void);
	void UpdateReportDataTransit(void);

	void InitPathParameter(void);

protected:
	bool SaveAutoValue(void);
	void SaveTaxiValue(void);
	void SaveTransitValue(void);
	bool SaveParameterValue(void);
	
protected:
	void ReSizeComponent( void );


protected:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
};
