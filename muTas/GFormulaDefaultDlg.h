#pragma once

//^#include "ResizeDialogEx.h"
#include "afxwin.h"
#include "model_common.h"

class KTarget;

// KGFormulaDefaultDlg 대화 상자입니다.

class KGFormulaDefaultDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KGFormulaDefaultDlg)

public:
	KGFormulaDefaultDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGFormulaDefaultDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6170_GFormulaDefaultDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void         InitializeData();
protected:
	void         InitReportHeader();
	void         ResizeComponent();
	void         LoadFormulaMasterData();
	void         LoadFormula();
	void         LoadDetailObject();
	void         UpdateReportData(std::map<int, std::map<int, std::vector<CString>>> &a_mapFormula);

protected:
	KTarget*     m_pTarget;
	std::map<int, CString> m_mapObjectName;
	KEMPurposeGroup        m_emPurposeGroup;
	CRect        m_rectParent;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboGroup();
	afx_msg void OnBnClickedCancel();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CComboBox           m_cboMasterGroup;
};

typedef std::shared_ptr<KGFormulaDefaultDlg> KGFormulaDefaultDlgPtr;