#include "StdAfx.h"
#include "KmzApp.h"

#include "MainFrameWnd.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"

#include "ProjectExplorerWnd.h"
#include "ImTasStatus.h"

KProjectExplorerWnd::KProjectExplorerWnd(void)
{
    
}

KProjectExplorerWnd::~KProjectExplorerWnd(void) {
    ClearProjectTree();
}

BEGIN_MESSAGE_MAP( KProjectExplorerWnd, CWnd )
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_SETFOCUS()
    ON_WM_CONTEXTMENU()
    ON_NOTIFY( NM_DBLCLK,        IDC_PROJECT_TREECTRL, &OnNMDblCllkTreeCtrl)
    ON_NOTIFY( TVN_ITEMEXPANDED, IDC_PROJECT_TREECTRL, OnExpandedTreeCtrl)
END_MESSAGE_MAP()


int KProjectExplorerWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if( CWnd::OnCreate( lpCreateStruct ) == -1 )
    {
        return -1;
    }

    if( !m_imgListProjectView.Create( 16, 16, ILC_MASK | ILC_COLOR24, 0, 1) )
    {
        AfxMessageBox( _T("Failed to create image list") );
        return -1;
    }

    CBitmap bmp; {
        bmp.LoadBitmap( IDB_PROJECTVIEW );
    }
    
    m_imgListProjectView.Add( &bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0) ) );
    
    if( m_oProjectTreeControl.GetSafeHwnd() == 0 ) {
        if( !m_oProjectTreeControl.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT,                                         
                                      CRect( 0, 0, 0, 0 ), this, IDC_PROJECT_TREECTRL ) )
        {
            TRACE0( "Unable to create tree control.\n" );
            return 0;
        }

        m_oProjectTreeControl.ModifyStyleEx( 0, WS_EX_STATICEDGE );
        m_oProjectTreeControl.SetImageList ( &m_imgListProjectView, TVSIL_NORMAL );
    }

    return 0;
}


void KProjectExplorerWnd::InsertProjectTree( void )
{
    KProject* pProject = ((CKmzApp*)AfxGetApp())->GetProject();

    ClearProjectTree();           

    if (pProject)
    {
        InsertTreeItemProjectInfo( pProject );
    }
    
    m_oProjectTreeControl.SortChildren(NULL);
    m_oProjectTreeControl.Invalidate();
    
    HTREEITEM hRoot = m_oProjectTreeControl.GetRootItem();
    if (hRoot)
    {
        m_oProjectTreeControl.SelectItem(hRoot);
    }
}


void KProjectExplorerWnd::ClearProjectTree( void )
{
	try
	{
		if (m_oProjectTreeControl.GetSafeHwnd() != NULL)
		{
			HTREEITEM hRoot = m_oProjectTreeControl.GetRootItem();
			if (hRoot != nullptr)
				RemoveTreeItem( hRoot );
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KProjectExplorerWnd::RemoveTreeItem( HTREEITEM hItem )
{
    if ( hItem == NULL ) return;

    HTREEITEM hNext;
    HTREEITEM hChild = m_oProjectTreeControl.GetChildItem( hItem );

    /// child item이 존재한다면 먼저 삭제
    while ( NULL != hChild )
    {
        hNext = m_oProjectTreeControl.GetNextItem( hChild, TVGN_NEXT );
        RemoveTreeItem( hChild );
        hChild = hNext;
    }

    /// tree에서 해당 Item 삭제
    TTreeItemData* pTreeData = (TTreeItemData*)m_oProjectTreeControl.GetItemData( hItem );
    KObject*       pObject   = pTreeData->Object;
        
    if (pTreeData->TreeItemType == KEMTreeItemTypeIODataTable)
    {
        KIOTable* pTable    = (KIOTable*)pObject;
        pTable->Unregister(this);
    }
    
    if( NULL != pTreeData )
    {
        delete pTreeData;
    }
        
    m_oProjectTreeControl.DeleteItem( hItem );
    m_oProjectTreeControl.SortChildren(NULL);
}


HTREEITEM KProjectExplorerWnd::InsertTreeItemProjectInfo( KProject* a_pProject )
{
    ASSERT( a_pProject );

    CString        strProjectName = a_pProject->GetName();
	KEMProjectType emProjectType  = a_pProject->GetProjectType();

	bool bKTDBBase = false;

    // 1. 프로젝트 Item 추가
    HTREEITEM      hItemProject  = m_oProjectTreeControl.InsertItem( strProjectName, 0, 0 );
    // 1.1 프로젝트 정보 추가
    TTreeItemData* pTreeItemData = new TTreeItemData;
    pTreeItemData->TreeItemType  = KEMTreeItemTypeProject;
    pTreeItemData->Object        = a_pProject;
    pTreeItemData->Name          = strProjectName;

    m_oProjectTreeControl.SetItemData( hItemProject, (DWORD_PTR)pTreeItemData );

    // 2. Base 시나리오 추가
    KScenario* pBaseScenario     = a_pProject->GetBaseScenario();
	
    if( NULL != pBaseScenario )
    {
        InsertTreeItemScenarioInfo( hItemProject, pBaseScenario, bKTDBBase );
    }

    const KTDScenarioMap& oScenarioMap  = a_pProject->GetScenarioMap();
    KTDScenarioMap::const_iterator iter = oScenarioMap.begin();
    KTDScenarioMap::const_iterator end  = oScenarioMap.end();

    while( iter != end )
    {
        KScenario* pScenario = iter->second;
        InsertTreeItemScenarioInfo( hItemProject, pScenario, bKTDBBase );
        ++iter;
    }

    return hItemProject;
}


HTREEITEM KProjectExplorerWnd::InsertTreeItemScenarioInfo(HTREEITEM a_hItemProject, KScenario* a_pScenario, bool a_bKTDBBase )
{
    ASSERT( NULL != a_hItemProject );
    ASSERT( NULL != a_pScenario );

    // 시나리오 정보 추가
    unsigned int         nID = a_pScenario->GetObjectID();
    CString  strScenarioName = a_pScenario->GetName();

    int nImageIndex = 4;	 
    if(a_pScenario->IsBaseScenario() == true)
    {
        nImageIndex = 2;	
    }
        
    HTREEITEM hItemScenario = NULL;
    hItemScenario = m_oProjectTreeControl.InsertItem( strScenarioName, nImageIndex, nImageIndex+1, a_hItemProject );	

    TTreeItemData* pItemData = new TTreeItemData;
	pItemData->TreeItemType  = KEMTreeItemTypeUserDefineProjectScenario;
	
    pItemData->Object    = a_pScenario;
    pItemData->Name      = strScenarioName;

    m_oProjectTreeControl.SetItemData( hItemScenario, (DWORD_PTR)pItemData );

    const KTDTargetMap&     oTargetMap = a_pScenario->GetTargetMap();
    KTDTargetMap::const_iterator begin = oTargetMap.begin();
    KTDTargetMap::const_iterator end   = oTargetMap.end();
    while( end != begin )
    {
        InsertTreeItemTargetInfo( hItemScenario, begin->second, a_bKTDBBase );
        ++begin;
    }

    m_oProjectTreeControl.SortChildren( hItemScenario );
    m_oProjectTreeControl.Expand( a_hItemProject, TVE_EXPAND );
    return hItemScenario;
}


HTREEITEM KProjectExplorerWnd::InsertTreeItemTargetInfo( HTREEITEM a_hItemScenario, KTarget* a_pTarget,  bool a_bKTDBBase )
{
    ASSERT( NULL != a_hItemScenario );
    ASSERT( NULL != a_pTarget );

    try
    {
        // Target 정보 추가
        unsigned int nID       = a_pTarget->GetObjectID();
        CString  strTargetName = a_pTarget->GetName();

        // 네트워크 정보 추가
        HTREEITEM hItemTarget  = m_oProjectTreeControl.InsertItem( strTargetName, 6, 7, a_hItemScenario );

        TTreeItemData*  pItemData  = new TTreeItemData;
		if(a_bKTDBBase == true )
		{
			pItemData->TreeItemType  = KEMTreeItemTypeKTDBProjectTarget;
		}
		else
		{
			pItemData->TreeItemType  = KEMTreeItemTypeUserDefineProjectTarget;
		}
        pItemData->Object          = a_pTarget;
        pItemData->Name            = strTargetName;

        m_oProjectTreeControl.SetItemData( hItemTarget, (DWORD_PTR)pItemData );

        if( a_pTarget->GetDBaseConnection() != nullptr )
        {
            InsertTreeItemNetworkData( hItemTarget );
            InsertTreeItemIODataInfo( hItemTarget, a_pTarget );                
        }

        m_oProjectTreeControl.SortChildren(a_hItemScenario);
        m_oProjectTreeControl.Expand( a_hItemScenario, TVE_EXPAND );

        return hItemTarget;
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
    return nullptr;
}


HTREEITEM KProjectExplorerWnd::InsertTreeItemNetworkData( HTREEITEM a_hItemTarget )
{
    CString    strNetwork;
    strNetwork.LoadString( IDS_PROJECTVIEW_NETWORK );
    HTREEITEM  hItemNetwork  = m_oProjectTreeControl.InsertItem( strNetwork, 8, 8, a_hItemTarget );

    TTreeItemData* pItemData = new TTreeItemData;
    pItemData->TreeItemType  = KEMTreeItemTypeNetworkData;
    pItemData->Object        = NULL;

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pItemData->Name = _T("지도");
    }
    else {
        pItemData->Name = _T("Map");
    }
    
    m_oProjectTreeControl.SetItemData( hItemNetwork, (DWORD_PTR)pItemData );
    m_oProjectTreeControl.Expand( a_hItemTarget, TVE_EXPAND );

    return hItemNetwork;
}


void KProjectExplorerWnd::InsertTreeItemIODataInfo( HTREEITEM a_hItemTarget, KTarget* a_pTarget )
{
    //CString   strIOData;
    //strIOData.LoadString( IDS_PROJECTVIEW_IODATA );

    KIOTables*  pIOTables = a_pTarget->Tables();
    int nTableCount = pIOTables->GetCount();

    std::map<KEMIOTableDisplayType, std::vector<KIOTable*>> mapTable;
    for(int i=0; i<nTableCount; ++i)
    {
        KIOTable* pTable       = pIOTables->GetTableAt(i);

        KEMIOTableDisplayType emIOTableDisplayType = pTable->TableDisplayType();
        std::map<KEMIOTableDisplayType, std::vector<KIOTable*>>::iterator iter = mapTable.find(emIOTableDisplayType);
        std::map<KEMIOTableDisplayType, std::vector<KIOTable*>>::iterator end  = mapTable.end();

        if (iter != end)
        {
            std::vector<KIOTable*>& vecTable = iter->second;
            vecTable.push_back(pTable);
        }
        else
        {
            std::vector<KIOTable*> vecTable;
            vecTable.push_back(pTable);
            mapTable.insert(std::make_pair(emIOTableDisplayType, vecTable));
        }
    }   

    std::map<KEMIOTableDisplayType, std::vector<KIOTable*>>::iterator iter = mapTable.begin();
    std::map<KEMIOTableDisplayType, std::vector<KIOTable*>>::iterator end  = mapTable.end();

    while (iter != end)
    {
        KEMIOTableDisplayType   emIOTableDisplayType = iter->first;
        std::vector<KIOTable*>& vecTable             = iter->second;
        int                     nCount               = vecTable.size();

        HTREEITEM      hItemIOFolder = NULL;
        TTreeItemData* pItemData     = new TTreeItemData;
        pItemData->TreeItemType      = KEMTreeItemTypeIODataFolder;
        pItemData->Object            = NULL;
        pItemData->Name              = _T("IODATA");

        switch (emIOTableDisplayType)
        {
		case  KEMIOTableDisplayTypeSystemManager:
			++iter;
			continue;

        case KEMIOTableDisplayTypeNetwork:
            if (nCount > 1) {
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("네트워크 데이터"), 9, 10, a_hItemTarget );
                }
                else {
                    hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("Network"), 9, 10, a_hItemTarget );
                }                
            }
            break;
        case KEMIOTableDisplayTypeMatrix:
            if (nCount > 1) {
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("기종점 통행 데이터"),  9, 10, a_hItemTarget );
                }
                else {
                    hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("Trip Matrix"),  9, 10, a_hItemTarget );
                }                
            }
            break;
		case KEMIOTableDisplayTypeParameter:
            if (nCount > 1) {
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("Attribute"),  9, 10, a_hItemTarget );
                }
                else {
                    hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("Attribute"),  9, 10, a_hItemTarget );
                }			    
            }
			break;
        case KEMIOTableDisplayTypeSED:
            if (nCount > 1) {
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("사회경제지표 데이터"),     9, 10, a_hItemTarget );
                }
                else {
                    hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("SED"),     9, 10, a_hItemTarget );
                }
            }
            break;
        default:
            if (nCount > 1) {
                hItemIOFolder = m_oProjectTreeControl.InsertItem( _T("ETC"),     9, 10, a_hItemTarget );
            }
            break;
        }
        
        m_oProjectTreeControl.SetItemData( hItemIOFolder, (DWORD_PTR)pItemData );

        // 자식을 입력
        for (int i=0; i<nCount; i++)
        {
            KIOTable* pTable    = vecTable[i];
            int nImage = 11;
            if (pTable->RecordCount() > 0)
            {
                nImage = 12;
            }

            HTREEITEM hIODataTable;
            if (nCount > 1)
            {
                hIODataTable = m_oProjectTreeControl.InsertItem(pTable->Caption(), nImage, nImage, hItemIOFolder);
            }
            else
            {
                hIODataTable = m_oProjectTreeControl.InsertItem(pTable->Caption(), nImage, nImage, a_hItemTarget);
            }

            TTreeItemData* pTableData = new TTreeItemData;
            pTableData->TreeItemType  = KEMTreeItemTypeIODataTable;
            pTableData->Object        = NULL;
            pTableData->m_pIOTable    = pTable;
            pTableData->Name          = pTable->Name();

            m_oProjectTreeControl.SetItemData(hIODataTable, (DWORD_PTR)pTableData);

            pTable->Register(this);
        }

        m_oProjectTreeControl.Expand( hItemIOFolder,   TVE_EXPAND );

        ++iter;
    }
        
    m_oProjectTreeControl.Expand( a_hItemTarget, TVE_EXPAND );
}

void KProjectExplorerWnd::OnSize( UINT nType, int cx, int cy )
{
    CWnd::OnSize( nType, cx, cy );
    if( m_oProjectTreeControl.GetSafeHwnd() )
    {
        CRect rectClient;
        GetClientRect( &rectClient );

        m_oProjectTreeControl.SetWindowPos( NULL, rectClient.left,    rectClient.top, 
                                                  rectClient.Width(), rectClient.Height(), 
                                       SWP_NOACTIVATE | SWP_NOZORDER );
    }
}


void KProjectExplorerWnd::OnSetFocus( CWnd* pOldWnd )
{
    m_oProjectTreeControl.SetFocus();
}


void KProjectExplorerWnd::OnContextMenu( CWnd* pWnd, CPoint point )
{
    COfficeTreeCtrl* pProjectTreeControl = (COfficeTreeCtrl*) &m_oProjectTreeControl;    
         if( pWnd != pProjectTreeControl ) return;

    UINT   nFlag(0);
    CPoint ptClient(point);

    pProjectTreeControl->ScreenToClient( &ptClient );
    HTREEITEM       hItem = pProjectTreeControl->HitTest( ptClient, &nFlag );
        if( NULL == hItem ) return;

    pProjectTreeControl->SelectItem( hItem );
    TTreeItemData*  pData = (TTreeItemData*)pProjectTreeControl->GetItemData( hItem );
        if( NULL == pData ) return;

    switch( pData->TreeItemType )
    {
    case KEMTreeItemTypeProject : 
        ShowProjectMenu( point );
        break;
	case KEMTreeItemTypeUserDefineProjectScenario :
		ShowScenarioMenu( point, false );
		break;
	case KEMTreeItemTypeKTDBProjectTarget :
        ShowTargetMenu( point, true );
        break;
	case KEMTreeItemTypeUserDefineProjectTarget :
		ShowTargetMenu( point, false );
		break;
	case KEMTreeItemTypeNetworkData :
    case KEMTreeItemTypeIODataTable :
        /*ShowTableMenu( point );*/
        break;
    }
}


void KProjectExplorerWnd::ShowProjectMenu( CPoint& point )
{
    CXTPMDIFrameWnd* pFrame       = (CXTPMDIFrameWnd*)AfxGetMainWnd();
    CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

    CMenu menu;
    menu.LoadMenu( IDR_POPUP_PROJECT );

    pCommandBars->TrackPopupMenuEx( menu.GetSubMenu(0), TPM_RIGHTBUTTON, point.x, point.y, pFrame );
}


void KProjectExplorerWnd::ShowScenarioMenu( CPoint& point, bool a_bKTDBBase )
{   
    CXTPMDIFrameWnd* pFrame       = (CXTPMDIFrameWnd*)AfxGetMainWnd();
    CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

    CMenu menu;
	menu.LoadMenu( IDR_POPUP_SCENARIO_USER );

    pCommandBars->TrackPopupMenuEx( menu.GetSubMenu(0), TPM_RIGHTBUTTON, point.x, point.y, pFrame );
}


void KProjectExplorerWnd::ShowTargetMenu( CPoint& point, bool a_bKTDBBase )
{    
    CXTPMDIFrameWnd* pFrame       = (CXTPMDIFrameWnd*)AfxGetMainWnd();
    CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

    CMenu menu;
	if( true == a_bKTDBBase )
	{
		menu.LoadMenu( IDR_POPUP_TARGET );
	}
	else 
	{
		menu.LoadMenu( IDR_POPUP_TARGET_USER );
	}

    pCommandBars->TrackPopupMenuEx( menu.GetSubMenu(0), TPM_RIGHTBUTTON, point.x, point.y, pFrame );
}


void KProjectExplorerWnd::ShowTableMenu( CPoint& point )
{
    CXTPMDIFrameWnd* pFrame       = (CXTPMDIFrameWnd*)AfxGetMainWnd();
    CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

    CMenu menu;
    menu.LoadMenu( IDR_POPUP_TABLE );
    pCommandBars->TrackPopupMenuEx( menu.GetSubMenu(0), TPM_RIGHTBUTTON, point.x, point.y, pFrame );
}


void KProjectExplorerWnd::OnNMDblCllkTreeCtrl( NMHDR* pNMHDR, LRESULT* pResult )
{
    HTREEITEM hItem;
    CPoint    ptMouse;
    ::GetCursorPos( &ptMouse );
    m_oProjectTreeControl.ScreenToClient( &ptMouse );
    hItem = m_oProjectTreeControl.HitTest( ptMouse );

    if( NULL != hItem )
    {
        TTreeItemData* pTreeItemData = (TTreeItemData*)m_oProjectTreeControl.GetItemData( hItem );
        if( NULL != pTreeItemData )
        {
            if( KEMTreeItemTypeNetworkData == pTreeItemData->TreeItemType )
            {
                DblClickItemNetwork( hItem, pTreeItemData );
            }
            else if( KEMTreeItemTypeIODataTable == pTreeItemData->TreeItemType )
            {
                DblClickItemTable( hItem, pTreeItemData );
            }
        }
    }

    *pResult = 0;
}


void KProjectExplorerWnd::DblClickItemNetwork( HTREEITEM hItem, TTreeItemData* pItemData )
{
    HTREEITEM      hParentTreeItem;
    TTreeItemData* pParentTreeItemData;

    hParentTreeItem = m_oProjectTreeControl.GetParentItem( hItem );

    while( NULL != hParentTreeItem )
    {
        // 자신의 상위 검색 : Target일때 Stop
        pParentTreeItemData = (TTreeItemData*)m_oProjectTreeControl.GetItemData( hParentTreeItem );

        if( KEMTreeItemTypeKTDBProjectTarget == pParentTreeItemData->TreeItemType || KEMTreeItemTypeUserDefineProjectTarget == pParentTreeItemData->TreeItemType )
        {
            break;
        }

        hParentTreeItem = m_oProjectTreeControl.GetParentItem( hParentTreeItem );
    }

	KProject*      pProject       = ((CKmzApp*)AfxGetApp())->GetProject();
	KEMProjectType emProjectType  = pProject->GetProjectType();

	bool bKTDBBase = false;

	if (bKTDBBase)
		ASSERT( KEMTreeItemTypeKTDBProjectTarget == pParentTreeItemData->TreeItemType );
	else
		ASSERT( KEMTreeItemTypeUserDefineProjectTarget == pParentTreeItemData->TreeItemType );

    KTarget*       pTarget    = (KTarget*)pParentTreeItemData->Object;
    CMainFrameWnd* pMainFrame = (CMainFrameWnd*)AfxGetMainWnd();

    pMainFrame->ShowNetworkView( pTarget );
}

void KProjectExplorerWnd::DblClickItemTable( HTREEITEM hItem, TTreeItemData* pItemData )
{    
    HTREEITEM      hParentTreeItem;
    TTreeItemData* pParentTreeItemData;

    hParentTreeItem = m_oProjectTreeControl.GetParentItem( hItem );

    while( NULL != hParentTreeItem )
    {
        // 자신의 상위 검색 : Target일때 Stop
        pParentTreeItemData = (TTreeItemData*)m_oProjectTreeControl.GetItemData( hParentTreeItem );

        if( KEMTreeItemTypeKTDBProjectTarget == pParentTreeItemData->TreeItemType || KEMTreeItemTypeUserDefineProjectTarget == pParentTreeItemData->TreeItemType )
        {
            break;
        }

        hParentTreeItem = m_oProjectTreeControl.GetParentItem( hParentTreeItem );
    }

	KProject*      pProject       = ((CKmzApp*)AfxGetApp())->GetProject();
	KEMProjectType emProjectType  = pProject->GetProjectType();

	bool bKTDBBase = false;

	if (bKTDBBase)
		ASSERT( KEMTreeItemTypeKTDBProjectTarget == pParentTreeItemData->TreeItemType );
	else
		ASSERT( KEMTreeItemTypeUserDefineProjectTarget == pParentTreeItemData->TreeItemType );

    KTarget*       pTarget    = (KTarget*)pParentTreeItemData->Object;
    CMainFrameWnd* pMainFrame = (CMainFrameWnd*)AfxGetMainWnd();

    pMainFrame->ShowTableView( pTarget, pItemData->Name );
}

void KProjectExplorerWnd::OnExpandedTreeCtrl( NMHDR* pNMHDR, LRESULT* pResult )
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    TVITEM item;
    item.mask = TVIF_HANDLE | TVIF_IMAGE;
    item.hItem = pNMTreeView->itemNew.hItem;
    m_oProjectTreeControl.GetItem( &item );
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
        m_oProjectTreeControl.SetItemImage( item.hItem, nImage, nImage );
    }
}

const TTreeItemData* KProjectExplorerWnd::GetSelectedTreeItemData( void )
{
    HTREEITEM hItem = m_oProjectTreeControl.GetSelectedItem();

    if( NULL != hItem )
    {
        return (TTreeItemData*)m_oProjectTreeControl.GetItemData( hItem );
    }

    return NULL;
}

void KProjectExplorerWnd::NotifyProcess( LPCTSTR a_strSubjectName/*=_T("")*/, Integer a_nxObjectID/*=0*/ )
{
    HTREEITEM hRoot = m_oProjectTreeControl.GetRootItem();
    if (hRoot != nullptr)
        UpdateTableInfo( hRoot, a_strSubjectName, a_nxObjectID );
}

void KProjectExplorerWnd::UpdateTableInfo( HTREEITEM hItem, LPCTSTR a_strSubjectName, Integer a_nxObjectID )
{
    if ( hItem == NULL ) return;

    HTREEITEM hNext  = NULL;
    HTREEITEM hChild = m_oProjectTreeControl.GetChildItem( hItem );
    while ( NULL != hChild )
    {
        hNext = m_oProjectTreeControl.GetNextItem( hChild, TVGN_NEXT );
        UpdateTableInfo( hChild, a_strSubjectName, a_nxObjectID );
        hChild = hNext;
    }
        
    TTreeItemData* pTreeData = (TTreeItemData*)m_oProjectTreeControl.GetItemData( hItem );
    KIOTable*      pTable    = pTreeData->m_pIOTable;

    if (pTable == nullptr)
        return;

    Integer nxObjectID = pTable->GetObjectID();
    CString strName    = pTable->Name();

    if (pTreeData->TreeItemType != KEMTreeItemTypeIODataTable)
        return;

    if (nxObjectID != a_nxObjectID)
        return;

    if (_tcsicmp(strName, a_strSubjectName) != 0)
        return;
        
    int nImage = 11;
    if (pTable->RecordCount() > 0)
    {
        nImage = 12;
    }

    m_oProjectTreeControl.SetItemImage(hItem, nImage, nImage);
}

void KProjectExplorerWnd::RedrawTreeItemData( void )
{
	KProject*      pProject       = ((CKmzApp*)AfxGetApp())->GetProject();
	KScenario*     pBaseScenario  = pProject->GetBaseScenario();
	KEMProjectType emProjectType  = pProject->GetProjectType();
	
	bool bKTDBBase = false;

	//#0 - KProject와 다른 정보를 삭제함
	//Tree에는 정보가 존재하나 project, scenario, Target에 정보가 없을경우 삭제
	//#1 - KProject의 정보가 Tree에 존재하지 않을 시 해당 정보를 Add함

	HTREEITEM hRoot  = m_oProjectTreeControl.GetRootItem();
	
	RemoveDisuseScenarioTreeItem(hRoot, pProject);
	AppendNewScenarioTreeItem(hRoot, pProject, bKTDBBase);
	UpdateTreeItemName(hRoot);
}

void KProjectExplorerWnd::RemoveDisuseScenarioTreeItem( HTREEITEM a_hItemProject, KProject* a_pProject )
{
	KScenario* pBaseScenario = a_pProject->GetBaseScenario();

	HTREEITEM hChild = m_oProjectTreeControl.GetChildItem( a_hItemProject );
	HTREEITEM hNext  = nullptr;
	bool      bUsed;

	while (nullptr != hChild)
	{
		bUsed = false;

		TTreeItemData* pTreeData     = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hChild);
		KScenario*     pTreeScenario = (KScenario*)pTreeData->Object;

		if (pBaseScenario == pTreeScenario)
		{
			bUsed = true;
		}
		else
		{
			const KTDScenarioMap& scenarioMap = a_pProject->GetScenarioMap();
			KTDScenarioMap::const_iterator bIter, eIter = scenarioMap.end();

			for (bIter = scenarioMap.begin(); bIter != eIter; ++bIter)
			{
				if (pTreeScenario == bIter->second)
				{
					bUsed = true;
					break;
				}
			}
		}

		if (!bUsed)
			RemoveTreeItem(hChild);
		else
			RemoveDisuseTargetTreeItem(hChild, pTreeScenario);

		hChild = m_oProjectTreeControl.GetNextItem(hChild, TVGN_NEXT);
	}
}

void KProjectExplorerWnd::RemoveDisuseTargetTreeItem( HTREEITEM a_hItemScenario, KScenario* a_pScenario )
{
	HTREEITEM hChild = m_oProjectTreeControl.GetChildItem( a_hItemScenario );
	HTREEITEM hNext  = nullptr;
	bool      bUsed;

	const KTDTargetMap&          targetMap    = a_pScenario->GetTargetMap();
	KTDTargetMap::const_iterator bIter, eIter = targetMap.end();

	while (nullptr != hChild)
	{
		bUsed = false;

		TTreeItemData* pTreeData   = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hChild);
		KTarget*       pTreeTarget = (KTarget*)pTreeData->Object;

		for (bIter = targetMap.begin(); bIter != eIter; ++bIter)
		{
			if (pTreeTarget == bIter->second)
			{
				bUsed = true;
				break;
			}
		}

		if (!bUsed)
			RemoveTreeItem(hChild);

		hChild = m_oProjectTreeControl.GetNextItem(hChild, TVGN_NEXT);
	}
}

void KProjectExplorerWnd::AppendNewScenarioTreeItem( HTREEITEM a_hItemProject, KProject* a_pProject, bool a_bKTDBBase )
{
	KScenario* pBaseScenario = a_pProject->GetBaseScenario();
	
	const KTDScenarioMap&          scenarioMap  = a_pProject->GetScenarioMap();
	KTDScenarioMap::const_iterator bIter, eIter = scenarioMap.end();

	//Tree에 Base Scenario가 있는지 체크;
	HTREEITEM hChild = m_oProjectTreeControl.GetChildItem(a_hItemProject);
	bool      bUsed  = false;

	while (hChild != nullptr)
	{
		TTreeItemData* pTreeData     = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hChild);
		KScenario*     pTreeScenario = (KScenario*)pTreeData->Object;

		if (pTreeScenario == pBaseScenario)
		{
			bUsed = true;
			break;
		}

		hChild = m_oProjectTreeControl.GetNextItem(hChild, TVGN_NEXT);
	}

	if (!bUsed)
		InsertTreeItemScenarioInfo(a_hItemProject, pBaseScenario, a_bKTDBBase);

	//Tree에서 Base를 제외한 Scenario가 있는지 체크함. 없을경우 추가
	for (bIter = scenarioMap.begin(); bIter != eIter; ++bIter)
	{
		KScenario* pScenario = bIter->second;
		bUsed = false;

		hChild = m_oProjectTreeControl.GetChildItem(a_hItemProject);

		while (hChild != nullptr)
		{
			TTreeItemData* pTreeData     = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hChild);
			KScenario*     pTreeScenario = (KScenario*)pTreeData->Object;

			if (pTreeScenario == pScenario)
			{
				bUsed = true;
				break;
			}

			hChild = m_oProjectTreeControl.GetNextItem(hChild, TVGN_NEXT);
		}

		if (!bUsed)
		{
			InsertTreeItemScenarioInfo(a_hItemProject, pScenario, a_bKTDBBase);
		}
	}

	//전체 Senario에 대해서 신규Target이 있으면 추가함;
	hChild = m_oProjectTreeControl.GetChildItem(a_hItemProject);

	while (hChild != nullptr)
	{
		TTreeItemData* pTreeData     = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hChild);
		KScenario*     pTreeScenario = (KScenario*)pTreeData->Object;

		AppendNewTargetTreeItem(hChild, pTreeScenario, a_bKTDBBase);

		hChild = m_oProjectTreeControl.GetNextItem(hChild, TVGN_NEXT);
	}
}

void KProjectExplorerWnd::AppendNewTargetTreeItem( HTREEITEM a_hItemScenario, KScenario* a_pScenario, bool a_bKTDBBase )
{
	const KTDTargetMap&          targetMap    = a_pScenario->GetTargetMap();
	KTDTargetMap::const_iterator bIter, eIter = targetMap.end();

	for (bIter = targetMap.begin(); bIter != eIter; ++bIter)
	{
		KTarget*  pTarget = bIter->second;
		HTREEITEM hChild  = m_oProjectTreeControl.GetChildItem(a_hItemScenario);
		bool      bUsed   = false;

		while (hChild != nullptr)
		{
			TTreeItemData* pTreeData   = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hChild);
			KTarget*       pTreeTarget = (KTarget*)pTreeData->Object;

			if (pTarget == pTreeTarget)
			{
				bUsed = true;
				break;
			}

			hChild = m_oProjectTreeControl.GetNextItem(hChild, TVGN_NEXT);
		}

		if (!bUsed)
		{
			InsertTreeItemTargetInfo(a_hItemScenario, pTarget, a_bKTDBBase);
		}
	}

	//Target정보는 있었으나, Map/DB 정보가 없어서 TargetYear만 표시되었을 시 하위 트리정보를 Update해줌
	HTREEITEM hChild = m_oProjectTreeControl.GetChildItem(a_hItemScenario);

	while (hChild != nullptr)
	{
		TTreeItemData* pTreeData   = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hChild);
		KTarget*       pTreeTarget = (KTarget*)pTreeData->Object;

		HTREEITEM hCChild = m_oProjectTreeControl.GetChildItem(hChild);

		if (hCChild == nullptr)
		{
			if (pTreeTarget->GetDBaseConnection() != nullptr) {
				InsertTreeItemNetworkData(hChild);
                InsertTreeItemIODataInfo(hChild, pTreeTarget);
            }
		}

		hChild = m_oProjectTreeControl.GetNextItem(hChild, TVGN_NEXT);
	}
}

void KProjectExplorerWnd::UpdateTreeItemName( HTREEITEM a_hItemProject )
{
	CString   strValue;

	TTreeItemData* pTreeData = (TTreeItemData*)m_oProjectTreeControl.GetItemData(a_hItemProject);
	KProject*      pProject  = (KProject*)pTreeData->Object;
	strValue = pProject->GetName();

	if (strValue.CompareNoCase(pTreeData->Name) != 0)
	{
		pTreeData->Name = strValue;
		m_oProjectTreeControl.SetItemText(a_hItemProject, strValue);
	}	

	HTREEITEM hChild = m_oProjectTreeControl.GetChildItem(a_hItemProject);
	

	while (hChild != nullptr)
	{
		TTreeItemData* pTreeData1 = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hChild);
		KScenario*     pScenario  = (KScenario*)pTreeData1->Object;
		
		strValue = pScenario->GetName();
		
		if (strValue.CompareNoCase(pTreeData1->Name) != 0)
		{
			pTreeData1->Name = strValue;
			m_oProjectTreeControl.SetItemText(hChild, strValue);
		}		

		HTREEITEM hCChild = m_oProjectTreeControl.GetChildItem(hChild);

		while (hCChild != nullptr)
		{
			TTreeItemData* pTreeData2 = (TTreeItemData*)m_oProjectTreeControl.GetItemData(hCChild);
			KTarget*       pTarget    = (KTarget*)pTreeData2->Object;

			strValue = pTarget->GetName();

			if (strValue.CompareNoCase(pTreeData2->Name) != 0)
			{
				pTreeData2->Name = strValue;
				m_oProjectTreeControl.SetItemText(hCChild, strValue);
			}

			hCChild = m_oProjectTreeControl.GetNextItem(hCChild, TVGN_NEXT);
		}

		m_oProjectTreeControl.SortChildren(hChild);
		hChild = m_oProjectTreeControl.GetNextItem(hChild, TVGN_NEXT);
	}
}


// void KProjectExplorerWnd::UpdateScenarioTree( HTREEITEM hProject,  KProject* pProject )
// {
// 	std::map< void*, HTREEITEM > itemMap;
// 	std::map< void*, HTREEITEM >::iterator itItem;
// 	HTREEITEM hItem = m_oProjectTreeControl.GetChildItem( hProject );
// 	while( NULL != hItem )
// 	{
// 		TTreeItemData* pItemData = (TTreeItemData*)m_oProjectTreeControl.GetItemData( hItem );
// 		if( (NULL == pItemData) || (NULL == pItemData->BaseObject) )
// 		{
// 			RemoveTreeItem(hItem );
// 		}
// 		else
// 		{
// 			itemMap.insert( std::make_pair( pItemData->BaseObject, hItem ) );
// 		}
// 		hItem = m_oProjectTreeControl.GetNextItem( hItem, TVGN_NEXT );
// 	}
// 
// 	/// base scenario insert/update
// 	KScenario* pBaseScenario = pProject->GetBaseScenario();
// 	if( NULL != pBaseScenario )
// 	{
// 		HTREEITEM hBaseScenario;
// 		itItem = itemMap.find( pBaseScenario );
// 		if( itemMap.end() != itItem )
// 		{
// 			hBaseScenario = itItem->second;
// 			CString strName = m_oProjectTreeControl.GetItemText( hBaseScenario );
// 			if( pBaseScenario->GetName() != strName )
// 			{
// 				m_oProjectTreeControl.SetItemText( hBaseScenario, pBaseScenario->GetName() );
// 			}
// 			UpdateTargetTree( hBaseScenario, pBaseScenario );
// 			itemMap.erase( itItem );
// 		}
// 		else
// 		{
// 			hBaseScenario = InsertTreeItemScenarioInfo( hProject, pBaseScenario );
// 		}
// 		m_oProjectTreeControl.SortChildren( hBaseScenario );
// 	}
// 
// 	/// scenario insert/update
// 	const KTDScenarioMap& scenarioMap = pProject->GetScenarioMap();
// 	KTDScenarioMap::const_iterator citScenarioMap = scenarioMap.begin();
// 	while( scenarioMap.end() != citScenarioMap )
// 	{
// 		HTREEITEM hScenario;
// 		itItem = itemMap.find( citScenarioMap->second );
// 		if( itemMap.end() != itItem )
// 		{
// 			hScenario = itItem->second;
// 			CString strName = m_oProjectTreeControl.GetItemText( hScenario );
// 			if( citScenarioMap->second->GetName() != strName )
// 			{
// 				m_oProjectTreeControl.SetItemText( hScenario, citScenarioMap->second->GetName() );
// 			}
// 
// 			UpdateTargetTree( hScenario, citScenarioMap->second );
// 			itemMap.erase( itItem );
// 		}
// 		else
// 		{
// 			hScenario = InsertTreeItemScenarioInfo( hProject, citScenarioMap->second );
// 		}
// 		++citScenarioMap;
// 		m_oProjectTreeControl.SortChildren( hScenario );
// 	}
// 
// 	/// remove not exist scenario
// 	itItem = itemMap.begin();
// 	while( itemMap.end() != itItem )
// 	{
// 		RemoveTreeItem( itItem->second );
// 		++itItem;
// 	}
// }
// 
// 
// void KProjectExplorerWnd::UpdateTargetTree( HTREEITEM hScenario, KScenario* pScenario )
// {
// 	std::map< void*, HTREEITEM > itemMap;
// 	std::map< void*, HTREEITEM >::iterator itItem;
// 
// 	HTREEITEM hItem = m_oProjectTreeControl.GetChildItem( hScenario );
// 	while( NULL != hItem )
// 	{
// 		TTreeItemData* pItemData = (TTreeItemData*)m_oProjectTreeControl.GetItemData( hItem );
// 		if( (NULL == pItemData) || (NULL == pItemData->BaseObject) )
// 		{
// 			RemoveTreeItem( hItem );
// 		}
// 		else
// 		{
// 			itemMap.insert( std::make_pair( pItemData->BaseObject, hItem ) );
// 		}
// 		hItem = m_oProjectTreeControl.GetNextItem( hItem, TVGN_NEXT );
// 	}
// 
// 	/// update/insert target 
// 	const KTDTargetMap& targetMap = pScenario->GetTargetMap();
// 	KTDTargetMap::const_iterator citTargetMap = targetMap.begin();
// 	while( targetMap.end() != citTargetMap )
// 	{
// 		itItem = itemMap.find( citTargetMap->second );
// 		if( itemMap.end() != itItem )
// 		{
// 			CString strName = m_oProjectTreeControl.GetItemText( itItem->second );
// 			if( citTargetMap->second->GetName() != strName )
// 			{
// 				m_oProjectTreeControl.SetItemText( itItem->second, citTargetMap->second->GetName() );
// 			}
// 			UpdateTargetItem( itItem->second, citTargetMap->second );
// 			itemMap.erase( itItem );
// 		}
// 		else
// 		{
// 			InsertTreeItemTargetInfo( hScenario, citTargetMap->second );
// 		}
// 		++citTargetMap;
// 	}
// 
// 	/// remove not exist target
// 	itItem = itemMap.begin();
// 	while( itemMap.end() != itItem )
// 	{
// 		RemoveTreeItem( itItem->second );
// 		++itItem;
// 	}
// 
// 	m_oProjectTreeControl.SortChildren( hScenario );
// }
// 
// 
// void KProjectExplorerWnd::UpdateTargetItem( HTREEITEM hTarget, KTarget* pTarget )
// {
// 	std::map< CString, HTREEITEM > itemMap;
// 	std::map< CString, HTREEITEM >::iterator itItem;
// 
// 	HTREEITEM hItem = m_oProjectTreeControl.GetChildItem( hTarget );
// 	while( NULL != hItem )
// 	{
// 		CString strItem = m_oProjectTreeControl.GetItemText( hItem );
// 		itemMap.insert( std::make_pair( strItem, hItem ) );
// 		hItem = m_oProjectTreeControl.GetNextItem( hItem, TVGN_NEXT );
// 	}
// 
// 	/// insert network item
// 	CString strNetwork;
// 	strNetwork.LoadString( IDS_PROJECTVIEW_NETWORK );
// 	if( pTarget->GetNetworkDataFileName().IsEmpty() == false )
// 	{
// 		itItem = itemMap.find( strNetwork );
// 		if( itemMap.end() == itItem )
// 		{
// 			InsertTreeItemNetworkData( hTarget );
// 		}
// 		else
// 		{
// 			itemMap.erase( itItem );
// 		}
// 	}
// 
// 	/// update/insert IO data item
// 	CString strIOData;
// 	strIOData.LoadString( IDS_PROJECTVIEW_IODATA );
// 	if( pTarget->GetDBaseConnection() != nullptr )
// 	{
// 		itItem = itemMap.find( strIOData );
// 		if( itemMap.end() == itItem )
// 		{
// 			InsertTreeItemIODataInfo( hTarget, pTarget );
// 		}
// 		else
// 		{
// 			updateIODataItem( itItem->second, pTarget );
// 			itemMap.erase( itItem );
// 		}
// 	}
// 
// 	/// remove not exist item
// 	itItem = itemMap.begin(); 
// 	while( itemMap.end() != itItem )
// 	{
// 		RemoveTreeItem( itItem->second );
// 		++itItem;
// 	}
// 
// 	m_oProjectTreeControl.SortChildren(hTarget);
// }
// 
// 
// void KProjectExplorerWnd::updateIODataItem( HTREEITEM hIOData, KTarget* pTarget )
// {
// 	std::map< CString, HTREEITEM > itemMap;
// 	std::map< CString, HTREEITEM >::iterator itItem;
// 	
// 	HTREEITEM hItem = m_oProjectTreeControl.GetChildItem( hIOData );
// 	while( NULL != hItem )
// 	{
// 		CString strItem = m_oProjectTreeControl.GetItemText( hItem );
// 		itemMap.insert( std::make_pair( strItem, hItem ) );
// 		hItem = m_oProjectTreeControl.GetNextItem( hItem, TVGN_NEXT );
// 	}
// 
// 	KIOTableManager* pIOTables = pTarget->Tables();
// 	int nTableCount = pIOTables->GetCount();
// 	for(int i = 0; i < nTableCount; ++i)
// 	{
// 		KIOTable* pTable = pIOTables->GetTableAt(i);
// 		itItem = itemMap.find(pTable->Caption());
// 		if(itemMap.end() == itItem)
// 		{
// 			HTREEITEM hTable = m_oProjectTreeControl.InsertItem(pTable->Caption(), 11, 11, hIOData);
// 			TTreeItemData* pItemData = new TTreeItemData;
// 			pItemData->TreeItemType = KEMTreeItemTypeIODataTable;
// 			pItemData->BaseObject = NULL;
// 			pItemData->Name = pTable->Name();
// 			m_oProjectTreeControl.SetItemData(hTable, (DWORD_PTR)pItemData);
// 			m_oProjectTreeControl.Expand(hIOData, TVE_EXPAND);
// 		}
// 	}
// 
// 	/// remove not exist items
// 	itItem = itemMap.begin(); 
// 	while( itemMap.end() != itItem )
// 	{
// 		RemoveTreeItem( itItem->second );
// 		++itItem;
// 	}
// 
// 	//m_wndProjectTree.SortChildren(hIOData);
// }
// 

