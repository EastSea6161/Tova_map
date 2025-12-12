#include "KNodeArrivedMinLink.h"

KMinLinkInfo::KMinLinkInfo(__int64 linkID, double cost) : LinkID(linkID), Cost(cost)
{
}


KMinLinkInfo::~KMinLinkInfo(void)
{
}

KNodeArrivedMinLink::KNodeArrivedMinLink(void) 
{
}

KNodeArrivedMinLink::~KNodeArrivedMinLink(void)
{
    RemoveAll();
}

void KNodeArrivedMinLink::RemoveAll()
{
    std::map<__int64 /*NodeID*/, KMinLinkInfo*>::iterator iter = m_records.begin();
    std::map<__int64 /*NodeID*/, KMinLinkInfo*>::iterator end  = m_records.end();

    while (iter != end)
    {
        KMinLinkInfo* pMinLinkInfo = iter->second;
        delete pMinLinkInfo;

        ++iter;
    }

    m_records.clear();
}


KMinLinkInfo* KNodeArrivedMinLink::GetMinLink(__int64 nodeID) 
{
    std::map<__int64 /*NodeID*/, KMinLinkInfo*>::iterator iter = m_records.find(nodeID);
    std::map<__int64 /*NodeID*/, KMinLinkInfo*>::iterator end  = m_records.end();

    if (iter != end)
    {
        return iter->second;
    }

    return NULL;

}


void KNodeArrivedMinLink::SetMinLink(__int64 nodeID, __int64 linkID, double cost)
{
    std::map<__int64 /*NodeID*/, KMinLinkInfo*>::iterator iter = m_records.find(nodeID);
    std::map<__int64 /*NodeID*/, KMinLinkInfo*>::iterator end  = m_records.end();

    KMinLinkInfo* pMinLinkInfo = NULL;

    if (iter != end)
    {
        pMinLinkInfo         = iter->second;

        if ( pMinLinkInfo->Cost > cost )
        {
            pMinLinkInfo->LinkID = linkID;
            pMinLinkInfo->Cost   = cost;  
        }        
    }
    else
    {
        pMinLinkInfo = new KMinLinkInfo(linkID, cost);
        m_records.insert(std::make_pair(nodeID, pMinLinkInfo));
    }
}

size_t KNodeArrivedMinLink::GetCount() const
{
    return m_records.size();
}