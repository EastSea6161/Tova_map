#include "StdAfx.h"
#include "ImChampDir.h"
#include "Target.h"

ImChampDir::ImChampDir(void)
{
}


ImChampDir::~ImChampDir(void)
{
}

CString ImChampDir::SetCurrentDirectoryToApp()
{
	TCHAR    szAppPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szAppPath, _MAX_PATH));		

	CString  strAppPath(szAppPath);

	int nIndex  = strAppPath.ReverseFind(_T('\\'));
	if (nIndex > 0) 
	{
		strAppPath = strAppPath.Left(nIndex);
	}
	else 
	{
		strAppPath.Empty();
	}       

	::SetCurrentDirectory(strAppPath);

    return strAppPath;
}

CString ImChampDir::GetAppDirectory()
{
	TCHAR    szAppPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szAppPath, _MAX_PATH));		

	CString  strAppPath(szAppPath);

	int nIndex  = strAppPath.ReverseFind(_T('\\'));
	if (nIndex > 0) 
	{
		strAppPath = strAppPath.Left(nIndex);
	}
	else 
	{
		strAppPath.Empty();
	}  

	return strAppPath;
}

CString ImChampDir::SetCurDirToTarget( KTarget* a_pTarget )
{
    CString strTargetPath = a_pTarget->GetLocation();
    ::SetCurrentDirectory(strTargetPath);

    return strTargetPath;
}

CString ImChampDir::SetCurDirToTargetSubDir( KTarget* a_pTarget, CString a_strFolder )
{
    CString strTargetPath = a_pTarget->GetLocation();
    CString strPath       = strTargetPath + _T("\\" + a_strFolder);

    ::SetCurrentDirectory(strPath);

    return strPath;
}

CString ImChampDir::SetCurDirToTargetModel( KTarget* a_pTarget )
{
    CString strTargetPath = a_pTarget->GetLocation();
    CString strPath       = strTargetPath + _T("\\" + DLL_MODEL_FOLDER_NAME);

    ::SetCurrentDirectory(strPath);

    return strPath;
}

CString ImChampDir::GetCurDir()
{
    TCHAR szCurrentDirectory[MAX_PATH] = {0};
    ::GetCurrentDirectory( MAX_PATH, szCurrentDirectory );
   
    CString strPath(szCurrentDirectory);
    return strPath;
}

CString ImChampDir::GetScenarioDirectory( KTarget* a_pTarget )
{
    try
    {
        //a_pTarget->GetParentObject()->GetParentObject()
    	//a_pTarget->GetParentObject()->
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
    return _T("");
}

CString ImChampDir::GetProjectDirectory( KTarget* a_pTarget )
{
     return _T("");
}

void ImChampDir::RemoveDirectory( CString a_strPath, QBicSimpleProgressPtr spSimpleProgress )
{
    CFileFind finder;
    BOOL bContinue = TRUE;

    if(a_strPath.Right(1) != _T("\\"))
        a_strPath += _T("\\");

    a_strPath += _T("*.*");
    bContinue = finder.FindFile(a_strPath);
    while(bContinue)
    {
        bContinue = finder.FindNextFile();
        if(finder.IsDots()) 
        {
            continue;
        }
        else if(finder.IsDirectory()) 
        {
            RemoveDirectory(finder.GetFilePath(), spSimpleProgress);
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


void ImChampDir::CopyDirectory( CString a_strSourcePath, CString a_strDestPath, bool a_bSubDirCopy, QBicSimpleProgressPtr spSimpleProgress )
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
                            CopyDirectory(strSrcPath, strDestPath, a_bSubDirCopy, spSimpleProgress);  
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
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}

void ImChampDir::CreateDirectory( CString a_strPath )
{
    try
    {
    	::CreateDirectory(a_strPath, NULL);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }    
}

void ImChampDir::CopyDirectoryWinSimpleProgress( CString a_strFromDir, CString a_strToDir )
{
	SHFILEOPSTRUCT shfo;
	int            nResult;
	
	try
	{
		//Important  You must ensure that the source and destination paths are double-null terminated. 
		//A normal string ends in just a single null character. 
		//If you pass that value in either the source or destination members, 
		//the function will not realize when it has reached the end of the string and will continue to read on in memory until 
		//it comes to a random double null value. This can at least lead to a buffer overrun, and possibly the unintended deletion of unrelated data.
		CString strFromDir = a_strFromDir + '\0';
		CString strToDir   = a_strToDir + '\0';

		shfo.hwnd   = nullptr;
		shfo.wFunc  = FO_COPY;
		shfo.pFrom  = strFromDir;
		shfo.pTo    = strToDir;
		shfo.fFlags = FOF_SIMPLEPROGRESS;
		shfo.lpszProgressTitle = _T("아쉽네...");

		nResult = SHFileOperation(&shfo);

		//오류 코드관련 정의는 QBicDev\ImChamp\_doc\SHFILEOPSTRUCT_errorcodes.txt 문서를 참조
		if (nResult != 0)
		{
			switch (nResult)
			{
			case 2 :
				ThrowException(_T("폴더를 찾을 수 없습니다."));
				break;
			case 1223 :
				ThrowException(_T("취소되었습니다."));//사용자 취소
				break;
			default:
				ThrowException(_T("폴더 복사에 실패했습니다."));
				break;
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("복사중 알수 없는 오류가 발생했습니다."));
	}
}
