#pragma once

class KTarget;
class KIOColumn;

#include "IOColumnCollection.h"
#include "DBaseAssignment.h"
#include "model_common.h"


// KInterOutputDesingDlg 대화 상자입니다.

class KInterOutputDesignDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KInterOutputDesignDlg)

public:
	KInterOutputDesignDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterOutputDesignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6981_InterOutPutDesignDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()

public:

	void SetTarget(KTarget* pTarget);

	void SetOutputInfo(bool a_bUseNew, TAssignResultInfo a_oTARsultInfo);

	void GetOutputInfo(bool& a_bUseNew, TAssignResultInfo& a_oTARsultInfo);


protected:

	CXTPReportControl	m_wndReportCtrl;
	int					m_nRadioOutputType;
	CString				m_strDescription;
	CComboBox			m_cboOverwriteInfo;

	bool				m_bUseOverWirte;
	TAssignResultInfo	m_oAssign;

private:

	int m_nRunSeq;
	KTarget* m_pTarget;
	std::vector<TAssignResultInfo>     m_oVecAssignResultInfo;


protected:

	void InitReportColumn(void);

	void InitReportRecord(void);

	void InitPutInfo(void);

	void ChangeComboBox(void);

	void InitOutputInfo(void);

	void GetOuputDesignInfo(void);

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioNew();
	afx_msg void OnBnClickedRadioOverwrite();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo1();

};
