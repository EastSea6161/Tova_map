#include "stdafx.h"
#include "QBicIPAddress.h"
#pragma comment(lib, "Iphlpapi.lib")
#include <Iphlpapi.h>

bool QBicIPAddress::GetEthernetMacAddress(CStringArray& ar_strArrMacAddress)
{   
    CString strOutMacAddress = _T("");

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];

    if (pAdapterInfo == NULL)
        return false;

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        delete pAdapterInfo;
        pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];
        if (pAdapterInfo == NULL)
            return false;
    }

    bool bFind(false);
    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            /************************************************************************/
            /*   // 17.07.05 (pAdapter->Type == IF_TYPE_IEEE80211) 추가함
             추후 문제가 발생시 제거 바람*/
            /************************************************************************/
            if (pAdapter->Type == MIB_IF_TYPE_ETHERNET || pAdapter->Type == IF_TYPE_IEEE80211) 
            {
                CStringA strMacAddress;
                strMacAddress.Format("%02X-%02X-%02X-%02X-%02X-%02X",
                    pAdapter->Address[0],
                    pAdapter->Address[1],
                    pAdapter->Address[2],
                    pAdapter->Address[3],
                    pAdapter->Address[4],
                    pAdapter->Address[5]);

                CStringA strIPAddress;
                strIPAddress.Format("%s", pAdapter->IpAddressList.IpAddress.String);

                strOutMacAddress = CA2T(strMacAddress);
                ar_strArrMacAddress.Add(strOutMacAddress);
                bFind = true;
            }

            pAdapter = pAdapter->Next;
        }
    }

    delete pAdapterInfo;
    pAdapterInfo = NULL;

    return bFind;
}
