/**
 * @file ListCtrlEx.cpp
 * @brief ZInnerEdit 구현, ZListCtrlEx 구현
 * @author 
 * @date 2010.06.23
 * @remark
 */


#include "stdafx.h"
#include "ListCtrlEx.h"



///////////////////////////////////////////////////////////////////////////////
// ZInnerEdit
///////////////////////////////////////////////////////////////////////////////
#define IDC_INNEREDIT 1001

/**************************************
* Constructors / Destructor           *
**************************************/
ZInnerEdit::ZInnerEdit(int nItem, int nSubItem, CString strText)
	: m_nItem(nItem), m_nSubItem(nSubItem), m_strText(strText), m_bESCKey(false)
{

}


ZInnerEdit::~ZInnerEdit(void)
{

}



/**************************************
* Overrides                           *
**************************************/
BOOL ZInnerEdit::PreTranslateMessage(MSG* pMsg)
{
	if( WM_KEYDOWN == pMsg->message )
	{
		if( (VK_RETURN == pMsg->wParam) || (VK_DELETE == pMsg->wParam) || 
			(VK_ESCAPE == pMsg->wParam) || GetKeyState(VK_CONTROL) )
		{
			::TranslateMessage(pMsg);
			::DispatchMessageA(pMsg);
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}



BEGIN_MESSAGE_MAP(ZInnerEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void ZInnerEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( (VK_ESCAPE == nChar) || (VK_RETURN == nChar) )
	{
		m_bESCKey = (VK_ESCAPE == nChar) ? true : false;
		GetParent()->SetFocus();
		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);

	CWnd* pwndParent = GetParent();
	ASSERT(pwndParent != NULL);

	// Resize edit control 
	CString strText;
	GetWindowText(strText);
	CWindowDC dc(this);
	CFont *pFont = pwndParent->GetFont();
	CFont *pOldFont = dc.SelectObject(pFont);
	CSize size = dc.GetTextExtent(strText);
	dc.SelectObject(pOldFont);
	size.cx += 5;

	CRect rectClient, rectParent;
	GetClientRect(&rectClient);
	pwndParent->GetClientRect(&rectParent);

	ClientToScreen(&rectClient);
	pwndParent->ScreenToClient(&rectClient);

	if( size.cx > rectClient.Width() )
	{
		if( size.cx + rectClient.left < rectParent.right )
		{
			rectClient.right = rectClient.left + size.cx;
		}
		else
		{
			rectClient.right = rectParent.right;
		}
		MoveWindow(&rectClient);
	}
}


int ZInnerEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);

	SetWindowText(m_strText);
	SetFocus();
	SetSel(0, -1);

	return 0;
}


void ZInnerEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CWnd* pwndParent = GetParent();
	ASSERT(NULL != pwndParent);

	CString strText;
	GetWindowText(strText);

	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom	= pwndParent->m_hWnd;
	dispinfo.hdr.idFrom		= GetDlgCtrlID();
	dispinfo.hdr.code		= LVN_ENDLABELEDIT;

	dispinfo.item.mask		= LVIF_TEXT;
	dispinfo.item.iItem		= m_nItem;
	dispinfo.item.iSubItem	= m_nSubItem;
	dispinfo.item.pszText	= m_bESCKey ? NULL : (LPTSTR)((LPCTSTR)strText);
	dispinfo.item.cchTextMax = strText.GetLength();

	pwndParent->GetParent()->SendMessage( WM_NOTIFY, pwndParent->GetDlgCtrlID(), 
		(LPARAM)&dispinfo);

	DestroyWindow();
}


void ZInnerEdit::OnNcDestroy()
{
	CEdit::OnNcDestroy();

	delete this;
}





///////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl
///////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(ZListCtrlEx, CListCtrl)


/**************************************
* Constructors / Destructor           *
**************************************/
ZListCtrlEx::ZListCtrlEx()
{

}


ZListCtrlEx::~ZListCtrlEx()
{
}



/**************************************
* Operations                          *
**************************************/
CEdit* ZListCtrlEx::editSubItem(int nItem, int nCol)
{
	if( !EnsureVisible(nItem, TRUE) )
	{
		return NULL;
	}

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( (nCol >= nColumnCount) || (GetColumnWidth(nCol) < 5) )
	{
		return NULL;
	}

	// Get the column offset
	int nOffset = 0;
	for( int i = 0; i < nCol; ++i )
	{
		nOffset += GetColumnWidth(i);
	}

	CRect rectItem;
	GetItemRect(nItem, &rectItem, LVIR_BOUNDS);

	CRect rectClient;
	GetClientRect(&rectClient);
	if( (nOffset + rectItem.left < 0) || (nOffset + rectItem.left > rectClient.right) )
	{
		CSize size(nOffset + rectItem.left, 0);
		Scroll(size);
		rectItem.left -= size.cx;
	}

	LV_COLUMN lvCol;
	lvCol.mask = LVCF_FMT;
	GetColumn(nCol, &lvCol);
	DWORD dwStyle;
	if( (lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT )
	{
		dwStyle = ES_LEFT;
	}
	else if( (lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT )
	{
		dwStyle = ES_RIGHT;
	}
	else
	{
		dwStyle = ES_CENTER;
	}

	rectItem.left += nOffset + 4;
	rectItem.right = rectItem.left + GetColumnWidth(nCol) - 3;
	if( rectItem.right > rectClient.right )
	{
		rectItem.right = rectClient.right;
	}

	dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	CEdit* pEdit = new ZInnerEdit( nItem, nCol, GetItemText(nItem, nCol) );
	pEdit->Create(dwStyle, rectItem, this, IDC_INNEREDIT);

	return pEdit;
}


int ZListCtrlEx::hitTest(CPoint& point, int& nCol) const
{
	int nColCount = 0;
	int nRow = HitTest(point);
	nCol = 0;

	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
	{
		return nRow;
	}

	nRow = GetTopIndex();
	int nBottom = nRow + GetCountPerPage();
	if( nBottom > GetItemCount() )
	{
		nBottom = GetItemCount();
	}

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	for( ; nRow <= nBottom; ++nRow )
	{
		CRect rectItem;
		GetItemRect(nRow, &rectItem, LVIR_BOUNDS);
		if( rectItem.PtInRect(point) )
		{
			for( nColCount = 0; nColCount < nColumnCount; ++nColCount )
			{
				int nColWidth = GetColumnWidth(nColCount);
				if( (point.x >= rectItem.left ) && (point.x <= rectItem.left + nColWidth) )
				{
					nCol = nColCount;
					return nRow;
				}
				rectItem.left += nColWidth;
			}
		}
	}
	return -1;
}



BEGIN_MESSAGE_MAP(ZListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT_EX(LVN_BEGINLABELEDIT, &ZListCtrlEx::OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT_EX(LVN_ENDLABELEDIT, &ZListCtrlEx::OnLvnEndlabeledit)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
BOOL ZListCtrlEx::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	
	return FALSE;
}


BOOL ZListCtrlEx::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	
	LV_ITEM* pItem = &pDispInfo->item;
	if( NULL != pItem->pszText )
	{
		SetItemText(pItem->iItem, pItem->iSubItem, pItem->pszText);
	}

	*pResult = 0;
	return FALSE;
}


int ZListCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}


void ZListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( GetFocus() != this )
	{
		SetFocus();
	}

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void ZListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( GetFocus() != this )
	{
		SetFocus();
	}

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}


void ZListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDblClk(nFlags, point);
	
	int nColumn;
	int nIndex = hitTest(point, nColumn);

	if( -1 != nIndex )
	{
		UINT nFlag = LVIS_FOCUSED;
		if( ((GetItemState(nIndex, nFlag) & nFlag) == nFlag) && (nColumn > 0 ) )
		{
			if( GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS )
			{
				editSubItem(nIndex, nColumn);
			}
		}
		else
		{
			SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, 
				LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}
