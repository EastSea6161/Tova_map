#include "stdafx.h"
#include "ImTasLicenseCheck.h"
#include "..\QBicCommon\QBicMd5.h"
#include "..\QBicCommon\QBicIPAddress.h"
#include <comdef.h>
#include <WbemIdl.h>
#pragma comment(lib, "wbemuuid.lib")


//CString ImTasLicenseChecker::extractSerialNumber() {
//    CString serialNumber;
//    wchar_t path[MAX_PATH];
//    if (GetModuleFileNameW(NULL, path, MAX_PATH)) {
//        std::wstring wsPath(path);
//        size_t pos = wsPath.find_last_of(L"\\/");
//        if (pos != std::wstring::npos) {
//            wsPath = wsPath.substr(0, pos);
//        }
//        wsPath += L"\\SerialNumber.txt";
//
//        std::wifstream file(wsPath);
//        if (file) {
//            std::wcout << L"Reading from file: " << wsPath << std::endl;
//            std::wstring line;
//            while (getline(file, line)) {
//                // Trim any leading/trailing whitespace from the line
//              // 앞뒤 공백 제거
//                line = std::wstring(line.c_str());
//                line.erase(0, line.find_first_not_of(L" \t\r\n"));
//                line.erase(line.find_last_not_of(L" \t\r\n") + 1);
//                serialNumber += line.c_str();  // 파일에서 읽은 문자열을 그대로 CString에 추가
//            }
//            std::wcout << L"Extracted serial number: " << (LPCTSTR)serialNumber << std::endl;
//            file.close();
//        }
//        else {
//            std::wcerr << L"Failed to open file: " << wsPath << std::endl;
//        }
//    }
//    else {
//        std::wcerr << L"Failed to get executable path" << std::endl;
//    }
//    return serialNumber;
//}

static inline std::wstring Trim(const std::wstring& s) {
    const auto first = s.find_first_not_of(L" \t\r\n");
    if (first == std::wstring::npos) return L"";
    const auto last = s.find_last_not_of(L" \t\r\n");
    return s.substr(first, last - first + 1);
}


CString ImTasLicenseChecker::extractSerialNumber()
{
    CString serialNumber;
    wchar_t path[MAX_PATH]{};

    if (!GetModuleFileNameW(nullptr, path, MAX_PATH))
        return _T("");

    std::wstring wsPath(path);
    size_t pos = wsPath.find_last_of(L"\\/");
    if (pos != std::wstring::npos) wsPath = wsPath.substr(0, pos);
    wsPath += L"\\SerialNumber.txt";

    std::wifstream file(wsPath);
    if (!file) return _T("");

    std::wstring line;
    while (std::getline(file, line)) {
        std::wstring trimmed = Trim(line);
        if (!trimmed.empty()) {
            serialNumber = trimmed.c_str();
            break;
        }
    }
    return serialNumber;
}

CString ImTasLicenseChecker::GetSystemUUID()
{
    CString result;

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    const bool needUninit = (hr == S_OK || hr == S_FALSE);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) return result;

    (void)CoInitializeSecurity(nullptr, -1, nullptr, nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr, EOAC_NONE, nullptr);

    CComPtr<IWbemLocator>  pLoc;
    CComPtr<IWbemServices> pSvc;

    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pLoc));
    if (FAILED(hr) || !pLoc) { if (needUninit) CoUninitialize(); return result; }

    hr = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, 0, 0, 0, 0, &pSvc);
    if (FAILED(hr) || !pSvc) { if (needUninit) CoUninitialize(); return result; }

    (void)CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE);

    CComPtr<IEnumWbemClassObject> pEnum;
    hr = pSvc->ExecQuery(bstr_t("WQL"),
        bstr_t("SELECT UUID FROM Win32_ComputerSystemProduct"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &pEnum);

    if (SUCCEEDED(hr) && pEnum) {
        CComPtr<IWbemClassObject> pObj; ULONG uRet = 0;
        if (SUCCEEDED(pEnum->Next(WBEM_INFINITE, 1, &pObj, &uRet)) && uRet == 1) {
            CComVariant vt;
            if (SUCCEEDED(pObj->Get(L"UUID", 0, &vt, 0, 0)) && vt.vt == VT_BSTR) {
                std::wstring ws = vt.bstrVal ? vt.bstrVal : L"";
                result = Trim(ws).c_str();
            }
        }
    }
    if (needUninit) CoUninitialize();
    return result; // 빈 문자열이면 읽기 실패
}


bool ImTasLicenseChecker::IsOK()
{
    try
    {
        CString strLicenseMacAddress(_T(""));
        ImTasLicenseInfo oImTasLicenseInfo;

        if (ReadLicenseFile(strLicenseMacAddress, oImTasLicenseInfo) == false)
        {
            TxLogDebugException();
            return false;
        }

        bool bMacAddCheck(false);
        
        CString serialNumber = extractSerialNumber();
        CString systemUUID = GetSystemUUID();

        if (serialNumber.CompareNoCase(systemUUID) != 0) 
        {
            CString msg = _T("SerialNumber.txt 파일과 현재 PC 의 UUID 가 일치하지 않습니다.");
            AfxMessageBox(msg);
			return false;
		}

        QBicMd5 oQBicMd5;    
        CStringArray strArray;
        CString strResult(_T(""));

        if (oQBicMd5.GetMd5String(serialNumber, strResult) == false)
        {
        TxLogDebugException();
        return false;
        }    
        if (strResult.CompareNoCase(strLicenseMacAddress) == 0)
        {
        bMacAddCheck = true;   
        }


        //원본 원본 
        //QBicMd5 oQBicMd5;    
        //CStringArray strArray;
        //QBicIPAddress oIPAddress;        
        //oIPAddress.GetEthernetMacAddress(strArray);
        //INT_PTR nCount = strArray.GetCount();
        //for (INT_PTR i=0; i<nCount; i++)
        //{
        //    CString strMacAddress = strArray.GetAt(i);
        //    CString strResult(_T(""));

        //    if (oQBicMd5.GetMd5String(strMacAddress, strResult) == false)
        //    {
        //        TxLogDebugException();
        //        return false;
        //    }    

        //    if (strResult.CompareNoCase(strLicenseMacAddress) == 0)
        //    {
        //        bMacAddCheck = true;
        //        break;
        //    }
        //}
        //원본 원본
        if (bMacAddCheck == false)
        {
            return false;
        }

        // 여기서 날짜 체크를 합니다.
        QBicTime& oStartDay   = oImTasLicenseInfo.m_oLicStart;
        QBicTime& oEndDay     = oImTasLicenseInfo.m_oLicEnd;
        QBicTime& oLastRunDay = oImTasLicenseInfo.m_oLastRunDay;
        QBicTime  oCurDay;
        
        CString strStartDay(_T("")), strEndDay(_T("")), strLastRunDay(_T("")), strCurDay(_T("")), strExpiredDay(_T(""));
        strStartDay   = oStartDay.ToString();
        strEndDay     = oEndDay.ToString();
        strLastRunDay = oLastRunDay.ToString();
        strCurDay     = oCurDay.ToString();

        int nDaySec(24 * 60 * 60);
        int nLicenseVersion = oImTasLicenseInfo.m_nLicVersion; 
		
        if (nLicenseVersion < 2) {
            oEndDay = QBicTime(2015, 6, 30); //1.0 버전 - 초창기 무료 공개 버전 제약
        }

        oEndDay += nDaySec; //저녁까지 사용하게
                
        strExpiredDay = oEndDay.ToString();

        /*
        TxLogDebug((LPCTSTR) strStartDay);
        TxLogDebug((LPCTSTR) strEndDay);
        TxLogDebug((LPCTSTR) strLastRunDay);
        TxLogDebug((LPCTSTR) strCurDay);
        TxLogDebug((LPCTSTR) strExpiredDay);
        */

        // 현재 일자가 라이센스 시작일 보다 작을 수 없다
        if (oStartDay > oCurDay) {
            return false;
        }

        // 마지막 실행 일자는 현재 보다 작을 수 없다
        if (oLastRunDay > oCurDay ) {
            return false;
        }

        // 마지막 실행 일자는 라이센스 시작일 보다 작을 수 없다
        if (oLastRunDay < oStartDay) {
            return false;
        }

        //★ 2017-04-12(정수호): Start
        //★ 라이선스 시작일자 + 15일 이내에 최초 실행 조건 추가.
        
        //★ 라이선스 시작일자와 마지막 실행일자가 같은 경우는 최초 실행시에만 존재한다. 
        //if (oStartDay == oLastRunDay) {
        //    int nOverDay(15);

        //    //★ 현재일과 라이선스 시작일자의 차이가 15일 이상인 경우
        //    //★ 
        //    __int64 nDayInterval = oCurDay.DayInterval(oStartDay);
        //    if (nDayInterval >= nOverDay) {
        //        return false;
        //    }
        //}
        //★ 2017-04-12 : End

        if (oCurDay > oEndDay)
        {
            int nOverDay(15);
            __int64 nDayInterval = oCurDay.DayInterval(oEndDay);
            if (nDayInterval >= nOverDay) {
                return false;
            }
            else {
                CString strMsg(_T(""));
                if (nDayInterval == 0)
                {
                    strMsg.Format(_T("TOVA License 사용기간이 만료되었습니다.\n현재 버전은 라이센스 만료기간 이후 %d일 더 사용할 수 있습니다."), nOverDay);
                }
                else
                {
                    strMsg.Format(_T("TOVA License 사용기간이 이미 %I64d일 지났습니다.\n현재 버전은 라이센스 만료기간 이후 %d일 더 사용할 수 있습니다."), nDayInterval, nOverDay);
                }
                
                AfxMessageBox(strMsg);
            }
        }

        if (WriteRunningDay() == false)
        {
            TxLogDebugException();
            return false;
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

    return true;
}


bool ImTasLicenseChecker::ReadLicenseFile(CString& ar_strMacAddress, ImTasLicenseInfo& ar_LicenseInfo)
{
    CString strMacAddress(_T(""));        
    CString strAppPath = GetAppPath();
    if (strAppPath.GetLength() < 1)
    {
        TxLogDebugException();
        return false;
    }

    CString strLicenseFileName(_T(""));
    strLicenseFileName.Format(_T("%s\\TOVA.lic"), strAppPath);

#pragma pack(push, 1)
	CFile cfile;
	CFileStatus rStatus;
	CFile::GetStatus(strLicenseFileName, rStatus);
	ULONGLONG ulong = rStatus.m_size;
	if (ulong < 350000) {
		TxLogDebugException();
		return false;
	}
    std::ifstream ifs(strLicenseFileName.GetBuffer(), std::ios::binary);

    if (!ifs.is_open())
    {
        TxLogDebugException();
        return false;
    }
        
    // TODO : 향후 추가 필요
    // step 1 : read version no
    ifs.seekg(802);
    int nVersion(1);
    ifs.read(reinterpret_cast<char*>( &nVersion ), sizeof(int) );  
	if (nVersion <= 0 || nVersion > 100) {	//혁 추가, 라이센스 이상치 체크
		ifs.close();
		TxLogDebugException();
		return false;
	}

    // read 라이센스 시작일자    
    int nStartYear (0);
    int nStartMonth(0);
    int nStartDay  (0);
    ifs.seekg(912);      
    ifs.read(reinterpret_cast<char*>( &nStartYear  ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nStartMonth ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nStartDay   ), sizeof(int) );
	if (nStartYear < 2000 || (nStartMonth < 1 || nStartMonth > 12) || (nStartDay < 1 || nStartDay > 31)) {
		ifs.close();
		TxLogDebugException();
		return false;
	}

    // read 라이센스 종료일자
    int nEndYear  (0);
    int nEndMonth (0);
    int nEndDay   (0);
    ifs.seekg(1100);      
    ifs.read(reinterpret_cast<char*>( &nEndYear  ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nEndMonth ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nEndDay   ), sizeof(int) );
	if (nEndYear < 2000 || (nEndMonth < 1 || nEndMonth > 12) || (nEndDay < 1 || nEndDay > 31)) {
		ifs.close();
		TxLogDebugException();
		return false;
	}

    // read 프로그램 실행일자
    int nRunYear  (0);
    int nRunMonth (0);
    int nRunDay   (0);
    ifs.seekg(1300);      
    ifs.read(reinterpret_cast<char*>( &nRunYear  ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nRunMonth ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nRunDay   ), sizeof(int) );
	if (nRunYear < 2000 || (nRunMonth < 1 || nRunMonth > 12) || (nRunDay < 1 || nRunDay > 31)) {
		ifs.close();
		TxLogDebugException();
		return false;
	}

    int nRunHour   (0);
    int nRunMinute (0);
    int nRunSec    (0);

    ifs.seekg(2300);  
    ifs.read(reinterpret_cast<char*>( &nRunHour  ),  sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nRunMinute ), sizeof(int) );  
    ifs.read(reinterpret_cast<char*>( &nRunSec   ),  sizeof(int) );
	

    // 1.0버전은 문제가 있음. 그래서 안전하게..
    if (nRunHour   >= 24) nRunHour   = 0;
    if (nRunMinute >= 60) nRunMinute = 0;
    if (nRunSec    >= 60) nRunSec    = 0;
	if (nRunHour < 0 || nRunMinute < 0 || nRunSec < 0) {
		ifs.close();
		TxLogDebugException();
		return false;
	}
    
    char szOut[1024] = {0};
    int nByte(0); 

    // read mac-address
    ifs.seekg(7802);
    ifs.read(reinterpret_cast<char*>( &nByte ), sizeof(int) );  

    ifs.read(szOut, nByte);     
    ifs.close();   

    USES_CONVERSION;
    CString strBinaryMacAddress = A2T(szOut);

    ar_LicenseInfo.m_nLicVersion = nVersion;
    ar_LicenseInfo.m_oLicStart   = QBicTime(nStartYear, nStartMonth, nStartDay);
    ar_LicenseInfo.m_oLicEnd     = QBicTime(nEndYear, nEndMonth, nEndDay);
    ar_LicenseInfo.m_oLastRunDay = QBicTime(nRunYear, nRunMonth, nRunDay, nRunHour, nRunMinute, nRunSec);

    ar_strMacAddress = strBinaryMacAddress;
    return true;

#pragma  pack(pop)  
}

CString ImTasLicenseChecker::GetAppPath()
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

bool ImTasLicenseChecker::WriteRunningDay()
{
    CString strMacAddress(_T(""));        
    CString strAppPath = GetAppPath();
    if (strAppPath.GetLength() < 1)
    {
        TxLogDebugException();
        return false;
    }

    CString strLicenseFileName(_T(""));
    strLicenseFileName.Format(_T("%s\\TOVA.lic"), strAppPath);

#pragma pack(push, 1)
    std::ofstream ofs(strLicenseFileName.GetBuffer(), std::ios::binary | std::ios::out | std::ios::in);
    if (!ofs.is_open())
    {
        AfxMessageBox(_T("라이센스 파일을 찾을 수 없습니다"));
        return false;
    }
    
    // 혼돈을 만든다. (1200~5005 Byte)
    ofs.seekp(1200); 
    std::srand((unsigned)time(0)); 
    for (int i=0; i<509; i++)
    {        
        int nRandomVariable = std::rand();
        ofs.write(reinterpret_cast<char*>( &nRandomVariable ), sizeof(int) );         
    }

    // 1200 + 2036 = 3236 위치
    std::ios::pos_type nPos = ofs.tellp();

    int nCount = std::rand() % 10 + 25;
    ofs.seekp(3240); 
    for (int i=0; i<nCount; i++)
    {        
        int nRandomVariable = std::rand();
        ofs.write(reinterpret_cast<char*>( &nRandomVariable ), sizeof(int) );         
    }
    
    nPos = ofs.tellp();

    // write 프로그램 실행일자
    QBicTime oDay;
    int nRunYear   = oDay.GetYear();
    int nRunMonth  = oDay.GetMonth();
    int nRunDay    = oDay.GetDay();
    int nRunHour   = oDay.GetHour();
    int nRunMinute = oDay.GetMinute();
    int nRunSec    = oDay.GetSecond();

    ofs.seekp(1300);      
    ofs.write(reinterpret_cast<char*>( &nRunYear  ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nRunMonth ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nRunDay   ), sizeof(int) );  

    ofs.seekp(2300);    
    ofs.write(reinterpret_cast<char*>( &nRunHour  ), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nRunMinute), sizeof(int) );  
    ofs.write(reinterpret_cast<char*>( &nRunSec   ), sizeof(int) );  
        
    ofs.close();  
#pragma  pack(pop)  
    return true;   
}
