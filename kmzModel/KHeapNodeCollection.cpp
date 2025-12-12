#include "KHeapNodeCollection.h"


KHeapNodeCollection::KHeapNodeCollection(void)
{
}


KHeapNodeCollection::~KHeapNodeCollection(void)
{
    RemoveAll();
}

KHeapNodeCollection::KHeapNodeCollection(const KHeapNodeCollection& v)
{
    RemoveAll();
    POSITION pos = v.GetStartPosition();
    const CAtlMap<Integer, KHeapNode*>::CPair* pair = NULL;

    while ( NULL != pos )
    {
        pair = v.GetNext(pos);
        m_nodeMap.SetAt(pair->m_key, new KHeapNode(*pair->m_value));
    }
}

KHeapNodeCollection& KHeapNodeCollection::operator = (const KHeapNodeCollection& v)
{
    if (&v == this)
    {
        return *this;
    }

    RemoveAll();
    POSITION pos = v.GetStartPosition();
    const CAtlMap<Integer, KHeapNode*>::CPair* pair = NULL;

    while ( NULL != pos )
    {
        pair = v.GetNext(pos);
        m_nodeMap.SetAt(pair->m_key, new KHeapNode(*pair->m_value));
    }

    return *this;
}

void KHeapNodeCollection::RemoveAll()
{
    POSITION pos = m_nodeMap.GetStartPosition();

    while ( NULL != pos )
    {
        KHeapNode* pNode = m_nodeMap.GetNextValue(pos);
        delete pNode;
    }

    m_nodeMap.RemoveAll();
}

KHeapNode* KHeapNodeCollection::SetNode(Integer nodeID)
{
    CAtlMap<Integer, KHeapNode*>::CPair* pair = m_nodeMap.Lookup(nodeID);

    if ( NULL == pair )
    {
        KHeapNode* pNode = new KHeapNode(nodeID);
        m_nodeMap.SetAt(nodeID, pNode);
        return pNode;
    }

    return pair->m_value;
}

//KHeapNode* KHeapNodeCollection::SetMinCostNode(Integer nodeID, Integer preNodeID, Integer preLinkID, Double val)
//{
//    CAtlMap<Integer, KHeapNode*>::CPair* pair = m_nodeMap.Lookup(nodeID);
//
//    KHeapNode* pNode = NULL;
//
//    if ( NULL == pair )
//    {
//        KHeapNode* pNode = new KHeapNode(nodeID, preNodeID, preLinkID, val);
//        m_nodeMap.SetAt(nodeID, pNode);
//        return pNode;
//    }
//
//    // 존재하는 경우 비교한다.
//    pNode = pair->m_value;
//    if ( pNode->GetCostValue() > val )
//    {        
//        pNode->SetPreNodeID(preNodeID);
//        pNode->SetPreLinkID(preLinkID);
//        pNode->SetCostValue(val);
//    }
//    return pNode;
//}

KHeapNode* KHeapNodeCollection::SetNode(const KHeapNode& node)
{
    Integer nodeID = node.GetNodeID();
    CAtlMap<Integer, KHeapNode*>::CPair* pair = m_nodeMap.Lookup(nodeID);

    KHeapNode* pNode = NULL;

    if ( NULL == pair )
    {
        KHeapNode* pNode = new KHeapNode(node);
        m_nodeMap.SetAt(nodeID, pNode);
        return pNode;
    }

    // 존재하는 경우 
    pNode->SetPreNodeID(node.GetPreNodeID());
    pNode->SetPreLinkID(node.GetPreLinkID());
    pNode->SetCostValue(node.GetCostValue());

    return pNode;
}

size_t KHeapNodeCollection::GetNodeCount() const
{
    return m_nodeMap.GetCount();
}

KHeapNode* KHeapNodeCollection::GetNode(Integer nodeID)
{
    CAtlMap<Integer, KHeapNode*>::CPair* pair = m_nodeMap.Lookup(nodeID);

    if ( NULL != pair )
    {
        return pair->m_value;
    }

    return NULL;
}

POSITION KHeapNodeCollection::GetStartPosition() const
{
    return m_nodeMap.GetStartPosition();
}

CAtlMap<Integer, KHeapNode*>::CPair* KHeapNodeCollection::GetNext(POSITION& pos)
{
    return m_nodeMap.GetNext(pos);
}

const CAtlMap<Integer, KHeapNode*>::CPair* KHeapNodeCollection::GetNext(POSITION& pos) const
{
    return m_nodeMap.GetNext(pos);
}

CAtlString KHeapNodeCollection::toString() const
{
    CAtlString str;
	str.Format(_T("KHeapNodeCollection : [TotalCount : %d]"), m_nodeMap.GetCount() );

	return str;
}