#include "stdafx.h"
#include "QBicDir.h"
#include "QBicFile.h"

void QBicDir::CreateDirectory( CString a_strPath )
{
    try
    {
        ::CreateDirectory(a_strPath, NULL);
    }
    catch (KExceptionPtr ex)
    {
        throw ex;
    }
    catch (...)
    {
        ThrowException(_T("At CreateDirectory")); 
    }    
}

void QBicDir::RemoveDirectory( CString a_strPath )
{
    try
    {
        CFileFind finder;
        BOOL bContinue = TRUE;

        if(a_strPath.Right(1) != _T("\\"))
            a_strPath += _T("\\");

        a_strPath += _T("*.*");
        bContinue = finder.FindFile(a_strPath);

        while(bContinue) {
            bContinue = finder.FindNextFile();
            if(finder.IsDots()) 
            {
                continue;
            }
            else if(finder.IsDirectory()) 
            {
                RemoveDirectory(finder.GetFilePath());
                ::RemoveDirectory((LPCTSTR)finder.GetFilePath());
            }
            else 
            {
                CString strDeleteFile = finder.GetFilePath();
                ::DeleteFile((LPCTSTR)strDeleteFile);            
            }
        }

        finder.Close();

        a_strPath = a_strPath.Left(a_strPath.ReverseFind('\\'));   
        ::RemoveDirectory((LPCTSTR)a_strPath);
    }
    catch (KExceptionPtr ex)
    {
    	throw ex;
    }
    catch (...)
    {
    	ThrowException(_T("At RemoveDirectory")); 
    }    
}

void QBicDir::CopyDirectory( CString a_strSourcePath, CString a_strDestPath, bool a_bSubDirCopy )
{
    try
    {
        WIN32_FIND_DATA w32FindData;
        HANDLE hFileSearch = FindFirstFile(a_strSourcePath + _T("\\*.*"), &w32FindData);
        if(INVALID_HANDLE_VALUE != hFileSearch)
        {
            ::CreateDirectory(a_strDestPath, NULL);

            CString strSrcPath, strDestPath;
            do 
            {
                if (_tcscmp(w32FindData.cFileName, _T(".")) != 0 && _tcscmp(w32FindData.cFileName, _T("..")) != 0)
                {
                    if (a_bSubDirCopy)
                    {
                        if (w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
                        { 
                            strSrcPath.Format (_T("%s\\%s"), a_strSourcePath, w32FindData.cFileName);
                            strDestPath.Format(_T("%s\\%s"), a_strDestPath,   w32FindData.cFileName);
                            CopyDirectory(strSrcPath, strDestPath, a_bSubDirCopy);  
                        }
                        else 
                        {
                            strSrcPath.Format (_T("%s\\%s"), a_strSourcePath, w32FindData.cFileName);
                            strDestPath.Format(_T("%s\\%s"), a_strDestPath,   w32FindData.cFileName);
                            CopyFile(strSrcPath, strDestPath, FALSE);
                        }
                    }
                    else
                    {
                        strSrcPath.Format (_T("%s\\%s"), a_strSourcePath, w32FindData.cFileName);
                        strDestPath.Format(_T("%s\\%s"), a_strDestPath,   w32FindData.cFileName);
                        CopyFile(strSrcPath, strDestPath, FALSE);
                    }
                }
                // 파일 속성 검사 : FILE_ATTRIBUTE_ARCHIVE*/FILE_ATTRIBUTE_NORMAL & w32FindData.dwFileAttributes
            } while(FindNextFile(hFileSearch, &w32FindData));

            FindClose(hFileSearch);
        }	
    }
    catch (KExceptionPtr ex)
    {
        throw ex;
    }
    catch (...)
    {
        ThrowException(_T("At CopyDirectory")); 
    }    
}

CString QBicDir::WindowsSpecialFolderPath(HWND hwndOwner, int CSIDL)
{
    // 주의: Windows Vista 이전 버전에서만 사용 가능
    // Windows에서 관리하는 특별한 폴더를 조회
    char pBuffer[MAX_PATH]; 
    SHGetSpecialFolderPath(hwndOwner/*GetSafeHwnd()*/, (LPWSTR)pBuffer, CSIDL/*CSIDL_INTERNET_CACHE, CSIDL_DESKTOP*/, 0); 
    
    return CString(pBuffer);
}

CString QBicDir::WindoswsGetKnownFolderPath( GUID ID )
{
    // Windows Vista 이후 버전에서 사용
    LPWSTR wszPath = NULL;
    SHGetKnownFolderPath(ID/*FOLDERID_InternetCache*/, KF_FLAG_CREATE, nullptr, &wszPath);

    return CString(wszPath);
}

std::vector<CString> QBicDir::DirFiles( CString strPath )
{
    // 현재 폴더에 있는 파일들의 경로정보리턴
    // 서브 폴더는 리턴하지 않는다.

    std::vector<CString> vecFiles;
    try
    {
        WIN32_FIND_DATA w32FindData;
        HANDLE hFileSearch = FindFirstFile(strPath + _T("\\*.*"), &w32FindData);
        if(INVALID_HANDLE_VALUE != hFileSearch)
        {
            CString strSrcPath;
            do 
            {
                if (_tcscmp(w32FindData.cFileName, _T(".")) != 0 && _tcscmp(w32FindData.cFileName, _T("..")) != 0)
                {
                    if ((w32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false) {
                        strSrcPath.Format (_T("%s"), w32FindData.cFileName);
                        vecFiles.push_back(strSrcPath);
                    }
                    
                }
                // 파일 속성 검사 : FILE_ATTRIBUTE_ARCHIVE*/FILE_ATTRIBUTE_NORMAL & w32FindData.dwFileAttributes
            } while(FindNextFile(hFileSearch, &w32FindData));

            FindClose(hFileSearch);
        }	

        return vecFiles;
    }
    catch (KExceptionPtr ex)
    {
        throw ex;
    }
    catch (...)
    {
        ThrowException(_T("At DirFiles")); 
    }    
}

int QBicDir::CreateDir( CString strPath )
{    
    bool bExist = QBicFile::FileExist(strPath);
    if ( bExist == true)
        return 2;

    int nResult(0);
    try{
        if ( ::CreateDirectory(strPath, NULL) == TRUE)
            nResult = 1;
        else 
            nResult = 0;
    } 
    catch (...)
    {
        ThrowException(_T("At CreateDir")); 
    }    

    return nResult;
}
