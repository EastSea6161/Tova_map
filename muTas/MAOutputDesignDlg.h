
#include "DBaseAssignment.h"
//^^ #include "IOColumns.h"
#pragma once

class KMultiModalInfo;
class KTarget;

// KMAOutputDesignDlg 대화 상자입니다.

class KMAOutputDesignDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KMAOutputDesignDlg)

public:
	KMAOutputDesignDlg(KTarget* a_pTarget, std::list<KMultiModalInfo> a_lstMultinfo, TAssignResultInfo& a_oMAssignResultInfo, int a_nModel, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMAOutputDesignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6937_MAOutputDesignDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


private:

	bool m_bUseBus;
	bool m_bUseSubway;
	bool m_bUseBusSubway;

	int m_nModeType;
	int m_nRunSeq;

	KTarget* m_pTarget;

	std::list<KMultiModalInfo>		m_lstMultiModalinfo;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	KIOColumns				m_oColumnCollection;
	TAssignResultInfo&				m_oMAssignResultInfo;
	std::vector<TAssignResultInfo>	m_vecMultiAssignInfoResult;

	CComboBox m_cboOverwriteInfo;

private:
	CString   m_strNodeGroupName;
	CString   m_strLinkGroupName;
	CString   m_strAttriGroupName;
	CString   m_strLineGroupName;
	CString   m_strTransitGroupName;
	CString   m_strODGroupName;

protected:

	virtual BOOL OnInitDialog();

	void InitReportColumn(void);

	void UpdateReportRecord(void);

	void GetInputModeColumn();

	void InitButtonInfo(void);


	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnRadioClicked(UINT nID);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
