#pragma once

class KCostLink
{
public:
    KCostLink(void);
    ~KCostLink(void);

    KCostLink(__int64 linkID);
    KCostLink(__int64 linkID, __int64 preLinkID, double cost);

public:
    KCostLink(const KCostLink& _link); 
    KCostLink& operator = (const KCostLink& _link);

private:
    __int64 m_linkID;
    __int64 m_preLinkID;
    double  m_cost;

public:
    __int64 GetLinkID();
    void    SetLinkID(__int64 id);

    __int64 GetPreLinkID();
    void    SetPreLinkID(__int64 id);

    double  GetCost();
    void    SetCost(double cost);
};

