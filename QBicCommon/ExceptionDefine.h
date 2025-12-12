#pragma once
#include "KException.h"
#include "KExceptionMap.h"
#include "KExceptionSqlite.h"
#include "TxLogger.h"

/**
*@brief 예외처리 Throw
*@param long a_nErrorCode 에러코드 Long형식
*/
inline void ThrowException (long a_nErrorCode)
{
    KExceptionPtr spException (new KException(a_nErrorCode));
    throw spException;
}

/**
*@brief 예외처리 Throw
*@param LPCTSTR a_strError 에러메세지 LPCTSTR형식
*/
inline void ThrowException (LPCTSTR a_strError)
{
    KExceptionPtr spException (new KException(a_strError));
    throw spException;
}

/**
*@brief 예외처리 Throw
*@param long a_nErrorCode 에러코드 Long형식
*@param LPCTSTR a_strError 에러메세지 LPCTSTR형식
*/
inline void ThrowException (long a_nErrorCode, LPCTSTR a_strError)
{
    KExceptionPtr spException (new KException(a_nErrorCode, a_strError));
    throw spException;
}

#ifndef TxExceptionPrint
#define TxExceptionPrint(ex) { \
    CString      strLogMsg; \
    KEMErrorType kemErrorType = ex->GetErrorType();  \
    long         nErrorCode   = ex->GetErrorCode();  \
    switch (kemErrorType)  \
{  \
    case KEMErrorTypeSqlite:   \
    strLogMsg.Format(_T("Sqlite Error : Error Code(%d), Error Message >> %s"), nErrorCode, ex->GetErrorMessage());  \
    break;  \
    case KEMErrorTypeMap: \
    strLogMsg.Format(_T("Map Error : Error Code(%d), Error Message >> %s"), nErrorCode, ex->GetErrorMessage());     \
    break; \
    default:  \
    strLogMsg.Format(_T("Unknown Error : Error Code(%d), Error Message >> %s"), nErrorCode, ex->GetErrorMessage()); \
    break;  \
}  \
    TxLogDebug(strLogMsg); }
#endif

/**
*@brief 예외처리 Throw
*@param HRESULT hr	HRESULT 형식 메세지
*/
inline void ThrowExceptionMap (HRESULT hr)
{
    CString strErrorMsg;
    strErrorMsg.Format(_T("Map Exception (%d)"), hr);
    KExceptionPtr spException(new KExceptionMap(hr, strErrorMsg));
    throw spException;
}

/**
*@brief 예외처리 Throw
*@param HRESULT hr	HRESULT 형식 메세지
*@param LPCTSTR a_strError 에러메세지 LPCTSTR형식
*/
inline void ThrowExceptionMap (HRESULT hr, LPCTSTR a_szErrorMsg)
{
    KExceptionPtr spException(new KExceptionMap(hr, a_szErrorMsg));
    throw spException;
}

/**
*@brief 예외처리 Throw
*@param Sqlite* pSqliteDB Sqlite형식
*/
inline void ThrowExceptionSqlite (Sqlite* pSqliteDB)
{
    KExceptionPtr spException (new KExceptionSqlite(SqliteErrorCode(pSqliteDB), SqliteErrorMsg(pSqliteDB)));
    throw spException;
}