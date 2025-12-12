
/**
 * @file ListCtrlEx.h
 * @brief ZInnerEdit 선언, ZListCtrlEx 선언
 * @author 
 * @date 2010.06.23
 * @remark
 */


#pragma once


/**
 * @brief ZListCtrlEx 안에서 subitem의 수정이 가능하도록 한다.
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.06.23
 */
class ZInnerEdit : public CEdit
{
	/**************************************
	* Constructors / Destructor           *
	**************************************/
public :
	ZInnerEdit(int nItem, int nSubItem, CString strText);
	virtual ~ZInnerEdit(void);


	/**************************************
	* Accessors                           *
	**************************************/


	/**************************************
	* Operations                          *
	**************************************/


	/**************************************
	* Overrides                           *
	**************************************/
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	/**************************************
	* Properties / Variables              *
	**************************************/
private : 
	int			m_nItem;
	int			m_nSubItem;
	CString		m_strText;
	bool		m_bESCKey;


	/**************************************
	* Message handlers                    *
	**************************************/
protected :
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
};



/**
 * @brief CListCtrl을 상속하여 편집기능을 개선(Subitem의 텍스트 수정가능)
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.06.23
 */
class AFX_EXT_CLASS ZListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(ZListCtrlEx)

	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	ZListCtrlEx();
	virtual ~ZListCtrlEx();


	/**************************************
	* Accessors                           *
	**************************************/


	/**************************************
	* Operations                          *
	**************************************/
public :
	CEdit* editSubItem(int nItem, int nCol);

private :
	int hitTest(CPoint& point, int& col) const;


	/**************************************
	* Overrides                           *
	**************************************/


	/**************************************
	* Properties / Variables              *
	**************************************/


	/**************************************
	* Message handlers                    *
	**************************************/
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

