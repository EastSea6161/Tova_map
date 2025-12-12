#include "StdAfx.h"
#include "TocWindow.h"

#include "KmzApp.h"
#include "Project.h"

//^^//^^#include "SEDData.h"
//^#include "DBaseConnector.h"

#include "MapView.h"
#include "ImChampFrameWindow.h"

#include "NaviItemAddDlg.h"
#include "NaviItemRenameDlg.h"


IMPLEMENT_DYNAMIC(KTocWindow, CWnd)

KTocWindow::KTocWindow(void)
{
}


KTocWindow::~KTocWindow(void)
{
}




int KTocWindow::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if(CWnd::OnCreate( lpCreateStruct ) == -1)
		return -1;

	if(!m_imgList.Create( 16, 16, ILC_MASK | ILC_COLOR24, 0, 1))
	{
		AfxMessageBox(_T("Failed to create image list"));
		return -1;
	}

	CBitmap bmp;
	bmp.LoadBitmap(IDB_NAVIGATION_VIEW);
	m_imgList.Add(&bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0)));

	if (m_wndTreeCtrl.GetSafeHwnd() == 0)
	{
		if( !m_wndTreeCtrl.Create( 
			WS_VISIBLE | TVS_HASLINES |TVS_LINESATROOT | TVS_HASBUTTONS | 
			TVS_SHOWSELALWAYS, CRect( 0, 0, 0, 0 ), this, IDC_SED_TREECTRL ) )
		{
			TRACE0( "Unable to create tree control.\n" );
			return 0;
		}
		m_wndTreeCtrl.ModifyStyleEx( 0, WS_EX_STATICEDGE );
		m_wndTreeCtrl.SetImageList( &m_imgList, TVSIL_NORMAL );
	}
	return 0;
}


void KTocWindow::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );
	if( m_wndTreeCtrl.GetSafeHwnd() )
	{
		CRect rectClient;
		GetClientRect( &rectClient );

		m_wndTreeCtrl.SetWindowPos( NULL, rectClient.left, rectClient.top, 
			rectClient.Width(), rectClient.Height(), 
			SWP_NOACTIVATE | SWP_NOZORDER );
	}
}


void KTocWindow::OnSetFocus( CWnd* pOldWnd )
{
	m_wndTreeCtrl.SetFocus();
}


BEGIN_MESSAGE_MAP(KTocWindow, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

