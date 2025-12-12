#pragma once

#include <wincrypt.h>	
/**
*@brief MD5 설정
*@details MD5 복호화 설정
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicMd5  
{
public:
    QBicMd5();
    virtual ~QBicMd5();
public:
	/**
	*@brief MD5 복호화 확인
	*@details 문제 발생시 에러 메세지 발생
	*@param CString ar_strBuffer
	*@param CString& ar_strResult
	*@return 이상없을 시 ture, 이상시 false를 반환
	*/
    bool GetMd5String(CString ar_strBuffer, CString& ar_strResult);
};
