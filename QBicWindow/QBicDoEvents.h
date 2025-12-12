#pragma once
/**
*@brief 이벤트 발생
*@details 큐빅웨어 이벤트 발생
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicDoEvents
{
private:
    QBicDoEvents(); 
    ~QBicDoEvents();

public:
	/**
	*@brief 이벤트 발생시 행동
	*/
    static void DoEvents();
};