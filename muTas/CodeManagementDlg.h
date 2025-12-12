/**
 * @file CodeManagementDlg.h
 * @brief KCodeManagementDlg 선언 파일
 * @author 
 * @date 2011.07.06
 * @remark
 */


#pragma once


#include "afxcmn.h"
#include "afxwin.h"


/* Forward declarations */
class KCodeManager;
class KCodeGroup;
class KTarget;


enum EMCodeManage
{
	EMCodeManageNone= 0,
	EMCodeManageNew = 1,
	EMCodeManageModify = 2,
	EMCodeManageRemove = 3
};

struct TUseGroupTblInfo
{
	CString strTblName;
	CString strColName;
};

struct STCodeGroupManageInfo
{
	EMCodeManage emManage;
	CString strName;
	CString strNewName;
	bool bUsed;
	std::list<TUseGroupTblInfo*> useGroupInfoList;
	int nKey;
	CString strSysCodeYN;
};

struct STCodeManageInfo
{
	EMCodeManage emManage;
	int nCode;
	CString strName;
	CString strNewName;
};

/**
 * @brief 코드 Group/data를 추가/수정/삭제하는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.07.06
 */
class KCodeManagementDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KCodeManagementDlg)
public:
	KCodeManagementDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCodeManagementDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4210_ManagementCodeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()

public:
	void CodeManager(KCodeManager* pCodeManager);
	const KCodeManager* CodeManager(void) const;
	void SetTarget(KTarget* pTarget);
protected:
	void initCodeGroupList(void);
	void updateCodeGroupList(void);
	void initCodeList(void);
	void ReloadCodeList(void);
	CXTPReportRecord* GetCurrentGroupRecord(void);
	void updateCodeGroupData(void);
	void updateCodeData(void);
	void GetUseGroupInfo(int nGKey, bool &bUse, std::list<TUseGroupTblInfo*> &useGroupInfoList);
	void SetFocusControl(int a_nIndex);

protected:
	KCodeManager* m_pCodeManager;
	KTarget*      m_pTarget;
	bool m_bNewGroup;
	bool m_bNewCode;
	int  m_nNewCode;
	int  m_nNewGroupKey;

	std::list<STCodeGroupManageInfo*> m_CodeGroupInfoList;
	std::map<STCodeGroupManageInfo*, std::list<STCodeManageInfo*>> m_CodeInfoMap;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();						//Remove Group
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnGroupSelChanging(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnGroupSelChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnGroupValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnGroupEditCanceled(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnCodeValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportGroup;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCode;
	CButton m_btnRemoveGroup;
	CButton m_btnAddCode;
	CButton m_btnRemoveCode;
public:
    afx_msg void OnBnClickedButton5();
    afx_msg void OnBnClickedButton6();
    afx_msg void OnBnClickedOk();

private:
    bool m_bEdited;
private:
    void DeleteAllCode();
public:
    afx_msg void OnBnClickedCancel();
};
