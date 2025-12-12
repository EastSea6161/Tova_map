/**
 * @file DefineModelDlg.h
 * @brief KDefineModelDlg ¼±¾ð
 * @author jyk@qbicware.com
 * @date 2012.01.05
 * @remark
 */


#pragma once
//#include "c:\program files\codejock software\mfc\xtreme toolkitpro v15.0.1\source\reportcontrol\xtpreportcontrol.h"

//^#include "ResizeDialogEx.h"

/* Forward declarations */
class KMode;

/**
 * @brief 
 * @remark
 * @warning
 * @version 1.0
 * @author jyk@qbicware.com
 * @date 2012.01.05
 */
class KDefineModeDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefineModeDlg)

	/**************************************
	* Constructors and Destructor         *
	**************************************/
public:
	KDefineModeDlg(CWnd* pParent = NULL);	//// standard constructor
	KDefineModeDlg(std::vector<KMode*> &vecModeInfo);
	virtual ~KDefineModeDlg();

// Dialog Data
	enum { IDD = IDD_1200_PrjModeDlg };

	/**************************************
	* Operations                          *
	**************************************/
public:
	void SetMaxModeId(int& a_nMaxModeId)
	{
		m_nMaxModeId = a_nMaxModeId;
	}

	void SetMode(std::vector<KMode*> &vecModeInfo);

	void SetModeGroup(KEMModeGroup a_emModeGroup)
	{
		m_emModeGroup = a_emModeGroup;
	}

	/**************************************
	* Overrides                           *
	**************************************/
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	void initModeList(void);
	void updateModeList(void);

protected:
	void ResizeComponet(void);
	/**************************************
	* Properties and Variables            *
	**************************************/
public:
	CString m_strClickedMode;
	CString m_strSelectedModeID;
	CString m_strSelectedModeGroup;
	int		m_nMaxModeId;

protected:

	enum EMModeManage
	{
		EMModeManageNone = 0,
		EMModeManageNew = 1,
		EMModeManageModify = 2,
		EMModeManageRemove = 3
	};

	struct TModeManageInfo
	{
		EMModeManage emManage;
		CString strName;
		CString strNewName;
		CString strSysDefineYN;
		int nModeGroupCode;
		int nDetailModeId;
	};
	
	std::list<TModeManageInfo*> m_ModeInfoList;
	std::vector<KMode*> *m_pvecModeInfo;
	KEMModeGroup m_emModeGroup;

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
	afx_msg void OnModeValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnBnClickedCheckSelectAll();

public:
	CString m_strSelectedModes;
	CString m_strMode;

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_reportMode;
};

