#include "KCostLink.h"


KCostLink::KCostLink(void) : m_linkID(0), m_preLinkID(0), m_cost(0.0)
{
}


KCostLink::~KCostLink(void)
{
}


KCostLink::KCostLink(__int64 linkID) : m_linkID(linkID), m_preLinkID(0), m_cost(0.0)
{
}


KCostLink::KCostLink(__int64 linkID, __int64 preLinkID, double cost) : m_linkID(linkID), m_preLinkID(preLinkID), m_cost(cost)
{
}

KCostLink::KCostLink(const KCostLink& _link)
{
    m_linkID     = _link.m_linkID;
    m_preLinkID  = _link.m_preLinkID;
    m_cost       = _link.m_cost;
}

KCostLink& KCostLink::operator = (const KCostLink& _link)
{
    if (&_link == this)
    {
        return *this;
    }

    m_linkID     = _link.m_linkID;
    m_preLinkID  = _link.m_preLinkID;
    m_cost       = _link.m_cost;

    return *this;
}


__int64 KCostLink::GetLinkID()
{
    return m_linkID;
}

void KCostLink::SetLinkID(__int64 id)
{
    m_linkID = id;
}

__int64 KCostLink::GetPreLinkID()
{
    return m_preLinkID;
}

void KCostLink::SetPreLinkID(__int64 id)
{
    m_preLinkID = id;
}

double KCostLink::GetCost()
{
    return m_cost;
}

void KCostLink::SetCost(double cost)
{
    m_cost = cost;
}

