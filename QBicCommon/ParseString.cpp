#include "stdafx.h"
#include "ParseString.h"

std::vector<CString> KParseString::ParseString( CString a_strInput, TCHAR a_chSep, bool a_bIncludeSpace )
{
    std::vector<CString> vecParseString;

    int     nTokenPos = 0; 
    CString strTok;

    while (AfxExtractSubString(strTok, a_strInput, nTokenPos++, a_chSep))
    {
        strTok.Trim();

        if (!a_bIncludeSpace)
        {
            if (strTok.IsEmpty())
            {
                continue;
            }
        }
        // else 스페이스도 데이터 
        vecParseString.push_back(strTok);
    }

    return vecParseString;
}

std::vector<CString> KParseString::ParseString( CString strLine, TCHAR chSep )
{
	return QBicSplit::Split(strLine, chSep);
}


// ############################################################################################

std::vector<CString> QBicSplit::Split(CString strLine, TCHAR chSep)
{
    std::vector<CString> vecStr;
    LPCTSTR lpszFullString = (LPCTSTR)strLine; {
        if (lpszFullString == nullptr)
            return vecStr;
    }

    LPCTSTR lpszPrePos = lpszFullString;

    int nLength = lstrlen(lpszFullString);
    int nPos(0);

    while(nPos <= nLength) {
        int nLen(0);
        lpszFullString = _tcschr(lpszFullString, chSep);
        if (lpszFullString == nullptr) {
            nLen = lstrlen(lpszPrePos);
            CString rString(_T("")); {
                Checked::memcpy_s(rString.GetBufferSetLength(nLen), nLen*sizeof(TCHAR), lpszPrePos, nLen*sizeof(TCHAR));
                rString.ReleaseBuffer();
            }
            
            vecStr.push_back(rString);
        } else {
            nLen = (int)(lpszFullString - lpszPrePos);
            CString rString(_T("")); {
                Checked::memcpy_s(rString.GetBufferSetLength(nLen), nLen*sizeof(TCHAR), lpszPrePos, nLen*sizeof(TCHAR));
                rString.ReleaseBuffer();                
            }            
            
            vecStr.push_back(rString);
            lpszPrePos = ++lpszFullString;            
        }   

        nPos += (nLen+1);
    }

    return vecStr;
}
