/**
 * @file DefinePurposeDlg.h
 * @brief KDefinePurposeDlg ¼±¾ð
 * @author 
 * @date 2010.07.30
 * @remark
 */


#pragma once

#include "model_common.h"
//^#include "ResizeDialogEx.h"

/* Forward declarations */
class KPurpose;

/**
 * @brief 
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.07.30
 */
class KDefinePurposeDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefinePurposeDlg)

	/**************************************
	* Constructors and Destructor         *
	**************************************/
public:
	KDefinePurposeDlg(CWnd* pParent = NULL);   // standard constructor
	KDefinePurposeDlg(std::vector<KPurpose*> &vecPurposeInfo);
	virtual ~KDefinePurposeDlg();

// Dialog Data
	enum { IDD = IDD_1100_PrjPurposeDlg };

	/**************************************
	* Operations                          *
	**************************************/
public:
	void SetMaxPurposeId(int& a_nMaxPurposeId)
	{
		m_nMaxPurposeId = a_nMaxPurposeId;
	}

	void SetPurpose(std::vector<KPurpose*> &vecPurposeInfo);

	void SetPurposeGroup(KEMPurposeGroup a_emPurposeGroup)
	{
		m_emPurposeGroup = a_emPurposeGroup;
	}

protected:
	void initializeTreeItems(void);
	
	/**************************************
	* Overrides                           *
	**************************************/
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	void initPurposeList(bool bPassengerMode);
	void updatePurposeList(void);

	/**************************************
	* Properties and Variables            *
	**************************************/

protected:
	void ResizeComponet(void);

public:
	CString m_strClickedPurpose;
	CString m_strSelectedPurposeID;
	CString m_strSelectedPurposeGroup;
	int		m_nMaxPurposeId;
	int		m_nSelectMaxPurposeID;

private:

	bool m_bPassenger;

protected:
	enum EMPurposeManage
	{
		EMPurposeManageNone = 0,
		EMPurposeManageNew = 1,
		EMPurposeManageModify = 2,
		EMPurposeManageRemove = 3
	};

	struct TPurposeManageInfo
	{
		EMPurposeManage emManage;
		CString strName;
		CString strNewName;
		CString strSysDefineYN;
		int nObjectGroupCode;
		int nDetailObjectId;
	};

	std::list<TPurposeManageInfo*> m_listPurposeInfoManager;
	std::vector<KPurpose*> *m_pVecPurposeInfo;
	KEMPurposeGroup m_emPurposeGroup;

	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	
	afx_msg void OnPurposeValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnBeginPurposeNameEdit(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnReportItemClick(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);

public:
	CString m_strSelectedPurposeNames;
	CString m_strPurpose;
	CImageList m_ilTreeState;
	//ZTriStateTreeCtrl m_trPurpose;

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_reportPurpose;
};




