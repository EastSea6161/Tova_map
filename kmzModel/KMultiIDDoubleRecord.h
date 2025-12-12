#pragma once

#include "KModelDefine.h"
#include "KMultiID.h"
#include "KDoubleRecord.h"

class KMultiIDDoubleRecord
{
public:
    KMultiIDDoubleRecord(size_t _columnCount);
    ~KMultiIDDoubleRecord(void);

private:
// #define CAtlODValuesMap CAtlMap  <KMultiID, KDoubleRecord*, KMutiIDTraits>
    CAtlODValuesMap m_records;
    size_t          m_columnCount;
public:
    KDoubleRecord* AddRecord(KMultiID key);
    KDoubleRecord* GetRecord(KMultiID key);
public:
    void RemoveAll();
    size_t GetColumnCount() const;
    size_t GetRecordCount() const;

    POSITION GetStartPosition() const;
    CAtlODValuesMap::CPair* GetNext(POSITION& pos);
    const CAtlODValuesMap::CPair* GetNext(POSITION& pos) const;
};
