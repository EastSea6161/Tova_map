#include "stdafx.h"
#include "KDlgNetworkEditorMain.h"
#include "afxdialogex.h"

#include <limits>
#include "DBaseMap.h"
#include "ImChampFrameWindow.h"
#include "MapView.h"

#include "KDlgNetworkStartID.h"

IMPLEMENT_DYNAMIC(KDlgNetworkEditorMain, KDialogEx)
    BEGIN_MESSAGE_MAP(KDlgNetworkEditorMain, KDialogEx)
        ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CTRL, &KDlgNetworkEditorMain::OnTcnSelchangeTab)
        ON_BN_CLICKED(IDC_BUTTON1, &KDlgNetworkEditorMain::OnBnClickedButton1)
        ON_BN_CLICKED(IDCANCEL, &KDlgNetworkEditorMain::OnBnClickedCancel)
    END_MESSAGE_MAP()

KDlgNetworkEditorMain::KDlgNetworkEditorMain(KMapView* pMapView, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDlgNetworkEditorMain::IDD, pParent), m_pMapView(pMapView)
{
    m_pCurDlg = nullptr;
}

KDlgNetworkEditorMain::~KDlgNetworkEditorMain()
{
}

void KDlgNetworkEditorMain::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_CTRL,  m_tabCtrl);
    DDX_Control(pDX, IDC_STATIC_TAB_BOX, m_tabBox);
}


BOOL KDlgNetworkEditorMain::OnInitDialog()
{
    KDialogEx::OnInitDialog();
    KDialogEx::UseKeyEscEnter(false, false);

    HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
    SetIcon( hIcon, TRUE );
    SetIcon( hIcon, FALSE );
    
    CRect oTabRect;	{
        m_tabBox.GetClientRect(&oTabRect);	
    }

	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_tabCtrl.InsertItem(0, _T("노드"));
		m_tabCtrl.InsertItem(1, _T("링크"));
		m_tabCtrl.InsertItem(2, _T("존 경계"));
		m_tabCtrl.InsertItem(3, _T("회전제약"));
		m_tabCtrl.InsertItem(4, _T("노선"));
	}
	else
	{
		m_tabCtrl.InsertItem(0, _T("Node"));
		m_tabCtrl.InsertItem(1, _T("Link"));
		m_tabCtrl.InsertItem(2, _T("Zone"));
		m_tabCtrl.InsertItem(3, _T("Turn"));
		m_tabCtrl.InsertItem(4, _T("Transit Line"));
	}

    m_tabCtrl.SetCurSel(1);
        
    m_dlgTocNodeEditor.Create(IDD_TOC_EDITOR_NODE, &m_tabBox);
    m_dlgTocNodeEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

    m_dlgTocLinkEditor.Create(IDD_TOC_EDITOR_LINK, &m_tabBox);
    m_dlgTocLinkEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

    m_dlgTocDemarcationEditor.Create(IDD_TOC_EDITOR_DEMARCATION, &m_tabBox);
    m_dlgTocDemarcationEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

    m_dlgTocTurnEditor.Create(IDD_TOC_EDITOR_TURN, &m_tabBox);
    m_dlgTocTurnEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);
        
    m_dlgTocTransitEditor.Create(IDD_TOC_EDITOR_TRANSIT, &m_tabBox);
    m_dlgTocTransitEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);
    
    SetMapView(m_pMapView);
    DisplayTabControl(m_tabCtrl.GetCurSel());	

    return TRUE;  
}

void KDlgNetworkEditorMain::SetMapView( KMapView* a_pMapView )
{
    m_pMapView = a_pMapView;

    try
    {
        m_dlgTocNodeEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);
        m_dlgTocLinkEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);
        m_dlgTocTurnEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);
        m_dlgTocDemarcationEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);

        m_dlgTocTransitEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);     
        
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }	
}

void KDlgNetworkEditorMain::OnTcnSelchangeTab( NMHDR *pNMHDR, LRESULT *pResult )
{
    DisplayTabControl(m_tabCtrl.GetCurSel());
    *pResult = 0;
}

void KDlgNetworkEditorMain::TemplateChanged( LPCTSTR a_strTemplateName )
{

}

void KDlgNetworkEditorMain::DisplayTabControl( int a_nTabIndex )
{
    if(m_pCurDlg != NULL)
    {
        m_pCurDlg->ActionFeedback(KDialogFeedback::KEMActionFeedbackDeActivate);
        m_pCurDlg->ShowWindow(SW_HIDE);
        m_pCurDlg = NULL;
    }

    int nIndex    = a_nTabIndex; /*m_tabCtrl.GetCurSel();*/
    int nTabCount = m_tabCtrl.GetItemCount();
    if (nIndex > nTabCount-1) {
        nIndex = 1;
    }

    m_tabCtrl.SetCurSel(nIndex);

    switch(nIndex)
    {
    case 0:
        m_dlgTocNodeEditor.ShowWindow(SW_SHOW);
        m_pCurDlg = &m_dlgTocNodeEditor;
        break;
    case 1:
        m_dlgTocLinkEditor.ShowWindow(SW_SHOW);
        m_pCurDlg = &m_dlgTocLinkEditor;
        break;
    case 2:
        m_dlgTocDemarcationEditor.ShowWindow(SW_SHOW);
        m_pCurDlg = &m_dlgTocDemarcationEditor;		
        break;
    case 3:
        m_dlgTocTurnEditor.ShowWindow(SW_SHOW);
        m_pCurDlg = &m_dlgTocTurnEditor;			
        break;
    case 4:		
    default:
        m_dlgTocTransitEditor.ShowWindow(SW_SHOW);
        m_pCurDlg = &m_dlgTocTransitEditor;		
        break;
    }

    if(m_pCurDlg != NULL) {
        m_pCurDlg->ActionFeedback(KDialogFeedback::KEMActionFeedbackActivate);
    }    
}

void KDlgNetworkEditorMain::MapEditCommand( int a_nType )
{
    DisplayTabControl(a_nType);
}

void KDlgNetworkEditorMain::NotifyNetworkEdior( int nType, __int64 nxID )
{
    //★ 0 : 노드(추가)
    if (nType == 0) {
        m_dlgTocNodeEditor.NotifyNodeAdded(nxID);
    }
    else if (nType == 1 || nType == 2) {
        m_dlgTocLinkEditor.NotifyNodeEdit(nType, nxID);
    }
    else if (nType == 5) {
        m_dlgTocTransitEditor.NotifyRouteAdd(nxID);
    }
}

void KDlgNetworkEditorMain::NotifyNetworkEdior( int nType, std::vector<__int64> vecID )
{
    if (nType == 1) {
        m_dlgTocLinkEditor.NotifyLinkEdit(vecID);
    }

    if (nType == 3) {
        //★ 회전정보(turn_id, f-link, at-node, t-link)
        m_dlgTocTurnEditor.NotifyTurnEdit(vecID);
    }
}


void KDlgNetworkEditorMain::OnBnClickedButton1()
{
    KDlgNetworkStartID oDlg(m_pMapView);
    oDlg.DoModal();
}


void KDlgNetworkEditorMain::OnBnClickedCancel()
{
    try
    {
        m_pMapView->StopShapeEdit();
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
        TxLogDebugException();
    }    

    KDialogEx::OnCancel();
}
