#include "stdafx.h"
#include "QBicStringFormat.h"
#include <cmath>

CString QBicStringFormat::FormatString( double dValue, int nDigit )
{// 뒷자리 소수점으로 채우기
    CString strNumber(_T("")); {
        strNumber.Format(_T("%f"), dValue);
    }

    _TCHAR szBuffer[256];        
    NUMBERFMT nf = {0, 1, 3, _T("."), _T(""), 1};
    int dp = nDigit; {
        if (dp == -1) {
            CString strDecSep(_T("."));
            dp = strNumber.Find(strDecSep);
            dp = (dp == -1) ? 0 : strNumber.GetLength() - dp - strDecSep.GetLength();
        }
    }

    nf.NumDigits = dp;
    ::GetNumberFormat(NULL, NULL, strNumber, &nf, szBuffer, sizeof(szBuffer));

    return CString(szBuffer);
}


CString QBicStringFormat::ToCommaStr( CString strNumber, int nDigit, bool bGlobal )
{
#pragma region comment
    // strNumber : 12356.455
    // nDigit(-1) -> 12,356.455
    // nDigit( 0) -> 12,356
    // nDigit( 1) -> 12,356.5
    // nDigit( 2) -> 12,356.46
    // nDigit( 3) -> 12,356.455
    // nDigit( 4) -> 12,356.4550
    // nDigit( 5) -> 12,356.45500  -> 뒤자리 0으로 채워짐.
    // bGlobal - 전세계 지원(어느 나라는 12.356,455123 식으로 표현되기도 함)
#pragma endregion comment
    
    if (strNumber.GetLength() == 0)
        return strNumber;

    if (nDigit < -1)
        nDigit = -1;
        
    if (bGlobal == false/*고정방법*/) 
    {
        _TCHAR szBuffer[256];        
        NUMBERFMT nf = {0, 1, 3, _T("."), _T(","), 1};
        int dp = nDigit; {
            if (dp == -1) {
                CString strDecSep(_T("."));
                dp = strNumber.Find(strDecSep);
                dp = (dp == -1) ? 0 : strNumber.GetLength() - dp - strDecSep.GetLength();
            }
        }
        
        nf.NumDigits = dp;
        ::GetNumberFormat(NULL, NULL, strNumber, &nf, szBuffer, sizeof(szBuffer));
        
        return CString(szBuffer);
    } 
    else 
    {
        _TCHAR szBuffer[10];
        _TCHAR szDecSep[10];
        _TCHAR szThousandsSep[10];

        NUMBERFMT nf;
        memset(&nf, 0, sizeof(nf));
        // set decimal separator string from locale default
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDecSep, sizeof(szDecSep)-1); {
            nf.lpDecimalSep = szDecSep;
        }

        // set thousand separator string from locale default
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, szThousandsSep, sizeof(szThousandsSep)-1); {
            nf.lpThousandSep = szThousandsSep;
        }

        // set leading zeroes from locale default
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILZERO, szBuffer, sizeof(szBuffer)-1); {
            nf.LeadingZero = _ttoi(szBuffer);
        }    

        // set grouping of digits from locale default
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SGROUPING, szBuffer, sizeof(szBuffer)-1); {
            nf.Grouping = _ttoi(szBuffer);
        }

        // set negative number mode from locale default
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, szBuffer, sizeof(szBuffer)-1); {
            nf.NegativeOrder = _ttoi(szBuffer);
        }

        int dp = nDigit; {
            if (dp == -1) {
                // set number of fractional digits from input string
                CString strDecSep = szDecSep;
                dp = strNumber.Find(szDecSep);
                dp = (dp == -1) ? 0 : strNumber.GetLength() - dp - strDecSep.GetLength();
            }
        }
        
        nf.NumDigits = dp;

        int nSize = (int)(_tcslen(strNumber) * 3) + 100;
        _TCHAR* pszFormattedNumber = new _TCHAR [nSize];

        if (GetNumberFormat(LOCALE_USER_DEFAULT, 0, strNumber, &nf, pszFormattedNumber, nSize-1)) {
            strNumber = pszFormattedNumber;
        }

        if (pszFormattedNumber)
            delete [] pszFormattedNumber;
    }

    return strNumber;    
}
