#pragma once

#include "MarkupStatic.h"
#include "afxwin.h"

class KTarget;


// KImportTableDlg 대화 상자입니다.

class KImportTableDlg : public KWhiteDlgEx
{
	DECLARE_DYNAMIC(KImportTableDlg)

public:
	KImportTableDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KImportTableDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4500_ImportTableDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget* m_pTarget;
private:
    int m_nInitFlag;
public:
	void SetInitValue(KTarget* a_pTarget, int nInitFlag);

protected:

	 void KRedrawWindow();

	 void InitButtonControl();

	 bool ImportNomalTable(CString a_strTableName);

	 void SetSEDDataInCombo();

	 void SetODDataInCombo();
	 
	 bool ExportTable(CString a_strTableName);
	 
	 void UpdateBtnImportState(void);
	 void UpdateBtnNode(void);
	 void UpdateBtnLink(BOOL a_bEnable);
	 void UpdateBtnTrasitInfo(BOOL a_bEnable);
	 void UpdateBtnZone(BOOL a_bEnable);

	 void UpdateBtnExportState(void);

protected:

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedNode();
	afx_msg void OnBnClickedZone();
	afx_msg void OnBnClickedLink();
	afx_msg void OnBnClickedTurn();
	afx_msg void OnBnClickedTransitinfo();
	afx_msg void OnBnClickedTransitline();
	afx_msg void OnBnClickedSed();
	afx_msg void OnBnClickedOd();
	afx_msg void OnBnClickedIntersection();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedViewlog();
	afx_msg void OnBnClickedSchedule();
	afx_msg void OnBnClickedAccessStopNode();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	//Arrow 
	ZArrowStatic m_uiArrow1;    
	ZArrowStatic m_uiArrow2;
	//btn
	CXTPButton m_btnNode;
	CXTPButton m_btnZone;
	CXTPButton m_btnSED;
	CXTPButton m_btnOD;
	CXTPButton m_btnLink;
	CXTPButton m_btnTurn;
	CXTPButton m_btnTransitInfo;
	CXTPButton m_btnTransitLine;
	CXTPButton m_btnSchedule;

	//combobox
	CComboBox m_cboSED;
	CComboBox m_cboOD;

	bool m_bIsImport;
public:
    afx_msg void OnBnClickedButtonDemarc();
	afx_msg void OnBnClickedRadio(UINT nID);
};
