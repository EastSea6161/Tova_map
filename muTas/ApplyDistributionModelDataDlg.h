#pragma once
#include "afxwin.h"


class KTarget;
class KProject;
class KPurpose;
// KApplyDistributionModelDataDlg 대화 상자입니다.

class KApplyDistributionModelDataDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KApplyDistributionModelDataDlg)

public:
	KApplyDistributionModelDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KApplyDistributionModelDataDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4191_ApplyDistributionModelDataDlg };

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
	void InitialPurposeInfo(void);
	bool UpdateDistributionModel(KPurpose* a_pPurpose, CString &a_strStartTime, CString &a_strEndTime);
	bool NewUpdateDistributionModel(KPurpose* a_pPurpose, CString &a_strStartTime, CString &a_strEndTime);
	bool DistributionSelect(KPurpose* a_pPurpose);
	bool DistributionSelectOnlyName(KPurpose* a_pPurpose);

protected:
	KTarget*  m_pTarget;
	CString   m_strColumnName;
	KProject* m_pProject;

protected:
	afx_msg void OnBnClickedOk();

protected:
	CString   m_strColumnDisplayName;
	CComboBox m_cboPurposeInfo;

};
