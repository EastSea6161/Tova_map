#pragma once

class ImTasLicenseInfo
{
public:
    ImTasLicenseInfo() {
        m_nLicVersion = 1;
    }

    ~ImTasLicenseInfo(){}
public:
    int m_nLicVersion;
public:
    QBicTime m_oLicStart;
    QBicTime m_oLicEnd;
    QBicTime m_oLastRunDay;
};

class ImTasLicenseChecker
{
public:
    ImTasLicenseChecker() {}
    ~ImTasLicenseChecker(){}
public:
    bool IsOK();
    CString extractSerialNumber();
private:
    bool ReadLicenseFile(CString& ar_strMacAddress, ImTasLicenseInfo& ar_LicenseInfo);
    CString GetAppPath(); 
    CString GetSystemUUID();
public:
    bool WriteRunningDay();
};