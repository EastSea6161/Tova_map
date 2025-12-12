#pragma once
#include "GroupGenerationInfo.h"

class KTarget;
class KCodeGroup;
class KIOColumn;

// KODGroupGenerationDlg 대화 상자입니다.

class KODGroupGenerationDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KODGroupGenerationDlg)

public:
	KODGroupGenerationDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KODGroupGenerationDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_2199_ODGROUP_GEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	KTarget*     m_pTarget;
	std::map<int, TCodeGoupColumn> m_mapCodeGoupColumn;
	std::map<Integer, int> m_mapResult;
	int          m_nRunResult;

protected:
	void         InitReportHeader();
	void         LoadZoneGroupGenInfo(std::map<CString, int> &a_mapZoneColumnGroupKey);
	void         LoadZoneGroupField();
	void         LoadReportData();
	void         LoadOutColumnName();
	void         ResizeComponent();
	
	bool         InputCheck();
	static unsigned __stdcall ThreadCaller(void* p); 
	void         Run();

private:
	std::map<KIntKey, TCodeName> m_mapKeyCodeName;
	void         KeyCodeName();

	int          AddCodeGroup();
	KIOColumn*   AddParaODCodeColumn( int a_nCodeGroup, CString a_strColumnName );
	void         UpdateParaODTable( KIOColumn* a_pColumn, std::map<Integer, int>& ar_mapValue );
	void         UpdateResultTable(KIOColumn* a_pColumn, int a_nODGroupCode);

private:
	CComboBox    m_cboGroupField;
	CComboBox    m_cboOutColumnName;

	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;  

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio(UINT nID);
			void ControlRadio();
	afx_msg void OnCbnSelchangeCboGroupField();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
