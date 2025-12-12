#include "StdAfx.h"
#include "ResultViewTreeCtrl.h"
#include "KmzApp.h"
#include "Target.h"
#include "Scenario.h"
#include "Project.h"
#include "Object.h"

#include "ViewResultInfo.h"
#include "KEMTree.h"

KResultViewTreeCtrl::KResultViewTreeCtrl()
{
	m_hCursor = theApp.LoadCursor(IDC_MODIFY);
	m_hCursorNone = theApp.LoadCursor(IDC_PAN);
}

KResultViewTreeCtrl::~KResultViewTreeCtrl()
{
	if (m_hCursor != NULL) {
		::DestroyCursor(m_hCursor);
	}
	if (m_hCursorNone != NULL) {
		::DestroyCursor(m_hCursorNone);
	}
}

BEGIN_MESSAGE_MAP(KResultViewTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CStateTreeCtrl)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateTreeCtrl message handlers

void KResultViewTreeCtrl::PreSubclassWindow() 
{
	CTreeCtrl::PreSubclassWindow();

	//CBitmap bitmap;
	//CImageList Imagelist;
	//bitmap.LoadBitmap(IDB_TREE_FOLDER);
	//m_ilStateImages.Create(16, 16, ILC_COLOR24|ILC_MASK, 2, 1);
	//m_ilStateImages.Add(&bitmap, RGB(0,255,0));
	//SetImageList(&m_ilStateImages, TVSIL_NORMAL);

	m_ilStateImages.Create(IDB_TREE_CHECK, 13, 1, RGB(255,255,255));
	SetImageList(&m_ilStateImages, TVSIL_STATE);
}

HTREEITEM KResultViewTreeCtrl::InsertItemState(LPCTSTR lpszItem, int nImage, int nSelectedImage, int nState/*=STATE_DISABLED*/, HTREEITEM hParent/*=TVI_ROOT*/, HTREEITEM hInsertAfter/*=TVI_LAST*/)
{
	TV_INSERTSTRUCT tvis;
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvis.item.stateMask = TVIS_STATEIMAGEMASK;
	tvis.item.state = INDEXTOSTATEIMAGEMASK(1);
	tvis.item.pszText = (TCHAR*)lpszItem;
	tvis.item.cchTextMax = (int)_tcslen(lpszItem);
	tvis.item.iImage = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	HTREEITEM hItem = CTreeCtrl::InsertItem(&tvis);

	SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);

	return hItem;
}


void KResultViewTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	GetCursorPos(&point);
	ScreenToClient(&point);

	HTREEITEM hItemCheck = HitTest(point, &nFlags);

	if (hItemCheck != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0)
	{
		if (GetCheck(hItemCheck))
		{
			UnCheckChildItems(hItemCheck);			
		}
		else
		{
			CheckChilldItems(hItemCheck);
		}
	}

	TVHITTESTINFO tvHitTest;
	tvHitTest.pt = point;
	HTREEITEM hItem = HitTest(&tvHitTest);

	if (hItem != NULL)
	{		
		if (ToggleCheckState(hItem, tvHitTest.flags))
		{
			return;
		}
		
	}	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void KResultViewTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_SPACE) {
		ToggleCheckState(GetSelectedItem(), TVHT_ONITEMSTATEICON);
	}	
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL KResultViewTreeCtrl::ToggleCheckState(HTREEITEM hItem, UINT uFlags)
{
	BOOL bOnStateIcon = ((uFlags & TVHT_ONITEMSTATEICON) == TVHT_ONITEMSTATEICON);

	if ((bOnStateIcon == FALSE) || (hItem == NULL)) {
		return FALSE;
	}

	return TRUE;
}

void KResultViewTreeCtrl::OnSelChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	UNREFERENCED_PARAMETER(pNMTreeView);

	*pResult = 0;
}


void KResultViewTreeCtrl::UpdateProjectTree( void )
{
	KProject* pProject = ((CKmzApp*)AfxGetApp())->GetProject();         

	if (pProject)
	{
		InsertTreeItemProjectInfo( pProject );
	}

	SortChildren(NULL);
	Invalidate();
}

HTREEITEM KResultViewTreeCtrl::InsertTreeItemProjectInfo( KProject* a_pProject )
{
	ASSERT( a_pProject );

	CString        strProjectName    = a_pProject->GetName();
	KEMProjectType emDataUse		 = a_pProject->GetProjectType();
	bool bKTDBBase = false;

	// 1. 프로젝트 Item 추가
	HTREEITEM    hItemProject  = InsertItemState( strProjectName, 0, 1 );
	// 1.1 프로젝트 정보 추가
	TTreeItemData* pTreeItemData = new TTreeItemData;
	pTreeItemData->TreeItemType  = KEMTreeItemTypeProject;
	pTreeItemData->Object    = a_pProject;
	pTreeItemData->Name          = strProjectName;

	//SetItemData( hItemProject, (DWORD_PTR)pTreeItemData );

	// 2. Base 시나리오 추가
	KScenario* pBaseScenario = a_pProject->GetBaseScenario();


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

HTREEITEM KResultViewTreeCtrl::InsertTreeItemScenarioInfo( HTREEITEM a_hItemProject, KScenario* a_pScenario, bool a_bKTDBBase )
{
	ASSERT( NULL != a_hItemProject );
	ASSERT( NULL != a_pScenario );

	unsigned int         nID = a_pScenario->GetObjectID();
	CString  strScenarioName = a_pScenario->GetName();

	int nImageIndex = 4;	 
	if(a_pScenario->IsBaseScenario() == true)
	{
		nImageIndex = 2;	
	}

	HTREEITEM hItemScenario = NULL;
	int nState = STATE_UNCHECKED;
	hItemScenario = InsertItemState( strScenarioName, 0, 1, nState ,  a_hItemProject );	

	TTreeItemData* pItemData = new TTreeItemData;
	pItemData->TreeItemType  = KEMTreeItemTypeUserDefineProjectScenario;

	pItemData->Object    = a_pScenario;
	pItemData->Name          = strScenarioName;

	SetItemData( hItemScenario, (DWORD_PTR)pItemData );

	const KTDTargetMap&     oTargetMap = a_pScenario->GetTargetMap();
	KTDTargetMap::const_iterator begin = oTargetMap.begin();
	KTDTargetMap::const_iterator end   = oTargetMap.end();
	while( end != begin )
	{
		InsertTreeItemTargetInfo( hItemScenario, begin->second, a_bKTDBBase );
		++begin;
	}

	SortChildren( hItemScenario );
	Expand( a_hItemProject, TVE_EXPAND );

	return hItemScenario;
}

HTREEITEM KResultViewTreeCtrl::InsertTreeItemTargetInfo( HTREEITEM a_hItemScenario, KTarget* a_pTarget, bool a_bKTDBBase )
{
	ASSERT( NULL != a_hItemScenario );
	ASSERT( NULL != a_pTarget );

	unsigned int nID       = a_pTarget->GetObjectID();
	CString  strTargetName = a_pTarget->GetName();

	int nState = STATE_UNCHECKED;
	HTREEITEM hItemTarget  = InsertItemState( strTargetName, 0, 1, nState, a_hItemScenario );

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

	SetItemData( hItemTarget, (DWORD_PTR)pItemData );


	if( a_pTarget->GetDBaseConnection() != nullptr )
	{
		InsertTreeItemAssignmentInfo( hItemTarget, a_pTarget );
	}

	SortChildren(a_hItemScenario);
	Expand( a_hItemScenario, TVE_EXPAND );

	return hItemTarget;
}

HTREEITEM KResultViewTreeCtrl::InsertTreeItemAssignmentInfo( HTREEITEM a_hItemScenario, KTarget* a_pTarget )
{
	KIOTables*   pIOTables    = a_pTarget->Tables();
	KIOTable*          pTable           = pIOTables->FindTable(m_strTableName);
	const KIOColumns* pColumnCollection = pTable->Columns();
	for(int i = 0; i < pColumnCollection->ColumnCount(); ++i)
	{
		KIOColumn* pColumn = pColumnCollection->GetColumn(i);
		int nDataType = pColumn->DataType();
		if (nDataType == 2)
		{
			CString strColumnName = pColumn->Caption();
			int nState = STATE_UNCHECKED;
			HTREEITEM hItemTarget  = InsertItemState( strColumnName, 0, 1, nState, a_hItemScenario );
		}
	}

	return a_hItemScenario;
}



void KResultViewTreeCtrl::SetModelChoice( int a_nEumChoiceModel )
{
	m_nChoiceModel = a_nEumChoiceModel;

	if (m_nChoiceModel == GENERATION_MODEL)
	{
		m_strTableName = _T("Zone");
	} 
	else if(m_nChoiceModel == DISTRIBUTION_MODEL)
	{ 
		m_strTableName = _T("purpose_od");
	}
	else if (m_nChoiceModel == MODECHOICE_MODEL)
	{
		m_strTableName =_T("mode_od");
	}
	else if(m_nChoiceModel == ASSIGNMENT_MODEL)
	{
		m_strTableName = _T("link");
	}
	else if(m_nChoiceModel == CHAIN_OD)
	{
		m_strTableName = _T("chain_od");
	}
	else if(m_nChoiceModel == VEHICLE_OD)
	{
		m_strTableName = _T("vehicle_mode_od");
	}
	else 
	{
		m_strTableName = _T("");
	}
}

int KResultViewTreeCtrl::GetModelChoice()
{
	return m_nChoiceModel;
}

void KResultViewTreeCtrl::CheckChilldItems( HTREEITEM a_hItem )
{
	SetCheck(a_hItem, TRUE);
	Expand(a_hItem, TVE_EXPAND);
	CheckParentsItems(a_hItem);

	HTREEITEM hChildItem = GetChildItem(a_hItem);

	while (hChildItem != NULL)
	{
		SetCheck(hChildItem, TRUE);

		if (ItemHasChildren(hChildItem))
		{
			CheckChilldItems(hChildItem);
		}

		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
	}
}

void KResultViewTreeCtrl::UnCheckChildItems( HTREEITEM a_hItem )
{
	SetCheck(a_hItem, FALSE);
	Expand(a_hItem, TVE_EXPAND);
	HTREEITEM hChildItem = GetChildItem(a_hItem);

	while (hChildItem != NULL)
	{
		SetCheck(hChildItem, FALSE);

		if (ItemHasChildren(hChildItem))
		{
			UnCheckChildItems(hChildItem);
		}

		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
	}
}

void KResultViewTreeCtrl::CheckParentsItems( HTREEITEM a_hItem )
{
	HTREEITEM hParentItem = GetParentItem(a_hItem);

	if (hParentItem != NULL)
	{
		SetCheck (hParentItem,TRUE);
		CheckParentsItems(hParentItem);
	}
}

void KResultViewTreeCtrl::GetTreeInfoList()
{
	HTREEITEM hFirstItem = GetRootItem();

	HTREEITEM hSenarioItem = GetChildItem(hFirstItem);
	while (hSenarioItem != NULL)
	{
		if (GetCheck(hSenarioItem))
		{
			KScenario* pScnarioInfo = GetTreeScenario(hSenarioItem);
			HTREEITEM hTargetItem = GetChildItem(hSenarioItem);
			while (hTargetItem != NULL)
			{
				if(GetCheck(hTargetItem))
				{
					KTarget* pTargetInfo = GetTreeTarget(pScnarioInfo, hTargetItem);
					HTREEITEM hColumnItem = GetChildItem(hTargetItem);
					while (hColumnItem != NULL)
					{
						if (GetCheck(hColumnItem))
						{
							GetTreeColumn(pScnarioInfo, pTargetInfo, hColumnItem);
						}
						hColumnItem = GetNextItem(hColumnItem, TVGN_NEXT);
					}					
				}
				hTargetItem = GetNextItem(hTargetItem, TVGN_NEXT);
			}
		}
		hSenarioItem = GetNextItem(hSenarioItem, TVGN_NEXT);
	}
}

KScenario* KResultViewTreeCtrl::GetTreeScenario( HTREEITEM a_hItem )
{
	while (a_hItem != NULL)
	{
		CString strScenarioName =  GetItemText(a_hItem);
		KProject* pProject = ((CKmzApp*)AfxGetApp())->GetProject();

		KScenario* pBaseScenario = pProject->GetBaseScenario();
		if (strScenarioName == 	pBaseScenario->GetName() )
		{
			return pBaseScenario;
		}

		const KTDScenarioMap& oScenarioMap  = pProject->GetScenarioMap();
		KTDScenarioMap::const_iterator iter = oScenarioMap.begin();
		KTDScenarioMap::const_iterator end  = oScenarioMap.end();

		while( iter != end )
		{
			KScenario* pScenario = iter->second;
			if (strScenarioName == pScenario->GetName())
			{
				return pScenario;
			}
			++iter;
		}
	}

	return NULL;
}

KTarget* KResultViewTreeCtrl::GetTreeTarget(KScenario* a_pKScenario, HTREEITEM a_hItem )
{
	CString strTargetName = GetItemText(a_hItem);
	KTarget* pKTarget = a_pKScenario->findTarget(strTargetName);
	
	if (pKTarget == NULL)
	{
		return NULL;
	}
	return pKTarget;
}

void KResultViewTreeCtrl::GetTreeColumn( KScenario* a_pKScenario, KTarget* a_pKTarget, HTREEITEM a_hItem )
{
	CString strColumnName = GetItemText(a_hItem);

	KIOTables*           pIOTables     = a_pKTarget->Tables();
	KIOTable*                  pTable            = pIOTables->FindTable(m_strTableName);
	const KIOColumns* pColumnCollection = pTable->Columns();
	for(int i = 0; i < pColumnCollection->ColumnCount(); ++i)
	{
		KIOColumn* pColumn = pColumnCollection->GetColumn(i);
		CString strColumnCaption = pColumn->Caption();
		if (strColumnCaption == strColumnName)
		{
			KViewResultInfo oViewResultInfo;
			oViewResultInfo.SetScenarioInfo(a_pKScenario);
			oViewResultInfo.SetTargetInfo(a_pKTarget);
			oViewResultInfo.SetColumnInfo(pColumn);
			m_vecKResultInfo.push_back(oViewResultInfo);
		}
	}
}

void KResultViewTreeCtrl::ClearViewResultInfo()
{
	size_t nxCount = m_vecKResultInfo.size();
	for (size_t i = 0; i < nxCount; i++)
	{
		KViewResultInfo oKResultInfo = m_vecKResultInfo[i];
		KIOColumn* pKColumn   = NULL;
		KTarget*   pKTarget   = NULL;
		KScenario* pKScenario = NULL;
		oKResultInfo.SetColumnInfo(pKColumn);
		oKResultInfo.SetScenarioInfo(pKScenario);
		oKResultInfo.SetTargetInfo(pKTarget);
	}

	std::vector<KViewResultInfo> vecResultInfo;
	vecResultInfo.swap(m_vecKResultInfo);
}


