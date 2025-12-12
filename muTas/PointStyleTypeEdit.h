#pragma once
//^#include "ResizeDialogEx.h"

// KPointStyleTypeEdit 대화 상자입니다.

class KPointStyleTypeEdit : public KDialogEx
{
	DECLARE_DYNAMIC(KPointStyleTypeEdit)

public:
	KPointStyleTypeEdit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPointStyleTypeEdit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_STYLE_POINT_TYPEEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL   OnInitDialog();	
private:
	CFont            m_oQbicDefaultMarkerFont;
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
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
};
