#pragma once
/**
*@brief Char형 정의
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicChar
{
    // 향후 복사 생성자를 만들어야 한다..
    // Deep Copy 구현해야 한다.
public:
    QBicChar() {
        m_pszValue = nullptr;
    }

    QBicChar(int nLen) {
        if (nLen < 1) {
            throw 1;
        }

        m_pszValue = new char[nLen]; {
            memset(m_pszValue, 0, nLen);
        }        
    }

    ~QBicChar(){
        if (m_pszValue)
            delete[] m_pszValue;
    }
public:
    operator char* () {
        return m_pszValue;
    }
    char* Char() { // 스마트 포인터
        return m_pszValue;
    }
public:
	/**
	*@brief 사이즈 확인
	*@param int nLen
	*/
    void SetSize(int nLen) {
        if (nLen < 1) {
            throw 1;
        }

        if (m_pszValue)
            delete[] m_pszValue;

        m_pszValue = new char[nLen]; {
            memset(m_pszValue, 0, nLen);
        }
    }
private:
    char* m_pszValue;
};

typedef std::shared_ptr<QBicChar> QBicCharPtr;
/**
*@brief 글자 인코딩
*@details 큐빅웨어에 맞게 Unicode로 재설정
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/

class AFX_EXT_CLASS QBicTextEncoding
{
    // Visual C++은 내부적으로 문자집합을 Multi Byte Character Set -MBCS, Wide Character -Unicode를 사용
    // Unicode: 모든 글자를 2Byte
    // UTF-8  : 영문/숫자/기호는 1바이트로, 한글과 한자 등은 3바이트로 표현
    // 향후, 큐빅웨어는 Unicode만 지원한다.

public:
    QBicTextEncoding();
    ~QBicTextEncoding();

public:
	/**
	*@brief UTF8 인코딩
	*@details UTF8 을 Unciode스타일로 바꾼다.
	*@param IN const char* szUtf8	UTF8 형식
	*@return 인코딩 된 글자 반환
	*/
    static CString UTF8ToWChar(IN const char* szUtf8)
    {//★ UTF8 (char*) -> Unicode CString
        int nLen = (int)strlen(szUtf8); {
            //정확한 길이 : int length = MultiByteToWideChar(CP_UTF8, 0, utf8, (int)strlen(utf8)+1, NULL, NULL);
        }       

        wchar_t* pBuf = new wchar_t[nLen+1]; {
            nLen = MultiByteToWideChar(CP_UTF8, 0, szUtf8, nLen, pBuf, nLen);
            pBuf[nLen] = 0;
        }

        CString strUnicode(_T("")); {
            strUnicode = CString(pBuf);
        }

        delete [] pBuf;

        return strUnicode;
    }
	/**
	*@brief Char 인코딩
	*@details Char 을 Unciode스타일로 바꾼다.
	*@param IN CString strUnicode	유니코드식
	*@return 인코딩 된 글자 반환
	*/
    static QBicCharPtr WChar2UTF8(IN CString strUnicode) 
    {//★ Unicode CString -> UTF8 (char*)
        const 
        int  Ix = 512; // 일반적으로 문자열 길이가 짧다.
        char pUTF8[Ix] = {0};        
        int  nLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, pUTF8, Ix, NULL, NULL); 

        if (nLen == 0) { //에러(버퍼용량부족, 등)
            nLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
            if (nLen > 0) {
                char*  pUTF8 = new char[nLen];
                nLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, pUTF8, nLen, NULL, NULL);   

                QBicCharPtr spUtf8 = QBicCharPtr(new QBicChar(nLen)); {
                    char* pDest = spUtf8->Char();
                    memcpy(pDest, pUTF8, nLen);
                }

                delete[] pUTF8;
                return spUtf8;
            }
        } 
        else {
            QBicCharPtr spUtf8 = QBicCharPtr(new QBicChar(nLen)); {
                char* pDest = spUtf8->Char();
                memcpy(pDest, pUTF8, nLen);
            }

            return spUtf8;
        }        

        return nullptr;
    }
	/**
	*@brief Unciode를 Char형으로
	*@details Unciode을 Char스타일로 바꾼다.
	*@param IN CString strUnicode 유니코드식
	*@return 인코딩 된 글자 반환
	*/
    static QBicCharPtr WChar2Char(IN CString strUnicode) 
    {//★ Unicode CString -> char*
        const 
        int  Ix = 512;
        char pUTF8[Ix] = {0};        
        int  nLen = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, pUTF8, Ix, NULL, NULL); 

        if (nLen == 0) {
            nLen = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
            if (nLen > 0) {
                char* pUTF8 = new char[nLen];
                nLen = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, pUTF8, nLen, NULL, NULL);   

                QBicCharPtr spUtf8 = QBicCharPtr(new QBicChar(nLen)); {
                    char* pDest = spUtf8->Char();
                    memcpy(pDest, pUTF8, nLen);
                }

                delete[] pUTF8;

                return spUtf8;
            }            
        } 
        else {
            QBicCharPtr spUtf8 = QBicCharPtr(new QBicChar(nLen)); {
                char* pDest = spUtf8->Char();
                memcpy(pDest, pUTF8, nLen);
            }

            return spUtf8;
        }        

        return nullptr;
    } 
};

/*
int MultiByteToWideChar(
__in   UINT CodePage,           // 변환할 코드 페이지( 기본적으로 CP_ACP 사용)
__in   DWORD dwFlags,           // 변환 타입을 나타낸다.
__in   LPCSTR lpMultiByteStr,   // 변환할 문자열
__in   int cbMultiByte,         // 변화할 문자열의 사이즈
__out  LPWSTR lpWideCharStr,    // 변환값 저장 버퍼
__in   int cchWideChar          // 변환될 값의 사이즈
);

int WideCharToMultiByte(
__in UINT     CodePage,     // 변환할 코드 페이지( 기본적으로 CP_ACP 사용)
__in DWORD    dwFlags,      // 변환 타입을 나타낸다.
__in_ecount(cchWideChar) LPCWSTR  lpWideCharStr,    // 변환할 문자열
__in int      cchWideChar,   // 변화할 문자열의 사이즈
__out_bcount_opt(cbMultiByte) __transfer(lpWideCharStr) LPSTR   lpMultiByteStr,  // 변환값 저장 버퍼
__in int      cbMultiByte,      // 변환될 값의 사이즈
__in_opt LPCSTR   lpDefaultChar,
__out_opt LPBOOL  lpUsedDefaultChar
);
*/