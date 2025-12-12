#include "StdAfx.h"
#include "SQLiteConnection.h"
#include "SQLiteResultSet.h"
#include "SQLitePreparedStatement.h"
#include "ExceptionDefine.h"
#include "QBicTextEncoding.h"

KSQLiteConnection::KSQLiteConnection() 
{
    m_pSqliteDB    = NULL;
    //m_pUpdateStmt  = NULL;
    m_pSqliteTail  = NULL;
    
    m_bReference   = false;
}

KSQLiteConnection::KSQLiteConnection(sqlite3* a_pDB) 
{
    m_pSqliteDB    = a_pDB;
    //m_pUpdateStmt  = NULL;
    m_pSqliteTail  = NULL;

    m_bReference   = true;
    SetPragma();
}

KSQLiteConnection::KSQLiteConnection( LPCTSTR a_strFileName )
{
    m_pSqliteDB    = NULL;
    //m_pUpdateStmt  = NULL;
    m_pSqliteTail  = NULL;

    m_bReference   = false;

	Connection(a_strFileName);
}

void KSQLiteConnection::SetPragma()
{
    if (m_pSqliteDB == NULL) {
        return;
    }
        
    sqlite3_exec(m_pSqliteDB, "PRAGMA encoding     = UTF-8",  NULL, NULL, NULL); //★ 2016-01-27
    sqlite3_exec(m_pSqliteDB, "PRAGMA CACHE_SIZE   = 5000",   NULL, NULL, NULL); 
    sqlite3_exec(m_pSqliteDB, "PRAGMA synchronous  = OFF",    NULL, NULL, NULL); 
    sqlite3_exec(m_pSqliteDB, "PRAGMA journal_mode = MEMORY", NULL, NULL, NULL); 
    //sqlite3_exec(m_pSqliteDB, "PRAGMA shrink_memory", NULL, NULL, NULL); 
}

KSQLiteConnection::~KSQLiteConnection(void)
{
    //TxLogDebug((LPCTSTR)m_oConnectionString.m_strUrl);
    CloseConnection();
}

void KSQLiteConnection::CloseConnection()
{
    try
    {
        //CloseStatement();

        if ( ! m_bReference && m_pSqliteDB )
        {
            SqliteClose(m_pSqliteDB);
        } 
    }
    catch (...)
    {
    }

    m_pSqliteDB    = NULL;
    //m_pUpdateStmt  = NULL;
    m_pSqliteTail  = NULL;
    m_bReference   = false;        
}

void KSQLiteConnection::CloseStatement()
{
//     if ( m_pUpdateStmt == NULL )
//     {
//         return;
//     }
// 
//     try
//     {
//         SqliteFinalize(m_pUpdateStmt);
//     }
//     catch (...)
//     {
//         TxLogDebugWarning();
//     }
//     
//     m_pUpdateStmt = NULL;
}

bool KSQLiteConnection::Connection(LPCTSTR a_strFileName)
{
    KConnectionString oConnectionString;
    oConnectionString.m_strUrl = a_strFileName;

    return Connection(oConnectionString);
}

bool KSQLiteConnection::Connection( KConnectionString& ar_ConnectionString )
{
    m_oConnectionString.m_strUrl      = ar_ConnectionString.m_strUrl;
    m_oConnectionString.m_strUserID   = ar_ConnectionString.m_strUserID;
    m_oConnectionString.m_strPassword = ar_ConnectionString.m_strPassword;

    try
    {
        CloseConnection();

        QBicCharPtr spPath = QBicTextEncoding::WChar2UTF8(ar_ConnectionString.m_strUrl);
        int nResult  = SqliteOpen( spPath->Char(), &m_pSqliteDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr );
        if( nResult != SQLITE_OK )
        {
            throw SqliteErrorMsg(m_pSqliteDB);
        }

        m_bReference = false;  
        SetPragma();
    }
    catch (...)
    {
        return false;
    }

    return true;
}

void KSQLiteConnection::BeginTransaction()
{
    if ( m_pSqliteDB == NULL)
    {
        ThrowException(_T("Sqlite is Null -Begin Transaction"));
    }

    if ( sqlite3_exec(m_pSqliteDB, "BEGIN TRANSACTION", NULL, NULL, NULL) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    }
}

void KSQLiteConnection::Commit()
{
    if ( m_pSqliteDB == NULL)
    {
        ThrowException(_T("Sqlite is Null - Commit"));
    }

    if ( sqlite3_exec(m_pSqliteDB, "COMMIT TRANSACTION", NULL, NULL, NULL) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    }
}

void KSQLiteConnection::RollBack()
{
    if ( m_pSqliteDB == NULL)
    {
        ThrowException(_T("Sqlite is Null"));
    }

    if ( sqlite3_exec(m_pSqliteDB, "ROLLBACK TRANSACTION", NULL, NULL, NULL) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    }
}

KResultSetPtr KSQLiteConnection::ExecuteQuery(LPCTSTR a_strSql)
{
    // TODO : Insert / Update 문 체크 
    //        Insert / Update, DML은 여기서 하지 마세요.
    
    if ( m_pSqliteDB == NULL)
    {
        ThrowException(_T("Sqlite is Null - ExecuteQuery"));
    }

    KResultSetPtr spResultSet;
    SqliteStatement* pSqliteStatement = nullptr;

    int nResult = SqlitePrepareV2(m_pSqliteDB, a_strSql, -1, &pSqliteStatement, &m_pSqliteTail);
    if( nResult == SQLITE_OK )
    {
        // pSqliteStatement -> KSQLiteResultSet 소멸시 Finalize
        spResultSet = KResultSetPtr(new KSQLiteResultSet(pSqliteStatement));
    }
    else
    {        
    //    ThrowExceptionSqlite(m_pSqliteDB);
    }
        
    return spResultSet;
}

int KSQLiteConnection::ExecuteUpdate(LPCTSTR a_strSql)
{
    if ( m_pSqliteDB == NULL)
    {
        ThrowException(_T("Sqlite is Null - ExecuteUpdate"));
    }

    SqliteStatement*  pSqliteStatement = nullptr;
    if ( SqlitePrepareV2(m_pSqliteDB, a_strSql, -1, &pSqliteStatement, &m_pSqliteTail) != SQLITE_OK )
    {
        ThrowExceptionSqlite(m_pSqliteDB);
    }

    int nResult = SqliteStep (pSqliteStatement); 
    SqliteFinalize(pSqliteStatement);
    return nResult;
}

KPreparedStatementPtr KSQLiteConnection::PrepareStatement(LPCTSTR a_strSql)
{
    if ( m_pSqliteDB == NULL)
    {
        ThrowException(_T("Sqlite is Null - PrepareStatement"));
    }

    KPreparedStatementPtr spPreparedStatement;
    SqliteStatement*      pSqliteStatement = nullptr;

    if( SqlitePrepareV2(m_pSqliteDB, a_strSql, -1, &pSqliteStatement, &m_pSqliteTail) == SQLITE_OK )
    {
        // pSqliteStatement -> KSQLitePreparedStatement 소멸시 Finalize
        spPreparedStatement = KPreparedStatementPtr(new KSQLitePreparedStatement(m_pSqliteDB, pSqliteStatement));
    }
    else
    {        
        ThrowExceptionSqlite(m_pSqliteDB);
    }

    return spPreparedStatement;
}

int KSQLiteConnection::Vaccum()
{
    int nResult(SQLITE_ERROR);

    try
    {
        nResult = sqlite3_exec(m_pSqliteDB, "VACUUM;", NULL, NULL, NULL);
    	if ( nResult != SQLITE_OK)
        {
        }
    }
    catch (...)
    {
    }

    return nResult;
}

void KSQLiteConnection::ReleaseMemory()
{
    sqlite3_db_release_memory(m_pSqliteDB);
}

