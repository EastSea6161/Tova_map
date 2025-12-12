/**
 * @file KTimeODDefPurposeMode.h
 * @brief Define Purpose and Mode 
 * @author jyk@qbicware.com
 * @date 2012.05.16
 * @remark 
 */

#pragma once

#include "timeod_common.h"

// KTimeODDefPurposeMode 대화 상자입니다.

class KTimeODDefPurposeModeDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KTimeODDefPurposeModeDlg)

public:
	KTimeODDefPurposeModeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTimeODDefPurposeModeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8030_TimeODDefinePurposeModeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	/**
	* @brief  전체 목적 Survey Code set을 설정한다.
	* @param[in] a_setTotalPurpseSurveyCode - 전체 목적 Survey Code set
	*/
	void TotalPurposeSurveyCodes(KTDSurveyCodeSet &a_setTotalPurpseSurveyCode)
	{
		m_setTotalPurpseSurveyCode = a_setTotalPurpseSurveyCode;
	}

	/**
	* @brief 전체 목적 Survey Code set을 반환한다.
	* @return KTDSurveyCodeSet& - 설정한 전체 목적 Survey Code set
	*/
	KTDSurveyCodeSet& TotalPurposeSurveyCodes(void)
	{
		return m_setTotalPurpseSurveyCode;
	}

	/**
	* @brief  전체 수단 Survey Code set을 설정한다.
	* @param[in] a_setTotalModeSurveyCode - 전체 수단 Survey Code set
	*/
	void TotalModeSurveyCodes(KTDSurveyCodeSet &a_setTotalModeSurveyCode)
	{
		m_setTotalModeSurveyCode = a_setTotalModeSurveyCode;
	}

	/**
	* @brief 전체 수단 Survey Code set을 반환한다.
	* @return KTDSurveyCodeSet& - 설정한 전체 수단 Survey Code set
	*/
	KTDSurveyCodeSet& TotalModeSurveyCodes(void)
	{
		return m_setTotalModeSurveyCode;
	}

	/**
	* @brief  목적 별 Survey Code 정보 vector를 설정한다.
	* @param[in] a_vecSurveyPurpose - 목적 별 Survey Code vector
	*/
	void PurposeSurvey(std::vector<TSurveyPurpose> &a_vecSurveyPurpose)
	{
		m_vecSurveyPurpose  =  a_vecSurveyPurpose;
		m_pVecSurveyPurpose = &a_vecSurveyPurpose;
	}

	/**
	* @brief 목적 별 Survey Code vector를 반환한다.
	* @return std::vector<TSurveyPurpose>& - 설정한 목적 별 Survey Code vector
	*/
	std::vector<TSurveyPurpose>& PurposeSurvey(void)
	{
		return m_vecSurveyPurpose;
	}

	/**
	* @brief  수단 별 Survey Code 정보 vector를 설정한다.
	* @param[in] a_vecSurveyMode - 수단 별 Survey Code vector
	*/
	void ModeSurvey(std::vector<TSurveyMode> &a_vecSurveyMode)
	{
		m_vecSurveyMode   =  a_vecSurveyMode;
		m_pVecSurveyMode  = &a_vecSurveyMode;
	}

	/**
	* @brief 수단 별 Survey Code vector를 반환한다.
	* @return std::vector<TSurveyMode>& - 설정한 수단 별 Survey Code vector
	*/
	std::vector<TSurveyMode>& ModeSurvey(void)
	{
		return m_vecSurveyMode;
	}

private:
	/**
	* @brief Report Control Header 초기화
	*/
	void InitReportHeader();
	void InitPurposeReportHeader();
	void InitModeReportHeader();

	/**
	* @brief 정보 Display
	*/
	void InitReportData();
	void InitPurposeReportData();
	void InitModeReportData();

	/**
	* @brief component resize seting
	*/
	void SetResizeComponent();

	/**
	* @brief 코드 집합을 (,) 구분된 문자열로 변환함.
	*/
	void CodesToStiring(KTDSurveyCodeSet &a_setSurveyCode, CString &a_strCodes);

	/**
	* @brief 이전에 할당되어 있던 코드를 삭제 시킨다. 
	*/
	void ClearPrevSurveyCode();

protected:
	KTDSurveyCodeSet               m_setTotalPurpseSurveyCode;
	KTDSurveyCodeSet               m_setTotalModeSurveyCode;

	std::vector<TSurveyPurpose>    m_vecSurveyPurpose;
	std::vector<TSurveyMode>       m_vecSurveyMode;
	std::vector<TSurveyPurpose>*   m_pVecSurveyPurpose;
	std::vector<TSurveyMode>*      m_pVecSurveyMode;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnItemPurposeButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult);
	afx_msg void OnItemModeButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult);
	afx_msg LRESULT OnSurveyPurposeEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSurveyModeEdit(WPARAM wParam, LPARAM lParam);
	
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_PurposeReportCtrl;
	CXTPOfficeBorder<CXTPReportControl,false> m_ModeReportCtrl;
	
};
