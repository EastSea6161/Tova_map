#pragma once
#include <map>
#include "KValueList.h"

#define  KIDList               KValueList<__int64>
#define  KLinkIDList           KValueList<__int64>

class KIDKeyIntegerValueList
{
public:
    KIDKeyIntegerValueList (void);
    KIDKeyIntegerValueList (KIDKeyIntegerValueList& r);
    ~KIDKeyIntegerValueList(void);

public:
    KIDKeyIntegerValueList& operator = (KIDKeyIntegerValueList& r);

private:    
    std::map<__int64, KIDList*> m_records;

public:
    KIDList* GetAt(__int64 key);
    KIDList* GetExistOrNew(__int64 key);

    void RemoveAll();
};
