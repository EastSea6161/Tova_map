#include "KCostLinkCollection.h"


KCostLinkCollection::KCostLinkCollection(void)
{
}


KCostLinkCollection::~KCostLinkCollection(void)
{
    RemoveAll();
}


KCostLink* KCostLinkCollection::InsertOrReplaceLink(__int64 linkID, __int64 preLinkID, double cost)
{
    std::map<__int64, KCostLink*>::iterator iter = m_records.find(linkID);
    std::map<__int64, KCostLink*>::iterator end  = m_records.end();

    KCostLink* pCostLink = NULL;

    if (iter != end)
    {
        pCostLink = iter->second;
        pCostLink->SetPreLinkID(preLinkID);
        pCostLink->SetCost(cost);
    }
    else
    {
        pCostLink = new KCostLink(linkID, preLinkID, cost);
        m_records.insert(std::make_pair(linkID, pCostLink));
    }

    return pCostLink;
}

KCostLink* KCostLinkCollection::GetLink(__int64 linkID)
{
    std::map<__int64, KCostLink*>::iterator iter = m_records.find(linkID);
    std::map<__int64, KCostLink*>::iterator end  = m_records.end();

    if (iter != end)
    {
        return iter->second;
    }

    return NULL;
}

void KCostLinkCollection::RemoveAll()
{
    std::map<__int64, KCostLink*>::iterator iter = m_records.begin();
    std::map<__int64, KCostLink*>::iterator end  = m_records.end();
    
    while (iter != end)
    {
        KCostLink* pCostLink = iter->second;
        delete     pCostLink;

        ++iter;
    }

    m_records.clear();
}

size_t KCostLinkCollection::GetLinkCount() const
{
    return m_records.size();
}

void KCostLinkCollection::GetLink(std::vector<__int64>& ar_vecLink)
{
	ar_vecLink.clear();
	std::map<__int64, KCostLink*>::iterator iter = m_records.begin();
	std::map<__int64, KCostLink*>::iterator end  = m_records.end();

	while (iter != end)
	{
		ar_vecLink.push_back(iter->first);
		++iter;
	}
}