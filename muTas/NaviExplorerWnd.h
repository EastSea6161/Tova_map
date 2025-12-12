#pragma once

#include "officeTreeCtrl.h"


/* Tree item data */
struct TNaviTreeItemData
{
	int     nNaviID;
	CString strNaviName;
	double  dMinX;
	double  dMinY;
	double  dMaxX;
	double  dMaxY;
};

#include "Project.h"

class KNaviExplorerWnd : public CWnd
{
	DECLARE_DYNAMIC(KNaviExplorerWnd)

public:
	KNaviExplorerWnd(void);
	~KNaviExplorerWnd(void);

public:
	/**
	* @brief Tree에서 선택 된 항목의 Data를 반환한다.
	* @return KSTTreeData* - Data
	*/
	const TNaviTreeItemData* GetSelectedTreeItemData( void );

	void UpdateTree( KProject* a_pProjecct );
	void ClearTree( void );

	void AddNaviItem( void );
	void RenameNaviItem( void );
	void RemoveNaviItem( void );
private:
	KProject* m_pProjecct;
	void UpdateTree();
private:
	/**
	* @brief Project 정보를 추가한다.
	* @param[in] pProject - Project
	* @return void - 추가된 Item의 Handle
	*/

	void RemoveTreeItem( HTREEITEM hItem );

	void InsertTreeItemRootInfo(  );
	void InsertTreeItemNaviInfo( HTREEITEM a_hItemRoot );

protected:
	COfficeTreeCtrl m_wndTreeCtrl;
	CImageList      m_imgList;

protected:
	void ShowRootItemMenu( CPoint& point );
	void ShowNaviItemMenu( CPoint& point );

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point );
	afx_msg void OnNMDblCllkTreeCtrl( NMHDR* pNMHDR, LRESULT *pResult );
	afx_msg void OnExpandedTreeCtrl( NMHDR* pNMHDR, LRESULT* pResult );

};

