#include "StdAfx.h"
#include "NaviExplorerWnd.h"
#include "DBaseConnector.h"
#include "ImChampFrameWindow.h"
#include "NaviItemAddDlg.h"
#include "NaviItemRenameDlg.h"

IMPLEMENT_DYNAMIC(KNaviExplorerWnd, CWnd)

	KNaviExplorerWnd::KNaviExplorerWnd(void)
{
	m_pProjecct = NULL;
}


KNaviExplorerWnd::~KNaviExplorerWnd(void)
{
}

void KNaviExplorerWnd::UpdateTree( KProject* a_pProjecct )
{
	TxLogDebugStartMsg();
	m_pProjecct = a_pProjecct;

	if (NULL == m_pProjecct)
		return;

	UpdateTree();
}

void KNaviExplorerWnd::UpdateTree()
{
	TxLogDebugStartMsg();

	if (NULL == m_pProjecct)
		return;

	if (NULL == m_wndTreeCtrl)
		return;

	ClearTree();

	InsertTreeItemRootInfo();

	m_wndTreeCtrl.SortChildren(NULL);
	m_wndTreeCtrl.Invalidate();

	TxLogDebugEndMsg();
}


void KNaviExplorerWnd::ClearTree( void )
{
	TxLogDebugVisitor();

	HTREEITEM hRoot = m_wndTreeCtrl.GetRootItem();

	if (NULL == hRoot)
		return;

	RemoveTreeItem(hRoot);
}


void KNaviExplorerWnd::RemoveTreeItem( HTREEITEM hItem )
{
	if ( hItem == NULL )
	{
		TxLogDebugWarning();
		return;
	}

	HTREEITEM hNext;
	HTREEITEM hChild = m_wndTreeCtrl.GetChildItem( hItem );

	// child item이 존재한다면 먼저 삭제
	while(NULL != hChild)
	{
		hNext = m_wndTreeCtrl.GetNextItem( hChild, TVGN_NEXT );
		RemoveTreeItem( hChild );
		hChild = hNext;
	}

	/// tree에서 해당 Item 삭제
	TNaviTreeItemData* pTreeData = (TNaviTreeItemData*)m_wndTreeCtrl.GetItemData( hItem );
	if(NULL != pTreeData)
	{
		delete pTreeData;
		pTreeData = NULL;
	}

	m_wndTreeCtrl.DeleteItem(hItem);
	m_wndTreeCtrl.SortChildren(NULL);
}


int KNaviExplorerWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if(CWnd::OnCreate( lpCreateStruct ) == -1)
		return -1;

	if(!m_imgList.Create( 16, 16, ILC_MASK | ILC_COLOR24, 0, 1))
	{
		AfxMessageBox(_T("Failed to create image list"));
		return -1;
	}

	CBitmap bmp;
	bmp.LoadBitmap(IDB_NAVIGATION_VIEW);
	m_imgList.Add(&bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0)));

	if (m_wndTreeCtrl.GetSafeHwnd() == 0)
	{
		if( !m_wndTreeCtrl.Create( 
			WS_VISIBLE | TVS_HASLINES |TVS_LINESATROOT | TVS_HASBUTTONS | 
			TVS_SHOWSELALWAYS, CRect( 0, 0, 0, 0 ), this, IDC_SED_TREECTRL ) )
		{
			TRACE0( "Unable to create tree control.\n" );
			return 0;
		}
		m_wndTreeCtrl.ModifyStyleEx( 0, WS_EX_STATICEDGE );
		m_wndTreeCtrl.SetImageList( &m_imgList, TVSIL_NORMAL );
	}
	return 0;
}


void KNaviExplorerWnd::InsertTreeItemRootInfo(  )
{
    HTREEITEM hItemRoot = nullptr;
    if (KmzSystem::GetLanguage() == KEMKorea) {
        hItemRoot = m_wndTreeCtrl.InsertItem(_T("위치"), 0, 0);
    }
    else {
        hItemRoot = m_wndTreeCtrl.InsertItem(_T("Navigation"), 0, 0);
    }
	
	InsertTreeItemNaviInfo(hItemRoot);

	m_wndTreeCtrl.Expand(hItemRoot, TVE_EXPAND);
}

void KNaviExplorerWnd::InsertTreeItemNaviInfo( HTREEITEM a_hItemRoot )
{
	CString          strQuery(_T(""));

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (nullptr == spDBaseConnection)
	{		
		return;
	}

	strQuery = _T("select Navi_ID, Navi_Name, Min_X, Min_Y, Max_X, Max_y from navigation");

	KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

	while(spResultSet->Next())
	{
		//#1 Set Pos and Name
		HTREEITEM hItemNavi = m_wndTreeCtrl.InsertItem( spResultSet->GetValueString(1), 1, 1, a_hItemRoot );

		//#2 Set NaviItem Data
		TNaviTreeItemData* pTreeItemData = new TNaviTreeItemData;

		pTreeItemData->nNaviID     = spResultSet->GetValueInt(0);
		pTreeItemData->strNaviName = spResultSet->GetValueString(1);
		pTreeItemData->dMinX       = spResultSet->GetValueDouble(2);
		pTreeItemData->dMinY       = spResultSet->GetValueDouble(3);
		pTreeItemData->dMaxX       = spResultSet->GetValueDouble(4);
		pTreeItemData->dMaxY       = spResultSet->GetValueDouble(5);

		m_wndTreeCtrl.SetItemData(hItemNavi, (DWORD_PTR)pTreeItemData);
	}
}


void KNaviExplorerWnd::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	if( m_wndTreeCtrl.GetSafeHwnd() )
	{
		CRect rectClient;
		GetClientRect( &rectClient );

		m_wndTreeCtrl.SetWindowPos( NULL, rectClient.left, rectClient.top, 
			rectClient.Width(), rectClient.Height(), 
			SWP_NOACTIVATE | SWP_NOZORDER );
	}
}


void KNaviExplorerWnd::OnSetFocus( CWnd* pOldWnd )
{
	m_wndTreeCtrl.SetFocus();
}


BEGIN_MESSAGE_MAP(KNaviExplorerWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY( NM_DBLCLK, IDC_SED_TREECTRL, &OnNMDblCllkTreeCtrl)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_SED_TREECTRL, OnExpandedTreeCtrl)
END_MESSAGE_MAP()


void KNaviExplorerWnd::OnContextMenu( CWnd* pWnd, CPoint point )
{
	COfficeTreeCtrl* pTreeCtrl = (COfficeTreeCtrl*)&m_wndTreeCtrl;

	if (pWnd != pTreeCtrl)
		return;

	UINT nFlag(0);
	CPoint ptClient(point);

	pTreeCtrl->ScreenToClient(&ptClient);
	HTREEITEM hItem = pTreeCtrl->HitTest(ptClient, &nFlag);

	if (NULL == hItem)
		return;

	pTreeCtrl->SelectItem(hItem);

	TNaviTreeItemData* pData = (TNaviTreeItemData*)pTreeCtrl->GetItemData(hItem);

	if (NULL == pData)
	{
		ShowRootItemMenu(point);

	}
	else
	{
		ShowNaviItemMenu(point);
	}
}

#include "Scenario.h"
#include "MainFrameWnd.h"

void KNaviExplorerWnd::OnNMDblCllkTreeCtrl( NMHDR* pNMHDR, LRESULT *pResult )
{
	KMapView* pMapView = ImChampFrameWindow::GetActiveMapView();

	if (NULL == pMapView)
	{
		CKmzApp*                     pApp          = (CKmzApp*)(AfxGetApp());
		KProject*                    pProject      = pApp->GetProject();
		KScenario*                   pBaseScenario = pProject->GetBaseScenario();
		const KTDTargetMap&          oTargetMap    = pBaseScenario->GetTargetMap();
		KTDTargetMap::const_iterator begin         = oTargetMap.begin();
		KTDTargetMap::const_iterator end           = oTargetMap.end();
		KTarget*                     pTarget       = NULL;

		while ( end != begin )
		{
			pTarget = begin->second;
			++begin;
		}

		CMainFrameWnd* pMainFrame = (CMainFrameWnd*)AfxGetMainWnd();
		pMainFrame->ShowNetworkView( pTarget );

		pMapView = ImChampFrameWindow::GetActiveMapView();
	}

	HTREEITEM hItem;
	CPoint    ptMouse;
	::GetCursorPos( &ptMouse );
	m_wndTreeCtrl.ScreenToClient( &ptMouse );

	hItem = m_wndTreeCtrl.HitTest( ptMouse );

	if( NULL != hItem )
	{
		TNaviTreeItemData* pTreeItemData = (TNaviTreeItemData*)m_wndTreeCtrl.GetItemData( hItem );
		if( NULL != pTreeItemData )
		{
			pMapView->ZoomExtent(
				pTreeItemData->dMinX, pTreeItemData->dMinY,
				pTreeItemData->dMaxX, pTreeItemData->dMaxY);
		}
	}
}


void KNaviExplorerWnd::OnExpandedTreeCtrl( NMHDR* pNMHDR, LRESULT* pResult )
{
    return;
    /*
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item;
	item.mask = TVIF_HANDLE | TVIF_IMAGE;
	item.hItem = pNMTreeView->itemNew.hItem;
	m_wndTreeCtrl.GetItem( &item );
	int nImage =  -1;

	switch( item.iImage )
	{
	case 0 : nImage = 1; break;
	case 2 : nImage = 3; break;
	case 4 : nImage = 5; break;
	case 6 : nImage = 7; break;
	case 9 : nImage = 10; break;
	case 1 : nImage = 0; break;
	case 3 : nImage = 2; break;
	case 5 : nImage = 4; break;
	case 7 : nImage = 6; break;
	case 10 : nImage = 9; break;
	}

	if( -1 != nImage )
	{
		m_wndTreeCtrl.SetItemImage( item.hItem, nImage, nImage );
	}
    */
}


void KNaviExplorerWnd::ShowRootItemMenu( CPoint& point )
{
	TxLogDebugVisitor();
	CXTPMDIFrameWnd* pFrame   = (CXTPMDIFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCmdBars = pFrame->GetCommandBars();

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_NAVI);

	pCmdBars->TrackPopupMenuEx(menu.GetSubMenu(0), TPM_RIGHTBUTTON, point.x, point.y, pFrame);
}


void KNaviExplorerWnd::ShowNaviItemMenu( CPoint& point )
{
	TxLogDebugVisitor();
	CXTPMDIFrameWnd* pFrame   = (CXTPMDIFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCmdBars = pFrame->GetCommandBars();

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_NAVI);

	pCmdBars->TrackPopupMenuEx(menu.GetSubMenu(1), TPM_RIGHTBUTTON, point.x, point.y, pFrame);
}


const TNaviTreeItemData* KNaviExplorerWnd::GetSelectedTreeItemData( void )
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();

	if (NULL != hItem)
		return (TNaviTreeItemData*)m_wndTreeCtrl.GetItemData(hItem);

	return NULL;
}


void KNaviExplorerWnd::AddNaviItem( void )
{
	TxLogDebugStartMsg();
	KMapView* pMapView = ImChampFrameWindow::GetActiveMapView();

	if (NULL == pMapView)
	{
		AfxMessageBox(_T("지도창을 활성화 해주세요."));
		return;
	}

	KNaviItemAddDlg oDlg;

	if ( IDOK != oDlg.DoModal())
		return;

	CString strNaviName = oDlg.NaviName();
	double  dMinX, dMinY, dMaxX, dMaxY;

	pMapView->GetCurrentExtent(dMinX, dMinY, dMaxX, dMaxY);

	CString          strQuery(_T(""));
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (nullptr == spDBaseConnection)
	{		
		return;
	}

	int nNextNaviID(0);

	try
	{
		spDBaseConnection->BeginTransaction();

		//#1. Next NaviID 조회
		strQuery = _T("select max(navi_id)+1 from navigation");

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
			nNextNaviID = spResultSet->GetValueInt(0);

		//#2. Insert Data
		strQuery.Format(_T("insert into navigation values( %d, '%s', %f, %f, %f, %f)"),
			nNextNaviID, strNaviName, dMinX, dMinY, dMaxX, dMaxY);

		int nResult = spDBaseConnection->ExecuteUpdate(strQuery);

		if (SQLITE_DONE != nResult)
		{
			CString strMsg;
			strMsg.Format(_T("Sqlite error code : %d"), nResult);

			ThrowException(strMsg);
		}

		spDBaseConnection->Commit();
	}
	catch(KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return;
	}

	UpdateTree();

	TxLogDebugEndMsg();
}


void KNaviExplorerWnd::RemoveNaviItem( void )
{
	TxLogDebugStartMsg();

	const TNaviTreeItemData* pItemData = GetSelectedTreeItemData();

	if (NULL == pItemData)
		return;

	int nSelect = -1;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		nSelect = AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO);
	}
	else {
		nSelect = AfxMessageBox(_T("Are you sure want to delete?"), MB_YESNO);
	}

	if (IDNO == nSelect)
		return;

	int nNaviID = pItemData->nNaviID;

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (nullptr == spDBaseConnection)
	{		
		return;
	}

	try
	{
		CString strQuery;
		strQuery.Format(_T("delete from navigation where navi_id = %d"), nNaviID);

		spDBaseConnection->BeginTransaction();

		spDBaseConnection->ExecuteUpdate(strQuery);

		spDBaseConnection->Commit();
	}
	catch(KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return;
	}

	UpdateTree();

	TxLogDebugEndMsg();
}


void KNaviExplorerWnd::RenameNaviItem( void )
{
	TxLogDebugStartMsg();

	const TNaviTreeItemData* pItemData = GetSelectedTreeItemData();

	if (NULL == pItemData)
		return;

	KNaviItemRenameDlg oDlg;

	if (IDOK != oDlg.DoModal())
		return;

	CString strNewName = oDlg.NaviName();
	int     nNaviID    = pItemData->nNaviID;

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (nullptr == spDBaseConnection)
	{		
		return;
	}

	try
	{
		CString strQuery;
		strQuery.Format(_T("update navigation set Navi_Name = '%s' where navi_id = '%d'"), strNewName, nNaviID);

		spDBaseConnection->BeginTransaction();

		spDBaseConnection->ExecuteUpdate(strQuery);

		spDBaseConnection->Commit();
	}
	catch(KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return;
	}

	UpdateTree();

	TxLogDebugEndMsg();
}
