#include "KDoubleRecordArray.h"

KDoubleRecordArray::KDoubleRecordArray(size_t _columnCount) : m_columnCount(_columnCount)
{
}


KDoubleRecordArray::~KDoubleRecordArray(void)
{
    RemoveRecords();
}

KDoubleRecordArray::KDoubleRecordArray(const KDoubleRecordArray& v)
{
    RemoveRecords();

    size_t nCount = v.GetRecordCount();

    for (size_t i=0; i<nCount; i++)
    {
        KDoubleRecord* srcRecord = v.GetRecordAt(i);
        KDoubleRecord* desRecord = new KDoubleRecord(v.m_columnCount);
        
        *desRecord = *srcRecord;
        /*size_t mCount = srcRecord->size();
        for (size_t n=0; n<mCount; n++)
        {
            desRecord->push_back(srcRecord->at(n));
        }*/
        m_records.Add(desRecord);
    }
}

KDoubleRecordArray& KDoubleRecordArray::operator=(const KDoubleRecordArray &v)
{
    if (&v == this)
    {
        return *this;
    }

    RemoveRecords();

    size_t nCount = v.GetRecordCount();

    for (size_t i=0; i<nCount; i++)
    {
        KDoubleRecord* pRecord = new KDoubleRecord(v.m_columnCount);
        *pRecord = *(v.GetRecordAt(i));
        m_records.Add(pRecord);
    }

    return *this;
}


KDoubleRecord* KDoubleRecordArray::AddNewRecord()
{
    KDoubleRecord* pRecord = new KDoubleRecord(m_columnCount);
    m_records.Add(pRecord);

    return pRecord;
}

size_t KDoubleRecordArray::GetRecordCount() const
{
    return m_records.GetCount();
}

KDoubleRecord* KDoubleRecordArray::GetRecordAt(size_t i) const
{
    return m_records.GetAt(i);
}

void KDoubleRecordArray::RemoveRecords()
{
    size_t nCount = m_records.GetCount();
    
    for (size_t i=0; i<nCount; i++)
    {
        KDoubleRecord* pRecord = m_records.GetAt(i);
        delete pRecord;
    }

    m_records.RemoveAll();
}

size_t KDoubleRecordArray::GetColumnCount() const
{
    return m_columnCount;
}
