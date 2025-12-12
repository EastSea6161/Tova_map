/**
 * @file KTimeODInputDlg.h
 * @brief Time OD Input 
 * @author jyk@qbicware.com
 * @date 2012.05.18
 * @remark 
 */

#pragma once
#include "afxwin.h"
#include "timeod_common.h"


class KTarget;
class KIOColumns;

// KTimeODOutputDesignDlg 대화 상자입니다.

class KTimeODOutputDesignDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KTimeODOutputDesignDlg)

public:
	KTimeODOutputDesignDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTimeODOutputDesignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8040_TimeODOutputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	/**
	* @brief Output 정보 vector를 설정한다.
	* @param[in] a_vecOutputModeInfo - 설정한 Output 정보 vector
	*/
	void OutputModeInfo(std::vector<TOutputModeInfo>    &a_vecOutputModeInfo)
	{
		m_vecOutputModeInfo = a_vecOutputModeInfo;
	}

	/**
	* @brief Output 정보 vector를 반환한다.
	* @return std::vector<TOutputModeInfo>& - 설정한 Ouput 정보 vector
	* @remark 
	* @warning 
	*/
	std::vector<TOutputModeInfo>& OutputModeInfo(void)
	{
		return m_vecOutputModeInfo;
	}

private:
	/**
	* @brief Report Control Header 초기화
	*/
	void InitReportHeader();

	/**
	* @brief 정보 Display
	*/
	void InitReportData();

	/**
	* @brief component resize seting
	*/
	void SetResizeComponent();


	void AddOutputColumns(CXTPReportRecord* a_pParent, TOutputModeInfo &a_oTOutModeInfo);


	bool IsNameExists(CXTPReportRecord* a_pParent, LPCTSTR strName);


	bool ApplyOutputField(CXTPReportRecord* a_pModeRecord);

protected:
	KTarget*     m_pTarget;

	std::vector<TOutputModeInfo>    m_vecOutputModeInfo;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg LRESULT OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam);
	
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportCtrl;

};
