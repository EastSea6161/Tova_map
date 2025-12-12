#pragma once

#include "KEMTree.h"

#define CTreeCtrl CXTPTreeCtrl

class KTarget;
class KPurpose;
class KGravityCailInfo;
class KGravityCalibrationDlg;

class KGravityTreeCtrl : public CTreeCtrl
{
public:
	KGravityTreeCtrl(void);


protected:

	virtual void PreSubclassWindow();

public:

	HTREEITEM InsertItemState(LPCTSTR lpszItem, int nImage, int nSelectedImage, int nState=STATE_UNCHECKED, HTREEITEM hParent=TVI_ROOT, HTREEITEM hInsertAfter=TVI_LAST);

	BOOL ToggleCheckState(HTREEITEM hItem, UINT uFlags);


public:

	virtual	~KGravityTreeCtrl(void);


	void SetTarget(KTarget* a_pTarget);

public:

	void CheckChilldItems(HTREEITEM a_hItem);

	void UnCheckChildItems(HTREEITEM a_hItem);

	void CheckParentsItems(HTREEITEM a_hItem);

	void UpdateProjectTree(void);

	void InsertTreeItemPurpose();

protected:

	CImageList              m_ilStateImages;
	HCURSOR                 m_hCursor;
	HCURSOR                 m_hCursorNone;
	KTarget*                m_pTarget;
	KGravityCalibrationDlg* m_pGravityCaliDlg;
	std::map<HTREEITEM, KGravityCailInfo*> m_mapGravityInfo;

protected:

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()


};

