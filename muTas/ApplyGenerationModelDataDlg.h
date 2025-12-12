#pragma once
#include "afxwin.h"


class KTarget;
class KProject;
class KPurpose;
// KApplyGenerationModelDataDlg 대화 상자입니다.

class KApplyGenerationModelDataDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KApplyGenerationModelDataDlg)

public:
	KApplyGenerationModelDataDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KApplyGenerationModelDataDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4192_ApplyGenerationModelDatadlg };

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
	bool UpdateGenerationModel(KPurpose* a_pPurpose, CString &a_strPAFlag, CString &a_strStartTime, CString &a_strEndTime);
	bool NewUpdateGenerationModel(KPurpose* a_pPurpose, CString &a_strPAFlag, CString &a_strStartTime, CString &a_strEndTime);
	bool GenerationSelect(KPurpose* a_pPurpose,  CString &a_strPAFlag);
	bool GenerationSelectOnlyName(KPurpose* a_pPurpose,  CString &a_strPAFlag);

protected:
	KTarget*  m_pTarget;
	CString   m_strColumnName;
	KProject* m_pProject;

public:
	afx_msg void OnBnClickedOk();
	
protected:
	CString   m_strColumnDisplayName;
	CComboBox m_cboPurposeInfo;
	int       m_nPAFlag;
};
