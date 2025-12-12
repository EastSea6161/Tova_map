#pragma once


enum LAYER_STATE 
{
	LAYER_NONE,
	LAYER_OFF,
	LAYER_ON,
	LAYER_ONOFF,
	LAYER_DISABLE
};


#define CTreeCtrl CXTPTreeCtrl
#define WM_CHANGE_LAYER_STATE	WM_USER + 101

class KMapLayerTreeCtrl : public CTreeCtrl
{
public:
	KMapLayerTreeCtrl();
	virtual ~KMapLayerTreeCtrl();

public:
	HTREEITEM InsertItemState(LPCTSTR lpszItem, int nImage, int nSelectedImage, int nState=LAYER_ON, HTREEITEM hParent=TVI_ROOT, HTREEITEM hInsertAfter=TVI_LAST);
	//HTREEITEM InsertItemState(LPCTSTR lpszItem, int nImage, int nSelectedImage, int nState=STATE_UNCHECKED, HTREEITEM hParent=TVI_ROOT, HTREEITEM hInsertAfter=TVI_LAST);
	BOOL      ToggleCheckState(HTREEITEM hItem, UINT uFlags);
	int       GetItemCheck(HTREEITEM hItem) const;
	BOOL      SetItemCheck(HTREEITEM hItem, int nState);
protected:
	CImageList  m_imageList;
	CImageList  m_ilStateImages;
	/// Message Handler
protected:
	DECLARE_MESSAGE_MAP()
protected:
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};








