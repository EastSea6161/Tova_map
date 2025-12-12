#pragma once
/**
*@brief  2016-01-28 이후 사용 금지 -> QBicTextEncoding 사용
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS CString2MBString 
{
private:
    char* m_pszValue;
public:
    ~CString2MBString();
    CString2MBString(const CString2MBString& right);
    CString2MBString (CString strValue);
public:
    CString2MBString& operator=(const CString2MBString& right);

public:
    operator char* () {return m_pszValue;}
};

typedef CString2MBString QBicMByteStr;
typedef CString2MBString QBicStr2MByte;


