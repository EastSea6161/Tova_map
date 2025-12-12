#pragma once

#include "MarkupStatic.h"
#include "afxwin.h"

class KTarget;

// KFImportTableDlg 대화 상자입니다.

class KFImportTableDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KFImportTableDlg)

public:
	KFImportTableDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KFImportTableDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4501_FImportTableDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;

public:
	void SetTarget(KTarget* a_pTarget);


protected:
	void KRedrawWindow();

	void SetSEDDataInCombo();

	void SetODDataInCombo();

	bool ImportNomalTable(CString a_strTableName);

	bool ExportTable(CString a_strTableName);
private:
	
	int m_nIEMode;
	// UI
	CMarkupStatic m_uiBackImage;

	//Arrow 
	ZArrowStatic m_uiArrow1;    
	ZArrowStatic m_uiArrow2;
	ZArrowStatic m_uiArrow3;
	ZArrowStatic m_uiArrow4;

public:

	virtual BOOL OnInitDialog();
	CComboBox m_cboSED;
	CComboBox m_cboOD;
	afx_msg void OnBnClickedNode();
	afx_msg void OnBnClickedZone();
	afx_msg void OnBnClickedLink();
	afx_msg void OnBnClickedOd();
	afx_msg void OnBnClickedSed();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedViewlog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedDemar();
	afx_msg void OnBnClickedRadioModeChoiceStatus(UINT nID);
			void UpdateDisplayView();

};
