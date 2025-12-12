// TriStateTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TriStateTreeCtrl.h"




// ZTriStateTreeCtrl
IMPLEMENT_DYNAMIC(ZTriStateTreeCtrl, CTreeCtrl)


/// Constructor and Destructor
ZTriStateTreeCtrl::ZTriStateTreeCtrl()
{

}

ZTriStateTreeCtrl::~ZTriStateTreeCtrl()
{
}


/// ZTriStateTreeCtrl message handlers
BEGIN_MESSAGE_MAP(ZTriStateTreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


void ZTriStateTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT nStateFlags = 0;
	HTREEITEM hItem = HitTest( point, &nStateFlags );
	if( (NULL != hItem) && (nStateFlags & TVHT_ONITEMSTATEICON) )
	{
		SelectItem( hItem );
		changeCheckState( hItem );
	}
	else
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);
	}
}

void ZTriStateTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	UINT nStateFlags = 0;
	HTREEITEM hItem = HitTest( point, &nStateFlags );
	if( (NULL != hItem) && (nStateFlags & TVHT_ONITEMSTATEICON) )
	{
		SelectItem( hItem );
		changeCheckState( hItem );
	}
	else
	{
		CTreeCtrl::OnLButtonDblClk(nFlags, point);
	}
}



/// Operation
void ZTriStateTreeCtrl::setStateImage( HTREEITEM hItem, int nStateImage )
{
	SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nStateImage), TVIS_STATEIMAGEMASK );
}


int ZTriStateTreeCtrl::getStateImage( HTREEITEM hItem )
{
	return GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
}


void ZTriStateTreeCtrl::changeCheckState( HTREEITEM hItem )
{
	int nState = getStateImage( hItem );
	if( ItemHasChildren( hItem ) == TRUE )
	{
		if( (TREE_UNCHECK == nState) || (TREE_HALFCHECK == nState) )
		{
			setStateImage( hItem, TREE_CHECK );
			updateParentCheckState( hItem );
			updateChildCheckState( hItem );
		}
		else if( nState == TREE_CHECK )
		{
			setStateImage( hItem, TREE_UNCHECK );
			updateParentCheckState( hItem );
			updateChildCheckState( hItem );
		}
	}
	else
	{
		if( TREE_CHECK == nState )
		{
			setStateImage( hItem, TREE_UNCHECK );
		}
		else
		{
			setStateImage( hItem, TREE_CHECK );
		}
		updateParentCheckState( hItem );
	}
}


void ZTriStateTreeCtrl::updateParentCheckState( HTREEITEM hItem )
{
	HTREEITEM hParent = GetParentItem( hItem );
	bool bChecked = false;
	bool bUnchecked = false;
	if( NULL != hParent )
	{
		HTREEITEM hChild = GetChildItem( hParent );
		while( NULL != hChild )
		{
			int nState = getStateImage( hChild );
			if( TREE_UNCHECK == nState )
			{
				bUnchecked = true;
			}
			else if( TREE_HALFCHECK == nState )
			{
				bUnchecked = true;
				bChecked = true;
			}
			else if( TREE_CHECK == nState )
			{
				bChecked = true;
			}

			hChild = GetNextItem( hChild, TVGN_NEXT );
		}

		if( (true == bChecked) && (true == bUnchecked) )
		{
			setStateImage( hParent, TREE_HALFCHECK );
		}
		else if( true == bChecked )
		{
			setStateImage( hParent, TREE_CHECK );
		}
		else if( true == bUnchecked )
		{
			setStateImage( hParent, TREE_UNCHECK );
		}

		updateParentCheckState( hParent );
	}

}


void ZTriStateTreeCtrl::updateChildCheckState( HTREEITEM hItem )
{
	int nState = getStateImage( hItem );

	HTREEITEM hChild = GetChildItem( hItem );
	while( NULL != hChild )
	{
		setStateImage( hChild, nState );
		updateChildCheckState( hChild );
		hChild = GetNextItem( hChild, TVGN_NEXT );
	}
}








