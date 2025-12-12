/**
 * @file KTimeODZoneCharacterDlg.h
 * @brief Time OD Zone Character
 * @author jyk@qbicware.com
 * @date 2012.05.15
 * @remark 
 */

#pragma once
#include "afxwin.h"
#include "timeod_common.h"

class KTarget;
class KIOColumns;

// KTimeODZoneCharacterDlg 대화 상자입니다.

class KTimeODZoneCharacterDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KTimeODZoneCharacterDlg)

public:
	KTimeODZoneCharacterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTimeODZoneCharacterDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8020_TimeODZoneCharacterDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	void SetTarget(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	/**
	* @brief Zone Character 설정 정보 vector를 설정한다.
	* @param[in] a_vecZoneCharacterInfo - 설정한 Zone Character 정보 vector
	*/
	void ZoneCharacterInfo(std::vector<TZoneCharacterInfo>    &a_vecZoneCharacterInfo)
	{
		m_vecZoneCharacterInfo = a_vecZoneCharacterInfo;
	}

	/**
	* @brief Input 정보 vector를 반환한다.
	* @return std::vector<TZoneCharacterInfo>& - 설정한 Zone Character 정보 vector
	* @remark 
	* @warning 
	*/
	std::vector<TZoneCharacterInfo>& ZoneCharacterInfo(void)
	{
		return m_vecZoneCharacterInfo;
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


	void InputSedGridComboData(	CXTPReportRecordItemEditOptions* a_pSedEditOptions );
	void InputNumGridComboData(	CXTPReportRecordItemEditOptions* a_pNumEditOptions );

	
	bool FindSedColumn(KIOColumn* a_pColumn, CXTPReportRecordItemConstraints* a_pConstraints);


	bool FindClassNum (int a_nClassNum, CXTPReportRecordItemConstraints* a_pConstraints);

protected:
	KTarget*     m_pTarget;

	std::vector<TZoneCharacterInfo>    m_vecZoneCharacterInfo;

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
