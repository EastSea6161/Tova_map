#include "StdAfx.h"
#include "KBulkDBase.h"
//^#include "DBaseConnector.h"

KBulkDBase::KBulkDBase() 
{
}


KBulkDBase::~KBulkDBase(void)
{
}

bool KBulkDBase::BulkZoneSelect( KIOTable* pIOTable, CString strProductionColumnName, 
                                                     CString strAttractionColumnName, 
                                 KDistributionZoneDataCollection& records)
{    
    records.RemoveAll();

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        CString strSQL = _T("Select zone_id, ") + 
                                    strProductionColumnName + _T(",") + 
                                    strAttractionColumnName + 
                         _T(" From ") + pIOTable->Name();  

        KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            __int64 nZoneID     = spResultSet->GetValueInt64(0);
            double  dProduction = spResultSet->GetValueDouble(1);
            double  dAttraction = spResultSet->GetValueDouble(2);
            records.AddRecord(nZoneID, dProduction, dAttraction);
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }
    
    return true;
}

bool KBulkDBase::BulkOneKeySelect( CString keyField, KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, CString strWhere )
{    
    records.RemoveAll();

    int nColumnCount = columnCollection.ColumnCount();
    if (nColumnCount < 1)
    {
        return false;
    }

    CString strSQL = _T("Select ") + keyField;

    for (int i=0; i<nColumnCount; i++)
    {
        strSQL = strSQL + _T(", ") + columnCollection.GetColumn(i)->Name();
    }

    strSQL = strSQL + _T(" From ") + pIOTable->Name();
    if ( strWhere.GetLength() > 0 )
    {
        strSQL = strSQL + _T(" Where ") + strWhere;
    }

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            __int64 nID     = spResultSet->GetValueInt64(0);    
            KSDoubleRecord* pRecord = records.AddRecord(nID);

            for (int i=0; i<nColumnCount; i++)
            {
                pRecord->SetAt(i, spResultSet->GetValueDouble(i+1));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }
    
    return true;
}

bool KBulkDBase::BulkOneKeySelect( CString keyField, KIOTable* pIOTable, CStringArray& strColumnsArray, KIDKeyDoubleRecords& records, CString strWhere )
{
    records.RemoveAll();   

    int nColumnCount = strColumnsArray.GetCount();
    if (nColumnCount < 1)
    {
        return false;
    }

    CString strSQL = _T("Select ") + keyField;

    for (int i=0; i<nColumnCount; i++)
    {
        strSQL = strSQL + _T(", ") + strColumnsArray.GetAt(i);
    }

    strSQL = strSQL + _T(" From ") + pIOTable->Name();
    if ( strWhere.GetLength() > 0 )
    {
        strSQL = strSQL + _T(" Where ") + strWhere;
    }

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            __int64 nID     = spResultSet->GetValueInt64(0);  
            KSDoubleRecord* pRecord = records.AddRecord(nID);

            for (int i=0; i<nColumnCount; i++)
            {
                pRecord->SetAt(i, spResultSet->GetValueDouble(i+1));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }
    
    return true;
}

bool KBulkDBase::BulkZoneSelect( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, CString strWhere)
{
    return BulkOneKeySelect( _T("zone_id"), pIOTable, columnCollection, records, strWhere );
}

bool KBulkDBase::BulkZoneSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, double >& records, CString strWhere )
{
    return BulkOneKeySelect( _T("zone_id"), pIOTable, fieldName, records, strWhere );
}

bool KBulkDBase::BulkZoneSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, int >& records, CString strWhere )
{
    return BulkOneKeySelect( _T("zone_id"), pIOTable, fieldName, records, strWhere );
}

bool KBulkDBase::BulkZoneSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, CString >& records, CString strWhere )
{
    return BulkOneKeySelect( _T("zone_id"), pIOTable, fieldName, records, strWhere );
}

bool KBulkDBase::BulkNodeSelect( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records)
{
    return BulkOneKeySelect( _T("node_id"), pIOTable, columnCollection, records);
}

bool KBulkDBase::BulkNodeSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, double >& records )
{
    return BulkOneKeySelect( _T("node_id"), pIOTable, fieldName, records );
}

bool KBulkDBase::BulkNodeSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, int >& records )
{
    return BulkOneKeySelect( _T("node_id"), pIOTable, fieldName, records );
}

bool KBulkDBase::BulkNodeSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, CString >& records )
{
    return BulkOneKeySelect( _T("node_id"), pIOTable, fieldName, records );
}

bool KBulkDBase::BulkLinkSelect( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, CString strWhere)
{
    return BulkOneKeySelect( _T("link_id"), pIOTable, columnCollection, records, strWhere);
}

bool KBulkDBase::BulkLinkSelect( KIOTable* pIOTable, CStringArray& strColumnsArray, KIDKeyDoubleRecords& records, CString strWhere )
{
    return BulkOneKeySelect( _T("link_id"), pIOTable, strColumnsArray, records, strWhere);
}

bool KBulkDBase::BulkLinkSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, double >& records )
{
    return BulkOneKeySelect( _T("link_id"), pIOTable, fieldName, records );
}

bool KBulkDBase::BulkLinkSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, int >& records )
{
    return BulkOneKeySelect( _T("link_id"), pIOTable, fieldName, records );
}

bool KBulkDBase::BulkLinkSelect( KIOTable* pIOTable, CString fieldName,  std::map<Integer, CString >& records )
{
    return BulkOneKeySelect( _T("link_id"), pIOTable, fieldName, records );
}

bool KBulkDBase::BulkLinkSelect(KIOTable* pIOTable, std::map<Integer, TFixedLinkDB>& records, CString strWhere /* = _T */)
{
    records.clear();

    CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed From Link ") ;
    if ( strWhere.GetLength() > 0 )
    {
        strSQL = strSQL + _T(" Where ") + strWhere;
    }

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            TFixedLinkDB     record;
            Integer nxLinkID = spResultSet->GetValueInt64 (0);  
            record.link_id   = nxLinkID;
            record.fnode_id  = spResultSet->GetValueInt64 (1);    
            record.tnode_id  = spResultSet->GetValueInt64 (2);    
            record.link_type = spResultSet->GetValueInt   (3);    
            record.length    = spResultSet->GetValueDouble(4);
            record.lane      = spResultSet->GetValueInt   (5);
            record.capa      = spResultSet->GetValueDouble(6);
            record.speed     = spResultSet->GetValueDouble(7);
            records.insert(std::make_pair(nxLinkID, record));
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }    

    return true;
}

bool KBulkDBase::BulkInputCoeff(KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, CString strWhere /* = _T */)
{
    return BulkOneKeySelect(_T("zone_id"), pIOTable, columnCollection, records, strWhere);
}

bool KBulkDBase::BulkLinkFTNodeSelect(KDBaseConPtr a_spDBaseConnection, std::map<Integer, TFTNode>& records)
{
    records.clear();

    TFTNode linkFTNode={0,0};
    CString strSQL = _T("Select link_id, fnode_id, tnode_id From Link");
    try
    {
        KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            Integer    linkID   = spResultSet->GetValueInt64 (0);        
            linkFTNode.fnode_id = spResultSet->GetValueInt64 (1);    
            linkFTNode.tnode_id = spResultSet->GetValueInt64 (2);    

            records.insert(std::make_pair(linkID, linkFTNode));
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }    
        
    return true;
}

bool KBulkDBase::BulkOneKeySelect(CString keyField, KIOTable* pIOTable, CString fieldName, std::map<Integer, double>& records, CString strWhere)
{
    records.clear();

    CString  strSQL = _T("Select ") + keyField + _T(", ") + fieldName;
    strSQL = strSQL + _T(" From ") + pIOTable->Name();
    if ( strWhere.GetLength() > 0 )
    {
        strSQL = strSQL + _T(" Where ") + strWhere;
    }

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            Integer nID     = spResultSet->GetValueInt64 (0);        
            double  dValue  = spResultSet->GetValueDouble(1);

            records.insert(std::make_pair(nID, dValue));
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }
    
    return true;
}

bool KBulkDBase::BulkOneKeySelect(CString keyField, KIOTable* pIOTable, CString fieldName, std::map<Integer, int>& records, CString strWhere)
{
    records.clear();    

    CString  strSQL = _T("Select ") + keyField + _T(", ") + fieldName;
    strSQL = strSQL + _T(" From ") + pIOTable->Name();
    if ( strWhere.GetLength() > 0 )
    {
        strSQL = strSQL + _T(" Where ") + strWhere;
    }

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            Integer nID     = spResultSet->GetValueInt64 (0);        
            int     nValue  = spResultSet->GetValueInt   (1);

            records.insert(std::make_pair(nID, nValue));
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }
    
    return true;
}

bool KBulkDBase::BulkOneKeySelect(CString keyField, KIOTable* pIOTable, CString fieldName, std::map<Integer, CString>& records, CString strWhere)
{
    records.clear();

    CString  strSQL = _T("Select ") + keyField + _T(", ") + fieldName;
    strSQL = strSQL + _T(" From ") + pIOTable->Name();

    if ( strWhere.GetLength() > 0 )
    {
        strSQL = strSQL + _T(" Where ") + strWhere;
    }

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            Integer nID       = spResultSet->GetValueInt64 (0);        
            CString strValue  = spResultSet->GetValueString(1);

            records.insert(std::make_pair(nID, strValue));
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}


bool KBulkDBase::BulkRoadRailTransportODSelect( KDBaseConPtr a_spDBaseConnection, int a_nPurposeID, KODKeyIntegerRecords& records )
{
	records.RemoveAll();
    
	CString strSQL(_T("")); 
	strSQL.AppendFormat(_T(" Select ozone_id, dzone_id, "));
	strSQL.AppendFormat(_T(" RoadVia_%d,   (Select zone_id From node where node_id = RoadVia_%d) as Rc, "), a_nPurposeID, a_nPurposeID );
	strSQL.AppendFormat(_T(" TerminalA_%d, (Select zone_id From node where node_id = TerminalA_%d) as Ta, "), a_nPurposeID, a_nPurposeID);
	strSQL.AppendFormat(_T(" TerminalB_%d, (Select zone_id From node where node_id = TerminalB_%d) as Tb From %s"), a_nPurposeID, a_nPurposeID, TABLE_F_TERMINAL_PATH_INFO);
	
    KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
    while (spResultSet->Next())
    {
        __int64 nOZoneID = spResultSet->GetValueInt64(0) ;
        __int64 nDZoneID = spResultSet->GetValueInt64(1) ;

        KIntegerRecord* pRecord = records.AddRecord(KODKey(nOZoneID, nDZoneID));

        Integer nxNodeID =  spResultSet->GetValueInt64(2);
        pRecord->SetAt(0, spResultSet->GetValueInt64(3));

        nxNodeID =  spResultSet->GetValueInt64(4);
        pRecord->SetAt(1, spResultSet->GetValueInt64(5));

        nxNodeID =  spResultSet->GetValueInt64(6);
        pRecord->SetAt(2, spResultSet->GetValueInt64(7));
    }

	return true;
}


bool KBulkDBase::BulkODSelect( KIOTable* pIOTable, CString strColumnName, std::map<KODKey, double>& records )
{
    records.clear();

    CString strSQL = _T("Select ozone_id, dzone_id, ") + strColumnName + _T(" ") + 
                     _T("From ") + pIOTable->Name();

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            __int64 nOZoneID = spResultSet->GetValueInt64(0) ;
            __int64 nDZoneID = spResultSet->GetValueInt64(1) ;

            records.insert(std::make_pair(KODKey(nOZoneID, nDZoneID), spResultSet->GetValueDouble(2)));
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }

    return true;
}

bool KBulkDBase::BulkODSelect( KIOTable* pIOTable, KIOColumns& columnCollection, KODKeyDoubleRecords& records )
{
    records.RemoveAll();
    
    int nColumnCount = columnCollection.ColumnCount();
    if (nColumnCount < 1)
    {
        return false;
    }

    CString strSQL = _T("Select ozone_id, dzone_id ");

    for (int i=0; i<nColumnCount; i++)
    {
        strSQL = strSQL + _T(",") + columnCollection.GetColumn(i)->Name();
    }

    strSQL = strSQL + _T(" From ") + pIOTable->Name();

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            __int64 nOZoneID = spResultSet->GetValueInt64(0) ;
            __int64 nDZoneID = spResultSet->GetValueInt64(1) ;

            KSDoubleRecord* pRecord = records.AddRecord(KODKey(nOZoneID, nDZoneID));
            for (int i=0; i<nColumnCount; i++)
            {
                pRecord->SetAt(i, spResultSet->GetValueDouble(i+2));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}

bool KBulkDBase::BulkODSelect( KIOTable* pIOTable, CString strFilter, KIOColumns& columnCollection, KODKeyDoubleRecords& records )
{
    records.RemoveAll(); 

    int nColumnCount = columnCollection.ColumnCount();
    if (nColumnCount < 1)
    {
        return false;
    }

    CString strSQL = _T("Select ozone_id, dzone_id ");
    for (int i=0; i<nColumnCount; i++)
    {
        strSQL = strSQL + _T(",") + columnCollection.GetColumn(i)->Name();
    }

    strSQL = strSQL + _T(" From  ") + pIOTable->Name();

    if (strFilter.GetLength() > 0)
    {
        strSQL = strSQL + _T(" Where ") + strFilter;
    }    

    try
    {
        KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            __int64 nOZoneID = spResultSet->GetValueInt64(0);
            __int64 nDZoneID = spResultSet->GetValueInt64(1);

            KSDoubleRecord* pRecord = records.AddRecord(KODKey(nOZoneID, nDZoneID));
            for (int i=0; i<nColumnCount; i++)
            {
                pRecord->SetAt(i, spResultSet->GetValueDouble(i+2));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}

bool KBulkDBase::BulkODSelect( KDBaseConPtr a_spDBaseConnection, CString strTableName, KIOColumns& columnCollection, KODKeyDoubleRecords& records )
{
    records.RemoveAll();

    int nColumnCount = columnCollection.ColumnCount();
    if (nColumnCount < 1)
    {
        return false;
    }

    CString strSQL = _T("Select ozone_id, dzone_id ");

    for (int i=0; i<nColumnCount; i++)
    {
        strSQL = strSQL + _T(",") + columnCollection.GetColumn(i)->Name();
    }

    strSQL = strSQL + _T(" From ") + strTableName;

    try
    {
        KResultSetPtr       spResultSet       = a_spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            __int64 nOZoneID = spResultSet->GetValueInt64(0);
            __int64 nDZoneID = spResultSet->GetValueInt64(1);

            KSDoubleRecord* pRecord = records.AddRecord(KODKey(nOZoneID, nDZoneID));
            for (int i=0; i<nColumnCount; i++)
            {
                pRecord->SetAt(i, spResultSet->GetValueDouble(i+2));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
        TxLogDebugException();
        return false;
    }

    return true;
}


bool KBulkDBase::BulkODSelectByModeChoice( KIOTable* pIOTable, CString strFilter, KIOColumns& columnCollection, KODKeyDoubleRecords& records )
{
	records.RemoveAll(); 

	int nColumnCount = columnCollection.ColumnCount();
	if (nColumnCount < 1)
	{
		return false;
	}

	CString strSQL = _T("Select ozone_id, dzone_id ");
	for (int i=0; i<nColumnCount; i++)
	{
		strSQL = strSQL + _T(",") + columnCollection.GetColumn(i)->Name();
	}

	strSQL = strSQL + _T(" From  ") + pIOTable->Name();

	if (strFilter.GetLength() > 0)
	{
		strSQL = strSQL + _T(" Where ") + strFilter;
	}    

	try
	{
		KDBaseConPtr spDBaseConnection = pIOTable->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
		while (spResultSet->Next())
		{
			__int64 nOZoneID = spResultSet->GetValueInt64(0);
			__int64 nDZoneID = spResultSet->GetValueInt64(1);

			KSDoubleRecord* pRecord = records.AddRecord(KODKey(nOZoneID, nDZoneID));
			for (int i=0; i<nColumnCount; i++)
			{
				pRecord->SetAt(i, (double)spResultSet->GetValueInt(i+2));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KBulkDBase::BulkOneKeyUpdate( CString keyField, KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, bool bAutoCommit )
{
    TxLogDebugStartMsg();
    
    CString strColumnNames;
    int nColumnCount = columnCollection.ColumnCount();
    if (nColumnCount < 1)
    {
        TxLogDebugException();
        return false;
    }

    for (int i=0; i<nColumnCount; i++)
    {
        strColumnNames = strColumnNames + columnCollection.GetColumn(i)->Name() + _T(" = ? ");
        if (i < nColumnCount-1)
        {
            strColumnNames = strColumnNames + _T(", ");
        }
    }

    CString  strSQL = _T(" Update ") + CString(pIOTable->Name()) + _T(" Set ") + strColumnNames; 
    strSQL = strSQL + _T(" Where ") + keyField + _T(" = ? ");

    TxLogDebug(strSQL.AllocSysString());

    KDBaseConPtr spDBConnection = pIOTable->GetDBaseConnection();

    try
    {   
        if (bAutoCommit)
        {
            spDBConnection->BeginTransaction();
        } 

        KPreparedStatementPtr spPreparedStatement = spDBConnection->PrepareStatement(strSQL);

        std::map<Integer, KSDoubleRecord*>::const_iterator iter = records.begin();
        std::map<Integer, KSDoubleRecord*>::const_iterator end  = records.end();

        while(iter != end)
        {
            KSDoubleRecord* pRecord = iter->second;

            for (int i=0; i<nColumnCount; i++)
            {
                spPreparedStatement->BindDouble(i+1, pRecord->GetAt(i));
            }

            spPreparedStatement->BindInt64(nColumnCount+1, iter->first);
            spPreparedStatement->ExecuteUpdate();

            spPreparedStatement->Reset();

            ++iter;
        }      

        if (bAutoCommit)
        {
            spDBConnection->Commit();    
        }
    }
    catch(...)
    {
        if (bAutoCommit)
        {
            spDBConnection->RollBack();
        }
        return false;
    }
    TxLogDebugEndMsg();
    return true;
}

void KBulkDBase::BulkOneKeyUpdate( KDBaseConPtr a_spDBaseConnection, CString a_strKeyField, KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& a_records )
{
	TxLogDebugStartMsg();
	CString strColumnNames;
	int nColumnCount = a_columnCollection.ColumnCount();

	if( nColumnCount < 1 )
		ThrowException(_T("No Update Column"));

	for (int i = 0; i < nColumnCount; ++i)
	{
		strColumnNames = strColumnNames + a_columnCollection.GetColumn(i)->Name() + _T(" = ? ");

		if (i < nColumnCount-1)
		{
			strColumnNames = strColumnNames + _T(", ");
		}
	}
	CString  strQuery = _T(" Update ") + CString(a_pIOTable->Name()) + _T(" Set ") + strColumnNames; 
	strQuery = strQuery + _T(" Where ") + a_strKeyField + _T(" = ? ");

	TxLogDebug(strQuery.AllocSysString());

	KPreparedStatementPtr oPreparedStatementPtr = a_spDBaseConnection->PrepareStatement(strQuery);

	std::map<Integer, KSDoubleRecord*>::const_iterator iter = a_records.begin();
	std::map<Integer, KSDoubleRecord*>::const_iterator end  = a_records.end();

	while(iter != end)
	{
		KSDoubleRecord* pRecord = iter->second;
		int nBindCount(1);

		for (int i=0; i<nColumnCount; i++)
		{
			oPreparedStatementPtr->BindDouble(nBindCount++, pRecord->GetAt(i));
		}
		oPreparedStatementPtr->BindInt64(nBindCount++, iter->first);

		oPreparedStatementPtr->ExecuteUpdate();
		oPreparedStatementPtr->Reset();
		++iter;
	}
	TxLogDebugEndMsg();
}


bool KBulkDBase::BulkODUpsert( KIOTable* pIOTable, KIOColumns& upsertColumnCollection, KODKeyDoubleRecords& upsertRecords, bool bAutoCommit  )
{
    TxLogDebugStartMsg();
 
    if (upsertRecords.GetRecordCount() < 1)
    {
        TxLogDebugEndMsg();
        return true;
    }
          
    // OD 전체 컬럼에서 OZone, DZone ID 삭제
    KIOColumns totalColumns = *(pIOTable->Columns());    
    int nIndex = totalColumns.GetColumnIndex(COLUMN_OZONE);
    if (nIndex >= 0)
    {
        totalColumns.RemoveColumn(nIndex);
    }
    nIndex = totalColumns.GetColumnIndex(COLUMN_DZONE);
    if (nIndex >= 0)
    {
        totalColumns.RemoveColumn(nIndex);
    }
    int nColumnCount = totalColumns.ColumnCount();
          
    // 전체 OD Select
    KODKeyDoubleRecords allRecords(nColumnCount);
    BulkODSelect(pIOTable, totalColumns, allRecords);   
          
    CString strTemp;
    strTemp.Format(_T("allRecords : %d, upsertRecords : %d"), allRecords.GetRecordCount(), upsertRecords.GetRecordCount());
    TxLogDebug(strTemp.AllocSysString());

    CArray<size_t> upsertIndexArray;
    int nUpserColumnCount = upsertColumnCollection.ColumnCount();
          
    for (int i=0; i<nUpserColumnCount; i++)
    {
        nIndex = totalColumns.GetColumnIndex(upsertColumnCollection.GetColumn(i));
        upsertIndexArray.Add(nIndex);
    }
          
    int nUpsertCount = upsertIndexArray.GetCount();
       
    { // 초기화 루틴 
        std::map<KODKey, KSDoubleRecord*>::iterator iter = allRecords.begin(); 
        std::map<KODKey, KSDoubleRecord*>::iterator end  = allRecords.end();

        for (/**/; iter != end; ++iter)
        {
            KSDoubleRecord* pRecord = iter->second;
            for (int i=0; i<nUpsertCount; i++)
            {
                pRecord->SetAt(upsertIndexArray[i], 0.0);
            }
        }
    }    
    
    // 입력 레코드
    std::map<KODKey, KSDoubleRecord*>::iterator iter = upsertRecords.begin();
    std::map<KODKey, KSDoubleRecord*>::iterator end  = upsertRecords.end();
    while (iter != end)
    {
        KSDoubleRecord* pOutRecord = allRecords.GetExistOrNewRecord(iter->first);
        KSDoubleRecord* pInRecord  = iter->second;
          
        for (int i=0; i<nUpsertCount; i++)
        {
            pOutRecord->SetAt(upsertIndexArray[i], pInRecord->GetAt(i));
        }        
        ++iter;
    }
                     
    // Delete Record
    CString strSQLDelete(_T(" Delete From ") + CString(pIOTable->Name()));
          
    // Insert Record
    CString strSQLInsert(_T(" Insert Into "));
    strSQLInsert = strSQLInsert + pIOTable->Name() + _T(" (ozone_id, dzone_id");
          
    for (int i=0; i<nColumnCount; i++)
    {
        strSQLInsert = strSQLInsert + _T(", ") + totalColumns.GetColumn(i)->Name();
    }
          
    strSQLInsert = strSQLInsert + _T(") Values (?, ? ");
          
    for (int i=0; i<nColumnCount; i++)
    {
        strSQLInsert = strSQLInsert + _T(", ?");
    }
          
    strSQLInsert = strSQLInsert + _T(")");
        
    KDBaseConPtr spDBConnection = pIOTable->GetDBaseConnection();

    try
    {  
        if (bAutoCommit)
        {
            spDBConnection->BeginTransaction();
        }   
                          
        if ( spDBConnection->ExecuteUpdate(strSQLDelete) != SQLITE_DONE )
        {
            TxLogDebug(strSQLDelete.AllocSysString());
          
            if (bAutoCommit)
            {
                spDBConnection->RollBack();
            }
            return false;
        }
         
        KPreparedStatementPtr prepareStatementPtr = spDBConnection->PrepareStatement(strSQLInsert);
             
        std::map<KODKey, KSDoubleRecord*>::iterator iter = allRecords.begin();
        std::map<KODKey, KSDoubleRecord*>::iterator end  = allRecords.end();
          
        while(iter != end)
        {
            KODKey          key     = iter->first;
            KSDoubleRecord* pRecord = iter->second;
          
            prepareStatementPtr->BindInt64(1, key.OriginID);
            prepareStatementPtr->BindInt64(2, key.DestinationID);
          
            for (int i=0; i<nColumnCount; i++)
            {
                prepareStatementPtr->BindDouble(i+3, pRecord->GetAt(i));             
            }            
          
            prepareStatementPtr->ExecuteUpdate();
            prepareStatementPtr->Reset();
          
            ++iter;
        }    
                  
        if (bAutoCommit)
        {
            spDBConnection->Commit(); 
        }

        TxLogDebugEndMsg();
        return true;
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
    }
    catch(...)
    {   
        TxLogDebugWarning();
    }

    if (bAutoCommit)
    {
        try
        {
            spDBConnection->RollBack();
        }
        catch(...)
        {
            TxLogDebugWarning();
        }        
    }
    
    return false;
}


bool KBulkDBase::BulkZoneUpdate( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, bool bAutoCommit )
{
    return BulkOneKeyUpdate(_T("zone_id"), pIOTable, columnCollection, records, bAutoCommit);
}


bool KBulkDBase::BulkZoneUpdate( KIOTable* pIOTable, KIOColumn* pColumn, std::map<Integer, double>& records, bool bAutoCommit )
{
    TxLogDebugStartMsg();

    KDBaseConPtr spDBConnection = pIOTable->GetDBaseConnection();

    CString strKeyField   = _T("zone_id");
    CString strColumnName = pColumn->Name();

    CString  strSQL = _T(" Update ") + CString(pIOTable->Name()) + _T(" Set ") + strColumnName + _T(" = ? ") ; 
    strSQL = strSQL + _T(" Where ")  + strKeyField + _T(" = ? ");
    
    try
    {  
        if (bAutoCommit)
        {
            spDBConnection->BeginTransaction();
        }        

        KPreparedStatementPtr spPreparedStatement = spDBConnection->PrepareStatement(strSQL);

        std::map<Integer, double>::const_iterator iter = records.begin();
        std::map<Integer, double>::const_iterator end  = records.end();

        while(iter != end)
        {
            spPreparedStatement->BindDouble(1, iter->second);
            spPreparedStatement->BindInt64 (2, iter->first);

            spPreparedStatement->ExecuteUpdate();
            spPreparedStatement->Reset();

            ++iter;
        }      

        if (bAutoCommit)
        {
            spDBConnection->Commit();  
        }
    }
    catch(...)
    {
        if (bAutoCommit)
        {
            spDBConnection->RollBack();
        }
        return false;
    }
    TxLogDebugEndMsg();
    return true;
}


bool KBulkDBase::BulkNodeUpdate( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, bool bAutoCommit )
{
    return BulkOneKeyUpdate(_T("node_id"), pIOTable, columnCollection, records, bAutoCommit);
}


void KBulkDBase::BulkNodeUpdata( KDBaseConPtr a_spDBaseConnection, KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& records )
{
	return BulkOneKeyUpdate(a_spDBaseConnection, COLUMN_NODE_ID, a_pIOTable, a_columnCollection, records);
}

void KBulkDBase::BulkTerminalResultUpdata( KDBaseConPtr a_spDBaseConnection, KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& records )
{
	return BulkOneKeyUpdate(a_spDBaseConnection, _T("Terminal_id"), a_pIOTable, a_columnCollection, records);
}


bool KBulkDBase::BulkLinkUpdate( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, bool bAutoCommit )
{
    return BulkOneKeyUpdate(_T("link_id"), pIOTable, columnCollection, records, bAutoCommit);
}


void KBulkDBase::BulkLinkUpdate( KDBaseConPtr a_spDBaseConnection, KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& records )
{
	return BulkOneKeyUpdate(a_spDBaseConnection, COLUMN_LINK_ID, a_pIOTable, a_columnCollection, records);
}


void KBulkDBase::BulkLineinfoUpdate( KDBaseConPtr a_spDBaseConnection, KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& records )
{
	return BulkOneKeyUpdate(a_spDBaseConnection, COLUMN_TRANSIT_ID, a_pIOTable, a_columnCollection, records);
}


bool KBulkDBase::BulkTransitSelect (KDBaseConPtr a_spDBaseConnection, std::vector<TFixedTransitDB>& records)
{
    records.clear();

    TFixedTransitDB record;

	CString strSQL(_T(""));
	strSQL.Append(_T(" SELECT transit_id, type, detail_type, name, peak_headway, offpeak_headway, avg_headway, "));
	strSQL.Append(_T(" peak_frequency, offpeak_frequency, total_frequency, commercial_speed, seat_count, capacity "));
	strSQL.AppendFormat(_T(" FROM %s order by transit_ID "), TABLE_TRANSIT);
	
    try
    {
        KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            record.transit_id        = spResultSet->GetValueInt64 (0);      
            record.type              = spResultSet->GetValueInt   (1);    
            record.detail_type       = spResultSet->GetValueInt   (2);  
            record.name              = spResultSet->GetValueString(3);

            record.peak_headway      = spResultSet->GetValueDouble(4);
            record.offpeak_headway   = spResultSet->GetValueDouble(5); 
            record.avg_headway       = spResultSet->GetValueDouble(6);  
            record.peak_frequency    = spResultSet->GetValueDouble(7); 
			record.offpeak_frequency = spResultSet->GetValueDouble(8);
            record.total_frequency   = spResultSet->GetValueDouble(9);
			record.commercial_speed  = spResultSet->GetValueDouble(10);

			record.seat_count        = spResultSet->GetValueInt   (11); 
            record.capacity          = spResultSet->GetValueInt   (12); 

            records.push_back(record);
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }   

    return true;
}


bool KBulkDBase::BulkTransitSelect ( KDBaseConPtr a_spDBaseConnection, std::vector<TFixedTransitFile>& records)
{
    TxLogDebugStartMsg();
    records.clear();

    TFixedTransitFile record;

	CString strFieldNameHeadway  (_T("avg_headway"));
	CString strFieldNameFrequency(_T("total_frequency"));

    CString strSQL(_T(""));
	strSQL.Append      (_T(" SELECT transit_ID, type, detail_type, "));
	strSQL.AppendFormat(_T(" %s, %s, "), strFieldNameHeadway, strFieldNameFrequency);
	strSQL.Append      (_T(" commercial_speed, seat_count, capacity "));
	strSQL.AppendFormat(_T(" FROM %s order by transit_ID "), TABLE_TRANSIT);

    try
    {
        KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            record.transit_id         = spResultSet->GetValueInt64 (0);        
            record.type               = spResultSet->GetValueInt   (1);  
            record.detail_type        = spResultSet->GetValueInt   (2);  
            record.headway            = spResultSet->GetValueDouble(3); 
            record.frequency          = spResultSet->GetValueDouble(4); 
            record.commercial_speed   = spResultSet->GetValueDouble(5); 
            record.seat_count         = spResultSet->GetValueInt   (6); 
            record.capacity           = spResultSet->GetValueInt   (7); 

            records.push_back(record);
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }    

    return true;
}

bool KBulkDBase::BulkTransitLinksSelect ( KDBaseConPtr a_spDBaseConnection, std::vector<TFixedTransitLink>& records, Integer transit_id )
{
    records.clear();
    TFixedTransitLink record;
    
    CString strSQL;
    strSQL.Format(_T("Select link_id, seq, station_yn From transit_links Where transit_id = %I64d order by seq "), transit_id);

    KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
    while(spResultSet->Next())
    {
        record.link_id    = spResultSet->GetValueInt64(0);        
        record.seq        = spResultSet->GetValueInt  (1);    
        record.station_yn = spResultSet->GetValueInt  (2);  

        records.push_back(record);
    }
    
    return true;
}


bool KBulkDBase::BulkTurnSelect( KDBaseConPtr a_spDBaseConnection, std::map<Integer, TFixedTurnDB>& records )
{
	records.clear();

	TFixedTurnDB turnRecord;
	CString strSQL = _T("Select turn_id, inlink_id, node_id, outlink_id, turn_type, turn_time From Turn ");

    try
    {
        KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        while (spResultSet->Next())
        {
            Integer    turnID     = spResultSet->GetValueInt64(0);        
            turnRecord.inlink_id  = spResultSet->GetValueInt64(1);    
            turnRecord.node_id    = spResultSet->GetValueInt64(2);    
            turnRecord.outlink_id = spResultSet->GetValueInt64(3);  
            turnRecord.turn_type  = spResultSet->GetValueInt  (4);  
            turnRecord.turn_time  = spResultSet->GetValueInt  (5);

            records.insert(std::make_pair(turnID, turnRecord));
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }	

	return true;
}


bool KBulkDBase::BulkSedPopSelect(KDBaseConPtr a_spDBaseConnection, int year, std::vector<TFixedSEDPop>& records)
{
    TxLogDebugStartMsg();
    records.clear();

	try
	{
		TFixedSEDPop record;

		CString strSQL;
		strSQL.Format(_T("Select zone_id, sex, age, value From sed_pop Where year = %d"), year);

		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			record.zone_id  = spResult->GetValueInt64 (0);        
			record.sex      = spResult->GetValueInt   (1);
			record.age      = spResult->GetValueInt   (2);
			record.value    = spResult->GetValueDouble(3);

			records.push_back(record);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		records.clear();
		return false;
	}

    TxLogDebugEndMsg();
    return true;
}

bool KBulkDBase::BulkLinkInfluenceOff(KDBaseConPtr a_spDBaseConnection)
{
    TxLogDebugStartMsg();

    CString  strSQL = _T(" Update Link Set Influence = 0 ");
    
    try
    {
		a_spDBaseConnection->ExecuteUpdate(strSQL);
    }
    catch(...)
    {
        return false;
    }

	TxLogDebugEndMsg();
    return true;
}

bool KBulkDBase::BulkSEDVectorSelect(KDBaseConPtr a_spDBaseConnection, std::vector<TFixedSEDVector>& records )
{
	TxLogDebugStartMsg();
	records.clear();
	TFixedSEDVector record;

	CString strSQL = _T("");
	strSQL.Format(_T("Select Zone_ID, Name, Full_Name, Main_Category From sed_vector"));
	KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);
	while (spResult->Next())
	{
		record.nZone_id      = spResult->GetValueInt64(0);
		record.strName       = spResult->GetValueString(1);
		record.strFull_Name  = spResult->GetValueString(2);
		record.nMCategory_id = spResult->GetValueInt(3); 
		records.push_back(record);
	}

	return true;
}

bool KBulkDBase::BulkSedPopSelect(KDBaseConPtr a_spDBaseConnection, std::vector<TFixedSEDPop>& records)
{
	TxLogDebugStartMsg();
	records.clear();

	try
	{
		TFixedSEDPop record;

		CString strSQL;
		strSQL.Format(_T("Select zone_id, sex, age, value From sed_pop"));

		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			record.zone_id  = spResult->GetValueInt64 (0);
			record.sex      = spResult->GetValueInt   (1);
			record.age      = spResult->GetValueInt   (2);
			record.value    = spResult->GetValueDouble(3);

			records.push_back(record);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}

bool KBulkDBase::BulkNodeSelectAll( KTarget* a_pTarget, std::map<Integer,TFixedNodeInfo>& ar_mapNode )
{
	ar_mapNode.clear();
	CString strSQL(_T(""));

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select node_id, name,node_type, x, y, zone_id, target_region, parking_fee From %s order by node_id "), TABLE_NODE);
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			TFixedNodeInfo oNode ={};

			oNode.node_id		= spResultSet->GetValueInt64(0);
			oNode.name			= spResultSet->GetValueString(1);
			oNode.node_type		= spResultSet->GetValueInt(2);
			oNode.x				= spResultSet->GetValueDouble(3);
			oNode.y				= spResultSet->GetValueDouble(4);
			oNode.zone_id		= spResultSet->GetValueInt64(5);
			oNode.target_region = spResultSet->GetValueInt(6);
			oNode.parking_fee	= spResultSet->GetValueDouble(7);

			ar_mapNode.insert(std::make_pair(oNode.node_id, oNode));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ar_mapNode.clear();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		ar_mapNode.clear();
		return false;
	}
	return true;
}


bool KBulkDBase::BulkSEDVectorSelect( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, CString strWhere)
{
	return BulkOneKeySelect( _T("zone_id"), pIOTable, columnCollection, records, strWhere);
}