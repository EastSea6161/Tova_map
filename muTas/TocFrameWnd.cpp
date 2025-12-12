// MapFrameWnd.cpp : 구현 파일입니다.
//


#include "stdafx.h"
#include "KmzApp.h"
#include "TocFrameWnd.h"

IMPLEMENT_DYNCREATE(KTocFrameWnd, CXTPFrameWnd)

KTocFrameWnd::KTocFrameWnd()
{
    TxLogDebugVisitor();
}

KTocFrameWnd::~KTocFrameWnd()
{
    TxLogDebugVisitor();
}

BEGIN_MESSAGE_MAP(KTocFrameWnd, CXTPFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int KTocFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    TxLogDebugStartMsg();

	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/// Create Child Toolbar
	if( !InitCommandBars() )
	{
		return -1;
	}
	
    TxLogDebugEndMsg();
	return 0;
}


BOOL KTocFrameWnd::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
    TxLogDebugStartMsg();
    m_wndTocFormView.Create(IDD_TOC_FORMVIEW, this);
    m_wndTocFormView.ShowWindow(SW_SHOW);        
    TxLogDebugEndMsg();
    return TRUE;
}

void KTocFrameWnd::OnSize(UINT nType, int cx, int cy)
{
	CXTPFrameWnd::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.   
    if (m_wndTocFormView.GetSafeHwnd())
    {
        m_wndTocFormView.MoveWindow(0, 0, cx, cy);
    }
}


