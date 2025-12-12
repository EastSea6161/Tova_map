#pragma once

#include "resource.h"

#include "StdioFileEx.h"

// KInterTransitAssignDlg 대화 상자입니다.

class KInterTransitAssignDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KInterTransitAssignDlg)

public:
	KInterTransitAssignDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterTransitAssignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6950_InterTransitAssignDlg };

private:

	void EnableSingleRoute();
    void EnableMultipleRoute();

public:

	bool WriteInputArgument(CStdioFileEx& of);

protected:
	
	int m_nNoOfRoute;    
	int m_nRadioAssignRule;
	int m_nMaxNoOfZoneConnectors;

	CSpinButtonCtrl m_spinMaxNoOfRoute;    
	CSpinButtonCtrl m_spinMaxNoOfZoneConnectors;

	ZNumberEdit m_edtInVehicleTime;
	ZNumberEdit m_edtWaitingTime;

	ZNumberEdit m_edtTransferTime;
	ZNumberEdit m_edtAccessWalkTime;
	ZNumberEdit m_edtEgressWalkTime;
	ZNumberEdit m_edtNoOfTransfers;
	ZNumberEdit m_edtCost;
	ZNumberEdit m_edtFailToBoard;
	ZNumberEdit m_edtSeatOccupancy;

	ZNumberEdit m_edtMaxWalkingTimeToStation;
	ZNumberEdit m_editTimeOfBoarding;

private:

	double m_dblInvehicleTime;
	double m_dblWaitingTime;
	double m_dblTransferTime;
	double m_dblAccessWalkTime;
	double m_dblEgressWalkTime;
	double m_dblNoOfTransfers;
	double m_dblCost;
	double m_dblFailToBoard;
	double m_dblSeatOccupancy;
	double m_dblMaxWalkingTimeToStation;
	double m_dblTimeOfBoarding; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit11();    
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
