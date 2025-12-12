
#include "StdAfx.h"
#include "MapLayerTreeCtrl.h"
	
// Constructor and Destructor
KMapLayerTreeCtrl::KMapLayerTreeCtrl()
{
}

KMapLayerTreeCtrl::~KMapLayerTreeCtrl()
{
}


/// ZTriStateTreeCtrl message handlers
BEGIN_MESSAGE_MAP(KMapLayerTreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void KMapLayerTreeCtrl::PreSubclassWindow() 
{
	CTreeCtrl::PreSubclassWindow();
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_MAPLAYER);

	m_imageList.Create(25, 18, ILC_COLOR24|ILC_MASK, 2, 1);
	m_imageList.Add(&bitmap, CXTPImageManager::GetBitmapMaskColor(bitmap, CPoint(0, 0) ) );
	SetImageList(&m_imageList, TVSIL_NORMAL);

	CBitmap bmp;
	bmp.LoadBitmap( IDB_LAYERONOFF_STATE );

	m_ilStateImages.Create( 13, 17, ILC_COLOR24 | ILC_MASK, 4, 1 );
	m_ilStateImages.Add( &bmp, RGB(255, 255, 255) );

	SetImageList(&m_ilStateImages, TVSIL_STATE);	
	//SetItemHeight(17);
}

HTREEITEM KMapLayerTreeCtrl::InsertItemState(LPCTSTR lpszItem, int nImage, int nSelectedImage, int nState/*=STATE_DISABLED*/, HTREEITEM hParent/*=TVI_ROOT*/, HTREEITEM hInsertAfter/*=TVI_LAST*/)
{
	TV_INSERTSTRUCT tvis;
	tvis.hParent         = hParent;
	tvis.hInsertAfter    = hInsertAfter;
	tvis.item.mask       = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvis.item.stateMask  = TVIS_STATEIMAGEMASK;
	tvis.item.state      = INDEXTOSTATEIMAGEMASK(1);
	tvis.item.pszText    = (TCHAR*)lpszItem;
	tvis.item.cchTextMax = (int)_tcslen(lpszItem);
	tvis.item.iImage     = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	HTREEITEM hItem = CTreeCtrl::InsertItem(&tvis);

	SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);

	return hItem;
}

BOOL KMapLayerTreeCtrl::ToggleCheckState(HTREEITEM hItem, UINT uFlags)
{
	BOOL bOnStateIcon = ((uFlags & TVHT_ONITEMSTATEICON) == TVHT_ONITEMSTATEICON);

	if ((bOnStateIcon == FALSE) || (hItem == NULL)) {
		return FALSE;
	}

	int nState = GetItemCheck(hItem);
	
	switch(nState)
	{
	case LAYER_OFF:
		nState = LAYER_ON;
		break;
	case LAYER_ON:
		nState = LAYER_OFF;
		// 현재 보여지고 있는지 체크
		break;
	default:
		nState = LAYER_OFF;
		break;
	}
	
	return SetItemCheck(hItem, nState);
}

int KMapLayerTreeCtrl::GetItemCheck(HTREEITEM hItem) const
{
	return (hItem == NULL)? LAYER_NONE: (GetItemState(hItem, TVIS_STATEIMAGEMASK)>>12);
}

BOOL KMapLayerTreeCtrl::SetItemCheck(HTREEITEM hItem, int nState)
{
	return SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
}

void KMapLayerTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	TVHITTESTINFO tvHitTest;
	tvHitTest.pt = point;

	HTREEITEM hItem = HitTest(&tvHitTest);
	if (hItem != NULL)
	{
		if (ToggleCheckState(hItem, tvHitTest.flags))
		{
			GetParent()->SendMessage(WM_CHANGE_LAYER_STATE, 0, LPARAM(hItem));
			return;
		}
	}	

	CTreeCtrl::OnLButtonDown(nFlags, point);
}