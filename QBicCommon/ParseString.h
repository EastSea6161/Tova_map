#pragma once
#include <bitset>
/**
*@brief 2016-01-28 이후 사용 금지 -> QBicSplit
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KParseString
{
public:
	KParseString() {};
	~KParseString(){};
public:
    static std::vector<CString> ParseString(CString a_strInput, TCHAR a_chSep, bool a_bIncludeSpace);
public:
	static std::vector<CString> ParseString(CString strLine, TCHAR chSep);
};

/**
*@brief Split 
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicSplit
{
public:
    QBicSplit() {}
    ~QBicSplit(){}

public:
	/**
	*@brief 문자열 배열 반환
	*@param CString strLine	문장
	*@param TCHAR chSep		시퀀스
	*@return 입력된 값 전달
	*/
    static std::vector<CString> Split(CString strLine, TCHAR chSep);
};