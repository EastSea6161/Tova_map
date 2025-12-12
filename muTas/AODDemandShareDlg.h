/**
 * @file AODDemandShareDlg.h
 * @brief KAODDemandShareDlg 선언 파일
 * @author 
 * @date 2011.06.17
 * @remark
 */


#pragma once
#include "afxwin.h"


/**
 * @brief 배정 모형의 OD Demand share per iteration 설정 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.17
 */
class KAODDemandShareDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAODDemandShareDlg)

public:
	KAODDemandShareDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAODDemandShareDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6760_AODDemandDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :
	
	/**
	* @brief Iteration을 반환한다.
	* @return int - Iteration
	*/
	int Iteration(void);

	/**
	* @brief Iteration을 설정한다.
	* @param[in] nIteration - Iteration
	*/
	void Iteration(int nIteration);

	/**
	* @brief Iteration별 비율을 반환한다.
	* @return std::list<double>& - Iteration별 비율
	*/
	std::list<double>& RateList(void);


protected :
	int m_nIteration;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CString m_strSum;
	CEdit m_edtIteration;

	double m_dRate;
	double m_dLastRate;
	std::list<double> m_RateList;

     /**
    * @brief ReportControl 초기화
    */
	void initReportControl(void);

    /**
    * @brief ReportControl Update
    */
	void updateReportRecord(void);

    /**
    * @brief Iteration에 따른 비율 계산
    */
	void AutoRateDevide(void);

    /**
    * @brief grid 내 표출 되어 있는 Rate의 전체 합
    */
	void SumTripRate(void);
public:

    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnEnChangeEdit1();
	afx_msg void OnRateValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	

};
