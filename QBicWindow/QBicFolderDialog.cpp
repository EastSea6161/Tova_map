#include "stdafx.h"
#include "QbicFolderDialog.h"

QBicFolderDialog::QBicFolderDialog(CWnd* pParentWnd) : CCommonDialog(pParentWnd)
{
    m_strInitPath = _T("C:\\");
    TCHAR pszPath[MAX_PATH]= {0};
    
    m_sPath = _T("");
    LPCTSTR pszTitle = _T( "Select the root folder for the browse dialog:" );
    UINT uFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    m_bi.hwndOwner      = pParentWnd->GetSafeHwnd();
    m_bi.pidlRoot       = NULL;
    m_bi.lpszTitle      = pszTitle;
    m_bi.ulFlags        = uFlags;
    m_bi.lpfn           = (BFFCALLBACK)&BrowseCallbackProc;
    m_bi.lParam         = (LPARAM)m_strInitPath.GetBuffer();    
    m_bi.pszDisplayName = pszPath/*new TCHAR[ MAX_PATH ]*/;
    m_strInitPath.ReleaseBuffer();
    ASSERT( m_bi.pszDisplayName != NULL );
}


void QBicFolderDialog::SetInitPath( CString strPath )
{
    m_strInitPath = strPath;
    m_bi.lParam   = (LPARAM)m_strInitPath.GetBuffer();   
    m_strInitPath.ReleaseBuffer();
}

INT_PTR QBicFolderDialog::DoModal()
{
    INT_PTR nRet = -1;
    m_bi.hwndOwner = PreModal();
    LPITEMIDLIST pItemIDList = ::SHBrowseForFolder( &m_bi );
    if( pItemIDList ) {
        TCHAR szFolPath[ MAX_PATH ];
        if( ::SHGetPathFromIDList( pItemIDList, szFolPath ) == TRUE) {
            nRet = IDOK;
            m_sPath = CString(szFolPath);
        }
    }
    else
        nRet = IDCANCEL;

    PostModal();

    return ( nRet );
}

INT_PTR CALLBACK QBicFolderDialog::BrowseCallbackProc( IN HWND hWnd, IN UINT uMsg, IN LPARAM lParam, IN LPARAM lpData )
{
    switch (uMsg)
    {
    case  BFFM_INITIALIZED:
        {
            ::SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)lpData);
        }
        break;
    }
    return 0;
}
