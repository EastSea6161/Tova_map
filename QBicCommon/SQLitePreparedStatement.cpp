#include "StdAfx.h"
#include "SQLitePreparedStatement.h"
#include "SQLiteResultSet.h"
#include "ExceptionDefine.h"

KSQLitePreparedStatement::KSQLitePreparedStatement(Sqlite* a_pSqliteDB, SqliteStatement* a_pStatement)
{
    m_pSqliteDB        = a_pSqliteDB;
    m_pSqliteStatement = a_pStatement;

    m_nParameterCount  = SqliteBindParameterCount(m_pSqliteStatement);
}

KSQLitePreparedStatement::~KSQLitePreparedStatement()
{
    try
    {
        SqliteFinalize(m_pSqliteStatement);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}

void KSQLitePreparedStatement::BindNull( int a_nIndex )
{
    if (a_nIndex < 1 || a_nIndex > m_nParameterCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    if ( SqliteBindNull(m_pSqliteStatement, a_nIndex) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    }
}

void KSQLitePreparedStatement::BindText(int a_nIndex, LPCTSTR a_strValue)
{
    if (a_nIndex < 1 || a_nIndex > m_nParameterCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    if ( SqliteBindText(m_pSqliteStatement, a_nIndex, a_strValue, -1, SQLITE_STATIC) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    }
}

void KSQLitePreparedStatement::BindInt(int a_nIndex, int a_nValue )
{ 
    if (a_nIndex < 1 || a_nIndex > m_nParameterCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    if ( SqliteBindInt(m_pSqliteStatement, a_nIndex, a_nValue) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    }
}

void KSQLitePreparedStatement::BindInt64( int a_nIndex, __int64 a_nxValue )
{
    if (a_nIndex < 1 || a_nIndex > m_nParameterCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    if ( SqliteBindInt64(m_pSqliteStatement, a_nIndex, a_nxValue) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    } 
}

void KSQLitePreparedStatement::BindDouble( int a_nIndex, double a_dValue )
{  
    if (a_nIndex < 1 || a_nIndex > m_nParameterCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    if ( SqliteBindDouble(m_pSqliteStatement, a_nIndex, a_dValue) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    } 
}

void KSQLitePreparedStatement::BindBlob( int a_nIndex, char* a_buffer, int a_nSize )
{
    if (a_nIndex < 1 || a_nIndex > m_nParameterCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    if ( SqliteBindBlob(m_pSqliteStatement, a_nIndex, a_buffer, a_nSize, SQLITE_STATIC) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    } 
}


void KSQLitePreparedStatement::Reset()
{
    //SqliteClearBindings(m_pSqliteStatement);    
    int nR = SqliteReset(m_pSqliteStatement);
    if ( nR != SQLITE_OK ) {
        SqliteClearBindings(m_pSqliteStatement);  
        SqliteReset(m_pSqliteStatement);
    } 
}

KResultSetPtr KSQLitePreparedStatement::ExecuteQuery()
{  
    return KResultSetPtr(new KSQLiteResultSet(m_pSqliteStatement, false));
}

int KSQLitePreparedStatement::ExecuteUpdate()
{
    return SqliteStep(m_pSqliteStatement);
}



