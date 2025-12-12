#include "stdafx.h"
#include "QBicWindowSystem.h"


int QBicWindowSystem::ProcessOnWindowsStatus()
{
    try
    {
        SYSTEM_INFO oSystemInfo ;
        GetNativeSystemInfo(&oSystemInfo);

        if (oSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        {
            // 64Bit 장비로 확인 된 경우
            BOOL bIsWow64 = FALSE;
            IsWow64Process(GetCurrentProcess(),&bIsWow64);
            if (bIsWow64 == TRUE)
            {
                //AfxMessageBox(_T("32Bit 프로그램이 64Bit 환경에서 구동 중"));
                return 1;
            }
            else
            {
                //AfxMessageBox(_T("64Bit 프로그램이 64Bit 환경에서 구동중"));
                return 2;
            }
        }
        else
        {        
            //AfxMessageBox(_T("// Only 32Bit, 64Bit Not Running"));
            return 0;
        }
    }
    catch (...)
    {	
    }    

    return -1;
}
