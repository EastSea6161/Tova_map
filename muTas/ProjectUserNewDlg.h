/**
 * @file ProjectUserNewDlg.h
 * @brief KProjectUserNewDlg 선언
 * @author jyk@qbicware.com
 * @date 2011.12.27
 * @remark
 */

#pragma once


#include "resource.h"
#include "afxwin.h"

/* Forward declarations */
class KPurpose;
class KMode;

/**
 * @brief Project를 새로 생성하기 위해 정보를 입력 받는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.07.30
 */
class KProjectUserNewDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KProjectUserNewDlg)

public:
	KProjectUserNewDlg (CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KProjectUserNewDlg ();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_1030_PrjNewUserPassengerDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

public:
	void SetPurpose(std::vector<KPurpose*> a_vecPurpose)
	{
		m_vecPurpose = a_vecPurpose;
	}

	void SetMode(std::vector<KMode*> a_vecMode)
	{
		m_vecMode = a_vecMode;
	}

protected:
	bool IsEmptyProjectName();
	bool IsEmptyPurpose();
	bool IsEmptyMode();
	bool IsProjectPath();
	bool IsValidPath();

	void UpdateDeafultPurposeData();
	void UpdatePurposeData();
	void DisplayPurposeData(std::vector<KPurpose*> &a_vecPurpose);
	void UpdateDefaultModeData();
	void UpdateModeData();
	void DisplayModeData(std::vector<KMode*> &a_vecMode);

	static int CALLBACK BrowseCallBackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lPData );

public:
	bool m_bEdit;
	std::vector<KPurpose*> m_vecPurpose;
	std::vector<KMode*> m_vecMode;

protected:
	int m_nMaxPurposeId;
	int m_nMaxModeId;

	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickedPrjbtnLocation();
	afx_msg void OnClickedPrjbtnMode();
	afx_msg void OnClickedPrjbtnPurpose();
	afx_msg void OnClickedPrjbtnTargetyear();
	afx_msg void OnBnClickedPrjchkBaseyear();
	afx_msg void OnBnClickedOk();
	
public:
	CString m_strName;
	CString m_strInvestigator;
	CString m_strOrganization;
	CString m_strDescription;
	CString m_strLocation;
	int m_nBaseYear;
	CString m_strTargetYear;

	bool    m_bEditedPurpose;
	CString m_strPurposeName;
	CString m_strPurposeId;
	CString m_strPurposeGroup;
	
	bool    m_bEditedMode;
	CString m_strModeName;
	CString m_strModeId;
	CString m_strModeGroup;
	
	CString m_strLocDisplay;
	CButton m_btnLocation;
	CButton m_btnMode;
	CButton m_btnPurpose;
	CButton m_btnTargetYear;
	BOOL m_bBaseYear;
	int m_nMethodology;
	CSpinButtonCtrl m_spnBaseYear;
	afx_msg void OnBnClickedBtnView();
};
