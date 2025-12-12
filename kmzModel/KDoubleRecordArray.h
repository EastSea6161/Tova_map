#pragma once
#include "KDoubleRecord.h"
#include "KModelDefine.h"

class KDoubleRecordArray
{
public:
    KDoubleRecordArray(size_t _columnCount);

    ~KDoubleRecordArray(void);
    KDoubleRecordArray(const KDoubleRecordArray& v);
    KDoubleRecordArray& operator=(const KDoubleRecordArray &v);

private:
    CAtlArray<KDoubleRecord*> m_records;
    size_t m_columnCount;
public:
    void RemoveRecords();
    KDoubleRecord* AddNewRecord();
    size_t GetRecordCount() const;
    KDoubleRecord* GetRecordAt(size_t i) const;
    size_t GetColumnCount() const;
};
