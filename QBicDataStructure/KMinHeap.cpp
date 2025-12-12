#include "KMinHeap.h"

KHeapNode::KHeapNode(void) : m_nodeID(0), m_preNodeID(0), m_cost(MaxCost), m_preLinkID(0), m_ppreLinkID(0)
{
}

KHeapNode::KHeapNode(Integer _nodeID) : m_nodeID(_nodeID), m_preNodeID(0), m_cost(MaxCost), m_preLinkID(0), m_ppreLinkID(0)
{
}

KHeapNode::KHeapNode(Integer _nodeID, Integer _preNodeID, Integer _preLinkID, Integer _ppreLinkID, Double _val) 
    : m_nodeID(_nodeID), m_preNodeID(_preNodeID), m_preLinkID(_preLinkID), m_cost(_val), m_ppreLinkID(_ppreLinkID)
{
}
KHeapNode::~KHeapNode(void)
{
}

KHeapNode::KHeapNode(const KHeapNode& _node)
{
    m_nodeID      = _node.m_nodeID;
    m_preNodeID   = _node.m_preNodeID;
    m_preLinkID   = _node.m_preLinkID;
    m_ppreLinkID  = _node.m_ppreLinkID;
    m_cost        = _node.m_cost;
}

KHeapNode& KHeapNode::operator = (const KHeapNode& _node)
{
    if (&_node == this)
    {
        return *this;
    }

    m_nodeID      = _node.m_nodeID;
    m_preNodeID   = _node.m_preNodeID;
    m_preLinkID   = _node.m_preLinkID;
    m_ppreLinkID  = _node.m_ppreLinkID;
    m_cost        = _node.m_cost;
    return *this;
}

void KHeapNode::ReplaceNode(const KHeapNode& _node)
{
    m_nodeID      = _node.m_nodeID;
    m_preNodeID   = _node.m_preNodeID;
    m_preLinkID   = _node.m_preLinkID;
    m_ppreLinkID  = _node.m_ppreLinkID;
    m_cost        = _node.m_cost;
}

void KHeapNode::SetNodeID(Integer _id)
{
    m_nodeID = _id;
}

Integer KHeapNode::GetNodeID() const
{
    return m_nodeID;
}

void KHeapNode::SetPreNodeID(Integer _id)
{
    m_preNodeID = _id;
}

Integer KHeapNode::GetPreNodeID() const
{
    return m_preNodeID;
}

void KHeapNode::SetPreLinkID(Integer _id)
{
    m_preLinkID = _id;
}

Integer KHeapNode::GetPreLinkID() const
{
    return m_preLinkID;
}

void KHeapNode::SetPPreLinkID(Integer _id)
{
    m_ppreLinkID = _id;
}

Integer KHeapNode::GetPPreLinkID() const
{
    return m_ppreLinkID;
}

void KHeapNode::SetCostValue(Double _val)
{
    m_cost = _val;
}

void KHeapNode::AddCostValue(Double _val)
{
    m_cost += _val;
}

Double KHeapNode::GetCostValue() const
{
    return m_cost;
}
