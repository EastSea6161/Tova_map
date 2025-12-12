#pragma once
/**
*@brief 키 생성기
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxKeyGenerator
{
public:
    TxKeyGenerator(void) : m_key(0) { }
    virtual ~TxKeyGenerator(void) {};

public :
	/**
	*@brief 유일키 생성기
	*@param void
	*@return 키 값
	*/
    __int64 GenerateUniqueKey( void ) {
        return m_key++;
    }

private :
    __int64 m_key;
};