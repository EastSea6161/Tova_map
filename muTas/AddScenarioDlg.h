/**
 * @file AddScenarioDlg.h
 * @brief KAddScenarioDlg 선언
 * @author 
 * @date 2010.07.30
 * @remark
 */

#pragma once


#include "Resource.h"


/**
 * @brief 새로운 시나리오 추가를 위해 시나리오 정보를 입력받는 Dialog
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.07.30
 */
class KAddScenarioDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAddScenarioDlg)

	/**************************************
	* Constructors and Destructor         *
	**************************************/
public:
	KAddScenarioDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~KAddScenarioDlg();

// Dialog Data
	enum { IDD = IDD_2110_AddScenarioDlg };


	/**************************************
	* Operations                          *
	**************************************/
protected :
	void updateButtonState(void);
    bool CheckDuplicateScenarioName();

	/**************************************
	* Overrides                           *
	**************************************/
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	/**************************************
	* Properties and Variables            *
	**************************************/
public:
	CString m_strProjectName;
	CString m_strNoOfScenario;
	CString m_strTargetYear;
	ZListCtrlEx m_lstScenario;
	
	std::vector<CString> m_arrScenario;
	std::set< CString >  m_ExistsNameSet;
	int m_nNewScenarioCount;
	int m_nNameIndex;

    void SetNameIndex(int a_nNameIndex)
    {
        m_nNameIndex = a_nNameIndex;
    }


	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickedPrjbtnAddscenario();
	afx_msg void OnClickedPrjbtnDeletescenario();
	afx_msg void OnClickedPrjbtnModifyscenario();
	afx_msg void OnLvnItemchangedPrjlstScenario(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditPrjlstScenario(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnEndlabeleditPrjlstScenario(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnInsertitemPrjlstScenario(NMHDR *pNMHDR, LRESULT *pResult);
};
