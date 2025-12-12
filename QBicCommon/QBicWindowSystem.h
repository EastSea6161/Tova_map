#pragma once
/**
*@brief 윈도우 시스템
*@details 윈도우 버전 확인
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/

class AFX_EXT_CLASS QBicWindowSystem
{
public:
    QBicWindowSystem() {};
    virtual ~QBicWindowSystem(){};
public:
	/**
	*@brief 윈도우 버전 확인
	*@details 64bit 장비 확인
	*@return 32bit 프로그램이 64bit에서 구동중이면 1, 64bit프로그램이 64bit에서 구동중이면 2, 그외는 0, 64bit가 아닐시 -1 반환 
	*/
    static int ProcessOnWindowsStatus();
};