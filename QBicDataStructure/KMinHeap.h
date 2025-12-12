#pragma once

#include <queue>
#include <vector>
#include "QBicDataStructureDefine.h"
#define KMinHeap  std::priority_queue <KHeapNode, std::deque<KHeapNode>, KHeapNodeCompare >
#define MaxCost   999999999.0
        // #include <limits> numeric_limits<double>::max( )

class KHeapNode
{
public:
    KHeapNode(void);    
    KHeapNode(Integer _nodeID);
    /*KHeapNode(Integer _nodeID, Integer _preNodeID, Integer _preLinkID, Double _val);*/
    KHeapNode(Integer _nodeID, Integer _preNodeID, Integer _preLinkID, Integer _ppreLinkID, Double _val);
    ~KHeapNode(void);

public:
    KHeapNode(const KHeapNode& _node); 
    KHeapNode& operator = (const KHeapNode& _node);

private:
    Integer m_nodeID;
    Integer m_preNodeID;
    Integer m_preLinkID;
    Integer m_ppreLinkID;
    Double  m_cost;

public:
    void    ReplaceNode(const KHeapNode& _node);
    void    SetNodeID(Integer _id);
    Integer GetNodeID() const;

    void    SetPreNodeID(Integer _id);
    Integer GetPreNodeID() const;

    void    SetPreLinkID(Integer _id);
    Integer GetPreLinkID() const;

    void    SetPPreLinkID(Integer _id);
    Integer GetPPreLinkID() const;

    void    SetCostValue(Double _val);
    void    AddCostValue(Double _val);
    Double  GetCostValue() const;
};

class KHeapNodeCompare
{
public:
    bool operator() (const KHeapNode& _node1, const KHeapNode& _node2)
    {
        return _node1.GetCostValue() > _node2.GetCostValue();
    }
};



