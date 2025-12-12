/**
 * @file ScenarioCopyDlg.h
 * @brief KScenarioopyDlg 선언
 * @author 
 * @date 2010.08.17
 * @remark
 */


#pragma once
#include "afxcmn.h"


/* Forward declarations */
class KScenario;


/**
 * @brief Scenario copy dialog
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.17
 */
class KScenarioCopyDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KScenarioCopyDlg)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_2120_CopyScenarioDlg };

	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KScenarioCopyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KScenarioCopyDlg();


	/**************************************
	* Accessors                           *
	**************************************/


	/**************************************
	* Operations                          *
	**************************************/


	/**************************************
	* Overrides                           *
	**************************************/
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	/**************************************
	* Properties / Variables              *
	**************************************/
public :
	CListCtrl m_lstDestScenario;

	KScenario* m_pSourceScenario;
	std::vector< KScenario* > m_DestScenarioArray;

	bool m_bCopyResult;

	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
