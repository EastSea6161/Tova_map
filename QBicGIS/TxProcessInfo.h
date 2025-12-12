#pragma once
#include <psapi.h>
/**
*@brief 프로세스 정보
*@details 프로세스 정보
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxProcessInfo
{
private:
    TxProcessInfo();
public:
    ~TxProcessInfo();

public:
	/**
	*@brief 윈도우 핸들로 프로세스 아이디 얻기
	*@param HWND hwnd	윈도우 핸들
	*@return 프로세스 ID
	*/
    static DWORD ProcessIDFromHWnd(HWND hwnd) // 윈도우 핸들로 프로세스 아이디 얻기   
    {   
        DWORD idProc;   
        GetWindowThreadProcessId( hwnd, &idProc );   
        return idProc;   
    }
	/**
	*@brief 프로세스 아이디로 윈도우 핸들 얻기
	*@param DWORD pid	프로세스 ID
	*@return 윈도우 핸들 / 실패 NULL
	*/
    static HWND GetWinHandle(DWORD pid) // 프로세스 아이디로 윈도우 핸들 얻기   
    {   
        HWND tempHwnd = FindWindow(NULL,NULL); // 최상위 윈도우 핸들 찾기   

        while( tempHwnd != NULL )   
        {   
            if( GetParent(tempHwnd) == NULL ) // 최상위 핸들인지 체크, 버튼 등도 핸들을 가질 수 있으므로 무시하기 위해   
                if( pid == ProcessIDFromHWnd(tempHwnd) )   
                    return tempHwnd;   
            tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT); // 다음 윈도우 핸들 찾기   
        }   
        return NULL;   
    } 
public:
	/**
	*@brief 프로세스 정보 표출
	*@param DWORD processID	프로세스 ID
	*@return 프로세스 정보 표출
	*/
    static CString PrintGetProcessInfo(DWORD processID)
    {
        CString strOut(_T(""));
        HANDLE  hProcess;
        PROCESS_MEMORY_COUNTERS pmc;

        strOut.AppendFormat(_T("$$$#Process ID: [%u]"), processID );

        hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
        if (NULL == hProcess)
            return strOut;
        if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
        {
            DWORD dwMegaByte = 1024 * 1024;
            
            strOut.AppendFormat(_T( "\tPeakWorkingSetSize: %ldMb"),       pmc.PeakWorkingSetSize / dwMegaByte);
            strOut.AppendFormat(_T( "\tWorkingSetSize: %ldMb"),           pmc.WorkingSetSize / dwMegaByte);
            strOut.AppendFormat(_T( "\tPageFaultCount: %ldMb"),           pmc.PageFaultCount / dwMegaByte);
            strOut.AppendFormat(_T( "\tQuotaPeakPagedPoolUsage: %ld"),    pmc.QuotaPeakPagedPoolUsage);
            strOut.AppendFormat(_T( "\tQuotaPagedPoolUsage: %ld"),        pmc.QuotaPagedPoolUsage);
            strOut.AppendFormat(_T( "\tQuotaPeakNonPagedPoolUsage: %ld"), pmc.QuotaPeakNonPagedPoolUsage);
            strOut.AppendFormat(_T( "\tQuotaNonPagedPoolUsage: %ld"),     pmc.QuotaNonPagedPoolUsage);
            strOut.AppendFormat(_T( "\tPagefileUsage: %ldMb"),            pmc.PagefileUsage / dwMegaByte); 
            strOut.AppendFormat(_T( "\tPeakPagefileUsage: %ldMb"),        pmc.PeakPagefileUsage / dwMegaByte);
        }

        CloseHandle( hProcess );
        return strOut;
    }
};

