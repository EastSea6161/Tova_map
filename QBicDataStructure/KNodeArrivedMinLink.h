#pragma once

#include <map>

#define MaxCost   999999999.0

class KMinLinkInfo
{
public:
    KMinLinkInfo(__int64 linkID=0, double cost=MaxCost);
    ~KMinLinkInfo(void);
public:
    __int64 LinkID;
    double  Cost;
};


class KNodeArrivedMinLink
{
public:
    KNodeArrivedMinLink (void);
    ~KNodeArrivedMinLink(void);

private:
    std::map<__int64 /*NodeID*/, KMinLinkInfo*> m_records;   

public:
    KMinLinkInfo* GetMinLink(__int64 nodeID) ;
    void          SetMinLink(__int64 nodeID, __int64 linkID, double cost);

    size_t GetCount () const;
    void   RemoveAll();
};

