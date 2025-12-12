#include "StdAfx.h"
#include "KxBulkDbase.h"


KxBulkDbase::KxBulkDbase( KDBaseConPtr spDBaseConnection, KIOTable* a_pIOTable, KIOColumns* a_pUpsertColumns )
    :	m_spDBaseConnection(spDBaseConnection),
    m_pTable(a_pIOTable),
    m_strImportTable(_T("")),
    m_strRenameTable(_T("")),
    m_pUpsertColumns(a_pUpsertColumns),
    m_nUpsertColumnCount(0)
{
    m_strOriginTable = m_pTable->Name();
    OnInitUpsert();
}


KxBulkDbase::~KxBulkDbase(void)
{
}

void KxBulkDbase::OnInitUpsert( void )
{
    m_strImportTable.Format(_T("Import_%s"), m_strOriginTable);
    m_strRenameTable.Format(_T("Rename_%s"), m_strOriginTable);

    RemoveWorkTable();

    m_nUpsertColumnCount = m_pUpsertColumns->ColumnCount();

    CreateImportTable();
    InitPrepared();
}

void KxBulkDbase::RemoveWorkTable( void )
{
    CString strQuery;

    try
    {
        //drop rename table;
        strQuery.Format(_T("drop table %s"), m_strImportTable);
        m_spDBaseConnection->ExecuteUpdate(strQuery);

        //지난 ODUpsert가 정상적으로 수행되지 않음
        TxLogDebugWarning();
    }
    catch (...)
    {
        //지난 ODUpsert가 정상적으로 수행될 경우 해당 블럭을 지남
    }

    try
    {
        //drop rename table;
        strQuery.Format(_T("drop table %s"), m_strRenameTable);
        m_spDBaseConnection->ExecuteUpdate(strQuery);

        //지난 ODUpsert가 정상적으로 수행되지 않음
        TxLogDebugWarning();
    }
    catch (...)
    {
        //지난 ODUpsert가 정상적으로 수행될 경우 해당 블럭을 지남
    }
}

void KxBulkDbase::CreateImportTable( void )
{
    TxLogDebugStartMsg();
    CString strQuery, strColumnName, strQueryConstraint;
    strQuery.Format(_T("Create Table [%s] ( [%s] Integer Not Null, [%s] Integer Not Null"), m_strImportTable, COLUMN_OZONE, COLUMN_DZONE);

    for (int i = 0; i < m_nUpsertColumnCount; ++i)
    {
        KIOColumn* pColumn = m_pUpsertColumns->GetColumn(i);
        strColumnName = pColumn->Name();

        strQuery.Append(_T(", ["));
        strQuery.Append(strColumnName);
        strQuery.Append(_T("] Double"));
    }

    //임시 생성 테이블의 OZone/Dzone의 PK 생성 로직
    strQueryConstraint.Format(_T(", CONSTRAINT [] PRIMARY KEY ([%s], [%s])"), COLUMN_OZONE, COLUMN_DZONE);
    strQuery.Append(strQueryConstraint);

    strQuery.Append(_T(")"));
    m_spDBaseConnection->ExecuteUpdate(strQuery);
    TxLogDebugEndMsg();

}

void KxBulkDbase::InitPrepared( void )
{
    TxLogDebugStartMsg();
    CString strPreparedQuery;
    strPreparedQuery.Format(_T("Insert Into %s Values( ?, ?"), m_strImportTable);

    for (int i = 0; i < m_nUpsertColumnCount; ++i)
    {
        strPreparedQuery.Append(_T(", ?"));
    }
    strPreparedQuery.Append(_T(") "));
    m_spPreparedStatement = m_spDBaseConnection->PrepareStatement(strPreparedQuery);
    TxLogDebugEndMsg();

}

void KxBulkDbase::ImportData( Integer a_nxOZoneID, Integer a_nxDZoneID, KSDoubleRecord* a_pRecord )
{
    int nBindCount(1);

    m_spPreparedStatement->BindInt64(nBindCount++, a_nxOZoneID);
    m_spPreparedStatement->BindInt64(nBindCount++, a_nxDZoneID);

    int nRecordCount = a_pRecord->GetCount();

    for (int i = 0; i < nRecordCount; ++i)
    {
        m_spPreparedStatement->BindDouble(nBindCount++, a_pRecord->GetAt(i));
    }
    m_spPreparedStatement->ExecuteUpdate();
    m_spPreparedStatement->Reset();
}

void KxBulkDbase::ImportData( KODKey a_odKey, int a_nValue )
{
    int nBindCount(1);

    m_spPreparedStatement->BindInt64(nBindCount++, a_odKey.OriginID);
    m_spPreparedStatement->BindInt64(nBindCount++, a_odKey.DestinationID);

    m_spPreparedStatement->BindInt(nBindCount++, a_nValue);
    m_spPreparedStatement->ExecuteUpdate();
    m_spPreparedStatement->Reset();
}

void KxBulkDbase::DeprecatedFnExecute( void )
{
    TxLogDebugStartMsg();
    CString strQuery, strCreateQuery;
    strQuery.Format(_T("select sql from sqlite_master where lower(name) = lower('%s')"), m_strOriginTable);
    KResultSetPtr oResultSetPtr = m_spDBaseConnection->ExecuteQuery(strQuery);

    if (oResultSetPtr->Next())
        strCreateQuery = oResultSetPtr->GetValueString(0);
    else
        ThrowException(_T("테이블이 존재하지 않습니다."));

    strQuery.Format(_T("ALTER TABLE [%s] RENAME TO [%s]"), m_strOriginTable, m_strRenameTable);

    m_spDBaseConnection->ExecuteUpdate(strQuery);
    m_spDBaseConnection->ExecuteUpdate(strCreateQuery);

    /*generate Join Query And Insert*/	
    strQuery.Format(_T("Insert Into  %s Select "), m_strOriginTable);

    const KIOColumns* pColumns = m_pTable->Columns();
    int nOriginColumnCount = pColumns->ColumnCount();

    for (int i = 0; i < nOriginColumnCount; ++i)
    {
        if( i != 0)
            strQuery.Append(_T(", "));

        KIOColumn* pColumn = pColumns->GetColumn(i);
        CString strOriginColumn = pColumn->Name();

        int nImportColumnCount = m_pUpsertColumns->ColumnCount();
        bool bDuplicated = false;

        for (int j = 0; j < nImportColumnCount; ++j)
        {
            KIOColumn* pImportColumn = m_pUpsertColumns->GetColumn(j);
            CString strImportColumn = pImportColumn->Name();

            if( strOriginColumn.CompareNoCase(strImportColumn) == 0)
                bDuplicated = true;
        }

        if( bDuplicated )
        {
            strQuery.Append(_T("B."));
        }
        else
        {
            strQuery.Append(_T("A."));
        }
        strQuery.Append(strOriginColumn);
    }
    // 	strQuery += _T(" From ") + m_strRenameTable + _T(" A, ") + m_strImportTable + _T(" B ");
    // 	strQuery += _T(" Where A.") + (CString)COLUMN_OZONE + _T(" = B.") + (CString)COLUMN_OZONE + _T(" And  A.") + (CString)COLUMN_DZONE + _T(" = B.") + (CString)COLUMN_DZONE;
    //left outer join으로 수정
    strQuery += _T(" From ") + m_strRenameTable + _T(" A left outer join ") + m_strImportTable + _T(" B ");
    strQuery += _T(" on A.") + (CString)COLUMN_OZONE + _T(" = B.") + (CString)COLUMN_OZONE + _T(" And  A.") + (CString)COLUMN_DZONE + _T(" = B.") + (CString)COLUMN_DZONE;


    TxLogDebug(strQuery.AllocSysString());
    m_spDBaseConnection->ExecuteUpdate(strQuery);

    strQuery.Format(_T("drop table %s"), m_strImportTable);
    m_spDBaseConnection->ExecuteUpdate(strQuery);

    strQuery.Format(_T("drop table %s"), m_strRenameTable);
    m_spDBaseConnection->ExecuteUpdate(strQuery);
    TxLogDebugEndMsg();
}

void KxBulkDbase::ExecuteFullOuterJoin( void )
{
    TxLogDebugStartMsg();
    CString strQuery, strCreateQuery;
    strQuery.Format(_T("select sql from sqlite_master where lower(name) = lower('%s')"), m_strOriginTable);
    KResultSetPtr oResultSetPtr = m_spDBaseConnection->ExecuteQuery(strQuery);

    if (oResultSetPtr->Next())
        strCreateQuery = oResultSetPtr->GetValueString(0);
    else
        ThrowException(_T("테이블이 존재하지 않습니다."));

    strQuery.Format(_T("ALTER TABLE [%s] RENAME TO [%s]"), m_strOriginTable, m_strRenameTable);

    m_spDBaseConnection->ExecuteUpdate(strQuery);
    m_spDBaseConnection->ExecuteUpdate(strCreateQuery);

    /*generate Join Query And Insert*/	
    strQuery.Format(_T("Insert Into %s "), m_strOriginTable);

    const KIOColumns* pColumns = m_pTable->Columns();
    int nOriginColumnCount = pColumns->ColumnCount();


#pragma region A left outer join B
    strQuery.Append(_T("Select "));
    for (int i = 0; i < nOriginColumnCount; ++i)
    {
        if( i != 0)
            strQuery.Append(_T(", "));

        KIOColumn* pColumn = pColumns->GetColumn(i);
        CString strOriginColumn = pColumn->Name();

        int nImportColumnCount = m_pUpsertColumns->ColumnCount();
        bool bDuplicated = false;

        for (int j = 0; j < nImportColumnCount; ++j)
        {
            KIOColumn* pImportColumn = m_pUpsertColumns->GetColumn(j);
            CString strImportColumn = pImportColumn->Name();

            if( strOriginColumn.CompareNoCase(strImportColumn) == 0)
                bDuplicated = true;
        }

        if( bDuplicated )
        {
            strQuery.Append(_T("B."));
        }
        else
        {
            strQuery.Append(_T("A."));
        }
        strQuery.Append(strOriginColumn);
    }
    strQuery += _T(" From ") + m_strRenameTable + _T(" A left outer join ") + m_strImportTable + _T(" B ");
    strQuery += _T(" on A.") + (CString)COLUMN_OZONE + _T(" = B.") + (CString)COLUMN_OZONE + _T(" And  A.") + (CString)COLUMN_DZONE + _T(" = B.") + (CString)COLUMN_DZONE;
#pragma endregion A left outer join B

    strQuery.Append(_T(" union "));

#pragma region B left outer join A

    strQuery.Append(_T("Select "));
    for (int i = 0; i < nOriginColumnCount; ++i)
    {
        if( i != 0)
            strQuery.Append(_T(", "));

        KIOColumn* pColumn = pColumns->GetColumn(i);
        CString strOriginColumn = pColumn->Name();

        int nImportColumnCount = m_pUpsertColumns->ColumnCount();

        //동일한 컬럼이 A와 B에 있을시 B의 컬럼을 우선으로 정함
        //컬럼이 PK일 시 B의 컬럼을 우선으로 함
        bool bDuplicated = false;

        //pk 일 경우는 B테이블의 컬럼을 사용함
        if (pColumn->PrimaryKey())
        {
            bDuplicated = true;
        }
        else
        {
            for (int j = 0; j < nImportColumnCount; ++j)
            {
                KIOColumn* pImportColumn = m_pUpsertColumns->GetColumn(j);
                CString strImportColumn = pImportColumn->Name();

                if( strOriginColumn.CompareNoCase(strImportColumn) == 0)
                    bDuplicated = true;
            }
        }

        if( bDuplicated )
        {
            strQuery.Append(_T("B."));
        }
        else
        {
            strQuery.Append(_T("A."));
        }
        strQuery.Append(strOriginColumn);
    }
    strQuery += _T(" From ") + m_strImportTable + _T(" B left outer join ") + m_strRenameTable + _T(" A ");
    strQuery += _T(" on A.") + (CString)COLUMN_OZONE + _T(" = B.") + (CString)COLUMN_OZONE + _T(" And  A.") + (CString)COLUMN_DZONE + _T(" = B.") + (CString)COLUMN_DZONE;

#pragma endregion B left outer join A

    TxLogDebug(strQuery.AllocSysString());
    m_spDBaseConnection->ExecuteUpdate(strQuery);

    strQuery.Format(_T("drop table %s"), m_strImportTable);
    m_spDBaseConnection->ExecuteUpdate(strQuery);

    strQuery.Format(_T("drop table %s"), m_strRenameTable);
    m_spDBaseConnection->ExecuteUpdate(strQuery);
    TxLogDebugEndMsg();
}
