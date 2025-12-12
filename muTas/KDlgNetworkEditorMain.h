#pragma once

#include "TocEditorNode.h"
#include "TocEditorLink.h"
#include "TocEditorTurn.h"
#include "TocEditorTransit.h"
#include "TocEditorDemarcation.h"

class KMapView;

class KDlgNetworkEditorMain : public KDialogEx
{
	DECLARE_DYNAMIC(KDlgNetworkEditorMain)

public:
	KDlgNetworkEditorMain(KMapView* pMapView, CWnd* pParent = NULL);  
	virtual ~KDlgNetworkEditorMain();
	enum { IDD = IDD_EDITOR_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	DECLARE_MESSAGE_MAP()

protected:	
    virtual BOOL OnInitDialog();

private:
    KMapView*  m_pMapView;		
public:
    void SetMapView(KMapView* a_pMapView);
private:
    KDialogEx*	          m_pCurDlg;

    KTocEditorNode        m_dlgTocNodeEditor;
    KTocEditorLink        m_dlgTocLinkEditor;
    KTocEditorTurn        m_dlgTocTurnEditor;
    KTocEditorTransit     m_dlgTocTransitEditor;
    KTocEditorDemarcation m_dlgTocDemarcationEditor;

protected:	
    CTabCtrl     m_tabCtrl;
    CStatic      m_tabBox;
private:
    afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    virtual void TemplateChanged(LPCTSTR a_strTemplateName);
    void DisplayTabControl(int a_nTabIndex);	

public:
    void MapEditCommand(int a_nType);
public:
    void NotifyNetworkEdior(int nType, __int64 nxID);
    void NotifyNetworkEdior(int nType, std::vector<__int64> vecID);
private:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedCancel();
};

typedef std::shared_ptr<KDlgNetworkEditorMain> KDlgNetworkEditorMainPtr;