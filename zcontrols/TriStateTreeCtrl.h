/**
 * @file TriStateTreeCtrl.h
 * @brief ZTriStateTreeCtrl 선언
 * @author 
 * @date 2010.07.01
 * @remark
 */


#pragma once


#define TREE_UNCHECK		1
#define	TREE_HALFCHECK		2
#define	TREE_CHECK			3


/**
 * @brief CTreeCtrl 컨트롤을 확장 클래스
 *            - Tri-State Checkbox 지원(2010.06.30)
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.06.30
 */
class AFX_EXT_CLASS ZTriStateTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(ZTriStateTreeCtrl)

	/// Constructor and Destructor
public:
	ZTriStateTreeCtrl();
	virtual ~ZTriStateTreeCtrl();


	/// Operation
public :
	void setStateImage( HTREEITEM hItem, int nStateImage );
	int getStateImage( HTREEITEM hItem );
	void changeCheckState( HTREEITEM hItem );
	void updateParentCheckState( HTREEITEM hItem );
	void updateChildCheckState( HTREEITEM hItem );


	/// Message Handler
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};



