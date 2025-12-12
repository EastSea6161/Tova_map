/**
 * @file ScenarioAddYearDlg.h
 * @brief KScenarioAddYearDlg 선언
 * @author 
 * @date 2010.08.18
 * @remark
 */


#pragma once
#include "afxwin.h"



/**
 * @brief Scenario에 target year를 추가하기위해 정보를 입력받는다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.18
 */
class kScenarioAddYearDlg : public KDialogEx
{
	DECLARE_DYNAMIC(kScenarioAddYearDlg)

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_2140_AddYearScenarioDlg };


	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	kScenarioAddYearDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~kScenarioAddYearDlg();


	/**************************************
	* Accessors                           *
	**************************************/


	/**************************************
	* Operations                          *
	**************************************/
protected :
	bool isYearExist( int nYear );

	/**************************************
	* Overrides                           *
	**************************************/
public :
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	/**************************************
	* Properties / Variables              *
	**************************************/
public:
	int m_nTargetYear;
	std::vector< int > m_YearArray;


	/**************************************
	* Message handlers                    *
	**************************************/
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

    afx_msg void OnBnClickedCancel();
};
