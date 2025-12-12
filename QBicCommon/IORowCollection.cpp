#include "StdAfx.h"
#include "IORowCollection.h"
#include "IOItem.h"
#include "IORow.h"

KIORowCollection::KIORowCollection(void)
{
}

KIORowCollection::~KIORowCollection(void)
{
	Clear();
}

size_t KIORowCollection::RowCount(void) const
{
	return m_vecRow.size();
}

KIORow* KIORowCollection::AddNewRow(void)
{
	KIORow* pRow = new KIORow;
	        m_vecRow.push_back(pRow);

	return pRow;
}

void KIORowCollection::Clear(void)
{
	size_t nxCount = m_vecRow.size();
	for(size_t i=0; i<nxCount; i++)
	{
		KIORow* pIORow = m_vecRow[i];
		        delete pIORow;
	}

	m_vecRow.clear();
}

KIORow* KIORowCollection::GetRow(size_t nIndex)
{
    if (nIndex >= m_vecRow.size())
        return nullptr;

	return m_vecRow[nIndex];
}