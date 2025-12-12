#pragma once
#include "IncludeQBicDataStructure.h"

class KLink
{
public:
    KLink(void);
    KLink(__int64 linkID);
    KLink(__int64 linkID, __int64 fromNodeID, __int64 toNodeID);
    KLink(__int64 linkID, __int64 fromNodeID, __int64 toNodeID, double cost);
    ~KLink(void);

public:
    KLink(const KLink& _link); 
    KLink& operator = (const KLink& _link);

private:
    __int64 m_linkID;
    __int64 m_fromNodeID;
    __int64 m_toNodeID;
    double  m_cost;

public:
    __int64 GetLinkID();
    void    SetLinkID(__int64 id);
    __int64 GetFromNodeID();
    void    SetFromNodeID(__int64 id);
    __int64 GetToNodeID();
    void    SetToNodeID(__int64 id);
    double  GetCost();
    void    SetCost(double cost);

    void    SetValues(/*__int64 linkID,*/ __int64 fromNodeID, __int64 toNodeID, double cost);

    CAtlString toString() const;
};


