#pragma once

#include "afxwin.h"
#include "TThemeType.h"
//^#include "ResizeDialogEx.h"

class KTarget;
class KMapView;


// KThemeNodeCategory 대화 상자입니다.
class KThemeNodeCategory : public KDialogEx
{
	DECLARE_DYNAMIC(KThemeNodeCategory)

public:
	KThemeNodeCategory(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeNodeCategory();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_NODE_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	KTarget*         m_pTarget;
	KMapView*        m_pMapView;
public:
	void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView);

protected:
	virtual BOOL   OnInitDialog();	
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);	
private:
	CFont           m_oQbicDefaultMarkerFont;
protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
protected:
	CComboBox m_cboCategory;
	CComboBox m_cboStyle;

protected:
	afx_msg void OnCbnSelchangeComboField();
			void FieldSelectChangeAction();
	afx_msg void OnCbnSelchangeComboStyle();
			void StyleSelectChangeAction();
private:
	CString m_strPreSelectedColumnName;
	int     m_nPreSelectedStyleID;
private:
	void LoadInitialData();
	void LoadInitialCategory();
	void LoadInitialStyle();
protected:
	afx_msg void OnBnClickedButtonClear();
	        void ClearNodeThemes();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonStyleSave();
	afx_msg void OnBnClickedButtonStyleEdit();
};
