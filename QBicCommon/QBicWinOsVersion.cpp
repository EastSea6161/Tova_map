#include "stdafx.h"
#include "QBicWinOsVersion.h"

void QBicWinOsVersion::WindowsOsVersion( DWORD& dwMajorVersion, DWORD& dwMinorVersion )
{
    OSVERSIONINFOEX osVersionInfoEx;
    ZeroMemory(&osVersionInfoEx, sizeof(OSVERSIONINFOEX));
    osVersionInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((LPOSVERSIONINFOW)&osVersionInfoEx);

    dwMajorVersion = osVersionInfoEx.dwMajorVersion;
    dwMinorVersion = osVersionInfoEx.dwMinorVersion;
}

CString QBicWinOsVersion::WindowsOsVersion()
{
    CString strOsVersion(_T(""));

    OSVERSIONINFOEX osVersionInfoEx;
    ZeroMemory(&osVersionInfoEx, sizeof(OSVERSIONINFOEX));
    osVersionInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((LPOSVERSIONINFOW)&osVersionInfoEx);

    switch (osVersionInfoEx.dwMajorVersion)
    {
    case 5:
        {
            switch (osVersionInfoEx.dwMinorVersion)
            {
            case 1:
                strOsVersion = _T("Windows XP");
                break;
            case 2:
                if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows XP Professional x64 Edition");
                    break;
                }
                else if (GetSystemMetrics(SM_SERVERR2) == 0)
                {
                    strOsVersion = _T("Windows Server 2003");
                    break;
                }
                else if (osVersionInfoEx.wSuiteMask & VER_SUITE_WH_SERVER)
                {
                    strOsVersion = _T("Windows Home Server");
                    break;
                }
                else if (GetSystemMetrics(SM_SERVERR2) != 0)
                {
                    strOsVersion = _T("Windows Server 2003 R2");
                    break;
                }
                break;
            default:
                strOsVersion.Format(_T("Can't find version name of dwMiniorVersion: %d"), osVersionInfoEx.dwMinorVersion);
                break;
            }
            break;
        }
    case 6:
        {
            switch (osVersionInfoEx.dwMinorVersion)
            {
            case 0:
                if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows Vista");
                    break;
                }
                else if (osVersionInfoEx.wProductType != VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows Server 2008");
                    break;
                }
                break;
            case 1:
                if (osVersionInfoEx.wProductType != VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows Server 2008 R2");
                    break;
                }
                else if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows 7");
                    break;
                }
                break;
            case 2:
                if (osVersionInfoEx.wProductType != VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows Server 2012");
                    break;
                }
                else if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows 8");
                    break;
                }
                break;
            case 3:
                if (osVersionInfoEx.wProductType != VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows Server 2012 R2");
                    break;
                }
                else if (osVersionInfoEx.wProductType == VER_NT_WORKSTATION)
                {
                    strOsVersion = _T("Windows 8.1");
                    break;
                }
                break;
            default:
                strOsVersion.Format(_T("Can't find version name of dwMiniorVersion: %d"), osVersionInfoEx.dwMinorVersion);
                break;
            }
            break;
        }
    default:
        break;
    }

    return strOsVersion;
}
