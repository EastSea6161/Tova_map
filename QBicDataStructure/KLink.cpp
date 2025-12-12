#include "KLink.h"


KLink::KLink(void) : m_linkID(0), m_fromNodeID(0), m_toNodeID(0), m_cost(0.0)
{
}

KLink::KLink(__int64 linkID)
    :m_linkID(linkID), m_fromNodeID(0), m_toNodeID(0), m_cost(0.0)
{
}

KLink::KLink(__int64 linkID, __int64 fromNodeID, __int64 toNodeID)
    :m_linkID(linkID), m_fromNodeID(fromNodeID), m_toNodeID(toNodeID), m_cost(0.0)
{
}

KLink::KLink(__int64 linkID, __int64 fromNodeID, __int64 toNodeID, double cost)
    :m_linkID(linkID), m_fromNodeID(fromNodeID), m_toNodeID(toNodeID), m_cost(cost)
{
}

KLink::~KLink(void)
{
}

KLink::KLink(const KLink& _link)
{
    m_linkID     = _link.m_linkID;
    m_fromNodeID = _link.m_fromNodeID;
    m_toNodeID   = _link.m_toNodeID;
    m_cost       = _link.m_cost;
}

KLink& KLink::operator = (const KLink& _link)
{
    if (&_link == this)
    {
        return *this;
    }

    m_linkID     = _link.m_linkID;
    m_fromNodeID = _link.m_fromNodeID;
    m_toNodeID   = _link.m_toNodeID;
    m_cost       = _link.m_cost;

    return *this;
}

__int64 KLink::GetLinkID()
{
    return m_linkID;
}

void KLink::SetLinkID(__int64 id)
{
    m_linkID = id;
}

__int64 KLink::GetFromNodeID()
{
    return m_fromNodeID;
}

void KLink::SetFromNodeID(__int64 id)
{
    m_fromNodeID = id;
}

__int64 KLink::GetToNodeID()
{
    return m_toNodeID;
}

void KLink::SetToNodeID(__int64 id)
{
    m_toNodeID = id;
}

double KLink::GetCost()
{
    return m_cost;
}

void KLink::SetCost(double cost)
{
    m_cost = cost;
}

void KLink::SetValues(/*__int64 linkID,*/ __int64 fromNodeID, __int64 toNodeID, double cost)
{
    /*m_linkID     = linkID;*/
    m_fromNodeID = fromNodeID;
    m_toNodeID   = toNodeID;
    m_cost       = cost;
}

CAtlString KLink::toString() const
{
    /*__int64 m_linkID;
    __int64 m_fromNodeID;
    __int64 m_toNodeID;
    double  m_cost;*/

    CAtlString str;
	str.Format(_T("KLink : [LinkID : %9I64d], [FromNodeID : %9I64d], [ToNodeID : %9I64d], [Cost : %9.2f]"), m_linkID, m_fromNodeID, m_toNodeID, m_cost );

	return str;
}