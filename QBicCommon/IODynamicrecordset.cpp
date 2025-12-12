#include "StdAfx.h"
#include "IODynamicRecordset.h"
#include "IOColumns.h"
#include "IOItem.h"
#include "IOQueryBuilder.h"
#include "IORow.h"
#include "IOTable.h"

KIODynamicRecordset::KIODynamicRecordset(KIOTable* pTable) : KIORecordset(pTable)
{
    m_pColumns = new KIOColumns;
    m_pRow     = new KIORow;    
}


KIODynamicRecordset::~KIODynamicRecordset(void)
{
    Clear();
}

void KIODynamicRecordset::Clear(void)
{
    m_nxRowCount = 0;

    delete m_pRow;
    delete m_pColumns;
}


int KIODynamicRecordset::ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder)
{
	try
	{
		m_mapRowID.clear();

		KIOQueryBuilder qb;
		CString strSQL = _T("");
		CString strTableName = _T("");

		if(_tcslen(strTables) > 0)
		{
			strTableName = strTables;
		}
		else
		{
			strTableName = m_pTable->Name();
		}

		*m_pColumns = *pColumns;
		m_strFilter = strFilter;
		m_strOrder  = strOrder;

		strSQL.Format(_T("Select count(*) From %s "), strTableName);
		if (_tcslen(strFilter) > 0)
		{
			strSQL.AppendFormat(_T(" Where %s "), strFilter);
		}       

		KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
		if(spResultSet->Next())
		{
			m_nxRowCount = (size_t)spResultSet->GetValueInt64(0);
		}

		m_strRowSelectSQL = qb.BuildSelectQuery(pColumns, strTableName);    
		m_spDBConnection = spDBConnection;

		ReReadRecord(0);
		return (int)m_nxRowCount;
	}
	catch (KExceptionPtr ex)
	{
		throw ex;
	}
	catch (...)
	{
		throw 1;
	}    
}

// int KIODynamicRecordset::ReadRecord( KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount )
// {
//     try
//     {
//         m_mapRowID.clear();
// 
//         KIOQueryBuilder qb;
//         CString strSQL = _T("");
//         CString strTableName = _T("");
// 
//         if(_tcslen(strTables) > 0) {
//             strTableName = strTables;
//         } else {
//             strTableName = m_pTable->Name();
//         }
// 
//         *m_pColumns = *pColumns;
//         m_strFilter = strFilter;
//         m_strOrder  = strOrder;
// 
//         strSQL.Format(_T("Select count(*) From %s "), strTableName);
//         if (_tcslen(strFilter) > 0)
//         {
//             strSQL.AppendFormat(_T(" Where %s "), strFilter);
//         }       
// 
//         strSQL.AppendFormat(_T(" Limit %d Offset %d "), nRecordCount, nPageOffset);
// 
//         KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
//         if(spResultSet->Next())
//         {
//             m_nxRowCount = (size_t)spResultSet->GetValueInt64(0);
//         }
// 
//         m_strRowSelectSQL = qb.BuildSelectQuery(pColumns, strTableName);    
//         m_spDBConnection = spDBConnection;
// 
//         ReReadRecord(nPageOffset);
//         return (int)m_nxRowCount;
//     }
//     catch (KExceptionPtr ex)
//     {
//         TxLogDebug(ex->GetErrorMessage());
//         throw ex;
//     }
//     catch (...)
//     {
//         TxLogDebugException();
//         throw 1;
//     }    
// }

const KIORow* KIODynamicRecordset::GetRow(size_t nxIndex) 
{
    ASSERT(nxIndex < m_nxRowCount);

    std::map<__int64, __int64>::iterator iter = m_mapRowID.find(nxIndex);
    std::map<__int64, __int64>::iterator end  = m_mapRowID.end();

    m_mapRowID.end();

    __int64 nxRowID(0);
    if (iter != end)
    {
        nxRowID = iter->second;
    }
    else
    {
        ReReadRecord((int)nxIndex);
        iter = m_mapRowID.find(nxIndex);
        if (iter != end)
        {
            nxRowID = iter->second;
        }
        else
        {
            return nullptr;
        }
    }

    CString strSQL;
    strSQL = m_strRowSelectSQL;

    // Edit by Nombara : 2013.05.21
    // Or 연산 문제점 해결
    strSQL += _T(" WHERE ");

    CString strRowID(_T(""));
    strRowID.Format(_T("%s.rowid"), m_pTable->Name());
    strSQL += strRowID;
    strSQL += _T(" = ");
    CString strID;
    strID.Format(_T("%I64d"), nxRowID);
    strSQL += strID;
    // Edit by Nombara : 2013.05.21

    m_pRow->Clear();

    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
    if(spResultSet->Next())
    {
        int nColumnCount = m_pColumns->ColumnCount();
        for(int i = 0; i < nColumnCount; ++i)
        {
            KIOItem* pItem = m_pRow->AddNewItem();
            pItem->SetValue(spResultSet->GetValueString(i));
        }
    }
    else
    {
        return nullptr;
    }

    return m_pRow;
}

void KIODynamicRecordset::ReReadRecord( int a_nStartIndex, int a_nBuffer/*=10000*/ )
{
    CString strSQL = _T("");
    CString strTableName = m_pTable->Name();
    
    //오더링 문제-.rowid, *
    CString strRowID(_T(""));
    strRowID.Format(_T("%s.rowid"), m_pTable->Name());

    strSQL.Format(_T("Select %s, * From %s "), strRowID, strTableName);
    if (_tcslen(m_strFilter) > 0)
    {
        strSQL.AppendFormat(_T(" Where %s "), m_strFilter);
    }    

    if (_tcslen(m_strOrder) > 0)
    {
        strSQL.AppendFormat(_T(" Order By %s "), m_strOrder);
    }

    strSQL.AppendFormat(_T(" Limit 10000 OFFSET %d "), a_nStartIndex);

    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);

    __int64 nxSequence = (__int64)a_nStartIndex;
    while(spResultSet->Next())
    {
        __int64 nxRowID = spResultSet->GetValueInt64(0);
        m_mapRowID.insert(std::make_pair(nxSequence, nxRowID));
        nxSequence++;
    }
}

/* 2013-11-19일 이전 버전
KIODynamicRecordset::KIODynamicRecordset(KIOTable* pTable)
	:	KIORecordset(pTable)
{
	m_pColumns = new KIOColumns;
	m_pRow     = new KIORow;
	m_strRowID = pTable->Name();
	m_strRowID += _T(".rowid");
}


KIODynamicRecordset::~KIODynamicRecordset(void)
{
	Clear();
}

void KIODynamicRecordset::Clear(void)
{
    m_nxRowCount = 0;

    delete m_pRow;
    delete m_pColumns;
}


int KIODynamicRecordset::ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder)
{
    m_vecRowID.clear();

	KIOQueryBuilder qb;
	CString strSQL = _T("");
	CString strTableName = _T("");

	if(_tcslen(strTables) > 0)
	{
		strTableName = strTables;
	}
	else
	{
		strTableName = m_pTable->Name();
	}

	strSQL = qb.BuildSelectQuery(pColumns, strTableName, strFilter, strOrder);
	CString strRowID = m_strRowID;
	strRowID += _T(", ");

	strSQL.Insert(strSQL.Find(_T(' ')) + 1, strRowID);
	
    KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
    while(spResultSet->Next())
    {
        __int64 nxRowID = spResultSet->GetValueInt64(0);
        m_vecRowID.push_back(nxRowID);
    }
    	
	*m_pColumns = *pColumns;
	m_strFilter = strFilter;
	
	m_strRowSelectSQL = qb.BuildSelectQuery(pColumns, strTableName);
	m_nxRowCount = m_vecRowID.size();

    m_spDBConnection = spDBConnection;

	return m_nxRowCount;
}

const KIORow* KIODynamicRecordset::GetRow(size_t nxIndex) const
{
	ASSERT(nxIndex < m_nxRowCount);

	CString strSQL;
	strSQL = m_strRowSelectSQL;
    
    // Edit by Nombara : 2013.05.21
    // Or 연산 문제점 해결
    strSQL += _T(" WHERE ");
    strSQL += m_strRowID;
    strSQL += _T(" = ");
    CString strID;
    strID.Format(_T("%I64d"), m_vecRowID[nxIndex]);
    strSQL += strID;
    // Edit by Nombara : 2013.05.21
	
	m_pRow->Clear();

    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
    if(spResultSet->Next())
    {
        int nColumnCount = m_pColumns->ColumnCount();
        for(int i = 0; i < nColumnCount; ++i)
        {
            KIOItem* pItem = m_pRow->AddNewItem();
            pItem->SetValue(spResultSet->GetValueString(i));
        }
    }
    else
    {
        return nullptr;
    }
    	
	return m_pRow;
}
*/
/* Limit 방법
KIODynamicRecordset::KIODynamicRecordset(KIOTable* pTable)
    :	KIORecordset(pTable)
{
    m_pColumns = new KIOColumns;
    m_pRow     = new KIORow;
    m_strRowID = pTable->Name();
    m_strRowID += _T(".rowid");
}


KIODynamicRecordset::~KIODynamicRecordset(void)
{
    Clear();
}

void KIODynamicRecordset::Clear(void)
{
    m_nxRowCount = 0;

    delete m_pRow;
    delete m_pColumns;
}


int KIODynamicRecordset::ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder)
{
    KIOQueryBuilder qb;
    CString strSQL = _T("");
    CString strTableName = _T("");

    if(_tcslen(strTables) > 0)
    {
        strTableName = strTables;
    }
    else
    {
        strTableName = m_pTable->Name();
    }

    strSQL.Format(_T("Select count(*) From %s "), strTableName);
    if (_tcslen(strFilter) > 0)
    {
        strSQL.AppendFormat(_T(" Where %s "), strFilter);
    }    

    KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
    if(spResultSet->Next())
    {
        m_nxRowCount = (size_t)spResultSet->GetValueInt64(0);
    }

    *m_pColumns = *pColumns;
    m_strFilter = strFilter;

    m_strRowSelectSQL = qb.BuildSelectQuery(pColumns, strTableName, strFilter, strOrder);
    m_spDBConnection = spDBConnection;

    return m_nxRowCount;
}

const KIORow* KIODynamicRecordset::GetRow(size_t nxIndex) const
{
    ASSERT(nxIndex < m_nxRowCount);

    CString strSQL;
    strSQL = m_strRowSelectSQL;

    strSQL.AppendFormat(_T(" Limit 1 Offset %d"), nxIndex);

    m_pRow->Clear();

    KResultSetPtr spResultSet = m_spDBConnection->ExecuteQuery(strSQL);
    if(spResultSet->Next())
    {
        int nColumnCount = m_pColumns->ColumnCount();
        for(int i = 0; i < nColumnCount; ++i)
        {
            KIOItem* pItem = m_pRow->AddNewItem();
            pItem->SetValue(spResultSet->GetValueString(i));
        }
    }
    else
    {
        return nullptr;
    }

    return m_pRow;
}
*/