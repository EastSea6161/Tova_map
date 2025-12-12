/**
 * @file ProjectView.h
 * @brief KProjectView 선언
 * @author 
 * @date 2010.07.27
 * @remark
 */


#pragma once


#include "officeTreeCtrl.h"


/* Forward declarations */
class KObject;
class KProject;
class KScenario;
class KTarget;


/* Tree item type */
enum KEMTreeItemType
{
	KEMTreeItemTypeProject, 
	KEMTreeItemTypeKDTBProjectScenario,
	KEMTreeItemTypeKTDBProjectTarget, 
	KEMTreeItemTypeNetworkData,
	KEMTreeItemTypeIODataFolder,
	KEMTreeItemTypeIODataTable,
	KEMTreeItemTypeUserDefineProjectScenario,
	KEMTreeItemTypeUserDefineProjectTarget
};


/* Tree item data */
class TTreeItemData
{
public:
    TTreeItemData() : Object(nullptr), m_pIOTable(nullptr)
    {}
    ~TTreeItemData() {}
public:
	KEMTreeItemType TreeItemType;
	KObject *       Object;
	CString         Name;

    KIOTable*       m_pIOTable;
};


class KProjectExplorerWnd : public CWnd, public KTableObserver
{
    DECLARE_MESSAGE_MAP();

public:
	KProjectExplorerWnd(void);
	~KProjectExplorerWnd(void);

public:
	void InsertProjectTree( void );
	const TTreeItemData* GetSelectedTreeItemData( void );
	void RedrawTreeItemData(void);

private:
	void RemoveDisuseScenarioTreeItem(HTREEITEM a_hItemProject, KProject* a_pProject);
	void RemoveDisuseTargetTreeItem(HTREEITEM a_hItemScenario, KScenario* a_pScenario);
	void AppendNewScenarioTreeItem(HTREEITEM a_hItemProject, KProject* a_pProject, bool a_bKTDBBase);
	void AppendNewTargetTreeItem(HTREEITEM a_hItemScenario, KScenario* a_pScenario, bool a_bKTDBBase);
	void UpdateTreeItemName(HTREEITEM a_hItemProject);

private:
	HTREEITEM InsertTreeItemProjectInfo ( KProject* pProject );
	HTREEITEM InsertTreeItemScenarioInfo( HTREEITEM a_hItemProject,  KScenario* a_pScenario, bool a_bKTDBBase );
	HTREEITEM InsertTreeItemTargetInfo  ( HTREEITEM a_hItemScenario, KTarget*   a_pTarget,   bool a_bKTDBBase );
	HTREEITEM InsertTreeItemNetworkData ( HTREEITEM a_hItemTarget );
	void InsertTreeItemIODataInfo( HTREEITEM a_hItemTarget, KTarget* a_pTarget );
    	
	void RemoveTreeItem  ( HTREEITEM hItem );
public:
	void ClearProjectTree();
private:
    virtual void NotifyProcess(LPCTSTR a_strSubjectName=_T(""), Integer a_nxObjectID=0);
    void UpdateTableInfo(HTREEITEM hItem, LPCTSTR a_strSubjectName, Integer a_nxObjectID);
protected :
	void DblClickItemNetwork( HTREEITEM hItem, TTreeItemData* pItemData );
	void DblClickItemTable  ( HTREEITEM hItem, TTreeItemData* pItemData );

	void ShowProjectMenu ( CPoint& point );
	void ShowScenarioMenu( CPoint& point, bool a_bKTDBBase );
	void ShowTargetMenu  ( CPoint& point, bool a_bKTDBBase );
	void ShowTableMenu   ( CPoint& point );    
    
protected :	
	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnNMDblCllkTreeCtrl( NMHDR* pNMHDR, LRESULT *pResult );
	afx_msg void OnExpandedTreeCtrl( NMHDR* pNMHDR, LRESULT* pResult );

protected:
    COfficeTreeCtrl m_oProjectTreeControl;
    CImageList		m_imgListProjectView;

    // 	/**
    // 	* @brief Scenario의 Tree를 갱신한다.
    // 	* @param[in] hProject - Tree의 Handle
    // 	* @param[in] pProject - Project 정보
    // 	*/
    // 	void UpdateScenarioTree( HTREEITEM hProject, KProject* pProject );
    // 
    // 	/**
    // 	* @brief Target year의 Tree를 갱신한다.
    // 	* @param[in] hScenario - Tree의 Handle
    // 	* @param[in] pScenario - Scenario 정보
    // 	*/
    // 	void UpdateTargetTree( HTREEITEM hScenario, KScenario* pScenario );
    // 
    // 	/**
    // 	* @brief Target 하위 데이터의 Tree를 갱신한다.
    // 	* @param[in] hTarget - Tree의 Handle
    // 	* @param[in] pTarget - Target 정보
    // 	*/
    // 	void UpdateTargetItem( HTREEITEM hTarget, KTarget* pTarget );
    // 
    // 	/**
    // 	* @brief IO Data의 하위 Tree를 갱신한다.
    // 	* @param[in] hIOData - Tree의 Handle
    // 	* @param[in] pTarget - Target 정보 
    // 	*/
    // 	void updateIODataItem( HTREEITEM hIOData, KTarget* pTarget );
};
