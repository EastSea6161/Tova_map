#pragma once
#include "afxwin.h"

class KIOTable;
class KIOColumn;
class KExistNameChecker;
class KTarget;

// KTableEditCodeFieldDlg 대화 상자입니다.

class KTableEditCodeFieldDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTableEditCodeFieldDlg)

public:
	KTableEditCodeFieldDlg(KIOTable* a_pIOTable, KIOColumn* a_pIOColumnEdit, KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableEditCodeFieldDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4157_EditCodeFieldDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	bool         InvalidateInput();
	void         DefaultCodeFiledData(KIOColumn &a_oIOColumnUpdate);
	void         InitGroupCode(std::map<int, CString> &a_mapGroupCode);
	void         InitComboCode();

private:
	KIOTable*    m_pIOTable;
	KIOColumn*   m_pIOColumnEdit;
	KExistNameChecker*     m_pNameChecker;
	
	std::map<int, CString> m_mapGroupCode;
	KTarget*     m_pTarget;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* a_pMsg);

	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboGroupCode();
			void EnableCheckCtrl();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
private:
	CComboBox    m_cboCodeGroup;
};
