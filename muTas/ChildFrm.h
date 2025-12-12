#pragma once

/* Forward declarations */
class KTarget;
class KMapFrameWnd;
class KMapView;
class KIOFrameWnd;
class KIOView;
class KTocFormView;
class KTocFrameWnd;
class KTocHideShowView;

//#include "TocFormView.h"

#define CChildFrameBase CXTPFrameWndBase<CMDIChildWnd>
class CChildFrame : public CChildFrameBase
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();
	virtual ~CChildFrame();

public :
	inline bool IsMapViewShowed( void )
	{
		return m_bShowMapView;
	}

	inline bool isIOViewShowed( void )
	{
		return m_bShowIOView;
	}

	KMapView* GetMapView( void );
	KIOView* GetIOView(void);

	KTocFormView* GetTocFormView();

	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year
	*/
	void LoadTarget(KTarget* pTarget);
	
	/**
	* @brief Target year를 반환한다.
	* @return KTarget - Target year
	*/
	KTarget* GetTarget(void);

public :

	/**
	* @brief Map View를 표출하거나 숨긴다.
	* @param[in] bShow - 표출 여부
	*/
	void ShowMapView( bool bShow = true );
	
	bool IsShowMapView() { return m_bShowMapView;}
	/**
	* @brief IO Data View를 표출하거나 숨긴다.
	* @param[in] bShow - 표출 여부
	*/
	void ShowIOView( bool bShow = true );

	/**
	* @brief Frame의 제목 문자열을 갱신한다.
	*/
	void UpdateTargetFrameTitle( void );

protected :
	void RecalcLayout( void );
	void updateTableMenuState( CCmdUI* pCmdUI );
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);	
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

protected:
    DECLARE_MESSAGE_MAP()
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();    
    afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);    
    afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	void CloseFrame();

private:
    KTarget*        m_pTarget;
private:
    bool m_bShowMapView;
    bool m_bShowIOView;
    bool m_bShowToc;

private:
    CXTSplitterWnd	m_wndMainSplitter;
      CXTSplitterWnd	m_wndLeftSplitter;
      KMapFrameWnd*     /*|-*/ m_pMapFrameWnd;
      KIOFrameWnd*      /*|-*/ m_pIOFrameWnd;

      CXTSplitterWnd	m_wndRightSplitter;
      KTocFrameWnd*     /*|-*/ m_pTocFrameWnd;
      KTocHideShowView* /*|-*/ m_pTocHideShowView;

public:
    void ShowToc(bool a_bShow, bool a_bRefresh = true);
    bool IsShowToc();
protected:
	/// Table manager menu
// 	afx_msg void OnFieldAddfield();
// 	afx_msg void OnUpdateFieldAddfield( CCmdUI* pCmdUI );
// 	afx_msg void OnFieldDeletefield();
// 	afx_msg void OnUpdateFieldDeletefield( CCmdUI* pCmdUI );
// 	afx_msg void OnFieldSort();
// 	afx_msg void OnUpdateFieldSort( CCmdUI* pCmdUI );
// 	afx_msg void OnFieldFreeze();
// 	afx_msg void OnUpdateFieldFreeze( CCmdUI* pCmdUI );
// 	afx_msg void OnFieldProperties();
// 	afx_msg void OnUpdateFieldProperties( CCmdUI* pCmdUI );
// 	afx_msg void OnTablemanagerView();
// 	afx_msg void OnUpdateTablemanagerView( CCmdUI* pCmdUI );
// 	afx_msg void OnTablemanagerCreategraph();
// 	afx_msg void OnUpdateTablemanagerCreategraph( CCmdUI* pCmdUI );
// 	afx_msg void OnTablemanagerSelectbyattributes();
// 	afx_msg void OnUpdateTablemanagerSelectbyattributes( CCmdUI* pCmdUI );
// 	afx_msg void OnTablemanagerCalculatefield();
// 	afx_msg void OnUpdateTablemanagerCalculatefield( CCmdUI* pCmdUI );
// 	afx_msg void OnTablemanagerImport();
// 	afx_msg void OnUpdateTablemanagerImport( CCmdUI* pCmdUI );
// 	afx_msg void OnTablemanagerExport();
// 	afx_msg void OnUpdateTablemanagerExport( CCmdUI* pCmdUI );
// 	afx_msg void OnTablemanagerReloadcache();
// 	afx_msg void OnUpdateTablemanagerReloadcache( CCmdUI* pCmdUI );
};
