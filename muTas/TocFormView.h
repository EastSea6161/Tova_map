#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MapLayerTreeCtrl.h"
#include "TocNodeProperties.h"
#include "TocLinkProperties.h"
#include "TocZoneProperties.h"
#include "TocEditorNode.h"
#include "TocEditorLink.h"
#include "TocEditorTurn.h"
#include "TocEditorTransit.h"
#include "TocEditorDemarcation.h"
#include "ITocForm.h"

class KMapView;

enum TTocTabFlag
{
	KEMSymbol  = 0,
	KEMNetworkEdit
};

class KTocFormView : public KDialogEx, public ITocParent
{
	DECLARE_DYNAMIC(KTocFormView)

public:
	KTocFormView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocFormView();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:	
	virtual BOOL   OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void   OnSize(UINT nType, int cx, int cy);
    afx_msg void   OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	int   m_nCurHeight;
	int   m_nScrollPos;	
	int   m_nFixScrollHeight;

private:
	KMapView*  m_pMapView;		
public:
	void    SetBuddyMapView(KMapView* a_pMapView);

private:
	CString GetCurrentSelectedLayerName();

private:
	KDialogEx*	          m_pCurDlg;
	KTocNodeProperties    m_dlgTocNodeProperties;
	KTocLinkProperties    m_dlgTocLinkProperties;
	KTocZoneProperties    m_dlgTocZoneProperties;	

	KTocEditorNode        m_dlgTocNodeEditor;
	KTocEditorLink        m_dlgTocLinkEditor;
	KTocEditorTurn        m_dlgTocTurnEditor;
	KTocEditorTransit     m_dlgTocTransitEditor;
	KTocEditorDemarcation m_dlgTocDemarcationEditor;

protected:
	KMapLayerTreeCtrl m_treeCtrlLayer;	

	afx_msg void OnRclickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblCllkTree1( NMHDR* pNMHDR, LRESULT *pResult );
	afx_msg void OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	
	LRESULT      OnTreeItemStateChangedNotifyRecv(WPARAM wParam, LPARAM lParam);
	
	HTREEITEM GetLayerItem(CString a_strLayerName);
public:
	void    MapChangedLayerState();
    void    MapEditCommand(int a_nType);

protected:	
	CTabCtrl     m_tabCtrl;
	TTocTabFlag  m_tabFlag;
    CStatic      m_tabBox;
private:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    virtual void TemplateChanged(LPCTSTR a_strTemplateName);
	void DisplayTabControl(int a_nTabIndex);	

protected:	
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();    
private:
    afx_msg void OnBnClickedButtonHide();

private:
    afx_msg void OnSymbologyThemeNode();
    afx_msg void OnClearThemeNode();
private:
    afx_msg void OnSymbologyThemeLink();
    afx_msg void OnClearThemeLink();
private:
    afx_msg void OnSymbologyThemeZone();
    afx_msg void OnClearThemeZone();
};
