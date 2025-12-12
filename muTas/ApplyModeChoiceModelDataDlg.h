#pragma once
#include "afxwin.h"


class KTarget;
class KMode;
class KProject;
// KApplyModeChoiceModelDataDlg 대화 상자입니다.

class KApplyModeChoiceModelDataDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KApplyModeChoiceModelDataDlg)

public:
	KApplyModeChoiceModelDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KApplyModeChoiceModelDataDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4190_ApplyModelDataDlg };

	/**************************************
	* Overrides                           *
	**************************************/
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetTarget(KTarget* a_pTarget);
	void SetColumnName(CString a_strColumnName);
	void SetColumnDisplayName( CString a_strColumnDisplayName);
	void SetProject(KProject* a_pProject);

protected:
	void InitialModeInfo(void);
	bool UpdateModeChoiceModel(KMode* a_pMode, CString &a_strStartTime, CString &a_strEndTime);
	bool NewUpdateModeChoiceModel(KMode* a_pMode, CString &a_strStartTime, CString &a_strEndTime);
	bool ModeChoiceSelect(KMode* a_pMode);
	bool ModeChoiceSelectOnlyName(KMode* a_pMode);

protected:
	KTarget*  m_pTarget;
	CString   m_strColumnName;
	KProject* m_pProject;

protected:
	afx_msg void OnBnClickedOk();

protected:
	CString   m_strColumnDisplayName;
	CComboBox m_cboModeInfo;
};
