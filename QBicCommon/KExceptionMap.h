#pragma once

#include "KException.h"

/**
*@brief 예외 처리
*@details 예외 처리 맵방식
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KExceptionMap : public KException
{
public:
	/**
	*@brief long 예외처리
	*@param long a_nErrorCode	관련 코드
	*/
    KExceptionMap(long a_nErrorCode) : KException(a_nErrorCode)
    {        
        m_nErrorType = KEMErrorTypeMap;
    }
	/**
	*@brief LPCTSTR 예외처리
	*@param LPCTSTR a_strErrorMsg	관련 메세지
	*/
    KExceptionMap(LPCTSTR a_strErrorMsg) : KException(a_strErrorMsg) 
    {
        m_nErrorType = KEMErrorTypeMap;
    }
	/**
	*@brief long, LPCTSTR 예외처리
	*@param long a_nErrorCode	관련 코드
	*@param LPCTSTR a_strErrorMsg	관련 메세지
	*/
    KExceptionMap(long a_nErrorCode, LPCTSTR a_strErrorMsg) : KException(a_nErrorCode, a_strErrorMsg)
    {
        m_nErrorType = KEMErrorTypeMap;
    }

    virtual ~KExceptionMap(void)
    {        
    }

};


