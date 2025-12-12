#pragma once
#include <tuple>
/**
*@brief 텍스트 읽기
*@details Ansi, UTF-8 지원
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
#include <boost/iostreams/device/mapped_file.hpp> // for mmap

class AFX_EXT_CLASS QBicTxtReader 
{
public:
    QBicTxtReader();
    ~QBicTxtReader();
private:
    int    m_nEncoding;
    size_t m_nPos;
private:
    boost::iostreams::mapped_file m_mmapFile;
public:
	/**
	*@brief 파일 열기
	*@details 파일을 선택하여 열기, 인코딩 여부 확인
	*@param CString strFileName 파일명
	*@return 성공시 반환값이 없고, 실패시 false를 리턴한다. 
	*@throws 디버그, false를 리턴
	*/
    bool Open(CString strFileName);
	/**
	*@brief 파일 닫기
	*@details 파일이 열려있으면 종료
	*/
    void Close();
	/**
	*@brief 인코딩 셋팅
	*@parma int a_nEncoding	인코딩
	*@details 받아온 인코딩을 멤버변수 인코딩에 입력
	*/
	void SetEncoding(int a_nEncoding);
public:
	/**
	*@brief 줄 읽기
	*@details UTF8ToWChar를 사용한다.
	*@param 줄 문장
	*@return 파일이 열려있지 않으면 false를 반환한다.
	*/
    bool ReadLine(CString& strLine);
	/**
	*@brief 줄 읽기
	*@details 벡터를 초기화 한 후, 그 벡터 안에 다시 글을 넣는다.
	*@param TCHAR chSep
	*@param std::vector<CString>& vecStr
	*@return 파일이 열려있지 않다면 false를, 열려있고 모두 읽으면 ture를 반환
	*@throws 
	*/
    bool ReadLine(TCHAR chSep, std::vector<CString>& vecStr);

    bool ReadLine(TCHAR chSep, std::vector<CString>& vecStr, CString& a_strLine);

private:
    int  m_nCRLF;
    void SetLF();
public:
    void SetLFOption() {
        m_nCRLF = 0;
    }
public:
	/**
	*@brief 현재 상태 확인
	*@details 파일의 사이즈를 확인 한다.
	*@return 파일의 상태 반환
	*/
	std::tuple<size_t, size_t> ReadStatus();
public:
	/**
	*@brief 텍스트 파일 인코딩 확인
	*@details 인코딩 별 encoding 값이 다르게 적용됨
	*@param CString filename	파일 명
	*@return 인코딩 리턴
	*/
    static int GetTxtFileEncoding(CString filename)
    {
        int encoding(0);

        unsigned char BomUTF8[]              = {0xEF, 0xBB, 0xBF};
        unsigned char BomUTF16BigEndian[]    = {0xFE, 0xFF};
        unsigned char BomUTF16LittleEndian[] = {0xFF, 0xFE};
        unsigned char BomUTF32BigEndian[]    = {0x00, 0x00, 0xFE, 0xFF};
        unsigned char BomUTF32LittleEndian[] = {0xFF, 0xFE, 0x00, 0x00};
        
        //★ UTF-8 Wihout BOM과 Ansi는 판별할 수 없다. ??
        
        DWORD dwBytesRead = 0;
        HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            hFile = NULL;
            CloseHandle(hFile);
            throw 1;//runtime_error("cannot open file");
        }
        
        BYTE *lpHeader = new BYTE[2];
        ReadFile(hFile, lpHeader, 2, &dwBytesRead, NULL);
        CloseHandle(hFile);

        if (dwBytesRead >= 2) {
            if (     lpHeader[0] == BomUTF8[0]              && lpHeader[1] == BomUTF8[1])
                encoding = 1;
            else if (lpHeader[0] == BomUTF16BigEndian[0]    && lpHeader[1] == BomUTF16BigEndian[1])
                encoding = 2;
            else if (lpHeader[0] == BomUTF16LittleEndian[0] && lpHeader[1] == BomUTF16LittleEndian[1])
                encoding = 3;
            else if (lpHeader[0] == BomUTF32BigEndian[0]    && lpHeader[1] == BomUTF32BigEndian[1])
                encoding = 4;
            else if (lpHeader[0] == BomUTF32LittleEndian[0] && lpHeader[1] == BomUTF32LittleEndian[1])
                encoding = 5;
            else
                encoding = 0; // UTF8(Bom이 없는 경우와 Ansi 구별할 수 없음)
        } else {
            encoding = 0;
        }
        delete []lpHeader;
        return encoding;
    }
};