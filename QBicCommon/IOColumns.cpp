#include "StdAfx.h"
#include "IOColumns.h"
#include "IOColumn.h"

KIOColumns::KIOColumns(void)
{
}

KIOColumns::KIOColumns(const KIOColumns& columns)
{
	int nColumnCount = columns.ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = new KIOColumn;
		*pColumn = *(columns.GetColumn(i));
		m_vecColumn.push_back(pColumn);
	}
}

KIOColumns::~KIOColumns(void)
{
	Clear();
}

void KIOColumns::Clear(void)
{
    std::vector<KIOColumn*>::iterator iter = m_vecColumn.begin();
    std::vector<KIOColumn*>::iterator end  = m_vecColumn.end();
    while(iter != end)
    {
        KIOColumn* pColumn = *iter;
        delete pColumn;

        ++iter;
    }
    m_vecColumn.clear();
}

KIOColumns& KIOColumns::operator=(const KIOColumns& columns)
{
	Clear();
	int nColumnCount = columns.ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = new KIOColumn;
		*pColumn = *(columns.GetColumn(i));
		m_vecColumn.push_back(pColumn);
	}
	return *this;
}

int KIOColumns::ColumnCount(void) const
{
	return (int)m_vecColumn.size();
}

KIOColumn* KIOColumns::AddNewColumn(void)
{
	KIOColumn* pColumn = new KIOColumn;
	m_vecColumn.push_back(pColumn);
	return pColumn;
}

int KIOColumns::AddNewColumn(const KIOColumn* pColumn)
{
	KIOColumn* pNewColumn = new KIOColumn;
	*pNewColumn = *pColumn;
	m_vecColumn.push_back(pNewColumn);
	return (int)(m_vecColumn.size() - 1);
}

void KIOColumns::RemoveColumn(int nIndex)
{
    std::vector<KIOColumn*>::iterator iter = m_vecColumn.begin();
    std::vector<KIOColumn*>::iterator end  = m_vecColumn.end();

    int nCheck(0);
    while(iter != end)
    {
        if (nCheck == nIndex)
        {
            KIOColumn* pColumn = *iter;
            delete pColumn;

            m_vecColumn.erase(iter);
            break;
        }       

        ++nCheck;
        ++iter;
    }
}

KIOColumn* KIOColumns::GetColumn(int nIndex) const
{
    if (nIndex < 0 || nIndex >= ColumnCount())
        return nullptr;

	return m_vecColumn[nIndex];
}

KIOColumn* KIOColumns::GetColumn(LPCTSTR strColumnName) const
{
    std::vector<KIOColumn*>::const_iterator iter = m_vecColumn.begin();
    std::vector<KIOColumn*>::const_iterator end  = m_vecColumn.end();

    while(iter != end)
    {
        KIOColumn* pColumn = *iter;
        if(_tcsicmp(pColumn->Name(), strColumnName) == 0)
        {
            return pColumn;
        }

        ++iter;
    }
	
	return NULL;
}

int KIOColumns::GetColumnIndex(KIOColumn* pColumn) const
{
	if(NULL != pColumn)
	{
		return GetColumnIndex(pColumn->Name());
	}
	return -1;
}

int KIOColumns::GetColumnIndex(LPCTSTR strColumnName) const
{
    std::vector<KIOColumn*>::const_iterator iter = m_vecColumn.begin();
    std::vector<KIOColumn*>::const_iterator end  = m_vecColumn.end();

    int nIndex(0);
    while(iter != end)
    {
        KIOColumn* pColumn = *iter;

        if(_tcsicmp(pColumn->Name(), strColumnName) == 0)
        {
            return nIndex;
        }
        ++nIndex;
        ++iter;
    }

	return -1;
}

int KIOColumns::GetPKColumnCount(void) const
{
	std::vector<KIOColumn*>::const_iterator iter = m_vecColumn.begin();
	std::vector<KIOColumn*>::const_iterator end  = m_vecColumn.end();

	int nPKCount(0);
	while(iter != end)
	{
		KIOColumn* pColumn = *iter;

		if (pColumn->PrimaryKey())
		{
			nPKCount++;
		}

		++iter;
	}

	return nPKCount;
}
