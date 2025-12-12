#include "StdAfx.h"
#include "KmzApp.h"
#include "GravityTreeCtrl.h"
#include "KEMTree.h"
#include "Target.h"
#include "Purpose.h"
#include "GravityCailInfo.h"
#include "PurposeInfoCollection.h"
#include "GravityCalibrationDlg.h"

KGravityTreeCtrl::KGravityTreeCtrl(void)
{
	//m_pGravityCaliDlg = new KGravityCalibrationDlg;
	m_hCursor = theApp.LoadCursor(IDC_MODIFY);
	m_hCursorNone = theApp.LoadCursor(IDC_PAN);
}

KGravityTreeCtrl::~KGravityTreeCtrl(void)
{
	if (m_hCursor != NULL) 
	{
		::DestroyCursor(m_hCursor);
	}
	if (m_hCursorNone != NULL)
	{
		::DestroyCursor(m_hCursorNone);
	}
}

BEGIN_MESSAGE_MAP(KGravityTreeCtrl, CTreeCtrl)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
END_MESSAGE_MAP()

void KGravityTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
	m_ilStateImages.Create(IDB_TREE_CHECK, 13, 1, RGB(255,255,255));
	SetImageList(&m_ilStateImages, TVSIL_STATE);

}

HTREEITEM KGravityTreeCtrl::InsertItemState( LPCTSTR lpszItem, int nImage, int nSelectedImage, int nState/*=STATE_UNCHECKED*/, HTREEITEM hParent/*=TVI_ROOT*/, HTREEITEM hInsertAfter/*=TVI_LAST*/ )
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

BOOL KGravityTreeCtrl::ToggleCheckState( HTREEITEM hItem, UINT uFlags )
{
	BOOL bOnStateIcon = ((uFlags & TVHT_ONITEMSTATEICON) == TVHT_ONITEMSTATEICON);

	if ((bOnStateIcon == FALSE) || (hItem == NULL))
	{
		return FALSE;
	}
	return TRUE;
}

void KGravityTreeCtrl::OnLButtonDown( UINT nFlags, CPoint point )
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


void KGravityTreeCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (nChar == VK_SPACE) 
	{
		ToggleCheckState(GetSelectedItem(), TVHT_ONITEMSTATEICON);
	}	
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}



void KGravityTreeCtrl::OnSelChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	UNREFERENCED_PARAMETER(pNMTreeView);

	HTREEITEM hItemCheck = pNMTreeView->itemNew.hItem;

	if (hItemCheck != NULL)
	{
		if (GetCheck(hItemCheck))
		{
			UnCheckChildItems(hItemCheck);			
		}
		else
		{
			CheckChilldItems(hItemCheck);

			std::map<HTREEITEM, KGravityCailInfo*>::iterator findIter = m_mapGravityInfo.find(hItemCheck);
			std::map<HTREEITEM, KGravityCailInfo*>::iterator end      = m_mapGravityInfo.end();
			if (findIter != end)
			{
				KGravityCailInfo* pGravityInfo = NULL;
				pGravityInfo = findIter->second;
				CString aaa = _T("");
				aaa.Format(_T("%d"),pGravityInfo->m_nGrade);
				GetDlgItem(IDC_EDIT1)->SetWindowText(aaa);
			}
		}
	}

	*pResult = 0;
}

void KGravityTreeCtrl::CheckChilldItems( HTREEITEM a_hItem )
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

void KGravityTreeCtrl::UnCheckChildItems( HTREEITEM a_hItem )
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

void KGravityTreeCtrl::CheckParentsItems( HTREEITEM a_hItem )
{
	HTREEITEM hParentItem = GetParentItem(a_hItem);

	if (hParentItem != NULL)
	{
		SetCheck (hParentItem,TRUE);
		CheckParentsItems(hParentItem);
	}
}

void KGravityTreeCtrl::UpdateProjectTree( void )
{
	
	InsertTreeItemPurpose();
	
	SortChildren(NULL);
	Invalidate();
}

void KGravityTreeCtrl::SetTarget( KTarget* a_pTarget )
{
	m_pTarget = a_pTarget;
}

void KGravityTreeCtrl::InsertTreeItemPurpose()
{
	if(NULL == m_pTarget)
	{
		return;
	}

	int nYear = m_pTarget->getTargetYear();
	CString strYear = _T("");
	strYear.Format(_T("%d"),nYear);

	HTREEITEM hItemYear = InsertItemState(strYear, 0, 1);
	
	KPurposeInfoCollection purposeInfos(m_pTarget);
	std::list<KPurpose*>::iterator iter = purposeInfos.begin();
	std::list<KPurpose*>::iterator end  = purposeInfos.end();

	while(iter != end)
	{
		KPurpose* pPurpose = *iter;
		CString strPurposeName = pPurpose->PurposeName();
		int nState = STATE_UNCHECKED;
		HTREEITEM hItemPurpose = InsertItemState(strPurposeName, 0, 1, nState, hItemYear);
		KGravityCailInfo* pGravityCailInfo = new KGravityCailInfo;
		m_mapGravityInfo.insert(std::make_pair(hItemPurpose, pGravityCailInfo));
		++iter;
	}

}

