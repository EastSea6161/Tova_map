#include "StdAfx.h"
#include "IOStaticRecordset.h"
#include "IOColumns.h"
#include "IOcolumn.h"
#include "IOItem.h"
#include "IOQueryBuilder.h"
#include "IORow.h"
#include "IORowCollection.h"
#include "IOTable.h"

KIOStaticRecordset::KIOStaticRecordset(KIOTable* pTable) : KIORecordset(pTable)
{
	m_pColumns = new KIOColumns;
	m_pRows    = new KIORowCollection;
}

KIOStaticRecordset::~KIOStaticRecordset(void)
{
	Clear();
}

void KIOStaticRecordset::Clear(void)
{
    delete m_pRows;
    delete m_pColumns;
}

int KIOStaticRecordset::ReadRecord(KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder)
{    
#pragma region History
    // #1. 2016-01-07
    // strTables -> 무시하는 것으로 결정 함
    // 현재 테이블의 레코드 셋에서 다른 테이블을 가져올 수 있도록 하려는 의도 였으나.. 
    // 타당하지 않는 것으로 결정
    /*CString strTableName = _T("");
    if(_tcslen(strTables) > 0) {
        strTableName = strTables;
    } else {
        strTableName = m_pTable->Name();
    }
    KIOQueryBuilder qb;
    strSQL = qb.BuildSelectQuery(pColumns, strTableName, strFilter, strOrder);*/


#pragma endregion History

    m_nxRowCount = 0;
    m_pRows->Clear(); 
    	
	CString strSQL = _T(""); {
        strSQL.Append(_T(" Select "));

        for (int i=0; i<pColumns->ColumnCount(); i++) {
            KIOColumn* pColumn = pColumns->GetColumn(i);
            if (i == 0) {
                strSQL.AppendFormat(_T(" %s "), pColumn->Name());
            } else {
                strSQL.AppendFormat(_T(",%s "), pColumn->Name());
            }
        }

        strSQL.Append(_T(" From "));

        if (m_pTable->ViewTable() == false) {
            strSQL.AppendFormat(_T("%s"), m_pTable->Name());
        }
        else {
            strSQL.Append(_T(" ( ")); {
                strSQL.AppendFormat(_T("%s"), m_pTable->ViewSQL());
            }
            strSQL.Append(_T(" ) "));
        }


        if (_tcslen(strFilter)) {
            strSQL.AppendFormat(_T(" Where %s "), strFilter);
        }

        if (_tcslen(strOrder)) {
            strSQL.AppendFormat(_T(" Order By %s "), strOrder);
        }
    }

	KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
    while(spResultSet->Next()) {
        KIORow* pRow = m_pRows->AddNewRow();
        for(int i = 0; i < pColumns->ColumnCount(); ++i) {
            KIOItem* pItem = pRow->AddNewItem();
            pItem->SetValue(spResultSet->GetValueString(i));
        }
    }

	*m_pColumns  = *pColumns;
	m_nxRowCount = m_pRows->RowCount();

	return (int)m_nxRowCount;
}

int KIOStaticRecordset::ReadRecord( KDBaseConnectionPtr spDBConnection, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount )
{
    #pragma region History
    // #1. 2016-01-07
    // strTables -> 무시하는 것으로 결정 함
    // 현재 테이블의 레코드 셋에서 다른 테이블을 가져올 수 있도록 하려는 의도 였으나.. 
    // 타당하지 않는 것으로 결정
    /*CString strTableName = _T("");
    if(_tcslen(strTables) > 0) {
        strTableName = strTables;
    } else {
        strTableName = m_pTable->Name();
    }
    KIOQueryBuilder qb;
    strSQL = qb.BuildSelectQuery(pColumns, strTableName, strFilter, strOrder);*/


#pragma endregion History

    m_nxRowCount = 0;
    m_pRows->Clear();

    CString strSQL = _T(""); {
        strSQL.Append(_T(" Select "));

        for (int i=0; i<pColumns->ColumnCount(); i++) {
            KIOColumn* pColumn = pColumns->GetColumn(i);
            if (i == 0) {
                strSQL.AppendFormat(_T(" %s "), pColumn->Name());
            } else {
                strSQL.AppendFormat(_T(",%s "), pColumn->Name());
            }
        }

        strSQL.Append(_T(" From "));

        if (m_pTable->ViewTable() == false) {
            strSQL.AppendFormat(_T("%s"), m_pTable->Name());
        }
        else {
            strSQL.Append(_T(" ( ")); {
                strSQL.AppendFormat(_T("%s"), m_pTable->ViewSQL());
            }
            strSQL.Append(_T(" ) "));
        }


        if (_tcslen(strFilter)) {
            strSQL.AppendFormat(_T(" Where %s "), strFilter);
        }

        if (_tcslen(strOrder)) {
            strSQL.AppendFormat(_T(" Order By %s "), strOrder);
        }

        if (m_enDBType == SQLite) {
            strSQL.AppendFormat(_T(" Limit %d Offset %d "), nRecordCount, nPageOffset);
        }
        else if (m_enDBType == MSSQL2012) {    
        }
    }

    KResultSetPtr spResultSet = spDBConnection->ExecuteQuery(strSQL);
    while(spResultSet->Next()) {
        KIORow* pRow = m_pRows->AddNewRow();
        for(int i = 0; i < pColumns->ColumnCount(); ++i) {
            KIOItem* pItem = pRow->AddNewItem();
            pItem->SetValue(spResultSet->GetValueString(i));
        }
    }

    *m_pColumns  = *pColumns;
    m_nxRowCount = m_pRows->RowCount();

    return (int)m_nxRowCount;
}

const KIORow* KIOStaticRecordset::GetRow(size_t nxIndex) 
{
	ASSERT(nxIndex < m_nxRowCount);	
	return m_pRows->GetRow(nxIndex);
}
