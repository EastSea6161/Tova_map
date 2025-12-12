
#pragma once

#include "ProjectExplorerWnd.h"
#include "KEMTree.h"


enum CHOICE_MODEL
{
	GENERATION_MODEL,
	DISTRIBUTION_MODEL,
	MODECHOICE_MODEL,
	ASSIGNMENT_MODEL,
	CHAIN_OD,
	VEHICLE_OD
};

#define CTreeCtrl CXTPTreeCtrl

class KProject;
class KTarget;
class KScenario;
class KObject;
class KScenario;
class KIOColumn;
class KViewResultInfo;



class KResultViewTreeCtrl : public CTreeCtrl
{
	// Construction
public:
	KResultViewTreeCtrl();

	// Operations
protected:
	HTREEITEM InsertItemState(LPCTSTR lpszItem, int nImage, int nSelectedImage, int nState=STATE_UNCHECKED, HTREEITEM hParent=TVI_ROOT, HTREEITEM hInsertAfter=TVI_LAST);
	BOOL ToggleCheckState(HTREEITEM hItem, UINT uFlags);
	
protected:
	virtual void PreSubclassWindow();

public:
	virtual ~KResultViewTreeCtrl();
public:
	std::vector<KViewResultInfo> m_vecKResultInfo;

protected:
	CImageList                   m_ilStateImages;
	HCURSOR                      m_hCursor;
	HCURSOR                      m_hCursorNone;
	int                          m_nChoiceModel;
	CString                      m_strTableName;

public:	
	void SetModelChoice(int a_nEumChoiceModel);
	int  GetModelChoice();
	void UpdateProjectTree( void );
	void ClearProjectTree( void );
	void GetTreeInfoList();
	void ClearViewResultInfo();
protected:
	HTREEITEM InsertTreeItemProjectInfo( KProject* a_pProject );
	HTREEITEM InsertTreeItemScenarioInfo(HTREEITEM a_hItemProject, KScenario* a_pScenario, bool a_bKTDBBase );
	HTREEITEM InsertTreeItemTargetInfo( HTREEITEM a_hItemScenario, KTarget* a_pTarget, bool a_bKTDBBase );
	HTREEITEM InsertTreeItemAssignmentInfo( HTREEITEM a_hItemScenario, KTarget* a_pTarget );
	void CheckChilldItems(HTREEITEM a_hItem);
	void UnCheckChildItems(HTREEITEM a_hItem);	
	void CheckParentsItems(HTREEITEM a_hItem);
	KScenario*    GetTreeScenario(HTREEITEM a_hItem);
	KTarget*      GetTreeTarget(KScenario* a_pKScenario, HTREEITEM a_hItem);
	void          GetTreeColumn(KScenario* a_pKScenario, KTarget* a_pKTarget, HTREEITEM a_hItem);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};