#pragma once
#pragma warning(disable:4996)

#include <locale>
#include <fstream>
#include <codecvt>
/**
*@brief 텍스트 작성
*@details Write함수를 이용하여 작성
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicTxtWriter 
{
public:
    QBicTxtWriter();
    ~QBicTxtWriter();

private:
    int m_nEncoding;
private:
    std::wofstream m_wofstream;
    std::wofstream m_ofstream;
public:
	/**
	*@brief 파일 열기
	*@details 인코딩에 따라 open함수이용 파일 열기
	*@param CString strFileName	파일 명
	*@param int nEncoding		인코딩
    *@param BOOST_IOS::openmode mode		open 옵션
	*@return 파일 open을 반환한다.
	*/
    bool Open(CString strFileName, int nEncoding=1/*1:UTF8*/, BOOST_IOS::openmode mode = BOOST_IOS::out);
	/**
	*@brief 파일 닫기
	*/
    void Close();
	/**
	*@brief 작성
	*@param CString strLine	줄 문장
	*@details 파일이 열려있지 않으면 작성할 수 없게 되있음
	*/
    void Write(CString strLine);
};