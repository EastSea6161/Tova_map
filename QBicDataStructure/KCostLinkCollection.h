#pragma once
#include <map>
#include <vector>
#include "KCostLink.h"

class KCostLinkCollection
{
public:
    KCostLinkCollection (void);
    ~KCostLinkCollection(void);

private:
    std::map<__int64, KCostLink*> m_records;

public:
    KCostLink* InsertOrReplaceLink(__int64 linkID, __int64 preLinkID, double cost);
    KCostLink* GetLink            (__int64 linkID);
	void       GetLink(std::vector<__int64>& ar_vecLink);
public:
    void       RemoveAll   ();    
    size_t     GetLinkCount() const;
};

