/**
 * @file KASetTransitAssignOptionsDlg.h
 * @brief Transit Assign Options 다이얼로그
 * @author nomplus2@qbicware.com
 * @date 2011.08.29
 * @remark 
 */

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

//#include "StdioFileEx.h"



/**
* @brief Transit Assign Options 다이얼로그
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.29
*/
class KASetTransitAssignOptionsDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KASetTransitAssignOptionsDlg)

public:
	KASetTransitAssignOptionsDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KASetTransitAssignOptionsDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6750_ATransitAssignDlg };

private:
    /**
    * @brief 단일 노선 조건
    */
    void EnableSingleRoute();

    /**
    * @brief 멀티 노선 조건
    */
    void EnableMultipleRoute();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnEnChangeEdit11();    
    afx_msg void OnBnClickedRadio1();
    afx_msg void OnBnClickedRadio2();

protected:
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
    

public:
	/*AssignModel Default Setting을 위한 외부제어 함수 추가.*/
	inline void RadioAssignRule(int a_nRadioIndex){m_nRadioAssignRule = a_nRadioIndex;}
	inline int RadioAssignRule(void){return m_nRadioAssignRule;}

	inline void NoOfRoute(int a_nNoOfRoute){m_nNoOfRoute = a_nNoOfRoute;}
	inline int NoOfRoute(void){return m_nNoOfRoute;}

	inline void InvehicleTime(double a_dInvehicleTime){m_dInvehicleTime = a_dInvehicleTime;}
	inline double InvehicleTime(void){return m_dInvehicleTime;}

	inline void WaitingTime(double a_dWaitingTime){m_dWaitingTime = a_dWaitingTime;}
	inline double WaitingTime(void){return m_dWaitingTime;}

	inline void TransferTime(double a_dTransferTime){m_dTransferTime = a_dTransferTime;}
	inline double TransferTime(void){return m_dTransferTime;}

	inline void AccessWalkTime(double a_dAccessWalkTime){m_dAccessWalkTime = a_dAccessWalkTime;}
	inline double AccessWalkTime(void){return m_dAccessWalkTime;}

	inline void EgressWalkTime(double a_dEgressWalkTime){m_dEgressWalkTime = a_dEgressWalkTime;}
	inline double EgressWalkTime(void){return m_dEgressWalkTime;}

	inline void NoOfTransfers(double a_dNoOfTransfers){m_dNoOfTransfers = a_dNoOfTransfers;}
	inline double NoOfTransfers(void){return m_dNoOfTransfers;}

	inline void Cost(double a_dCost){m_dCost = a_dCost;}
	inline double Cost(void){return m_dCost;}

	inline void FailToBoard(double a_dFailToBoard){m_dFailToBoard = a_dFailToBoard;}
	inline double FailToBoard(void){return m_dFailToBoard;}

	inline void SeatOccupancy(double a_dSeatOccupancy){m_dSeatOccupancy = a_dSeatOccupancy;}
	inline double SeatOccupancy(void){return m_dSeatOccupancy;}

	inline void MaxNoOfZoneConnectors(int a_nMaxNoOfZoneConnectors){m_nMaxNoOfZoneConnectors = a_nMaxNoOfZoneConnectors;}
	inline int MaxNoOfZoneConnectors(void){return m_nMaxNoOfZoneConnectors;}

	inline void MaxWalkingTimeToStation(double a_dMaxWalkingTimeToStation){m_dMaxWalkingTimeToStation = a_dMaxWalkingTimeToStation;}
	inline double MaxWalkingTimeToStation(void){return m_dMaxWalkingTimeToStation;}

	inline void TimeOfBoarding(double a_dTimeOfBoarding){m_dTimeOfBoarding = a_dTimeOfBoarding;}
	inline double TimeOfBoarding(void){return m_dTimeOfBoarding;}

    
private:
	int		m_nRadioAssignRule;
	int		m_nNoOfRoute;
	double	m_dInvehicleTime;
    double	m_dWaitingTime;
    double	m_dTransferTime;
    double	m_dAccessWalkTime;
    double	m_dEgressWalkTime;
    double	m_dNoOfTransfers;
    double	m_dCost;
    double	m_dFailToBoard;
    double	m_dSeatOccupancy;
	int		m_nMaxNoOfZoneConnectors;
	double	m_dMaxWalkingTimeToStation;
    double	m_dTimeOfBoarding; 

public:
    /**
    * @brief Transit Assign Option Argument File Write
    */
    bool WriteInputArgument(CStdioFileEx& of);
};
