/**
 * @file KTimeODInputDlg.h
 * @brief Time OD Input 
 * @author jyk@qbicware.com
 * @date 2012.05.14
 * @remark 
 */

#pragma once
#include "afxwin.h"
#include "timeod_common.h"


class KTarget;
class KIOColumns;

// KTimeODInputDlg 대화 상자입니다.

class KTimeODInputDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KTimeODInputDlg)

public:
	KTimeODInputDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTimeODInputDlg();
	
// 대화 상자 데이터입니다.
	enum { IDD = IDD_8010_TimeODInputDlg };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	/**
	* @brief Input 정보 vector를 설정한다.
	* @param[in] a_vecInputModeInfo - 설정한 Input 정보 vector
	*/
	void InputModeInfo(std::vector<TInputModeInfo>    &a_vecInputModeInfo)
	{
		m_vecInputModeInfo = a_vecInputModeInfo;
	}

	/**
	* @brief Input 정보 vector를 반환한다.
	* @return std::vector<TInputModeInfo>& - 설정한 Input 정보 vector
	* @remark 
	* @warning 
	*/
	std::vector<TInputModeInfo>& InputModeInfo(void)
	{
		return m_vecInputModeInfo;
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


	void CheckboxControl(BOOL a_bCheck);


	void InputNameGridComboData(	CXTPReportRecordItemEditOptions* a_pFieldNameEditOptions );
	void InputTimeGridComboData(	CXTPReportRecordItemEditOptions* a_pParaODEditOptions    );

	
	bool FindODColumn(KIOColumn* a_pColumn, CXTPReportRecordItemConstraints* a_pConstraints);

protected:
	KTarget*     m_pTarget;

	std::vector<TInputModeInfo>    m_vecInputModeInfo;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
	afx_msg void OnSelectCancel();
	afx_msg void OnSelectAll();
	afx_msg void OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ReportCtrl;
};
