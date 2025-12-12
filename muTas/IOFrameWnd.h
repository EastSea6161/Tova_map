#pragma once

class KIOTableView;
class KIOView;

class KIOFrameWnd : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(KIOFrameWnd)

protected:
	KIOFrameWnd();           
	virtual ~KIOFrameWnd();

public :
	KIOView* GetIOView(void) const;

protected :
	KIOView* m_pIOView;
	CXTPControlLabel* m_pRecordValueLabel;

	CXTPControlButton* m_pBtnBackwardFirst;
	CXTPControlButton* m_pBtnBackward;
	CXTPControlButton* m_pBtnForwardward;
	CXTPControlButton* m_pBtnEnd;
	CXTPControlLabel*  m_pLblPageInfo;

protected :
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnInitCommandsPopup(CXTPCommandBar* pCommandBar);

private:
	int PageStartNo();
protected:
	afx_msg void OnTableViewPageFirst( void );  
	afx_msg void OnUpdateTableViewPageFirst(CCmdUI* pCmdUI);

	afx_msg void OnTableViewPagePre( void );  
	afx_msg void OnUpdateTableViewPagePre(CCmdUI* pCmdUI);

	afx_msg void OnUpdateTableViewPageInfo(CCmdUI* pCmdUI);

	afx_msg void OnTableViewPageNext( void );  
	afx_msg void OnUpdateTableViewPageNext(CCmdUI* pCmdUI);

	afx_msg void OnTableViewPageEnd( void );  
	afx_msg void OnUpdateTableViewPageEnd(CCmdUI* pCmdUI);

	afx_msg void OnTableViewPageNumber( UINT nID);  
	afx_msg void OnUpdateTableViewPageNumber(CCmdUI* pCmdUI);
};
