#include "StdAfx.h"
#include "IORow.h"
#include "IOItem.h"


KIORow::KIORow(void)
{
}

KIORow::KIORow(const KIORow& row)
{
	int nCount = row.ItemCount();
	for(int i = 0; i < nCount; ++i)
	{
		KIOItem* pDestItem = new KIOItem;
		KIOItem* pSrcItem  = row.GetItem(i);
		
        *pDestItem = *pSrcItem;
        m_vecItem.push_back(pDestItem);
	}
}

KIORow::~KIORow(void)
{
	Clear();
}

KIORow& KIORow::operator=(const KIORow &row)
{
	Clear();

	int nCount = row.ItemCount();
	for(int i = 0; i < nCount; ++i)
	{
		KIOItem* pDestItem = new KIOItem;
		KIOItem* pSrcItem  = row.GetItem(i);

		*pDestItem = *pSrcItem;
		
        m_vecItem.push_back(pDestItem);
	}

	return *this;
}


KIOItem* KIORow::AddNewItem(void)
{
	KIOItem* pItem = new KIOItem;
	m_vecItem.push_back(pItem);
	return pItem;
}


int KIORow::AddNewItem(const KIOItem* pItem)
{
	KIOItem* pNewItem = new KIOItem;
	*pNewItem = *pItem;
	m_vecItem.push_back(pNewItem);
	return (ItemCount() - 1);
}	


void KIORow::Clear(void) 
{
    std::vector<KIOItem*>::iterator iter = m_vecItem.begin();
    std::vector<KIOItem*>::iterator end  = m_vecItem.end();

    while(iter != end)
    {
        KIOItem* pItem = *iter;
        delete   pItem;

        ++iter;
    }

    m_vecItem.clear();
}


int KIORow::ItemCount(void) const
{
	return (int)m_vecItem.size();
}


KIOItem* KIORow::GetItem(int nIndex) const
{
    int nCount = ItemCount();
    if (nIndex < 0 || nIndex >= nCount)
        return nullptr;
        	
	return m_vecItem[nIndex];
}

