// MapFrameWnd.cpp : 구현 파일입니다.
//


#include "stdafx.h"
#include "KmzApp.h"
#include "MapFrameWnd.h"
#include "MapView.h"


/******************************************************************************
* KMapFrameWnd                                                                *
******************************************************************************/
IMPLEMENT_DYNCREATE(KMapFrameWnd, CXTPFrameWnd)


/**************************************
* Constructors and Destructor         *
**************************************/
KMapFrameWnd::KMapFrameWnd()
{
    TxLogDebugVisitor();
	m_pwndMapView = NULL;
}


KMapFrameWnd::~KMapFrameWnd()
{
    TxLogDebugVisitor();
}


void KMapFrameWnd::ShowDockingPanePopupMenu(CXTPDockingPane* pPopupPane, CPoint pt, LPRECT lprcExclude)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_PANES));
	CMenu* pPopup = menu.GetSubMenu(0);

	if (pPopupPane->IsHidden())
	{
		pPopup->CheckMenuItem(ID_POPUP_AUTOHIDE, MF_BYCOMMAND|MF_CHECKED);
		pPopup->EnableMenuItem(ID_POPUP_FLOATING, MF_BYCOMMAND|MF_DISABLED);
		pPopup->EnableMenuItem(ID_POPUP_DOCKABLE, MF_BYCOMMAND|MF_DISABLED);
	}
	else if (pPopupPane->GetOptions() & xtpPaneNoDockable)
	{
		pPopup->CheckMenuItem(ID_POPUP_FLOATING, MF_BYCOMMAND|MF_CHECKED);
	}
	else
	{
		pPopup->CheckMenuItem(ID_POPUP_DOCKABLE, MF_BYCOMMAND|MF_CHECKED);
	}

    pPopup->EnableMenuItem(ID_POPUP_HIDE, MF_DISABLED);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = lprcExclude ? *lprcExclude : CRect(0, 0, 0, 0);

	int nCommand = GetCommandBars()->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD, pt.x, pt.y, NULL, &tpm);

	switch (nCommand)
	{
	case ID_POPUP_HIDE:
		pPopupPane->Close();
		break;		

	case ID_POPUP_AUTOHIDE:
		m_paneManager.ToggleAutoHide(pPopupPane);
		break;

	case ID_POPUP_DOCKABLE:
		if (pPopupPane->GetOptions() & xtpPaneNoDockable)
		{
			pPopupPane->SetOptions(pPopupPane->GetOptions() & ~xtpPaneNoDockable);
		}
		break;

	case ID_POPUP_FLOATING:
		if ((pPopupPane->GetOptions() & xtpPaneNoDockable) == 0)
		{
			if (!pPopupPane->IsFloating())
				m_paneManager.ToggleDocking(pPopupPane);

			pPopupPane->SetOptions(pPopupPane->GetOptions() | xtpPaneNoDockable);
		}

		break;
	}
}


BOOL KMapFrameWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if( (NULL != m_pwndMapView) && (m_pwndMapView->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo )) )
	{
		return TRUE;
	}

	// otherwise, do default handling
	return CXTPFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BEGIN_MESSAGE_MAP(KMapFrameWnd, CXTPFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_XTP_CREATECONTROL()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND( ID_MAPVIEW_EDITOPERATION, OnMapViewEditOperation )
END_MESSAGE_MAP()


BOOL KMapFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
    TxLogDebugVisitor();
    if( !CXTPFrameWnd::PreCreateWindow(cs) )
        return FALSE;

    return TRUE;
}

int KMapFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    TxLogDebugStartMsg();

	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/// Create Child Toolbar
	if( !InitCommandBars() )
	{
		return -1;
	}

	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways = false;

    /*
	CXTPToolBar* pMapControlBar = (CXTPToolBar*)pCommandBars->Add( _T("Map Toolbar"), xtpBarTop );
	pMapControlBar->LoadToolBar   ( IDR_MAPVIEW );
	pMapControlBar->EnableDocking ( xtpFlagAlignTop | xtpFlagHideWrap );
	pMapControlBar->SetShowGripper( FALSE );
    */
    m_pMapControlBar = (CXTPToolBar*)pCommandBars->Add( _T("Map Toolbar"), xtpBarTop );
    m_pMapControlBar->LoadToolBar   ( IDR_MAPVIEW );
    m_pMapControlBar->EnableDocking ( xtpFlagAlignTop | xtpFlagHideWrap );
    m_pMapControlBar->SetShowGripper( FALSE );
	/// Create Docking layer view
	m_paneManager.InstallDockingPanes(this);

	m_paneManager.SetTheme                     ( xtpPaneThemeVisualStudio2005 );
	m_paneManager.SetAlphaDockingContext       ( TRUE );
	m_paneManager.SetShowDockingContextStickers( TRUE );
	m_paneManager.SetShowContentsWhileDragging ( FALSE);
	m_paneManager.SetDefaultPaneOptions        ( xtpPaneHasMenuButton );

	// Create docking panes.
	CXTPDockingPane* pwndStatusPane = m_paneManager.CreatePane(ID_VIEW_STATUS,  CRect(0, 0, 200, 40),  xtpPaneDockBottom );
	pwndStatusPane->SetOptions(pwndStatusPane->GetOptions() | xtpPaneNoCloseable | xtpPaneNoHideable | xtpPaneNoCaption );
    TxLogDebugEndMsg();
	return 0;
}


BOOL KMapFrameWnd::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
    TxLogDebugStartMsg();

    CCreateContext cc;
    cc.m_pNewViewClass = RUNTIME_CLASS( KMapView );

	TxLogDebugVisitor();

    m_pwndMapView = (KMapView*)CreateView( &cc, AFX_IDW_PANE_FIRST );
    
	TxLogDebugVisitor();

    // edit by nombara, 2012. 02. 16 : 
    //                  왜 코드를 넣었을 까 ? 정확한 의미 파악이 안됨. 
    //                  일정 시간 이후로 문제가 없다면 아래 코드는 삭제 할 것
    // m_pwndMapView->SendMessage( WM_INITIALUPDATE );

    m_pwndMapView->ModifyStyle  ( WS_BORDER, 0 );
    m_pwndMapView->ModifyStyleEx( WS_EX_CLIENTEDGE, 0 );
    	
    SetActiveView( m_pwndMapView );	
    
    TxLogDebugEndMsg();
    return TRUE;
}

BOOL KMapFrameWnd::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    //return wndCommandEdit.PreTranslateMessage(pMsg);
    // 
    BOOL bShift = FALSE;

    switch(pMsg->message)
    {
    case WM_KEYDOWN:
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
        {
            HWND hCommandEdit = m_wndCommandView.GetCommandEditHandle();
            ::SendMessage(hCommandEdit, WM_KEYDOWN, pMsg->wParam, pMsg->lParam);    
            return TRUE;
        }

        break;

    case WM_CHAR:
        HWND hCommandEdit = m_wndCommandView.GetCommandEditHandle();
        if( pMsg->hwnd != hCommandEdit)
        {
            ::SendMessage(hCommandEdit, WM_CHAR, pMsg->wParam, pMsg->lParam);
            return TRUE;
        }        
        break;
    }      

    // 모달메시지인경우 : LBUTTONDOWN 막기
    if (m_wndCommandView.IsModal() && pMsg->message == WM_LBUTTONDOWN)
    {
        HWND hCommandEdit = m_wndCommandView.GetCommandEditHandle();
        if( pMsg->hwnd != hCommandEdit)
        {  
            AfxMessageBox(_T("명령창에서 입력(선택)해 주세요."));
            return TRUE;
        }        
    }
    
    return CXTPFrameWnd::PreTranslateMessage(pMsg);
}

int KMapFrameWnd::OnCreateControl( LPCREATECONTROLSTRUCT lpCreateControl )
{   
    if( ! lpCreateControl->bToolBar )
    {
        return FALSE;
    }

    CXTPToolBar* pToolbar = DYNAMIC_DOWNCAST( CXTPToolBar, lpCreateControl->pCommandBar );
    if( NULL == pToolbar )
    {
        return FALSE;
    }

    if (ID_MAPVIEW_FULLEXTENT == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("전체화면"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPZOOMEXTENT);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    }
    else if (ID_MAPVIEW_ZOOMIN == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("화면확대"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPZOOMIN);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    }
    else if (ID_MAPVIEW_ZOOMOUT == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("화면축소"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPZOOMOUT);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    }
    else if (ID_MAPVIEW_PAN == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("화면이동"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPPAN);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    } 
    else if (ID_MAPVIEW_BACKWARD == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("이전화면"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPPREVIEW);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    } 
    else if (ID_MAPVIEW_FORWARD == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("이후화면"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPNEXTVIEW);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    }
    else if (ID_MAPVIEW_SYNC_NONE == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("동기화중지"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPSYNCNONE);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    } 
    else if (ID_MAPVIEW_SYNC_POS == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("동기화(위치)"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPSYNCPOS);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    }
    else if (ID_MAPVIEW_SYNC_POS_SCALE == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("동기화(위치+스케일)"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPSYNC);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    }
	else if( ID_MAPVIEW_IDENTIFY_LAYER == lpCreateControl->nID )
	{
		m_pComboTarget = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
        if (KmzSystem::GetLanguage() == KEMKorea) {
            m_pComboTarget->AddString( _T("노드") );
            m_pComboTarget->AddString( _T("링크") );
            m_pComboTarget->AddString( _T("존") ); 
        }
        else {
            m_pComboTarget->AddString( _T("Node") );
            m_pComboTarget->AddString( _T("Link") );
            m_pComboTarget->AddString( _T("Zone") ); 
        }
		
		m_pComboTarget->SetCurSel( 1 );

		m_pComboTarget->SetCaption( _T("Identify:") );
        if (KmzSystem::GetLanguage() == KEMKorea) {
            m_pComboTarget->SetCaption( _T("속성보기:") );
        }
		m_pComboTarget->SetStyle( xtpComboLabel );
		m_pComboTarget->SetWidth( 100 );
		lpCreateControl->pControl = m_pComboTarget;
	}
    else if (ID_MAPVIEW_IDENTIFY == lpCreateControl->nID)
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("속성보기"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_IDENTIFY);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    }
	else if( ID_MAPVIEW_EDITLABEL == lpCreateControl->nID )
    {
        CXTPControlLabel* pLabel = (CXTPControlLabel*)CXTPControlLabel::CreateObject();
        pLabel->SetCaption( _T("Edit:") );
        pLabel->SetStyle( xtpButtonCaption );
        lpCreateControl->pControl = pLabel;
    }
    else if( ID_MAPVIEW_EDITOPERATION == lpCreateControl->nID )
    {/*Start, Stop Editing*/
        lpCreateControl->controlType = xtpControlPopup;
    }
    else if (ID_MAPVIEW_FIND == lpCreateControl->nID)
    {
        m_pMapEditFindButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        m_pMapEditFindButton->SetStyle(xtpButtonIcon);
        m_pMapEditFindButton->SetCaption(_T("Find"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_FIND);
        m_pMapEditFindButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = m_pMapEditFindButton;
    }
    else if( ID_MAPVIEW_EDITTARGET == lpCreateControl->nID )
    {/*Target Layer*/
        m_pComboTarget = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
        if (KmzSystem::GetLanguage() == KEMKorea) {
            m_pComboTarget->AddString( _T("노드") );
            m_pComboTarget->AddString( _T("링크") );     
        }
        else {
            m_pComboTarget->AddString( _T("Node") );
            m_pComboTarget->AddString( _T("Link") );     
        }
             
        m_pComboTarget->SetCurSel( 1 );
        m_pComboTarget->SetCaption( _T("Select:") );

        if (KmzSystem::GetLanguage() == KEMKorea) {
            m_pComboTarget->SetCaption( _T("선택:") );
        }

        m_pComboTarget->SetStyle( xtpComboLabel );
        m_pComboTarget->SetWidth( 80 );
        lpCreateControl->pControl = m_pComboTarget;
    }
    else if (ID_MAPVIEW_SELECTION_RECT == lpCreateControl->nID)
    {
        m_pSelectionRectButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        m_pSelectionRectButton->SetStyle(xtpButtonIcon);
        m_pSelectionRectButton->SetCaption(_T("SelectByRect"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPSELECT_RECT/*IDI_ICON_MAPDRAG*/);
        m_pSelectionRectButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = m_pSelectionRectButton;
    }
    else if (ID_MAPVIEW_SELECTION_SEARCH == lpCreateControl->nID)
    {
        m_pSelectionPolyButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        m_pSelectionPolyButton->SetStyle(xtpButtonIcon);
        m_pSelectionPolyButton->SetCaption(_T("SelectByPolygon"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPSELECT_POLYGON/*IDI_ICON_MAPDRAG*/);
        m_pSelectionPolyButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = m_pSelectionPolyButton;
    }      
    else if ( ID_MAPVIEW_CLEARSELECTION == lpCreateControl->nID )
    {
        m_pMapRefreshButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        m_pMapRefreshButton->SetStyle(xtpButtonIcon);
        m_pMapRefreshButton->SetCaption(_T("Clear"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPSELECT_NONE);
        m_pMapRefreshButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = m_pMapRefreshButton;
    }
    else if ( ID_MAPVIEW_PASTE == lpCreateControl->nID )
    {
        m_pMapPasteButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        m_pMapPasteButton->SetStyle(xtpButtonIcon);
        m_pMapPasteButton->SetCaption(_T("Paste"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPATT_PASTE);
        m_pMapPasteButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = m_pMapPasteButton;
    }
    else if( ID_MAPVIEW_LINKTYPE == lpCreateControl->nID )
    {
        m_pComboLinkType = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
        m_pComboLinkType->SetCaption( _T("Reserved: ") );
        m_pComboLinkType->SetWidth( 120 );
        m_pComboLinkType->SetStyle( xtpComboLabel );
        lpCreateControl->pControl = m_pComboLinkType;
        /*
        m_pComboLinkType = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
        m_pComboLinkType->AddString( _T("Unidirection(단선)") );
        m_pComboLinkType->AddString( _T("Bidirection(양선)") );
        m_pComboLinkType->SetCurSel( 1 );
        m_pComboLinkType->SetCaption( _T("Link Type: ") );
        m_pComboLinkType->SetStyle( xtpComboLabel );
        m_pComboLinkType->SetWidth( 170 );
        lpCreateControl->pControl = m_pComboLinkType;
        */
    }
    else if( ID_MAPVIEW_LINKWIDTH == lpCreateControl->nID )
    {
        m_pEditLinkWidth = (CXTPControlEdit*)CXTPControlEdit::CreateObject();
        m_pEditLinkWidth->SetEditStyle(ES_NUMBER);
        m_pEditLinkWidth->ShowSpinButtons( TRUE );
        m_pEditLinkWidth->SetWidth( 120 );
        m_pEditLinkWidth->SetCaption( _T("Reserved: ") );
        m_pEditLinkWidth->ShowLabel( TRUE );
        lpCreateControl->pControl = m_pEditLinkWidth;
    }
    else if ( ID_SHAPE_EDIT_START == lpCreateControl->nID )
    {
        m_pMapEditStartButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        m_pMapEditStartButton->SetStyle(xtpButtonIcon);
        m_pMapEditStartButton->SetCaption(_T("Start"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPEDIT);
        m_pMapEditStartButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = m_pMapEditStartButton;
    }
    else if ( ID_SHAPE_EDIT_END == lpCreateControl->nID )
    {
        m_pMapEditStopButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        m_pMapEditStopButton->SetStyle(xtpButtonIcon);
        m_pMapEditStopButton->SetCaption(_T("End"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_MAPEDIT_END);
        m_pMapEditStopButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = m_pMapEditStopButton;
    }   
    else if ( ID_MAPVIEW_RULER == lpCreateControl->nID )
    {
        CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        pButton->SetStyle(xtpButtonIcon);
        pButton->SetCaption(_T("Ruler"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_RULER);
        pButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = pButton;
    }    
    else if ( ID_SCREEN_CAPTURE == lpCreateControl->nID )
    {
        m_pMapEditStopButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
        m_pMapEditStopButton->SetStyle(xtpButtonIcon);
        m_pMapEditStopButton->SetCaption(_T("Save Image"));

        HICON hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_CAPTURE);
        m_pMapEditStopButton->SetCustomIcon(hIcon);
        lpCreateControl->pControl = m_pMapEditStopButton;
    }
	else if (ID_MAP_BACKGROUND_COLOR == lpCreateControl->nID)
	{
		//HICON hIcon = AfxGetApp()->LoadIcon(IDI_MAP_BACKGROUND_COLOR);
		//CXTPControlPopupColor* pPopupColor = new CXTPControlPopupColor();

		//pPopupColor->SetCustomIcon(hIcon);
		//pPopupColor->SetCaption(_T(""));
		//if (KmzSystem::GetLanguage() == KEMKorea) 
		//{
		//	pPopupColor->SetTooltip(_T("지도 바탕화면 설정"));
		//}
		//else
		//{
		//	pPopupColor->SetTooltip(_T("Set map background color"));
		//}

		//CXTPPopupBar* pColorBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());
		//pPopupColor->SetCommandBar(pColorBar);

		//CXTPControlButtonColor* pButton = new CXTPControlButtonColor();
		//pButton->SetCaption(_T("No Fill"));
		//pColorBar->GetControls()->Add(pButton, XTP_IDS_COLOR_CUST);
		//pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_MAP_BACKGROUND_SELECTOR);
		//pColorBar->GetControls()->Add(new CXTPControlButtonColor(), XTP_IDS_MORE_COLORS);

		////pColorBar->SetTearOffPopup(_T("Text Color"), 1005, 0);
		//pColorBar->EnableCustomization(FALSE);

		CXTPControlPopupColor* pPopupColor = new CXTPControlPopupColor();
		HICON hIcon = AfxGetApp()->LoadIcon(IDI_MAP_BACKGROUND_COLOR);
		pPopupColor->SetCustomIcon(hIcon);

		CXTPPopupBar* pColorBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());
		pPopupColor->SetCommandBar(pColorBar);


		CXTPControlButtonColor* pButton = new CXTPControlButtonColor();
		pButton->SetCaption(_T("No Fill"));
		pColorBar->GetControls()->Add(pButton, XTP_IDS_COLOR_CUST);
		pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_MAP_BACKGROUND_SELECTOR);
		pColorBar->GetControls()->Add(new CXTPControlButtonColor(), XTP_IDS_MORE_COLORS);

		pColorBar->EnableCustomization(FALSE);
		pColorBar->InternalRelease();

		lpCreateControl->pControl = pPopupColor;
	}
    //
    /*
    else if( ID_MAPVIEW_EDITTASK == lpCreateControl->nID )
    {
        m_pComboTask = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
        m_pComboTask->AddString( _T("Rectangle") );
        m_pComboTask->AddString( _T("Polygon") );
        m_pComboTask->SetCurSel( 0 );
        //m_pComboTask->SetCaption( _T("Task: ") );
        m_pComboTask->SetStyle( xtpComboLabel );
        m_pComboTask->SetWidth( 100 );
        lpCreateControl->pControl = m_pComboTask;
    }  
    */
    return TRUE;
}

LRESULT KMapFrameWnd::OnDockingPaneNotify( WPARAM wParam, LPARAM lParam )
{
    TxLogDebugVisitor();

    if (wParam == XTP_DPN_SHOWWINDOW)
    {
        CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

        if (!pPane->IsValid())
        {
            if (pPane->GetID() == ID_VIEW_STATUS)
            {
                if( m_wndCommandView.GetSafeHwnd() == 0 )
                {
                    m_wndCommandView.Create( _T("STATIC"), NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0 );					
                }
                pPane->Attach( &m_wndCommandView );
            }
        }
        return TRUE;
    }

    if (wParam == XTP_DPN_CONTEXTMENU)
    {
        XTP_DOCKINGPANE_CLICK* pClick = (XTP_DOCKINGPANE_CLICK*)lParam;

        CXTPDockingPane* pPopupPane = pClick->pPane;
        if (!pPopupPane) 
            return FALSE;

        ShowDockingPanePopupMenu(pPopupPane, pClick->pt, pClick->rcExclude);

        return TRUE;
    }
    return FALSE;
}


void KMapFrameWnd::OnSetFocus(CWnd* pOldWnd)
{
    TxLogDebugVisitor();
	CXTPFrameWnd::OnSetFocus(pOldWnd);
        
	if( NULL != m_pwndMapView )
	{
		m_pwndMapView->SetFocus();
	}
}


void KMapFrameWnd::OnSize(UINT nType, int cx, int cy)
{
	CXTPFrameWnd::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void KMapFrameWnd::OnMapViewEditOperation( void )
{

}

KMapView* KMapFrameWnd::GetMapView( void )
{
    try
    {
        if (m_pwndMapView->GetSafeHwnd() == NULL)
            return NULL;
    }
    catch(...)
    {
        return NULL;
    }		
    return m_pwndMapView;
}

KCommandWnd* KMapFrameWnd::GetCommandView( void )
{
    try
    {
        if (m_wndCommandView.GetSafeHwnd() == NULL)
            return NULL;
    }
    catch(...)
    {
        return NULL;
    }	

    return &m_wndCommandView;
}

