#pragma once

#include "afxdlgs.h"
/**
*@brief 폴더 다이얼로그
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicFolderDialog : public CCommonDialog
{
public:
    QBicFolderDialog(CWnd* pParentWnd);
private:
    CString m_sPath;
    BROWSEINFO  m_bi;
    static INT_PTR CALLBACK BrowseCallbackProc( IN HWND hWnd, IN UINT uMsg, IN LPARAM lParam, IN LPARAM lpData );
private:
    CString m_strInitPath;
public:
    void SetInitPath(CString strPath);
public:
    INT_PTR DoModal();
public:
    CString GetPath() {
        return m_sPath;
    }

    CString GetPathName() {
        int nLen = m_sPath.GetLength();
        if (nLen == 0)
            return _T("");

        int nIdx = m_sPath.ReverseFind(_T('\\'));
        nIdx++;
        if (nIdx >= nLen)
            return _T("");

        return m_sPath.Mid(nIdx, nLen-nIdx);
    }
};
