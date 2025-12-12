#include "stdafx.h"
#include "CString2MBString.h"

CString2MBString::~CString2MBString()
{
    if (m_pszValue)
        delete[] m_pszValue;
}

CString2MBString::CString2MBString( CString strValue ) : m_pszValue(nullptr)
{    
#ifndef _UNICODE
    char* szStr = nullptr;
    int nLen = strValue.GetLength();
    szStr = strValue.GetBuffer(nLen);
    strValue.ReleaseBuffer(nLen); 

    m_pszValue = new char[nLen + 1];
    memcpy(m_pszValue, szStr, nLen + 1);
#else /*Unicode Type CString*/
    wchar_t* wc;
    wc = strValue.GetBuffer(strValue.GetLength());
    strValue.ReleaseBuffer(strValue.GetLength());
    int nLen(0); {
        nLen = WideCharToMultiByte(CP_ACP, 0, wc, -1, NULL, 0, NULL, NULL);
    }

    m_pszValue = new char[nLen];
    WideCharToMultiByte(CP_ACP, 0, wc, -1, m_pszValue, nLen, 0, 0);
#endif
}

CString2MBString::CString2MBString( const CString2MBString& right )
{
    m_pszValue = nullptr;

    if (right.m_pszValue)
    {
        int nLen = (int)strlen(right.m_pszValue);
        m_pszValue = new char[nLen + 1];
        memcpy(m_pszValue, right.m_pszValue, nLen + 1);
    }
}

CString2MBString& CString2MBString::operator=( const CString2MBString& right )
{
    if (&right == this)
        return *this;

    if (m_pszValue)
        delete[] m_pszValue;

    m_pszValue = nullptr;

    if (right.m_pszValue)
    {
        int nLen = (int)strlen(right.m_pszValue);
        m_pszValue = new char[nLen + 1];
        memcpy(m_pszValue, right.m_pszValue, nLen + 1);
    }

    return *this;
}
