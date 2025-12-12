#include "stdafx.h"
#include "ImChampFileExist.h"
#include "Target.h"
#include "NetworkEditLog.h"

bool ImChampFileExist::IsExistBushFileMessage( KTarget* a_pTarget )
{
    CString strLocation(_T(""));  
    CString strFilePath(_T(""));

    strLocation.Format(_T("%s\\PATHVOLUME\\"), a_pTarget->GetLocation());
    strFilePath = strLocation + _T("*.bsh");

    CFileFind file;
    BOOL bResult = file.FindFile(strFilePath);

    if(FALSE == bResult)
    {
        CString strMsg(_T("")); {
            strMsg.Append(_T("경로 분석 결과 정보가 없습니다.\r\n"));
            strMsg.Append(_T("메뉴 > 모형 > 도로 통행배정 모형을 먼저 실행해 주세요.\r\n"));
            strMsg.Append(_T("(실행 시,\r\n"));
            strMsg.Append(_T(" > 균형 배정법 : SOBA 선택 \r\n"));
            strMsg.Append(_T(" > 실행 옵션 선택에서 경로 교통량 분석을 선택해야 함)\r\n"));
        }
        AfxMessageBox(strMsg);
        return false;
    }

    return true;
}

bool ImChampFileExist::BushFileCheckMessage(KTarget* pTarget)
{
    CString strFile(_T("")); {
        strFile.Format(_T("%s\\PATHVOLUME\\*.bsh"), pTarget->GetLocation());
    }

    CFileFind oFinder; 
    BOOL bR = oFinder.FindFile(strFile);
    if (bR == FALSE) {
        CString strMsg(_T("")); {
            strMsg.Append(_T("경로 분석 결과 정보가 없습니다.\r\n"));
            strMsg.Append(_T("메뉴 > 모형 > 도로 통행배정 모형을 먼저 실행해 주세요.\r\n"));
            strMsg.Append(_T("(실행 시,\r\n"));
            strMsg.Append(_T(" > 균형 배정법 : SOBA 선택 \r\n"));
            strMsg.Append(_T(" > 실행 옵션 선택에서 경로 교통량 분석을 선택해야 함)\r\n"));
        }
        AfxMessageBox(strMsg);

        return false;
    }

    CTime oFileTime;
    while (oFinder.FindNextFile() == TRUE) {
        if (oFinder.IsDirectory() == TRUE)
            continue;

        bR = oFinder.GetCreationTime(oFileTime);
        if (bR == FALSE)
            continue;

        break;
    }

    CTime oEditTime = KNetworkEditLog::GetLastEditTime(pTarget);
    if (oFileTime < oEditTime) {
        AfxMessageBox(_T("네트워크가 편집이 되어 \n분석 결과가 상이할 수 있으니 유의하시길 바랍니다."));
    }
        
    return true;
}

bool ImChampFileExist::MultiModalFileCheckMessage( KTarget* pTarget)
{
    CString strFile(_T("")); {
        strFile.Format(_T("%s%s%s\\TransitPathAssign\\*.dat"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME);
    }

    CFileFind oFinder; 
    BOOL bR = oFinder.FindFile(strFile);
    if (bR == FALSE) {
        CString strMsg(_T("대중교통 통행배정 결과 정보가 존재하지 않습니다.\n모형 > 대중교통 통행배정 모형을 실행해 주세요."));
        AfxMessageBox(strMsg);
        return false;
    }
       
    CTime oFileTime;
    while (oFinder.FindNextFile() == TRUE) {
        if (oFinder.IsDirectory() == TRUE)
            continue;

        bR = oFinder.GetCreationTime(oFileTime);
        if (bR == FALSE)
            continue;

        break;
    }

    CTime oEditTime = KNetworkEditLog::GetLastEditTime(pTarget);
    if (oFileTime < oEditTime) {
        AfxMessageBox(_T("네트워크가 편집이 되어 \n분석 결과가 상이할 수 있으니 유의하시길 바랍니다."));
    }

    return true;
}

bool ImChampFileExist::InterModalFileCheckMessage( KTarget* pTarget )
{
    CString strFile(_T("")); {
        strFile.Format(_T("%s%s%s*.dat"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_INTERMODAL_FOLDER_NAME);
    }

    CFileFind oFinder; 
    BOOL bR = oFinder.FindFile(strFile);
    if (bR == FALSE) {
        CString strMsg(_T("복합수단 통행배정 결과가 존재하지 않습니다.\n모형 > 복합수단 통행배정 모형을 먼저 실행해 주세요. "));
        AfxMessageBox(strMsg);
        return false;
    }

    CTime oFileTime;
    while (oFinder.FindNextFile() == TRUE) {
        if (oFinder.IsDirectory() == TRUE)
            continue;

        bR = oFinder.GetCreationTime(oFileTime);
        if (bR == FALSE)
            continue;

        break;
    }

    CTime oEditTime = KNetworkEditLog::GetLastEditTime(pTarget);
    if (oFileTime < oEditTime) {
        AfxMessageBox(_T("네트워크가 편집이 되어 \n분석 결과가 상이할 수 있으니 유의하시길 바랍니다."));
    }

    return true;
}

bool ImChampFileExist::IsExistMultiModalFileMessage( KTarget* pTarget )
{
    CString strDataFileName(_T(""));  
    CString strFilePath(_T(""));

    strFilePath.Format(_T("%s%s%s\\TransitPathAssign\\"), pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME);
    strFilePath.Append(_T("*.dat"));

    CFileFind file;
    BOOL bResult = file.FindFile(strFilePath);

    if(FALSE == bResult)
    {
        CString strMsg(_T("대중교통 통행배정 결과 정보가 존재하지 않습니다.\n모형 > 대중교통 통행배정 모형을 실행해 주세요."));
        AfxMessageBox(strMsg);
        return false;
    }

    return true;
}

bool ImChampFileExist::IsExistInterModalGeneraterFileMessage( KTarget* a_pTarget )
{
	try
	{
		CString strLocation(_T(""));  
		CString strFilePath(_T(""));

		strLocation.Format(_T("%s\\%s\\%s%s"), a_pTarget->GetLocation(), DLL_MODEL_FOLDER_NAME, INTERMODAL_GENERATOR_PATH, INTERMODAL_GEN_RESULT_PATH );
		strFilePath = strLocation + _T("*.dat");

		CFileFind file;
		BOOL bResult = file.FindFile(strFilePath);

		if(FALSE == bResult)
		{
			CString strMsg(_T(""));
			strMsg.Append(_T("복합수단 경로 생성 결과 정보가 존재하지 않습니다."));
			strMsg.Append(_T("\n메뉴 > 경로 만들기 > 복합수단 경로 생성을 먼저 실행해 주세요."));
			
			AfxMessageBox(strMsg);
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


