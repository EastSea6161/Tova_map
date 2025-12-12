#include "MinHeapLink.h"


KMinHeapLink::KMinHeapLink(void): m_linkID(0), m_toNodeID(0), m_fromNodeID(0), m_preLinkID(0), m_cost(MaxCost)
{

}

KMinHeapLink::KMinHeapLink(Integer _linkID): m_linkID(_linkID), m_toNodeID(0), m_fromNodeID(0), m_preLinkID(0), m_cost(MaxCost)
{

}

KMinHeapLink::KMinHeapLink(Integer _linkID, Integer _fromNodeID, Integer _toNodeID, Integer _preLinkID, Double _val): 
	m_linkID(_linkID), m_fromNodeID(_fromNodeID), m_toNodeID(_toNodeID), m_preLinkID(_preLinkID), m_cost(_val)
{

}


KMinHeapLink::~KMinHeapLink()
{

}


KMinHeapLink::KMinHeapLink(const KMinHeapLink& _link)
{
	m_linkID		= _link.m_linkID;
	m_fromNodeID	= _link.m_fromNodeID;
	m_toNodeID		= _link.m_toNodeID;
	m_preLinkID		= _link.m_preLinkID;
	m_cost			= _link.m_cost;
}

KMinHeapLink& KMinHeapLink::operator=( const KMinHeapLink& _link )
{
	if (&_link == this)
	{
		return *this;
	}

	m_linkID		= _link.m_linkID;
	m_fromNodeID	= _link.m_fromNodeID;
	m_toNodeID		= _link.m_toNodeID;
	m_preLinkID		= _link.m_preLinkID;
	m_cost			= _link.m_cost;
	return *this;
}

void KMinHeapLink::Replacelink( const KMinHeapLink& _link )
{
	m_linkID		= _link.m_linkID;
	m_fromNodeID	= _link.m_fromNodeID;
	m_toNodeID		= _link.m_toNodeID;
	m_preLinkID		= _link.m_preLinkID;
}

void KMinHeapLink::SetLinkID( Integer _id )
{
	m_linkID = _id;
}

Integer KMinHeapLink::GetLinkID() const
{
	return m_linkID;
}

void KMinHeapLink::SetFromNodeID( Integer _id )
{
	m_fromNodeID = _id;
}

Integer KMinHeapLink::SetFromNodeID() const
{
	return m_fromNodeID;
}

void KMinHeapLink::SetToNodekID( Integer _id )
{
	m_toNodeID = _id;
}

Integer KMinHeapLink::GetToNodekID() const
{
	return m_toNodeID;
}

void KMinHeapLink::SetPreLinkID( Integer _id )
{
	m_preLinkID = _id;
}

Integer KMinHeapLink::GetPreLinkID() const
{
	return m_preLinkID;
}

void KMinHeapLink::SetCostValue( Double _val )
{
	 m_cost = _val;
}

void KMinHeapLink::AddCostValue( Double _val )
{
	 m_cost += _val;
}

Double KMinHeapLink::GetCostValue() const
{
	return m_cost;
}
