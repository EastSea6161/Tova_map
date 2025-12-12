#pragma once
/**
*@brief 윈도우 버전 확인
*@details 윈도우 버전 확인 클래스
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicWinOsVersion
{
public:
    QBicWinOsVersion() {}
    ~QBicWinOsVersion(){}

public:
	/**
	*@brief 윈도우 중요도
	*@param DWORD& dwMajorVersion	전문가용 버전
	*@param DWORD& dwMinorVersion	일반용 버전
	*/
    static void WindowsOsVersion(DWORD& dwMajorVersion, DWORD& dwMinorVersion);
	/**
	*@brief 윈도우 버전 확인
	*@details 윈도우 xp ~ 8.1까지 / 10 업데이트 요청
	*@return 윈도우 버전명 반환
	*/
    static CString WindowsOsVersion(); 
};