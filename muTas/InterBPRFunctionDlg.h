#pragma once

#include "resource.h"

/* Forward declarations */
class KTarget;
class KCodeGroup;
class KInterFunctionBPR;
class KIOColumn;

// KInterSetFunctionVDFBPRDlg 대화 상자입니다.

class KInterBPRFunctionDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KInterBPRFunctionDlg)

public:
	KInterBPRFunctionDlg(KTarget* a_pTarget, KCodeGroup* a_pCodeData, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterBPRFunctionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6930_InterBPRFunctionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void SetFunction(KInterFunctionBPR* pFunction);
	KInterFunctionBPR* GetFunction(void);

protected:
	ZNumberEdit m_edtAlpha;
	ZNumberEdit m_edtBeta;
	ZNumberEdit m_edtTheta;
	CComboBox m_cmbVariable;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	KTarget* m_pTarget;
	KCodeGroup* m_pCodeData;
	KInterFunctionBPR* m_pFunction;
	std::list<KIOColumn*> m_ColumnList;

	void initColumnList(void);

	void updateControlState(void);

	void initReportControl(void);

	void updateReportRecord(void);
	
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnReportValueChanged(NMHDR*  pNotifyStruct, LRESULT* result);
	afx_msg void OnBnClickedCancel();
};
