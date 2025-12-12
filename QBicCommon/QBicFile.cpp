#include "stdafx.h"
#include "QBicFile.h"
#include <sys/stat.h>
#include "CString2MBString.h"

bool QBicFile::FileExist(CString strFilePath)
{
    if ( PathFileExists(strFilePath) == FALSE )
        return false;
    
    return true;
}

void QBicFile::DeleteFile( CString strFilePath )
{
    ::DeleteFile((LPCTSTR)strFilePath);
}

bool QBicFile::CopyFile( CString strSrcPath, CString strDestPath )
{
    if ( ::CopyFile(strSrcPath, strDestPath, FALSE) == TRUE)
        return true;

    return false;
}

std::vector<CString> QBicFile::SplitFilePath( CString strPath )
{//2015-12-30 By nombara    
    TCHAR  szDrive[64], szDir[512], szName[512], szExt[64];

    TCHAR* szPath = (TCHAR*)(LPCTSTR)strPath;    
    _tsplitpath_s(szPath, szDrive, szDir, szName, szExt);

    CString strDrive, strDir, strName, strExt; {
        strDrive.Format(_T("%s"), szDrive);
        strDir.Format(_T("%s"),   szDir); {
            CString strRDir(_T(""));
            int nLen = strDir.GetLength();
            for (int i=0; i<nLen; i++) {
                TCHAR szTemp = strDir.GetAt(i);
                if ((i == 0 || i == nLen -1) && szTemp == _T('\\')) {
                } else {
                    strRDir.AppendFormat(_T("%s"), CString(szTemp));
                }
            }
            strDir = strRDir;
        }
        strName.Format(_T("%s"), szName);
        
        CString strTemp(_T("")); {
            strTemp.Format(_T("%s"), szExt);
        }
        
        int nFind = strTemp.Find(_T('.'));
        if (nFind == 0) {
            strExt = strTemp.Mid(1);
        } 
        else {
            strExt = strTemp;
        }
    }
    
    std::vector<CString> vecInfo; {
        vecInfo.push_back(strDrive);
        vecInfo.push_back(strDir);
        vecInfo.push_back(strName);
        vecInfo.push_back(strExt);
    }

    return vecInfo;
}
