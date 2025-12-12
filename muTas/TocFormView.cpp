// TocFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocFormView.h"
#include "afxdialogex.h"
#include "KmzSystem.h"

#include <limits>
#include "DBaseMap.h"
#include "ImChampFrameWindow.h"
#include "MapView.h"

IMPLEMENT_DYNAMIC(KTocFormView, KDialogEx)

KTocFormView::KTocFormView(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTocFormView::IDD, pParent)
{
	m_pMapView         = NULL;
	m_pCurDlg          = NULL;
	m_tabFlag          = KEMSymbol;
	m_nScrollPos       = 0;
	m_nFixScrollHeight = 700/*지도:700*/;
}

KTocFormView::~KTocFormView()
{
	m_pMapView = NULL;
	m_pCurDlg  = NULL;
}

void KTocFormView::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_CTRL, m_treeCtrlLayer);
    DDX_Control(pDX, IDC_TAB_CTRL,  m_tabCtrl);
    DDX_Control(pDX, IDC_STATIC_TAB_BOX, m_tabBox);
}


BEGIN_MESSAGE_MAP(KTocFormView, KDialogEx)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_CTRL,   &KTocFormView::OnRclickTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_CTRL,   &KTocFormView::OnNMDblCllkTree1)
	ON_MESSAGE(WM_CHANGE_LAYER_STATE,     &KTocFormView::OnTreeItemStateChangedNotifyRecv)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CTRL, &KTocFormView::OnSelchangedTree1)

	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CTRL, &KTocFormView::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDC_RADIO1, &KTocFormView::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &KTocFormView::OnBnClickedRadio2)
    ON_BN_CLICKED(IDC_BUTTON_HIDE, &KTocFormView::OnBnClickedButtonHide)

    ON_COMMAND(ID_SYMBOLOGY_TOC_THEME_NODE, &KTocFormView::OnSymbologyThemeNode)
    ON_COMMAND(ID_CLEAR_TOC_THEME_NODE,     &KTocFormView::OnClearThemeNode)

    ON_COMMAND(ID_SYMBOLOGY_TOC_THEME_LINK, &KTocFormView::OnSymbologyThemeLink)
    ON_COMMAND(ID_CLEAR_TOC_THEME_LINK,     &KTocFormView::OnClearThemeLink)

    ON_COMMAND(ID_SYMBOLOGY_TOC_THEME_ZONE, &KTocFormView::OnSymbologyThemeZone)
    ON_COMMAND(ID_CLEAR_TOC_THEME_ZONE,     &KTocFormView::OnClearThemeZone)
END_MESSAGE_MAP()

BOOL KTocFormView::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(false, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.	
	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddImChampTooltip(IDC_TREE_CTRL, _T("지도 보기(On/Off) 설정"));
		AddImChampTooltip(IDC_BUTTON_HIDE, _T("숨기기"));
	}
	else
	{
		AddImChampTooltip(IDC_TREE_CTRL, _T("On/Off Setting"));
		AddImChampTooltip(IDC_BUTTON_HIDE, _T("Hide"));
	}
    AddImChampTooltip(IDC_RADIO1,    _T("지도 보기 설정"));
    AddImChampTooltip(IDC_RADIO2,    _T("네트워크 편집"));
    
	SetBackgroundColor(RGB(255,255,255));
	m_treeCtrlLayer.SetBkColor(RGB(255,255,255));

	CFont& oFont = XTPAuxData().font;	
    GetDlgItem(IDC_STATIC_INFO)->SetFont(&oFont);
    GetDlgItem(IDC_BUTTON_HIDE)->SetFont(&oFont);
	GetDlgItem(IDC_TREE_CTRL)->SetFont(&oFont);
	GetDlgItem(IDC_TAB_CTRL)->SetFont(&oFont);
	GetDlgItem(IDC_RADIO1)->SetFont(&oFont);
	GetDlgItem(IDC_RADIO2)->SetFont(&oFont);	

	m_nScrollPos = 0;

	CRect oTabRect;	
	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_tabCtrl.InsertItem(0, _T("노드"));
		m_tabCtrl.InsertItem(1, _T("링크"));
		m_tabCtrl.InsertItem(2, _T("존 경계"));
	}
	else
	{
		m_tabCtrl.InsertItem(0, _T("Node"));
		m_tabCtrl.InsertItem(1, _T("Link"));
		m_tabCtrl.InsertItem(2, _T("Zone"));
	}
	
	m_tabCtrl.SetCurSel(1);

    m_tabBox.GetClientRect(&oTabRect);	
    m_dlgTocNodeProperties.Create(IDD_TOC_NODE_PROPERTIES, &m_tabBox);
    m_dlgTocNodeProperties.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

    m_dlgTocLinkProperties.Create(IDD_TOC_LINK_PROPERTIES, &m_tabBox);
    m_dlgTocLinkProperties.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

    m_dlgTocZoneProperties.Create(IDD_TOC_ZONE_PROPERTIES, &m_tabBox);
    m_dlgTocZoneProperties.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

    m_dlgTocNodeEditor.Create(IDD_TOC_EDITOR_NODE, &m_tabBox);
    m_dlgTocNodeEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

    m_dlgTocLinkEditor.Create(IDD_TOC_EDITOR_LINK, &m_tabBox);
    m_dlgTocLinkEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

	m_dlgTocTurnEditor.Create(IDD_TOC_EDITOR_TURN, &m_tabBox);
	m_dlgTocTurnEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

	m_dlgTocDemarcationEditor.Create(IDD_TOC_EDITOR_DEMARCATION, &m_tabBox);
	m_dlgTocDemarcationEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);

    if (KmzSystem::IsPassengerSystem()) {
        m_dlgTocTransitEditor.Create(IDD_TOC_EDITOR_TRANSIT, &m_tabBox);
        m_dlgTocTransitEditor.SetWindowPos(NULL, 0, 0, oTabRect.Width()-0, oTabRect.Height(), SWP_NOZORDER);
    }

	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(BST_CHECKED);

	DisplayTabControl(m_tabCtrl.GetCurSel());	
		
	return TRUE;  
}


HBRUSH KTocFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void KTocFormView::OnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	*pResult = 0;
}

LRESULT KTocFormView::OnTreeItemStateChangedNotifyRecv(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hItem = (HTREEITEM)lParam;    
	if (hItem == NULL)
		return 1;

    if (m_pMapView == nullptr)
        return 1;
    
	int nLayerState = m_treeCtrlLayer.GetItemCheck(hItem);	
    int nLayerID    = m_treeCtrlLayer.GetItemData(hItem);

    ITxLayerPtr spLayer = m_pMapView->MapGetLayer(nLayerID);
    if (spLayer == nullptr)
        return 1;

    switch (nLayerState)
    {
    case 1:
        spLayer->LayerOn(false);
        break;
    case 2:
        spLayer->LayerOn(true);
        break;
    default:
        spLayer->LayerOn(true);
        break;
    }		

    m_pMapView->MapRefresh();
    return 1;
}

void KTocFormView::MapChangedLayerState()
{
    if (m_pMapView == nullptr)
        return;

    HTREEITEM hItem = nullptr;
    double dScale = m_pMapView->MapGetScale();

    CString strCaption(_T(""));
    hItem = GetLayerItem(TABLE_NODE);
    if (hItem != nullptr) {        
        ITxLayerPtr spLayer = m_pMapView->MapGetLayer(KLayerID::Node());
        strCaption.Format(_T("%s"), TABLE_NODE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
			strCaption = _T("노드");
		}

        if (spLayer != nullptr) 
        {
            ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
            
            bool bOn = spLayer->LayerOn();
            if (bOn == true) {
                if (spLayer->IsScaleDispMode() == true) 
                {                    
                    if (spLayer->IsDrawScale(dScale) == true) {
                        m_treeCtrlLayer.SetItemCheck(hItem, 2);                           
                    } else {
                        m_treeCtrlLayer.SetItemCheck(hItem, 3);   
                        strCaption.Format(_T("%s (Invisible)"), TABLE_NODE);
						if (KmzSystem::GetLanguage() == KEMKorea) {
							//strCaption.Format(_T("%s (보이지 않음)"), "노드");
							strCaption = _T("노드 (보이지 않음)");
						}
                    }                    
                } else 
                {
                    m_treeCtrlLayer.SetItemCheck(hItem, 2); 
                }
            } else {
                m_treeCtrlLayer.SetItemCheck(hItem, 1); 
            }

            m_treeCtrlLayer.SetItemText(hItem, strCaption);
        }
    }
    
    hItem = GetLayerItem(TABLE_LINK);
    if (hItem != nullptr) {        
        ITxLayerPtr spLayer = m_pMapView->MapGetLayer(KLayerID::Link());
        strCaption.Format(_T("%s"), TABLE_LINK);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strCaption = _T("링크");
		}

        if (spLayer != nullptr) 
        {
            ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);

            bool bOn = spLayer->LayerOn();
            if (bOn == true) {
                if (spLayer->IsScaleDispMode() == true) {
                    if (spLayer->IsDrawScale(dScale) == true) {
                        m_treeCtrlLayer.SetItemCheck(hItem, 2);                           
                    } else {
                        m_treeCtrlLayer.SetItemCheck(hItem, 3);   
                        strCaption.Format(_T("%s (Invisible)"), TABLE_LINK);
						if (KmzSystem::GetLanguage() == KEMKorea) {
							//strCaption.Format(_T("%s (보이지 않음)"), "링크");
							strCaption = _T("링크 (보이지 않음)");
						}
                    }                    
                } else {
                    m_treeCtrlLayer.SetItemCheck(hItem, 2); 
                }
            } else {
                m_treeCtrlLayer.SetItemCheck(hItem, 1); 
            }

            m_treeCtrlLayer.SetItemText(hItem, strCaption);
        }
    }

    hItem = GetLayerItem(TABLE_ZONE);
    if (hItem != nullptr) {        
        ITxLayerPtr spLayer = m_pMapView->MapGetLayer(KLayerID::Zone());
        strCaption.Format(_T("%s"), TABLE_ZONE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strCaption = _T("존 경계");
		}

        if (spLayer != nullptr) 
        {
            ITxFeatureLayerPtr spFLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);

            bool bOn = spLayer->LayerOn();
            if (bOn == true) {
                if (spLayer->IsScaleDispMode() == true) 
                {
                    if (spLayer->IsDrawScale(dScale) == true) {
                        m_treeCtrlLayer.SetItemCheck(hItem, 2);                           
                    } else {
                        m_treeCtrlLayer.SetItemCheck(hItem, 3);   
                        strCaption.Format(_T("%s (Invisible)"), TABLE_ZONE);
						if (KmzSystem::GetLanguage() == KEMKorea) {
							//strCaption.Format(_T("%s (보이지 않음)"), "존 경계");
							strCaption = _T("존 경계 (보이지 않음)");
						}
                    }                    
                } else 
                {
                    m_treeCtrlLayer.SetItemCheck(hItem, 2); 
                }
            } else {
                m_treeCtrlLayer.SetItemCheck(hItem, 1); 
            }

            m_treeCtrlLayer.SetItemText(hItem, strCaption);
        }
    }
}

HTREEITEM KTocFormView::GetLayerItem( CString a_strLayerName )
{
	HTREEITEM hRootItem  = m_treeCtrlLayer.GetRootItem();
	if (m_treeCtrlLayer.ItemHasChildren(hRootItem) == FALSE)
		return NULL;

	HTREEITEM hChildItem = m_treeCtrlLayer.GetChildItem(hRootItem);	

    int nLayerID(-1);
    if (_tcsicmp(a_strLayerName, TABLE_NODE) == 0) {
        nLayerID = KLayerID::Node();
    } else if (_tcsicmp(a_strLayerName, TABLE_LINK) == 0) {
        nLayerID = KLayerID::Link();
    } else if (_tcsicmp(a_strLayerName, TABLE_ZONE) == 0) {
        nLayerID = KLayerID::Zone();
    } else {
        return NULL;
    }

	while (hChildItem != NULL) { 
        int nItemData = m_treeCtrlLayer.GetItemData(hChildItem);
        if (nItemData == nLayerID) {
            return hChildItem;
        }

		hChildItem = m_treeCtrlLayer.GetNextItem(hChildItem, TVGN_NEXT);
	}

	return NULL;
}


void KTocFormView::OnRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
    if (m_pMapView == nullptr) {
        *pResult = 0;
        return;
    }

	CPoint ptInTree, ptInSrceen;
	GetCursorPos(&ptInSrceen); 
	ptInTree = ptInSrceen;
	m_treeCtrlLayer.ScreenToClient(&ptInTree);       

	HTREEITEM hHitItem = m_treeCtrlLayer.HitTest(ptInTree);     
	if( NULL == hHitItem )
	{
		*pResult = 0;
		return;
	}

	m_treeCtrlLayer.SelectItem(hHitItem);    

	// Popup 삭제
	CString strLayerName = GetCurrentSelectedLayerName();

	if( strLayerName.GetLength() == 0 )
	{
		*pResult = 0;
		return;
	}

	CMenu menuThemes;
	VERIFY(menuThemes.LoadMenu(IDR_POPUP_TOC_CONTEXT));

	CMenu* pPopupThemes = menuThemes.GetSubMenu(0);
	ASSERT(pPopupThemes != NULL);
	
    //CWnd* pWndPopupThemesOwner = this;
	//while (pWndPopupThemesOwner->GetStyle() & WS_CHILD)
	//	pWndPopupThemesOwner = pWndPopupThemesOwner->GetParent();

	if ( strLayerName.CompareNoCase(TABLE_NODE) == 0 )
	{
		pPopupThemes = menuThemes.GetSubMenu(5);
	}
	else if ( strLayerName.CompareNoCase(TABLE_LINK) == 0 )
	{
		pPopupThemes = menuThemes.GetSubMenu(6);
	}
	else if ( strLayerName.CompareNoCase(TABLE_ZONE) == 0 )
	{
		pPopupThemes = menuThemes.GetSubMenu(7);
	}
	else
	{
		*pResult = 0;
		return;
	}        

	pPopupThemes->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptInSrceen.x, ptInSrceen.y, this/*pWndPopupThemesOwner*/);

	*pResult = 0;
	return;

    // Backup-Button Menu	
    /*
    CRect    rectButton;
    CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON3);

    pButton->GetWindowRect(&rectButton);
    TPMPARAMS tpmParams;
    tpmParams.cbSize=sizeof(TPMPARAMS);
    tpmParams.rcExclude= rectButton;

    CMenu menu;
    menu.LoadMenu( IDR_POPUP_TOC_CONTEXT );

    CMenu* pSubMenu = menu.GetSubMenu(1);
    pSubMenu->TrackPopupMenuEx(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, rectButton.right, rectButton.bottom, this, &tpmParams);
    */
}


void KTocFormView::OnNMDblCllkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{   
    return;

	CPoint ptInTree, ptInSrceen;
	GetCursorPos(&ptInSrceen); 
	ptInTree = ptInSrceen;
	m_treeCtrlLayer.ScreenToClient(&ptInTree);       

	HTREEITEM hHitItem = m_treeCtrlLayer.HitTest(ptInTree);     
	if( NULL == hHitItem ) {
		*pResult = 0;
		return;
	}

	m_treeCtrlLayer.SelectItem(hHitItem);    

	// Popup 삭제
	CString strLayerName = GetCurrentSelectedLayerName();

	if( strLayerName.GetLength() == 0 )
	{
		*pResult = 0;
		return;
	}

	if ( strLayerName.CompareNoCase(TABLE_NODE) == 0 )
	{
		m_pMapView->ShowNodeThemes();
	}
	else if ( strLayerName.CompareNoCase(TABLE_LINK) == 0 )
	{
		m_pMapView->ShowLinkThemes();
	}
	else if ( strLayerName.CompareNoCase(TABLE_ZONE) == 0 )
	{
		m_pMapView->ShowZoneThemes();
	}

	*pResult = 0;
	return;
}


CString KTocFormView::GetCurrentSelectedLayerName()
{
	HTREEITEM hItem = m_treeCtrlLayer.GetSelectedItem();
	if( NULL == hItem ) {
		return _T("");
	}

    int nLayerID    = m_treeCtrlLayer.GetItemData(hItem);

    CString strName(_T(""));
    if (nLayerID == KLayerID::Node()) {
        strName = TABLE_NODE;
    } else if (nLayerID == KLayerID::Link()) {
        strName = TABLE_LINK;
    } else if (nLayerID == KLayerID::Zone()) {
        strName = TABLE_ZONE;
    } else {
        return _T("");
    }
    
	return strName;
}


void KTocFormView::SetBuddyMapView( KMapView* a_pMapView )
{
	m_pMapView = a_pMapView;
	int nState = LAYER_ON;

	m_treeCtrlLayer.DeleteAllItems();

	HTREEITEM hItemRoot;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		hItemRoot = m_treeCtrlLayer.InsertItemState(_T("지도"), 0, 0, 0); {
			m_treeCtrlLayer.SetItemData(hItemRoot, -1);
			m_treeCtrlLayer.SetItemBold(hItemRoot);
		}
	}
	else
	{
		hItemRoot = m_treeCtrlLayer.InsertItemState(_T("Map"), 0, 0, 0); {
			m_treeCtrlLayer.SetItemData(hItemRoot, -1);
			m_treeCtrlLayer.SetItemBold(hItemRoot);
		}
	}
	/*HTREEITEM hItemRoot = m_treeCtrlLayer.InsertItemState(_T("Map"), 0, 0, 0); {
        m_treeCtrlLayer.SetItemData(hItemRoot, -1);
	    m_treeCtrlLayer.SetItemBold(hItemRoot);
    }*/

    HTREEITEM hItem = nullptr;
    if (true) {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			hItem  = m_treeCtrlLayer.InsertItemState(_T("노드"), 1, 1, nState, hItemRoot);
		}
		else
		{
			hItem  = m_treeCtrlLayer.InsertItemState(_T("Node"), 1, 1, nState, hItemRoot);
		}
        m_treeCtrlLayer.SetItemData(hItem, KLayerID::Node());
    }

    if (true) {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			hItem = m_treeCtrlLayer.InsertItemState(_T("링크"), 2, 2, nState, hItemRoot);
		}
		else
		{
			hItem = m_treeCtrlLayer.InsertItemState(_T("Link"), 2, 2, nState, hItemRoot);
		}
        m_treeCtrlLayer.SetItemData(hItem, KLayerID::Link());
    }
	
    if (true) {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			hItem  = m_treeCtrlLayer.InsertItemState(_T("존 경계"), 3, 3, nState, hItemRoot);
		}
		else
		{
			hItem  = m_treeCtrlLayer.InsertItemState(_T("Zone"), 3, 3, nState, hItemRoot);
		}
        m_treeCtrlLayer.SetItemData(hItem, KLayerID::Zone());
    }
    
	m_treeCtrlLayer.Expand(hItemRoot, TVE_EXPAND);
	//m_treeCtrlLayer.SetFont(&XTPAuxData().font);

    try
    {
        m_dlgTocNodeProperties.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);
        m_dlgTocLinkProperties.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);
        m_dlgTocZoneProperties.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);

        m_dlgTocNodeEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);
        m_dlgTocLinkEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);
		m_dlgTocTurnEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);
		m_dlgTocDemarcationEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);

        if (KmzSystem::IsPassengerSystem()) {   
            m_dlgTocTransitEditor.SetTargetMap(m_pMapView->GetTarget(), m_pMapView);           
        }
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


void KTocFormView::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);

	m_nCurHeight = cy;
	int nScrollMax;

	if (cy < m_nFixScrollHeight/*m_rect.Height()*/)
	{
		nScrollMax = /*m_rect.Height()*/m_nFixScrollHeight - cy;
	}
	else
	{//스크롤이 없다.
		nScrollMax   = 0;
		ScrollWindow(0, m_nScrollPos);
		m_nScrollPos = 0;
	}

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_ALL; // SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin   = 0;
	si.nMax   = nScrollMax;
	si.nPage  = si.nMax/10;
	si.nPos   = 0;
	SetScrollInfo(SB_VERT, &si, TRUE); 
}

void KTocFormView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nDelta;
	int nMaxPos = /*m_rect.Height()*/m_nFixScrollHeight - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(nMaxPos/100,nMaxPos-m_nScrollPos);
		break;

	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(nMaxPos/100,m_nScrollPos);
		break;

	case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(nMaxPos/10,nMaxPos-m_nScrollPos);
		break;

	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(nMaxPos/10,m_nScrollPos);
		break;

	default:
		return;
	}

	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT, m_nScrollPos, TRUE);
	ScrollWindow(0,-nDelta);

	KDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void KTocFormView::OnBnClickedRadio1()
{
	m_tabFlag = KEMSymbol;
	DisplayTabControl(m_tabCtrl.GetCurSel());
}

void KTocFormView::OnBnClickedRadio2()
{
	m_tabFlag = KEMNetworkEdit;
	DisplayTabControl(m_tabCtrl.GetCurSel());
}

void KTocFormView::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	DisplayTabControl(m_tabCtrl.GetCurSel());
	*pResult = 0;
}

void KTocFormView::DisplayTabControl(int a_nTabIndex)
{
	if(m_pCurDlg != NULL)
	{
        m_pCurDlg->ActionFeedback(KDialogFeedback::KEMActionFeedbackDeActivate);
		m_pCurDlg->ShowWindow(SW_HIDE);
		m_pCurDlg = NULL;
	}

	int nIndex    = a_nTabIndex; /*m_tabCtrl.GetCurSel();*/
	int nTabCount = m_tabCtrl.GetItemCount();

	if (KmzSystem::IsPassengerSystem())
	{
		if (m_tabFlag == KEMSymbol )
		{
			if (nTabCount > 2)
			{
				m_tabCtrl.DeleteItem(4);
				m_tabCtrl.DeleteItem(3);
				m_tabCtrl.DeleteItem(2);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					m_tabCtrl.InsertItem(2, _T("존 경계"));
				}
				else {
					m_tabCtrl.InsertItem(2, _T("Zone"));
				}
			}
		}
		else
		{
			if (nTabCount == 3)
			{
				m_tabCtrl.DeleteItem(2);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					m_tabCtrl.InsertItem(2, _T("존 경계"));
					m_tabCtrl.InsertItem(3, _T("회전제약"));
					m_tabCtrl.InsertItem(4, _T("노선"));
				}
				else {
					m_tabCtrl.InsertItem(2, _T("Zone"));
					m_tabCtrl.InsertItem(3, _T("Turn"));
					m_tabCtrl.InsertItem(4, _T("Transit"));
				}
			}
		}
	}
	else
	{
		if (m_tabFlag == KEMSymbol )
		{
			if (nTabCount == 2)
			{	
				m_tabCtrl.DeleteItem(3);
				m_tabCtrl.DeleteItem(2);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					m_tabCtrl.InsertItem(2, _T("존 경계"));
				}
				else {
					m_tabCtrl.InsertItem(2, _T("Zone"));
				}
			}
		}
		else
		{
			if (nTabCount == 3)
			{
				m_tabCtrl.DeleteItem(2);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					m_tabCtrl.InsertItem(2, _T("존 경계"));
					m_tabCtrl.InsertItem(3, _T("회전제약"));
				}
				else {
					m_tabCtrl.InsertItem(2, _T("Zone"));
					m_tabCtrl.InsertItem(3, _T("Turn"));
				}
			}
		}
	}	
		
	nTabCount = m_tabCtrl.GetItemCount();
    if (nIndex > nTabCount-1)
    {
        nIndex = 1;
    }

	m_tabCtrl.SetCurSel(nIndex);
	
	if (m_tabFlag == KEMSymbol)
	{			
		switch(nIndex)
		{
		case 0:
			m_dlgTocNodeProperties.ShowWindow(SW_SHOW);
			m_pCurDlg = &m_dlgTocNodeProperties;
			break;
		case 1:
			m_dlgTocLinkProperties.ShowWindow(SW_SHOW);
			m_pCurDlg = &m_dlgTocLinkProperties;
			break;
		case 2:
		default:
			m_dlgTocZoneProperties.ShowWindow(SW_SHOW);
			m_pCurDlg = &m_dlgTocZoneProperties;	
			break;
		}
	}

	if (m_tabFlag == KEMNetworkEdit)
	{
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
	}

    if(m_pCurDlg != NULL)
    {
        m_pCurDlg->ActionFeedback(KDialogFeedback::KEMActionFeedbackActivate);
    }
}

void KTocFormView::TemplateChanged( LPCTSTR a_strTemplateName )
{

}

void KTocFormView::MapEditCommand( int a_nType )
{
    CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
    m_tabFlag = KEMNetworkEdit;
    //m_tabCtrl.SetCurSel(a_nType);
    DisplayTabControl(a_nType);
}

void KTocFormView::OnBnClickedButtonHide()
{
    if (m_pMapView)
        ImChampFrameWindow::GetChildFrame(m_pMapView)->ShowToc(false);
}


void KTocFormView::OnSymbologyThemeNode()
{
    if (m_pMapView)
        m_pMapView->ShowNodeThemes();
}

void KTocFormView::OnClearThemeNode()
{
}

void KTocFormView::OnSymbologyThemeLink()
{
    if (m_pMapView)
        m_pMapView->ShowLinkThemes();
}

void KTocFormView::OnClearThemeLink()
{
    if (m_pMapView)
        m_pMapView->ClearLinkThemes();
}

void KTocFormView::OnSymbologyThemeZone()
{
    if (m_pMapView)
        m_pMapView->ShowZoneThemes();
}

void KTocFormView::OnClearThemeZone()
{
    if (m_pMapView)
        m_pMapView->ClearZoneThemes();
}
