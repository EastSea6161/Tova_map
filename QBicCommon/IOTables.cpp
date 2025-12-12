#include "StdAfx.h"
#include "IOTables.h"
#include "IOTable.h"
#include "TableOwner.h"

KIOTables::KIOTables(KIOTableOwner* pOwner)
	: m_pOwner(pOwner)
{
}

KIOTables::~KIOTables(void)
{
    Clear();
}

KIOTable* KIOTables::CreateTable(LPCTSTR strName)
{	
	KIOTable* pIOTable  = FindTable(strName);
	if(NULL != pIOTable) {
		return pIOTable;
	}

	KIOTable* pNewTable = new KIOTable(m_pOwner);
	pNewTable->Name(strName);
	m_lstTable.push_back(pNewTable);

	return pNewTable;
}

void KIOTables::DeleteTable(LPCTSTR strName)
{
	CString strTableName(strName);

	std::list<KIOTable*>::iterator itTable, itTableEnd = m_lstTable.end();
	for(itTable = m_lstTable.begin(); itTable != itTableEnd; ++itTable) {
		KIOTable* pTable = *itTable;
		if(strTableName.CompareNoCase(pTable->Name()) == 0) {
			delete pTable;
			m_lstTable.erase(itTable);
			return;
		}
	}
}

KIOTable* KIOTables::FindTable(LPCTSTR strName)
{	
	CString strTableName(strName);

	std::list<KIOTable*>::iterator iter, end = m_lstTable.end();
	for(iter = m_lstTable.begin(); iter != end; ++iter) {
		KIOTable* pIOTable = *iter;
		if(strTableName.CompareNoCase(pIOTable->Name()) == 0) {
			return pIOTable;
		}
	}
	
	return nullptr;
}

KIOTable* KIOTables::FindTable( __int64 nxID )
{
#pragma region History
    // Create by nombara :2015-01-07
#pragma endregion History

    for (auto iter = m_lstTable.begin(); iter != m_lstTable.end(); ++iter) {
        KIOTable* pIOTable = *iter;
        if (pIOTable->GetObjectID() == nxID)
            return pIOTable;
    }

    return nullptr;
}

void KIOTables::Clear(void)
{
	std::list<KIOTable*>::iterator iter, end = m_lstTable.end();
	for(iter = m_lstTable.begin(); iter != end; ++iter) {
		KIOTable* pIOTable = *iter;
        delete pIOTable;
        pIOTable = nullptr;
	}

	m_lstTable.clear();
}

int KIOTables::GetCount(void)
{
	return (int)m_lstTable.size();
}

KIOTable* KIOTables::GetTableAt(int nIndex)
{	
	if((0 > nIndex) || ((int)m_lstTable.size() <= nIndex)) {
		return nullptr;	
	}

	std::list<KIOTable*>::iterator iter = m_lstTable.begin();
	for(int i=0; i<nIndex; ++i) {
		++iter;
	}
	
	return *iter;
}