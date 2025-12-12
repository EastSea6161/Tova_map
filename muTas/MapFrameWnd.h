#pragma once

#include "CommandWnd.h"
#include "TocWindow.h"
#include "TocFormView.h"
/* Forward declaration */
class KMapView;

class KMapFrameWnd : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(KMapFrameWnd)
    DECLARE_MESSAGE_MAP()

protected:
	// 동적 만들기에 사용되는 protected 생성자입니다.
	KMapFrameWnd();           
	virtual ~KMapFrameWnd();
public :
	KMapView*    GetMapView( void );    
    KCommandWnd* GetCommandView( void );

protected :
	void ShowDockingPanePopupMenu( CXTPDockingPane* pPane, CPoint pt, LPRECT lpreExclude );
protected :
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);    
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg int OnCreateControl( LPCREATECONTROLSTRUCT lpCreateControl );
    afx_msg LRESULT OnDockingPaneNotify( WPARAM wParam, LPARAM lParam );
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus( CWnd* pOldWnd );	
	afx_msg void OnMapViewEditOperation( void );

protected:	
/*-----------------지도 툴바-----------------------*/
    CXTPToolBar*            m_pMapControlBar;    
        CXTPControlComboBox*	m_pComboTarget;
        CXTPControlComboBox*	m_pComboTask;
        CXTPControlComboBox*	m_pComboLinkType;
        CXTPControlEdit*		m_pEditLinkWidth;
        CXTPControlButton*      m_pMapEditFindButton;        
        CXTPControlButton*      m_pSelectionRectButton;
        CXTPControlButton*      m_pSelectionPolyButton;   
        CXTPControlButton*      m_pMapRefreshButton;
        CXTPControlButton*      m_pMapPasteButton;
        CXTPControlButton*      m_pMapEditStartButton;
        CXTPControlButton*      m_pMapEditStopButton;        

        //IDI_ICON_MAPEDIT_STOP
        //CXTPControlCheckBox*    m_pCheckSelection;
/*------------------   지도   --------------------*/
    KMapView*		        m_pwndMapView;
/*-----------------레이어+커맨드--------------------*/
    CXTPDockingPaneManager  m_paneManager;
        KCommandWnd             m_wndCommandView;    
};
