/**
 * @file KTimeODSelectSurveyCodeDlg.h
 * @brief Select Survey code
 * @author jyk@qbicware.com
 * @date 2012.05.16
 * @remark 
 */

#pragma once
#include "timeod_common.h"

// KTimeODSelectSurveyCodeDlg 대화 상자입니다.

class KTimeODSelectSurveyCodeDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KTimeODSelectSurveyCodeDlg)

public:
	KTimeODSelectSurveyCodeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTimeODSelectSurveyCodeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8034_TimeODSelectSurveyCodeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetSurveyDataType(KEMSurveyDataType a_emSurveyDataType)
	{
		m_emSurveyDataType = a_emSurveyDataType; 
	}

	void SetName(CString a_strName)
	{
		m_strName = a_strName;
	}

	void SetTotalCodes(KTDSurveyCodeSet &a_setTotalCode)
	{
		m_setTotalCode = a_setTotalCode;
	}

	/**
	* @brief 선택한 code 정보 set을 설정한다.
	* @param[in] a_setSelectedCode - 선택한 code 정보 set
	*/
	void SelectedCodes(KTDSurveyCodeSet &a_setSelectedCode)
	{
		m_setSelectedCode = a_setSelectedCode;
	}

	/**
	* @brief 선택한 code 정보 set을 반환한다.
	* @return KTDSurveyCodeSet& - 선택한 code 정보 set
	*/
	KTDSurveyCodeSet& SelectedCodes(void)
	{
		return m_setSelectedCode;
	}


private:
	void InitTitle();

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

	/**
	* @brief 선택 했던 코드가 존재 하는지 여부
	*/
	bool IsSelected( int &a_nSelectedCode );
	 
protected:
	CString           m_strName;
	KEMSurveyDataType m_emSurveyDataType;
	KTDSurveyCodeSet  m_setTotalCode;
	KTDSurveyCodeSet  m_setSelectedCode;

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
public:
	CString m_strTitle;
};
