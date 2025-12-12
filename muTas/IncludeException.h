#pragma once

#include "../QBicCommon/IncludeQBicCommon.h"
#include "../QBicCommon/ExceptionDefine.h"

// inline void ThrowException (long a_nErrorCode)
// {
//     KExceptionPtr spException (new KException(a_nErrorCode));
//     throw spException;
// }
// 
// inline void ThrowException (LPCTSTR a_strError)
// {
//     KExceptionPtr spException (new KException(a_strError));
//     throw spException;
// }
// 
// inline void ThrowException (long a_nErrorCode, LPCTSTR a_strError)
// {
//     KExceptionPtr spException (new KException(a_nErrorCode, a_strError));
//     throw spException;
// }
// 
// #ifndef TxExceptionPrint
// #define TxExceptionPrint(ex) { \
//     CString      strLogMsg; \
//     KEMErrorType kemErrorType = ex->GetErrorType();  \
//     long         nErrorCode   = ex->GetErrorCode();  \
//     switch (kemErrorType)  \
// {  \
//     case KEMErrorTypeSqlite:   \
//     strLogMsg.Format(_T("Sqlite Error : Error Code(%d), Error Message >> %s"), nErrorCode, ex->GetErrorMessage());  \
//     break;  \
//     case KEMErrorTypeMap: \
//     strLogMsg.Format(_T("Map Error : Error Code(%d), Error Message >> %s"), nErrorCode, ex->GetErrorMessage());     \
//     break; \
//     default:  \
//     strLogMsg.Format(_T("Unknown Error : Error Code(%d), Error Message >> %s"), nErrorCode, ex->GetErrorMessage()); \
//     break;  \
// }  \
//     TxLogDebug(strLogMsg.AllocSysString()); \
// }
// #endif
// 
// inline void ThrowExceptionMap (HRESULT hr)
// {
//     CString strErrorMsg;
//     strErrorMsg.Format(_T("Map Exception (%d)"), hr);
//     KExceptionPtr exceptionPtr(new KExceptionMap(hr, strErrorMsg));
//     throw exceptionPtr;
// }
// 
// inline void ThrowExceptionMap (HRESULT hr, LPCTSTR a_szErrorMsg)
// {
//     KExceptionPtr exceptionPtr(new KExceptionMap(hr, a_szErrorMsg));
//     throw exceptionPtr;
// }
// 
// inline void ThrowExceptionSqlite (Sqlite* pSqliteDB)
// {
//     KExceptionPtr exceptionPtr (new KExceptionSqlite(SqliteErrorCode(pSqliteDB), SqliteErrorMsg(pSqliteDB)));
//     throw exceptionPtr;
// }