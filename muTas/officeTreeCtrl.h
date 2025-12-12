/**
 * @file officeTreeCtrl.h
 * @brief COfficeTreeCtrl 선언, 구현
 * @author 
 * @date 2010.07.27
 * @remark
 */


#pragma once



/**
* @brief CProjectView의 Tree control
* @remark Project와 그 구성을 트리형태로 표출한다.
* @warning
* @version 1.0
* @author 
* @date 2010.07.27
*/
class COfficeTreeCtrl : public CXTPOfficeBorder< CTreeCtrl >
{
	/**************************************
	* Overrides                           *
	**************************************/
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT *pResult )
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;
		switch( pNMHDR->code )
		{
		case TTN_SHOW : 
			{
				HWND hWnd = TreeView_GetToolTips( m_hWnd );
				if( NULL != hWnd )
				{
					::SetWindowPos( hWnd, HWND_TOP, 0, 0, 0, 0, 
						SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
				}
			}
			break;
		}
		return CXTPOfficeBorder< CTreeCtrl >::OnNotify(wParam, lParam, pResult );
	}
};