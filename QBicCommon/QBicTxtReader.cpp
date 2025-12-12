#include "stdafx.h"
#include "QBicTxtReader.h"
#include "QBicTextEncoding.h"

QBicTxtReader::QBicTxtReader()
{
    m_nPos       = 0;
    m_nEncoding  = -1;
    m_nCRLF      = 1;
}

QBicTxtReader::~QBicTxtReader()
{
    Close();
}

void QBicTxtReader::SetEncoding(int a_nEncoding)
{
	m_nEncoding = a_nEncoding;
}

bool QBicTxtReader::Open( CString strFileName)
{
    int nError = _taccess(strFileName, 4); 
    if (nError != 0)
        return false;

	//▶ 시간형식(64Bit), 파일크기(64Bit - 용량이 큰 파일은 32Bit(2,147,483,647) 사용시 음수를 리턴)
    struct _stat64 statFile;
    if (_tstat64(strFileName, &statFile) == 0) {
        if (statFile.st_size < 1)
            return false;
    }

    try {
		if (m_nEncoding < 0) { // 외부에서 입력된 Encoding 정보가 있다면(>=0), Encoding 정보 찾기 미실행
			m_nEncoding = GetTxtFileEncoding(strFileName);
			if (m_nEncoding > 1)
				throw _T("Not Supported Encording");
		}
    } catch (...) {
        return false;
    }    

    try {
        m_nPos = 0;
        QBicCharPtr spPath = QBicTextEncoding::WChar2Char(strFileName);
        m_mmapFile.open(spPath->Char(), BOOST_IOS::in);

        SetLF();

        return m_mmapFile.is_open();
    } catch (int& ex) {
    	CString strError(_T(""));
        strError.Format(_T("Error : %d"), ex);
        TxLogDebug((LPCTSTR)strError);
    } catch (KExceptionPtr ex) {
        TxExceptionPrint(ex);
    } catch (...) {
        TxLogDebugException();
    }
    
    Close();
    return false;
}

void QBicTxtReader::SetLF()
{
    if (m_mmapFile.is_open() == false)
        return;

    size_t nxSize = m_mmapFile.size();
    auto pPos = m_mmapFile.const_data();
    auto pEnd = pPos + nxSize;

    auto pCurPos = pPos;
    auto pPrePos(pCurPos);

    pCurPos = static_cast<const char*>(memchr(pPos, '\n', pEnd - pPos));
    if (pCurPos == nullptr) {
        // 엔터 없이 첫줄에 모든 것이 있다.
        return;
    }

    size_t nxLineByteCount(0);
    if (pCurPos != nullptr) {
        nxLineByteCount = pCurPos - pPos;
    }
    else {
        nxLineByteCount = pEnd - pPos;
    }

    if (nxLineByteCount < 1) {
        m_nCRLF = 0;
        return;
    }

    pPrePos = pCurPos - 1;
    
    if (*pPrePos == '\r') {
        m_nCRLF = 1;
        return;
    }

    m_nCRLF = 0;
}


void QBicTxtReader::Close()
{
    m_nPos = 0;
    if (m_mmapFile.is_open() == true) {
        m_mmapFile.close();
    }
}

bool QBicTxtReader::ReadLine( CString& strLine )
{
    if (m_mmapFile.is_open() == false)
        return false;

    size_t nxSize = m_mmapFile.size();
    if (m_nPos >= nxSize)
        return false;

    //★ 첫번째에 BOM 코드가 있을 수 있다. 2016-09-27
    if (m_nPos == 0) {
        if (m_nEncoding == 1) {
            m_nPos += 3;
        }
        else if (m_nEncoding == 2) {
            m_nPos += 2;
        }
        else if (m_nEncoding == 3) {
            m_nPos += 2;
        }
        else if (m_nEncoding == 4) {
            m_nPos += 4;
        }
        else if (m_nEncoding == 5) {
            m_nPos += 4;
        }
        else if (m_nEncoding == 100) { // pos을 이동하지 않고 QBicTextEncoding::UTF8ToWChar 를 실행해야 함
            m_nPos += 0;
        }
        else {
        }
    }

    if (m_nPos >= nxSize)
        return false;

    auto pPos = m_mmapFile.const_data(); 
    auto pEnd = pPos + nxSize;  

    auto pCurPos = pPos + m_nPos;
    auto pPrePos(pCurPos);

    pCurPos = static_cast<const char*>(memchr(pCurPos, '\n', pEnd - pCurPos));

    size_t nxLineByteCount(0); 
    if (pCurPos != nullptr) {
        nxLineByteCount = pCurPos - pPrePos;
    }
    else { 
        nxLineByteCount = pEnd - pPrePos;
    }
        
    char* pBuffer = new char[nxLineByteCount + 1]; {
        memset(pBuffer, 0, nxLineByteCount + 1);
    }
    
    //★ 2017-02-07 :
    //★ 완벽한 개행문자 처리는 하지 않음.. 누군가 잘 처리 부탁
    //★ Windows Text / Unix Text의 개행 문자가 차이가 있다.
    //★ Windows -> CRLF
    //★ Unix    ->   LF
    /*
      char cR = '\r'; //★ CR : 13
      char cN = '\n'; //★ LF : 10
    */
    
    if (pCurPos != nullptr) {
        if (m_nCRLF) {
            memcpy(pBuffer, pPrePos, nxLineByteCount - 1); // '\r' 제외
        }
        else {
            memcpy(pBuffer, pPrePos, nxLineByteCount);
        }        
    }        
    else {
        memcpy(pBuffer, pPrePos, nxLineByteCount);
    }        

    m_nPos = m_nPos + nxLineByteCount + 1;

    if (m_nEncoding == 0) {
        strLine = CString(pBuffer);        
    } else {
        strLine = QBicTextEncoding::UTF8ToWChar(pBuffer);
    }

    delete[] pBuffer;
    return true;
}

bool QBicTxtReader::ReadLine( TCHAR chSep, std::vector<CString>& vecStr )
{
    vecStr.clear();

    CString strLine(_T(""));
    bool bR = ReadLine(strLine);
    if (bR == false)
        return false;

    LPCTSTR lpszFullString = (LPCTSTR)strLine; {
        if (lpszFullString == nullptr)
            return true;
    }

    LPCTSTR lpszPrePos = lpszFullString;

    int nLength = lstrlen(lpszFullString);
    int nPos(0);

    while(nPos <= nLength) {
        int nLen(0);
        lpszFullString = _tcschr(lpszFullString, chSep);
        if (lpszFullString == nullptr) {
            nLen = lstrlen(lpszPrePos);
            CString rString(_T(""));
            Checked::memcpy_s(rString.GetBufferSetLength(nLen), nLen*sizeof(TCHAR), lpszPrePos, nLen*sizeof(TCHAR));
            rString.ReleaseBuffer();
            vecStr.push_back(rString);
        } else {
            nLen = (int)(lpszFullString - lpszPrePos);
            CString rString(_T(""));
            Checked::memcpy_s(rString.GetBufferSetLength(nLen), nLen*sizeof(TCHAR), lpszPrePos, nLen*sizeof(TCHAR));
            rString.ReleaseBuffer();
            vecStr.push_back(rString);

            lpszPrePos = ++lpszFullString;            
        }   

        nPos += (nLen+1);
    }
    
    return true;
}

bool QBicTxtReader::ReadLine(TCHAR chSep, std::vector<CString>& vecStr, CString& a_strLine)
{
    vecStr.clear();
    a_strLine = _T("");

    bool bR = ReadLine(a_strLine);
    if (bR == false)
        return false;

    LPCTSTR lpszFullString = (LPCTSTR)a_strLine; {
        if (lpszFullString == nullptr)
            return true;
    }

    LPCTSTR lpszPrePos = lpszFullString;

    int nLength = lstrlen(lpszFullString);
    int nPos(0);

    while (nPos <= nLength) {
        int nLen(0);
        lpszFullString = _tcschr(lpszFullString, chSep);
        if (lpszFullString == nullptr) {
            nLen = lstrlen(lpszPrePos);
            CString rString(_T(""));
            Checked::memcpy_s(rString.GetBufferSetLength(nLen), nLen * sizeof(TCHAR), lpszPrePos, nLen * sizeof(TCHAR));
            rString.ReleaseBuffer();
            vecStr.push_back(rString);
        }
        else {
            nLen = (int)(lpszFullString - lpszPrePos);
            CString rString(_T(""));
            Checked::memcpy_s(rString.GetBufferSetLength(nLen), nLen * sizeof(TCHAR), lpszPrePos, nLen * sizeof(TCHAR));
            rString.ReleaseBuffer();
            vecStr.push_back(rString);

            lpszPrePos = ++lpszFullString;
        }

        nPos += (nLen + 1);
    }

    return true;
}

std::tuple<size_t, size_t> QBicTxtReader::ReadStatus()
{
	size_t nxSize = m_mmapFile.size();
	return std::make_tuple(m_nPos, nxSize);
}
