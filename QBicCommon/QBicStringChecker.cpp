#include "stdafx.h"
#include "QBicStringChecker.h"
#include "CString2MBString.h"

QBicStringChecker::QBicStringChecker(void)
{    
}


QBicStringChecker::~QBicStringChecker(void)
{
}

bool QBicStringChecker::IsNatural(const CString strValue)
{// 정수인가 ?
    if (strValue.GetLength() < 1)
        return false;

    LPTSTR lpszInvalid = 0;
    long   nValue = _tcstol(strValue, &lpszInvalid, 10);

    if (*lpszInvalid) {
        return false;
    }
    else {
        return true;
    }
}

bool QBicStringChecker::IsNumeric(CString a_strCheckString)
{// 숫자인가?
    int nLength = a_strCheckString.GetLength();
    
	if (nLength < 1)
        return false;

	LPTSTR lpszInvalid;
	double dValue = _tcstod(a_strCheckString, &lpszInvalid);

	if (*lpszInvalid) {
		return false;
	}
	else {
		return true;
	}
}

bool QBicStringChecker::IsSignedInteger(CString strValue)
{// 정수인가 ?
    return IsNatural(strValue);
}

bool QBicStringChecker::IsUnSignedInteger(CString strValue)
{// 0 또는 양수인가 ?
    if (strValue.GetLength() < 1)
        return false;

    LPTSTR lpszInvalid = 0;
    long   nValue = _tcstol(strValue, &lpszInvalid, 10);

    if (*lpszInvalid) {
        return false;
    }

    if (nValue >= 0)
        return true;
    else 
        return false;
}

bool QBicStringChecker::IsValidFileName(const CString a_strCheckString, OUT CString& ar_strErrorMsg)
{
    CString strCheckString = a_strCheckString;
    strCheckString.Trim();

    int nLength = strCheckString.GetLength();
    if (nLength == 0) {
        ar_strErrorMsg = _T("파일명:공백");
        return false;
    }

	for(int i = 0; i < nLength; i++)
	{
		TCHAR szChar = strCheckString[i];

		if( strCheckString[i] == '\\' || 
			strCheckString[i] == '/'  ||
			strCheckString[i] == ':'  ||
			strCheckString[i] == '*'  ||
			strCheckString[i] == '?'  ||
			strCheckString[i] == '"'  ||
			strCheckString[i] == '<'  ||
			strCheckString[i] == '>'  ||
			strCheckString[i] == '|'   )
		{
			ar_strErrorMsg.Format(_T("특수문자 포함 오류:%c"), szChar);
			return false;
		} else if (__isascii(szChar) != 0) {
            int nAsciiCode = __toascii(szChar); 
            if (65 <= nAsciiCode && nAsciiCode <= 90) {// 대문자
                continue;
            }
            if (97 <= nAsciiCode && nAsciiCode <= 122) { // 소문자
                continue;
            }

            if (48 <= nAsciiCode && nAsciiCode <= 57) { // 숫자
                continue;
            }

            if (95 == nAsciiCode) // _
                continue;

            if (true) {
                ar_strErrorMsg.Format(_T("특수문자 포함 오류: %c"), szChar);
                return false;
            }
        }
	}

	return true;
}

bool QBicStringChecker::IsValidColumnName( CString a_strCheckString, CString& ar_strErrorMsg )
{
	for(int i = 0; i < a_strCheckString.GetLength(); i++)
	{
		TCHAR szChar = a_strCheckString[i];

		if( a_strCheckString[i] == '\\' || 
			a_strCheckString[i] == '/'  ||
			a_strCheckString[i] == ':'  ||
			a_strCheckString[i] == '*'  ||
			a_strCheckString[i] == '?'  ||
			a_strCheckString[i] == '"'  ||
			a_strCheckString[i] == '<'  ||
			a_strCheckString[i] == '>'  ||
			a_strCheckString[i] == '|'  ||
			a_strCheckString[i] == '['  ||
			a_strCheckString[i] == ']'  ||
			a_strCheckString[i] == '\'')
		{
			ar_strErrorMsg.Format(_T("%c"), szChar);
			return false;
		}
	}

	return true;
}

bool QBicStringChecker::IsValidTableName( CString a_strCheckString, CString& ar_strErrorMsg )
{
	for(int i = 0; i < a_strCheckString.GetLength(); i++)
	{
		TCHAR szChar = a_strCheckString[i];
		int nAsciiCode = __toascii(szChar); // 테이블명 가능 문자 :  65~90(대문자), 97~122(소문자), 48~57(숫자), 95('_'문자), 35('#'문자) , 36('$'문자);
		
		if (65 <= nAsciiCode && nAsciiCode <= 90) // 대문자
			continue;

		if (97 <= nAsciiCode && nAsciiCode <= 122) // 소문자
			continue;

		if (48 <= nAsciiCode && nAsciiCode <= 57)  // 숫자
		{
			if (0 == i) // 첫글자는 알파벳만
			{
				return false;
			}

			continue;
		}

		if (95 == nAsciiCode || 35 == nAsciiCode || 36 == nAsciiCode) //  95('_'문자), 35('#'문자) , 36('$'문자);
		{
			if (0 == i)
			{
				return false;
			}

			continue;
		}

		ar_strErrorMsg.Format(_T("%c"), szChar);
			return false;
	}

	return true;
}

bool QBicStringChecker::IncludeHangul( CString strCheck )
{
    CString2MBString oStr(strCheck);
    char* lpString = oStr;

    BYTE			ch1,ch2;
    while(*lpString){
        ch1 = (BYTE)*lpString;
        if ( ch1 & 0x80 ){
            ch2 = (BYTE)*(lpString+1);
            if ( (ch1>0x80 && ch1<=0xA0) || (ch1>=0xB0 && ch1<=0xC5) ){
                if ( (ch2>0x40 && ch2<0x5B) || (ch2>0x60 && ch2<0x7B) || (ch2>0x80 && ch2<0xFF) )
                    return true;
            }
            else if ( ch1>0xA0 && ch1<0xB0 ){
                if ( (ch2>0x40 && ch2<0x5B) || (ch2>0x60 && ch2<0x7B) || (ch2>0x80 && ch2<0xA1) )
                    return true;
                if ( ch1 == 0xA4 ){
                    if ( ch2>0xA0 && ch2<0xFF )
                        return true;
                }
            }
            else if ( ch1 == 0xC6 ){
                if ( (ch2>0x40 && ch2<0x53 ) || (ch2>0xA0 && ch2<0xFF) )
                    return true;
            }
            else if ( ch1 > 0xC6 && ch1 < 0xC9 ){
                if ( ch2>0xA0 && ch2<0xFF )
                    return true;
            }
            lpString++;
        }
        lpString++;
    }

    return false;
}

bool QBicStringChecker::IsValidName(CString strCheck, CString& strOutMsg)
{
    for(int i=0; i<strCheck.GetLength(); i++) {
        TCHAR szChar = strCheck[i];
        int   nCode = __toascii(szChar); 
        // 65~90(대문자), 97~122(소문자)
        // 48~57(숫자), 95('_'문자) 
        
        // 첫글자는 무조건 영문으로 시작, '_'는 중간이후 사용가능
        if(i==0) {
            if (! ( (65 <= nCode && nCode <= 90) || (97 <= nCode && nCode <= 122) ) ) {
                strOutMsg = _T("첫글자는 영문으로 시작해 주세요");
                return false;
            }
            continue; 
        }

        // 영문통과
        if ((65 <= nCode && nCode <= 90) || (97 <= nCode && nCode <= 122)) 
            continue;
        // 숫자통과
        if (48 <= nCode && nCode <= 57)
            continue;
        //'_' 통과
        if (nCode == 95)
            continue;
        
        strOutMsg.Format(_T("허용되지 않는 문자가 포함되었습니다-%c"), szChar);
        return false;
    }

    return true;
}
