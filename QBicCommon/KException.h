#pragma once
#include <boost/shared_ptr.hpp>

enum KEMErrorType
{
    KEMErrorTypeUnknown  = 0,
    KEMErrorTypeSqlite   = 1,
    KEMErrorTypeMap      = 2
};

/**
*@brief 예외처리
*@details 속성별 예외처리
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KException
{
public:
	/**
	*@brief long 예외처리
	*@param long a_nErrorCode	관련 코드
	*/
    KException(long a_nErrorCode)
    {
        m_nErrorType  = KEMErrorTypeUnknown;
        m_nErrorCode  = a_nErrorCode;
    }
	/**
	*@brief LPCTSTR 예외처리
	*@param LPCTSTR a_strErrorMsg	관련 메세지
	*/
    KException(LPCTSTR a_strErrorMsg) 
    { 
        m_nErrorType  = KEMErrorTypeUnknown;
        m_strErrorMsg = a_strErrorMsg;
        m_nErrorCode  = 0;
    }
	/**
	*@brief long, LPCTSTR 예외처리
	*@param long a_nErrorCode	관련 코드
	*@param LPCTSTR a_strErrorMsg	관련 메세지
	*/
    KException(long a_nErrorCode, LPCTSTR a_strErrorMsg)
    {
        m_nErrorType  = KEMErrorTypeUnknown;
        m_nErrorCode  = a_nErrorCode;
        m_strErrorMsg = a_strErrorMsg;
    }

    virtual ~KException(void)
    {        
    }

public:
	/**
	*@brief 에러의 타입
	*@return 에러 타입 반환
	*/
    virtual KEMErrorType GetErrorType()
    {
        return m_nErrorType;
    }

	/**
	*@brief 에러의 메세지
	*@return 에러 메세지 반환
	*/
    virtual LPCTSTR GetErrorMessage() 
    {
        return m_strErrorMsg;
    }

	/**
	*@brief 에러의 코드
	*@return 에러 코드 반환
	*/
    virtual long    GetErrorCode()
    {
        return m_nErrorCode;
    }
    
protected:
    CString      m_strErrorMsg;
    long         m_nErrorCode;
    KEMErrorType m_nErrorType;
};

typedef std::shared_ptr<KException> KExceptionPtr;


