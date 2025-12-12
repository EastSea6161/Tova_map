#include "stdafx.h"
#include "ChildFrm.h"

// void CChildFrame::OnFieldAddfield()
// {
// 	BOOL bEnable = FALSE;
// 	if(true == m_bShowIOView)
// 	{
// 		KIOView* pView = GetIOView();
// 		if(NULL != pView)
// 		{
// 			// todo : modify this code
// 			//pView->commandTableView( ID_TABLEVIEWMENU_ADD );
// 		}
// 	}
// }
// 
// 
// void CChildFrame::OnUpdateFieldAddfield( CCmdUI* pCmdUI )
// {
// 	updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnFieldDeletefield()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// 
// void CChildFrame::OnUpdateFieldDeletefield( CCmdUI* pCmdUI )
// {
// 	/// $task : Delete field 구현 후 아래 수정
// 	pCmdUI->Enable( FALSE );
// 	//updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnFieldSort()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// 
// void CChildFrame::OnUpdateFieldSort( CCmdUI* pCmdUI )
// {
// 	/// $task : Field sort 구현 후 아래 수정
// 	pCmdUI->Enable( FALSE );
// 	//updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnFieldFreeze()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// 
// void CChildFrame::OnUpdateFieldFreeze( CCmdUI* pCmdUI )
// {
// 	/// $task : Field Freeze 구현 후 아래 수정
// 	pCmdUI->Enable( FALSE );
// 	//updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnFieldProperties()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// 
// void CChildFrame::OnUpdateFieldProperties( CCmdUI* pCmdUI )
// {
// 	/// $task : Field Properties 구현 후 아래 수정
// 	pCmdUI->Enable( FALSE );
// 	//updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnTablemanagerView()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// 
// void CChildFrame::OnUpdateTablemanagerView( CCmdUI* pCmdUI )
// {
// 	/// $task : Table manager View 구현 후 아래 수정
// 	pCmdUI->Enable( FALSE );
// 	//updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnTablemanagerCreategraph()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// 
// void CChildFrame::OnUpdateTablemanagerCreategraph( CCmdUI* pCmdUI )
// {
// 	/// $task : Create graph 구현 후 아래 수정
// 	pCmdUI->Enable( FALSE );
// 	//updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnTablemanagerSelectbyattributes()
// {
// 	BOOL bEnable = FALSE;
// 	if(true == m_bShowIOView)
// 	{
// 		KIOView* pView = GetIOView();
// 		if(NULL != pView)
// 		{
// 			// todo : modify this code
// 			//pView->commandTableView( ID_TABLEVIEWMENU_SELECTBYATTRIBUTES );
// 		}
// 	}
// }
// 
// 
// void CChildFrame::OnUpdateTablemanagerSelectbyattributes( CCmdUI* pCmdUI )
// {
// 	updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnTablemanagerCalculatefield()
// {
// 	BOOL bEnable = FALSE;
// 	if(true == m_bShowIOView)
// 	{
// 		KIOView* pView = GetIOView();
// 
// 		if(NULL != pView)
// 		{
// 			// todo : modify this code
// 			//pView->commandTableView( ID_TABLEVIEWMENU_CALCULATEFIELD );
// 		}
// 	}
// }
// 
// 
// void CChildFrame::OnUpdateTablemanagerCalculatefield( CCmdUI* pCmdUI )
// {
// 	updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnTablemanagerImport()
// {
// 	BOOL bEnable = FALSE;
// 	if(true == m_bShowIOView)
// 	{
// 		KIOView* pView = GetIOView();
// 
// 		if(NULL != pView)
// 		{
// 			// todo : modify this code
// 			//pView->commandTableView( ID_TABLEVIEWMENU_IMPORT );
// 		}
// 	}
// }
// 
// 
// void CChildFrame::OnUpdateTablemanagerImport( CCmdUI* pCmdUI )
// {
// 	updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnTablemanagerExport()
// {
// 	BOOL bEnable = FALSE;
// 	if(true == m_bShowIOView)
// 	{
// 		KIOView* pView = GetIOView();
// 
// 		if(NULL != pView)
// 		{
// 			// todo : modify this code
// 			//pView->commandTableView( ID_TABLEVIEWMENU_EXPORT );
// 		}
// 	}
// }
// 
// 
// void CChildFrame::OnUpdateTablemanagerExport( CCmdUI* pCmdUI )
// {
// 	updateTableMenuState( pCmdUI );
// }
// 
// 
// void CChildFrame::OnTablemanagerReloadcache()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// }
// 
// 
// void CChildFrame::OnUpdateTablemanagerReloadcache( CCmdUI* pCmdUI )
// {
// 	/// $task : Releoad cache 구현 후 아래 수정
// 	pCmdUI->Enable( FALSE );
// 	//updateTableMenuState( pCmdUI );
// }
