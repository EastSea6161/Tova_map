#pragma once
#include "ModeChoiceModelInfo.h"
#include "afxwin.h"

class KTarget;
class KCodeGroup;
// KMDefVarSubPage 대화 상자입니다.

class KMDefVarSubPage : public KDialogEx
{
	DECLARE_DYNAMIC(KMDefVarSubPage)

public:
	KMDefVarSubPage(int a_nPageID, int a_nPurposeID, CString a_strPurposeName, TDefVarDefaultODGroup &a_oTDefVarDefaultODGroup, KTarget* a_pTarget, std::map<int, CString> &a_mapFunctionInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMDefVarSubPage();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_6521_MDefVarSubPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	int          m_nPageID;
	int          m_nPurposeID;
	TDefVarDefaultODGroup m_oTDefVarDefaultODGroup;
	KTarget*     m_pTarget;
	std::map<int, CString> &m_mapFunctionInfo;
	int          m_nDefaultFunctionID;
	std::map<KCodeGroup*, std::map<int, int>> m_mapODGroupFunctionID;
	CString      m_strPurposeName;

	bool         m_bUseODGroup;
	int          m_nNoneGroupFunctionID;

public:
	int GetPageID()
	{
		return m_nPageID;
	}

	int GetPurposeID()
	{
		return m_nPurposeID;
	}

	bool IsUsedODGroup()
	{
		return m_bUseODGroup;
	}

	void         ExternalInitialize(TDefVarDefaultODGroup &a_oTDefVarDefaultODGroup);

	void         UpdateFunctionInfo();
	bool         InvalidateInputInfo();
	void         UsedFunctionIDSet( std::set<int> &a_setUsedFunctionID );

	void         GetResultInfo(TDefVarDefaultODGroup &a_oTDefVarGroupResultInfo);

protected:
	void         SetDefaultFunctionID();
	void         InitComboFunctionNoneGroup();
	void         InitReportHeader(void);

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_ctrlReport;
	CComboBox    m_cboODGroup;
	CComboBox    m_cboFunctionNoneGroup;

private:
	virtual BOOL OnInitDialog();
	void         InitOpen();
	afx_msg void OnBnClickedCheckGroup();
	void         ControlCheckGroup();
	afx_msg void OnCbnSelchangeComboGroup();
	void         ControlComboGroup();
	afx_msg void OnCbnSelchangeComboFunction();
	void         ControlComboNoneGroupFunction();

	afx_msg void OnReportValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
};

typedef std::shared_ptr<KMDefVarSubPage> KMDefVarSubPagePtr;
