#pragma once
//^#include "ResizeDialogEx.h"

// KPointStyleValueEdit 대화 상자입니다.

class KPointStyleValueEdit : public KDialogEx
{
	DECLARE_DYNAMIC(KPointStyleValueEdit)

public:
	KPointStyleValueEdit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPointStyleValueEdit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_STYLE_POINT_VALUEEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL   OnInitDialog();	
private:
	CFont           m_oQbicDefaultMarkerFont;
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	CString m_strPreValue;
	CString m_strPreCaption;
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportItemValueChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
	void RecalRange();
protected:
	CComboBox m_cboStyle;
	afx_msg void OnCbnSelchangeComboStyle();
private:
	int     m_nPreSelectedStyleID;
	void UpdateUIState();
	void LoadInitialData();
	void LoadInitialStyle();
	void StyleSelectChangeAction();
protected:
	afx_msg void OnEnSetfocusEditStyleName();
	afx_msg void OnCbnSetfocusComboStyle();
	afx_msg void OnBnClickedButtonContentsAdd();
	afx_msg void OnBnClickedButtonContentsDelete();

	void Apply();
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonStyleDelete();

	void PreStyle(int& ar_nStyle, COLORREF& ar_clrColor, double& ar_dSize, double& ar_dMin, double& ar_dMax);

protected:
	int GetDisplayDecimalPoint(int a_nRealDigit);
	int m_nRealDigit;
	afx_msg void OnBnClickedRadioEditMethod(UINT nID);
};
