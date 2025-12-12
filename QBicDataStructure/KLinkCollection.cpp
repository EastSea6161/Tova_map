#include "KLinkCollection.h"


KLinkCollection::KLinkCollection(void)
{

}

KLinkCollection::KLinkCollection(const KLinkCollection& p )
{
	RemoveAll();

	std::map<__int64, KLink*>::const_iterator iter = p.m_records.begin();
	std::map<__int64, KLink*>::const_iterator end  = p.m_records.end();
	while(iter != end)
	{
		__int64 nxID = iter->first;
		KLink* pLink	= iter->second;

		KLink* pNewLink = new KLink(nxID);
		pNewLink->SetFromNodeID(pLink->GetFromNodeID());
		pNewLink->SetToNodeID(pLink->GetToNodeID());
		pNewLink->SetCost(pLink->GetCost());
		m_records.insert(std::make_pair(nxID, pNewLink));
		++iter;
	}

}


KLinkCollection::~KLinkCollection(void)
{
    RemoveAll();
}


void KLinkCollection::RemoveAll()
{
    std::map<__int64, KLink*>::iterator iter = m_records.begin();
    std::map<__int64, KLink*>::iterator end  = m_records.end();

    while (iter != end)
    {
        KLink* pLink = iter->second;
        delete pLink;

        ++iter;
    }

    m_records.clear();
}

KLink* KLinkCollection::GetExistOrNew(__int64 linkID)
{
    std::map<__int64, KLink*>::iterator iter = m_records.find(linkID);
    std::map<__int64, KLink*>::iterator end  = m_records.end();

    KLink* pLink = NULL;

    if (iter != end)
    {
        pLink = iter->second;
    }
    else
    {
        pLink = new KLink(linkID);
        m_records.insert(std::make_pair(linkID, pLink));
    }

    return pLink;
}

KLink* KLinkCollection::GetLink(__int64 linkID)
{
    std::map<__int64, KLink*>::iterator iter = m_records.find(linkID);
    std::map<__int64, KLink*>::iterator end  = m_records.end();

    KLink* pLink = NULL;
    if (iter != end)
    {
        pLink = iter->second;
    }

    return pLink;
}


CAtlString KLinkCollection::toString() const
{
    CAtlString str;
	str.Format(_T("KLinkCollection : [TotalCount : %d]"), m_records.size() );

	return str;
}

std::map<__int64, KLink*>::iterator KLinkCollection::begin()
{
    return m_records.begin();
}

std::map<__int64, KLink*>::iterator KLinkCollection::end()
{
    return m_records.end();
}

void KLinkCollection::DeleteLink( __int64 linkID )
{
	m_records.erase(linkID);
}

KLinkCollection& KLinkCollection::operator=( const KLinkCollection& p )
{
	if (&p == this)
	{
		return *this;
	}

	RemoveAll();

	std::map<__int64, KLink*>::const_iterator iter = p.m_records.begin();
	std::map<__int64, KLink*>::const_iterator end  = p.m_records.end();
	while(iter != end)
	{
		__int64 nxID = iter->first;
		KLink* pLink	= iter->second;

		KLink* pNewLink = new KLink(nxID);
		pNewLink->SetFromNodeID(pLink->GetFromNodeID());
		pNewLink->SetToNodeID(pLink->GetToNodeID());
		pNewLink->SetCost(pLink->GetCost());
		m_records.insert(std::make_pair(nxID, pNewLink));
		++iter;
	}

	return *this;
}
