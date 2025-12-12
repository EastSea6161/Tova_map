#include "StdAfx.h"
#include "KDBaseOD2PASum.h"


KDBaseOD2PASum::KDBaseOD2PASum(void)
{
}


KDBaseOD2PASum::~KDBaseOD2PASum(void)
{
}

bool KDBaseOD2PASum::OD2Production(KTarget* pTarget, KIOTable* pTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records)
{
    TxLogDebugStartMsg();
    int nColumnCount = columnCollection.ColumnCount();
    records.RemoveAll();

    KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

    CString strSQL  = _T(" Select OZone_ID ");
    for (int i=0; i<nColumnCount; i++)
    {
        KIOColumn* pColumn = columnCollection.GetColumn(i);
        strSQL += _T(", Sum(") + CString(pColumn->Name()) + _T(") ");
    }
    strSQL += _T(" From ") + CString(pTable->Name()) + _T(" Group By(OZone_ID) ");

    TxLogDebug(strSQL.AllocSysString());

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

    while(spResult->Next())
    {     
        Integer key = spResult->GetValueInt64(0);
        KSDoubleRecord* pRecord = records.AddRecord(key);

        for (int i=1; i<nColumnCount; i++)
        {
            pRecord->SetAt(i-1, spResult->GetValueDouble(i));
        }      
    }

    TxLogDebugEndMsg();
    return true;
}

bool KDBaseOD2PASum::OD2Attraction(KTarget* pTarget, KIOTable* pTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records)
{
    TxLogDebugStartMsg();
    int nColumnCount = columnCollection.ColumnCount();
    records.RemoveAll();

    KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

    CString strSQL  = _T(" Select DZone_ID ");
    for (int i=0; i<nColumnCount; i++)
    {
        KIOColumn* pColumn = columnCollection.GetColumn(i);
        strSQL += _T(", Sum(") + CString(pColumn->Name()) + _T(") ");
    }
    strSQL += _T(" From ") + CString(pTable->Name()) + _T(" Group By(DZone_ID) ");
    TxLogDebug(strSQL.AllocSysString());

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

    while(spResult->Next())
    {     
        Integer key = spResult->GetValueInt64(0);
        KSDoubleRecord* pRecord = records.AddRecord(key);

        for (int i=1; i<nColumnCount; i++)
        {
            pRecord->SetAt(i-1, spResult->GetValueDouble(i));
        }      
    }

    TxLogDebugEndMsg();
    return true;
}