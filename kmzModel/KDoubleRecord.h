#pragma once

#include "KModelDefine.h"

class KDoubleRecord
{
public:
    KDoubleRecord();
    KDoubleRecord(size_t _dataCount);
    KDoubleRecord(KDoubleRecord& _record);
    ~KDoubleRecord(void);

private:
    CAtlArray<Double> m_dataArray;
    size_t m_dataCount;
private:
    void Initial(size_t size);
public:
    void SetAt(size_t i, Double val);   
    void RemoveAll();
    const Double& GetAt(size_t i) const;
    Double& GetAt(size_t i);
    size_t GetCount() const;

    KDoubleRecord& operator=(const KDoubleRecord& _record);
};
