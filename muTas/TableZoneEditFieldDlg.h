#pragma once
#include "afxwin.h"

class KProject;
class KIOTable;
class KIOColumn;
class KExistNameChecker;
class KTarget;
// KTableZoneEditFieldDlg 대화 상자입니다.

class KTableZoneEditFieldDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTableZoneEditFieldDlg)

public:
	KTableZoneEditFieldDlg(KIOTable* a_pIOTable, KIOColumn* a_pIOColumnEdit, KTarget* a_pTarget, KProject* a_pProject, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableZoneEditFieldDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4154_EditZoneFieldDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	bool         InvalidateInput();
	void         InitComboPurpose();
	void         InitRadioPAFlag(CString &a_strInitPAFlag);

private:
	KIOTable*    m_pIOTable;
	KIOColumn*   m_pIOColumnEdit;
	KExistNameChecker* m_pNameChecker;
	KProject*    m_pProject;
	KTarget*     m_pTarget;

private:
	int          m_nInitObjectID;
	CString      m_strInitPAFlag;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* a_pMsg);

	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboGroupCode();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
private:
	CComboBox    m_cboPurpose;
};
