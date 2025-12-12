#include "StdAfx.h"
#include "Target.h"
#include "TableOwnerTarget.h"
#include "IOColumnToString.h"
#include "IORelationColumn.h"
#include "IOAssignRelationColumn.h"
#include "IORecordsetFactory.h"

KTableOwnerTarget::KTableOwnerTarget(KTarget* pTarget)
	:	KIOTableOwner(KEMTableOwnerTypeTarget),
		m_pTarget(pTarget)
{
}

KTableOwnerTarget::~KTableOwnerTarget(void)
{
}


KCodeManager* KTableOwnerTarget::GetCodeManager(void)
{
	if(NULL != m_pTarget)
	{
		return m_pTarget->CodeManager();
	}
	return NULL;
}


KIOTables* KTableOwnerTarget::Tables(void)
{
	if(NULL != m_pTarget)
	{
		return m_pTarget->Tables();
	}
	return NULL;
}

KTarget* KTableOwnerTarget::GetTarget(void)
{
	return m_pTarget;
}

Integer KTableOwnerTarget::GetObjectID()
{
    Integer nxReturn(0);
    try
    {
    	nxReturn = m_pTarget->GetObjectID();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }

    return nxReturn;
}

KDBaseConPtr KTableOwnerTarget::GetDBConnection(void)
{
    return m_pTarget->GetDBaseConnection();
}

void KTableOwnerTarget::BeginTransaction()
{
    KDBaseConPtr spDBConnection = GetDBConnection();
    spDBConnection->BeginTransaction();
}

void KTableOwnerTarget::Commit()
{
    KDBaseConPtr spDBConnection = GetDBConnection();
    spDBConnection->Commit();
}

void KTableOwnerTarget::Rollback()
{
    KDBaseConPtr spDBConnection = GetDBConnection();
    spDBConnection->RollBack();
}


void KTableOwnerTarget::ExecuteUpdate( LPCTSTR strSQL )
{
    try
    {
        KDBaseConPtr spDBaseConnection = GetDBConnection();
        spDBaseConnection->ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        throw -1;
    }
}


CString KTableOwnerTarget::GetIndexQuery( LPCTSTR strSQL )
{
	try
	{
		CString strIndexQuery = _T("");

		KDBaseConPtr spDBaseConnection = GetDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		if (spResultSet->Next())
			strIndexQuery = spResultSet->GetValueString(0);

		return strIndexQuery;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw -1;
	}
}


CString KTableOwnerTarget::MakeFieldsString(const KIOColumns* pColumns)
{
	CString strColumns = _T("");

	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(i > 0)
		{
			strColumns += _T(", ");
		}
		strColumns += pColumn->Name();
	}

	return strColumns;
}


CString KTableOwnerTarget::MakeValuesString(const KIOColumns* pColumns, const KIORow* pRow)
{
	CString strValues = _T("");

	int nItemCount = pRow->ItemCount();
	for(int i = 0; i < nItemCount; ++i)
	{
		KIOItem* pItem = pRow->GetItem(i);
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if(i > 0)
		{
			strValues += _T(", ");
		}

        strValues += _T("'");
        strValues += pItem->GetValueAsString();
        strValues += _T("'");
	}

	return strValues;
}


CString KTableOwnerTarget::MakeSetString(const KIOColumns* pColumns, const KIORow* pRow)
{
	CString strSets = _T("");

	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		KIOItem* pItem = pRow->GetItem(i);

		if(i > 0)
		{
			strSets += _T(", ");
		}

		strSets += pColumn->Name();
		strSets += _T(" = ");
        strSets += _T("'");
        strSets += pItem->GetValueAsString();
        strSets += _T("'");
	}

	return strSets;
}


void KTableOwnerTarget::AddColumnBeforeAction( KIOTable* a_pTable, KIOColumn& column, bool bDefault /*= false*/ )
{
    try
    {
        AlterTableAddColumn(a_pTable, column, bDefault);
        InsertColumnDescription(a_pTable, column);	
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
        throw -1;
    }
}

void KTableOwnerTarget::AlterTableAddColumn( KIOTable* a_pTable, KIOColumn& column, bool bDefault )
{
    try
    {
        CString strTableName = a_pTable->Name();

        CString strSQL = _T("ALTER TABLE ");
        strSQL += strTableName;
        strSQL += _T(" ADD COLUMN \"");
        strSQL += column.Name();
        strSQL += _T("\" ");

        switch(column.DataType())
        {
        case KEMIODataTypeInteger : 
            strSQL += TSQLiteDataTypeInteger; 
            if (bDefault) strSQL += _T(" DEFAULT 0 ");
            break;
        case KEMIODataTypeDouble : 
            strSQL += TSQLiteDataTypeDouble; 
            if (bDefault) strSQL += _T(" DEFAULT 0 ");
            break;

        case KEMIODataTypeString : strSQL += TSQLiteDataTypeVarchar2; break;
        default : throw;
        }

        ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	throw -1;
    }    
}

void KTableOwnerTarget::InsertColumnDescription( KIOTable* a_pTable, KIOColumn &column )
{
    try
    {
        CString strTableName = a_pTable->Name();
        KIOColumnToString cs(strTableName, column, KEMIOColumnToStringInsert);

        CString strSQL = _T("INSERT OR IGNORE INTO column_description ( ");
        strSQL += cs.GetFieldsString();
        strSQL += _T(" ) VALUES (");
        strSQL += cs.GetValuesString();
        strSQL += _T(" )");

        ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	throw -1;
    }    
}

void KTableOwnerTarget::RemoveColumnBeforeAction( KIOTable* a_pTable, LPCTSTR strColumnName )
{
    try
    {
        DeleteColumn             (a_pTable, strColumnName);
    	DeleteColumnDescription  (a_pTable, strColumnName);
        DeleteRelationTableColumn(a_pTable, strColumnName);
        DeleteAssignRelationTableColumn(a_pTable, strColumnName);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	throw -1;
    }
}

void KTableOwnerTarget::DeleteColumn( KIOTable* a_pTable, LPCTSTR a_strColumnName )
{       
    try
    {
		CString strSQL        = _T("");
        CString strColumns    = _T("");//컬럼의 명칭 정보를 가짐, Select에 이용
        CString strColumnInfo = _T("");//컬럼의 명칭과 데이터 타입 정보를 가짐, Create Table에 사용됨
        CString strPKColumns  = _T("");
		CString strIndexQuery = _T("");//테이블의 Index가 존재할시 sqlite_master에서 가져온 값
		CString strQueryGetIndex;	   //sqlite_master에서 테이블의 index를 조회하는 쿼리

        CString strTableName  = a_pTable->Name();
        KIOColumns* pColumns  = a_pTable->Columns();

		strQueryGetIndex.Format(
			_T("SELECT sql FROM sqlite_master WHERE LOWER(tbl_name) = LOWER('%s') AND LOWER(type) = LOWER('index')"), strTableName);
		strIndexQuery = GetIndexQuery(strQueryGetIndex);

		int nNewColumnCount = 0; // 남게 될 field 문자열 생성
		int nPKColumnCount  = 0;

		std::vector<TColumns> vecColumns;
		GetTableColumns(strTableName, vecColumns);
		
        int nColumn = vecColumns.size();

		for (int i = 0; i < nColumn; ++i)
		{
			TColumns oColumn = vecColumns[i];

			if(_tcscmp(oColumn.strColumnName, a_strColumnName) == 0)
				continue;

			strColumnInfo += oColumn.strColumnName;
			strColumns    += oColumn.strColumnName;

			if (oColumn.emDataType == KEMIODataTypeInteger)
			{
				strColumnInfo += _T(" INTEGER");
			}
			else if(oColumn.emDataType == KEMIODataTypeDouble)
			{
				strColumnInfo += _T(" DOUBLE");
			}
			else
			{
				strColumnInfo += _T(" VARCHAR");
			}

			strColumnInfo += _T(", ");
			strColumns    += _T(", ");
			++nNewColumnCount;

			if (oColumn.bIsPKColumn)
			{
				strPKColumns += oColumn.strColumnName;
				strPKColumns += _T(", ");
				++nPKColumnCount;
			}

		}
        
//         int nColumnCount = pColumns->ColumnCount();
//         for(int i = 0; i < nColumnCount; ++i)
//         {
//             KIOColumn* pColumn = pColumns->GetColumn(i);
//             if(_tcscmp(pColumn->Name(), a_strColumnName) != 0)
//             {
//                 strColumnInfo += pColumn->Name();
//                 strColumns    += pColumn->Name();
//                 if (pColumn->DataType() == KEMIODataTypeDouble)
//                 {
//                     strColumnInfo += _T(" DOUBLE");
//                 }
//                 else if (pColumn->DataType() == KEMIODataTypeInteger)
//                 {
//                     strColumnInfo += _T(" INTEGER");
//                 }
//                 else
//                 {
//                     strColumnInfo += _T(" VARCHAR");
//                 }
//                 strColumnInfo += _T(", ");
//                 strColumns    += _T(", ");
//                 ++nNewColumnCount;
// 
//                 if (pColumn->PrimaryKey())
//                 {
//                     strPKColumns += pColumn->Name();
//                     strPKColumns += _T(", ");
//                     ++nPKColumnCount;
//                 }
//             }
//         }

        if(nNewColumnCount == nColumn)
            return;

        strColumnInfo.Truncate(strColumnInfo.GetLength() - 2);
        strColumns.Truncate(strColumns.GetLength() -2);

        //Table에 pkColumn 있을 시 pk추가 로직.
        if ( nPKColumnCount != 0 )
        {
            strPKColumns.Truncate(strPKColumns.GetLength() - 2);
            strPKColumns = _T(", PRIMARY KEY ( ") + strPKColumns + _T(")");;
        }

        // temp table 삭제
        strSQL.Format(_T("DROP TABLE temp_%s"), strTableName);
        try
        {
            ExecuteUpdate(strSQL); //이곳은 에러가 발생할 수 있음. 지우지 않기 때문에...	
        }
        catch (...)
        {
        }

        strSQL.Format(_T("CREATE TABLE temp_%s(%s %s)"), strTableName, strColumnInfo, strPKColumns);
        ExecuteUpdate(strSQL);

        // temp table에 데이터 삽입
        strSQL.Format(_T("INSERT INTO temp_%s SELECT %s FROM %s"), strTableName, strColumns, strTableName);
        ExecuteUpdate(strSQL);

        // table을 삭제
        strSQL.Format(_T("DROP TABLE %s"), strTableName);
        ExecuteUpdate(strSQL);

        // temp table의 이름을 현재 table의 이름으로 변경
        strSQL.Format(_T("ALTER TABLE temp_%s RENAME TO %s"), strTableName, strTableName);
        ExecuteUpdate(strSQL);

		//기존 테이블에 Index가 존재하면, 기존의 Index를 다시 부여함	14/01/21
		if (!strIndexQuery.IsEmpty())
			ExecuteUpdate(strIndexQuery);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        ThrowException(_T("DeleteColumn"));
    }
}

void KTableOwnerTarget::DeleteColumnDescription( KIOTable* a_pTable, LPCTSTR strColumnName )
{
    CString strTableName = a_pTable->Name();
    CString strSQL = _T("");
    strSQL.Format(_T("DELETE FROM column_description WHERE table_name = '%s' AND column_name = '%s'"), strTableName, strColumnName);

    ExecuteUpdate(strSQL);
}

void KTableOwnerTarget::DeleteRelationTableColumn( KIOTable* a_pTable, LPCTSTR strColumnName )
{
    CString strTableName = a_pTable->Name();
    KIORelationColumn oIORelationColumn(strTableName);

    std::vector<TRelationColumnInfo> &vecRelationColumnInfo = oIORelationColumn.GetRelationInfo();

    for( size_t i= 0; i< vecRelationColumnInfo.size(); i++ )
    {
        TRelationColumnInfo &oTRelationColumnInfo = vecRelationColumnInfo[i];

        CString strSQL = _T("");
        strSQL.Format(_T(" DELETE FROM %s WHERE %s = '%s' "), oTRelationColumnInfo.strTableName, oTRelationColumnInfo.strColumnName, strColumnName);

        ExecuteUpdate(strSQL);
    }
}

void KTableOwnerTarget::DeleteAssignRelationTableColumn( KIOTable* a_pTable, LPCTSTR strColumnName )
{
    if (KmzSystem::IsFreightSystem() == true)
    {
        return;
    }

    CString strTableName = a_pTable->Name();

    KIOAssignRelationColumn oIOAssignRelationColumn(GetDBConnection(), strTableName);

    if (oIOAssignRelationColumn.IsAssignRelationTable() == false) 
    {
        return;
    }

    oIOAssignRelationColumn.DeleteAssignColumn(strColumnName);
}

void KTableOwnerTarget::UpdateColumnBeforeAction( KIOTable* a_pTable, KIOColumn& column )
{
    try
    {
    	UpdateColumnDescription(a_pTable, column);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	throw -1;
    }
}

void KTableOwnerTarget::UpdateColumnDescription( KIOTable* a_pTable, KIOColumn& column )
{
    CString strTableName = a_pTable->Name();
    KIOColumnToString cs(strTableName, column, KEMIOColumnToStringUpdate);

    CString strSQL = _T("UPDATE column_description SET ");
    strSQL += cs.GetFieldsAndValuesString();
    strSQL += _T("WHERE table_name = '");
    strSQL += strTableName;
    strSQL += _T("' AND column_name = '");
    strSQL += column.Name();
    strSQL += _T("'");

    ExecuteUpdate(strSQL);
}

int KTableOwnerTarget::TableRecordCount( KIOTable* a_pTable ) 
{    
    int nRecordCount(0);

    try
    {
        CString strTableName = a_pTable->Name();

        CString strSQL(_T(""));
        strSQL.Format(_T("SELECT count(*) FROM %s"), strTableName);

        KDBaseConPtr spDBConnection = GetDBConnection();
        KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

        if (spResultSet->Next())
        {
            nRecordCount = spResultSet->GetValueInt(0);
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	ThrowException(_T("At ")); 
    }    

    return nRecordCount;
}

int KTableOwnerTarget::TableRecordCount( KIOTable* a_pTable, CString strWhere )
{
	int nRecordCount(0);

	try
	{
		CString strTableName = a_pTable->Name();
		CString strSQL(_T(""));

		strWhere = strWhere.Trim();
		if (strWhere.GetLength() > 2) {
			strSQL.Format(_T("Select Count(*) From %s Where %s "), strTableName, strWhere);
		}else {
			strSQL.Format(_T("Select Count(*) From %s "), strTableName);
		}

		KDBaseConPtr spDBConnection = GetDBConnection();
		KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

		if (spResultSet->Next())
		{
			nRecordCount = spResultSet->GetValueInt(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("At ")); 
	}    

	return nRecordCount;
}

void KTableOwnerTarget::UpdateRow( KIOTable* a_pTable, const KIOColumns* pColumns, const KIORow* pRow, LPCTSTR strFilter )
{
    try
    {
        CString strTableName = a_pTable->Name();

        int nColumnCount = pColumns->ColumnCount();
        int nItemCount   = pRow->ItemCount();

        if (nColumnCount != nItemCount)
            ThrowException(_T("Column Count != Item Count"));

        CString strSQL = _T("");

        CString strSet = MakeSetString(pColumns, pRow);
        if(strSet.IsEmpty() == true)
        {
            return;
        }

        if(_tcslen(strFilter) > 0)
        {
            strSQL.Format(_T("UPDATE %s SET %s WHERE %s"), strTableName, strSet, strFilter);
        }
        else
        {
            strSQL.Format(_T("UPDATE %s SET %s"), strTableName, strSet);
        }

        return ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	ThrowException(_T("At UpdateRow")); 
    }    
}

void KTableOwnerTarget::InsertRow( KIOTable* a_pTable, const KIOColumns* pColumns, const KIORow* pRow )
{
    try
    {
        CString strTableName = a_pTable->Name();

        int nColumnCount = pColumns->ColumnCount();
        int nItemCount   = pRow->ItemCount();

        if (nColumnCount != nItemCount)
            ThrowException(_T("Column Count != Item Count"));

        CString strSQL = _T("");

        CString strFields= MakeFieldsString(pColumns);
        if(strFields.IsEmpty() == true)
        {
            throw 1;
        }

        CString strValues = MakeValuesString(pColumns, pRow);
        if(strValues.IsEmpty() == true)
        {
            throw 1;
        }

        strSQL.Format(_T("Insert Into %s ( %s ) Values(%s)"), strTableName, strFields, strValues);

        return ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        ThrowException(_T("At InsertRow")); 
    }    
}


void KTableOwnerTarget::DeleteRow( KIOTable* a_pTable, LPCTSTR strFilter )
{
    try
    {
        CString strTableName = a_pTable->Name();
        CString strSQL = _T("");

        strSQL.Format(_T(" Delete From %s "), strTableName);
        if (_tcslen(strFilter) > 0)
        {
            strSQL.AppendFormat(_T(" Where %s "), strFilter);
        }

        return ExecuteUpdate(strSQL);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        ThrowException(_T("At DeleteRow")); 
    } 
}


KIORecordset* KTableOwnerTarget::GetRecordset( KIOTable* a_pTable, KEMIORecordsetType emRecordsetType )
{
    KIOColumns* pColumns = a_pTable->Columns();
    KIORecordset* pRecordset = KIORecordsetFactory::CreateRecordset(a_pTable, emRecordsetType);
    if(NULL != pRecordset)
    {
        pRecordset->ReadRecord(GetDBConnection(), pColumns, _T(""), _T(""), _T(""));
    }

    return pRecordset;
}

KIORecordset* KTableOwnerTarget::GetRecordset( KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, LPCTSTR strFilter, LPCTSTR strOrder )
{
	KIOColumns* pColumns = a_pTable->Columns();
	KIORecordset* pRecordset = KIORecordsetFactory::CreateRecordset(a_pTable, emRecordsetType);
	if(NULL != pRecordset)
	{
		pRecordset->ReadRecord(GetDBConnection(), pColumns, _T(""), strFilter, strOrder);
	}

	return pRecordset;
}

KIORecordset* KTableOwnerTarget::GetRecordset( KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strFilter, LPCTSTR strOrder )
{
    KIORecordset* pRecordset = KIORecordsetFactory::CreateRecordset(a_pTable, emRecordsetType);
    if(NULL != pRecordset)
    {
        pRecordset->ReadRecord(GetDBConnection(), pColumns, _T(""), strFilter, strOrder);
    }

    return pRecordset;
}

KIORecordset* KTableOwnerTarget::GetRecordset( KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder )
{
    KIORecordset* pRecordset = KIORecordsetFactory::CreateRecordset(a_pTable, emRecordsetType);
    if(NULL != pRecordset)
    {
        pRecordset->ReadRecord(GetDBConnection(), pColumns, strTables, strFilter, strOrder);
    }

    return pRecordset;
}

KIORecordset* KTableOwnerTarget::GetRecordset( KIOTable* a_pTable, KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount )
{
	// Paging은 오직 Static 만 적용
	if (emRecordsetType != KEMIORecordsetTypeStatic) {
		return nullptr;
	}

	KIOStaticRecordset* pRecordset = (KIOStaticRecordset*)KIORecordsetFactory::CreateRecordset(a_pTable, emRecordsetType);
	if (NULL != pRecordset) {
		pRecordset->ReadRecord(GetDBConnection(), pColumns, strTables, strFilter, strOrder, nPageOffset, nRecordCount);
	}

	return pRecordset;
}

void KTableOwnerTarget::GetTableColumns( CString a_strTableName, std::vector<TColumns>& a_vecColumns )
{
	//KIOColumns의 경우 column_description 테이블에서 disable 된 컬럼이 있는 테이블의 컬럼을 삭제시
	//disable된 컬럼또한 DB구조상에서 삭제되는 버그가 발생하여 Column_description을 직접조회하여 테이블을 생성하도록 로직 변경

	a_vecColumns.clear();
	CString strQuery;

	try
	{
		CString strPK = _T("Y");

		strQuery.Format(_T("SELECT column_name, data_type, pk_yn FROM column_description WHERE lower(table_name) = lower('%s') ORDER BY display_order")
			, a_strTableName);

		KDBaseConPtr spDBaseConnection = GetDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			TColumns oColumns = {};
			oColumns.strColumnName = spResultSet->GetValueString(0);
			oColumns.emDataType    = (KEMIODataType)spResultSet->GetValueInt(1);

			if (strPK.CompareNoCase(spResultSet->GetValueString(2)) == 0)
			{
				oColumns.bIsPKColumn = true;
			}
			else
			{
				oColumns.bIsPKColumn = false;
			}
			
			a_vecColumns.push_back(oColumns);
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

