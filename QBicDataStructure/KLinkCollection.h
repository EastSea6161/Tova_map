#pragma once
#include <map>
#include "QBicDataStructureDefine.h"
#include "KLink.h"


class KLinkCollection
{
public:
    KLinkCollection (void);
	KLinkCollection(const KLinkCollection& p);
    ~KLinkCollection(void);

public:
	KLinkCollection& operator=(const KLinkCollection& p);

private:
    std::map<__int64, KLink*> m_records;

public:
    KLink* GetExistOrNew(__int64 linkID);
    KLink* GetLink(__int64 linkID);

public:
    void   RemoveAll();
	void   DeleteLink(__int64 linkID);
    size_t GetLinkCount() const;

    std::map<__int64, KLink*>::iterator begin();
    std::map<__int64, KLink*>::iterator end();

    CAtlString toString() const;
};

