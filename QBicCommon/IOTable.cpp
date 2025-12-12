#include "StdAfx.h"
#include "TableOwner.h"
#include "IOTable.h"
#include "IOColumn.h"
#include "IOColumns.h"
#include "IOItem.h"
#include "IORecordset.h"
#include "IORow.h"


KIOTable::KIOTable(KIOTableOwner* pOwner) : m_pOwner(pOwner)
{
    m_bViewTable = false;
    m_strViewSQL = _T("");
    TableID      = 0;

	m_pColumns = new KIOColumns;
}

KIOTable::~KIOTable(void)
{
	Clear();
}

void KIOTable::Clear(void)
{
    delete m_pColumns;
}

KEMIOTableType KIOTable::TableType(void)
{
	return m_emIOTableType;
}

void KIOTable::TableType(KEMIOTableType a_emIOTableType)
{
	m_emIOTableType = a_emIOTableType;
}

const LPCTSTR KIOTable::Name(void) const 
{
	return m_strName;
}

void KIOTable::Name(LPCTSTR strName)
{
	m_strName = strName;
}

__int64 KIOTable::GetObjectID()
{
#pragma region History
    // 2016-01-17
    // 최초 설계시 TableID 정보가 없었음.
    // 과거 버전과의 상호 호환성을 유지할 수 있도록 수정
#pragma endregion History

    if (TableID != 0) {
        return TableID;
    }

    __int64 nxObjectID(0);
    try
    {
        nxObjectID = m_pOwner->GetObjectID();	
    }
    catch (KExceptionPtr ex) {
    	throw ex;
    }
    catch (...) {
    	ThrowException(_T("At GetObjectID")); 
    }

    return nxObjectID;
}

const LPCTSTR KIOTable::Caption(void) const
{
	return m_strCaption;
}


void KIOTable::Caption(LPCTSTR strCaption)
{
	m_strCaption = strCaption;
}

int KIOTable::RecordCount(void) 
{
	return m_pOwner->TableRecordCount(this);
}

int KIOTable::RecordCount( CString strWhre )
{
    return m_pOwner->TableRecordCount(this, strWhre);
}

KIORecordset* KIOTable::GetRecordset(KEMIORecordsetType emRecordsetType)
{
	return m_pOwner->GetRecordset(this, emRecordsetType);
}

KIORecordset* KIOTable::GetRecordset(KEMIORecordsetType emRecordsetType, LPCTSTR strFilter, LPCTSTR strOrder)
{
    return m_pOwner->GetRecordset(this, emRecordsetType, strFilter, strOrder);
}

KIORecordset* KIOTable::GetRecordset(KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strFilter, LPCTSTR strOrder)
{
    return m_pOwner->GetRecordset(this, emRecordsetType, pColumns, strFilter, strOrder);
}

KIORecordset* KIOTable::GetRecordset(KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder)
{
    return m_pOwner->GetRecordset(this, emRecordsetType, pColumns, strTables, strFilter, strOrder);
}

KIORecordset* KIOTable::GetRecordset( KEMIORecordsetType emRecordsetType, const KIOColumns* pColumns, LPCTSTR strTables, LPCTSTR strFilter, LPCTSTR strOrder, int nPageOffset, int nRecordCount )
{
    return m_pOwner->GetRecordset(this, emRecordsetType, pColumns, strTables, strFilter, strOrder, nPageOffset, nRecordCount);
}

KIOColumns* KIOTable::Columns(void) const
{
	return m_pColumns;
}

KIOColumn* KIOTable::AddColumn(KIOColumn& column, bool bDefault)
{
    try
    {
        if (_tcslen(column.Name()) <= 0)
            return nullptr;

        // 소유자의 Add Column Business 호출
    	m_pOwner->AddColumnBeforeAction(this, column, bDefault);

        KIOColumn* pColumn = m_pColumns->AddNewColumn();
        *pColumn = column;

        return pColumn;
    }
    catch (KExceptionPtr ex)
    {
    	throw ex;
    }
    catch (...)
    {
    	throw -1;
    }

    /*
	ASSERT(_tcslen(column.Name()) > 0);

    // 중복입력 방지
	if(m_pColumns->GetColumnIndex(column.Name()) != -1)
	{
		return NULL;
	}

	KIOColumn* pColumn = NULL;

	// DB에 column 추가를 수행
	if((InsertColumn(column, bDefault) == true) && (InsertColumnDescription(column) == true))
	{
		pColumn = m_pColumns->AddNewColumn();
		*pColumn = column;
	}

	return pColumn;
    */
}

KIOColumn* KIOTable::UpdateColumn(KIOColumn& column)
{
    KIOColumn* pColumn = nullptr;

    try
    {
        pColumn  = m_pColumns->GetColumn(column.Name());
        if (pColumn == nullptr)
            return nullptr;

        if (&column == pColumn)
            ThrowException(_T("Can not modify yourself"));  

        m_pOwner->UpdateColumnBeforeAction(this, column);
        pColumn  = m_pColumns->GetColumn(column.Name());        

        *pColumn = column;
    }
    catch (KExceptionPtr ex)
    {
    	throw ex;
    }
    catch (...)
    {
    	ThrowException(_T("UpdateColumn"));        
    }	

    return pColumn;
}

void KIOTable::RemoveColumn(LPCTSTR strColumnName)
{
    try
    {
    	int nIndex = m_pColumns->GetColumnIndex(strColumnName);
        if (nIndex < 0) return;

        m_pOwner->RemoveColumnBeforeAction(this, strColumnName);
        m_pColumns->RemoveColumn(nIndex);
    }
    catch (KExceptionPtr ex)
    {
    	throw ex;
    }
    catch (...)
    {
    	throw -1;
    }
}

KDBaseConnectionPtr KIOTable::GetDBaseConnection()
{
    return m_pOwner->GetDBConnection();
}

void KIOTable::BeginTransaction(void)
{
    m_pOwner->BeginTransaction();
}

void KIOTable::Commit(bool bNotify)
{
    m_pOwner->Commit();
    if (bNotify)
	    Notify();
}

void KIOTable::Rollback(void)
{
	m_pOwner->Rollback();
}

void KIOTable::UpdateRow(const KIOColumns* pColumns, const KIORow* pRow, LPCTSTR strFilter)
{
	m_pOwner->UpdateRow(this, pColumns, pRow, strFilter);
}

void KIOTable::InsertRow(const KIOColumns* pColumns, const KIORow* pRow)
{
    m_pOwner->InsertRow(this, pColumns, pRow);
}

void KIOTable::DeleteRow(LPCTSTR strFilter)
{
    m_pOwner->DeleteRow(this, strFilter);
}

KIOTableOwner* KIOTable::GetOwner(void)
{
	return m_pOwner;
}

CString KIOTable::GetUsableColumnName(void)
{
	CString strRet;
	int nIndex = 0;
	int nColumnCount = m_pColumns->ColumnCount();
	TCHAR chPrefix = m_strName.GetAt(0);

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = m_pColumns->GetColumn(i);
		CString strColumnName = pColumn->Name();
		if(strColumnName.GetAt(0) != chPrefix)
		{
			continue;
		}
		strColumnName.Delete(0, 1);
		int nColumnIndex = _ttoi(strColumnName);
		if(nIndex < nColumnIndex)
		{
			nIndex = nColumnIndex;
		}
	}

	nIndex += 1;
	strRet.Format(_T("%c%d"), chPrefix, nIndex);
	return strRet;
}
