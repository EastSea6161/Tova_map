#pragma once
#include "IncludeSqlite.h"
#include "KException.h"

//#define ThrowSqliteException (pDB) throw KExceptionPtr(new KExceptionSqlite(SqliteErrorMsg(pDB))) 
/**
*@brief 예외 처리
*@details 예외처리 Sqlite
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KExceptionSqlite : public KException
{
public:
	/**
	*@brief long 예외처리
	*@param long a_nErrorCode	관련 코드
	*/
    KExceptionSqlite(long a_nErrorCode) : KException(a_nErrorCode)
    {        
        m_nErrorType = KEMErrorTypeSqlite;
    }
	/**
	*@brief LPCTSTR 예외처리
	*@param LPCTSTR a_strErrorMsg	관련 메세지
	*/
    KExceptionSqlite(LPCTSTR a_strErrorMsg) : KException(a_strErrorMsg) 
    {
        m_nErrorType = KEMErrorTypeSqlite;
    }
	/**
	*@brief long, LPCTSTR 예외처리
	*@param long a_nErrorCode	관련 코드
	*@param LPCTSTR a_strErrorMsg	관련 메세지
	*/
    KExceptionSqlite(long a_nErrorCode, LPCTSTR a_strErrorMsg) : KException(a_nErrorCode, a_strErrorMsg)
    {
        m_nErrorType = KEMErrorTypeSqlite;
    }

    virtual ~KExceptionSqlite(void)
    {        
    }

public:
};

// KExceptionPtr exceptionPtr (new KExceptionSqlite(SqliteErrorMsg(m_pSqliteDB)));
/*throw exceptionPtr;*/

