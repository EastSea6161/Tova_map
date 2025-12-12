#pragma once

//^#include "ResizeDialogEx.h"
#include "model_common.h"
#include "ImTasDBase.h"

class KTarget;

// KDFormulaDefaultDlg 대화 상자입니다.

class KDFormulaDefaultDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDFormulaDefaultDlg)

public:
	KDFormulaDefaultDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDFormulaDefaultDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6370_DFormulaDefaultDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void         InitReportHeader();
	void         ResizeComponent();
	void         LoadDetailObject();
	void         LoadFormula();
	void         LoadFormulaMasterData();
	void         UpdateReportData(std::map<int, std::vector<TImpedanceFunction>> &a_mapImpedanceFunction);

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

typedef std::shared_ptr<KDFormulaDefaultDlg> KDFormulaDefaultDlgPtr;