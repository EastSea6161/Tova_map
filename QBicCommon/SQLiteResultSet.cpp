#include "StdAfx.h"
#include "SQLiteResultSet.h"
#include "ExceptionDefine.h"

KSQLiteResultSet::KSQLiteResultSet(SqliteStatement* a_pStatement, bool a_bReset)
{
    m_pSqliteStatement = a_pStatement;
    m_nColumnCount     = SqliteColumnCount(m_pSqliteStatement);
    m_bReset           = a_bReset;
}

KSQLiteResultSet::~KSQLiteResultSet()
{
    try
    {
        if (m_bReset)
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

bool KSQLiteResultSet::Next() 
{
    int nResult = SqliteStep(m_pSqliteStatement);
    /* SQLITE_ROW : sqlite3_step() has another row ready */
    if ( nResult == SQLITE_ROW)
    {
        return true;
    }

    return false;
}

CString KSQLiteResultSet::GetValueString(int a_nIndex) const
{
    if (a_nIndex<0 || a_nIndex >= m_nColumnCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    return SqliteColumnText(m_pSqliteStatement, a_nIndex);
}

int KSQLiteResultSet::GetValueInt(int a_nIndex) const
{
    if (a_nIndex<0 || a_nIndex >= m_nColumnCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    return SqliteColumnInt(m_pSqliteStatement, a_nIndex);
}

__int64 KSQLiteResultSet::GetValueInt64(int a_nIndex) const
{
    if (a_nIndex<0 || a_nIndex >= m_nColumnCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    return SqliteColumnInt64(m_pSqliteStatement, a_nIndex);
}

double KSQLiteResultSet::GetValueDouble(int a_nIndex) const
{
    if (a_nIndex<0 || a_nIndex >= m_nColumnCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    return SqliteColumnDouble(m_pSqliteStatement, a_nIndex);
}

const void* KSQLiteResultSet::GetValueBytes( int a_nIndex, int& ar_nSize )
{
    if (a_nIndex<0 || a_nIndex >= m_nColumnCount )
    {
        ThrowException(_T("Invalid Index Error"));
    }

    ar_nSize = SqliteColumnBytes(m_pSqliteStatement, a_nIndex);

    return SqliteColumnBlob(m_pSqliteStatement, a_nIndex);
}
