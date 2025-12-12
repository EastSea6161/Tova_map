// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "KmzApp.h"

#include "ChildFrm.h"
#include "IOFrameWnd.h"
#include "IOView.h"
#include "MainFrameWnd.h"
#include "MapFrameWnd.h"
#include "MapView.h"
#include "Target.h"

#include "TocFrameWnd.h"
#include "TocFormView.h"
#include "TocHideShowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
    ON_WM_SIZE()
	ON_WM_MDIACTIVATE()
	ON_WM_DESTROY()

// 	ON_COMMAND(ID_FIELD_ADDFIELD, &CChildFrame::OnFieldAddfield)
// 	ON_UPDATE_COMMAND_UI( ID_FIELD_ADDFIELD, &CChildFrame::OnUpdateFieldAddfield )
// 	ON_COMMAND(ID_FIELD_DELETEFIELD, &CChildFrame::OnFieldDeletefield)
// 	ON_UPDATE_COMMAND_UI( ID_FIELD_DELETEFIELD, &CChildFrame::OnUpdateFieldDeletefield )
// 	ON_COMMAND(ID_FIELD_SORT, &CChildFrame::OnFieldSort)
// 	ON_UPDATE_COMMAND_UI( ID_FIELD_SORT, &CChildFrame::OnUpdateFieldSort )
// 	ON_COMMAND(ID_FIELD_FREEZE, &CChildFrame::OnFieldFreeze)
// 	ON_UPDATE_COMMAND_UI( ID_FIELD_FREEZE, &CChildFrame::OnUpdateFieldFreeze )
// 	ON_COMMAND(ID_FIELD_PROPERTIES, &CChildFrame::OnFieldProperties)
// 	ON_UPDATE_COMMAND_UI( ID_FIELD_PROPERTIES, &CChildFrame::OnUpdateFieldProperties )
// 	ON_COMMAND(ID_TABLEMANAGER_VIEW, &CChildFrame::OnTablemanagerView)
// 	ON_UPDATE_COMMAND_UI( ID_TABLEMANAGER_VIEW, &CChildFrame::OnUpdateTablemanagerView )
// 	ON_COMMAND(ID_TABLEMANAGER_CREATEGRAPH, &CChildFrame::OnTablemanagerCreategraph)
// 	ON_UPDATE_COMMAND_UI( ID_TABLEMANAGER_CREATEGRAPH, &CChildFrame::OnUpdateTablemanagerCreategraph )
// 	ON_COMMAND(ID_TABLEMANAGER_SELECTBYATTRIBUTES, &CChildFrame::OnTablemanagerSelectbyattributes)
// 	ON_UPDATE_COMMAND_UI( ID_TABLEMANAGER_SELECTBYATTRIBUTES, &CChildFrame::OnUpdateTablemanagerSelectbyattributes )
// 	ON_COMMAND(ID_TABLEMANAGER_CALCULATEFIELD, &CChildFrame::OnTablemanagerCalculatefield)
// 	ON_UPDATE_COMMAND_UI( ID_TABLEMANAGER_CALCULATEFIELD, &CChildFrame::OnUpdateTablemanagerCalculatefield )
// 	ON_COMMAND(ID_TABLEMANAGER_IMPORT, &CChildFrame::OnTablemanagerImport)
// 	ON_UPDATE_COMMAND_UI( ID_TABLEMANAGER_IMPORT, &CChildFrame::OnUpdateTablemanagerImport )
// 	ON_COMMAND(ID_TABLEMANAGER_EXPORT, &CChildFrame::OnTablemanagerExport)
// 	ON_UPDATE_COMMAND_UI( ID_TABLEMANAGER_EXPORT, &CChildFrame::OnUpdateTablemanagerExport )
// 	ON_COMMAND(ID_TABLEMANAGER_RELOADCACHE, &CChildFrame::OnTablemanagerReloadcache)
// 	ON_UPDATE_COMMAND_UI( ID_TABLEMANAGER_RELOADCACHE, &CChildFrame::OnUpdateTablemanagerReloadcache )
END_MESSAGE_MAP()

CChildFrame::CChildFrame()
	:	m_bShowMapView(false),
		m_bShowIOView(false),	
		m_pMapFrameWnd(NULL),
		m_pIOFrameWnd(NULL),
        m_pTarget(NULL),
        m_bShowToc(true)
{
    TxLogDebugVisitor();
}

CChildFrame::~CChildFrame()
{
    TxLogDebugVisitor();
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    TxLogDebugVisitor();

    // TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
    if( !CChildFrameBase::PreCreateWindow(cs) )
        return FALSE;

    cs.style&=~(LONG)FWS_ADDTOTITLE;
    return TRUE;
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    TxLogDebugStartMsg();

    if (CChildFrameBase::OnCreate(lpCreateStruct) == -1)
        return -1;

    KMapView* pMapView = m_pMapFrameWnd->GetMapView();
    KIOView*  pIOView  = m_pIOFrameWnd->GetIOView();

    pMapView->SetIOView(pIOView);

    TxLogDebugEndMsg();
    return 0;
}

BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
    TxLogDebugStartMsg();
       
    CRect oRectClient;
    GetClientRect(oRectClient);

    m_wndMainSplitter.CreateStatic( this, 1, 2 );
    int nLeftID  = m_wndMainSplitter.IdFromRowCol(0, 0);
    int nRightID = m_wndMainSplitter.IdFromRowCol(0, 1);
    
      m_wndLeftSplitter.CreateStatic( &m_wndMainSplitter, 2, 1, WS_CHILD|WS_VISIBLE, nLeftID );
        m_wndLeftSplitter.CreateView( 0, 0, RUNTIME_CLASS(KMapFrameWnd), oRectClient.Size(), pContext );
        m_wndLeftSplitter.CreateView( 1, 0, RUNTIME_CLASS(KIOFrameWnd),  oRectClient.Size(), pContext );

      m_wndRightSplitter.CreateStatic( &m_wndMainSplitter, 1, 2, WS_CHILD|WS_VISIBLE, nRightID );
        m_wndRightSplitter.CreateView( 0, 0, RUNTIME_CLASS(KTocFrameWnd), oRectClient.Size(), pContext );
        m_wndRightSplitter.CreateView( 0, 1, RUNTIME_CLASS(KTocHideShowView), oRectClient.Size(), pContext );
    
    m_pMapFrameWnd = (KMapFrameWnd*) m_wndLeftSplitter.GetPane(0, 0);
    m_pIOFrameWnd  = (KIOFrameWnd* ) m_wndLeftSplitter.GetPane(1, 0);    

    m_pTocFrameWnd     = (KTocFrameWnd*)     m_wndRightSplitter.GetPane(0, 0);
    m_pTocHideShowView = (KTocHideShowView*) m_wndRightSplitter.GetPane(0, 1);

    m_wndMainSplitter.EnableNoSize(TRUE);
    TxLogDebugEndMsg();
    return TRUE;
}

void CChildFrame::OnDestroy()
{
	TxLogDebugStartMsg();
	CXTPFrameWndBase<CMDIChildWnd>::OnDestroy();
	CMainFrameWnd* pFrame = (CMainFrameWnd*)AfxGetMainWnd();

	pFrame->TargetChildFrameClosedNotify( m_pTarget->GetObjectID() );
	TxLogDebugEndMsg();
}

void CChildFrame::PostNcDestroy( void )
{
    TxLogDebugVisitor();
    // $task : Child frame 종료처리 추가
    CChildFrameBase::PostNcDestroy();
}

void CChildFrame::CloseFrame()
{
	SendMessage(WM_CLOSE);	
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // let the view have first crack at the command
    // otherwise, do default handling
    return CChildFrameBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
    CChildFrameBase::AssertValid();
}


void CChildFrame::Dump(CDumpContext& dc) const
{
    CChildFrameBase::Dump(dc);
}
#endif //_DEBUG


void CChildFrame::LoadTarget(KTarget* pTarget)
{
    TxLogDebugStartMsg();

	m_pTarget = pTarget;

	KMapView* pMapView = m_pMapFrameWnd->GetMapView();
	pMapView->LoadMapView(m_pTarget);

    // TocFormView에 지도 등록(등록된 지도 정보로 부터 기초 정보 로딩->다이얼로그 세팅)
    m_pTocFrameWnd->SetBuddyMapView(pMapView);

    // MapView에 TocFormView 등록(MapView는 자신의 변화를 Toc에 통보한다)
    pMapView->SetBuddyTocFormView(m_pTocFrameWnd->GetTocFormView());
    TxLogDebugEndMsg();
}


KTarget* CChildFrame::GetTarget(void)
{
	return m_pTarget;
}


void CChildFrame::ShowMapView( bool bShow )
{
    TxLogDebugStartMsg();
    
    if( m_bShowMapView == bShow )
    {
        return;
    }

    m_bShowMapView = bShow;
    RecalcLayout();

    UpdateTargetFrameTitle();
    ActivateFrame( SW_SHOW );	
    TxLogDebugEndMsg();
}


void CChildFrame::ShowIOView( bool bShow )
{
    TxLogDebugStartMsg();
    UpdateTargetFrameTitle();
    ActivateFrame( SW_SHOW );

	if(m_bShowIOView == bShow)
	{
		return;
	}
	m_bShowIOView = bShow;
	RecalcLayout();
    TxLogDebugEndMsg();
}


void CChildFrame::UpdateTargetFrameTitle( void )
{
    if (m_pTarget == nullptr)
        return;

    CString  strTitle(_T(""));
    CString  strScenaioName(_T(""));

    KObject* pScenario = m_pTarget->GetParentObject();
    if ( pScenario != NULL )
    {
        strScenaioName = pScenario->GetName();
    }
	
	strTitle.Format( _T("%s - %s"), strScenaioName, m_pTarget->GetName() );
	SetWindowText( strTitle );
}


void CChildFrame::RecalcLayout( void )
{
    UpdateTargetFrameTitle();

    // 지도와 테이블 정보가 보이지 않는 경우 Hide 시킨다.
    // 2014-06-24 : GetMapView 오류(Map Control 창만이 보이는 문제)

    if( (false == m_bShowMapView) && (false == m_bShowIOView) )
    {
        ShowWindow(SW_HIDE);
        return;
    }
    else
    {
        ShowWindow(SW_SHOW);
    }

    CRect oRect;
    GetClientRect( &oRect );

    UINT dpi = GetDpiForWindow(m_hWnd);

    int nTocBase = 250;
    int nHideShowWidth = MulDiv(25, (int)dpi, USER_DEFAULT_SCREEN_DPI);
    int nTocWidth = MulDiv(nTocBase, (int)dpi, USER_DEFAULT_SCREEN_DPI) + (nHideShowWidth - 25);

    if (m_bShowToc == true)
    {
        nHideShowWidth = 0;
    }
    else
    {
        nTocWidth = 0;
    }

    int nLeftWidth = (int)(oRect.Width() - (int)nTocWidth - (int)nHideShowWidth);
    if (oRect.Height() <= 700 && m_bShowToc == true)
    {
        // 스크롤 사이즈를 빼준다
        nLeftWidth -= 15;
        nTocWidth  += 15;
    }

    // 500보다 작으면 최소 500을 유지 시켜준다.
    if (nLeftWidth < 100)
        nLeftWidth = 100;

	if( (true == m_bShowMapView) && (true == m_bShowIOView) )
	{ //지도+테이블        
        m_wndMainSplitter.ShowColumn();
        m_wndMainSplitter.SetColumnInfo(0, nLeftWidth, 10);
        m_wndMainSplitter.SetColumnInfo(1, nTocWidth + nHideShowWidth, 10);

        m_wndLeftSplitter.ShowRow();		        
        m_wndLeftSplitter.SetRowInfo( 0, (int)(oRect.Height()*0.7), 0 );

        if (m_bShowToc == true) {
            m_wndRightSplitter.HideColumn(1);
        }
        else
        {
            m_wndRightSplitter.HideColumn(0);
        }
	}
	else if( true == m_bShowMapView )
	{ //지도        
        m_wndMainSplitter.ShowColumn();  
        m_wndMainSplitter.SetColumnInfo(0, nLeftWidth, 10);
        m_wndMainSplitter.SetColumnInfo(1, nTocWidth + nHideShowWidth, 10);

        m_wndLeftSplitter.HideRow( 1 );  

        if (m_bShowToc == true)
        {
            m_wndRightSplitter.HideColumn(1);
        }
        else
        {
            m_wndRightSplitter.HideColumn(0);
        }
	}
	else if( true == m_bShowIOView)
	{ //테이블
        
        m_wndMainSplitter.ShowColumn();  
        m_wndMainSplitter.HideColumn(1);
        m_wndMainSplitter.SetColumnInfo(0, (int)(oRect.Width()), 10);
        m_wndLeftSplitter.HideRow( 0 );
	}
    else
    {
        return;
    }

    m_wndLeftSplitter.RecalcLayout();   
    m_wndMainSplitter.RecalcLayout();
}

KMapView* CChildFrame::GetMapView( void )
{
	if( NULL != m_pMapFrameWnd )
	{
		//return (KMapView*)m_pMapFrameWnd->GetActiveView();
		return m_pMapFrameWnd->GetMapView();
	}
	return NULL;
}

KIOView* CChildFrame::GetIOView(void)
{
    if(NULL != m_pIOFrameWnd)
    {
        return m_pIOFrameWnd->GetIOView();
    }
    return NULL;
}

KTocFormView* CChildFrame::GetTocFormView()
{
	if( NULL != m_pMapFrameWnd )
	{
		return m_pTocFrameWnd->GetTocFormView();
	}
	return NULL;
}

void CChildFrame::updateTableMenuState( CCmdUI* pCmdUI )
{
	BOOL bEnable = FALSE;
	if( true == m_bShowIOView)
	{
		KIOView* pView = GetIOView();
		if(NULL != pView)
		{
			KIOTableView* pTableView = pView->GetActiveTableView();
			if(NULL != pTableView)
			{
				bEnable = TRUE;
			}
		}
	}

	pCmdUI->Enable( bEnable );
}

void CChildFrame::OnMDIActivate( BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd )
{
    CChildFrameBase::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

    HICON hIconActivate   = AfxGetApp()->LoadIcon(IDR_TARGET_SELECT);
    HICON hIconDeactivate = AfxGetApp()->LoadIcon(IDR_TARGET_NO_SELECT);
        
    if(pActivateWnd != NULL)
    {
        if (pActivateWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
		{
            pActivateWnd->SetIcon(hIconActivate, TRUE);
			CChildFrame* pFrame   = (CChildFrame*)pActivateWnd;
			KMapView*    pMapView = pFrame->GetMapView();
			if (pMapView != NULL)
			{
				pMapView->ActivatedFrameWndNotifyProcess();
			}

			KIOView* pIOView = pFrame->GetIOView();
			if (pIOView != NULL)
			{
				pIOView->ActivatedFrameWndNotifyProcess();
			}
		}
    }   
        
    if(pDeactivateWnd != NULL)
    {
        if (pDeactivateWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
		{
            pDeactivateWnd->SetIcon(hIconDeactivate, TRUE);
			CChildFrame* pFrame   = (CChildFrame*)pDeactivateWnd;
			KMapView*    pMapView = pFrame->GetMapView();
			if (pMapView != NULL)
			{
				pMapView->DeActivatedFrameWndNotifyProcess();
			}

			KIOView* pIOView = pFrame->GetIOView();
			if (pIOView != NULL)
			{
				pIOView->DeActivatedFrameWndNotifyProcess();
			}
		}
    }

	this->GetParent()->Invalidate(NULL);
}

void CChildFrame::OnSize( UINT nType, int cx, int cy )
{
    CChildFrameBase::OnSize(nType, cx, cy);
    RecalcLayout();
}

void CChildFrame::ShowToc( bool a_bShow, bool a_bRefresh /*= true*/ )
{
    m_bShowToc = a_bShow;
    if (a_bRefresh)
        RecalcLayout();
}

bool CChildFrame::IsShowToc()
{
    return m_bShowToc;
}


