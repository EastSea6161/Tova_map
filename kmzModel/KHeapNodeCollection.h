#pragma once
#include "KModelDefine.h"
#include "KMinHeap.h"

#define KNodeID Integer
class KHeapNodeCollection
{
public:
    KHeapNodeCollection(void);
    ~KHeapNodeCollection(void);

    KHeapNodeCollection(const KHeapNodeCollection& v);
    KHeapNodeCollection& operator = (const KHeapNodeCollection& v);

private:
    CAtlMap<KNodeID, KHeapNode*> m_nodeMap;

public:
    void       RemoveAll();
    
    KHeapNode* SetNode(Integer nodeID);
    /*KHeapNode* SetMinCostNode(Integer nodeID, Integer preNodeID, Integer preLinkID, Double val);*/
    KHeapNode* SetNode(const KHeapNode& node);

    size_t     GetNodeCount() const;
    KHeapNode* GetNode(Integer nodeID);

    POSITION   GetStartPosition() const;
    CAtlMap<KNodeID, KHeapNode*>::CPair* GetNext(POSITION& pos);
    const CAtlMap<KNodeID, KHeapNode*>::CPair* GetNext(POSITION& pos) const;

    CAtlString toString() const;
};

